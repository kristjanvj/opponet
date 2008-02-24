
// ***************************************************************************
// 
// OppoNet Project
//// This file is a part of the opponet project, jointly managed by the 
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

#include "NodeFactory.h"

//#define __NODE_FACTORY_DEBUG__

// The module class needs to be registered with OMNeT++
Define_Module(NodeFactory);

//
// Constructor
//
// Initialize event messages
//
NodeFactory::NodeFactory()
{
	m_moduleCount = 0;
  m_generateCount = 0;
  m_initializedCount = 0;
  m_destroyedCount = 0;
  m_totalLifetime = 0.0;
  m_traceType = None;
}

//
// initialize
//
// Override of virtual base class method.
// Initialize simulation at startup.
//
void NodeFactory::initialize()
{
  // Read parameters from the ini file
	hasPar("scenarioSizeX") ? m_scenarioSizeX = par("scenarioSizeX") : m_scenarioSizeX = 1000;
	hasPar("scenarioSizeY") ? m_scenarioSizeY = par("scenarioSizeY") : m_scenarioSizeY = 1000;
  hasPar("traceFile") ? m_traceFile = (const char *)par("traceFile") : m_traceFile = "";

  // Display initial message
	ev << fullPath() << ": Initializing object factory" << endl;
	ev << "    Scenario size:   (" << m_scenarioSizeX << "," << m_scenarioSizeY << ") m" << endl;
  ev << "    Trace file:      " << m_traceFile << endl;

  // Trace file must be defined. Display error if not.
  if ( m_traceFile == "" )
    error("Trace file is undefined. Cannot initialize trace based mobility or contacts.");
  else       
  {
    readXmlTrace();
    recordScalar("factory.initialized", m_initializedCount);
  }
 	
	if ( ev.isGUI() )
	{
	  WATCH(m_initializedCount);
    WATCH(m_generateCount);
    WATCH(m_destroyedCount);
	}		
}


void NodeFactory::finish()
{
  // Dispose of any remaining dynamically created modules.
  NodeFactoryItem *item;
	for( unsigned int i=0; i < m_createdItems.size(); i++ )
	{
		item = (NodeFactoryItem*)m_createdItems[i];
		if ( item == NULL )
			continue;
		m_totalLifetime += simTime() - item->getCreateTime();
		item->getModule()->callFinish();
		item->getModule()->deleteModule();
		m_destroyedCount++;
	}    

  //
  // Some final reporting
  //
  
  double aveLifetime = 0.0;
  if ( m_generateCount > 0 )
    aveLifetime = m_totalLifetime / m_generateCount;

  ev << fullPath() << ": Finishing run at " << simTime() << endl;
  ev << "    Total initialized: " << m_initializedCount << endl;
  ev << "    Total created:     " << m_generateCount << endl;
  ev << "    Total destroyed:   " << m_destroyedCount << endl;
  ev << "    Ave lifetime:      " << aveLifetime << endl;

  recordScalar("factory.created", m_generateCount );
  recordScalar("factory.destroyed", m_destroyedCount );
  recordScalar("factory.ave.lifetime", aveLifetime );  
}

/**
 * The factory only handles create and destroy events, that is events scheduled in response
 * to scripted events read from an ini file.
 *
 * The simulation is terminated after the last node has been destroyed, when no nodes 
 * remain to be instantiated.
 */
void NodeFactory::handleMessage(cMessage *msg)
{
  if ( msg->kind() == CREATE_EVENT_KIND )
  {      
    #ifdef __NODE_FACTORY_DEBUG__
    ev << fullPath() << ": Create event handled" << endl;
    #endif
    CreateEvent *te = check_and_cast<CreateEvent*>(msg);
    createNode(te);
    delete msg;
  }
  else if ( msg->kind() == DESTROY_EVENT_KIND )
  {
    #ifdef __NODE_FACTORY_DEBUG__
    ev << fullPath() << ": Destroy event handled" << endl;
    #endif
    DestroyEvent *te = check_and_cast<DestroyEvent*>(msg);
    // Destroy node returns the number of nodes left to instantiate in the simulation.
    if ( destroyNode(te) < 1 ) 
    {
      #ifdef __NODE_FACTORY_DEBUG__
      ev << fullPath() << ": Objectfactory terminating simulation. "
                       << "All entities initialized have been destroyed" << endl;
      ev << "    initialized: " << m_initializedCount << endl;
      ev << "    generated:   " << m_generateCount << endl;
      ev << "    destroyed:   " << m_destroyedCount << endl;      
      #endif
      // Terminate the simulation as we have no remaining nodes left to instantiate.
      endSimulation();
    }
    delete msg;            
  }
}

