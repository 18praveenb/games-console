#!/usr/bin/env python3
import click
import curses

@click.command()
@click.argument('p0name')
@click.argument('p1name')
def minegame(p0name, p1name):
    window = curses.initscr()
    curses.noecho()
    window.keypad(True)
    
    game_state = [{'soldier': 100, 'mine': 5}, {'soldier': 100, 'mine': 5}]

    info = "Select a move..."

    def draw_board():
        window.addstr(f"{p0name}: {game_state[0]}\n")
        window.addstr(f"{p1name}: {game_state[1]}\n")
    
    def lost(player):
        return (game_state[player]['soldier'] == 0
                and game_state[player]['mine'] == 0)
    
    def winner():
        return p0name if lost(0) else p1name if lost(1) else None
    
    def get_move(pname, pstate):
        window.clear()
        window.addstr(f"{pname}'s turn\n")
        draw_board()
        window.refresh()
        while True:
            inp = window.getstr().decode(encoding='utf-8')
            if inp.isdigit() and pstate['soldier'] >= int(inp):
                inp = int(inp)
            elif inp == 'mine' and pstate['mine'] > 0:
                inp = 'mine'
            else:
                window.addstr(f"invalid move {inp}\n")
                inp = None
            if inp is not None:
                window.addstr(f"confirm {inp} [y/n]?\n")
                confirm = chr(window.getch())
                if confirm == 'y':
                    return inp
                else:
                    print(f"deconfirmed {confirm}")
        window.refresh()
    
    while winner() == None:
        p0move = get_move(p0name, game_state[0])
        p1move = get_move(p1name, game_state[1])

        window.clear()
        window.addstr(f"{p0name} used {p0move}\n")
        window.addstr(f"{p1name} used {p1move}\n")
        soldiers = [0, 0]
        if p0move == 'mine':
            game_state[0]['mine'] -= 1
        else: 
            soldiers[0] = p0move
        if p1move == 'mine':
            game_state[1]['mine'] -= 1
        else:
            soldiers[1] = p1move
        
        if soldiers[0] >= soldiers[1] or p0move == 'mine':
            game_state[1]['soldier'] -= soldiers[1]
        if soldiers[1] >= soldiers[0] or p1move == 'mine':
            game_state[0]['soldier'] -= soldiers[0]
        
        draw_board()
        inp = window.getstr().decode(encoding='utf-8')
        
        window.refresh()
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
    minegame()