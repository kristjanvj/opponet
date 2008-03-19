
"""
topo.py 1.0

Provides topography classes for use with the urbanmob mobility generator.

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


