
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

#include "NodeFactoryItem.h"

NodeFactoryItem::NodeFactoryItem()
{
  m_id = -1;
	m_module = NULL;
	m_tCreateTime = 0.0;
}

NodeFactoryItem::NodeFactoryItem( cModule *module, int id, simtime_t createTime )
{
	NodeFactoryItem::NodeFactoryItem();
	m_module = module;
	m_id = id;
	m_tCreateTime = createTime;
}

