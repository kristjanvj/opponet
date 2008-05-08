
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
// Base trace event message
//
// Trace event messages are utilized by the node factory object to signal
// the various scripted events. This is the base message. See the derived
// messages for further details.
//
message TraceEvent
{
  fields:
    double time;
    int nodeID;  
}

//
// Create event signals a node creation. Used by NodeFactory.
//
message CreateEvent extends TraceEvent
{
  fields:
    double x;
    double y;
    string type;
    string name;
    string prefix;
    string iconPath;
    string mobilityModel;
}

//
// Destroy event signals a node destruction. Used by NodeFactory.
//
message DestroyEvent extends TraceEvent
{
}

//
// Contact event signals a node contact when using the contact
// driven modeling approach.
//
message ContactEvent extends TraceEvent
{
  fields:
    int type;
    int id;
    int peerId;
}

