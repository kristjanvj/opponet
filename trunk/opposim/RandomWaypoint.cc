
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

#include "RandomWaypoint.h"

cRandomWaypoint::cRandomWaypoint( double x, double y,
																	double velocityMean, double velocitySd,
																	double pauseMean, double pauseSd,
			                            int scenarioWidth, int scenarioHeight )
{
	m_fMeanVelocity = velocityMean;
	m_fSdVelocity = velocitySd;
	m_fMeanPause = pauseMean;
	m_fSdPause = pauseSd;

	m_dScenarioWidth = scenarioWidth;
	m_dScenarioHeight = scenarioHeight;

	double initialX = x;
	double initialY = y;

  // Use the guidelines from paper by Navidi and Camp from 2003 to initialize initial
  // location to the steady state distribution.
  // The zero problem with the velocity distribution is also addressed using 
  // a truncated normal distribution with a minimum speed limit.
  // Note: The initial paused state distribution suggested by Navidi and Camp
  // is not presently implemented.
	if ( initialX < 0 && initialY < 0 )
	{
  	double u1,r;
    do
    {
  		initialX = uniform( 0.0, (double)m_dScenarioWidth );
  		initialY = uniform( 0.0, (double)m_dScenarioHeight );

    	m_fLocationX = initialX;
    	m_fLocationY = initialY;
    	m_fLocationZ = 0.0;

    	m_fWaypointX = uniform( 0.0, (double)m_dScenarioWidth );
    	m_fWaypointY = uniform( 0.0, (double)m_dScenarioHeight );
      
      double diffX = m_fWaypointX-m_fLocationX;
      double diffY = m_fWaypointY-m_fLocationY;
      
      r = sqrt(diffX*diffX+diffY*diffY)/sqrt(2.0);
      u1 = uniform( 0.0, 1.0 );
    }
    while ( u1 >= r );
    
    double u2 = uniform( 0.0, 1.0 );
    
    m_fLocationX = u2*m_fLocationX + (1-u2)*m_fWaypointX;
    m_fLocationY = u2*m_fLocationY + (1-u2)*m_fWaypointY;
    
    // A truncated normal distribution with a minimum is used to prevent the problems
    // with RWP noted when the distribution used for velocity includes zero.
  	m_fVelocity = MIN_VELOCITY + truncnormal( ( m_fMeanVelocity - MIN_VELOCITY ), m_fSdVelocity );    
  }
  else
  {
    initialize( initialX, initialY );
  }

	m_tLastUpdate = 0.0;
	m_fNextMoveTime = 0.0;
}

void cRandomWaypoint::initialize( double x, double y )
{
	m_fLocationX = x;
	m_fLocationY = y;
	m_fLocationZ = 0.0;

	m_fWaypointX = uniform( 0.0, (double)m_dScenarioWidth );
	m_fWaypointY = uniform( 0.0, (double)m_dScenarioHeight );

	m_fVelocity = normal( m_fMeanVelocity, m_fSdVelocity );
}

void cRandomWaypoint::updateLocation( simtime_t curTime )
{
  // Handle movement outside the scenario boundaries
	if ( checkOffMap() )
		initialize( m_fLocationX, m_fLocationY );

  // Calculate the delta time from the last update
	double fDeltaTime = curTime - m_tLastUpdate;
	m_tLastUpdate = curTime;

  // Dont do anyting if we are currently pausing
	if ( m_fNextMoveTime > curTime )
		return;

  // Calculate the distance to move in this update
	double fUpdateDistance = m_fVelocity* fDeltaTime;
	// Calculate the distance to the waypoint from our current location
	double fWaypointDistance = getDistance( m_fWaypointX, m_fWaypointY, 0.0 );

	if ( fWaypointDistance < fUpdateDistance )
	{
	  // We have reached the waypoint. Initialize a new waypoint location.
	  // Set the next time to move from the pause distribution.
		initialize( m_fWaypointX, m_fWaypointY );
		m_fNextMoveTime = curTime + truncnormal( m_fMeanPause, m_fSdPause );
	}
	else
	{
	  // Navigate towards the waypoint
		double percentMovement = fUpdateDistance/fWaypointDistance;
		m_fLocationX += percentMovement * ( m_fWaypointX - m_fLocationX );
		m_fLocationY += percentMovement * ( m_fWaypointY - m_fLocationY );

    // Next move time is only used when pausing at a waypoint.
		m_fNextMoveTime = 0.0;
	}
}

bool cRandomWaypoint::checkOffMap()
{
	bool offMap = ( ( m_fLocationX < 0 || m_fLocationX > m_dScenarioWidth ) ||
	          	    ( m_fLocationY < 0 || m_fLocationY > m_dScenarioHeight ) );

	if ( m_fLocationX < 0 )
		m_fLocationX = 0;
	if ( m_fLocationX > m_dScenarioWidth )
		m_fLocationX = m_dScenarioWidth;
	if ( m_fLocationY < 0 )
		m_fLocationY = 0;
	if ( m_fLocationY > m_dScenarioHeight )
		m_fLocationY = m_dScenarioHeight;

	return offMap;
}