void NodeFactory::createNode( CreateEvent *event )
{
	ev << fullPath() << ": Creating a dynamic scenario object" << endl;

	// Get the module type object from the given class name
	cModuleType *moduleType = findModuleType( event->getType() );
	if ( moduleType == NULL )
	{
		ev << fullPath() << ": The module type " << event->getType() << " is not found. "
		                 << "Using default SimpleNode" << endl;
		cModuleType *moduleType = findModuleType( "SimpleNode" );
		if ( moduleType == NULL )
		{
		  error("Default node type not found");
		}
  }

	// Create the module name 
	char szModuleName[100];
	if ( event->getName() == "" )
  	sprintf( szModuleName, "%s%.4d", event->getPrefix(), ++m_moduleCount );
  else
    sprintf( szModuleName, (const char *)event->getName() );	

	// create module
	cModule *module = moduleType->create( szModuleName, this->parentModule() );
	ev << fullPath() << ": Creating module " << szModuleName << endl;

	// Set the icon to use
	char szDisplayString[100];
	if ( event->getIconPath() == "" )
  	sprintf( szDisplayString, "i=%s", "device/palm2_s" );
  else
    sprintf( szDisplayString, "i=%s", event->getIconPath() );
	module->setDisplayString( szDisplayString );

  // Set the mobility module to use. Note that ContactTrace is included here although not
  // strictly a mobility module.
  string mobilityModel = "";
  if ( m_traceType == MobilityTrace )
  {
    mobilityModel = "TraceMobility";
    if ( strcmp( event->getMobilityModel(),"") != 0 )
      mobilityModel = event->getMobilityModel();    
  }
  else if ( m_traceType == ContactTrace )
  {
    mobilityModel = "ContactNotifier";
  }
  else
  {
    error("Unspecified or unsupported trace");
  }

	// Set the object parameters. Parameters for submodules can be set in ini file.
	if ( module->hasPar("x") )
		module->par("x") = event->getX();
	if ( module->hasPar("y") )
		module->par("y") = event->getY();
	if ( module->hasPar("x") )
		module->par("z") = 0;
				
  if ( module->hasPar("mobilityModel") )
    module->par("mobilityModel") = mobilityModel.c_str();

	// Call buildInside to create the module.
	module->buildInside();

	// create activation message
	module->scheduleStart( simTime() );
	module->callInitialize();
    
  // Populate the navigation modules of the created nodes with the cached events read from
  // the initial trace file.
  if ( mobilityModel == "TraceMobility" && _pendingWaypointsLists[event->getNodeID()].size() != 0 )
  {
    #ifdef __NODE_FACTORY_DEBUG__
    ev << fullPath() << ": Starting to set move events in created navigator object" << endl;
    #endif
    cModule *submodule = module->submodule("navigator");
    if ( submodule != NULL )
    {
      waypointEventsList waypointList = _pendingWaypointsLists[event->getNodeID()];          
      TraceMobility *mobility = check_and_cast<TraceMobility*>(submodule);   
      mobility->initializeTrace( &waypointList );
      _pendingWaypointsLists.erase( event->getNodeID() ); 
    }
  }
  else if ( mobilityModel == "ContactNotifier" && _pendingContactsLists[event->getNodeID()].size() != 0 )
  {
    #ifdef __NODE_FACTORY_DEBUG__
    ev << fullPath() << ": Starting to set contact events in created notifier object" << endl;
    #endif
    cModule *submodule = module->submodule("navigator");
    if ( submodule != NULL )
    {
      contactEventsList contactsList = _pendingContactsLists[event->getNodeID()];          
      ContactNotifier *mobility = check_and_cast<ContactNotifier*>(submodule);   
      mobility->initializeTrace( &contactsList );
      _pendingContactsLists.erase( event->getNodeID() ); 
    }
  }
  
  // Store the created module in our dynamic objects list.
	NodeFactoryItem *item = new NodeFactoryItem( module, event->getNodeID(), simTime() );
	m_createdItems.push_back( item );

  #ifdef __NODE_FACTORY_DEBUG__
  ev << fullPath() << ": Creating and storing dynamic object " << szModuleName << " at " << simTime() << " s" << endl;
  ev << fullPath() << ": Objects presently active are " << m_createdItems.size() << endl;
  ev << fullPath() << ": Total created objects are " << m_moduleCount << endl;
  #endif
  
  // Update the count of generated modules
  m_generateCount++;
}

