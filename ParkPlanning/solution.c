#include <stdio.h>
#include <stdlib.h>

int** malloc_matrix(int area_size);
void load_area_construct_pref(int area_size, int **trees, int **rocks);
int get_answer(int **rock_pref, int **tree_pref,int area_size, int park_size, int middle_distance, int min_rocks);

int main(int argc, char **argv){
    int area_size;
    int park_size;
    int middle_distance;
    int min_rocks;

    scanf("%d %d %d %d", &area_size, &park_size, &middle_distance, &min_rocks);

    int **trees = malloc_matrix(area_size);
    int **rocks = malloc_matrix(area_size);

    load_area_construct_pref(area_size, trees, rocks);

    int ret = get_answer(rocks, trees, area_size, park_size, middle_distance, min_rocks);

    printf("%d\n", ret);

    for (int i = 0; i < area_size+1; i++)
    {
        free(trees[i]);
        free(rocks[i]);
    }

    free(rocks);
    free(trees);
    return EXIT_SUCCESS;
}

int** malloc_matrix(int area_size){
    int **row_holder = (int**)malloc((area_size+1) * sizeof(int*));
    for (int i = 0; i < area_size+1; ++i)
    {
        row_holder[i] = (int*)calloc(area_size+1, sizeof(int));
    }
    return row_holder;
}

void load_area_construct_pref(int area_size, int **trees, int **rocks){
    int tree_increment[] = {0,1,0};
    int rock_increment[] = {0,0,1};
    int tmp;
    for (int i = 1; i < area_size+1; ++i)
    {
        for (int j = 1; j < area_size+1; ++j)
        {
            scanf("%d", &tmp);
            trees[i][j] = trees[i-1][j] + trees[i][j-1] - trees[i-1][j-1] + tree_increment[tmp];
            rocks[i][j] = rocks[i-1][j] + rocks[i][j-1] - rocks[i-1][j-1] + rock_increment[tmp];
        }
    }
}

int get_answer(int **rock_pref, int **tree_pref,int area_size, int park_size, int mid_dist, int min_rocks){
    int mid_len = park_size - 2 * mid_dist;
    int reduced_size = area_size - mid_dist;
    int best_tree_num = -1;
    int num_trees = 0;
    int num_rocks = 0;

    for (int row = mid_dist+mid_len; row < reduced_size+1; ++row)
    {
        for (int col = mid_dist+mid_len; col < reduced_size+1; ++col)
        {
            num_rocks = rock_pref[row][col] - rock_pref[row-mid_len][col] - rock_pref[row][col-mid_len] + rock_pref[row-mid_len][col-mid_len];
            num_trees = tree_pref[row+mid_dist][col+mid_dist] - tree_pref[row-mid_dist-mid_len][col+mid_dist] - tree_pref[row+mid_dist][col-mid_dist-mid_len] + tree_pref[row-mid_dist-mid_len][col-mid_dist-mid_len];
            if ((num_rocks >= min_rocks) && (num_trees > best_tree_num))
            {
                best_tree_num = num_trees;
            }
        }   
    }
    return best_tree_num;
}
