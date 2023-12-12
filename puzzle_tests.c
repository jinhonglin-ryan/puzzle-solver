#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "puzzle.h"
#include "ppm_io.h"
#include <unistd.h>

//Tests puzzle_create and puzzle_destroy
void test_puzzle_create(void) {

  Puzzle *p1 = puzzle_create(3);
  Puzzle *p2 = puzzle_create(5);

  assert(p1->size == 3);
  for (int i = 0; i < 9; ++i) {
    assert(p1->tiles[i] == 0);
  }

  assert(p2->size == 5);
  for (int i = 0; i < 25; ++i) {
    assert(p2->tiles[i] == 0);
  }

  puzzle_destroy(&p1);
  puzzle_destroy(&p2);

  assert(p1 == NULL);
  assert(p2 == NULL);

}

//Tests puzzle_set_tile and puzzle_get_tile
void test_puzzle_set_tile(void) {
  int init[4][4] = {
    { 5, 7, 1, 2 },
    { 13, 9, 3, 4 },
    { 0, 8, 6, 11 },
    { 14, 15, 10, 12 },
  };

  Puzzle *p3 = puzzle_create(4);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      puzzle_set_tile(p3, j, i, init[i][j]);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      assert(puzzle_get_tile(p3, j, i) == init[i][j]);

  puzzle_destroy(&p3);
}

// ...other test functions...

// //Tests C_command
// void test_puzzle_C_command(void) {
//   Puzzle *p4 = puzzle_create(3);
//   FILE* in = fopen("testC.txt", "r");
//   assert(in != NULL);

//   //Call and check C_command
//   int return_value = handle_C_command(in, &p4);
//   assert(return_value == 0);
//   assert(p4->size >= 2);
//   assert(p4->size <= 20);
//   assert(p4->size == 5);

//   puzzle_destroy(&p4);
// }

// //Tests T_command
// void test_puzzle_T_command(void) {

//   // Test case 1: valid input
//   Puzzle *p5 = puzzle_create(3);
//   FILE* in1 = fopen("testT1.txt", "r");
//   int res1 = handle_T_command(in1, p5);
//   fclose(in1);
//   assert(res1 == 0);
//   assert(p5->tiles[0] == 1);
//   assert(p5->tiles[1] == 2);
//   assert(p5->tiles[2] == 3);
//   assert(p5->tiles[3] == 4);
//   assert(p5->tiles[4] == 5);
//   assert(p5->tiles[5] == 6);
//   assert(p5->tiles[6] == 7);
//   assert(p5->tiles[7] == 8);
//   assert(p5->tiles[8] == 0);
//   puzzle_destroy(&p5);


//   //Test case 2: invalid input (too few tile numbers)
//   Puzzle *p6 = puzzle_create(3);
//   FILE* in2 = fopen("testT2.txt", "r");
//   int res2 = handle_T_command(in2, p6);
//   fclose(in2);
//   assert(res2 == 2);
//   puzzle_destroy(&p6);
// }

// //Tests P_command
// void test_puzzle_P_command(void) {
//   int p_size = 4;

//   //Initializing a puzzle
//   Puzzle *p7 = puzzle_create(p_size);
//   for (int i=0; i<16; i++) {
//     (p7->tiles)[i] = i+1;
//   }
  
//   //Call and check P_command
//   assert(handle_P_command(p7) == 0);
//   puzzle_destroy(&p7); 
// }

// //Tests W_command
// void test_puzzle_W_command(void) {
//   int p_size = 4;
//   Puzzle *p8 = puzzle_create(p_size);
//   for (int i=0; i<16; i++) {
//     (p8->tiles)[i] = i+1;
//   }
//   (p8->tiles)[0] = 0;

//   FILE* in = fopen("testW.txt", "r");
//   assert(in != NULL);
//   Image* im = malloc(sizeof(Image *));
//   im->rows = 300;
//   im->cols = 300;
//   im->data = malloc(300 * 300 * sizeof(Pixel));
//   Pixel pix = {255, 0, 0};
//   for (int i=0; i<900; i++) {
//     (im->data)[i] = pix;
//   }

//   //Write scrambled image to file1
//   FILE* out3 = fopen("testWori.ppm", "w");
//   WritePPM(out3, im);
//   fclose(out3);

  
//   //Call and check W_command
//   int return_value = handle_W_command(in, p8, im);
//   assert(return_value == 0);

//   //Check if the expected files were created 
//   assert(access("testW_image.ppm", F_OK) == 0);
//   assert(access("testW_puzzle.txt", F_OK) == 0);

//   //Clean up the test files
//   //remove("testW_image.ppm");
//   //remove("testW_puzzle.txt");

//   fclose(in);
//   puzzle_destroy(&p8); 
//   free(im->data);
//   free(im);
// }

//Tests S_command
void test_puzzle_S_command(void) {
  
  //Initializing a puzzle to have this configuration
  int init[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 12, 13, 14, 15, 16};

  Puzzle *p9 = puzzle_create(4);
  for (int i=0; i<16; i++) {
    p9->tiles[i] = init[i];
  }
  assert(handle_S_command(p9, 'u') == 0);
  assert(p9->tiles[14] == 0);
  assert(handle_S_command(p9, 'd') == 0);
  assert(p9->tiles[10] == 0);
  assert(handle_S_command(p9, 'l') == 0);
  assert(p9->tiles[11] == 0);
  assert(handle_S_command(p9, 'r') == 0);
  assert(p9->tiles[10] == 0);

  puzzle_destroy(&p9); 
}


