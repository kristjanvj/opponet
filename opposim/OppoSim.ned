
// ***************************************************************************
//
// OppoNet Project
//
// This file is a part of the opponet project, jointly managed by the 
// Laboratory for Communications Networks (LCN) at KTH in Stockholm, Sweden 
// and the Laboratory for Dependable Secure Systems (LDSS) at Reykjavik
// University, Iceland.
//
// ***************************************************************************
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// ***************************************************************************

//
// @titlepage
// <h1>opposim demonstration simulator</h1>
// 
// The opposim demonstration is part of the opponet project jointly managed by  
// Olafur R. Helgason at the Laboratory for Communications Networks (LCN) at KTH 
// in Stockholm, Sweden and Kristjan V. Jonsson at the Laboratory for Dependable 
// Secure Systems (LDSS) at Reykjavik University, Iceland.
// olafur.helgason@gmail.com and kristjanvj@gmail.com.
//
// This demonstration and all code created in this project is for academic purposes
// and is free for use and modification, subject to the GNU General Public License,
// version 3. See <a href="http://www.fsf.org">Free software foundation</a> home
// page for more details. 
//
// The purpose of the opponet project is to create components for support of 
// simulation of opportunistic wireless networks in the OMNeT++ discrete event
// simulator. 
// This simulation presents the following modules:
// - NodeFactory dynamically creates and destroys nodes during the simulation run
//   using commands read from a trace file. Both mobility and contact traces are supported.
//   Mobility traces specify create, destroy and waypoint commands of individual nodes
//   and thus control their movement in fine-grained steps. Contact traces can also contain
//   create and destroy commands, but their main utility is specification of contact
//   establish and break events with peer nodes. Such traces can eg be created in experiments
//   with actual systems by logging movement or contact information.
// - TraceMobility is a mobility module derived from the BasicMobility base class of the 
//   Mobility framework (MF) for OMNeT++. The module is initialized with waypoint commands
//   at the creation of a mobile node by the node factory. It handles the node mobility 
//   autonomously for the duration of the run.
// - ContactNotifier serves a similar purpose as the TraceMobility module. It is however
//   not a mobility module in the traditional sense since it has no notion of mobility.
//   The ContactNotifier is initialized with contact events when created by the node
//   factory. It then supplies its hosting module with contact notifications through
//   the Blackboard publish/subscribe mechanism for the duration of the run.
// - ContactSubscriber demonstrates a very simple subscriber for contact notifications.
//
// Further explanations are provided in the documentation for individual nodes.
// See also:
//    Olafur R. Helgason and Kristjan V. Jonsson, Opportunistic Networking in OMNeT++, 
//    First International Conference on Simulation Tools and Techniques for Communications, 
//    Networks and Systems (SIMUTOOLS), OMNeT++ Workshop, Marseille, France, March 7, 2008.
//
// Two trace files are provided with this simulation:
// - mobtrace1.xml demonstrates a very simple XML mobility script with three nodes. Two use
//   scripted mobility while the third uses random waypoint mobility. This demonstrates 
//   how mobility models can be combined in a mobility trace driven simulation.
// - contacttrace1.xml demonstrates a very simple contact script involving two nodes.
//
// <h2>Build</h2>
// Build according to the supplied information. The model was developed and tested on a
// linux platform but should work unmodified on a windows platform. Please let us know if
// this is not the case.
//
// <h2>Usage</h2>
// - ./opposim -f trace.ini   (for the mobility trace file)
// - ./opposim -f contact.ini (for the contact trace file)
//  

import
    "ChannelControl",
    "NodeFactory";

//
// This module defines the demo simulation for the opposim model. A simple
// scenario utilizing a single node factory module is created. A ChannelControl
// module is included as required by the MobilityFramework, but not utilized
// in this model. No mobile nodes are created. All mobile nodes utilized in the
// scenario are created dynamically by the node factory module as specified in
// the supplied trace files.
//
// @author Kristjan V. Jonsson
// @author Olafur R. Helgason
// @version 1.0
//
module OppoSim
    parameters:
        // parametes for the framework
        scenarioSizeX: numeric const,
        scenarioSizeY: numeric const;

    submodules:
        channelcontrol: ChannelControl;
            parameters:
                playgroundSizeX = scenarioSizeX,
                playgroundSizeY = scenarioSizeY;
            display: "p=127,56;i=old/earth1";
        factory: NodeFactory;
            parameters:
                scenarioSizeX = scenarioSizeX,
                scenarioSizeY = scenarioSizeY;
            display: "p=46,56;i=block/cogwheel";
    display: "b=$scenarioSizeX,$scenarioSizeY";
endmodule


//
// The network definition
//
// @author Kristjan V. Jonsson
// @author Olafur R. Helgason
//
network square : OppoSim
    parameters:
        scenarioSizeX = input(1000,"scenarioSizeX"),
        scenarioSizeY = input(1000,"scenarioSizeY");
endnetwork



