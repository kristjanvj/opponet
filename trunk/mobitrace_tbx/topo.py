
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

import string
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
      raise Exception("Street already exists")
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
    print "\nTopology debug listing:";
    print "\nNeigbors:"
    for node in self.__nodelist:
      nlist = node.neighbours()
      outstr = "node " + str(node) + ': '
      for neigh in nlist:
        outstr += str(neigh) + ' '
      print outstr
    print "\nStreet lengths:"
    for street in self.__streetlist:
      print "street " + str(street) + ' = ' + str(street.length())
    print;  
      
      
class TopoParser:
  """
  Topology file parser class.
  """
       
  
  def parse(self,filename,log_level=0):
    """
    Reads a topology specification file. 
    
    Returns Topology object, routing map and entry list
    
    The format of the topology file is as follows:
    
      [Nodes]
      {node number} {x coordinate} {y coordinate}

      [Streets]
      {street number} {node a} {node b}

      [Entries]
      {node number}

      [Routing]
      ({current node},{previous node}) = ({next node},{probability}), ...  
    """
        
    if log_level>0: print "Parsing topo file %s" % filename;    
        
    try:
      tf = open(filename,"r");
      
      t = Topology()
      n = {}
      e  = []
      rt = {}
      section = "";    
      linecount = 0;
      
      for line in tf:
        # Slice away comments
        linecount+=1
        line = line.strip("\n");
        p=line.find("#");
        if p != -1: 
          line = line[0:p];

        # Strip away whitespace
        line = line.strip();
        if line == "": continue;

        # Convert to lower case - the specification is not case sensitive
        line = line.lower();

        if line.find("[") != -1: 
          # This is a section heading
          section = line.strip("[]");
          if log_level>0: print "\nSection:", section;
        else:      
          try:
            if section == "nodes":
              # Create a list of nodes. Used for creation of streets
              self.__addTopoNode(n,line,log_level);
            elif section == "streets":
              # Create a street. The node must exist
              self.__addTopoStreet(t,n,line,log_level);
            elif section == "entries":
              # Assign an entry to a specified node. The node must exist.
              self.__addTopoEntry(n,e,line,log_level);
            elif section == "routing":
              # Add an routing entry
              self.__addTopoRouting(n,rt,line,log_level);
            else:
              raise("Unknown section label");
          except Exception, ex:
            raise Exception( "EXCEPTION: '%s' in file '%s' -- line %d\nSource line: '%s'" % \
                             (ex.message,filename,linecount,line));                 
      tf.close();
         
      return t,rt,e;

    except IOError, error:
      print error;
      raise Exception("Topology undefined");


  def __addTopoNode(self,n,line,log_level=0):
    """
    Add a new topology node to the collection
    """
    elements = line.split(); 
    
    nodeId = self.__parseIntStr(elements[0]);
    if nodeId == None:
      raise Exception("Node specification invalid");
      
    if n.has_key(nodeId):
      raise Exception("Node %d already exists" % nodeId);  
      
    x = self.__parseIntStr(elements[1]);
    y = self.__parseIntStr(elements[2]);
    if x == None or y == None:
      raise Exception("Invalid node location");
      
    n[nodeId] = Node(nodeId,Position(x,y));
    if log_level>0: print "Adding node ", nodeId, " at location",x,y;
    

  def __addTopoStreet(self,t,n,line,log_level=0):
    """
    add a new street to the topology
    """
    elements = line.split(); 
    
    streetId = self.__parseIntStr(elements[0]);
    
    nodeA = self.__parseIntStr(elements[1]);
    nodeB = self.__parseIntStr(elements[2]);  
    if nodeA == None or nodeB == None:
      raise Exception("Invalid node id for street %d" % streetId);
      
    if not n.has_key(nodeA) or not n.has_key(nodeB):
      raise Exception("Undefined nodes used for street %d" % streetId);
      
    t.add_street(Street(streetId,n[nodeA],n[nodeB]))          
    if log_level>0: print "Adding street %d -- a=%d, b=%d" % (streetId, nodeA, nodeB);


  def __addTopoEntry(self,n,e,line,log_level=0):
    """
    Specify a node as an entry to the topology.
    """
    line = line.strip(); 
    
    nodeId = self.__parseIntStr(line);
    if nodeId == None:
      raise Exception("Node specification invalid");
    if not n.has_key(nodeId):
      raise Exception("Node specification invalid");  
    e.append(nodeId)          ;
    if log_level>0: print "Adding entrypoint at %d" % nodeId;
    
    
  def __addTopoRouting(self,n,rt,line,log_level=0):
    """
    Add a routing entry to the topology
    """
    #
    # Split the line on the =
    #
    parts = line.split("=")
    if len(parts) != 2:
      raise Exception("Invalid syntax for routing entry %s" % line);
    
    #
    # Process the left side
    #
    
    nstr = parts[0].strip();
    nstr = nstr[1:len(nstr)-1]
    nodes = nstr.split(",");
    if len(nodes) != 2:
      raise Exception("Invalid syntax for routing entry %s" % line);
    
    n1 = self.__parseIntStr(nodes[0]);
    n2 = self.__parseIntStr(nodes[1]);
    if ( n1 != None and not n.has_key(n1) ) or ( n2 != None and not n.has_key(n2) ):
      raise Exception("Invalid node id for routing entry %s" % line);
    if rt.has_key((n1,n2)):
      raise Exception("Key (%s,%s) exists in routing table" % (n1,n2));
    
    #
    # Process the right side
    #
    
    # Split the list of probability pairs (node,prob)
    probs = parts[1].split("),(");
    plist=[];
    for prob in probs:
      # tidy up the pairs
      p = prob.strip();
      p = p.strip("(");
      p = p.strip(")");
      ps = p.split(","); 
      if len(ps) != 2:
        raise Exception("Invalid syntax for routing entry %s" % line);      
      ndest = self.__parseIntStr(ps[0]);           
      if ndest != None and not n.has_key(ndest):
        raise Exception("Invalid node id for routing entry %s" % line);
      pr = self.__parseFloatStr(ps[1]);
      if pr == None or pr < 0 or pr > 1:
        raise Exception("Invalid routing probability for routing entry %s" % line);
      # Append the parsed node,prob pair to the list
      plist.append((ndest,pr));
    # Add a routing entry
    rt[(n1,n2)]=plist;  
    if log_level>0: print "Adding routing entry (%s,%s)=%s" % (n1,n2,plist);


  def __parseIntStr(self,s):
    """
    Parses a string to integer. Returns None if the string is not a valid integer.
    """
    try:
      return string.atoi(s);
    except ValueError:
      return None;

      
  def __parseFloatStr(self,s):
    """
    Parses a string to float. Returns None if the string is not a valid float.
    """
    try:
      return string.atof(s);
    except ValueError:
      return None;    
      

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


