
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

/**
 * @file TraceTypes.h
 * @brief Type declaratons and enums for the trace mobility model.
 * @author Kristjan V. Jonsson
 */

#ifndef __TYPES_INCLUDED__
#define __TYPES_INCLUDED__

// Defines for traced event types
#define NO_EVENT_KIND  0
#define CREATE_EVENT_KIND 1
#define WAYPOINT_EVENT_KIND 2
#define DESTROY_EVENT_KIND 3
#define CONTACT_EVENT_KIND 4

/**
 * @brief Waypoint data structure.
 *
 * Data structure for a single waypoint event. Used by the NodeFactory object for
 * storing cached waypoints until they are forwarded to a newly created TraceMobility
 * module.
 */
struct WAYPOINT_EVENT
{
  int id;
  double time;
  double x;
  double y;
  double speed;
};

/**
 * @brief Contact event types used in the contact driven modeling approach
 */
enum ContactEventType {Contact,Break};
/**
 * @brief Contact event data structure.
 *
 * Used by the NodeFactory to hold a single contact event when read from a
 * trace file. The collection of contact events is cached by the NodeFactory 
 * until a node utilizing the contact driven mobility approach is created.
 * The contact events are then passed on to the ContactNotifier module
 * contained in the node.
 */
struct CONTACT_EVENT
{
  ContactEventType type;
  int id;
  double time;
  int peerId;
};

/**
 * Container for cached waypoint events read from a trace file.
 */
typedef std::list<WAYPOINT_EVENT> waypointEventsList; 
/**
 * Container for cached contact events read from a trace file.
 */
typedef std::list<CONTACT_EVENT> contactEventsList;

#endif /* __TYPES_INCLUDED__ */
