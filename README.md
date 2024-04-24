# Navigation Mesh
Automatic generation of navigation meshes and pathfinding on the meshes.<br />
It uses a polygon triangulation algorithm to generate navigation meshes and the A* algorithm to find a path.<br />

<img align="center" src="/Description/Images/Screenshot1.png">

Used C++, SDL2, OpenGL, and ImGui.<br />
All source codes are in the NavMesh folder, and the executable is in the Demo folder with libraries.

### How to use the demo
Use left/right mouse buttons to set start/end points of a path.<br />
You can change the weight of the heuristic for A* using "Weight" bar.<br />
Use "Show all nodes considered" option to see how it affects the pathfinding.<br />
"Path smoothing" option shows a more narrowed path.<br />
You may change colors using the color editor.<br />

### How to edit the geography in the demo
Click "Edit mode" in the UI.<br />
To move walls, drag a red box at the center of a wall.<br />
To change the shape of the wall, drag a red box on the vertices of a wall.<br />

## Algorithms
Generating navigation meshes uses Polygon Triangulation algorithm, which has two parts.
1. Partition the polygon into y-monotone pieces.
2. Triangulate each piece.

After triangulation has done, there is the pathfinding section.
1. Find the path between two given points on the polygon using A*.
2. Smooth the path using Funnel Algorithm.

You can read the detailed explanation of the algorithms below.

### Partitioning

[Description](Description/Partitioning.md)

### Triangulation

[Description](Description/Triangulation.md)

### Pathfinding

[Description](Description/Pathfinding.md)