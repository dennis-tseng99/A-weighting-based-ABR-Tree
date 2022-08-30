#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define  TRUE   1
#define  FALSE  0
#define  BLACK  0
#define  RED    1
#define  _ORDER_ 5

typedef struct RBT_TREE  RBT_NODE;
struct RBT_TREE {
    int         data;
    int         color;
    int         level;
    RBT_NODE    *left;
    RBT_NODE    *right;
    RBT_NODE    *parent;
};
RBT_NODE    *root;
RBT_NODE    *CREATE_NODE(int data);
RBT_NODE    *Find_inorder_immed_successor(RBT_NODE *tree);
RBT_NODE    *Find_inorder_immed_predecessor(RBT_NODE *tree);
RBT_NODE    *Insert_BSTree(RBT_NODE *tree, int data);

void        Insert_BSNode(RBT_NODE *tree, RBT_NODE *node);
void        Insert_RBTree(RBT_NODE *tree, int data);
void        Insert_RBNode(RBT_NODE *tree, RBT_NODE *new_node);
void        Print_RBTree(RBT_NODE *tree);

void        Split_tree(RBT_NODE *tree);
void        Amortised_promote(RBT_NODE *tree, RBT_NODE *mid);
void        Amortised_promote_left(RBT_NODE *tree, RBT_NODE *mid);

int         Count_weight(RBT_NODE *tree, int cnt);

/***********************************************************
 * main
 ***********************************************************/    
int main(void)
{
    RBT_NODE    *tree=NULL,*node;
    int         input[] = { 1,12,8,2,25,6,14,28,17,7,52,16,48,68,3,26,29,53,55,45 }; //order=5
    int         i;
    
    for(i=0; i<sizeof(input)/sizeof(int); i++) {
        printf("\ninput:%d\n",input[i]);
        Insert_BSTree(root,input[i]);
    }
    printf("\n---------- final result ------------\n");
    Print_RBTree(root);
}

/***********************************************************
 * Insert_BSTree
 ***********************************************************/    
RBT_NODE *Insert_BSTree(RBT_NODE *tree, int data)
{
    static RBT_NODE *node;
    RBT_NODE        *gp; 
    RBT_NODE        *gp_tmp;
    RBT_NODE        *mid;
    int             rw=0,lw=0; //right-weight, left-weight

    node = CREATE_NODE(data);
    if (root == NULL) {
        root = node;
        root->color = BLACK;
        return root;
    }
    Insert_BSNode(root, node);
    
    if (data == 10)
        printf("test\n");
    
    if (node->parent && node->parent->color == RED) { //2 continuous RED color nodes
        gp = node->parent->parent;
        for(;gp; gp=gp->parent) {
            rw = Count_weight(gp->right,1);
            lw = Count_weight(gp->left,1);
            
            if (rw>lw && (rw-lw)>=2) {
                mid = Find_inorder_immed_successor(gp->right);
                Amortised_promote(gp,mid);
                gp = mid;
                printf("Amortise prompt right:\n");
                Print_RBTree(root);
            }
            else if (lw>rw && (lw-rw)>=2) {
                mid = Find_inorder_immed_predecessor(gp->left);
                Amortised_promote(gp,mid);
                gp = mid;
                printf("Amortise prompt left:\n");
                Print_RBTree(root);
            }
            
            if (gp->color == BLACK && (rw+lw-1) >= _ORDER_) {
                Split_tree(gp);
                printf("Split:\n");
                Print_RBTree(root);
            }
        } //for
    }
    return root;
}

/***********************************************************
 * Insert_BSNode :
 ***********************************************************/    
void Insert_BSNode(RBT_NODE *tree, RBT_NODE *node)
{
    if (node->data < tree->data) {
        if (tree->left == NULL) {
            tree->left = node;
            node->parent = tree;
            return;
        }
        Insert_BSNode(tree->left, node);
    }
    else if (node->data > tree->data) {
        if (tree->right == NULL) {
            tree->right = node;
            node->parent = tree;
            return;
        }
        Insert_BSNode(tree->right, node);
    }
}

/***********************************************************
 * Find_inorder_immed_successor :
 ***********************************************************/  