/**
 * Search through the createdItems list and destroy the node in our dynamic collection
 * whose id matches that of the DestroyEvent.
 *
 * @todo This search could be more elegant.
 */
int NodeFactory::destroyNode( DestroyEvent *event )
{
  cModule *module;
	NodeFactoryItem *item;
  CREATED_ITEMS_VECTOR_TYPE::iterator iter;
  for( iter = m_createdItems.begin(); iter != m_createdItems.end(); iter++ )
	{
		item = (*iter);
    if ( item == NULL )
      continue;
		if ( item->getId() == event->getNodeID() )
		{          
		  m_totalLifetime += simTime() - item->getCreateTime();
			module = item->getModule();
      module->callFinish();
      module->deleteModule();
			delete item;
      m_createdItems.erase(iter);
      break;
		}
  }
  m_destroyedCount++;
  
  // Return the number of node which are active or yet to be activated. 
  return ( m_initializedCount - m_destroyedCount );
}

/**
 * Parse the XML trace file supplied at startup.
 *
 * @todo Add validation of creation, destroy and waypoint event times when parsed.
 *       Display warnings if times invalid.
 */
void NodeFactory::readXmlTrace()
{
  xmlTextReaderPtr reader = xmlReaderForFile( m_traceFile.c_str(), NULL, 0 );
  if ( reader == NULL )
  {
    error("Unable to open config file");
  }
  else
  {
    const xmlChar *namestr;
    int depth;
    string name;
    
    int ret = xmlTextReaderRead(reader); 
    while ( ret == 1)
    {
      namestr = xmlTextReaderConstName(reader);
      depth = xmlTextReaderDepth(reader);
      name = (const char *)namestr;
      std::transform(name.begin(),name.end(),name.begin(),::tolower);
      
      if ( xmlTextReaderNodeType(reader) == 1 )
      {
        if ( xmlTextReaderDepth(reader) == 0 )
        {
          if ( name == "mobility-trace" )
            readXmlMobilityTrace(reader);
          else if ( name == "contact-trace" )
            readXmlContactTrace(reader);
        }
      }
      ret = xmlTextReaderRead(reader);
 		}

		xmlCleanupParser();
		xmlFreeTextReader(reader);
  }
}

