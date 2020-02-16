#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <optional>
#include <boost/range/irange.hpp>
#include <ncurses.h>
#include "board.hpp"

namespace TicTacToe {
    int int_from_digit(char digit) {
        return (int) (digit - '0');
    }

    Point::Point(int r, int c): r {r}, c {c} {};

    std::ostream &operator<<(std::ostream &stream, const Point &point) {
        return stream << "(" << point.r << ", " << point.c << ")";
    }

    Board::Board(int dim):
        dim {dim},
        board {std::vector(dim*dim, Player::empty)} {};

    Player& Board::operator()(Point p) {
        return board[p.r + dim * p.c]; 
    }

    void Board::print(Point active, bool print_active) {
        for(int i : boost::irange(0, dim)) {
            for (int j : boost::irange(0, dim)) {
                if (!print_active && i == active.r && j == active.c) {
                    printw("  ");
                }
                if (print_active || i != active.r || j != active.c) {
                    attron(A_BOLD);
                    printw("%c ", (*this)(Point(i, j)));
                    attroff(A_BOLD);
                }
            }
            printw("\n");
        }
    }

    Player Board::winner() {
        if (this->won(Player::x)) {
            return Player::x;
        } else if (this->won(Player::o)) {
            return Player::o;
        } else {
            return Player::empty;
        }
    }

    bool Board::won(Player player) {
        // Verticals
        for (int c : boost::irange(0, dim)) {
            bool won_column = true;
            for (int r : boost::irange(0, dim)) {
                if ((*this)(Point(r, c)) != player) {
                    won_column = false;
                }
            }
            if (won_column) {
                return true;
            }
        }

        // Horizontals
        for (int r : boost::irange(0, dim)) {
            bool won_row = true;
            for (int c : boost::irange(0, dim)) {
                if ((*this)(Point(r, c)) != player) {
                    won_row = false;
                }
            }
            if (won_row) {
                return true;
            }
        }

        // Diagonals
        bool d1 = true;
        bool d2 = true;
        for (int d : boost::irange(0, dim)) {
            if ((*this)(Point(d, d)) != player) {
                d1 = false;
            }
            if ((*this)(Point(dim - 1 - d, d)) != player) {
                d2 = false;
            }
        }
        return d1 || d2;
    }

    bool Board::valid(Point p) {
        return (0 <= p.r) && (p.r < dim) && (0 <= p.c) && (p.c < dim);
    }

    Point Board::clamp(Point p) {
        return Point(std::clamp(p.r, 0, dim - 1),
                    std::clamp(p.c, 0, dim - 1));
    }
}
int main(int argc, char *argv[]) {
    using namespace TicTacToe;
    Board board = Board(3);
    Point active = Point(0, 0);
    unsigned int frame = 0;
    Player player = Player::x;
    std::string title, info = "Select a move...";
    
    initscr();
    nodelay(stdscr, true);
    noecho();
    keypad(stdscr, true);
    
    while (board.winner() == Player::empty) {
        std::ostringstream title_stream;
        title_stream << (char) player << " turn";
        title = title_stream.str();
        mvaddstr(0, 0, title.c_str());
        printw("\n");
        board.print(active, (frame % 30) < 16);
        mvprintw(1 + board.dim, 0, info.c_str());
        printw("\n");
        
        int input = getch();
        switch (input) {
            case KEY_LEFT:
            case 'a':
                active.c -= 1;
                break;
            case KEY_RIGHT:
            case 'd':
                active.c += 1;
                break;
            case KEY_UP:
            case 'w':
                active.r -= 1;
                break;
            case KEY_DOWN:
            case 's':
                active.r += 1;
                break;
            case '\n':
            case ' ':
                if (board(active) == Player::empty) {
                    board(active) = player;
                    player = player == Player::x ? Player::o : Player::x;
                    info = "Select a move...";
                } else {
                    info = "Invalid move!";
                }
                break;
        }

        flushinp();
        refresh();
        
        frame += 1;
        active = board.clamp(active);
        
        napms(16);
    }
    
    std::ostringstream win_stream;
    win_stream << "Player " << (char) board.winner() << " won!";
    std::string win_message = win_stream.str();
    mvprintw(1 + board.dim, 0, win_message.c_str());
    
    refresh();
    timeout(-1);
    getchar();
    
    endwin();
}