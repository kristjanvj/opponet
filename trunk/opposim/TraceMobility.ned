
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
// Trace mobility module.
//
// Used in conjunction with the NodeFactory object to implement the
// trace mobility model.
//
// Trace mobility uses externally generated traces or measurement 
// results to control a nodes movement. The module is initialized by the 
// factory at a certain location upon creation. It then supplies the module
// with its waypoint list and an optional destroy event. 
// The mobility module is then autonomous for the duration of its lifetime.
//
// @author  Olafur R. Helgason
// @author  Kristjan V. Jonsson
// @version 1.0 
//
simple TraceMobility
    parameters:    
        debug: bool,              // debug switch
        x: numeric,               // initial x location
        y: numeric,               // initial y location
        updateInterval: numeric;  // The update interval which is used to interpolate between waypoints
endsimple

