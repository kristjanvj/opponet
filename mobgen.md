Utility to convert simple text files into XML mobility traces

# Introduction #

[mobgen](mobgen.md) is a part of the MobiTrace toolbox of the OppoNet project. mobgen generates a [XML mobility trace file](TraceFileFormat.md) from a simple text based description.

# Usage #

Please refer to the general notes on the MobiTrace toolbox page. The script takes the following parameters:
```
  mobgen {options} {input file} {output file}
    options:
    -h|--help:    Display help text.
    -f|--force:   Force overwriting of an existing output file.
    -r|--random:  Simple normally distributed variation of the velocity and
                  speed. The standard deviation is fixed at 1/2 the value
                  (speed or pause).
```
Example:
```
  mobgen -f textfile outfile.xml
```

# Input file format #

A simple flat text format is used for specification of mobility in this approach. It has been found to be somewhat easier than [XML traces](TraceFileFormat.md) to generate by hand for small simulations and test cases.

One event is speciﬁed per line in the input ﬁle, whose format is as follows:
```
{command} {time} {node} (...)
```
**command** is create, destroy or waypoint. **time** is in seconds from the beginning of the scenario. **node** is an integer uniquely identifying the node.

```
create {time} {node} {x y} (type)
```
**create** speciﬁes creation of a mobile node at a speciﬁed time and location. Type is an optional parameter, specifying the type of node to be created. This string must correspond to an BasicMobility-derived module in the simulation.

```
destroy {time} {node}
```
**destroy** speciﬁes the destruction of a node. A negative or zero time means that the node will be destroyed node after the last leg of its journey is travelled and its ﬁnal pause is done. A destroy event with a speciﬁed time will destroy the node at that exact time, regardless of any remaining waypoint events.

```
waypoint {time} {node} {x y} {velocity} (pause)
```
**waypoint** speciﬁes the (x,y) coordinates of next waypoint, its velocity and, optionally, a pause time at the destination. Normally distributed variations of velocity and pause
times are supported. An activation time less than zero means that the time of travel is deduced from the distance to the next waypoint and the speciﬁed velocity. If however
the next consecutive event speciﬁes a time, the velocity is deduced from the distance and travel time.

## Example ##

```
# Test trace file

# Node 1
create  0.0 1  100 100 SimpleNode
waypoint -1 1 200 200 2.0 10.0
waypoint -1 1 300 300 3.0 10.0
waypoint -1 1 400 400 4.0 10.0
waypoint -1 1 500 500 5.0 10.0
destroy   -1 1

# Node 2
create    0.0 2 500 500 SimpleNode
waypoint 10.0 2 600 600 
waypoint 20.0 2 700 700
waypoint 40.0 2 800 800
destroy 100.0 2
```