void NodeFactory::readXmlMobilityTrace( xmlTextReaderPtr reader )
{
  #ifdef __NODE_FACTORY_DEBUG__
  ev << fullPath() << ": Reading mobility trace file" << endl;
  #endif
 
  m_traceType = MobilityTrace;
  
  const xmlChar *namestr;
  const xmlChar *value;
  const char* valueStr;

  int curEventKind = NO_EVENT_KIND;
  TraceEvent *curEvent = NULL;
  WAYPOINT_EVENT curWaypointEvent;
  string curValueLbl;    
  string curSubValueLbl;
  string ccurSubValueLbl;
  int depth;
  double x,y;

  string name, cname;

  int ret = xmlTextReaderRead(reader); 
  while ( ret == 1)
  {
    namestr = xmlTextReaderConstName(reader);
    depth = xmlTextReaderDepth(reader);
    
    cname = (const char *)namestr;
    name = cname;
    std::transform(name.begin(),name.end(),name.begin(),::tolower);
    	        		  
    if ( xmlTextReaderNodeType(reader) == 1 )
    {
      if ( xmlTextReaderDepth(reader) == 1 )
      {
        // Handle element
        if ( name == "create" )
        {
          #ifdef __NODE_FACTORY_DEBUG__
          ev << "  CREATE" << endl;
          #endif
          curEvent = new CreateEvent();
          curEvent->setKind(CREATE_EVENT_KIND);
          curEventKind = CREATE_EVENT_KIND;
        }
        else if ( name == "destroy" )          
        {
          #ifdef __NODE_FACTORY_DEBUG__
          ev << "  DESTROY" << endl;
          #endif 
          curEvent = new DestroyEvent();
          curEvent->setKind(DESTROY_EVENT_KIND);
          curEventKind = DESTROY_EVENT_KIND;
        }
        else if ( name == "waypoint" )
        {
          #ifdef __NODE_FACTORY_DEBUG__
          ev << "  WAYPOINT" << endl;
          #endif
          curEventKind = WAYPOINT_EVENT_KIND;
          curWaypointEvent.id = -1;
          curWaypointEvent.time = 0.0;
          curWaypointEvent.x = 0.0;
          curWaypointEvent.y = 0.0;
          curWaypointEvent.speed = 0.0;
        }
      }
      else if ( xmlTextReaderDepth(reader) == 2 )
      {
        curValueLbl = name;          
      }
      else if ( xmlTextReaderDepth(reader) == 3 )
      {
        curSubValueLbl = name; 
        ccurSubValueLbl = cname;   
      }
    }
    if ( xmlTextReaderNodeType(reader) == 15 )
    {
      // Handle end element
      if ( depth == 1 )
      {
        if ( curEventKind == CREATE_EVENT_KIND )
          m_initializedCount++;
          
        if ( curEventKind == CREATE_EVENT_KIND || curEventKind == DESTROY_EVENT_KIND )
        {
          scheduleAt(curEvent->getTime(), curEvent);
          curEventKind = NO_EVENT_KIND;
        }
        else if ( curEventKind == WAYPOINT_EVENT_KIND )
        {
          waypointEventsList el;
          if(_pendingWaypointsLists.count(curWaypointEvent.id) != 0)
            el = _pendingWaypointsLists[curWaypointEvent.id];                            
          el.push_back(curWaypointEvent);
          _pendingWaypointsLists[curWaypointEvent.id] = el;                            
          curEventKind = NO_EVENT_KIND;
        }
      }
    }
    if ( xmlTextReaderNodeType(reader) == 3 )
    {
      // Handle a value. Use the curValueLbl and curSubValueLbl to
      // identify the property which the value corresponds to.
  	  if ( xmlTextReaderHasValue(reader) )
      {
  			value = xmlTextReaderConstValue(reader);
        valueStr = (const char *)value;       
        #ifdef __NODE_FACTORY_DEBUG__  
        ev << "    " << curValueLbl << " = " << value << endl;
        #endif
      }
                                      
      if ( curEventKind == CREATE_EVENT_KIND )
      {
        if ( curValueLbl == "time" )
          curEvent->setTime( atof(valueStr) );          
        else if ( curValueLbl == "nodeid" )
          curEvent->setNodeID( atoi(valueStr) );           
        else if ( curValueLbl == "type" )
          ((CreateEvent*)curEvent)->setType( valueStr );
        else if ( curValueLbl == "name" )
          ((CreateEvent*)curEvent)->setName( valueStr );
        else if ( curValueLbl == "prefix" )
          ((CreateEvent*)curEvent)->setPrefix( valueStr );
        else if ( curValueLbl == "icon" )
          ((CreateEvent*)curEvent)->setIconPath( valueStr );            
        else if ( curValueLbl == "mobilitymodel" )
          ((CreateEvent*)curEvent)->setMobilityModel( valueStr ); 
        else if ( curValueLbl == "location" && curSubValueLbl == "xpos" )
        {
          if ( _validateLocation( atof(valueStr), xCoordinate ) )
            ((CreateEvent*)curEvent)->setX( atof(valueStr) );                                
        }
        else if ( curValueLbl == "location" && curSubValueLbl == "ypos" )
        {
          if ( _validateLocation( atof(valueStr), yCoordinate ) )
            ((CreateEvent*)curEvent)->setY( atof(valueStr) );                                
        }
      }
      else if ( curEventKind == DESTROY_EVENT_KIND )
      {
        if ( curValueLbl == "time" )
          curEvent->setTime( atof(valueStr) );          
        else if ( curValueLbl == "nodeid" )
          curEvent->setNodeID( atoi(valueStr) );           
      }
      else if ( curEventKind == WAYPOINT_EVENT_KIND )
      {
        if ( curValueLbl == "time" )
          curWaypointEvent.time = atof(valueStr);          
        else if ( curValueLbl == "nodeid" )
          curWaypointEvent.id = atoi(valueStr);           
        else if ( curValueLbl == "speed" )
          curWaypointEvent.speed = atof(valueStr);
        else if ( curValueLbl == "destination" && curSubValueLbl == "xpos" )
        {
          if ( _validateLocation( atof(valueStr), xCoordinate ) )
            curWaypointEvent.x = atof(valueStr);                                
        }
        else if ( curValueLbl == "destination" && curSubValueLbl == "ypos" )
        {
          if ( _validateLocation( atof(valueStr), yCoordinate ) )
            curWaypointEvent.y = atof(valueStr);  
        }
      }
    } 	    		  
    
    ret = xmlTextReaderRead(reader);
  }
}

