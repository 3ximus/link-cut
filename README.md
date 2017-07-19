# Link cut trees
Implementation of link cut trees to represent a forest of rooted, unorderes trees.<br>

It supports the folloeing operations:
 - `link(v, w)`​ - Adds an edge linking node v to node w. If such an edge already exists or this insertion would create a cycle then the operation has no effect.
 - `cut(v, w)` ​- Removes the edge linking the node v to the node w, if such an edge exists. If the edge does not exist this operation has no effect.
 - `connected(v, w)` ​- Returns true if there is a connection from v to w. If such a connection does not exist it returns false. A connection may consist of a single edge, or a sequence of edges, provided that it links v to w.

Splay trees were used and, as the theoretical analysis predicted, we obtained a log(N) bound on all three operations. Experimental results can be seen in the report.