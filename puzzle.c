#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"
#include "puzzle.h"

int main(int argc, char **argv) {
  if (argc < 1 || argc > 2) {
    fprintf(stderr, "Usage: ./puzzle [<command file>]\n");
    return 1;
  }

  Puzzle * input_puzzle = NULL;
  Image * background = NULL;
  FILE* in = NULL;
  FILE* bg_in = NULL;
  char cmd; // receive commands 
  int ret; // receive return from fscanf

  if (argc == 2) {
    in = fopen(*(argv+1), "r");
  } else {
    in = stdin;
  } 

  if (in == NULL) {
    fprintf(stderr, "Error: Cannot open the file.\n");
    return 1;
  }

  while (fscanf(in, " %c", &cmd) == 1) {

    if (cmd == 'C') {
      
      ret = handle_C_command(in, &input_puzzle);

      if (ret == 1) { // returning 1 means invalid size
        fprintf(stderr, "Invalid puzzle size\n");
        return 1;
      }
      if (ret == 2) { // returning 2 means invalid input
        fprintf(stderr, "Invalid input\n");
        return 1;
      }
    }

  
    else if (cmd == 'T') {
     
      if (input_puzzle == NULL) { // if no puzzle has been initialized
        fprintf(stderr, "No puzzle\n");
        return 1;
      }
      ret = handle_T_command(in, input_puzzle);

      if (ret == 1) { // returning 1 means invalid tile value
        fprintf(stderr, "Invalid tile value\n");
        return 1;
      }

      if (ret == 2) { // returning 2 means invalid input
        fprintf(stderr, "Invalid input\n");
        return 1;
      }
    }

    else if (cmd == 'I') {
     
      char ppm_filename[256]; //maximum 255 characters

      if (fscanf(in, "%s", ppm_filename) != 1) {
        fprintf(stderr, "Invalid input\n"); 
        return 1;
      }

      bg_in = fopen(ppm_filename, "rb");

      if (bg_in == NULL) {
        fprintf(stderr, "Could not open image file '%s'\n", ppm_filename);
        return 1;
      }

      //Create the image struct that stores the background image 
      background = ReadPPM(bg_in);

      if (background == NULL) {
        fprintf(stderr, "Could not open image file '%s'\n", ppm_filename);
        image_destroy(background);
        return 1; // if we fail to read PPM data 
      }

    }

    else if (cmd == 'P') {
      
      if (input_puzzle == NULL) { // if no puzzle has been initialized
        fprintf(stderr, "No puzzle\n");
        return 1;
      }
      handle_P_command(input_puzzle);
    }

    else if (cmd == 'W') {
      

      if (input_puzzle == NULL) { // if no puzzle has been initialized
        fprintf(stderr, "No puzzle\n");
        return 1;
      }

      if (background == NULL) {
        fprintf(stderr, "No image\n");
        return 1; 
      }


      //Initializing the filenames for storing image and puzzle configuration outputs 
      char filename_im[256];
      char filename_pu[256];

      //Read in the filenames
      //Check errors 
      //return 1 means invalid input
      if (fscanf(in, "%s %s", filename_im, filename_pu) != 2) {
          fprintf(stderr, "Invalid input\n");
          return 1;
      }

      ret = handle_W_command(input_puzzle, background, filename_im, filename_pu);
      
      //All the required output errors for W_command
      if (ret == 2) { 
        fprintf(stderr, "Invalid image dimensions\n");
        return 1;
      } else if (ret == 3) {
        fprintf(stderr, "Could not open output image file '%s'\n", filename_im);
        return 1;
      } else if (ret == 4) {
        fprintf(stderr, "Could not open output puzzle file '%s'\n", filename_pu);
        return 1;
      } else if (ret == 5) {
        fprintf(stderr, "Could not write puzzle data '%s'\n", filename_pu);
        return 1;
      }

    } 

    else if (cmd == 'S') {
      
    // assume we call S after C and T command
    // no need to check if the puzzle exists

      char dir;
      if (fscanf(in, " %c", &dir) != 1) {
        fprintf(stderr, "Invalid input\n"); 
        return 1;
      }
      ret = handle_S_command(input_puzzle, dir);
      if (ret == 1) { // returning 1 means cannot move the puzzle
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 1;
      }

      if (ret == 2) { // returning 2 means invalid input
        fprintf(stderr, "Invalid input\n");
        return 1;
      }
    }

    else if (cmd == 'K') {
      
      if (input_puzzle == NULL) { // if no puzzle has been initialized
        fprintf(stderr, "No puzzle\n");
        return 1;
      }
      ret = is_solved(input_puzzle); // check to see if the puzzle is in the solved state
      if (ret == 0) {
        printf("Solved\n");
      }
      else if (ret == 1) {
        printf("Not solved\n");
      }
    }

    else if (cmd == 'V') {
      if (input_puzzle == NULL) { // if no puzzle has been initialized
        fprintf(stderr, "No puzzle\n");
        return 1;
      }
      handle_V_command(input_puzzle);
    }

    else if (cmd == 'Q') {
      fclose(in);
      if (bg_in != NULL) {
        fclose(bg_in);
      }
      if (background != NULL) {
        image_destroy(background);
      }
      if (input_puzzle != NULL) {
        puzzle_destroy(&input_puzzle);
      }
      return 0; // quits the program
    }
    
    else { //Invalid command letter 
      fprintf(stderr, "Invalid command '%c'\n", cmd);
      fclose(in);
      if (bg_in != NULL) {
        fclose(bg_in);
      }
      if (background != NULL) {
        image_destroy(background);
      }
      if (input_puzzle != NULL) {
        puzzle_destroy(&input_puzzle);
      }
      return 1;
    }
  }

  if (bg_in != NULL) {
      fclose(bg_in);
  }
  if (background != NULL) {
    image_destroy(background);
  }
  if (input_puzzle != NULL) {
    puzzle_destroy(&input_puzzle);
  }
  fclose(in);
  return 0;
}