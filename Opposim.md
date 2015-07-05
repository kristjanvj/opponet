A simple demonstration simulator for the Opponet project

# Introduction #

Opposim is a very simple demonstration for the [OMNeT++](OMNETPP.md) compontents of the Opposim project. The simulation involves only mobility or contact functionality - expanding the simulation to do something useful is completely up to the users.

# Building opposim #

Download, install and build the latest [OMNeT++](OMNETPP.md) and [Mobility Framework](MobilityFramework.md) releases. See the installation and build instructions for the projects for further details. Opposim was developed on a Linux platform (Fedora 6 to be exact) but should build on other OSes supported by OMNeT++. Both OMNeT++ and MF are available for Windows platforms so porting to that platform should prove to be straight forward. We assume that OMNeT++ and MF are built and installed in a subdirectory of the users home directory.

Get the latest version of opposim from the downloads area or via anonymous subversion access:
```
svn checkout http://opponet.googlecode.com/svn/trunk/ opponet-read-only
```
Unpack the release of opposim into a subdirectory of the users home directory. The release contains only sources and some test files so we have to start by building an executable. Two bash scripts are provided to create GUI or console versions. cd to your opposim directory and

  * enter **./mkgui** for creating a makefile for a GUI version
  * enter **./mkcmd** for creating a makefile for a console version

Type **make** to build the executable, which will be named **opposim**.

# Running opposim #

The XML trace file to be processed by the factory must be specified in an ini file under the _Parameters_ section, e.g.
```
  square.factory.traceFile = "mobtrace1.xml";  # For trace mobility
```

Two ini files are provided to run the opposim simulation with the provided trace files:

  * trace.ini specifies a mobility trace XML file to be loaded into the node factory on startup. Enter **./opposim -f trace.ini** to run.
  * contact.ini specifies a contact trace XML file to be loaded into the node factory on startup. Enter **./opposim -f contact.ini** to run.

The common settings are in both cases to be found in the standard omnetpp.ini intialisation file.

# The elements of a Opposim simulation #

The NodeFactory component is the only component instantiated at startup, apart from the ChannelControl module required for [Mobility Framework](MobilityFramework.md) compatibility.

## Mobility trace demonstration ##

The mobility trace simulation instantiates nodes of the type SimpleNode, which is a very basic OMNeT++ compound module, containing only a generic Navigator module and a Blackboard. The generic navigator can be assigned the class names of any BasicMobility-derived mobility module at startup, either in the simulation ini file or the XML trace file as done in this case.

The demo simulation, defined in trace.ini, spawns three nodes during the simulation lifetime. Two use a trace mobility model where each waypoint is explicitly scripted in the XML file. The third uses a random waypoint mobility model and demonstrates how mobility models may be mixed within the simulation.

## Contact trace demonstration ##

The contact trace simulation instantiates nodes of the type SimpleContactNode. These are identical to the SimpleNode compound module, apart from a ContactSubscriber module. The ContactNotifier is instantiated in this case in the navigator role through the definition in the XML trace file.