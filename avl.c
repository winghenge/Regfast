#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

void update_bf(struct AVL_Node *node){
    // update the heights
    if (node->left) 
        // if there is a left node, grab the longest hight it has:
        // left or right
        node->left_h = (node->left->left_h > node->left->right_h) ? 
        node->left->left_h+1 : node->left->right_h+1;
    else node->left_h = 0;

    if (node->right)
        // if there is a right node, grab the longest hight it has:
        // left or right
        node->right_h = (node->right->left_h > node->right->right_h) ?
        node->right->left_h+1 : node->right->right_h+1;
    else node->right = 0;

    // re-caulculate the bf
    node->b_factor = node->left_h - node->right_h;

    return;
}

void balance(struct AVL_Node *pivot, struct AVL_Node **parent){
    /* 
     * This function will not only perform the single or double rotation,
     * but also determine which rotation (if any) are needed based off of a node's
     * balance factors
     *
     * Types of rotations that need to be performed:
     *
     * Right rotate:
     *
     *      p          P
     *      |          |
     *      X          Y
     *     /          / \
     *    Y      ->  z   X
     *   /
     *  Z
     *
     *  Balance factors:
     *  X:2     X:0 
     *  Y:1 ->  Y:0
     *  Z:0     Z:0
     *
     * Right-Left rotate:
     *
     *      p              P             P
     *      |              |             |
     *      X              X             Z
     *       \    Right     \    Left   / \
     *        Y    ->        Z    ->   X   Y
     *       /                \                  .
     *      Z                  Y     
     *                       
     *  Balance factors:
     *  X:-2    X:-2    X:0
     *  Y:1  -> Y:0  -> Y:0
     *  Z:0     Z:-1    Z:0            
     *
     * Right rotate:
     *
     *      p           P
     *      |           |
     *      X           Y
     *       \         / \
     *        Y   ->  X   Z
     *         \
     *          Z
     *
     *  Balance factors:
     *  X:-2     X:0 
     *  Y:-1 ->  Y:0
     *  Z:0      Z:0
     *
     * Right-Left rotate:
     *
     *      p              P           P
     *      |              |           |
     *      X              X           Z
     *     /      Right   /    Left   / \
     *    Y        ->    Z      ->   Y   X
     *     \            /                  .
     *      Z          Y     
     *                       
     *  Balance factors:
     *  X:2     X:2    X:0
     *  Y:-1 -> Y:0 -> Y:0
     *  Z:0     Z:1    Z:0
     */

    // Determine if the pivot node needs to be rotated
    // if balance factor is 1, 0, -1 we dont need to do anything
    if (pivot->b_factor <= 1 && pivot->b_factor > -1) return;

    // at this point, we need to wither perform a right or left rotate on our
    // pivot, but will also need to check the roating child to see if an 
    // aditional rotation is needed as well
    if (pivot->b_factor > 0){
        // Imbalanced on left side -> Right rotate
        // the child node to rotate
        struct AVL_Node *child = pivot->left;

        // check to see if we need to rotate the child node first before the
        // pivot
        // for a right rotate, the chld node will need to be rotated if it has
        // a balance factor of -1
        if (child->b_factor < 0){
            // the sub-child node that needs to be left rotated first, before
            // the pivot can be right rotated
            struct AVL_Node *subchild = child->right;

            // Perform the left rotation:
            // first, the left child of the SubChild becomes the right child of
            // the Child
            child->right = subchild->left;

            // now the Child can become the left child of the SubChild
            subchild->left = child;

            // finally the SubChild can become the left child of Pivot
            pivot->left = subchild;

            // update the balance factors for Child
            update_bf(child);

            // what we call child outside of this scope is no longer 
            // the left child of pivot, so update
            child = subchild;
        }

        // now we can perform the right rotation:
        // first the right child of the Child becomes the left child of Pivot
        pivot->left = child->right;

        // now the Pivot can become the right child of Child
        child->right = pivot;

        // now the Child becomes the child of Parent
        (*parent) = child;

        // update balance factors
        update_bf(child);
        update_bf(pivot);
        update_bf(*parent);

    }
    else{
        // Imbalanced on right side -> left rotate
        // the child node to rotate
        struct AVL_Node *child = pivot->right;

        // check to see if we need to rotate the child node first before the
        // pivot
        // for a left rotate, the chld node will need to be rotated if it has
        // a balance factor of 1
        if (child->b_factor > 0){
            // the sub-child node that needs to be right rotated first, before
            // the pivot can be left rotated
            struct AVL_Node *subchild = child->left;

            // now we can perform the right rotation:
            // first the right child of the SubChild becomes the left 
            // child of Child
            child->left = subchild->right;

            // now the Child can become the right child of SubChild
            subchild->right = child;

            // now the SubChild becomes the child of Pivot
            pivot->right = subchild;

            // update the balance factors for Child
            update_bf(child);

            // what we call child outside of this scope is no longer
            // the left child of pivot, so update
            child = subchild;

        }

        // Perform the left rotation:
        // first, the left child of the Child becomes the right child of
        // the Pivot
        pivot->right = child->left;

        // now the pivot can become the left child of the Child
        child->left = pivot;

        // finally the Child can become the child of the Parent
        (*parent) = child;

        // update balance factors
        update_bf(child);
        update_bf(pivot);
        update_bf(*parent);

    }


    return;
}



