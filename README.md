# Puzzle Solver

## Members
- Jinhong Lin
- Dianzheng Yu

## Example of Program Functionality

### Example Input (test06.txt)
```plaintext
C 4
T 5 7 1 2 0 9 3 4 13 8 6 11 14 15 10 12
I ingo.ppm
W ingo_scrambled.ppm ingo_scrambled.txt
S l
S u
S l
...
S u
W ingo_solved.ppm ingo_solved.txt
```

This input configures the puzzle, specifies the background image, performs a series of moves, and saves the initial and final configurations.

### Background Image
![Example Image](https://github.com/jinhonglin-ryan/Puzzle_Solver/blob/main/puzzle_background/ingo.png)

### Initial Scrambled Puzzle Configuration
![Example Image](https://github.com/jinhonglin-ryan/Puzzle_Solver/blob/main/puzzle_background/ingo_scrambled.png)

### Final Solved Puzzle Configuration
![Example Image](https://github.com/jinhonglin-ryan/Puzzle_Solver/blob/main/puzzle_background/ingo_solved.png)


## Running the Program

The executable for the program should be named "puzzle."

### Invocation without Command Line Arguments
If invoked without command line arguments:
```bash
./puzzle
```
The program reads commands from stdin.

### Invocation with a Single Argument
If invoked with a single argument:
```bash
./puzzle input.txt
```
The program reads commands from the file named by the command line argument.

## Input Format

### Commands

- `C size`: Create an initial puzzle with `size` rows and columns.
- `T tile...`: Initialize the puzzle configuration with a series of tile numbers.
- `I filename`: Load the background image from the specified PPM file.
- `P`: Print the sequence of tile numbers reflecting the current puzzle configuration.
- `W filename1 filename2`: Write the puzzle image to `filename1` and the puzzle configuration to `filename2`.
- `S direction`: Slide a free tile in the specified direction.
- `K`: Check whether the puzzle is in the "winning" configuration.
- `V`: Compute a series of moves to solve the puzzle.
- `Q`: Quit the program.

## Testing

The files `test01.txt` through `test09.txt` are example inputs. Corresponding example outputs, generated image files, and generated puzzle configuration files are found in the "expected" directory. You can run the puzzle executable on the example inputs and compare the generated outputs to the expected files using commands like `diff` and `compare`.

Example:
```bash
$ ./puzzle test06.txt > test06_out.txt
$ diff expected/test06_out.txt test06_out.txt
$ compare expected/ingo_scrambled_test06.ppm ingo_scrambled_test06.ppm \
    ingo_scrambled_test06_diff.png
$ diff expected/ingo_scrambled_test06.txt ingo_scrambled_test06.txt
$ compare expected/ingo_solved_test06.ppm ingo_solved_test06.ppm \
    ingo_solved_test06_diff.ppm
$ diff expected/ingo_solved_test06.txt ingo_solved_test06.txt
```

Feel free to explore and test the Puzzle Solver using different inputs!