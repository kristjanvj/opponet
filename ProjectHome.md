Opponet is an open source effort to create components and tools for support of opportunistic wireless networking in the OMNeT++ discrete event simulator. We provide a NodeFactory component which uses traces to dynamically instantiate mobile nodes during a simulation run. Two distinct models are supported: Mobility and contact trace based. Mobility traces consist of create, destroy and waypoint commands. Contact traces consist of create, destroy, contact establish and contact break events. OMNeT++ simple modules are embedded in the mobile nodes. These are TraceMobility and ContactNotifier which handle mobility and contact events locally for each mobile node during its lifetime. Both TraceMobility and ContactNotifier inherit from the [BasicMobility](MobilityFramework.md) base class of the [Mobility Framework (MF)](MobilityFramework.md) and can be used in existing simulations which utilize the MF.

**NOTE: opponet and opposim were written for the OMNeT++ 3.4 framework. A very rough port to version 4.1 is available as a tarball under downloads. See the [Installation instructions](install41instructions.md) for details.**

**UPDATE: An updated version of the code is now integrated into a MiXiM branch. See https://github.com/olafur/mixim.**

The project is described in
> Helgason, O. R. & Jónsson, K. V., Opportunistic Networking in OMNeT++. First International Conference on Simulation Tools and Techniques for Communications, Networks and Systems (SIMUTOOLS 2008), OMNeT++ Workshop, 2008.
The paper is available on the downloads page.


---


# Usage #

The most convenient way to start experimenting with the trace driven approach to mobility and contact support is to build and run the [Opposim](Opposim.md) simulator. This is a very simple simulator which demonstrates our node factory and associated component.

We also provide a number of tools for generation of traces and conversion of existing traces into our format in the [MobiTrace](MobiTrace.md) toolbox.

Opponet is an academic project and based solely on our interest and past research projects. The code is released under a GPL license and is free for all to use within its terms. We encourage you all to try the code and are interested in receiving feedback, either by email or though the issue tracking system (requires a google account). We are also very interested in recruiting some collaborators for the project.

The newest releases of the code are available from downloads or via anonymous subversion access:
```
svn checkout http://opponet.googlecode.com/svn/trunk/ opponet-read-only
```