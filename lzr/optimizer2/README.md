1. Decimate: Remove all pre-existing interpolation and blanks
2. Path Discovery: Splits the frame into lit segments (either by blanks or hard angles)
3. Traversal Solve: Solves for the minimum time/energy traversal of the paths, accounting for cycles that can be entered from any of their points
4. Path Reassembly: Build a new frame with the correct path traversal order
5. Interpolate: Add extra points wherever it will make the galvos happy.