void NodeFactory::readXmlContactTrace( xmlTextReaderPtr reader )
{
  #ifdef __NODE_FACTORY_DEBUG__
  ev << fullPath() << ": Reading contact trace file" << endl;
  #endif

  m_traceType = ContactTrace;
  
  const xmlChar *namestr;
  const xmlChar *value;
  const char* valueStr;

  int curEventKind = NO_EVENT_KIND;
  TraceEvent *curEvent = NULL;
  CONTACT_EVENT curContactEvent;
  string curValueLbl;    
  string curSubValueLbl;
  string ccurSubValueLbl;
  int depth;
  double x,y;

  string name, cname;

  int ret = xmlTextReaderRead(reader); 
  while ( ret == 1)
  {
    namestr = xmlTextReaderConstName(reader);
    depth = xmlTextReaderDepth(reader);
    
    cname = (const char *)namestr;
    name = cname;
    std::transform(name.begin(),name.end(),name.begin(),::tolower);
    	        		  
    if ( xmlTextReaderNodeType(reader) == 1 )
    {
      if ( xmlTextReaderDepth(reader) == 1 )
      {
        // Handle element
        if ( name == "create" )
        {
          curEvent = new CreateEvent();
          curEvent->setKind(CREATE_EVENT_KIND);
          curEventKind = CREATE_EVENT_KIND;
        }
        else if ( name == "destroy" )          
        {
          curEvent = new DestroyEvent();
          curEvent->setKind(DESTROY_EVENT_KIND);
          curEventKind = DESTROY_EVENT_KIND;
        }
        else if ( name == "contact" )
        {
          curEventKind = CONTACT_EVENT_KIND;
          curContactEvent.type = Contact;
          curContactEvent.time = 0.0;
          curContactEvent.id = -1;
          curContactEvent.peerId = -1;
        }
        else if ( name == "break" )
        {
          curEventKind = CONTACT_EVENT_KIND;
          curContactEvent.type = Break;
          curContactEvent.time = 0.0;
          curContactEvent.id = -1;
          curContactEvent.peerId = -1;
        }
      }
      else if ( xmlTextReaderDepth(reader) == 2 )
      {
        curValueLbl = name;          
      }
      else if ( xmlTextReaderDepth(reader) == 3 )
      {
        curSubValueLbl = name; 
        ccurSubValueLbl = cname;   
      }
    }
    if ( xmlTextReaderNodeType(reader) == 15 )
    {
      // Handle end element      
      if ( depth == 1 )
      {
        if ( curEventKind == CREATE_EVENT_KIND )
          m_initializedCount++;
          
        if ( curEventKind == CREATE_EVENT_KIND || curEventKind == DESTROY_EVENT_KIND )
        {
          scheduleAt(curEvent->getTime(), curEvent);
          curEventKind = NO_EVENT_KIND;
        }
        else if ( curEventKind == CONTACT_EVENT_KIND )
        {
          contactEventsList el;
          if(_pendingContactsLists.count(curContactEvent.id) != 0)
            el = _pendingContactsLists[curContactEvent.id];                            
          el.push_back(curContactEvent);
          _pendingContactsLists[curContactEvent.id] = el;
          curEventKind = NO_EVENT_KIND;
        }
      }
    }
    if ( xmlTextReaderNodeType(reader) == 3 )
    {
      // Handle value. Use curValueLbl and curSubValueLbl to assign values
      // parsed to the correct attribute.
  	  if ( xmlTextReaderHasValue(reader) )
      {
  			value = xmlTextReaderConstValue(reader);
        valueStr = (const char *)value;         
      }
                                      
      if ( curEventKind == CREATE_EVENT_KIND )
      {
        if ( curValueLbl == "time" )
          curEvent->setTime( atof(valueStr) );          
        else if ( curValueLbl == "nodeid" )
          curEvent->setNodeID( atoi(valueStr) );           
        else if ( curValueLbl == "type" )
          ((CreateEvent*)curEvent)->setType( valueStr );
        else if ( curValueLbl == "name" )
          ((CreateEvent*)curEvent)->setName( valueStr );
        else if ( curValueLbl == "prefix" )
          ((CreateEvent*)curEvent)->setPrefix( valueStr );
        else if ( curValueLbl == "icon" )
          ((CreateEvent*)curEvent)->setIconPath( valueStr );            
        else if ( curValueLbl == "location" && curSubValueLbl == "xpos" )
        {
          if ( _validateLocation( atof(valueStr), xCoordinate ) )
            ((CreateEvent*)curEvent)->setX( atof(valueStr) );                                
        }
        else if ( curValueLbl == "location" && curSubValueLbl == "ypos" )
        {
          if ( _validateLocation( atof(valueStr), yCoordinate ) )
            ((CreateEvent*)curEvent)->setY( atof(valueStr) );                                
        } 
      }
      else if ( curEventKind == DESTROY_EVENT_KIND )
      {
        if ( curValueLbl == "time" )
          curEvent->setTime( atof(valueStr) );          
        else if ( curValueLbl == "nodeid" )
          curEvent->setNodeID( atoi(valueStr) );           
      }
      else if ( curEventKind == CONTACT_EVENT_KIND )
      {
        if ( curValueLbl == "time" )
          curContactEvent.time = atof(valueStr);          
        else if ( curValueLbl == "nodeid" )
          curContactEvent.id = atoi(valueStr);           
        else if ( curValueLbl == "peerid" )
          curContactEvent.peerId = atoi(valueStr);
      }
    } 	
    		      
    ret = xmlTextReaderRead(reader);
  }  
}


/**
 * @todo Add the node id and location for easier debugging of traces.
 */
bool NodeFactory::_validateLocation( double coordinate, COORD_TYPE ct )
{
  double refval = 0.0;
  if ( ct == xCoordinate )
    refval = m_scenarioSizeX;
  else if ( ct == yCoordinate )
    refval = m_scenarioSizeY;
     
  if ( refval == 0 || ( coordinate >= 0.0 && coordinate <= refval ) )
    return true;
  else
  {
    error("Location of node out of bounds");
    return false;
  }
}

