Members:
Jinhong Lin 
Dianzheng Yu


*Running the Program*

The program executable should be named “puzzle”.

There are two ways to invoke the program. If invoked without command line arguments,
./puzzle
then the program should read commands from stdin.

If the program is invoked with a single argument, e.g.
./puzzle input.txt
then the program should read commands from the file named by the command line argument.


*Input Format*

Command	                Description
C size	                Create initial puzzle with size rows and columns
T tile...	            Initialize puzzle configuration with series of tile numbers
I filename	            Load the background image from specified PPM file
P	                    Print sequence of tile numbers reflecting current puzzle configuration
W filename1 filename2	Write puzzle image to filename1 and puzzle configuration to filename2
S direction	            Slide a free tile in specified direction
K	                    Check to see whether the puzzle is in the “winning” configuration
V	                    Compute a series of moves to solve the puzzle
Q	                    Quit the program




*Testing*
The files test01.txt through test09.txt are example inputs. The corresponding example outputs, generated image files, and generated puzzle configuration files are found in the “expected” directory. When you run your puzzle executable on the example inputs, you can compare the generated outputs to the expected files. The compare and diff programs are useful for comparing images and text files. Here is a scenario in which you could run your puzzle program on the test06.txt input and compare the results (note that “$” is used to indicate the command prompt, and the compare commands are on two lines, with the \ character being used as a line continuation):

$ ./puzzle test06.txt > test06_out.txt
$ diff expected/test06_out.txt test06_out.txt
$ compare expected/ingo_scrambled_test06.ppm ingo_scrambled_test06.ppm \
	ingo_scrambled_test06_diff.png
$ diff expected/ingo_scrambled_test06.txt ingo_scrambled_test06.txt
$ compare expected/ingo_solved_test06.ppm ingo_solved_test06.ppm \
	ingo_solved_test06_diff.ppm
$ diff expected/ingo_solved_test06.txt ingo_solved_test06.txt


Example of Program Functionality
This section shows a quick example of how the program works.

The example input shown here is in the file test06.txt included in the starter files, so the following invocation could be used to run the program with this input:

./puzzle test06.txt
The input file test06.txt has the following contents:

C 4
T 5 7 1 2 0 9 3 4 13 8 6 11 14 15 10 12
I ingo.ppm
W ingo_scrambled.ppm ingo_scrambled.txt
S l
S u
S l
S l
S r
S d
S r
S d
S l
S l
S u
S r
S u
S u
S r
S r
S d
S d
S d
S l
S l
S u
S r
S u
S l
S l
S u
W ingo_solved.ppm ingo_solved.txt
The meaning of each of these commands is fully described in the Input Format section below. However, here is a brief summary of what this input does:

Specifies that the puzzle is 4x4 (the C command)
Specifies a particular arrangement of tiles (the T command and the numbers that follow it)
Specifies that ingo.ppm is the background image for the puzzle (the I command)
Saves the initial (scrambled) configuration of the puzzle to image ingo_scrambled.ppm and text file ingo_scrambled.txt (the first W command)
Performs a series of moves (the S commands)
Saves the final solved configuration of the puzzle to the image ingo_solved.ppm and text file ingo_solved.txt (the second W command)
This is the background image (ingo.ppm):

Picture of Ingo the cat
This is the initial scrambled puzzle configuration (the image is ingo_scrambled.ppm):

5	7	1	2
9	3	4
13	8	6	11
14	15	10	12 Scrambled puzzle image

After all of the “S” commands complete, the puzzle is in its “solved” configuration (the image is ingo_solved.ppm):

1	2	3	4
5	6	7	8
9	10	11	12
13	14	15	   Solved puzzle image
