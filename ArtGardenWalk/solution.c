#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// #pragma GCC optimize("Ofast")

typedef struct c
{
    int score;
    int dist;
}cell;

int rows, cols;

bool is_inside(int r_pos, int c_pos){
    if (r_pos < 0 || r_pos >= rows || c_pos < 0 || c_pos >= cols) return false;
    return true;
}

short** prep_map(short** restrict map){
    short **new_map = (short**)malloc(rows * sizeof(short*));
    for (size_t i = 0; i < rows; ++i)
    {
        new_map[i] = (short*)malloc(cols * sizeof(short));
    }
    int dirs[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            if (map[r][c] == 0)
            {
                new_map[r][c] = 0;
                for (size_t i = 0; i < 4; ++i)
                {
                    if (is_inside(r+dirs[i][0], c+dirs[i][1]))
                    {
                        new_map[r][c] += map[r+dirs[i][0]][c+dirs[i][1]];
                    }
                }
            } else {
                new_map[r][c] = SHRT_MIN;
            }
        }  
    }
    for (size_t j = 0; j < rows; ++j)
    {
        free(map[j]);
    }
    free(map);
    return new_map;
}

cell get_cell(cell c1, cell c2, cell c3){
    cell arr[3] = {c1, c2, c3}; 
    cell ret = c1;
    for (size_t i = 0; i < 3; i++)
    {
        if (arr[i].score > ret.score || (arr[i].score == ret.score && arr[i].dist < ret.dist))
        {
            ret = arr[i];
        }   
    }
    //printf("chose sc %d dis %d\n", ret.score, ret.dist);
    return ret;
}

cell getmax(short** restrict map, cell*** restrict dp){

    // 1 means moving right
    int dp_r, dp_c;
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            dp_r = r+1;
            dp_c = c+1;
            if (map[r][c] == SHRT_MIN)
            {
                dp[1][dp_r][dp_c].score = INT_MIN;
                dp[1][dp_r][dp_c].dist = INT_MAX;
            } else {
                cell chosen = get_cell(dp[0][dp_r-1][dp_c], dp[1][dp_r-1][dp_c], dp[1][dp_r][dp_c-1]);
                dp[1][dp_r][dp_c].score = map[r][c] + chosen.score;
                dp[1][dp_r][dp_c].dist += chosen.dist; 
            }
            if (map[r][cols-1-c] == SHRT_MIN)
            {
                dp[0][dp_r][cols-c].score = INT_MIN;
                dp[0][dp_r][cols-c].dist = INT_MAX;
            } else {
                cell chosen = get_cell(dp[0][r][cols-c], dp[1][r][cols-c], dp[0][dp_r][cols-c+1]);
                dp[0][dp_r][cols-c].score = map[r][cols-1-c] + chosen.score;
                dp[0][dp_r][cols-c].dist += chosen.dist;
            }
        }
    }
    cell ret = dp[0][rows][1];
    for (size_t i = 1; i < cols+1; i++)
    {
        if (dp[0][rows][i].score > ret.score || (dp[0][rows][i].score == ret.score && dp[0][rows][i].dist < ret.dist))
        {
            ret = dp[0][rows][i];
        }
        if (dp[1][rows][i].score > ret.score || (dp[1][rows][i].score == ret.score && dp[1][rows][i].dist < ret.dist))
        {
            ret = dp[1][rows][i];
        }
        //printf("current ret sc %d dis %d\n", ret.score, ret.dist);
    }
    return ret;
}

void print_map(short **map){
    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            printf("%hd ",map[r][c]); 
        } 
        printf("\n"); 
    }
}

void printdp(cell **map){
    for (size_t r = 0; r < rows+2; ++r)
    {
        for (size_t c = 0; c < cols+2; ++c)
        {
            printf("|%d %d|",map[r][c].score, map[r][c].dist); 
        } 
        printf("\n"); 
    }
}

int main() {
    scanf("%d %d", &rows, &cols);
    short **map = (short**)malloc(rows * sizeof(short*));
    for (size_t i = 0; i < rows; ++i)
    {
        map[i] = (short*)malloc(cols * sizeof(short)); 
    }
    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            scanf("%hi", &(map[r][c]));
        }  
    }
    map = prep_map(map);

    cell ***dp = (cell***)malloc(2 * sizeof(cell**));
    dp[0] = (cell**)malloc((rows+2) * sizeof(cell*));
    dp[1] = (cell**)malloc((rows+2) * sizeof(cell*));
    for (size_t j = 0; j < rows + 2; ++j)
    {
        dp[0][j] = (cell*)malloc((cols+2) * sizeof(cell));
        dp[1][j] = (cell*)malloc((cols+2) * sizeof(cell));
        for (size_t k = 0; k < cols+2; ++k)
        {
            dp[0][j][k].score = 0;
            dp[1][j][k].score = 0;
            dp[0][j][k].dist = 1;
            dp[1][j][k].dist = 1;
        }
        dp[0][j][0].score = INT_MIN;
        dp[1][j][0].score = INT_MIN;
        dp[0][j][cols+1].score = INT_MIN;
        dp[1][j][cols+1].score = INT_MIN;
        dp[0][j][0].dist = INT_MAX;
        dp[1][j][0].dist = INT_MAX;
        dp[0][j][cols+1].dist = INT_MAX;
        dp[1][j][cols+1].dist = INT_MAX;
    }
    cell fin = getmax(map, dp);
    printf("%d %d\n", fin.score, fin.dist-1);
    // printdp(dp[0]);
    //     printf("XXXXXXXXX\n");
    // printdp(dp[1]);

    for (size_t j = 0; j < rows + 2; ++j)
    {
        free(dp[0][j]);
        free(dp[1][j]);
    }
    free(dp[0]);
    free(dp[1]);
    free(dp);
    for (size_t j = 0; j < rows; ++j)
    {
        free(map[j]);
    }
    free(map);
    return 0;
}
 