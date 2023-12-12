#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "puzzle.h"
#include <time.h>
#include "ppm_io.h"

// Create a puzzle
Puzzle *puzzle_create(int size) {
    Puzzle* p = (Puzzle*)malloc(sizeof(Puzzle));
    p->size = size;
    p->tiles = (int*)calloc(size*size, sizeof(int)); 
    return p;
}

//Destroy a puzzle 
void puzzle_destroy(Puzzle **p) {
    free((*p)->tiles);
    (*p)->tiles = NULL;
    free(*p);
    *p = NULL;
    p = NULL;
}

void image_destroy (Image * im) {
  free(im->data);  // free the pixel data first
  free(im);        // then free the image struct
}

//Set a value for the chosen tile
void puzzle_set_tile(Puzzle *p, int col, int row, int value) {
    (p->tiles)[row*(p->size)+col] = value;
}

//Get the value of the chosen tile
int puzzle_get_tile(const Puzzle *p, int col, int row) {
    int tile_value = (p->tiles)[row*(p->size)+col];
    return tile_value;
}

//C size: Create initial puzzle with size rows and columns  
int handle_C_command(FILE* in, Puzzle **p) {
    int num_size;
    //Read the input from file
    //Return 1 if the size is invalid
    //Return 2 if the input of size is invalid
    if (fscanf(in, "%d", &num_size) != 1) {
        return 2;
    }
    if (num_size < 2 || num_size > 20) {
        return 1;
    }
    //create the puzzle
    *p = puzzle_create(num_size);
    return 0;
}

//T tile: Initialize puzzle configuration with series of tile numbers
int handle_T_command(FILE* in, Puzzle *p) {
    int row;
    int col;
    int temp_num;
    int p_size = p->size;
    int tile_size = p_size * p_size;
    for (row = 0; row < p_size; row++) {
        for (col = 0; col < p_size; col++) {
            int ret = fscanf(in, "%d", &temp_num);
            if (ret != 1) {
                return 2; // fail to scan an integer, invalid input
            }
            if (temp_num < 0 || temp_num > tile_size) {
                return 1; // scanned integer is not in the valid range
            }
            puzzle_set_tile(p, col, row, temp_num); // if no error, set the current tile to the input value
        }
    }
    return 0;
}


//P: Print sequence of tile numbers reflecting current puzzle configuration
int handle_P_command(Puzzle *p) {
    int p_size = p->size;
    int tile_size = p_size*p_size; 
    for (int i=0; i<tile_size; i++) {
        printf("%d ", (p->tiles)[i]);
    }
    printf("\n");
    return 0;
}


int handle_W_command(Puzzle *p, Image* im, const char filename_im[256], const char filename_pu[256]) {

    int check_row_valid = im->rows%p->size;
    int check_col_valid = im->cols%p->size;
    
    if (check_row_valid!=0 || check_col_valid!=0) { 
        return 2; // background image rows or columns not evenly divisible by puzzle rows/columns
    }

    FILE* out1 = fopen(filename_im, "wb");
    if (!out1) {
        return 3; //Output image file can’t be opened	
    }

    FILE* out2 = fopen(filename_pu, "w");
    if (!out2) {
        return 4; // Output puzzle configuration file can’t be opened
    }


    //Initialize important parameters for use
    int p_size = p->size;
    int block_row_size = im->rows/p_size;
    int block_col_size = im->cols/p_size;
    int background_row_size = im->rows;
    int background_col_size = im->cols;

    //Array for referring to the row position of the corners of each block
    int row_corner_pos[p_size*p_size];
    for (int i=0; i<p_size; i++) {
        for (int j=0; j<p_size; j++) {
            row_corner_pos[i*p_size+j] = i;
        }
    }

    //Array for reffering to the col position of the corners of each block
    int col_corner_pos[p_size*p_size];
    for (int i=0; i<p_size; i++) {
        for (int j=0; j<p_size; j++) {
            col_corner_pos[i*p_size+j] = j;
        }
    }

    //The scrambled image that will be outputted as ppm
    Image * output_im = (Image *)malloc(sizeof(Image)); 
    output_im->data = malloc(background_row_size*background_col_size*sizeof(Pixel)); 
    output_im->rows = background_row_size; 
    output_im->cols = background_col_size; 

    //Iterating through each 
    for (int i=0; i<p_size; i++) {
        for (int j=0; j<p_size; j++) {

            //A pointer to pixels that store the original image
            Pixel * ori_pix = im->data; 
            //A pointer to pixels that store the scrambled image 
            Pixel * scram_pix = output_im->data; 

            //Output pixel array
            scram_pix += (p_size*block_row_size*block_col_size*i)+(block_col_size*j);

            //Check the tile number for this part of pixels
            int pu_tile_num = (p->tiles)[i*p_size+j];
            if (pu_tile_num != 0) {
                //Find the corresponding pixels from the original image
                int row_corner = row_corner_pos[pu_tile_num-1];
                int col_corner = col_corner_pos[pu_tile_num-1];
                ori_pix += (p_size*block_row_size*block_col_size*row_corner)+(block_col_size*col_corner);

                for (int c=0; c<block_row_size; c++) {
                    for (int d=0; d<block_col_size; d++) {
                        (*(scram_pix+d)).r = (*(ori_pix+d)).r;
                        (*(scram_pix+d)).b = (*(ori_pix+d)).b;
                        (*(scram_pix+d)).g = (*(ori_pix+d)).g;

                    }
                    scram_pix += p_size*block_row_size;
                    ori_pix += p_size*block_row_size;
                }

            }

            else { // if the tile number is gap number, set it to black
                for (int c=0; c < block_row_size; c++) {
                    for (int d=0; d < block_col_size; d++) {
                        (*(scram_pix+d)).r = 0;
                        (*(scram_pix+d)).g = 0;
                        (*(scram_pix+d)).b = 0;
                    }
                    scram_pix += p_size*block_row_size;
                }
            }
        }
    }
    
    WritePPM(out1, output_im);
    fclose(out1);

    //Check if we can successfully write the puzzle configuration data
    for (int i=0; i<p->size * p->size; i++) {
        if (fprintf(out2, "%d ", p->tiles[i]) < 1) {
            image_destroy(output_im);
            fclose(out2);
            return 5; // return 5 indicate we can't write the puzzle config data
        }
    }

    fclose(out2);
    image_destroy(output_im);

    return 0;
}


