#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct elem{
    short int row_pos;
    short int col_pos;
    long int distance;
    short int color;
    struct elem *next;
}q_elem;

typedef struct q{
    q_elem *head;
    q_elem *tail;
    long int num_elem;
}queue;

q_elem* new_elem(short int row_pos, short int col_pos, short int color, long int distance);
queue* init_q(short int rows, short int cols);
void enqueue(q_elem *to_enq);
q_elem* dequeue();
bool is_valid(short int row_pos, short int col_pos, short int color);
void get_shortest_path();

int rows, cols, colors;
short int ***maps;
bool ***visited;
queue *node_q;
int main(int argc, char const *argv[])
{
    scanf("%d %d %d", &rows, &cols, &colors);
    node_q = init_q(rows, cols);
    maps = (short int***)malloc(sizeof(short int**) * (colors + 1));
    visited = (bool***)malloc(sizeof(bool**) * (colors + 1));
    for (int m = 0; m < colors+1; ++m)
    {
        maps[m] = (short int**)malloc(sizeof(short int*) * rows);
        visited[m] = (bool**)malloc(sizeof(bool*) * rows);
        for (int r = 0; r < rows; ++r)
        {
            maps[m][r] = (short int*)malloc(sizeof(short int) * cols);
            visited[m][r] = (bool*)calloc(cols, sizeof(bool));
        }
    }
    short int tmp;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            scanf("%hd", &tmp);
            for (int x = 0; x < colors+1; ++x)
            {
                if (abs(tmp) == x)
                {
                    maps[x][i][j] = 0;
                } else {
                    maps[x][i][j] = tmp; 
                }
            }
        }  
    }
    get_shortest_path();
    for (int d = 0; d < colors+1; ++d)
    {
        for (int e = 0; e < rows; ++e)
        {
            free(maps[d][e]);
            free(visited[d][e]);
        }
        free(maps[d]);
        free(visited[d]);
    }
    free(maps);
    free(visited);
    while (node_q->num_elem > 0)
    {
        free(dequeue());
    }
    free(node_q);

    return 0;
}

void get_shortest_path(){
    q_elem *start = new_elem(rows-1, 0, 0, 0);
    enqueue(start);
    int row_increments[] = {-1, 1, 0, 0};
    int col_increments[] = {0, 0, -1, 1};
    short int new_r, new_c;
    while (node_q->num_elem > 0)
    {
        q_elem *curr = dequeue();
        if (curr->row_pos == 0 && curr->col_pos == cols-1)
        {
            printf("%ld\n", curr->distance);
            free(curr);
            return;
        }
        for (int i = 0; i < 4; ++i)
        {
            new_r = curr->row_pos + row_increments[i];
            new_c = curr->col_pos + col_increments[i];
            if (is_valid(new_r, new_c, curr->color))
            {
                visited[curr->color][new_r][new_c] = true;
                if (maps[curr->color][new_r][new_c] < 0)
                {
                    enqueue(new_elem(new_r, new_c, abs(maps[curr->color][new_r][new_c]), curr->distance+1));
                } else {
                    enqueue(new_elem(new_r, new_c, curr->color, curr->distance+1));
                }
            }
        }
        free(curr);  
    }
}

q_elem* new_elem(short int row_pos, short int col_pos, short int color, long int distance){
    q_elem *new = (q_elem*)malloc(sizeof(q_elem));
    new->row_pos = row_pos;
    new->col_pos = col_pos;
    new->color = color;
    new->distance = distance;
    return new;
}

queue* init_q(short int rows, short int cols){
    queue *new = (queue*)malloc(sizeof(queue));
    new->head = NULL;
    new->tail = NULL;
    new->num_elem = 0;
    return new;
}

void enqueue(q_elem *to_enq){
    if (node_q->num_elem == 0)
    {
        node_q->head = to_enq;
        node_q->tail = to_enq;
    } else {
        node_q->tail->next = to_enq;
        node_q->tail = to_enq;
    }
    node_q->num_elem++;
}

q_elem* dequeue(){
    if (node_q->num_elem == 0) return NULL;
    q_elem *head = node_q->head;
    if (node_q->num_elem == 1)
    {
        node_q->head = NULL;
        node_q->tail = NULL;
    } else {
        node_q->head = node_q->head->next;
    }
    node_q->num_elem--;
    return head;
}

bool is_valid(short int row_pos, short int col_pos, short int color){
    return (row_pos >= 0 && row_pos < rows
         && col_pos >= 0 && col_pos < cols
         && maps[color][row_pos][col_pos] <= 0
         && !visited[color][row_pos][col_pos]);
}

