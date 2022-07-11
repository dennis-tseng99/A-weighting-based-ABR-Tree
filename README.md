# A-weighting-based-ABR-Tree
A new algorithm is proposed to resolve this skew issue of RB-Tree.

**[Background]** 
RED-BLACK Tree and AVL Tree have been applied in skb structure in Linux networking kernel, or in searching the IP routing information based table(RIB) for routing entry lookup. If more insertions or deletions are needed, then RB-Tree is used. On the other hand, when searching is the key point, then AVL-Tree is suggested. This is because, after running a long time, RB-Tree tends to become a skew tree which means the total search path is longer than AVL-Tree. 
-- AVL trees are more rigidly balanced and hence provide faster look-ups. Thus, for a look-up intensive task use an AVL tree.
-- For an insert intensive tasks, use a Red-Black tree.

In this patent, a new algorithm is proposed to resolve this skew issue of RB-Tree.
Let’s take a look about the relationship between B-Tree and RB-Tree. The structure of Red-Black Tree is equivalent to the 2-3-4 tree which is a B-tree of order 4 or m-way B-Tree where m=4. Although 2-3-4 tree has a good balance property with all external nodes are at the same depth, searching a key located in a 2-3-4 B-Tree node will become a sequential search. Worse than that, when a B-Tree is of large order, then we need a linear search within a node to move to the right key value. If you don’t want to sequentially search the link list in a B-Tree node, then you must resize the array now and then to save memory. This results in B-Tree can be only used for searching disk sector in which records are linearly located in memory.
To alleviate this problem of search time, Red-Black Tree was proposed. Red-Black Tree can provide a O(log n) speed efficiency to search a key located in each B-Tree node.  Also, Red-Black tree is self-balancing which can avoid developing a normal binary tree to a skewed tree after several insertions and deletions.
However, currently, the structure of Red-Black Tree is only equivalent to the B-Tree of order 4, not for B-Tree of any order. When order increases, we cannot make use any insertion algorithm of Red-Black Tree any more, and thus the sequential search loading will be heavier when searching a key located in a long link list inside B-Tree. 
In this patent, we provide a homogeneous Red-Black Tree like insertion algorithm which can be applied to any order of B-Tree, including a B-Tree of order 4 of course. This algorithm will combine 3 concepts, AVL tree, B-Tree and RB-Tree, together, so called ABR-Tree. So that we can make use of all advantages of self-balancing of AVL Tree, shorter depth path of B-Tree and an amortized rotation of RB-Tree simultaneously. 

**[Traditional insertion of Red-Black Tree]**
There are two operations for the insertion of Red-Black tree :
1.	Rotation : when two continuous red node are met. There will have LL,LR,RL and RR different rotations.
2.	Recoloring : when the color of parent of new inserted node is red, and the color of its arm is also red. That is to say : when its parent is red, and the other child of its parent is also red, then recolor its parent and parent’s two children is necessary.

**[Insertion algorithm of ABR-Tree]**
Instead of using original RB-Tree’s insertion rules, some modifications are needed for B-Tree of any order :
1.	Amortized promotion : For each red color node, when the weighting difference of its left and right red color nodes is larger than or equal to 2, then amortized promotion or called amortized rotation is necessary. The term “Amortized” is used to keep the local red color area in balancing for the tree now and then. At this stage, the term “weighting” means either the total left red children or right red children of current node. Each node includes “left child weighting” and “right child weighting”. The reason why “weight” rather than “depth” originally adapted in BF(Balance Factor) by AVL-Tree is that ABR-Tree is actually a B-Tree of any order.
2.	Recoloring or splitting : when the summation of left weight and right weight of current node is equal to or larger than order value, then recolor current node and its children is necessary.

**[AVL-Tree vs ABR-Tree]**
AVL Tree tends to be heavier than ABR Tree due to its less balance. The following diagram-1 and 2, just a snapshot from my attached file, show that AVL-Tree is heavier(61) than ABR-Tree(60) with order=5, where diagram-1 fully meets the requirement of AVL Tree in which the BF(Balance Factor) of each node is small than 2. That is to say, we must spend more lookup time in AVL-Tree. Please also refer the attached file in which more detail, step by step of weighting comparison are depicted for AVL-Tree, RB-Tree and ABR-Tree respectively. Implicitly, the structure of ABR-Tree is actually equivalent to the combination of RB-Tree and B-Tree of any order, hence you can see there still have red and black color nodes in ABR-Tree.
The input sequence is : 1,12,8,2,25,6,14,28,17,7,52,16,48,68
