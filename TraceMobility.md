Mobility component for trace based mobility in OMNeT++ simulations.

# Introduction #

The TraceMobility component is used in conjunction with the NodeFactory object to implement the trace mobility model in OppoSim. Trace mobility uses externally generated traces or measurement results to control a nodes movement. The module is initialized by the NodeFactory module at a certain location upon creation. The factory then supplies the module
with its waypoint list. The mobility module is then autonomous for the duration of its lifetime.


# Details #

TraceMobility inherits from the BasicMobility class of the Mobility Framework and can thus be substituted with minimum effort into existing simulations which use Mobility Framework (http://mobility-fw.sourceforge.net) for mobility support.