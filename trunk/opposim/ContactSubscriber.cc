
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

#include "ContactSubscriber.h"

// The module class needs to be registered with OMNeT++
Define_Module(ContactSubscriber);


void ContactSubscriber::initialize(int stage)
{
	BasicModule::initialize(stage);
	
	if ( stage == 0 )
	{
    ev << fullPath() << ": Initializing ContactSubscriber module" << endl;

	  // Subscribe to a host contact notification from blackboard
	  cModule *parent = parentModule();
	  if ( parent == NULL )
      error("Parent not found");
	  HostContact contact;
	  catHostContact = bb->subscribe(this, &contact, parent->id());	
	}
}

void ContactSubscriber::receiveBBItem(int category, const BBItem *details, int scopeModuleId)
{
	Enter_Method_Silent();
	
	BasicModule::receiveBBItem(category, details, scopeModuleId);
  if( category == catHostContact ) 
  {
  	// Get the move information and update the display
  	const HostContact *contact = static_cast<const HostContact *>(details);
  	ev << fullPath() << ": Receving BB update: Contact: " << "id=" << contact->id 
  	                 << ", peer=" << contact->peerId << ", type=" << contact->type << endl;  	
  } 
}



