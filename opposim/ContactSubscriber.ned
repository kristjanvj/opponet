
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
// Contact subscriber module.
//
// The purpose of this module is to provide a very simple subscriber for contact
// establish and break events from the ContactNotifier module.
// The module subscribes to the HostContact event through the Blackboard.
// A simple trace is printed upon reception of a HostContact. A real implementation
// would of course put this information to a better use.
//
// @author  Kristjan V. Jonsson
// @author  Olafur R. Helgason
// @version 1.0 
//
simple ContactSubscriber
    parameters:
        debug: bool;
endsimple
