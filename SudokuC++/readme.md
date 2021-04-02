> **The Academic Honor Policy (see Brightspace) is applicable to all work you do in CS 3270/5270.**

# CS 3270: Programming Languages
## Project 1

## Goal

Gain experience in advanced C++ OOP programming and recursive backtracking algorithms.

## GitHub notes

**Important:** you will need to download the **Google Test submodule** after you have cloned your repository. This can be done in CLion with your Project 1 project open. Select **View | Tool Windows | Terminal** and beside the prompt, type the following command:

```
git submodule add https://github.com/google/googletest.git lib/googletest
```

The GoogleTest submodule will be cloned into the `lib/googletest` folder in your project. When you commit, the `lib` folder will be checked. That is okay, go ahead and commit and push with the `lib` folder. The submodule is also needed in your repository on GitHub in order for the tests to run on GitHub. If you have any problems with this step, please contact the instructor.

## Assignment

Create a **Sudoku** solver for a standard 9x9 puzzle.

Sudoku is a popular puzzle where you fill in numbers on a grid, trying to keep certain conditions true. To learn more about how Sudoku works, check out <http://en.wikipedia.org/wiki/Sudoku>. You'll find a sample puzzle and an explanation of the rules.

Write a C++ class that reads a file containing an unfinished Sudoku puzzle, then solves the puzzle using a recursive backtracking algorithm.

