
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

import
    "Blackboard",
    "BasicMobility",
    "ContactSubscriber";

//
// The simple contact node model demonstrates a mobile node which manages peer
// contacts using contact traces. The node is instantiated dynamically by the
// NodeFactory object during a simulation run. This node is similar to the
// SimpleNode module, except a simple demonstration subscriber module for
// contact notifications is added.
//
// @author Kristjan V. Jonsson
// @version 1.0
//
module SimpleContactNode
    parameters:
        x: numeric,
        y: numeric,
        z: numeric,
        mobilityModel: string;
    submodules:
        blackboard: Blackboard;
            display: "p=261,88;i=block/blackboard";
        csubscribe: ContactSubscriber;
            parameters:
                debug=false;
            display: "p=88,88;i=block/control";
        navigator: mobilityModel like BasicMobility;
            parameters:
                x = x,
                y = y;
            display: "p=175,88;i=block/control";
    display: "b=316,200";
endmodule

