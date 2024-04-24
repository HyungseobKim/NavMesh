# Navigation Mesh
[Main Menu](../README.md)
<br /><br />
[Prev Chapter - Partitioning](Partitioning.md)
<br /><br />
[Next Chapter - Pathfinding](Pathfinding.md)

## Polygon Triangulation
Here we will discuss the algorithm that triangulates monotone pieces, so we can generate navigation meshes.
Since we already divided the polygon into a simpler shape, this algorithm is not so complicated actually.
Our stratgey is similar to the previous section; scanning each piece from the top to the bottom, vertex by vertex.

### Label vertices
Similar to partitioning, we need a preparation before the actual algorithm, which is labeling vertices.
What different to partitioning is that we have only two types of vertex this time; left and right.
When you draw a line from the highest vertex to the lowest vertex of the piece along the edges, you have two chains.
The vertices on the left chain are left vertices and the right chain are right vertices.
You do not need to care the top vertex and the bottom vertex.

### Triangulation
We will use a greedy algorithm that looks at each vertex one by one in order of decreasing y-coordinate.
Each time we handle a vertex, we will add new diagonals as much as possible or save candidates that can have more diagonals.
We will use a stack to save those candidate, so we can ensure that the top element is always the lowest vertex.
Lastly, when we handle each vertex, there can be two scenarios.

Figure 1 shows the first case which is when the current vertex and the vertex at the top of the stack are on the same chain.
It means the vertex at the top of the stack is already connected to the current vertex, so discard it.
For the rest of vertices in the stack, check if a diagonal can be added to the current vertex.
If it is possible, connect and check the next vertex in the stack.
Repeat this untill you meet a vertex that can not be connected to the current vertex.

<img align="left" width="300" src="/Description/Images/SameChainExample.png">
<img align="center" width="300" src="/Description/Images/HandleSameChainExample.png">

Figure 2 shows the result of handling the current vertex from Figure 1.
We could connect three vertices on the stack from the current vertex, so they are popped from the stack.
However, the fourth vertex could not be connected to the current vertex, so this is where we stop.
But before going to the next vertex, there is one thing we need to do, which is pushing vertices to the stack.
First, insert the vertex which is popped lastly (the last one we could connect to the current vertex, e.g. the third vertex in Figure 2).
If you see Figure 2, you can notice that the other popped vertices can not be connected to any other vertices.
Second, insert the current vertex. It has a chance to get more diagonals in the future.

The other scenario is when the current vertex and the top vertex of the stack are on the different chain. Figure 3 shows this case.
Since they are on the different chain and this polygon is y-monotone,
we can add a new diagonal to all vertices in the stack except the last one which is the highest vertex that is already connected to the current vertex.

<img align="left" width="300" src="/Description/Images/DifferentChainExample.png">
<img align="center" width="300" src="/Description/Images/HandleDifferentChainExample.png">

The result of handling Figure 3 is Figure 4.
Similar to the previous scenario, we need to insert two vertices into the stack; the current vertex and the vertex previously on top of the stack.
They have a chance to get more diagonals in the future, while other vertices do not have.

### Naviagation Mesh Generation
We just have looked through how the triangulation alogrithm works, but you might notice that we just added diagonals,
which doesn't give us nav-mesh triangles magically.
The positive thing is that since this algorithm is simple and clear, we can easily achieve it.

Let's take a look at Figure 2 and 4 again.
You can notice that each time we add a new diagonal, it creates a new triangle.

In the first scenario, three vertices of a new triangle are always the ***current*** vertex, the ***diagonal's the other*** vertex,
and the ***previously popped*** vertex from the stack.

In the second scenario, three vertices of a new triangle are always the ***current*** vertex, the ***diagonal's the other*** vertex, and the ***next*** vertex in the ***stack***.

With using this fact, we can construct triangles much easier than organizing pieces in the partitioning.
```
Triangulation (y-monotone piece) {
    sort vertices of the piece in order of decreasing y-coordinate;

    stack of vertices S;
    insert V0 into S;
    insert V1 into S;

    for (i from 2 to n-1 where n is the number of vertices of the piece) {
        if (Vi and S.top are on the different chain) {
            pop all vertices from S;
            add diagonal from Vi to each popped vertex except the last one;
            construct triangles using Vi, each popped vertex, and the next top vertex in S;

            insert Vi-1 into S;
            insert Vi into S;
        } else {
            let top <- S.pop;

            pop the other vertices from S as long as a line from Vi to them are inside the piece. add these diagonals;
            construct triangles using Vi, each popped vertex, and the previously popped vertex;

            insert the last vertex has been popped into S;
            insert Vi into S;
         }
     }

     add diagonals from Vn to all vertices in S except the first and the last one;
     construct triangles using Vn, each popped vertex, and the previously popped vertex;

     construct a triangle using Vn, the last vertex of S, and the previously popped vertex;
}
```

One last thing you should not forget is setting neighbors for each navigation mesh, so you can do pathfinding.

[Back to the top](#navigation-mesh)