The input file will simply contain the numbers in the puzzle delimited by spaces on each line, where zeroes are used to indicate unknowns. For example, the example puzzle below (from <http://commons.wikimedia.org/wiki/Image:Sudoku-by-L2G-20050714.gif>) would be represented in an input as:

```
5 3 0 0 7 0 0 0 0
6 0 0 1 9 5 0 0 0
0 9 8 0 0 0 0 6 0
8 0 0 0 6 0 0 0 3
4 0 0 8 0 3 0 0 1
7 0 0 0 2 0 0 0 6
0 6 0 0 0 0 2 8 0
0 0 0 4 1 9 0 0 5
0 0 0 0 8 0 0 7 9
```

Using public methods available in the `Sudoku` class that you will implement, we can show the execution of solving a Sudoku puzzle with the code below.

```C++
// Create object.
Sudoku puzzle;
puzzle.loadFromFile("../txt/sudoku-test1.txt");

// Print initial Sudoku puzzle.
std::cout << "Puzzle:\n\n" << puzzle;

// Solve puzzle. Print solved puzzle or "No solution".
if (puzzle.solve()) {
    std::cout << puzzle << std::endl;
} else {
    std::cout << "No Solution" << std::endl;
}
```

The output of the code above is given below. The output format of the puzzle (both unsolved and solved) should match this example output.

```
Puzzle:

  4 3 |   8   | 2 5
6     |       |
      |     1 |   9 4
------+-------+------
9     |     4 |   7
      | 6   8 |
  1   | 2     |     3
------+-------+------
8 2   | 5     |
      |       |     5
  3 4 |   9   | 7 1

Solution:

1 4 3 | 9 8 6 | 2 5 7
6 7 9 | 4 2 5 | 3 8 1
2 8 5 | 7 3 1 | 6 9 4
------+-------+------
9 6 2 | 3 5 4 | 1 7 8
3 5 7 | 6 1 8 | 9 4 2
4 1 8 | 2 7 9 | 5 6 3
------+-------+------
8 2 1 | 5 6 7 | 4 3 9
7 9 6 | 1 4 3 | 8 2 5
5 3 4 | 8 9 2 | 7 1 6
```

Note: it is okay to have trailing whitespace at the end of each line. For example, it is okay to have three trailing whitespaces after `5` on the first line of the unsolved puzzle above.

## Functional specifications

You are to create a C++ class called `Sudoku`. This class should be implemented in the `Sudoku.h` and `Sudoku.cpp` files. The public interface of this class is as follows:

* `Sudoku()`: Default constructor. Should initialize the object with an empty puzzle (81 zeroes).
* `void loadFromFile(std::string filename)`: Reinitializes the object with a new puzzle from the specified file. The passed string will be the relative path of the text file (e.g., `"../txt/sudoku1-test.txt"`). You may assume the input file has the specified format of nine rows of nine digits separated by a space, and the numbers represent a valid Sudoku puzzle. Note: It is recommended that you use the extraction operator to read in the values, as it will automatically skip all white spaces (blanks, newlines) for you.
* `bool solve()`: The entry point for your solver. Returns `true` if a solution was found, otherwise returns `false`. If no solution was found, you may leave the puzzle in either (1) its initial state, or (2) the state when you determined no solution was possible.
* `bool equals(const Sudoku& other) const`: Determines if two puzzles are equal. Two puzzles are equal if the values in each corresponding cell are the same.
* `std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku)`: A *friend* function that prints to the output stream `out` the current puzzle contents in a nicely formatted manner as seen in the example above. Instead of printing to `std::cout`, print to the passed output stream `out` and return it.

**DO NOT** add or remove any item in the public interface other than additional constructors or destructors (a destructor is not listed above, because our solution does not use any dynamic memory). You may add items in the private interface (e.g., helper functions).

## Debugging

You may want to debug your code as you try to solve a problem in your code without running Google Test. The `main.cpp` file allows the execution of a program that simply tries to solve one puzzle. You can specify the text file containing a puzzle and ask the program to solve that single puzzle. To enable this, select **Run | Edit Configurations...**. On the left select **gtest**. Enter a value for **Program arguments**. This can be any value, a `1` will do. Then run the project.

![Edit configuration](images/clion-edit-configuration.png)

Enter the name of the text file. The program assumes the file is located in the `txt` folder, so you just need to enter the file name. If you just press `<Enter>`, the text file `sudoku-test1.txt` will be used.

Below is an example execution:

```
Enter puzzle text file (assumes file is in "txt" folder).
Pressing <Enter> will run the file "sudoku-test1.txt".
sudoku-test2.txt

Puzzle:

8   3 |   2 9 | 7 1 6
    6 |   1 8 | 5   4
      |   6   |     8
------+-------+------
    5 |   4 6 |   8
7   9 |   3 5 | 6 4 2
  6   |   9   | 1   5
------+-------+------
6     |   7   |   5 1
    1 | 6 5   | 8
5     | 9 8 1 | 4 6 3

Solution:

8 4 3 | 5 2 9 | 7 1 6
9 7 6 | 3 1 8 | 5 2 4
1 5 2 | 7 6 4 | 3 9 8
------+-------+------
3 1 5 | 2 4 6 | 9 8 7
7 8 9 | 1 3 5 | 6 4 2
2 6 4 | 8 9 7 | 1 3 5
------+-------+------
6 9 8 | 4 7 3 | 2 5 1
4 3 1 | 6 5 2 | 8 7 9
5 2 7 | 9 8 1 | 4 6 3
```

To run the tests in Google Test again, remove the value from **Program arguments**. Below is the expected output of the tests in Google Test (timings removed):

```
[==========] Running 5 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 5 tests from SudokuTest
[ RUN      ] SudokuTest.EqualsMethod
[       OK ] SudokuTest.EqualsMethod
[ RUN      ] SudokuTest.Print
[       OK ] SudokuTest.Print
[ RUN      ] SudokuTest.SolveOne
[xxxxxxxxxx] Manual timing:
[       OK ] SudokuTest.SolveOne
[ RUN      ] SudokuTest.SolveMultiple
[xxxxxxxxxx] Manual timing:
[xxxxxxxxxx] Manual timing:
[       OK ] SudokuTest.SolveMultiple
[ RUN      ] SudokuTest.Unsolvable
[xxxxxxxxxx] Manual timing:
[       OK ] SudokuTest.Unsolvable
[----------] 5 tests from SudokuTest

[----------] Global test environment tear-down
[==========] 5 tests from 1 test case ran.
[  PASSED  ] 5 tests.
```

## Style guidelines

* **Names:** Use proper conventions for identifier names. That is, use `UpperCamelCase` for class names, `SCREAMING_SNAKE_CASE` for constants, and `camelCase` for everything else. Also, use meaningful identifier names. For example, a variable that stores a speed value should be called `speed` instead of just `s`.

* **Constants:** Use proper constant variables rather than literal values.

* **Line lengths:** No lines should exceed 100 columns.

* **Indentation:** Use proper and consistent indentation.

* **Whitespace:** Methods should be separated by at least one empty line.

* **Global variables:** Prohibited.

* **Comments:** Use Javadoc-style comments to document all methods. You may place all the method header comments either in the `.cpp` or in the `.h` file, but please be consistent by choosing one file to put the comments in. CLion can help construct a comments template to document a method. On the empty line right above the method header, type `/**` and then hit **Enter**. A template will be generated that includes lines for parameters and any return value. You can then complete the description of these items in addition to a short description of what the method does.

   For your reference, below is an example of Javadoc-style comments for a method.

   ```
  /**
   * Returns an Image object that can then be painted on the screen.
   *
   * @param  url  An absolute URL giving the base location of the image.
   * @param  name The location of the image, relative to the url argument.
   * @return      The image at the specified URL.
   */
  ```

## Grading

For comparison purposes, we will time your solution program. We want this information so that we can compare our C++ code against solutions written in other programming languages. How fast your program finds a solution will not be a part of your grade – unless your solution is grossly inefficient. We will define "grossly inefficient" as being 5x slower than our simple-minded solver, or one that exhausts all stack space. Note that we will test your submission on several more puzzles in addition to the three puzzles provided for you.

An executable version of our solution is available on Brightspace under **Content | Assignments** in the `solution-executables.zip` file. The output of the program includes the time it took to solve a puzzle. You can use this to compare with your solution. Note that the program will ask for the name of the text file containing the puzzle that you want it to solve. The text file should be in the same folder as the executable.

For MacOS users, you may need to change the access permissions for the solution file using the following command in the terminal (navigate to the folder where the file is located first):

```
chmod +x solutionMAC
```

Then to run the file, type:

```
./solutionMAC
```

## Deliverables

To make sure everyone is making sufficient progress on this project, we will have two deadlines. The required deliverables for each deadline is given below.

* **Checkpoint deadline**: Submission of code to date on GitHub. Your checkpoint submission should pass the `EqualsMethod` and `Print` test cases. These tests are in `tests/SudokuTest.cpp`. For these two test cases to pass, you will need to have completed the following: the constructor, the `loadFromFile()` method, the `equals()` method, and the `operator<<` function.

  Make sure to commit and push to GitHub as we will check that your submission passes the checkpoint by confirming that the `EqualsMethod` and `Print` test cases passed on GitHub. You can confirm that the two tests passed on GitHub by clicking on the **Actions** tab. Then under **All workflows** click on **Project 1** of the first listed item. Then select either **Clang** or **GCC** on the left. Then on the right expand the **Run.** step. You can look at the output to confirm that the two tests passed.

  Note that at this point the `solve` method does not need to be fully functional and hence it is okay if the test cases `SolveOne`, `SolveMultiple`, and `Unsolvable` do not pass yet (and the overall report of the build shows a red x-mark). However, in order to pass the `EqualsMethod` and `Print` tests, your program still needs to compile. This means that the `solve` method needs to exist. You may define your `solve` method and simply return `false`.

  Failure to have made progress to this point will cost you 10% of your final grade on the project. Checkpoint submissions must be pushed to GitHub by the deadline (i.e., you can't submit the checkpoint submission late).

* **Final deadline**: Final project submission.

## Academic honesty

As stated in class, there are many solutions to Sudoku in many different programming languages available on the Internet. Do not look at the code you may find there. Using code that you find on the Internet is unethical, and of course you would miss the learning opportunity that you get by developing this yourselves. This instructor will report any violations to the university's Honor Council.

## Final notes

* Ensure that your name, VUnetID, email address, and the honor code appear in the header comments at the top of `Sudoku.cpp` and `Sudoku.h`.

* We will run your submission on additional hidden test cases during grading.

* Be sure to use *include guards* in your `Sudoku` header file. See header files of previous assignments for an example.

* You are required to implement `Sudoku.cpp` and `Sudoku.h`. Do not alter any of the existing project files!

* All files necessary to compile and run your program must reside in the GitHub.com repository.

* For full credit, your program must compile with a recent version of the Clang and GNU C++ compilers and run successfully on GitHub. You will have to push to GitHub in order to trigger a build.
