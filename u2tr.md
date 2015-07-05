Utility to convert UDel formatted mobility files into XML mobility traces

# Introduction #

[u2tr](u2tr.md) is a part of the MobiTrace toolbox of the OppoNet project. It generates a [XML mobility trace file](TraceFileFormat.md) from a UDel generated mobility file.

# UDel #

UDel is a sophisticated mobility simulator developed at the University of Delaware (http://udelmodels.eecis.udel.edu). The tool uses sophisticated models of human behaviour and mobility patterns to generate mobility files in a text file format.

## Trace file format ##

(Note: The text in this section is copied from the UDel website)

```
NodeID Time (XCoordinate,YCoordinate,ZCoordinate) NotUsed NotUsed HostType # NodeType FloorNum TaskNum 
```

HostType is a text string and may be MOBILE-HOST, which includes people and cars, BASE-STATION, FIXED-RELAY, SEMI-PERMANENT. These last three node types are specified by the map file. SEMI-PERMANENT nodes are for making three level hierarchies of infrastructures.

NodeType
```
PERSON = 0
CAR = 1    1
UAV=2  //UAV (Unmanned Aerial Vehicles) not currently supported
BASESTATION = 3
FIXEDRELAT = 4
SEMIPERMANEN = 5
```

FloorNum
```
Person inside, the floor num is the floor that the person is on
Person outside simulated area or about to exit simulated area = 100000
Car outside simulated area or about to exit simulated area = 100001
Person walking outside = -999
Person driving in car = -997
Person in parking lot = -995
Person in subway station = -994
Car inside simulated area = -996
Outdoor infrastructure = -1000
Indoor infrastructure = the floor where the base station is located
```

TaskNum. The mobility model performs a sequence of tasks. The task number specifies which task is being performed. The task number is used extensively by the Mobility Parser Tool. If the parser is not used, then the task number can be set to zero.

### Notes ###

A line is generates for each node each second of the simulation. QualNet interpolates the position between seconds.

The mobility parser generate a list of trips for each node.



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

# Implementation #

We omit all datapoints with FloorNum==100000 or 100001 as these nodes are outside or about to depart the scenario.

A **create** command is entered when a node is first seen in the UDel trace file. A **waypoint** command is added for each datapoint inside the simulated area.