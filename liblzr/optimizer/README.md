The Frame Optimizer
===================

This optimizer is mainly concerned with the order in which points get drawn by the laser. It is intended to correct blanking stupidities, so that the engine or GUI can stay focused on graphics, rather than the mechanics of a laser projector. To that end, this program will not alter or delete points **unless they are blanked**. The optimizer can do whatever it wants with blanked points.

The mechanism works by first splitting the frame into seperate `path` segments. A point is considered the start (or end) of a `path` when it meets one (or both) of the following criteria:

- Blanking is toggled (or the color is set to black)
- The point creates a corner of less than 135 degrees (45 deviation from a straight line)

Each one of these "path boundry" points is considered an opportunity for the laser to leave, and draw something else. A simply cost function is used to make this descision. Most of the time, when drawing contiguous shapes, that cost function will overwhelmingly choose to stay until the shape is finished, rather than leave in the middle.

The cost function looks at two factors when evaluating the appeal of a new `path`:

- The distance between the laser's current location, and the start of the `path` segment in question. *Note: both ends of a `path` are considered starting points*
- The difference between the exit angle of the current path, and the entrance angle of the next `path`.
- If a blank jump is necessary, the angle differences between the current `path`, the proposed blank jump, and the next `path`

The distance clause is fairly obvious (classic travelling salesman problem). The angle clause is intended to keep the laser moving in roughly the same direction it already was. For instance, when approaching the perfect crossing of two lines, the laser will prefer to draw two straight lines, rather than make two 90 degree turns. Or, if drawing seperate horizontal lines, it will prefer to scan lines in alternating directions, rather than a raster zig-zag.

Incoming blanked points are tossed and regenerated. This is neccessary to re-order the paths, and helps normalize the blanking jumps. There is a simple setting for the number of "anchor" points to place when entering and exiting a jump.
