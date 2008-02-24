
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
 
#include "TraceMobility.h"
#include <FWMath.h>

//#define __TRACE_MOBILITY_DEBUG__

Define_Module(TraceMobility);

void TraceMobility::initialize(int stage)
{
  BasicMobility::initialize(stage);

  if (stage == 0)
  {        
    updateInterval = par("updateInterval");
    moveCategory = bb->getCategory(&move);

    // Initialize the start position
    move.startPos.x = par("x");
    move.startPos.y = par("y");
    	
    ev << fullPath() << ": initializing TraceMobility module." << endl;  	
    ev << "    initial x:       " << move.startPos.x << endl;
    ev << "    initial y:       " << move.startPos.y << endl;
    ev << "    update interval: " << updateInterval << endl;
    ev << "    debug:           " << debug << endl;
      
    // Set initial move data
    _targetPos = move.startPos;
    move.speed = 0;
    move.startTime = simTime();
    move.direction = Coord(0,0);
    
    m_curWaypoint = 0; 

    // Update blackboard & screen position
    updatePosition();

    // Create the events
    updateEvent = new cMessage("updateEvent");
  }
  else if ( stage == 1 )
  {
    //
  }
}

void TraceMobility::finish()
{
  cancelAndDelete(updateEvent);
}

void TraceMobility::handleMessage(cMessage *msg)
{
  if ( msg == updateEvent )
  {
    makeMove();
    updatePosition();
  }
  else 
  {
    BasicMobility::handleMessage(msg);
  }        
}

void TraceMobility::makeMove()
{
  #ifdef __TRACE_MOBILITY_DEBUG__
  ev << fullPath() << ": TraceMobility::makeMove"<<endl;
  ev << "    (" << simTime() << ") stepping forward. step #=" << _step 
     << " startPos: " << move.startPos.info() << endl;  
  #endif
  _step++;
    
  if( _step >= _numSteps )
  {
    #ifdef __TRACE_MOBILITY_DEBUG__
    ev << "    ****** destination reached ******" << endl;
    #endif  

    // Stop the movement    
	  move.startPos = _targetPos;
    move.startTime = simTime();
	  move.speed = 0;
    if ( updateEvent->isScheduled() )
      cancelEvent( updateEvent );
    
    // Schedule a new event if any remain
    if ( !m_eventList.empty() )
    {
      WAYPOINT_EVENT waypoint = m_eventList.front();
      m_eventList.pop_front();

      #ifdef __TRACE_MOBILITY_DEBUG__
      ev << "    (" << simTime() << ") new position: x=" << waypoint.x << " y=" << waypoint.y 
          << " speed=" << waypoint.speed << endl;
      #endif
      setTarget( waypoint.time, waypoint.x, waypoint.y, waypoint.speed );
    }  
  } 
  else if( _step < _numSteps )
  {
	  // step forward
	  move.startPos = _stepTarget;
	  move.startTime = simTime();
	  _stepTarget += _stepSize;
    if ( updateEvent->isScheduled() )
      cancelEvent( updateEvent );
    scheduleAt( simTime() + updateInterval, updateEvent );
  } 
}

void TraceMobility::setTarget(double time, double x, double y, double speed )
{
  Coord newTarget(x,y);
  m_curWaypoint++;

  double activateTime = time;
  if ( activateTime < simTime() )
    activateTime = simTime();
    
  // Set the target position and distance to target
  _targetPos = newTarget;
  double distance = move.startPos.distance(_targetPos);
  move.speed = speed; 
  double travelTime = distance / move.speed;

  // Get the number of steps needed to be covered.
  _numSteps = static_cast<int>(ceil(travelTime/updateInterval));
    
  _stepSize = (_targetPos - move.startPos)/_numSteps;
  _stepTarget = move.startPos + _stepSize;
  _step = 0;
  move.setDirection(_targetPos);
   
  #ifdef __TRACE_MOBILITY_DEBUG__
  ev << fullPath() << ": (" << simTime() << ") WP=" << m_curWaypoint << " Activate time=" << activateTime 
                   << " dest=(" << move.startPos.x << "," << move.startPos.y << ") "
                   << " distance=" << distance << " travelTime=" << travelTime << " velocity=" << move.speed
                   << " numSteps=" << _numSteps << endl; 
  #endif  

  if ( updateEvent->isScheduled() )
    cancelEvent( updateEvent );
  scheduleAt( activateTime, updateEvent );
}

void TraceMobility::initializeTrace( const waypointEventsList *eventList )
{
  Enter_Method_Silent();
  
  if ( eventList == NULL )
    return;
    
  // Copy the given event list to the internal list.
  m_eventList.resize(eventList->size());
  std::copy(eventList->begin(),eventList->end(),m_eventList.begin());

  WAYPOINT_EVENT waypoint;
  
  #ifdef __TRACE_MOBILITY_DEBUG__  
  waypointEventsList::iterator i = m_eventList.begin();
  for(; i != m_eventList.end();i++)
  {
    waypoint = (*i);
    ev << "event: id=" << waypoint.id << " time=" << waypoint.time
       << " x=" << waypoint.x << " y=" << waypoint.y
       << " speed=" << waypoint.speed << endl;
  }  
  #endif  
      
  if ( m_eventList.size() != 0 )
  {
    waypoint = m_eventList.front();
    m_eventList.pop_front();
    #ifdef __TRACE_MOBILITY_DEBUG__  
    ev << "First scheduled event: id=" << waypoint.id << " time=" << waypoint.time
       << " x=" << waypoint.x << " y=" << waypoint.y
       << " speed=" << waypoint.speed << endl;
    #endif    

    setTarget( waypoint.time, waypoint.x, waypoint.y, waypoint.speed );      
  }
  else
  {
    error("No events to schedule");
  }
}