//Tests is_solved_command
void test_is_solved(void) {

  //Initializing a puzzle that is in solved status
  Puzzle *p10 = puzzle_create(4);
  for (int i=0; i<16; i++) {
    p10->tiles[i] = i+1;
  }
  
  //Assign a gap in a random place in the puzzle 
  //Check if each of them is still perceived as solved  
  
  for (int i=0; i<16; i++) {
    p10->tiles[i] = 0;
    assert(is_solved(p10) == 0);
    //Fill the gap with original number to prepare for next check
    p10->tiles[i] = i+1;
  }
  
  puzzle_destroy(&p10); 
}


//Tests V_command
void test_puzzle_V_command(void) {

  //Initializing a puzzle to have this configuration
  int init[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 12, 13, 14, 15, 16};

  Puzzle *p11 = puzzle_create(4);
  for (int i=0; i<16; i++) {
    p11->tiles[i] = init[i];
  }

  //Move around the gap and check if 
  //the smallest number of moves is right 
  handle_S_command(p11, 'u');
  for (int i = 0; i < 16; i++) {
    printf("%d ", p11->tiles[i]);
  }
  printf("\n");
  assert(handle_V_command(p11) == 1);


  handle_S_command(p11, 'r');
  for (int i = 0; i < 16; i++) {
    printf("%d ", p11->tiles[i]);
  }
  printf("\n");
  assert(handle_V_command(p11) == 1);
  handle_S_command(p11, 'r');
  for (int i = 0; i < 16; i++) {
    printf("%d ", p11->tiles[i]);
  }
  printf("\n");
  assert(handle_V_command(p11) == 1);
  handle_S_command(p11, 'd');
  for (int i = 0; i < 16; i++) {
    printf("%d ", p11->tiles[i]);
  }
  printf("\n");
  assert(handle_V_command(p11) == 1);
  
  puzzle_destroy(&p11); 
}


//Tests compute_manhattan_distance 
// void test_manhattan(void) {
//   //Test case 1
//   int tiles1[4] = {1, 2, 3, 4};
//   assert(compute_manhattan_distance(tiles1, 2) == 2);

//   //Test case 2
//   int tiles2[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
//   assert(compute_manhattan_distance(tiles2, 3) == 0);

//   //Test case 3
//   int tiles3[9] = {2, 8, 3, 1, 6, 4, 7, 0, 5};
//   assert(compute_manhattan_distance(tiles3, 3) == 16);
// }



//Tests solve_puzzle 
void test_solve_puzzle(void) {
  
    //Test case 1
    Puzzle* p12 = puzzle_create(3);
    int tiles1[9] = {1, 2, 3, 4, 5, 6, 7, 0, 8};
    memcpy(p12->tiles, tiles1, 9*sizeof(int));
    char steps1[10];
    int result1 = solve_puzzle(p12, steps1, 10, 0);
    assert(result1 == 1);
    printf("%c \n", steps1[0]);
    puzzle_destroy(&p12);


    //Test case 2
    Puzzle* p13 = puzzle_create(3);
    int tiles2[9] = {1, 2, 3, 4, 5, 6, 0, 7, 8};
    memcpy(p13->tiles, tiles2, 9*sizeof(int));
    char steps2[10];
    int result2 = solve_puzzle(p13, steps2, 10, 0);
    assert(result2 == 2);
    printf("%c \n", steps2[0]);
    printf("%c \n", steps2[1]);
    puzzle_destroy(&p13);


    // //Test case 3
    // Puzzle* p14 = puzzle_create(3);
    // int tiles3[9] = {1, 2, 3, 4, 5, 6, 8, 7, 0};
    // memcpy(p14->tiles, tiles3, 9*sizeof(int));
    // char steps3[100];
    // int result3 = solve_puzzle(p14, steps3, 100, 0);
    // assert(result3 == 0);
    // assert(strcmp(steps2, "") == 0);
    // puzzle_destroy(&p14);

    //Ryan test case: 
    int tile4[9] = {1, 3, 6, 4, 5, 9, 7, 0, 8};
    Puzzle *p15 = puzzle_create(3);
    memcpy(p15->tiles, tile4, 9*sizeof(int));
    char * steps = malloc(sizeof(char) * 4);
    int i = solve_puzzle(p15, steps, 10, 0);
    printf("%d\n",i);
    for (int i = 0; i < 4; i++) {
        printf("%c ", steps[i]);
    }
    for (int i = 0; i < 9; i++) {
        printf("%d ", p15->tiles[i]);
    }
    //assert(is_solved(p15) == 0);
    puzzle_destroy(&p15);
}

int main(void) {
  test_puzzle_create();
  test_puzzle_set_tile();
  // ...call other test functions...

//   test_puzzle_C_command();
//   test_puzzle_T_command();
//   test_puzzle_P_command();
//   test_puzzle_W_command();
  test_puzzle_S_command();
  test_is_solved();
  test_puzzle_V_command();
//   test_manhattan();
  test_solve_puzzle();

  printf("All tests passed!\n");
  return 0;
}
