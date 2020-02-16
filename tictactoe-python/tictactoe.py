#!/usr/bin/env python3
import click
import curses

def median(a, b, c):
    return min(a, max(b, c)) if a > b else min(b, max(a, c))

@click.command()
@click.argument('board-size')
def tictactoe(board_size):
    window = curses.initscr()
    curses.noecho()
    window.nodelay(True)
    window.keypad(True)
    
    try:
        board_size = int(board_size)
    except ValueError:
        print(f"Invalid board size {board_size}")
        exit(1)
    
    board = [['.'] * board_size for _ in range(board_size)]
    player = 'x'
    r, c, frame = 0, 0, 0
    info = "Select a move..."

    def draw_board():
        for i in range(board_size):
            for j in range(board_size):
                if frame % 30 < 16 and i == r and c == j:
                    window.addstr("  ")
                else:
                    window.addstr(f'{board[i][j]} ')
            window.addstr("\n")
    
    def won(player):
        rows = [True] * board_size
        cols = [True] * board_size
        diag = [True, True]
        for i in range(board_size):
            for j in range(board_size):
                if board[i][j] != player:
                    rows[i] = False
                    cols[j] = False
                    if i == j:
                        diag[0] = False
                    if i == board_size - 1 - j:
                        diag[1] = False
        return any([any(rows), any(cols), any(diag)])
    
    def winner():
        return 'x' if won('x') else 'o' if won('o') else '.'
    
    while winner() == '.':
        window.clear()
        
        inp = window.getch()
        if inp in [ord('w'), curses.KEY_UP]:
            r -= 1
        elif inp in [ord('d'), curses.KEY_RIGHT]:
            c += 1
        elif inp in [ord('s'), curses.KEY_DOWN]:
            r += 1
        elif inp in [ord('a'), curses.KEY_LEFT]:
            c -= 1
        elif inp in [ord(' '), ord('\n')]:
            if board[r][c] == '.':
                info = "Selct a move..."
                board[r][c] = player
                player = 'x' if player == 'o' else 'o'
            else:
                info = "Invalid move!"
        
        r = median(0, r, board_size - 1)
        c = median(0, c, board_size - 1)
        
        window.addstr(f"{player}'s turn\n")
        draw_board()
        window.addstr(info)
        
        window.refresh()
        frame = (frame + 1) % 3600
        curses.napms(16)
    
    window.clear()
    window.addstr(f'{winner()} won!\n')
    draw_board()
    window.addstr('Press any key to quit.\n')
    window.refresh()
    window.timeout(-1)
    window.getch()
    curses.endwin()

if __name__ == '__main__':
    tictactoe()