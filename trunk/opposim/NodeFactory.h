
// ***************************************************************************
// 
// OppoNet Project
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

#ifndef __NODE_FACTORY_INCLUDED__
#define __NODE_FACTORY_INCLUDED__

#include <omnetpp.h>
#include <string>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "NodeFactoryItem.h"
#include "TraceMobility.h"
#include "ContactNotifier.h"
#include "TraceEvents_m.h"

using namespace std;

typedef vector<cModule*> MODULE_VECTOR_TYPE;
typedef vector<NodeFactoryItem*> CREATED_ITEMS_VECTOR_TYPE;

enum COORD_TYPE {xCoordinate,yCoordinate};
enum TRACE_TYPE {None,MobilityTrace,ContactTrace};

/**
 *
 * @brief Node factory object. Creates nodes dynamically using definitions from a tracefile.
 *
 * The node factory instantiates nodes dynamically during a simulation run from definitions
 * in a XML trace file. A mobility trace defines create, destroy and waypoint events for
 * a collection of nodes. Such a trace is created using an external mobility generator, e.g.
 * UrbanMobility from the MobiTrace toolkit, or UDel (http://udelmodels.eecis.udel.edu).
 * Contact traces can additionally be used. Such traces can e.g. be created from contact 
 * measurements conducted with mobile devices.
 *
 * @author Kristjan V. Jonsson
 * @author Olafur R. Helgason
 * @version 1.0 
 */
class NodeFactory : public cSimpleModule
{
	private:
		int 		      m_scenarioSizeX;      /**< @brief The width of the scenario */
		int 		      m_scenarioSizeY;      /**< @brief The height of the scenario */
    string        m_traceFile;          /**< @brief Name of the tracefile */

    /** @brief The number of initialized modules, i.e. create commands read from file */
    unsigned long m_initializedCount;   
    /** @brief The number of generated modules */
    unsigned long m_generateCount;      
    /** @brief The number of destroyed modules. The simulation ends when the number of 
        destroyed modules equals that of generated ones */
    unsigned long m_destroyedCount;     
    /** @brief The total lifetime of generated nodes. Used to calculate average lifetime */
    double        m_totalLifetime;
    /** @brief Running serial of created modules. Used for appending a unique serial to the
        node prefix when generating a unique name in the simulation */
		unsigned long m_moduleCount;  
		
		/** @brief The type of trace in effect. Either mobility or contact traces can be 
		           in effect at the same time. Trace types cannot be mixed. */
		TRACE_TYPE m_traceType;
    /** @brief The generated modules */
		CREATED_ITEMS_VECTOR_TYPE m_createdItems;
  
    /** @brief A collection of lists of waypoints read from the trace file. The lists
               are assigned to TraceMobility modules of created nodes and then
               cleared. */
    map<int, waypointEventsList> _pendingWaypointsLists;
    /** @brief A collection of contact events read from the trace file. The lists
               are assigned to ContactNotifier modules upon node creation. */
    map<int, contactEventsList> _pendingContactsLists;


  public:
    /** @brief Constructor */
    NodeFactory();

  protected:
  	/** @brief Overrides of virtual base class functions. */
    virtual void initialize();
    /** @brief Overrides of virtual base class functions. */
    virtual void finish();
    /** @brief Overrides of virtual base class functions. Handles create and destroy messages. */
    virtual void handleMessage(cMessage *msg);

    /** @brief Create a node. Triggered by a CreateEvent message */
    void createNode( CreateEvent *event );
    /** @brief Destroy a node. Triggered by a DestroyEvent message */
    int  destroyNode( DestroyEvent *event );
    
    /** @brief Reads a XML trace file. The filename is specified 
               as a module startup parameter. */
    void readXmlTrace();       
    /** @brief Reads a XML mobility trace. */
    void readXmlMobilityTrace( xmlTextReaderPtr reader ); 
    /** @brief Reads a XML contact trace file. */
    void readXmlContactTrace( xmlTextReaderPtr reader );     
    
  private:
    /** @brief Validates a create or waypoint location. Used when parsing the xml trace file */
    bool _validateLocation( double coordinate, COORD_TYPE ct );
};

#endif /* __NODE_FACTORY_INCLUDED__ */
