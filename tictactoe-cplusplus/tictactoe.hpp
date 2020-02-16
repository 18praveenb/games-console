#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <optional>

namespace TicTacToe {
    int int_from_digit(char digit);

    int median(int a, int b, int c);

    struct Point {
        public:
            int r, c;
            Point(int r, int c);
    };

    std::ostream &operator<<(std::ostream &stream, const Point &point);

    enum Player {
        empty = '.', x = 'x', o = 'o'
    };

    class Board {
        public:
            int dim;
            std::vector<Player> board;
            Board(int dim);
            Player& operator()(Point p);
            void print(Point active, bool print_active);
            bool valid(Point p);
            Player winner();
            bool won(Player player);
            Point clamp(Point p);
    };
}