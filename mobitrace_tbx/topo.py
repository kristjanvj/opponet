#! /usr/bin/python

from math import sqrt
from random import *
#from rpy import r
#import numpy

class Position:
  def __init__(self,x,y):
    self.x = x
    self.y = y

  def __str__(self):
    return '(' + str(self.x) +',' + str(self.y) + ')'

  def distance(p1, p2):
    if(p1.x == p2.x):
      return float(abs(p2.y-p1.y))
    if(p1.y == p2.y):
      return float(abs(p2.x-p1.x))
    return sqrt( (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) )

  def direction(p1, p2):
    if(p1 == p2):
      return (0,0)
    dist = Position.distance(p1, p2)
    xdir = (p1.x - p2.x)/dist
    ydir = (p1.y - p2.y)/dist
    return (xdir, ydir)

# A base class for random generators
class RandomGenerator:
  def generate(self):
    raise NotImplementedError('generate() is virtual and must be overridden')

# Not really a random generator
class DeterministicGenerator(RandomGenerator):
  def __init__(self, mean):
    self.__mean = mean

  def generate(self):
    return self.__mean

class ExponentialGenerator(RandomGenerator):
  def __init__(self, lambd):
    self.__lambd = lambd

  def generate(self):
    return expovariate(self.__lambd)

class LognormalGenerator(RandomGenerator):
  def __init__(self, mu, sigma):
    self.__mu = mu
    self.__sigma = sigma
  
  def generate(self):
    return lognormvariate(self.__mu, self.__sigma)

class ParetoGenerator(RandomGenerator):
  def __init__(self, shape):
    self.__shape = shape

  def generate(self):
    return paretovariate(self.__shape)

class DiscreteUniformGenerator(RandomGenerator):
  def __init__(self, min, max):
    self.__min = min
    self.__max = max

  def generate(self):
    return randint(self.__min, self.__max)

#
# ExponentialBatchGenerator generates batches separated by exponentially
#  distributed interarrival times.
# generate() always returns interarrival time until next arrival where each
#  arrival in batch is counted seperately.  I.e. it returns interarrival time 
#  upon first call.  Successive calls will return zero until 'batch has been 
#  emptied'.  This way the generate() function can be used to when summing 
#  interarrival time with base time.
#  example:
#    basetime = basetime + ebg.generate()
#
class ExponentialBatchGenerator(RandomGenerator):
  def __init__(self, lambd, batch_size_generator):
    self.__lambd = lambd
    self.__bszg = batch_size_generator
    self.__outstanding = 0

  def generate(self):
    if self.__outstanding == 0:
      self.__outstanding = self.__bszg.generate() - 1
      return expovariate(self.__lambd)
    else:
      self.__outstanding -= 1
      return float(0)

# Markov Modulated Poisson Process
class MMPPGenerator(RandomGenerator):
  def __init__(self, Q, lambdas):
    self.__Q = Q
    self.__lambdas = lambdas
    # TODO: Verify Q matrix
    if Q.shape[0] != Q.shape[1]:
      raise 'MMPPGenerator: Q matrix dimension mismatch'
    if Q.shape[0] != len(lambdas):
      raise 'MMPPGenerator: Num arrival rates does not match Q size'
    self.__state = randint(0,Q.shape[0]-1)
    self.__timeToTransition = expovariate(-Q[self.__state, self.__state])
    self.__timeToArrival = self.__iarrivalTime = expovariate(lambdas[self.__state])
    self.__timeSinceLastArrival = 0.0

  def generate(self):
    while True:
      if self.__timeToArrival < self.__timeToTransition:
        self.__timeToTransition -= self.__timeToArrival
        retval = self.__iarrivalTime
        self.__timeToArrival = self.__iarrivalTime = expovariate(self.__lambdas[self.__state])
        return retval
      else:
        self.__timeToArrival = self.__timeToArrival - self.__timeToTransition
        self.transition()
        self.__timeToTransition = expovariate(-self.__Q[self.__state, self.__state])

  def transition(self):
    begin = 0.0
    state_intervals = {}
    for tostate in range(0, self.__Q.shape[0]):
      if tostate == self.__state:
        continue
      prob_to_state = self.__Q[self.__state,tostate]/(-self.__Q[self.__state,self.__state])
      state_intervals[tostate] = (begin,begin+prob_to_state)
      begin = begin+prob_to_state
    random_value = random()
    for state, interval in state_intervals.iteritems():
      if interval[0] <= random_value and random_value < interval[1]:
        self.__state = state
        break

