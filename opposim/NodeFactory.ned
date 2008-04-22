
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

//
// Node factory module
//
// The node factory instantiates nodes dynamically during a simulation run from definitions
// in a XML trace file. A mobility trace defines create, destroy and waypoint events for
// a collection of nodes. Such a trace is created using an external mobility generator, e.g.
// UrbanMobility from the MobiTrace toolkit, or UDel (http://udelmodels.eecis.udel.edu).
// Contact traces can additionally be used. Such traces can e.g. be created from contact 
// measurements conducted with mobile devices.
//
// @author  Kristjan V. Jonsson
// @author  Olafur R. Helgason
// @version 1.0 
//
simple NodeFactory
  parameters:
    scenarioSizeX: numeric,
    scenarioSizeY: numeric,
    traceFile: string;
endsimple

