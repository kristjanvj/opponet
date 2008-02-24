
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


#ifndef __COORDINATE_VECTOR_INCLUDED__
#define __COORDINATE_VECTOR_INCLUDED__

#include <omnetpp.h>
#include <string>

using namespace std;

/**
 * @brief Coordinate vector class. Stores (x,y,z) coordinates for a point.
 *
 * The coordinate vector class stores the (x,y,z) coordinates for a point and
 * includes basic functions such as calculating distance to another coordinate.
 *
 * @author Kristjan V. Jonsson
 */
class cCoordinateVector
{
	protected:
		double m_fLocationX;
		double m_fLocationY;
		double m_fLocationZ;

		double m_fDirectionVectorX;
		double m_fDirectionVectorY;
		double m_fDirectionVectorZ;

	public:
		cCoordinateVector();
		cCoordinateVector( cCoordinateVector &vector );
		cCoordinateVector( double x, double y, double z );

		void setCoordinates( cCoordinateVector &vector );
		void setCoordinates( double x, double y, double z );

		void increment( double x, double y, double z );

		void setDirection( cCoordinateVector v );
		void getDirection( double &x, double &y, double &z );
		static cCoordinateVector getDirection( cCoordinateVector v1, cCoordinateVector v2 );
		double getDirectionX();
		double getDirectionY();
		double getDirectionZ();
		void reverseDirection();

		double getX();
    void   setX( double x );
		double getY();
    void   setY( double y );
		double getZ();
    void   setZ( double z );

		// Get distance to another node
		double getDistance( cCoordinateVector &vector );
		double getDistance( double x, double y, double z );

	protected:
		double _max( double a1, double a2, double a3 );
};


#endif /* __COORDINATE_VECTOR_INCLUDED__ */
