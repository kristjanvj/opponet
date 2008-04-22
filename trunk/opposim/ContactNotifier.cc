
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
  
#include "ContactNotifier.h"
#include <FWMath.h>

Define_Module(ContactNotifier);

void ContactNotifier::initialize(int stage)
{
  BasicMobility::initialize(stage);

  if (stage == 0)
  {        
    ev << fullPath() << ": initializing ContactNotifier module." << endl;  	
    
    // Create a contact event and register with the blackboard
    contactEvent = new ContactEvent("contact");  	    	
    hostContactCategory = bb->getCategory(&hostContact);    	
  }
  else if ( stage == 1 )
  {
    //
  }
}

void ContactNotifier::finish()
{
  cancelAndDelete(contactEvent);
}

void ContactNotifier::handleMessage(cMessage *msg)
{
  if ( msg == contactEvent )
  {
    // Handle the contact event
    notifyContact();
  }
  else 
  {  
     BasicMobility::handleMessage(msg);
  }        
}


void ContactNotifier::initializeTrace( const contactEventsList *eventList )
{
  Enter_Method("initializeTrace");
     
  ev << fullPath() << ": Initializing contact and break events" << endl;
  
  if ( eventList == NULL )
    return;
    
  // Copy the given event list to the internal list.
  m_eventList.resize(eventList->size());
  std::copy(eventList->begin(),eventList->end(),m_eventList.begin());

  CONTACT_EVENT ce;
  
  #ifdef __CONTACT_NOTIFIER_DEBUG__  
  contactEventsList::iterator i = m_eventList.begin();
  for(; i != m_eventList.end();i++)
  {
    ce = (*i);
    ev << "event: id=" << ce.id << " time=" << ce.time << " peer=" << ce.peerId;
    ev << " ct=";
    if ( ce.type == Contact )
      ev << "Contact";
    else if ( ce.type == Break )
      ev << "Break";
    ev << endl;
  }  
  #endif
      
  if ( m_eventList.size() > 0 )
  {
    // Schedule the first contact event in the list
    ce = m_eventList.front();
    m_eventList.pop_front();   
    
    contactEvent->setId(ce.id);
    contactEvent->setPeerId(ce.peerId);
    contactEvent->setType(ce.type);
    scheduleAt(simTime()+ce.time,contactEvent);
  }   
}


void ContactNotifier::notifyContact()
{
  #ifdef __CONTACT_NOTIFIER_DEBUG__
  ev << fullPath() << ": Notifying contact. "
                   << " Id= " << contactEvent->getId() << " PeerId=" << contactEvent->getPeerId()
                   << " Type=" << contactEvent->getType() << endl;
  #endif
    
  hostContact.id = contactEvent->getId();
  hostContact.peerId = contactEvent->getPeerId();
  hostContact.type = contactEvent->getType();               
  bb->publishBBItem(hostContactCategory, &hostContact, hostId);                   

  if ( m_eventList.size() > 0 )
  {
    // Scheduling the next contact in the list
    CONTACT_EVENT ce = m_eventList.front();
    m_eventList.pop_front();   
    
    contactEvent->setId(ce.id);
    contactEvent->setPeerId(ce.peerId);
    contactEvent->setType(ce.type);
    #ifdef __CONTACT_NOTIFIER_DEBUG__
    ev << fullPath() << ": Scheduling next contact event @ " << simTime() + ce.time << endl;
    #endif
    scheduleAt(simTime()+ce.time,contactEvent);
  }   
}

