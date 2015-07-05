A simple subscriber to contact information in the opposim demo

# Introduction #

ContactSubscriber is a simple demonstration subscriber used in the [Opposim](Opposim.md) demo. It is included in the SimpleContactNode module and subscribes, through the Blackboard, to contact notifications issued by the ContactNotifier.

This demo implementation simply outputs contact events to the console, e.g.

> square.my laptop 1.csubscribe: Receving BB update: Contact: id=1, peer=2, type=0

> square.my laptop 1.csubscribe: Receving BB update: Contact: id=1, peer=2, type=1

> square.my laptop 1.csubscribe: Receving BB update: Contact: id=1, peer=3, type=0

> square.my laptop 1.csubscribe: Receving BB update: Contact: id=1, peer=3, type=1

More advanced implementations would presumably put this information to better use. This task is however left to our users.