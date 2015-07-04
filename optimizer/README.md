The Frame Optimizer
===================

This optimizer is mainly concerned with the order in which points get drawn by the laser. It will not alter or delete duplicate sets of points **unless they are blanked**. The optimizer can do whatever it wants with blanked points.

The mechanism works by first splitting the frame into seperate `path` segments. A point is considered the start (or end) of a `path` when it meets one (or both) of the following criteria:

- Blanking is toggled (or the color is set to black)

or

- The point creates a corner of less than 135 degrees (45 deviation from a straight line)

Each one of these "path boundry" points is considered an opportunity for the laser to leave, and draw something else. A simply cost function is used to make this descision. Most of the time, when drawing contiguous shapes, that cost function will overwhelmingly choose to stay until the shape is finished, rather than leave in the middle.

Incoming blanked points are tossed, and regenerated with an easing function of your choice. This way, the laser can perform acceleration curves during blanking jumps, in an effort to move as fast as possible without sacrificing endpoint accuracy. By default, this system uses a quadratic function.

The cost function looks at two factors when evaluating the appeal of a new `path`:

- The distance between the laser's current location, and the start of the `path` segment in question. *Note: both ends of a `path` are considered starting points*
- The difference between the exit angle of the current path, and the entrance angle of the next `path`.

The distance clause is fairly obvious (classic travelling salesman problem). The angle clause is intended to keep the laser moving in roughly the same direction it already was. For instance, when approaching the perfect crossing of two lines, the laser will prefer to draw two straight lines, rather than make two 90 degree turns.
