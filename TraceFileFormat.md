Description of trace file formats used in the opponet project.

# Mobility trace format #

A mobility trace includes a series of node create, destroy and
waypoint events, providing an arbitrarily ﬁne-grained con-
trol over the lifetime and mobility of a population of nodes.
A mobility trace can be created by mobility generators or constructed from measurements, e.g. tracking the movement of a population of nodes equipped with GPS locators.

  * _Create_ events specify the arrival of a node with a given id into the scenario at a given time and location. A node type designation enables nodes of various types and capabilities to be instantiated. A simulation could e.g. include pedestrians, vehicles and access points, all diﬀering in capabilities and resources. A mobility model designation can optionally be included. The default is TraceMobility, but any BasicMobility-derived mobility modules can be speciﬁed here, allowing mobility models to be mixed in the same simulation. A optional name can be used to specify a string for display in a GUI environment or output traces. Similarily, an icon can be speciﬁed for easier visualization of diﬀerent node roles or classes. Nodes are created at rest, and a waypoint command is required for a TraceMobility enabled node to start its journey. The parameters in eﬀect for nodes with other mobility models, e.g. random waypoint (RWP), take eﬀect at create time and must be speciﬁed in the initialization ﬁle for the scenario.

  * _Waypoint_ events specify a waypoint change, i.e. the next destination on a nodes journey. The waypoint event speciﬁes a node id and activation time. A node moves with a ﬁxed speed on the leg between the current and destination waypoints. Note that pauses can be implemented by letting the journey between two successive waypoints be shorter in duration than the diﬀerence in their activation times. Waypoint events are only required when nodes change direction or velocity. Relatively compact traceﬁles are thus feasible, even with a large number of generated nodes.

  * _Destroy_ events specify the departure of a node with a given id from the scenario at a given time. The node is destroyed regardless of any remaining waypoint events.

Mobility traces are in **XML** format and are of the following form:

```
<mobility-trace>
  <create>
    <time>0.0</time>
    <nodeid>1</nodeid>
    <type>SimpleNode</type>
    <name>mobile node 1</name>
    <icon>device/palm_s</icon>
    <mobilityModel>TraceMobility</mobilityModel>
    <location>
      <xpos>0.0</xpos>
      <ypos>0.0</ypos>
    </location>
  </create>
  <destroy>
    <time>60.0</time>
    <nodeid>1</nodeid>
  </destroy>
  <waypoint>
    <nodeid>1</nodeid>
    <time>10.0</time>
    <destination>
      <xpos>10.0</xpos>
      <ypos>10.0</ypos>
    </destination>
    <speed>2.0</speed>
  </waypoint>
</mobility-trace>
```

A single node of type SimpleNode is created at t = 0.0 at (x, y) = (0.0, 0.0) and starts moving towards (x, y) = (10.0, 10.0) at t = 10.0 s with v = 2.0 m/s. It then remains stationary at the ﬁnal destination until it is destroyed at t = 60.0 s.


# Contact trace format #

A contact trace contains peer contact establishment and
break events for a population of nodes. Such a trace provides
one further level of abstraction for a protocol simulator, in
that it does not associate any location (nor mobility) with
the nodes. Traces of this kind can for example be generated
in opportunistic networking experiments or by offline generation.

Contact traces cannot be used concurrently
with any mobility model, since contact information cannot,
in general, be assumed to have any associated location data.

A contact trace consists of sequences of contact and break
events:

  * _Contact_ events signify a discovery event by a node, speciﬁed by a nodeid, of a peer with id peerid at a given time.

  * _Break_ events signify a broken or lost contact between a node, speciﬁed by a nodeid, and a peer with id peerid at a given time.

_Create_ and _destroy_ events can optionally be employed
to instantiate and remove nodes from the simulation. If
omitted, the nodes are simply created when ﬁrst encountered
in the trace ﬁle.


Contact traces are in **XML** format and are of the following form:

```
<contact-trace>
  <create>
    <time>0.0</time>
    <nodeid>1</nodeid>
    <type>SimpleNode</type>
  </create>
  <create>
    <time>0.0</time>
    <nodeid>2</nodeid>
    <type>SimpleNode</type>
  </create>
  <contact>
    <time>5.0</time>
    <nodeid>1</nodeid>
    <peerid>2</peerid>
  </contact>
  <break>
    <time>10.0</time>
    <nodeid>1</nodeid>
    <peerid>2</peerid>
  </break>
</contact-trace>
```

Two nodes, Node 1 and Node 2, are created at startup in this example. Node 1 discovers its peer Node 2 at t = 5.0 s. The contact is broken at t = 10.0 s.