RBT_NODE *Find_inorder_immed_successor(RBT_NODE *tree)
{
    RBT_NODE *node;
    
    if (tree == NULL) {
        return NULL;
    }
    
    if (tree->color == BLACK) {
        return NULL;
    }
    
    node = Find_inorder_immed_successor(tree->left);
    if (node) {
        return node;
    }
    else if (tree->color == RED && (tree->left == NULL || tree->left->color == BLACK)) {
        return tree;
    }
    node = Find_inorder_immed_successor(tree->right);
    return node;
}

/***********************************************************
 * Find_inorder_immed_predecessor :
 ***********************************************************/  
RBT_NODE *Find_inorder_immed_predecessor(RBT_NODE *tree)
{
    RBT_NODE *node;
    
    if (tree == NULL) {
        return NULL;
    }
    
    if (tree->color == BLACK) {
        return NULL;
    }
    
    node = Find_inorder_immed_successor(tree->right);
    if (node) {
        return node;
    }
    else if (tree->color == RED && (tree->right == NULL || tree->right->color == BLACK)) {
        return tree;
    }
    node = Find_inorder_immed_successor(tree->left);
    return node;
}

/*----------------------------------------------------------
 * Count_weight
 *
 * input : tree - node with unbalance weight >= 2
 *---------------------------------------------------------*/
int Count_weight(RBT_NODE *tree, int cnt)
{
    RBT_NODE *mid,*parent;

    if (tree == NULL || tree->color != RED) {
        return cnt;
    }
    cnt++;
    cnt = Count_weight(tree->left, cnt);
    cnt = Count_weight(tree->right, cnt);
    return cnt;
}

/*----------------------------------------------------------
 * Amortised_promote
 *
 * input : 
 *  tree - node with unbalance weight >= 2
 *  mid - 
 *---------------------------------------------------------*/
void Amortised_promote(RBT_NODE *tree, RBT_NODE *mid)
{
    RBT_NODE *parent,*midUP,*midL,*midR;

    midUP = mid->parent;
    midL = mid->left;
    midR = mid->right;
    
    parent = tree->parent;
    mid->parent = parent;
    
    if (parent) {
        if (parent->right == tree) {
            /*
             * parent      parent 
             *   \           \
             *  tree    =>   mid
             *     \         /
             *     mid     tree
             */
            parent->right = mid;
        }
        else if (parent->left == tree) {
            /*
             *   parent     parent
             *     /          /
             *  tree    =>   mid
             *     \         /
             *     mid      tree
             */
            parent->left = mid;
        }
    }
    else{
        root = mid;
    }
    mid->color = tree->color;
        
    if (tree->right && mid->data >= tree->right->data) {
        /*
         *  tree(midUP)      
         *    \
         *    mid   =>   mid
         *    / \        /  \
         *  midL midR  tree midR
         *               \
         *              midL
         *
         *     tree            mid
         *     / \            /   \   
         *       midUP      tree  midUP
         *        \         / \    \
         *        mid        midL  midR
         *        / \         
         *      midL midR      
         *
         *     tree            mid
         *     / \            /   \   
         *       xxx(R)     tree  xxx
         *        \           \     \
         *        midUP(R)   midL  midUP
         *         \                 \
         *         mid(R)            midR
         *         /   \         
         *     midL(B) midR(B)      
         */
        mid->left = tree;
        mid->left->color = RED;
        
        if (tree->right == mid) {
            mid->right = midR;
        }
        else if (tree->right == midUP) {
            mid->right = midUP;
        }
        else{
            mid->right = tree->right;
        }
        
        tree->right = midL;
        if (midL) {
            midL->parent = tree;
        }
        if (midUP != tree) {
            midUP->right = midR;
        }
        tree->parent = mid;
    }
    else if (tree->left && mid->data <= tree->left->data) {
        /*
         *  tree(midUP)      
         *    / \
         *   mid   =>   mid
         *   / \        /  \
         * midL midR  midL tree
         *                  /
         *                midR
         *
         *      tree        mid
         *      /  \       /   \   
         *    midUP      midUP tree 
         *     /         /      / \
         *   mid      midL    midR
         *   / \         
         * midL midR   
         *
         *       tree            mid
         *       / \            /   \   
         *    xxx(R)           xxx  tree  
         *      /              /     /
         *   midUP(R)       midUP  midR  
         *     /              / \
         *   mid(R)         midL   
         *   /    \         
         * midL(B) midR(B) 
         */
        mid->right = tree;
        mid->right->color = RED;
        
        if (tree->left == mid) {
            mid->left = midL;
        }
        else if (tree->right == midUP) {
            mid->left = midUP;
        }
        else{
            mid->left = tree->left;
        }
        
        tree->left = midR;
        if (midR) {
            midR->parent = tree;
        }
        if (midUP != tree) {
            midUP->left = midL;
        }
        tree->parent = mid;
    }
    else if (tree->right && mid->data <= tree->right->data) {
        /*
         *  tree      
         *    \
         *    midUP   =>  mid
         *    /          /   \
         *   mid       tree  midUP
         *   / \         \    /
         * midL midR    midL midR
         *
         *    9                      9
         *   / \           =>       / \
         *     23(tree)              24(mid)
         *     / \                   /   \
         *    14 45(node)      23(tree)  45(node)
         *       /                 /\     /
         *      25(midUP)        14 midL 25(midUP)
         *      /                       /
         *     24(mid)                 midR
         *    /  \
         *  midL midR
         */
        mid->left = tree;                                             
        mid->left->color = RED;                                 
                                                                                       
        if (tree->right == midUP) {                           
            mid->right = midUP;                                      
        }                                                                             
        else{                                          
            mid->right = tree->right;                               
        }                                                                            
        mid->right->color = RED;                                                 
        mid->right->parent = mid;                             
                                                                                      
        midUP->left = midR;                                                            
        if (midR) {                                                 
            midR->parent = midUP;                                   
        }                                                                   
                                                                  
        tree->right = midL;                                                           
        if (midL) {                                               
            midL->parent = tree;                                   
        }     
        tree->parent = mid;
    }
    else if (tree->left && mid->data >= tree->left->data) {
        /*
         *    tree      
         *     /
         *   midUP   =>    mid
         *     \          /   \
         *     mid     midUP tree
         *     / \       \     /
         *  midL midR   midL midR
         *
         *    tree      
         *    / \                   
         *  node             mid
         *    \              /  \    
         *   midUP         node tree
         *     \           /     /          
         *     mid        midUP midR       
         *    /  \         \ 
         *  midL midR     midL
         */ 
        mid->right = tree;
        mid->right->color = RED;
        
        if (mid->left == midUP) {
            mid->left = midUP;
        }
        else{
            mid->left = tree->left;
        }
        mid->left->color = RED;
        mid->left->parent = mid;
        
        midUP->right = midL;
        if (midL) {
            midL->parent = midUP;
        }
        
        tree->left = midR;
        if (midR) {
            midR->parent = tree;
        }
        tree->parent = mid;
    }
}