int insert_node(struct AVL_Node **root, int val){
    static struct AVL_Node **prev = NULL;
    static int size = 0;

    // perform startup on prev array
    if (!prev){
        printf("STARTUP PREV\n");
        prev = (struct AVL_Node **)malloc(sizeof(struct AVL_Node *) * 128);
        size = 128;
    }

    // now lets perform 2 checks now to make sure the array is big enough
    // instead of a check on each node
    // make sure that size >= MAX(right_h, left_h) on root
    // although we must first ensure that root != NULL
    if (*root){
        if ( size < (*root)->left_h ){
            // double size
            free(prev);
            size *= 2;
            prev = (struct AVL_Node **)malloc(sizeof(struct AVL_Node *) 
                * size);
        }
        else if ( size < (*root)->right_h ){
            // double size
            free(prev);
            size *= 2;
            prev = (struct AVL_Node **)malloc(sizeof(struct AVL_Node *) 
                * size);
        }
    }
    else{
        // we have a null tree, declare root
        *root = (struct AVL_Node *)malloc(sizeof(struct AVL_Node));
        (*root)->val = val;
        return 0;
    }

    
    // get ready to traverse the tree
    struct AVL_Node *cur = (*root);

    // a count of how deep in the tree we are
    int cnt = 0;
    
    while(1){
        // remember the path
        prev[cnt++] = cur;
        if (cur->val > val){
            // need to insert onto the left
            // check to see if we can insert as a child of this node
            // or need to continue traversing a tree
            if (cur->left){
                // traverse the tree further
                // set the current node as the next child
                cur = cur->left;
            }
            else{
                // we can insert the child here
                cur->left = (struct AVL_Node *)malloc(sizeof(struct AVL_Node));
                cur->left->val = val;
                break;
            }
        }
        else{
            // need to insert onto the right
            // check to see if we can insert as a child of this node
            // or need to continue traversing a tree
            if (cur->right){
                // traverse the tree further
                // set the current node as the next child
                cur = cur->right;
            }
            else{
                // we can insert the child here
                cur->right = (struct AVL_Node *)malloc(sizeof(struct AVL_Node));
                cur->right->val = val;
                break;
            }
        }
    }

    return 1;
}

int remove_node(struct AVL_Node **root, int val){

    return 0;
}

int free_mem(struct AVL_Node **root){

    // if we are NULL, return
    if (!*root) return 0;

    // keep track of the number of nodes freed
    int freed = 1;

    // remove the child nodes first
    freed += free_mem(&(*root)->left);
    freed += free_mem(&(*root)->right);

    // now we can remove ourselfs
    free(*root);

    *root = NULL;


    return freed;
}

void print_tree(struct AVL_Node *root, int h){
    if (!root) return;

    print_tree(root->right, h+1);

    for(int i = 0; i < h; i++) printf("  ");
    printf("%d\n", root->val);

    print_tree(root->left, h+1);

    return;
}

