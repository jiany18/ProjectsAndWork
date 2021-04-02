
#ifndef SUDOKU_H
#define SUDOKU_H

#include <array>
#include <iostream>
#include <string>

const int MAX = 9;

class Sudoku {
private:
    /**
     * the 9X9 grid of the sudoku puzzle with an integer value from 0-9 assigned to each. "0"
     * means that the specific location does not have a solution yet
     */
    int content[MAX][MAX];

    /**
     * A recursive function that tries all possibility at a location and fill in the solution
     * once we find one. Return true if a possibility can be found.
     *
     * @param col   the column of the location we try to solve
     * @param row   the row of the location we try to solve
     * @return      true if the given location is solvable
     */
    bool solve(int col, int row);

    /**
     * Determine if a number can be put in the specified location without contradicting other
     * numbers in the same row, column, or box
     *
     * @param col   the column of the specified location
     * @param row   the row of the specified location
     * @param num   the number we try to put into the specified location
     * @return      true if the number can be put in the location without contradicting other
     * existing numbers
     */
    bool solvable(int col, int row, int num);

    /**
     * Check a specific column if a specific num is already present
     *
     * @param col   the column to be checked
     * @param num   the number to be checked
     * @return      true if the number isn't found in the column; false if we found the number in
     * the given column
     */
    bool checkCol(int col, int num);

    /**
     * Check a specific row if a specific num is already present
     *
     * @param row   the row to be checked
     * @param num   the number to be checked
     * @return      true if the number isn't found in the row; false if we found the number in
     * the given row
     */
    bool checkRow(int row, int num);

    /**
     * Check if a number is already presented in the 3x3 box of the given location
     *
     * @param col   the column of the location
     * @param row   the row of the location
     * @param num   the number to be checked
     * @return      true if the number isn't found in the box; false if the number is present in
     * the box
     */
    bool checkBox(int col, int row, int num);

    /**
     * Set a certain location to the content value of 0
     *
     * @param col   the column of the location
     * @param row   the row of the location
     */
    void erase(int col, int row);

public:
    /**
     * Default constructor. Initialize the object with an empty puzzle
     */
    Sudoku();

    /**
     * re initialize the object with a new puzzle from the specified file
     *
     * @param filename the file where we get the new puzzle
     */
    void loadFromFile(std::string filename);

    /**
     * find the solution of the puzzle
     *
     * @return true if a solution can be found and false otherwise
     */
    bool solve();

    /**
     * Check if two puzzles are the same
     *
     * @param other the other sudoku puzzle to be compared to
     * @return      true if the two puzzles are the same and false otherwise
     */
    bool equals(const Sudoku& other) const;

    /**
     * prints out the output stream in a specific format
     * @param out    output stream
     * @param sudoku the sudoku puzzle to be printed
     * @return       puzzle printed out in our preferred format
     */
    friend std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku);
};

#endif // SUDOKU_H
