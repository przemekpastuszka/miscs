import curses

screen = curses.initscr()

curses.noecho()
curses.cbreak()
screen.keypad(1)

win = curses.newwin(100, 100, 0, 0)

curses.nocbreak();
screen.keypad(1)
curses.echo()

curses.endwin()
