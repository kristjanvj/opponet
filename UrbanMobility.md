Simple mobility generator for street scenarios

# Introduction #

The **UrbanMobility** (urbanmob) tool is a part of the MobiTrace toolbox for opponet and generates a [mobility trace](TraceFileFormat.md) in XML format.

It generates mobility patterns of nodes (pedestrians, vehicles etc.) in an urban area. It takes as input a map, routing probabilities and a set of generators. The map speciﬁes a grid of streets and intersections in the form of a graph of nodes and vertices, along with the node positions. Node generators can be attached to positions (entries) on the map where each generator is essentially an arrival process of nodes, with inter-arrival time and node speed given by some probability distributions. The routing probabilities specify node behavior at intersections, i.e. with which probability each street at the intersection is next selected by the node. Intersections can also have exits where nodes can leave the area (for derground transportation, area boundary etc.). The simulated street scenario is specified in a topology file as discussed in a following section.


# Parameters and usage #

Please refer to the general notes on the MobiTrace toolbox page. The script takes the following parameters:

```
  urbanmob [options]
    options:
    -h|--help:          Display help text.
    -n|--nodes:         Number of nodes to generate
    -a|--arrival-rate:  Arrival rate of nodes at each entry
    -t|--topology-file: Topology specification file
    -l|--log-level:     Log level for debug purposes. 0=none, 1=events, 2=xml
```

Example:
```
    urbanmob --nodes 10 --arrival-rate 0.2 --topology-file crossroads.topo
```

# Topology file #

The topology file is a text file the following form:

```
[Nodes]
{node number} {x coordinate} {y coordinate}

[Streets]
{street number} {node a} {node b}

[Entries]
{node number}

[Routing]
({current node},{previous node}) = ({next node},{probability}), ...
```

Destination nodes specified in routing cannot include previous or current node. Additionally, the routing probabilities for the destinations must add up to one.


## Example ##

This is an example of a simple topology file of a crossroads scenario. The scenario is 1000x1000 meters and contains five nodes. Four border nodes are connected to a single center node via four streets. Two entries are specified at border nodes 1 and 2.

```

#
# Crossroads.topo
#
# Topology file for the urbanmob mobility generator.
# A simple topology file for a street grid consisting of four streets in a crossroads configuration.
#

#
# This section defines the nodes in the scenario
#
[Nodes]
1 0 500
2 500 0
3 1000 500
4 500 1000
5 500 500

#
# This section configures legs between nodes, i.e. the streets
#
[Streets]
1 1 5
2 2 5
3 3 5
4 4 5

#
# This section defines which nodes are entries to the scenario.
# Nodes are generated with the arrival rate specified on the command line at the entries.
#
[Entries]
1
2

#
# This section defines the routing.
#
[Routing]
(1,None) = (5,1.0)
(2,None) = (5,1.0)
(3,None) = (5,1.0)
(4,None) = (5,1.0)
(5,1) = (2,0.25),(3,0.5),(4,0.25)
(5,2) = (1,0.25),(3,0.5),(4,0.25)
(5,3) = (1,0.5),(2,0.25),(4,0.25)
(5,4) = (1,0.25),(2,0.5),(3,0.25)      
(1,5) = (None,1.0)
(2,5) = (None,1.0)
(3,5) = (None,1.0)
(4,5) = (None,1.0)      
```