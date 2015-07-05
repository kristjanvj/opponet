Simple mobile node OMNeT++ compound module

# Introduction #

SimpleNode is used in the [Opposim](Opposim.md) simulation demo to represent a mobile node. It is instantiated dynamically during a simulation by the NodeFactory module in accordance to events specified in a XML [trace file](TraceFileFormat.md).

The SimpleNode module contains a Blackboard module for communicating mobility updates to other modules within the node. See the [Mobility Framework](MobilityFramework.md) documentation for details regarding to subscribing to updates through the Blackboard.

The node also contains a generic navigator object of the type BasicMobility, which can be assigned the class name of any BasicMobility-derived mobility module at startup. The type of mobility model can thus be determined at run-time. The [Opposim](Opposim.md) demonstration uses both a TraceMobility and a RandomWaypointMobility modules in the navigator role.