# Generates event from multiple generators
# Constructor takes a list of generators
# generate function generates num_events in total from the generators
#  return value from generate() is a list of tuples on the form 
#    (event_time, index_of_generator)
#  where it is assumed that time starts at 0 (and thus event_time of
#  the first generated event is the first interarrival time).
class MultiEventGeneration:
  def __init__(self, generators):
    self.__generators = generators

  def generate(self, num_events):
    time = 0.0
    event_times = [] 
    for gen in self.__generators:
      event_times.append(gen.generate())
    ret = []
    for ev in range(0, num_events):
      time = min(event_times)
      next_gen_idx = event_times.index(time)
      ret.append((event_times[next_gen_idx], next_gen_idx))
      iarrival_time = self.__generators[next_gen_idx].generate()
      event_times[next_gen_idx] += iarrival_time
    return ret

# Deprecated.  Use MultiEventGeneration
#  Still used in code for stockholm topologies
class MultiGenerator:
  def __init__(self, distributions):
    self.__distributions = distributions

  def generate(self, num_events):
    time = 0.0
    event_times = [0.0] * len(self.__distributions)
    for i in range(0, len(self.__distributions)):
      func = self.__distributions[i][0]
      args = self.__distributions[i][1]
      event_times[i] = func(*args)
    ret = []
    for ev in range(0,num_events):
      time = min(event_times)
      next_gen_idx = event_times.index(time)
      ret.append((event_times[next_gen_idx], next_gen_idx))
      func = self.__distributions[next_gen_idx][0]
      args = self.__distributions[next_gen_idx][1]
      iarrival_time = func(*args)
      event_times[next_gen_idx] += iarrival_time
    return ret

# This is just here for backwards compatibility
# Should be removed since it is now replaced by MultiGenerator
class NodeGeneration:
  def __init__(self, num_generators, rates):
    self.__num_generators = num_generators
    if(num_generators != len(rates)):
      raise 'NodeGenerator: Number of generators not equal to number of rates'
    self.__rates = rates

  def generate(self, num_events):
    time = 0.0
    num_generated = 0
    event_times = [0.0] * self.__num_generators
    ret = []
    for i in range(self.__num_generators):
      event_times[i] = expovariate(self.__rates[i])
    while(num_generated < num_events):
      time = min(event_times)
      next_gen_idx = event_times.index(time)
      ret.append((event_times[next_gen_idx], next_gen_idx))
      iarrival_time = expovariate(self.__rates[next_gen_idx])
      event_times[next_gen_idx] += iarrival_time
      num_generated += 1
    return ret


class Node:
  def __init__(self, n_id, pos):
    self.__n_id = n_id
    self.__pos = pos
    self.__neigbour_list = []

  def __str__(self):
    return str(self.__n_id)
  
  def __cmp__(self, other):
    return cmp(self.__n_id, other.__n_id) 

  def id(self):
    return self.__n_id

  def pos(self):
    return self.__pos

  def add_neighbour(self, node):
    if(self.__neigbour_list.count(node) == 0):
      self.__neigbour_list.append(node)

  def neighbours(self):
    return self.__neigbour_list

class Street:
  def __init__(self, s_id, node1, node2):
    neigh1 = node1.neighbours()
    if(neigh1.count(node2) != 0):
      raise str(node2) + ' is already neighbour of ' + str(node1)
    neigh2 = node2.neighbours()
    if(neigh2.count(node1) != 0):
      raise str(node1) + ' is already neighbour of ' + str(node2)
    self.__s_id = s_id
    self.__nodes = node1, node2
    self.__length = Position.distance(node1.pos(), node2.pos())

  def __str__(self):
    return str(self.__s_id)

  def __eq__(self, other):
    return self.__s_id == other.__s_id

  def length(self):
    return self.__length

  def get_nodes(self):
    return self.__nodes


