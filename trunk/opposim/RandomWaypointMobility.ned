// ***************************************************************************
// 
// PodSim Project
//
// author:      Kristjan V. Jonsson
//
// copyright:   (C) 2007 Kristjan V. Jonsson
//
// This module is part of the podsim project, developed as part
// of a masters dissertation done at the Laboratory for communications
// networks (LCN) at KTH in Sweden and at Reykjavik University 
// in Iceland.
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
// Random waypoint mobility model.
//
// This OMNeT++ module wraps the RandomWaypoint mobility class. It implements
// a random waypoint mobility model.
//
// Random waypoint is a simple mobility model which works as follows:
// -# A node is initialized at a uniformly distributed location within a fixed boundary.
// -# The node picks a new waypoint after an optional pause, which is also
//    selected from a random distribution.
// -# The node navigates towards this point, interpolating a new position at 
//    each discrete update.
// -# Upon arriving a the new location, the node selects a new waypoint, optionally
//    waiting before starting its journey towards the new location.
// This process is repeated for the duration of the simulation.
//
// The accuracy of the random waypoint model has recently been called into 
// question in a number of articles, among others one by Navidi and Camp from 2003.
// They however provide a number of guidelines for initializing the nodes to
// increase the accuracy of the simulation. This implemenation follows the 
// guidelines set forth by Navidi and Camp to initialize initial location of
// the nodes to be consistent with the expected steady state distribution.
// Note: The initial paused state distribution suggested by Navidi and Camp
// is not presently implemented.
//
// The velocity and pause times are drawn from a truncated normal distribution.
// The zero speed problem is addressed using a minimum speed, which is presently
// fixed at 0.5 m/s. Otherwise, a number of stuck nodes (ones with zero or very
// low velocity) is expected during a prolonged simulation.
//
// @author  Kristjan V. Jonsson
// @version 1.0
//
simple RandomWaypointMobility
  parameters:    
    debug : bool,             // debug switch
    x: numeric,               // Initial x location
    y: numeric,               // Initial y location
    updateInterval: numeric,  // Update interval in seconds
    velocity: numeric,        // Mean veloctity in m/s
    velocitySd: numeric,      // Standard deviation of the veloctity. 
    pauseTimeMean: numeric,   // Mean pause time in seconds
    pauseTimeSd: numeric;     // Standard deviation of the pause time.
endsimple

