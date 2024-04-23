# Navigation Mesh
## Partition into monotone pieces
Triangulating a polygon requires a preceding step, which is partitioning polygon into simpler shapes.
In this step, we will split the polygon into y-monotone pieces, which means if you walk along the polygon's edges from the highest vertex to the lowest vertex, you never go up, so always go down or horizontally.
To achieve it, we will scan the entire polygon from the top to the bottom with a horizontal scan line.
During a scanning, when each time the scan line meets a vertex, we will check whether a new diagonal can be added.
So let's start to look at this algorithm step by step.

### Label vertices
As I mentioned, we will scan all the vertices from the highest one to the lowest one and see what we can do for each vertex.
The way the partitioning algorithm handles a vertex depends on the type of vertices.
There are five types of vertices, and here are an example and the definition of them.

#### Start vertex
A vertex is a start vertex if they meet both of these conditions:
1. Both neighbor vertices lie below this vertex.
2. The interior angle at this vertex is less than π.

Start vertices are usually at the top of a polygon. In Figure 1, only $V_1$ is a start vertex.

#### Split vertex
Conditions to be a split vertex is similar to the start vertex.
1. Both neighbor vertices lie below this vertex.
2. The interior angle at this vertex is greater than π.

In Figure 1, only $V_z$ is a split vertex.

#### End vertex
You can think end vertex as the opposite of start vertex.
1. Both neighbor vertices lie above this vertex.
2. The interior angle at this vertex is less than π.

In Figure 1, only $V_3$ is an end vertex.

#### Merge vertex
The relationship between merge vertex and end vertex is similar to the one between split vertex and start vertex.
1. Both neighbor vertices lie above this vertex.
2. The interior angle at this vertex is greater than π.

In Figure 1, only $V_x$ is a merge vertex.

#### Regular vertex
Vertices do not satisfy any of other types so far are regular vertices.
In Figure1, $V_2$ and $V_y$ are regular vertices.

#### Note on labeling
Now we know about the types of vertex and need to label them to run the partitioning algorithm.
If you see the way we classified vertices, you can notice that we will need to compare each vertex with neighbors.
Therefore, you may want to define a struct of vertex and let them hold the position of the vertex and a way to access neighbors.

One thing you need to beware during finding neighbors is the order of vertices.
Since the algorithm scans vertices in the counter-clockwise order, you need a way to differentiate two neighbors; previous and next.
However, the vertices on the hole must be the clockwise order.
The reason is, actually, the clockwise order on a hole is counter-clockwise to the polygon.
Let's see an example.

If you see Figure 2, there are blue arrows point the same direction, which is from the inside of the polygon to the outside.
Each red arrow seems to point the opposite directions, but in fact, both of them point the same direction repect to blue arrows.
If you see the vertices on the hole in the view of the polygon, you can notice that the red arrow is the counter-clockwise order.

### Partitioning
Now we can discuss about the partitioning algorithm.
As I mentioned before, we will scan all vertices from the top to the bottom.
The goal of sweep is each time the sweep line meets a vertex, by using the vertex, adding a new diagonal that divides the polygon when it is possible.
For this algorithm, we need two specific data structures.

The first is **priority queue**.
We want to scan all vertices from the highest one to the lowest one, so it will be good to have all vertices in a priority queue,
and the priority of a vertex is its y-coordinate. If two vertices have the same y-coordinate then the leftmost one has a higher priority.
Because no new vertices are generated during sweep, we could also sort the vertices on y-coordinate.

The second is **binary search tree**.
During scanning, we need to find the edge to the left of each vertex.
Therefore we store the edges intersecting the sweep line in the leaves of a dynamic binary search tree.
The left-to-right order of the leaves of tree corresponds to the left-to-right order of the edges.
Because we are only interested in edges to the left of split and merge vertices we only need to store edges in tree that have the interior of polygon to their right.
See ... if you want to check my implementation of binary search tree.

The one more thing that we need to store together with edges is a vertex called "helper" of the edge, which is a possible candidate to be used to build a diagonal.
Therefore, you may want to define a struct of edges and let them hold two vertices that construct the edge and one "helper" vertex.

