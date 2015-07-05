Simple generator for tracefiles using the random waypoint mobility model

# Introduction #

rwpy is a simple implementation of the Random Waypoint
mobility model (RWP), which generates mobility traces for
a ﬁxed number _n_ of nodes, given a running time _tr_ . It is a part of the MobiTrace toolbox of the Opponet project. This application is essentially a proof-of-concept, as RWP mobility can easily be implemented in a MF-derived mobility module.
One such module, RandomWaypointMobility, is actually provided with the [Opposim](Opposim.md) demonstration. A scripting approach to this simple mobility model is thus
not strictly necessary. Like UrbanMobility, rwpy generates
a XML [traceﬁle](TraceFileFormat.md) of create, destroy and waypoint events.

## Usage ##

Please refer to the general notes on the MobiTrace toolbox page. The script takes the following parameters:
```
  rwpy {options} filename
    where options are:
       -h|--help      Display help text
       -n|--nodes     Number of generated nodes (default 10)
       -t|--time      Scenario time (default 3600 sec)
       -x|--xmax      Scenario width (default 1000 m)
       -y|--ymax      Scenario height (default 1000 m)
       -v|--velocity  Mean velocity (m/s)
       -s|--vel_sd    Standard deviation of the velocity 
       -p|--pause     Mean pause time (sec)
       -d|--pause_sd  Standard deviation of the pause times
```

# The random waypoint mobility model #

Random waypoint is a simple mobility model, but nevertheless one which has been widely used in mobility research. It works as follows:
  1. A node is initialized at a uniformly distributed location within a fixed boundary.
  1. The node picks a new waypoint after an optional pause, which is also selected from a random distribution.
  1. The node navigates towards this point, interpolating a new position at each discrete update.
  1. Upon arriving a the new location, the node selects a new waypoint, optionally waiting before starting its journey towards the new location.
This process is repeated for the duration of the simulation.

The accuracy of the random waypoint model has recently been called into
question in a number of articles, e.g. (NAV04,BET03,YOO03). It is nevertheless a de-facto reference mobility model and well worth exploring for some applications
Navidi and Camp provide a number of guidelines (NAV04) for initializing the nodes to
be consistent with the expected steady state distribution, which increases the accuracy of the simulation. This implemenation follows those guidelines, although the initial paused state distribution suggested is not presently implemented.

The velocity and pause times in rwpy are drawn from a truncated normal distribution.
The zero speed problem is addressed using a minimum speed, which is presently
fixed at 0.5 m/s. Otherwise, a number of stuck nodes (ones with zero or very
low velocity) is expected during a prolonged simulation.

### References ###

**NAV04** Navidi, W. & Camp, T. Stationary Distributions for the Random Waypoint Mobility Model. IEEE Transactions on Mobile Computing, 2004, 3, pp. 99-108

**BET03** Bettstetter, C.; Resta, G. & Santi, P. The Node Distribution of the Random Waypoint Mobility Model for Wireless Ad Hoc Networks. IEEE Transactions on Mobile Computing, 2003, 2, pp. 257-269

**YOO03** Yoon, J.; Liu, M. & Noble, B. Random Waypoint Considered Harmful. Proceedings of INFOCOM. IEEE, 2003