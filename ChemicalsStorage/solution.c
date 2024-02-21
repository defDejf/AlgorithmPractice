#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct node
{
    bool is_valid;
    int current_balance;
    int box_cost;
    int distance;
    int box_sum;
    struct node *parent;
    struct node *left;
    struct node *right;
}node;

typedef struct result{
    int cost;
    int balance;
}res;

node* new_node();

void update_balance(node *to_update);

res get_res(int box_len, int box_idx, short int* box_arr, int node_len, node **node_arr, res best);

void put_box(node *where, int box_val);

void remove_box(node *where, int box_val);

res new_res(int balance, int cost);

void get_current(node **node_arr, int node_len, res *old);

void free_nodes(int node_len, node **node_arr);

int main(int argc, char *argv[]){
    int num_rooms, num_boxes;
    scanf("%d %d", &num_rooms, &num_boxes);
    short int *box_arr = (short int*)malloc(num_boxes * sizeof(short int));

    for (int i = 0; i < num_boxes; ++i)
    {
        scanf("%hd", &box_arr[i]);
    }

    node **node_arr = (node**)malloc(sizeof(node*) * num_rooms);
    for (int j = 0; j < num_rooms; ++j)
    {
        node_arr[j] = new_node();
    }
    int from, to, cost;
    for (int k = 0; k < num_rooms-1; ++k)
    {
        scanf("%d %d %d", &from, &to, &cost);
        if (node_arr[from]->left == NULL)
        {
            node_arr[from]->left = node_arr[to];
            node_arr[to]->parent = node_arr[from];
            node_arr[to]->distance += node_arr[from]->distance + cost;
            node_arr[from]->is_valid = false;
        } else {
            node_arr[from]->right = node_arr[to];
            node_arr[to]->parent = node_arr[from];
            node_arr[to]->distance += node_arr[from]->distance + cost;
            node_arr[from]->is_valid = false;
        }
    }

    res best_out = new_res(0, 0);
    res fin = get_res(num_boxes, 0, box_arr, num_rooms, node_arr, best_out);
    printf("%d %d\n", fin.balance, fin.cost);
    free_nodes(num_rooms, node_arr);
    free(box_arr);
    
    return EXIT_SUCCESS;
}

node* new_node(){
    node *new = (node*)malloc(sizeof(node));
    new->is_valid = true;
    new->box_sum = 0;
    new->box_cost = 0;
    new->distance = 0;
    new->current_balance = 0;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

res new_res(int balance, int cost){
    res new = {balance, cost};
    return new;
}

res get_res(int box_len, int box_idx, short int* box_arr, int node_len, node **node_arr, res best){
    if (box_idx == box_len)
    {
        return best;
    }
    res tmp_best = {INT_MAX, INT_MAX};
    for (int i = 0; i < node_len; ++i)
    {
        if (node_arr[i]->is_valid)
        {
            put_box(node_arr[i], box_arr[box_idx]);
            get_current(node_arr, node_len, &best);
            res recur = get_res(box_len, box_idx+1, box_arr, node_len, node_arr, best);
            if (recur.balance < tmp_best.balance)
            {
                tmp_best = recur;
            } else if ((recur.balance == tmp_best.balance) && (recur.cost < tmp_best.cost))
            {
                tmp_best = recur;
            }
            remove_box(node_arr[i], box_arr[box_idx]);
        }
    }
    return tmp_best;
}

void put_box(node *where, int box_val){
    //printf("NODE %d\n", where->id);
    where->box_sum += box_val;
    where->box_cost += where->distance;
    if (where->parent != NULL)
    {
        where->parent->is_valid = true;
        if (where->parent->left != NULL && where->parent->left->box_sum == 0)
        {
            where->parent->is_valid = false;
        }
        if (where->parent->right != NULL && where->parent->right->box_sum == 0)
        {
            where->parent->is_valid = false;
        }
    }
    if (where->left != NULL)
    {
        where->left->is_valid = false;
    }
    if (where->right != NULL)
    {
        where->right->is_valid = false;
    }
    update_balance(where);
}

void remove_box(node *where, int box_val){
    where->box_sum -= box_val;
    where->box_cost -= where->distance;
    if (where->parent != NULL)
    {
        if (where->box_sum == 0)
        {
            where->parent->is_valid = false;
        }
        where->current_balance = abs(where->box_sum - where->parent->box_sum);
    }
    if (where->box_sum == 0)
    {
        if (where->right != NULL)
        {
            where->right->is_valid = true;
        }
        if (where->left != NULL)
        {
            where->left->is_valid = true;
        }
    }
    update_balance(where);
}

void update_balance(node *to_update){
    if (to_update->left != NULL)
    {
        to_update->left->current_balance = abs(to_update->box_sum - to_update->left->box_sum);
    }
    if (to_update->right != NULL)
    {
        to_update->right->current_balance = abs(to_update->box_sum - to_update->right->box_sum);
    }
    if (to_update->parent != NULL)
    {
        to_update->current_balance = abs(to_update->box_sum - to_update->parent->box_sum);
    }
}

void get_current(node **node_arr, int node_len, res *old){
    int balance = 0;
    int cost = 0;
    for (int i = 0; i < node_len; ++i)
    {
        balance += node_arr[i]->current_balance;
        cost += node_arr[i]->box_cost;
    }
    old->balance = balance;
    old->cost = cost;
}

void free_nodes(int node_len, node **node_arr){
    for (int i = 0; i < node_len; ++i)
    {
        free(node_arr[i]);
    }
    free(node_arr);
}
