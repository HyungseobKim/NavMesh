Automatic generation of navigation meshes and pathfinding.
It uses a polygon triangulation algorithm to generate navigation meshes and the A* algorithm to find a path.
You can see the detailed explanation here: https://hyungseob.bitbucket.io/Source/SoloProjects/NavMesh.html

Used C++, SDL2, OpenGL, and ImGui.
All source codes are in the NavMesh folder, and the executable is in the Demo folder with libraries.

How to use:
Click the left mouse to set the path's starting point.
Click the right mouse to set the destination of the path.
You can change the weight of the heuristic for A*. You can toggle path smoothing or show all nodes. You may change colors with the color editor.

How to edit the geography: 
Click Edit mode in UI. To move walls, drag a red box at the center of a wall. To change the shape of the wall, drag a red box on the vertices of a wall.
