Simple OMNeT++ compound module for contact driven simulations

# Introduction #

SimpleContactNode is used in the [Opposim](Opposim.md) simulation demo to represent a mobile node. It is instantiated dynamically during a simulation by the NodeFactory module in accordance to events specified in a XML [trace file](TraceFileFormat.md). It is very similar to a SimpleNode compount module, but additionally includes a ContactSubscriber module to demonstrate how clients can subscribe to contact information from the ContactNotifier.