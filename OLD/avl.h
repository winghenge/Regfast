
#ifndef AVL_H
#define AVL_H

struct AVL_Node{
    struct AVL_Node *left;
    struct AVL_Node *right;

    int right_h;
    int left_h;

    int b_factor;
    
    // val will be used as UID as well
    int val;

};

struct AVL_Root{
    struct AVL_Node *root;
};

int insert_node(struct AVL_Node **root, int val);
int remove_node(struct AVL_Node **root, int val);
int free_mem(struct AVL_Node **root);

void print_tree(struct AVL_Node *root, int h);

#endif //AVL_H
