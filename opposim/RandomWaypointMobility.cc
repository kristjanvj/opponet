
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
 
#include "RandomWaypointMobility.h"
#include <FWMath.h>

Define_Module(RandomWaypointMobility);


/**
 * Reads the parameters.
 * If the host is not stationary it calculates a random position and
 * schedules a timer to trigger the first movement
 */
void RandomWaypointMobility::initialize(int stage)
{
  BasicMobility::initialize(stage);

  EV << fullPath() << ": initializing RandomWaypointMobility, stage=" << stage << endl;

  if (stage == 0)
  {
  	hasPar("velocity") ? m_fMeanVelocity = par("velocity") : m_fMeanVelocity = 1.0;
  	hasPar("velocitySd") ? m_fSdVelocity = par("velocitySd") : m_fSdVelocity = 0.0;
  	hasPar("pauseTimeMean") ? m_fMeanPause = par("pauseTimeMean") : m_fMeanPause = 0.0;
  	hasPar("pauseTimeSd") ? m_fSdPause = par("pauseTimeSd") : m_fSdPause = 0.0;
  	
  	EV << "   velocity - mean: " << m_fMeanVelocity << endl;
  	EV << "   velocity - sd:   " << m_fSdVelocity << endl;
  	EV << "   pause - mean:    " << m_fMeanPause << endl;
  	EV << "   pause - sd:      " << m_fSdPause << endl;
  	 	
    _initialize();
  }
  else if ( stage == 1 )
  {
    //
  }
}

void RandomWaypointMobility::makeMove()
{
  _updateLocation();	
  move.startTime = simTime();
}
void RandomWaypointMobility::_updateLocation()
{
  // Handle movement outside the scenario boundaries
	if ( _checkOffMap() )
		_pickWaypoint();

  // Calculate the delta time from the last update
	double fDeltaTime = simTime() - m_tLastUpdate;
	m_tLastUpdate = simTime();

  // Dont do anyting if we are currently pausing
	if ( m_fNextMoveTime > simTime() )
		return;

  // Calculate the distance to move in this update
	double updateDistance = move.speed * fDeltaTime;
	// Calculate the distance to the waypoint from our current location
  double waypointDistance = move.startPos.distance(targetPos);

	if ( waypointDistance < updateDistance )
	{
	  // We have reached the waypoint. Initialize a new waypoint location.
	  // Set the next time to move from the pause distribution.
		_pickWaypoint();
		m_fNextMoveTime = simTime() + truncnormal( m_fMeanPause, m_fSdPause );		
	}
	else
	{
	  // Navigate towards the waypoint
		double percentMovement = updateDistance/waypointDistance;
		move.startPos.x += percentMovement * ( targetPos.x - move.startPos.x );
		move.startPos.y += percentMovement * ( targetPos.y - move.startPos.y );

    // Next move time is only used when pausing at a waypoint.
		m_fNextMoveTime = 0.0;
	}
}

bool RandomWaypointMobility::_checkOffMap()
{
	return ( ( move.startPos.x < 0 || move.startPos.x > playgroundSizeX() ) ||
	         ( move.startPos.y < 0 || move.startPos.y > playgroundSizeY() ) );
}

/**
 * Use the guidelines from paper by Navidi and Camp from 2003 to initialize initial
 * location to the steady state distribution.
 * The zero problem with the velocity distribution is also addressed using 
 * a truncated normal distribution with a minimum speed limit.
 * Note: The initial paused state distribution suggested by Navidi and Camp
 * is not presently implemented.
 */
void RandomWaypointMobility::_initialize()
{
	double u1,r;
	double x1,y1,x2,y2;
  do
  {
		x1 = uniform( 0.0, playgroundSizeX() );
		y1 = uniform( 0.0, playgroundSizeY() );
    x2 = uniform( 0.0, playgroundSizeX() );
  	y2 = uniform( 0.0, playgroundSizeY() );
       
    r = sqrt(pow(x1-x2,2)+pow(y1-y2,2))/sqrt(2.0);
    u1 = uniform( 0.0, 1.0 );
  }
  while ( u1 >= r );

  double u2 = uniform( 0.0, 1.0 );
  
  move.startPos.x = u2*x1 + (1-u2)*x2;
  move.startPos.y = u2*y1 + (1-u2)*y2;
  
  targetPos.x = x2;
  targetPos.y = y2;
  
  // A truncated normal distribution with a minimum is used to prevent the problems
  // with RWP noted when the distribution used for velocity includes zero.
	move.speed = MIN_VELOCITY + truncnormal( ( m_fMeanVelocity - MIN_VELOCITY ), m_fSdVelocity );    
}

void RandomWaypointMobility::_pickWaypoint()
{
	targetPos.x = uniform( 0.0, playgroundSizeX() );
	targetPos.y = uniform( 0.0, playgroundSizeY() );

	move.speed = normal( m_fMeanVelocity, m_fSdVelocity );
}


