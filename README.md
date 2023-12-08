# Navigation Mesh
Automatic generation of navigation meshes and pathfinding.<br />
It uses a polygon triangulation algorithm to generate navigation meshes and the A* algorithm to find a path.<br />
You can see the detailed explanation here: https://hyungseob.bitbucket.io/Source/SoloProjects/NavMesh.html


Used C++, SDL2, OpenGL, and ImGui.<br />
All source codes are in the NavMesh folder, and the executable is in the Demo folder with libraries.

How to use:<br />
Click the left mouse to set the path's starting point.<br />
Click the right mouse to set the destination of the path.<br />
You can change the weight of the heuristic for A*.<br />
You can toggle path smoothing or show all nodes.<br />
You may change colors with the color editor.<br />

How to edit the geography: <br />
Click Edit mode in UI.<br />
To move walls, drag a red box at the center of a wall.<br />
To change the shape of the wall, drag a red box on the vertices of a wall.
