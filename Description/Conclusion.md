# Navigation Mesh - Conclusion
[Main Menu](../README.md)
<br /><br />
[Prev Chapter - Pathfinding](Pathfinding.md)

## Summary and future work
We have looked through all algorithms I used for this demo.
First, we paritioned a polygon into y-monotone pieces. Second, we trianulated each piece.
Both steps required some extra works to organized pieces and navigation mesh triangles.
Then we looked how A* works on navigation meshes and funnel algorithm smooths the path.
And here are some topics you may want to try on top of my work.

### Merging navigation meshes
I triangulated polygons and used it for meshes, but actually, it does not need to be a triangle.
Navigation meshes can be any shape as long as they are convex,
which guarantess that moving straight from any point inside a mesh to any other point inside the mesh.
If you are interested in this topic, you may want to check this article:
[Tozour02] Paul Tozour. 2002. Buildig a Near-Optimal Navigation Mesh. In *AI Game Programming Wisdom* 1, ed. Steve Rabin, 171-185. Charles River Media.

### Path correctness
Since I did pathfinding upon the center point of edges, there are some cases that fail to get an optimal path.
Pathfinding upon the center point of a mesh or completely different solution may have better result.

### Size of agent
Usually, the purpose of pathfinding is to find a path for an agent to move.
If the space is continuous and you want to make the agent be apart from obstacles during moving, you need to consider the size of agent.
To achive it, you need to modify the path smoothing algorithm; 
instead of drawing lines from the starting point to the left and right vertex of edges,
drawing lines to the points that are apart from the left and right vertex by the size of the agent.

## References
[Berg 08] Berg, M. de, O Cheong, M. van Kreveld and M. Overmars. 2008. *Computational Geometry*, 3rd Ed. Berlin: Springer.<br />
[Snook 00] Greg Snook. 2000. Simplified 3D Movement and Pathfinding Using Navigation Meshes. In *Game Programming Gems 1*, ed. Mark A. DeLoura, 288-304. Charles River Media.<br />
http://digestingduck.blogspot.com/2010/03/simple-stupid-funnel-algorithm.html

[Back to the top](#navigation-mesh)