class Topology:
  def __init__(self):
    self.__nodelist = []
    self.__streetlist = []

  def add_street(self, street):
    # TODO: Verifiy that there is only one street from node1 to node
    if(self.__streetlist.count(street) != 0):
      raise 'add_street error'
    else:
      self.__streetlist.append(street)
    nodes = street.get_nodes()
    nodes[0].add_neighbour(nodes[1])
    nodes[1].add_neighbour(nodes[0])
    if(self.__nodelist.count(nodes[0]) == 0):
      self.__nodelist.append(nodes[0])
    if(self.__nodelist.count(nodes[1]) == 0):
      self.__nodelist.append(nodes[1])

  def nodes(self):
    return self.__nodelist

  def streets(self):
    return self.__streetlist
  
  def debug(self):
    print "Neigbors:"
    for node in self.__nodelist:
      nlist = node.neighbours()
      outstr = "node " + str(node) + ': '
      for neigh in nlist:
        outstr += str(neigh) + ' '
      print outstr
    print "Street length:"
    for street in self.__streetlist:
      print "street " + str(street) + ' = ' + str(street.length())

def random_path(rtable, entry):
  path = []
  now = entry
  last = None
  while(now != None):
    path.append(now)
    temp = now
    now = random_neighbour(now, last, rtable)
    last = temp
#    print last,now
  return path

def random_neighbour(now, last, rtable):
  random_intervals = []
  nlist = rtable[(now,last)]
  intervalbegin = 0
  for ent in nlist:
    neigh = ent[0]
    prob = ent[1]
    intervalend = intervalbegin + prob
    random_intervals.append((intervalbegin, intervalend))
    intervalbegin = intervalend
  coin = random() 
  ival = filter(lambda x: x[0] <= coin and coin < x[1], random_intervals)
  if(len(ival) != 1):
    raise 'Random interval error'
  neigh_idx = random_intervals.index(ival[0])
  return nlist[neigh_idx][0]

def verify_rtable(rtable,topo):
  for node_pair, problist in rtable.iteritems():
    this_node = node_pair[0]
    last_node = node_pair[1]
    # last_node must be neighbour on topology
    nl = topo.nodes()
    neigh_list = filter(lambda n: n.id() == this_node, nl)[0].neighbours()
    if(last_node != None and len(filter(lambda n: n.id() == last_node, neigh_list)) != 1):
      raise 'verify_rtable: last_node not neighbour'
    # all neighbours in routing table must be in topology
    # all neighbours in topology must be in routing table
    prob = 0
    topo_neigh_list_id = map(lambda n: n.id(),neigh_list)
    topo_neigh_list_id.sort()
    rt_neigh_list_id = []
    if(last_node != None):
      rt_neigh_list_id.append(last_node)
    for e in problist:
      neigh = e[0]
      if(neigh != None):
        rt_neigh_list_id.append(neigh)
      prob += e[1]
      if(neigh != None and len(filter(lambda n: n.id() == neigh, neigh_list)) != 1):
        print node_pair, problist
        raise 'verify_rtable: rtable neighbour not in topology'
    # Routing probabilities must sum to 1
    if(prob != 1.0):
      print node_pair, problist
      raise 'verify_rtable: proabilities do not sum to 1.0'
    rt_neigh_list_id.sort()
    if(topo_neigh_list_id != rt_neigh_list_id):
      print node_pair, problist
      raise 'verify_rtable: topology and routing table neighbour mismatch'

def hyperexponential(prob_to_rates):
  probabilities = prob_to_rates.keys()
  sum = reduce(lambda x,y: x+y, probabilities)
  if(sum != 1.0):
    raise 'hyperexponential: Probabilities do not sum to 1'
  l = prob_to_rates.items()
  range_to_rates = {}
  last_upper = 0.0
  for prob, rate in prob_to_rates.iteritems():
    lower = last_upper
    upper = last_upper + prob
    range_to_rates[(lower, upper)] = rate
    last_upper = upper
    
  coin = random()
  for prange, rate in range_to_rates.iteritems():
    if prange[0] <= coin and coin < prange[1]:
      #print coin, prange
      return expovariate(rate)

"""
def confidence_intervals(data, percentage):
  alpha = 1 - percentage
  mean = r.mean(data)
  deviation = r.sqrt(r.var(data))
  n = len(data)
  error = r.qt(alpha/2, df=n-1)*deviation/r.sqrt(n)
  lower = mean + error
  upper = mean - error
  return (lower, mean, upper)
"""
