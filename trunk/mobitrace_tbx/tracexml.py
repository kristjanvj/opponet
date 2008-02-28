ROOT_NODE_NAME = "mobility-trace"

CREATE_NODE_NAME = "create"
DESTROY_NODE_NAME = "destroy"
WAYPOINT_NODE_NAME = "waypoint"

ID_LABEL = "nodeid"
TIME_LABEL = "time"
TYPE_LABEL = "type"
PREFIX_LABEL = "prefix"
ICON_LABEL = "icon"
SPEED_LABEL = "speed"
LOCATION_LABEL = "location"
DEST_LABEL = "destination"
X_LABEL = "xpos"
Y_LABEL = "ypos"
MOBILITY_MODEL_LABEL = "mobilityModel"
MOBILITY_MODEL = "TraceMobility"

DEFAULT_TYPE = "SimpleNode"
DEFAULT_PREFIX = "node"
DEFAULT_ICON = "device/palm_s"


import xml.dom.minidom
import xml.dom.ext



def showDocument(doc):
  """
  Display the generated XML document on the console
  """
  xml.dom.ext.PrettyPrint(doc);
  
  
def saveDocument(doc,filename):
  """
  Save the generated XML document to file
  """
  file_object = open(filename,"w");
  xml.dom.ext.PrettyPrint(doc,file_object);
  file_object.close();
  
  
def createRootNode():
  """
  Create the root XML document node. Returns the XML document and the root element.
  """  doc = xml.dom.minidom.Document();
  root_element = doc.createElement(ROOT_NODE_NAME);
  doc.appendChild(root_element);
  return doc,root_element;
  

def addCreateNode(doc,root,id,time,x,y,type=DEFAULT_TYPE,prefix=DEFAULT_PREFIX,icon=DEFAULT_ICON):
  """
  Add a create command node
  """  
  create_element = doc.createElement(CREATE_NODE_NAME);
  addValueNode(doc,create_element,ID_LABEL,id);
  addValueNode(doc,create_element,TIME_LABEL,time);
  addValueNode(doc,create_element,TYPE_LABEL,type);
  addValueNode(doc,create_element,PREFIX_LABEL,prefix);
  addValueNode(doc,create_element,ICON_LABEL,icon);      
  addValueNode(doc,create_element,MOBILITY_MODEL_LABEL,MOBILITY_MODEL);
  addLocationNode(doc,create_element,x,y,LOCATION_LABEL);
  root.appendChild(create_element);
  
  
def addDestroyNode(doc,root,id,time):
  """
  Add a destroy command node
  """
  destroy_element = doc.createElement(DESTROY_NODE_NAME);
  addValueNode(doc,destroy_element,ID_LABEL,id);
  addValueNode(doc,destroy_element,TIME_LABEL,time);  
  root.appendChild(destroy_element);
  
  
def addWaypointNode(doc,root,id,time,x,y,speed):
  """
  Add a waypoint command node
  """  
  #print "waypoint",id,time,x,y,speed;
  waypoint_element = doc.createElement(WAYPOINT_NODE_NAME);
  addValueNode(doc,waypoint_element,ID_LABEL,id);
  addValueNode(doc,waypoint_element,TIME_LABEL,time);
  addLocationNode(doc,waypoint_element,x,y,DEST_LABEL);  
  addValueNode(doc,waypoint_element,SPEED_LABEL,speed);
  root.appendChild(waypoint_element);
  
  
def addValueNode(doc,parent,label,value):
  """
  Add a value element to a node.
  """
  id_element = doc.createElement(label);
  val_element = doc.createTextNode(str(value))
  id_element.appendChild(val_element);
  parent.appendChild(id_element); 
  
  
def addLocationNode(doc,parent,x,y,label):
  """
  Add a location element to a node
  """  
  loc_element = doc.createElement(label);
  addValueNode(doc,loc_element,X_LABEL,x);
  addValueNode(doc,loc_element,Y_LABEL,y);
  parent.appendChild(loc_element);
