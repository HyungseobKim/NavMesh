# Navigation Mesh - Pathfinding
[Main Menu](../README.md)
<br /><br />
[Prev Chapter - Partitioning](Triangulation.md)
<br /><br />
[Next Chapter - Conclusion](Conclusion.md)

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
There are not many things that you need to modify A* to use it on navigation meshes.
The only thing you need to decide is the standard point of each mesh.
One is the center point of mesh. With this one, you can simply do pathfinding with all center points.
The actual path will look like going to the center of a mesh first,
then moving along the center point of adjacent meshes, and moving to the destination point.
The other point you can use is the center point of each edge, which is I used.

<img align="center" src="/Description/Images/PathFindingExample.png">

You can read the explanation below or my implementation here: [Astar.h](../NavMesh/Astar.h), [Astar.cpp](../NavMesh/Astar.cpp)

```
Astar (start node, end node, nodes) {
    priority queue of nodes Q;
    push end node into Q;
    
    while (Q is not empty) {
        let current node <- Q.pop;
        
        if (current node is start node) {
            RestorePath(start node, end node);
            return;
        }
        
        ProcessNode(current node);
    }
}
```
The main loop is simple and identical to the common implementation of A*.
The priority queue gives you the node with the smallest cost.
We start with the end node instead of start node for easier path restoration.
Every time we get a node from Q, we check if it is the start node.
If it is, we have found the path, so restore the path and finish the search.
Otherwise, process the node.

```
ProcessNode (node to process) {
    for (each neighbor node adjacent to node to process) {
        if (neighbor is on the open list) {
            let distance <- the distance between the origin of neighbor and the origin of node to process;
            let new given cost <- distance + the given cost of node to process;
            let new cost <- new given cost + heuristic of neighbor to end node;
            
            if (new cost < the cost of neighbor) {
                set parent of neighbor <- node to process;
                set origin of neighbor <- the center of the edge that neighbor and node to process shares;
                set given cost of neighbor <- new given cost;
                set cost of neighbor <- new cost;
                
                decrease key of neighbor inside Q;
            }
        }
        else if (neighbor is not on the closed list) {
            set status of neighbor <- open list;
            set parent of neighbor <- node to process;
            set origin of neighbor <- the center of the edge that neighbor and node to process shares;
            
            let distance <- the distance between the origin of neighbor and the origin of node to process
            set given cost of neighbor <- distance + the given cost of node to process;
            set cost of neighbor <- given cost of neighbor + heuristic of neighbor to end node;
        
            push neighbor into Q;
        }
    }
}
```
Processing a node is evaluating the cost of the node and deciding where the node should be.
If the node was on the ***closed*** list, this node was already visited, so ***skip*** it. Otherwise, compute the cost to the node.
If the node was ***not*** on the ***any*** list, ***push*** it into the priority queue.
If the node was on the ***open*** list, ***compare*** the new cost to the previously computed cost of the node.
If the new cost is smaller, decrease the position of the node inside the priority queue.
If the new cost is greater or equal, skip it.

## Path smoothing
A* is a fast and reliable algorithm, but there is something we can improve.
See Figure 1. Even the path can be single straight line, it is not.
It is because the pathfinding is based on the center point of edges.
You can see the path curves very much to go to the center of edges.
The way I resolve this is simple stupid funnel algorithm.

Funnel algorithm keeps track of both left and right side of a funnel and try to tighten the funnel.
Let's say we draw a line from the starting point to one of all edges' left and right vertex.
If both left and right lines are inside the movable area and the line to the left vertex is at the left and the line to the right vertex is at the right,
it means we can draw a straight line from the starting point to the center of two vertices.
Funnel algorithm keeps checking it by moving forward each side alternately.
If one line goes over the other line, it means we meet a corner, so add it to the new path and set it as a new starting point.
Once one of left and right line is reaches the end point, finish the algorithm.

<img align="center" src="/Description/Images/FunnelAlgorithm.png">

Figure 2 shows how funnel algorithm works.
The blue lines are left sides of the funnel, the orange lines are right sides of the funnel, and the red lines are the new path.


```
FunnelAlgorithm (edges that the path is passing) {
    
    collection of points P;
    
    collection of vertices L;
    collection of vertices R;
    
    push all left vertex of edges into L;
    push all right vertex of edges into R;
    
    let corner <- starting point of the path;
    
    let left index <- 0;
    let right index <- 0;
    
    let portal left <- L[0];
    let portal right <- R[0];
    
    for each (i from 1 to n where n is the number of edges) {
        let left <- L[i];
        let right <- R[i];
        
        if (right is not portal right AND TriArea(corner, portal right, right) <= 0) {
            if (corner is portal right OR TriArea(corner, portal left, right) > 0 {
                set portal right <- right;
                set right index <- i;
            }
            else {
                set corner <- portal left;
                push corner into P;
                
                set i <- left index;
                
                set left index <- left index + 1;
                set right index <- left index;
                
                if (left index > n) return P;
                
                set portal left = L[left index];
                set portal right = R[right index];
                
                continue;
            }
        }
        
        if (left is not portal left AND TriArea(corner, portal left, left) <= 0) {
            if (corner is portal left OR TriArea(corner, portal right, left) < 0) {
                portal left = left;
                left index = i;
            }
            else {
                set corner <- portal right;
                push corner into P;
                
                set i <- right index;
                
                set right index <- right index + 1;
                set left index <- right index;
                
                if (left index > n) return P;
                
                set portal left = L[left index];
                set portal right = R[right index];
            }
        }
    }
```
`TriaArea` is the way I check if one line is going over the other line, which uses cross product.
Since magnitude of cross product represents the area of parallelogram, we can know their status by checking the sign of the result.
`TriaArea` function also can be used to determine the left and right vertex of an edge.
```
TriArea (p, p1, p2) {
    let v1 <- p1 - p;
    let v2 <- p2 - p;
    
    return v2.x * v1.y - v1.x * v2.y;
}
```

[Back to the top](#navigation-mesh---pathfinding)
