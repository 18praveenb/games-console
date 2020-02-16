#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <optional>
#include <boost/range/irange.hpp>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include "board.hpp"

int int_from_digit(char digit) {
    return (int) (digit - '0');
}

Point::Point(int x, int y):
    x {x},
    y {y}
    {};

std::optional<Point> Point::from_string(std::string str) {
    std::istringstream stream = std::istringstream(str);
    int x, y, mode = 0;
    while (stream.good()) {
        if (std::isdigit(stream.peek())) {
            switch (mode) {
                case 0:
                    x = int_from_digit(stream.get());
                    mode += 1;
                    break;
                case 1:
                    x = 10*x + int_from_digit(stream.get());
                    break;
                case 2:
                    y = int_from_digit(stream.get());;
                    mode += 1;
                    break;
                case 3:
                    y = 10*y + int_from_digit(stream.get());
                    break;
            }
        } else {
            switch (mode) {
                case 0:
                    stream.ignore();
                    break;
                case 1:
                    stream.ignore();
                    mode += 1;
                    break;
                case 2:
                    stream.ignore();
                    break;
                case 3:
                    return std::optional<Point>(Point(x, y));
            }
        }
    }
    return std::optional<Point>();
}

Board::Board(int dim):
    dim {dim},
    board {std::vector(dim*dim, CellState::empty)}
    {};

CellState& Board::operator()(Point p) {
    return board[p.x + dim * p.y];
}

void Board::print() {
    for(int i : boost::irange(0, dim)) {
        for (int j : boost::irange(0, dim)) {
            printw("%c ", (*this)(Point(i, j)));
        }
        printw("\n");
    }
}

bool Board::valid(Point p) {
    return (0 <= p.x) && (p.x < dim) && (0 <= p.y) && (p.y < dim);
}

int main(int argc, char *argv[]) {
    Board board = Board(3);
    Point active = Point(0, 0);
    CellState player = CellState::x;
    printw("Tic Tac Toe");
    while (true) {

    }
}

int main_old(int argc, char *argv[]) {
    Board board = Board(3);
    CellState player = CellState::x;
    printw("Welcome to Tic Tac Toe\n");
    while (true) {
        game_loop:;
        board.print();
        printw("Player %c's turn", player);
        std::string input;
        std::getline(std::cin, input);
        std::optional<Point> move_o = Point::from_string(input);
        if (!move_o) {
            printw("Invalid input %s", input);
            goto game_loop;
        }
        Point move = move_o.value();
        std::cout << "Move to " << move << std::endl;
        if (!board.valid(move)) {
            printw("Invalid coordinate\n");
            goto game_loop;
        }
        if (board(move) != CellState::empty) {
            printw("Square is not empty!\n");
            goto game_loop;
        }
        board(move) = player;
        printw("Success!\n");
        player = player == CellState::x ? CellState::o : CellState::x;
    }
    return 0;
}

int main_demo(int argc, char *argv[]) {
    int x = 0;
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    while (true) {
        mvprintw(0, 0, "%d\n", x);
        int ch = getch();
        if (ch == KEY_LEFT || ch == KEY_DOWN) {
            x -= 1;
        } else if (ch == KEY_UP || ch == KEY_RIGHT) {
            x += 1;
        }
        refresh();
    }
    endwin();

    return 0;
}