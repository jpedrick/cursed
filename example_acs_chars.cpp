//  Copyright notice:
//  This file is part of Cursed.
//  Cursed is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Cursed is also covered by GNU Affero General Public License with the 
//  following permission under the GNU Affero GPL version 3 section 7:
//      If you modify this Program, or any covered work, by linking or
//      combining it with other code, such other code is not for that reason
//      alone subject to any of the requirements of the GNU Affero GPL
//      version 3.
//
//  Cursed is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Cursed.  If not, see <https://www.gnu.org/licenses/>.
//
//  You should also have received a copy of the GNU Affero General Public License
//  along with Cursed.  If not, see <https://www.gnu.org/licenses/>.
#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include "Draw.hpp"
#include "Window.hpp"
#include "Color.hpp"
#include "UTF.hpp"


int main(void)
{
    ::setlocale(LC_ALL, "");
    WINDOW* win = initscr();
    ::start_color();
    cursed::Window cwin( cursed::Direction::Horizontal);

    const cursed::Color red   = {1000,    0 ,   0 };
    const cursed::Color green = {   0, 1000 ,   0 };
    const cursed::Color blue  = {   0,    0 ,1000 };
    {
        cursed::ColorPair blueRed(blue, red);
        auto attrs = cursed::Draw::AttributeGuard{ &cwin, blueRed | A_STANDOUT | A_UNDERLINE | A_BLINK };
        mvprintw(0,7,"NCURSES EXTENDED CHARACTERS\n");
    }
    wprintw(win,"colors: %d, color_pairs: %d\n", COLORS, COLOR_PAIRS);
    wprintw(win,"Unicode EURO character \u20ac\n");
    wprintw(win,"Unicode Black Up Arrow %s\n", cursed::UTF16::blackArrow[(int)cursed::UTF16::BlackArrow::Up] );
    wprintw(win,"Unicode Black Up Arrow %s\n", cursed::UTF16::blackArrow[(int)cursed::UTF16::BlackArrow::Left] );
    wprintw(win,"Unicode Black Up Arrow %s\n", cursed::UTF16::blackArrow[(int)cursed::UTF16::BlackArrow::Right] );
    wprintw(win,"Upper left corner         "); waddch(win,ACS_ULCORNER); wprintw(win,"  ACS_ULCORNER\n"); 
    wprintw(win,"Lower left corner         "); waddch(win,ACS_LLCORNER); wprintw(win,"  ACS_LLCORNER\n");
    wprintw(win,"Upper right corner        "); waddch(win,ACS_URCORNER); wprintw(win,"  ACS_URCORNER\n");
    wprintw(win,"Lower right corner        "); waddch(win,ACS_LRCORNER); wprintw(win,"  ACS_LRCORNER\n");
    wprintw(win,"Tee pointing right        "); waddch(win,ACS_LTEE); wprintw(win,"  ACS_LTEE\n");
    wprintw(win,"Tee pointing left         "); waddch(win,ACS_RTEE); wprintw(win,"  ACS_RTEE\n");
    wprintw(win,"Tee pointing up           "); waddch(win,ACS_BTEE); wprintw(win,"  ACS_BTEE\n");
    wprintw(win,"Tee pointing down         "); waddch(win,ACS_TTEE); wprintw(win,"  ACS_TTEE\n");
    wprintw(win,"Horizontal line           "); waddch(win,ACS_HLINE); wprintw(win,"  ACS_HLINE\n");
    wprintw(win,"Vertical line             "); waddch(win,ACS_VLINE); wprintw(win,"  ACS_VLINE\n");
    wprintw(win,"Large Plus or cross over  "); waddch(win,ACS_PLUS); wprintw(win,"  ACS_PLUS\n");
    wprintw(win,"Scan Line 1               "); waddch(win,ACS_S1); wprintw(win,"  ACS_S1\n");
    wprintw(win,"Scan Line 3               "); waddch(win,ACS_S3); wprintw(win,"  ACS_S3\n");
    wprintw(win,"Scan Line 7               "); waddch(win,ACS_S7); wprintw(win,"  ACS_S7\n");
    wprintw(win,"Scan Line 9               "); waddch(win,ACS_S9); wprintw(win,"  ACS_S9\n");
    wprintw(win,"Diamond                   "); waddch(win,ACS_DIAMOND); wprintw(win,"  ACS_DIAMOND\n");
    wprintw(win,"Checker board (stipple)   "); waddch(win,ACS_CKBOARD); wprintw(win,"  ACS_CKBOARD\n");
    wprintw(win,"Degree Symbol             "); waddch(win,ACS_DEGREE); wprintw(win,"  ACS_DEGREE\n");
    wprintw(win,"Plus/Minus Symbol         "); waddch(win,ACS_PLMINUS); wprintw(win,"  ACS_PLMINUS\n");
    wprintw(win,"Bullet                    "); waddch(win,ACS_BULLET); wprintw(win,"  ACS_BULLET\n");
    wprintw(win,"Arrow Pointing Left       "); waddch(win,ACS_LARROW); wprintw(win,"  ACS_LARROW\n");
    wprintw(win,"Arrow Pointing Right      "); waddch(win,ACS_RARROW); wprintw(win,"  ACS_RARROW\n");
    wprintw(win,"Arrow Pointing Down       "); waddch(win,ACS_DARROW); wprintw(win,"  ACS_DARROW\n");
    wprintw(win,"Arrow Pointing Up         "); waddch(win,ACS_UARROW); wprintw(win,"  ACS_UARROW\n");
    wprintw(win,"Board of squares          "); waddch(win,ACS_BOARD); wprintw(win,"  ACS_BOARD\n");
    wprintw(win,"Lantern Symbol            "); waddch(win,ACS_LANTERN); wprintw(win,"  ACS_LANTERN\n");
    wprintw(win,"Solid Square Block        "); waddch(win,ACS_BLOCK); wprintw(win,"  ACS_BLOCK\n");
    wprintw(win,"Less/Equal sign           "); waddch(win,ACS_LEQUAL); wprintw(win,"  ACS_LEQUAL\n");
    wprintw(win,"Greater/Equal sign        "); waddch(win,ACS_GEQUAL); wprintw(win,"  ACS_GEQUAL\n");
    wprintw(win,"Pi                        "); waddch(win,ACS_PI); wprintw(win,"  ACS_PI\n");
    wprintw(win,"Not equal                 "); waddch(win,ACS_NEQUAL); wprintw(win,"  ACS_NEQUAL\n");
    wprintw(win,"UK pound sign             "); waddch(win,ACS_STERLING); wprintw(win,"  ACS_STERLING\n\n");
    refresh();
    wrefresh(win);
    getch();
    endwin();
    return 0;
}