//S direction: slide a free tile in specified direction
// return 0 if successful
// return 2 if the direction is invalid, not one of d, u, l, r
// return 1 if we fail to move in the direction 
int handle_S_command(Puzzle *p, char dir) { 
    int p_size = p->size;
    int tile_size = p_size*p_size;
    int gap_pos = 0;
    
    // find the gap
    for(int i = 0; i < tile_size; i++) {
        if ((p->tiles)[i] == 0) {
            gap_pos = i;
            break;
        }
    }

    // determine the tile to swap with the gap
    int swap_pos;
    switch(dir) {
        case 'd': // down
            if (gap_pos < p_size) {
                // gap is already in the top row, cannot move up
                return 1;
            }
            swap_pos = gap_pos - p_size;
            break;
        case 'u': // up
            if (gap_pos >= tile_size - p_size) {
                // gap is already in the bottom row, cannot move down
                return 1;
            }
            swap_pos = gap_pos + p_size;
            break;
        case 'r': // right
            if (gap_pos % p_size == 0) {
                // gap is already in the leftmost column, cannot move left
                return 1;
            }
            swap_pos = gap_pos - 1;
            break;
        case 'l': // left
            if (gap_pos % p_size == p_size - 1) {
                // gap is already in the rightmost column, cannot move right
                return 1;
            }
            swap_pos = gap_pos + 1;
            break;
        default:
            // invalid direction
            return 2;
    }

    // swap the gap with the chosen tile
    int temp = (p->tiles)[swap_pos];
    (p->tiles)[swap_pos] = 0;
    (p->tiles)[gap_pos] = temp;

    return 0;
}
//K: Check to see whether the puzzle is in the "winning" configuration
//Return 1 if it is not in the winning condition
//Return 0 if it is in the winning condition
int is_solved(Puzzle *p) {
    int expected_tile = 1;
    for (int i = 0; i < p->size * p->size; i++) {
        if (p->tiles[i] != expected_tile && p->tiles[i] != 0) { // regardless of where the gap position is at 
            return 1;
        }
        expected_tile++;
    }
    return 0;
}

//V: Compute a series of moves to solve the puzzle
// return the number of steps needed to solve the puzzle
int handle_V_command(Puzzle *p) {
    char steps[40];
    int num_steps = solve_puzzle(p, steps, 40, 0);
    if (num_steps == -1) {
        printf("No solution found\n");
        return -1;
    }
    for (int i = 0; i < num_steps; i++) {
        printf("S %c\n", steps[i]);
    }
    return num_steps;
}


// return the manhattan distance of a given puzzle 
int compute_manhattan_distance(int* tiles, int size) {
    int distance = 0;
    for (int i = 0; i < size*size; i++) {
        if (tiles[i] != 0) {
            // Calculate the row and column of the tile in its solved position
            int target_row = (tiles[i]-1) / size;
            int target_col = (tiles[i]-1) % size;
            // Calculate the current row and column of the tile
            int current_row = i / size;
            int current_col = i % size;
            // Add the absolute difference between the current and target rows and columns to distance
            distance += abs(target_row - current_row) + abs(target_col - current_col);
        }
    }
    return distance;
}


// return the minimum number of steps required to solve the puzzle
// return -1 if no solution can be found
int solve_puzzle(Puzzle* p, char steps[], int max_steps, int cur_steps) {
    // if the puzzle is already solved, return current number of steps
    if (!is_solved(p))
        return cur_steps;

    // if the max number of steps has been reached, it means we can't solve the puzzle
    if (cur_steps >= max_steps)
        return -1;

    // Initialize an array to store the Manhattan distance for each of the four directions
    int distances[4];
    int min_distance = INT_MAX;
    int min_index = -1;

    //Try each of the four possible moves.
    for (int i = 0; i < 4; i++) {
        char direction = "udlr"[i];
        Puzzle* copy = puzzle_create(p->size);
        memcpy(copy->tiles, p->tiles, p->size * p->size * sizeof(int));

        // Apply the direction to the copy and check if it is valid
        if (!handle_S_command(copy, direction)) {
            distances[i] = compute_manhattan_distance(copy->tiles, copy->size);
            if (distances[i] < min_distance) {
                min_distance = distances[i];
                min_index = i;
            }
        }
        free(copy);
    }

    // If all four directions fail, return -1 to indicate failure
    if (min_index == -1) {
        return -1;
    }

    // Apply the direction with the minimum distance
    char direction = "udlr"[min_index];
    Puzzle* copy = puzzle_create(p->size);
    memcpy(copy->tiles, p->tiles, p->size * p->size * sizeof(int));
    handle_S_command(copy, direction);

    // Store the direction with the minimum distance in the array 
    steps[cur_steps] = direction;
    int result = solve_puzzle(copy, steps, max_steps, cur_steps+1);
    free(copy);
    return result;
}

