
"""
topo.py 1.0

Provides node generation support for the urbanmob mobility generator.

Authors: Olafur R. Helgason (olafur.helgason@gmail.com and
         Kristjan V. Jonsson (kristjanvj@gmail.com)
         (c) Olafur R. Helgason (2007)
        
Part of the MobiTrace toolbox of the OppoNet project. OppoNet is a open-source
effort to create software and modules for support of modeling of opportunistic
wireless systems in OMNeT++. Project hompage: http://www.ee.kth.se/lcn/opponet.
"""

# =============================================================================
#
# LICENSING
#
# =============================================================================
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version 3
# as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# =============================================================================

__AUTHOR__  = "Olafur R. Helgason"
__AUTHOR__  = "Kristjan V. Jonsson"
__VERSION__ = 1.0

from math import sqrt
from random import *


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
    
    
    
