// Name: Yumeng Jiang
// VUnetID: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS 3270 - Vanderbilt University
// Project 1 - Sudoku.cpp
// Date:03/05/2021
// Honor statement:I did not give or receive any unauthorized aid on this assignment.

#ifndef SUDOKU_CPP
#define SUDOKU_CPP

#include <Sudoku.h>
#include <array>
#include <fstream>
#include <iostream>

Sudoku::Sudoku()
{
    for (int i = 0; i < MAX; ++i) {
        for (int j = 0; j < MAX; ++j) {
            content[i][j] = 0;
        }
    }
}

void Sudoku::loadFromFile(std::string filename)
{
    std::ifstream infile(filename);
    for (int i = 0; i < MAX; ++i) {
        for (int j = 0; j < MAX; ++j) {
            infile >> content[i][j];
        }
    }
    infile.close();
}

bool Sudoku::solve()
{
    return solve(0, 0);
}

bool Sudoku::solve(int col, int row)
{
    if (col >= MAX || row >= MAX) {
        return true;
    }
    if (content[col][row] != 0) {
        if (row == MAX - 1) {
            return solve(col + 1, 0);
        } else {
            return solve(col, row + 1);
        }
    }
    bool worked = false;
    for (int num = 1; num <= MAX && !worked; num++) {
        if (solvable(col, row, num)) {
            content[col][row] = num;
            if (row == MAX - 1) {
                worked = solve(col + 1, 0);
            } else {
                worked = solve(col, row + 1);
            }
            if (!worked) {
                erase(col, row);
            }
        }
    }
    return worked;
}

void Sudoku::erase(int col, int row)
{
    content[col][row] = 0;
}

bool Sudoku::solvable(int col, int row, int num)
{
    bool noAttack = checkCol(col, num) && checkRow(row, num) && checkBox(col, row, num);
    return noAttack;
}

bool Sudoku::checkCol(int col, int num)
{
    for (int i = 0; i < MAX; i++) {
        if (content[col][i] == num) {
            return false;
        }
    }
    return true;
}

bool Sudoku::checkRow(int row, int num)
{
    for (int i = 0; i < MAX; i++) {
        if (content[i][row] == num) {
            return false;
        }
    }
    return true;
}

bool Sudoku::checkBox(int i, int j, int num)
{
    for (int a = (i - i % 3); a < (i - i % 3 + 3); a++) {
        for (int b = (j - j % 3); b < (j - j % 3 + 3); b++) {
            if (content[a][b] == num) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::equals(const Sudoku& other) const
{
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (content[i][j] != other.content[i][j]) {
                return false;
            }
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku)
{
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            int cur = sudoku.content[i][j];
            if (cur == 0) {
                out << "  ";
            } else {
                out << cur << " ";
            }
            if (j == 2 || j == 5) {
                out << "| ";
            }
        }
        out << "\n";
        if (i == 2 || i == 5) {
            out << "------+-------+------\n";
        }
    }
    return out;
}

#endif // SUDOKU_CPP