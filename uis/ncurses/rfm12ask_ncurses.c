#include <stdio.h>
#include <ncurses.h>
#include <signal.h>
#include "../../lib/core.h"

#define WIDTH 20
#define HEIGHT 10

int n_choices;

void sig_handler(int sig) {
	printf("got signal: %d\n", sig);
    dt_shm();
    exit(0);
}

void print_menu(WINDOW *menu_win, int highlight);

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;
    int ret;

    n_choices = init();

    initscr();
    clear();
    noecho();
    cbreak();   /* Line buffering disabled - pass on everything */

    menu_win = newwin(HEIGHT, WIDTH, 2, 0);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down,\npress enter to switch socket");
    refresh();
    print_menu(menu_win, highlight);
    while(1)
    {
        c = wgetch(menu_win);
        switch(c)
        {   case KEY_UP:
                if(highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
        }
        print_menu(menu_win, highlight);
        if(c != 10)
            continue;
        ret = control(&dev_arr[choice-1], !dev_arr[choice-1].state);
        mvprintw(HEIGHT+2, 0, "switched: '%s' to '%i'\nreturn code: %i", dev_arr[choice-1].label, dev_arr[choice-1].state, ret);
        clrtoeol();
        refresh();
        print_menu(menu_win, highlight);
    }
}

void print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i)
    {   if(highlight == i + 1)
        {   wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, x, "[%i]  %s", dev_arr[i].state, dev_arr[i].label);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "[%i]  %s", dev_arr[i].state, dev_arr[i].label);
        ++y;
    }
    wrefresh(menu_win);
}
