# Navigation Mesh
[Main Menu](../README.md)
<br /><br />
[Prev Chapter - Partitioning](Triangulation.md)

## Pathfinding on navigation meshes
The algorithm that I used to do pathfinding on navigation meshes is A*.
It is optimal and very fast with proper heuristic and data structure.
Since it works well with graphs, it works with navigation meshes as well.

### A*
The common implementation of A* is very similar to the implementation of Dijkstra's algorithm using priority queue (a.k.a Uniform Cost search).
It looks like heuristic-added Dijkstra's algorithm. However, it is not true always.
To understand this you need to understand two properties of heuristic.

#### Heuristic
Heuristic is a reasonable guess. It may not be exactly same to the actual value,
but as long as it is under the admissible range and has constant tendency, we can improve the search using this information.
One example of heuristics in pathfinding is the distance between a point to the destination.
The actual path from a point to the destination may be longer than the distance between them,
but it is not overrating and is constant for all points.
And they are the two properties of heuristic; admissibility and consistency.

##### Admissibility
A heuristic is admissible when it is less or equal than the actual value.
If it is greater than the actual value, it does not guarantee optimality.
However, if you can accept some inaccuracy, you may want to increase heuristic intentionally, because it will boost up the search even more.
If the space is not complicated, it can help to reduce the inaccuracy.
You can check this in my demo and code, which is "weight".

##### Consistency
Consistency is a stronger property than admissibility, so if a heuristic is consistent, it is admissible.
Imagine paths between three points A, B, and C. There are two ways that can go to B from A.
One is the going directly, and the other one is going to C and then going to B.
Let's say $h(x)$ is a heuristic at point $x$ to the destination and $v(x, y)$ is an actual value between $x$ and $y$.
Then, the heuristic is consistent if $h(A)$ is less or equal than $v(A, C)$ + $h(C)$.

If a heuristic is admissible but not consistent, it still guarantees optimality, but it may visit nodes more than once.
In worst case, it needs to check all possible paths between starting point and destination.
It means, it is very different to Dijkstra's algorithm.
Rather, it is a brute-force algorithm with bound optimization.
Luckily, the distance between two points is a consistent heuristic, so we can use it for pathfindings.

### A* on navigation mesh
There are not many things to change to use A* on navigation meshes.
The only thing you need to decide is the standard point of each mesh.
One is the center point of mesh. With this one, you can simply do pathfinding with all center points.
The actual path will look like going to the center of a mesh first,
then moving along the center point of adjacent meshes, and moving to the destination point.
The other point you can use is the center point of each edge, which is I used and will explain here.

You can see my implementation here: [Astar.h](../NavMesh/Astar.h), [Astar.cpp](../NavMesh/Astar.cpp)

<img align="center" src="/Description/Images/PathFindingExample.png">

[Back to the top](#navigation-mesh)
