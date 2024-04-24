### Partition into monotone pieces

Triangulating a polygon requires a preceding step, which is partitioning polygon into simpler shapes.
In this step, we will split the polygon into y-monotone pieces, which means if you walk along the polygon's edges from the highest vertex to the lowest vertex, you never go up, so always go down or horizontally.
To achieve it, we will scan the entire polygon from the top to the bottom with a horizontal scan line.
During a scanning, when each time the scan line meets a vertex, we will check whether a new diagonal can be added.
So let's start to look at this algorithm step by step.

#### Label vertices
As I mentioned, we will scan all the vertices from the highest one to the lowest one and see what we can do for each vertex.
The way the partitioning algorithm handles a vertex depends on the type of vertices.
There are five types of vertices, and here are an example and the definition of them.

<img align="right" src="/Description/Images/VerticesExample.png">

##### Start vertex
A vertex is a start vertex if they meet both of these conditions:
1. Both neighbor vertices lie below this vertex.
2. The interior angle at this vertex is less than π.

Start vertices are usually at the top of a polygon. In Figure 1, only $V_1$ is a start vertex.

##### Split vertex
Conditions to be a split vertex is similar to the start vertex.
1. Both neighbor vertices lie below this vertex.
2. The interior angle at this vertex is greater than π.

In Figure 1, only $V_z$ is a split vertex.

##### End vertex
You can think end vertex as the opposite of start vertex.
1. Both neighbor vertices lie above this vertex.
2. The interior angle at this vertex is less than π.

In Figure 1, only $V_3$ is an end vertex.

##### Merge vertex
The relationship between merge vertex and end vertex is similar to the one between split vertex and start vertex.
1. Both neighbor vertices lie above this vertex.
2. The interior angle at this vertex is greater than π.

In Figure 1, only $V_x$ is a merge vertex.

##### Regular vertex
Vertices do not satisfy any of other types so far are regular vertices.
In Figure1, $V_2$ and $V_y$ are regular vertices.

#### Note on labeling
<img align="right" height="300" src="/Description/Images/VertexOrderExample.png">

Now we know about the types of vertex and need to label them to run the partitioning algorithm.
If you see the way we classified vertices, you can notice that we will need to compare each vertex with neighbors.
Therefore, you may want to define a struct of vertex and let them hold the position of the vertex and a way to access neighbors.
<br /><br />
One thing you need to beware during finding neighbors is the order of vertices.
Since the algorithm scans vertices in the counter-clockwise order, you need a way to differentiate two neighbors; previous and next.
However, the vertices on the hole must be the clockwise order.
The reason is, actually, the clockwise order on a hole is counter-clockwise to the polygon.
Let's see an example.
<br /><br />
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
See [BinaryTree.h](NavMesh/BinaryTree.h) and [BinaryTree.hpp](NavMesh/BinaryTree.hpp) if you want to check my implementation of binary search tree.

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

#### Partitioning Example
Because this is one most complicated part of this article, I have a detailed example here.

<img align="left" height="300" src="/Description/Images/PartitioningExample1.png">
<br /><br /><br /><br />
All vertices are in the priority queue.<br />
There is no intersecting edge yet.<br /><br />

$$Q: \{ V_1, V_2, V_z, V_y, V_x, V_3 \}$$

$$T: Ø$$

<br /><br /><br />
<img align="left" height="300" src="/Description/Images/PartitioningExample2.png">

<br /><br /><br />
$V_1$ is a start vertex.<br />
Insert $E_1$ in $T$ and the helper is $V_1$.<br />

$$Q: \{ V_2, V_z, V_y, V_x, V_3 \}$$

$$T: \{ E_1→V_1 \}$$

$E_1$ starts to intersect the scan line.

<br /><br /><br />
<img align="left" height="300" src="/Description/Images/PartitioningExample3.png">

<br />
$V_2$ is a regular vertex.<br />
The interior of the polygon lies on the right.<br />
The helper of $E_1$ is not a merge vertex.<br />
Delete $E_1$ from $T$.<br />
Insert $E_2$ in $T$ and the helper is $V_2$.<br />

$$Q: \{ V_z, V_y, V_x, V_3 \}$$

$$T: \{ E_2→V_2 \}$$

$E_1$ ends to intersect the scan line, and $E_2$ starts to intersect the scan line.

<br />
<img align="left" height="300" src="/Description/Images/PartitioningExample4.png">

<br />
$V_z$ is a split vertex.<br />
The edge directly left to $V_z$ is $E_2$.<br />
Insert a diagonal connecting $V_z$ and the helper of $E_2$.<br />
Set the helper of $E_2$ as $V_z$.<br />
Insert $E_z$ in $T$ and the helper is $V_z$.<br />

$$Q: \{ V_y, V_x, V_3 \}$$

$$T: \{ E_2→V_2, E_z→V_z \}$$

$E_z$ starts to intersect the scan line.

<br />
<img align="left" height="300" src="/Description/Images/PartitioningExample5.png">

<br /><br /><br />
$V_y$ is a regular vertex.<br />
The interior of the polygon does not lie on the right.<br />
The edge directly left to $V_y$ is $E_2$.<br />
The helper of $E_2$ is not a merge vertex.<br />
Set the helper of $E_2$ as $V_y$.<br />

$$Q: \{ V_x, V_3 \}$$

$$T: \{ E_2→V_y, E_z→V_z \}$$

<br />
<img align="left" height="300" src="/Description/Images/PartitioningExample6.png">

<br />
$V_x$ is a merge vertex.<br />
The helper of $E_z$ is not a merge vertex.<br />
Delete $E_z$ from $T$.<br />
The edge directly left to $V_x$ is $E_2$.<br />
The helper of $E_2$ is not a merge vertex.<br />
Set the helper of $E_2$ as $V_x$.<br />

$$Q: \{ V_3 \}$$

$$T: \{ E_2→V_x \}$$

$E_z$ ends to intersect the scan line.

<br />
<img align="left" height="300" src="/Description/Images/PartitioningExample7.png">

<br />
$V_3$ is an end vertex.<br />
The helper of $E_2$ is a merge vertex.<br />
Insert a diagonal connecting $V_3$ and the helper of $E_2$.<br />
Delete $E_2$ from $T$.<br />

$$Q: Ø$$

$$T: Ø$$

$E_2$ ends to intersect the scan line.<br />
No more vertices in $Q$, so we have done.<br /><br />

### Organizing Pieces
Now we have finished partitioning a polygon into y-monotone pieces, but there is one more step to do to complete partitioning.
We have done this to triangulate each piece, but we don't have any piece yet.
What we have are just bunch of edges and diagonals, so we need to construct pieces from them.
Depends on your implementation, you may not need this step, but I will explain how I organized pieces.

<img align="right" height="300" src="/Description/Images/OrganizingPiecesExample.png">

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