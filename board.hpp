#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <optional>

int int_from_digit(char digit);

struct Point {
    public:
        int x;
        int y;
        Point(int x, int y);
        static std::optional<Point> from_string(std::string str);
};

enum CellState { empty = '.', x = 'x', o = 'o' };

class Board {
    private:
        int dim;
        std::vector<CellState> board;

    public:
        Board(int dim);
        CellState& operator()(Point p);
        void print();
        bool valid(Point p);
};