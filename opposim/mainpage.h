/**
 @mainpage opposim demonstration simulator
 
 @author Olafur R. Helgason (olafur.helgason@gmail.com)
 @author Kristjan V. Jonsson (kristjanvj@gmail.com)
 @version 1.0 
 
 The opposim demonstration is part of the opponet project jointly managed by  
 Olafur R. Helgason at the Laboratory for Communications Networks (LCN) at KTH 
 in Stockholm, Sweden and Kristjan V. Jonsson at the Laboratory for Dependable 
 Secure Systems (LDSS) at Reykjavik University, Iceland.

 This demonstration and all code created in this project is for academic purposes
 and is free for use and modification, subject to the GNU General Public License,
 version 3. See <a href="http://www.fsf.org">Free software foundation</a> home
 page for more details. 

 The purpose of the opponet project is to create components for support of 
 simulation of opportunistic wireless networks in the <a href="http://www.omnetpp.org">OMNeT++</a> 
 discrete event simulator. 
 This simulation presents the following modules:
 - NodeFactory dynamically creates and destroys nodes during the simulation run
   using commands read from a trace file. Both mobility and contact traces are supported.
   Mobility traces specify create, destroy and waypoint commands of individual nodes
   and thus control their movement in fine-grained steps. Contact traces can also contain
   create and destroy commands, but their main utility is specification of contact
   establish and break events with peer nodes. Such traces can eg be created in experiments
   with actual systems by logging movement or contact information.
 - TraceMobility is a mobility module derived from the BasicMobility base class of the 
   <a href="http://mobility-fw.sourceforge.net">Mobility framework (MF)</a> for OMNeT++. 
   The module is initialized with waypoint commands
   at the creation of a mobile node by the node factory. It handles the node mobility 
   autonomously for the duration of the run.
 - ContactNotifier serves a similar purpose as the TraceMobility module. It is however
   not a mobility module in the traditional sense since it has no notion of mobility.
   The ContactNotifier is initialized with contact events when created by the node
   factory. It then supplies its hosting module with contact notifications through
   the Blackboard publish/subscribe mechanism for the duration of the run.
 - ContactSubscriber demonstrates a very simple subscriber for contact notifications.

 Further explanations are provided in the documentation for individual nodes.
 See also:
    Olafur R. Helgason and Kristjan V. Jonsson, Opportunistic Networking in OMNeT++, 
    First International Conference on Simulation Tools and Techniques for Communications, 
    Networks and Systems (SIMUTOOLS), OMNeT++ Workshop, Marseille, France, March 7, 2008.

 Two trace files are provided with this simulation:
 - mobtrace1.xml demonstrates a very simple XML mobility script with three nodes. Two use
   scripted mobility while the third uses random waypoint mobility. This demonstrates 
   how mobility models can be combined in a mobility trace driven simulation.
 - contacttrace1.xml demonstrates a very simple contact script involving two nodes.

 @section Build
 Build according to the supplied information. The model was developed and tested on a
 linux platform but should work unmodified on a windows platform. Please let us know if
 this is not the case.

 @section Usage
 - ./opposim -f trace.ini   (for the mobility trace file)
 - ./opposim -f contact.ini (for the contact trace file) 
*/  