#### Main loop
Once you have all vertices in a priority queue and a binary search tree to store edges,
we can simply handle each vertex until there is no vertex in the queue.
Here is the pseudo code:
```
DividePolygonIntoMonotone (polygon) {
    priority_queue of vertices Q;
    push all vertices of the polygon into Q;

    binary_tree of edges BT;

    while (Q is not empty) {
        Vi = Q.pop();
        HandleVertex(Vi);
    }
}
```
As you can see, the main loop is quiet simple. What really important is handling each vertex.

#### Vertex handling
`HandleVertex` function will check the type of a vertex and call the appropriate handling function.
There are five types of vertex, and our strategy is to add a new diagonal or to save a candidate that can be used for a diagonal later.
As I already mentioned before, we will use the concept of "Edge".
The edge Ei means the edge between two vertices Vi and Vi+1, which is the next vertex of Vi.

##### Start vertex
By the definition of start vertex, we can know the next vertex lies ***below*** Vi,
which means the edge Ei ***starts*** to intersect the scan line, so let's ***push*** Ei into the binary tree.
```
HandleStartVertex (Vi) {
    set helper of Ei <- Vi;
    insert Ei into BT;
}
```
##### End vertex
Opposite to start vertex, we can know the previous vertex lies ***above*** Vi by the definition of end vertex.
It means the edge from the previous vertex does ***not*** intersect the scan line anymore, so let's ***delete*** Ei-1 from the binary tree.
The thing we should not forget when we delete an edge is if there is a proper candidate, we should add a diagonal.
```
HandleEndVertex (Vi) {
    if (helper of Ei-1 is a merge vertex) {
        insert the diagonal connecting Vi to the helper of Ei-1;
    }

    delete Ei-1 from Bt;
}
```
##### Split vertex
Similar to start vertex, we can know the edge Ei ***starts*** to intersect the scan line.
In addition to that, since the split vertex is where the polygon ***divides***, it is a good point to add a new diagonal.
```
HandleSplitVertex (Vi) {
    find Ej on Bt which is directly left to Vi;
    insert the diagonal connecting Vi to the helper of Ej;
    set helper of Ej <- Vi;

    set helper of Ei <- Vi;
    insert Ei into BT;
}
```
##### Merge vertex
Similar to end vertex, we can know the edge from the previous vertex does ***not*** intersect the scan line anymore.
In addition to that, since the merge vertex is where the polygon ***unites***, it is a good point to add a new diagonal.
However, we haven't met the other vertex that can construct a diagonal, let's add an edge that has Vi as a helper.
```
HandleMergeVertex (Vi) {
    if (helper of Ei-1 is a merge vertex) {
        insert the diagonal connecting Vi to the helper of Ei-1;
    }

    delete Ei-1 from Bt;

    find Ej on Bt which is directly left to Vi;

    if (helper of Ej is a merge vertex) {
        insert diagonal connecting Vi to the helper of Ej;
    }

    set helper of Ej <- Vi;
}
```
##### Regular vertex
Regular vertices are the most complicated vertices to handle.
If a regular vertex is on the ***left*** side of the polygon, we can know the edge from the previous vertex does ***not*** intersect anymore,
while the edge to the next vertex ***starts*** to intersect with scan line.
On the other hand, if a regular vertex is on the ***right*** side of the polygon,
we do not need to care Ei and just need to check whether Vi can be a candidate to add a new diagonal.
```
HandleRegularVertex (Vi) {
    if (interior of the polygon lies right of Vi) {
        if (helper of Ei-1 is a merge vertex) {
            insert the diagonal connecting Vi to the helper of Ei-1;
        }

        delete Ei-1 from Bt;

        set helper of Ei <- Vi;
        insert Ei into BT;
    } else {
        find Ej on Bt which is directly left to Vi;

        if (helper of Ej is a merge vertex) {
            insert the diagonal connecting Vi to the helper of Ej;
        }

        helper of Ej = Vi;
    }
}
```
The way to figure out whether the interior of the polygon lies left or right is comparing one of the neighbors with Vi.
If the ***previous*** vertex lies ***above*** or the ***next*** vertex lies ***below*** Vi, then the interior of the polygon lies ***right*** to Vi.
In the opposite case, the interior of the polygon lies ***left*** to Vi.

