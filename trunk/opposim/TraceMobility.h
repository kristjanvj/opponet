
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

#ifndef __TRACE_MOBILITY_INCLUDED__
#define __TRACE_MOBILITY_INCLUDED__

#include <omnetpp.h>
#include <BasicMobility.h>
#include "TraceTypes.h"

/**
 * @brief Trace mobility module. 
 *
 * Used in conjunction with the NodeFactory object to implement the
 * trace mobility model.
 *
 * Trace mobility uses externally generated traces or measurement 
 * results to control a nodes movement. The module is initialized by the 
 * factory at a certain location upon creation. It then supplies the module
 * with its waypoint list and an optional destroy event. 
 * The mobility module is then autonomous for the duration of its lifetime.
 *
 * @version 1.0 
 * @author  Olafur R. Helgason
 * @author  Kristjan V. Jonsson
 */
class TraceMobility : public BasicMobility
{
  private:
    /** @brief The update interval in seconds */
		double m_updateInterval;
    
    /** @brief location update event */		
    cMessage *updateEvent;

    /** @brief The current position of the host */    
    Coord curPos;        
    /** @brief Target position of the host */
    Coord targetPos;
    
    /** @brief The pending event list. Contains waypoint updates */
    waypointEventsList m_eventList;
    
    Coord _targetPos;
    
    Coord _stepSize;
    int _numSteps;
    int _step;
    Coord _stepTarget;

    Coord _nextTarget;
    double _nextSpeed;
    bool _isNextTargetSet;    
    
    int m_curWaypoint;
  
  public:
    Module_Class_Members( TraceMobility, BasicMobility, 0 );

    /** @brief Initializes mobility model parameters. */
    virtual void initialize(int);
    /** @brief Called when the module is destroyed */
    virtual void finish();
    /** @brief The message handler. */
    virtual void handleMessage(cMessage *msg);
    
    /** @brief Initialize the waypoint event list. Called by the 
               trace factory object upon creation of the node */    
    void initializeTrace( const waypointEventsList *eventList );

  protected:
    /** @brief Move the host one step */
    virtual void makeMove();    
    /** @brief Set a new target or waypoint for the node movement */
    void setTarget(double time, double x, double y, double speed );
};

#endif /* __TRACE_MOBILITY_INCLUDED__ */

