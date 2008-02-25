
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

// ***************************************************************************
//
// Random waypoint mobility module
//
// @version 1.0 
// @author  Kristjan V. Jonsson
//
// ***************************************************************************

 
#ifndef __RANDOM_WAYPOINT_MOBILITY_INCLUDED__
#define __RANDOM_WAYPOINT_MOBILITY_INCLUDED__

#include <omnetpp.h>
#include <BasicMobility.h>

// The minimum velocity of the normal distribution
#define MIN_VELOCITY  0.5

/**
 * @brief Random waypoint mobility module.
 *
 * Random waypoint is a simple mobility model which works as follows:
 * -# A node is initialized at a uniformly distributed location within a fixed boundary.
 * -# The node picks a new waypoint after an optional pause, which is also
 *    selected from a random distribution.
 * -# The node navigates towards this point, interpolating a new position at 
 *    each discrete update.
 * -# Upon arriving a the new location, the node selects a new waypoint, optionally
 *    waiting before starting its journey towards the new location.
 * This process is repeated for the duration of the simulation.
 *
 * The accuracy of the random waypoint model has recently been called into 
 * question in a number of articles, among others one by Navidi and Camp from 2003.
 * They however provide a number of guidelines for initializing the nodes to
 * increase the accuracy of the simulation. This implemenation follows the 
 * guidelines set forth by Navidi and Camp to initialize initial location of
 * the nodes to be consistent with the expected steady state distribution.
 * Note: The initial paused state distribution suggested by Navidi and Camp
 * is not presently implemented.
 *
 * The velocity and pause times are drawn from a truncated normal distribution.
 * The zero speed problem is addressed using a minimum speed, which is presently
 * fixed at 0.5 m/s. Otherwise, a number of stuck nodes (ones with zero or very
 * low velocity) is expected during a prolonged simulation.
 *
 * This class wraps the cRandomWaypoint class for use in an OMNeT++ simulation. 
 * 
 * @author Kristjan V. Jonsson
 */
class  RandomWaypointMobility : public BasicMobility
{
  protected:        
    /** @brief Target position of the host - the next waypoint*/
    Coord targetPos;
    
    /** The mean velocity used for the simulation */
		double m_fMeanVelocity;
		/** The standard deviation for the velocity distribution */
		double m_fSdVelocity;
		/** The mean pause time */
		double m_fMeanPause;
		/** The standard deviation of the pause time distribution */
		double m_fSdPause;

    /** The next move time. Used to pause nodes when they reach a waypoint. */
		simtime_t m_fNextMoveTime; 
		/** Last update time. Used to calculate the distance to interpolate between waypoints. */
		simtime_t m_tLastUpdate; 
		
  public:
    Module_Class_Members( RandomWaypointMobility, BasicMobility, 0 );

    /** @brief Initializes mobility model parameters. */
    virtual void initialize(int);

  protected:
    /** @brief Move the host */
    virtual void makeMove();
    
  protected:
    /** @brief update the node location given a update interval */
		void _updateLocation();
		/** @brief check if the node is outside the allowed boundaries */
		bool _checkOffMap();
		
		void _initialize();
		void _pickWaypoint();
};

#endif /* __RANDOM_WAYPOINT_MOBILITY_INCLUDED__ */

