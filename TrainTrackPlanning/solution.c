#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct tree_node {
    int val;
    struct tree_node *left;
    struct tree_node *right;
    int max_depth;
    int connects_nodes;
} node;

node* new_node(int value);
void deallocate (node *root);
node* build_tree(int *preorder, int n, int* idx, int min, int max);


int main() {
    int node_count;
    scanf("%d", &node_count);

    int* node_array = malloc(sizeof(int) * node_count);

    if (node_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < node_count; i++) {
        scanf("%d", &node_array[i]);
    }

    int idx = 0;
    node *root = build_tree(node_array, node_count, &idx, INT_MIN, INT_MAX);
    printf("%d\n", root->connects_nodes);
    free(node_array);
    deallocate(root);

    return 0;
}

node* build_tree(int *preorder, int n, int* idx, int min, int max) {
    if (*idx == n) {
        return NULL;
    }

    int val = preorder[*idx];

    if (val < min || val > max) {
        return NULL;
    }

    node *root = new_node(val);
    (*idx)++;

    root->left = build_tree(preorder, n, idx, min, val - 1);
    root->right = build_tree(preorder, n, idx, val + 1, max); 

    if (root->left != NULL && root->right != NULL)
    {
        root->max_depth += MAX(root->right->max_depth,root->left->max_depth);
        if (root->left->connects_nodes + root->right->max_depth >= root->right->connects_nodes + root->left->max_depth)
        {
            root->connects_nodes += root->left->connects_nodes + root->right->max_depth;
        } else {
            root->connects_nodes += root->right->connects_nodes + root->left->max_depth;
        } 
    } else if (root->right == NULL && root->left != NULL)
    {
        root->max_depth += root->left->max_depth;
        root->connects_nodes += root->left->connects_nodes;
        
    } else if(root->left == NULL && root->right != NULL){
        root->max_depth += root->right->max_depth;
        root->connects_nodes += root->right->connects_nodes;
    }

    return root;
}

node* new_node(int value){
    node *new_n = (node*)malloc(sizeof(node));
    new_n->val = value;
    new_n->connects_nodes = 1;
    new_n->max_depth = 1;
    new_n->left = NULL;
    new_n->right = NULL;
    return new_n;
}

void deallocate (node *root){
    if (root == NULL) return;
    deallocate(root->left);
    deallocate(root->right);
    free(root);
}
   
    