### Organizing Pieces
Now we have finished partitioning a polygon into y-monotone pieces, but there is one more step to do to complete partitioning.
We have done this to triangulate each piece, but we don't have any piece yet.
What we have are just bunch of edges and diagonals, so we need to construct pieces from them.
Depends on your implementation, you may not need this step, but I will explain how I organized pieces.

Figure 9 shows the result of partitioning from the example above.
You can see that there are two pieces, and the only edges they are sharing are diagonals.
It means all ***diagonals*** must be part of a piece ***twice*** while other ***edges*** must be part of a peice only ***once***.
We will use this fact. We will look through all edges and add to a piece until all of them are used.
If we find an unused edge, we start to build a new piece.
Keep adding edges to the piece and mark them used.
Once we have a closed shape, we just have built a piece, so check if there is an unused edge and repeat until all of them are used.
```
OrganizePieces (polygon) {
    for (each vertex of polygon) {
        if (edge to the next vertex is already part of a piece) skip;

        a container of vertices P for a new piece;
        insert this vertex into P;
        insert the next vertex into P;

        let prev <- this vertex;
        let curr <- the next vertex;

        while (curr is not the first vertex of a piece) {
            let next <- GetNextEdge(curr, prev);
            insert next into P;

            if (edge from curr to next is not diagonal) mark it used;

            prev <- curr;
            curr <- next;
        }
    }
}
```
In the above pseudo code, the interesting part is `GetNextEdge` function.
It's because there are cases that you have more than one connected lines.
When a vertex has diagonal(s) you have to pick one of them or the edge of the orginal polygon.
Which one should we choose? The answer is the line has the ***smallest*** inner angle.
This way, we can ensure that there is no other line between the line from the previous vertex and the line to the next vertex,
so we can construct a piece successfully.

## Polygon triangulation
Here we will discuss the algorithm that triangulates monotone pieces, so we can generate navigation meshes.
Since we already divided the polygon into simpler shapes, this algorithm is not so complicated actually.
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
If it is possible, connect and check the next vertex in the stack. If it is not, close this case.

Figure 2 shows the result of handling the current vertex from Figure 1.
We could connect three vertices on the stack from the current vertex, so they are popped from the stack.
However, the fourth vertex could not be connected to the current vertex, so this is where we stop.
But before going to the next vertex, there is one thing we need to do, which is pushing vertices to the stack.
First, insert the vertex which is popped lastly (the last one we could connect to the current vertex, e.g. the third vertex in Figure 2).
Second, insert the current vertex. If you see Figure 2, you can notice that vertices connected to the current vertex can not have more diagonals.

The other scenario is when the current vertex and the top vertex of the stack are on the different chain. Figure 3 shows this case.
Since they are on the different chain and this polygon is y-monotone,
we can add a new diagonal to all vertices in the stack except the last one which is the highest vertex that is already connected to the current vertex.
The result of handling Figure 3 is Figure 4.
Similar to the previous scenario, we need to insert two vertices into the stack; the current vertex and the vertex previously on top of the stack.

### Naviagation Mesh Generation
We just have looked through how the triangulation alogrithm works, but you might notice that we just added diagonals,
which doesn't give us nav-mesh triangles magically.
The positive thing is that since this algorithm is simple and clear, we can easily achieve it.
Let's take a look at Figure 2 and 4 again.
You can notice that each time we add a new diagonal, it creates a new triangle.
In the first scenario, three vertices of a new triangle are always the current vertex, the other vertex of the diagonal,
and the previously popped vertex from the stack.
In the second scenario, three verties of a new triangle are always the current vertex, the other vertex of the diagonal, and the next vertex in the stack.
With using this fact, we can construct triangles much easier than in the partitioning.
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
