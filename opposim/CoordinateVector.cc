
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

#include "CoordinateVector.h"

cCoordinateVector::cCoordinateVector()
{
	m_fLocationX = 0.0;
	m_fLocationY = 0.0;
	m_fLocationZ = 0.0;
}

cCoordinateVector::cCoordinateVector( cCoordinateVector &vector )
{
	m_fLocationX = vector.m_fLocationX;
	m_fLocationY = vector.m_fLocationY;
	m_fLocationZ = vector.m_fLocationZ;
}

cCoordinateVector::cCoordinateVector( double x, double y, double z )
{
	m_fLocationX = x;
	m_fLocationY = y;
	m_fLocationZ = z;
}

void cCoordinateVector::setCoordinates( cCoordinateVector &vector )
{
	m_fLocationX = vector.m_fLocationX;
	m_fLocationY = vector.m_fLocationY;
	m_fLocationZ = vector.m_fLocationZ;
}

void cCoordinateVector::setCoordinates( double x, double y, double z )
{
	m_fLocationX = x;
	m_fLocationY = y;
	m_fLocationZ = z;
}

void cCoordinateVector::increment( double x, double y, double z )
{
	m_fLocationX += x;
	m_fLocationY += y;
	m_fLocationZ += z;
}

void cCoordinateVector::setDirection( cCoordinateVector v )
{
	m_fDirectionVectorX = v.getX() - getX();
	m_fDirectionVectorY = v.getY() - getY();
	m_fDirectionVectorZ = v.getZ() - getZ();

	double largest = _max( m_fDirectionVectorX, m_fDirectionVectorY, m_fDirectionVectorZ );

	if ( largest > 0 )
	{
		m_fDirectionVectorX = m_fDirectionVectorX/largest;
		m_fDirectionVectorY = m_fDirectionVectorY/largest;
		m_fDirectionVectorZ = m_fDirectionVectorZ/largest;
	}
}

void cCoordinateVector::getDirection( double &x, double &y, double &z )
{
	x = m_fDirectionVectorX;
	y = m_fDirectionVectorY;
	z = m_fDirectionVectorZ;
}

// static
cCoordinateVector cCoordinateVector::getDirection( cCoordinateVector v1, cCoordinateVector v2 )
{
	cCoordinateVector vector = v1;
	vector.setDirection( v2 );
	return vector;
}

double cCoordinateVector::getDirectionX()
{
	return m_fDirectionVectorX;
}

double cCoordinateVector::getDirectionY()
{
	return m_fDirectionVectorY;
}

double cCoordinateVector::getDirectionZ()
{
	return m_fDirectionVectorZ;
}

void cCoordinateVector::reverseDirection()
{
	m_fDirectionVectorX = -m_fDirectionVectorX;
	m_fDirectionVectorY = -m_fDirectionVectorY;
	m_fDirectionVectorZ = -m_fDirectionVectorZ;
}

double cCoordinateVector::getX()
{
	return m_fLocationX;
}

void cCoordinateVector::setX( double x )
{
  m_fLocationX = x;
}

double cCoordinateVector::getY()
{
	return m_fLocationY;
}

void cCoordinateVector::setY( double y )
{
  m_fLocationY = y;
}

double cCoordinateVector::getZ()
{
	return m_fLocationZ;
}

void cCoordinateVector::setZ( double z )
{
  m_fLocationZ = z;
}

// Get distance to another node
double cCoordinateVector::getDistance( cCoordinateVector &vector )
{
	// Return magnitude of vector
	double dist_x = m_fLocationX - vector.getX();
	double dist_y = m_fLocationY - vector.getY();
	double dist_z = m_fLocationZ - vector.getZ();
	return sqrt( dist_x*dist_x + dist_y*dist_y + dist_z*dist_z );
}

double cCoordinateVector::getDistance( double x, double y, double z )
{
	// Return magnitude of vector
	double dist_x = m_fLocationX - x;
	double dist_y = m_fLocationY - y;
	double dist_z = m_fLocationZ - z;
	return sqrt( dist_x*dist_x + dist_y*dist_y + dist_z*dist_z );
}

double cCoordinateVector::_max( double a1, double a2, double a3 )
{
	double aa1 = fabs(a1);
	double aa2 = fabs(a2);
	double aa3 = fabs(a3);

	if ( aa1 > aa2 && aa1 > aa3 )
		return aa1;
	else if ( aa2 > aa1 && aa2 > aa3 )
		return aa2;
	else if ( aa3 > aa1 && aa3 > aa2 )
		return aa3;
	else
		return 0.0;
}
