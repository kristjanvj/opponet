Factory module for dynamic instantiation of mobile nodes in OMNeT++ simulations of opportunistic networks.

# Introduction #

The node factory module dynamically instantiates mobile nodes in OMNeT++ simulations in accordance to descriptions given in [trace files](TraceFileFormat.md).

  * A mobility trace defines create, destroy and waypoint events for a collection of nodes. Such a trace is created using an external mobility generator, e.g. UrbanMobility from the MobiTrace toolkit, or UDel (http://udelmodels.eecis.udel.edu).

  * Contact traces can additionally be used. Such traces can e.g. be created from contact measurements conducted with mobile devices.