
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
// Contact notifier module.
//
// Used in conjunction with the NodeFactory object to implement the
// contact trace model.
//
// Contact trace based mobility uses externally generated traces or measurement 
// results to control a nodes movement. Contact traces are XML scripts of
// contact establish and break events. See the NodeFactory module for more information.
// The ContactNotifier module is initialized by the factory upon creation. It then supplies 
// hosting module with contact establish and break events for the duration of the run.
// The events are made available as notifications through the Blackboard using a HostContact
// structure. See the simple ContactSubscriber implementation as an example.
//
// Note that ContactNotifier inherits from BasicMobility. It can thus be substituted for
// BasicMobility-derived mobility modules in host node modules although it is not
// strictly a mobility module.
//
// @author  Kristjan V. Jonsson
// @author  Olafur R. Helgason
// @version 1.0 
//
simple ContactNotifier
    parameters:    
        debug: bool,              // debug switch
        x: numeric,               // initial x location
        y: numeric;               // initial y location
endsimple

