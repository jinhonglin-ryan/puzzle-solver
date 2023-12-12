#ifndef PUZZLE_H
#define PUZZLE_H
#include "ppm_io.h" // for Image data type

typedef struct {
  int size; 
  int* tiles;

} Puzzle;

Puzzle *puzzle_create(int size);
void puzzle_destroy(Puzzle **p);
void puzzle_set_tile(Puzzle *p, int col, int row, int value);
int puzzle_get_tile(const Puzzle *p, int col, int row);

//C size: Create initial puzzle with size rows and columns  
int handle_C_command(FILE* in, Puzzle **p);
//T tile: Initialize puzzle configuration with series of tile numbers
int handle_T_command(FILE* in, Puzzle *p);
//P: Print sequence of tile numbers reflecting current puzzle configuration
int handle_P_command(Puzzle *p);
//W filename 1 filename: Write puzzle image to filename 1 and puzzle configuration to filename2
int handle_W_command(Puzzle *p, Image* im, const char filename_im[256], const char filename_pu[256]);
//S direction: slide a free tile in specified direction
int handle_S_command(Puzzle *p, char dir);
//K: Check to see whether the puzzle is in the "winning" configuration
int is_solved(Puzzle *p);
//V: Compute a series of moves to solve the puzzle
int handle_V_command(Puzzle *p);
//Function to find a solution to solve the puzzle
int solve_puzzle(Puzzle* p, char steps[], int max_steps, int cur_steps);
//Calculate the manhattan distance of a given puzzle 
int compute_manhattan_distance(int* tiles, int size); 
void image_destroy (Image * im);

#endif // PUZZLE_H