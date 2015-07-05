Contact notifier for trace based mobility simulations in OMNeT++ simulations.

# Introduction #

The **ContactNotifier** manages the contact events of a node when performing contact-driven simulations. The events are speciﬁed in a [trace file](TraceFileFormat.md). It is used in the SimpleContactNode in the [Opposim](Opposim.md) demo.

There is no actual mobility of nodes at the simulation level when employing the contact-driven approach. The contact trace simply lists the contact events, which may
be a consequence of node mobility. Therefore, nodes in a contact-driven simulation do not have a mobility module, but instead employ the **ContactNotifier** to publish contact events to subscribing modules of the the node through the Blackboard. Although not a mobility module as such, the **ContactNotifier** serves a comparable purpose of notifying submodules of status change, and is indeed derived from the BasicMobility class of the MF. **ContactNotifier** can thus be considered to be in the navigator role when using the contact driven approach to opportunistic networking simulation. A Blackboard item of type HostContact is employed to notify subscribers of contact events.