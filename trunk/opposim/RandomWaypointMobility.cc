
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

//#define __RANDOM_WAYPOINT_MOBILITY_DEBUG__

Define_Module(RandomWaypointMobility);


/**
 * Reads the parameters.
 * If the host is not stationary it calculates a random position and
 * schedules a timer to trigger the first movement
 */
void RandomWaypointMobility::initialize(int stage)
{
  BasicMobility::initialize(stage);

  EV << "initializing RandomWaypointMobility stage " << stage << endl;

  if (stage == 0)
  {
    double x = 0.0;
  	double y = 0.0;
  	double z = 0.0;

  	hasPar("x") ? x = par("x") : x = 0.0;
  	hasPar("y") ? y = par("y") : y = 0.0;

  	double velocityMean = 0.0;
  	double velocitySd = 0.0;
  	double pauseTimeMean = 0.0;
  	double pauseTimeSd = 0.0;

  	hasPar("velocity") ? velocityMean = par("velocity") : velocityMean = 1.0;
  	hasPar("velocitySd") ? velocitySd = par("velocitySd") : velocitySd = 0.0;
  	hasPar("pauseTimeMean") ? pauseTimeMean = par("pauseTimeMean") : pauseTimeMean = 0.0;
  	hasPar("pauseTimeSd") ? pauseTimeSd = par("pauseTimeSd") : pauseTimeSd = 0.0;

  	hasPar("activateTime") ? m_tActivate = par("activateTime") : m_tActivate = 0.0;

  	hasPar("updateInterval") ? m_updateInterval = par("updateInterval") : m_updateInterval = 1.0;

  	m_mobility = new cRandomWaypoint( x, y, velocityMean, velocitySd,
  	                                  pauseTimeMean, pauseTimeSd,
  	                                  (int)playgroundSizeX(), (int)playgroundSizeY() );
  	                                  
    move.speed=1;  // Dummy to get the base class to scedule event
  }
  else if ( stage == 1 )
  {
    //
  }
}

void RandomWaypointMobility::makeMove()
{
  m_mobility->updateLocation( simTime() );
	
  move.startTime = simTime();
  move.startPos.x = m_mobility->getX(); 
  move.startPos.y = m_mobility->getY();
  
  fixIfHostGetsOutside();
}

void RandomWaypointMobility::fixIfHostGetsOutside()
{
    Coord dummy;
    double dum;

    handleIfOutside( RAISEERROR, targetPos, dummy, dummy, dum );
}