/***********************************************************
 * Split_tree
 ***********************************************************/    
void Split_tree(RBT_NODE *tree)
{
    if (tree->left) {
        tree->left->color = BLACK;
    }
    if (tree->right) {
        tree->right->color = BLACK;
    }
    if (tree == root)  return;
    
    tree->color = RED;
}

/*----------------------------------------------------------
 * CREATE_NODE
 *---------------------------------------------------------*/
RBT_NODE *CREATE_NODE(int data)
{
    RBT_NODE *node;
    
    node = (RBT_NODE*)malloc(sizeof(RBT_NODE));
    node->data = data;
    node->color = RED;
    node->level = 0;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

/***********************************************************
 * Print_RBTree
 ***********************************************************/    
void Print_RBTree(RBT_NODE *tree)
{
    RBT_NODE    *queue[128];
    RBT_NODE    *node,*tmp;
    int         front=0,rear;
    int         parent_data,left_data,right_data;
    int         level,prev_level=0;
    
    queue[front++] = tree;
    for(rear=0; rear != front; rear++) {
        node = queue[rear];
        if (node->parent) {
            parent_data = node->parent->data;
        }
        else{
            parent_data = 0;
        }
        
        if (node->left) {
            left_data = node->left->data;
        }
        else{
            left_data = 0;
        }
        
        if (node->right) {
            right_data = node->right->data;
        }
        else{
            right_data = 0;
        }
        
        for(level=1,tmp=node; tmp->parent; level++) {
            tmp = tmp->parent;
        }
        if (level != prev_level) {
            prev_level = level;
            if (node != root) {
                printf("\n");
            }
        }
        
        printf("%d[%s(%d,%d,%d)] ",node->data, (node->color == BLACK ? "B" : "R"), parent_data, left_data, right_data);
                
        if (node->left) {
            queue[front++] = node->left;
        }
        if (node->right) {
            queue[front++] = node->right;
        }
    }
    printf("\n");
}
