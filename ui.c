#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"

// Elemento navegable (asignatura o tema)
typedef struct {
    int es_asig;   // 1=asignatura, 0=tema
    int asig_idx;
    int tema_idx;  // solo si es_asig==0
} Item;

#define MAX_ITEMS (MAX_ASIG * (MAX_TEMAS + 1))

static int build_items(Data *d, Item items[]) {
    int n = 0;
    for (int i = 0; i < d->n_asigs; i++) {
        items[n++] = (Item){1, i, 0};
        if (d->asigs[i].expandida) {
            for (int j = 0; j < d->asigs[i].n_temas; j++)
                items[n++] = (Item){0, i, j};
        }
    }
    return n;
}

static void get_fecha(char *buf) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buf, 11, "%d/%m/%Y", tm);
}

static void draw_ui(Data *d, Item items[], int n, int selected, int rows, int cols) {
    clear();

    // Contar totales
    int total = 0, hechos = 0;
    for (int i = 0; i < d->n_asigs; i++)
        for (int j = 0; j < d->asigs[i].n_temas; j++) {
            total++;
            if (d->asigs[i].temas[j].done) hechos++;
        }

    // Título
    char titulo[64];
    snprintf(titulo, sizeof(titulo), " GESTOR ESTUDIO | %d/%d temas completados ", hechos, total);
    attron(A_BOLD);
    mvprintw(0, (cols - strlen(titulo)) / 2, "%s", titulo);
    attroff(A_BOLD);
    mvhline(1, 0, '-', cols);

    // Items
    for (int i = 0; i < n; i++) {
        Item *it = &items[i];
        int row = i + 2;
        if (row >= rows - 2) break;

        if (it->es_asig) {
            Asignatura *a = &d->asigs[it->asig_idx];
            int t = a->n_temas, h = 0;
            for (int j = 0; j < t; j++) if (a->temas[j].done) h++;
            char line[MAX_LEN + 32];
            snprintf(line, sizeof(line), " %s %s (%d/%d)",
                a->expandida ? "v" : ">", a->nombre, h, t);
            if (i == selected) attron(A_BOLD | A_REVERSE);
            else attron(A_BOLD);
            if ((int)strlen(line) > cols - 12)
                line[cols - 12] = 0;
            mvprintw(row, 0, "%-*s", cols - 12, line);
            attroff(A_BOLD | A_REVERSE);
        } else {
            Tema *t = &d->asigs[it->asig_idx].temas[it->tema_idx];
            char prio_str[4];
            snprintf(prio_str, sizeof(prio_str), "[%c]", t->prio);
            char line[MAX_LEN + 32];
            snprintf(line, sizeof(line), "    %s [%c]   %s",
                prio_str, t->done ? 'x' : ' ', t->desc);

            if (i == selected) {
                attron(COLOR_PAIR(1));
            } else if (t->done) {
                attron(COLOR_PAIR(2));
            } else if (t->prio == 'A') {
                attron(COLOR_PAIR(4));
            } else if (t->prio == 'M') {
                attron(COLOR_PAIR(5));
            } else {
                attron(COLOR_PAIR(6));
            }

            mvprintw(row, 0, "%-*s", cols - 12, line);
            mvprintw(row, cols - 11, "%s", t->fecha);
            attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(4) | COLOR_PAIR(5) | COLOR_PAIR(6));
        }
    }

    mvhline(rows - 2, 0, '-', cols);
    mvprintw(rows - 1, 0, " a:asignatura  t:tema  e:editar  d:borrar  J/K:mover  enter:completar  tab:expandir  q:salir");
    refresh();
}

void run_ui(Data *d) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED,   COLOR_BLACK);
    init_pair(5, COLOR_YELLOW,COLOR_BLACK);
    init_pair(6, COLOR_CYAN,  COLOR_BLACK);

    Item items[MAX_ITEMS];
    int n, selected = 0, ch;
    int rows, cols;

    while (1) {
        n = build_items(d, items);
        if (selected >= n) selected = n - 1;
        if (selected < 0) selected = 0;
        getmaxyx(stdscr, rows, cols);
        draw_ui(d, items, n, selected, rows, cols);

        ch = getch();

        if (ch == 'q') break;

        else if ((ch == 'k' || ch == KEY_UP) && selected > 0) selected--;
        else if ((ch == 'j' || ch == KEY_DOWN) && selected < n - 1) selected++;

        else if (ch == '\t') {
            Item *it = &items[selected];
            int ai = it->asig_idx;
            d->asigs[ai].expandida = !d->asigs[ai].expandida;
        }

        else if (ch == '\n' && !items[selected].es_asig) {
            Tema *t = &d->asigs[items[selected].asig_idx].temas[items[selected].tema_idx];
            t->done = !t->done;
            data_save(d);
        }

        else if (ch == 'a') {
            echo(); curs_set(1);
            char buf[MAX_LEN];
            mvprintw(rows - 3, 2, "Nueva asignatura: ");
            clrtoeol();
            getnstr(buf, MAX_LEN - 1);
            noecho(); curs_set(0);
            if (strlen(buf) > 0 && d->n_asigs < MAX_ASIG) {
                Asignatura *a = &d->asigs[d->n_asigs++];
                strncpy(a->nombre, buf, MAX_LEN);
                a->n_temas = 0;
                a->expandida = 1;
                data_save(d);
            }
        }

        else if (ch == 'e' && n > 0) {
            Item *it = &items[selected];
            char buf[MAX_LEN];
            if (it->es_asig) {
                strncpy(buf, d->asigs[it->asig_idx].nombre, MAX_LEN);
            } else {
                strncpy(buf, d->asigs[it->asig_idx].temas[it->tema_idx].desc, MAX_LEN);
            }
            int len = strlen(buf), pos = len;
            while (1) {
                mvprintw(rows - 3, 2, "Editar: %-50s", buf);
                move(rows - 3, 10 + pos);
                curs_set(1); refresh();
                int c = getch();
                if (c == '\n') break;
                else if (c == 27) { len = -1; break; }
                else if ((c == KEY_BACKSPACE || c == 127) && pos > 0) {
                    memmove(&buf[pos-1], &buf[pos], len - pos + 1);
                    pos--; len--;
                }
                else if (c == KEY_LEFT && pos > 0) pos--;
                else if (c == KEY_RIGHT && pos < len) pos++;
                else if (c >= 32 && c < 127 && len < MAX_LEN - 1) {
                    memmove(&buf[pos+1], &buf[pos], len - pos + 1);
                    buf[pos++] = c; len++;
                }
            }
            curs_set(0);
            if (len > 0) {
                if (it->es_asig) {
                    strncpy(d->asigs[it->asig_idx].nombre, buf, MAX_LEN);
                } else {
                    strncpy(d->asigs[it->asig_idx].temas[it->tema_idx].desc, buf, MAX_LEN);
                    mvprintw(rows - 3, 2, "Prioridad actual [%c] - nueva (a)lta (m)edia (b)aja (enter=mantener): ",
                        d->asigs[it->asig_idx].temas[it->tema_idx].prio);
                    clrtoeol();
                    int p = getch();
                    if (p == 'a') d->asigs[it->asig_idx].temas[it->tema_idx].prio = 'A';
                    else if (p == 'm') d->asigs[it->asig_idx].temas[it->tema_idx].prio = 'M';
                    else if (p == 'b') d->asigs[it->asig_idx].temas[it->tema_idx].prio = 'B';
                }
                data_save(d);
            }
        }

        else if (ch == 'd' && n > 0) {
            Item *it = &items[selected];
            mvprintw(rows - 3, 2, "¿Borrar? (s/n): ");
            clrtoeol();
            curs_set(1);
            int c = getch();
            curs_set(0);
            if (c == 's') {
                if (it->es_asig) {
                    int ai = it->asig_idx;
                    memmove(&d->asigs[ai], &d->asigs[ai+1],
                        (d->n_asigs - ai - 1) * sizeof(Asignatura));
                    d->n_asigs--;
                } else {
                    Asignatura *a = &d->asigs[it->asig_idx];
                    int ti = it->tema_idx;
                    memmove(&a->temas[ti], &a->temas[ti+1],
                        (a->n_temas - ti - 1) * sizeof(Tema));
                    a->n_temas--;
                }
                data_save(d);
                if (selected >= n - 1 && selected > 0) selected--;
            }
        }

        else if (ch == 't' && n > 0) {
            echo(); curs_set(1);
            char buf[MAX_LEN];
            mvprintw(rows - 3, 2, "Nuevo tema: ");
            clrtoeol();
            getnstr(buf, MAX_LEN - 1);
            if (strlen(buf) > 0) {
                noecho(); curs_set(0);
                mvprintw(rows - 3, 2, "Prioridad (a)lta (m)edia (b)aja: ");
                clrtoeol();
                int p = getch();
                char prio = (p == 'a') ? 'A' : (p == 'm') ? 'M' : 'B';
                int ai = items[selected].asig_idx;
                Asignatura *a = &d->asigs[ai];
                if (a->n_temas < MAX_TEMAS) {
                    Tema *t = &a->temas[a->n_temas++];
                    strncpy(t->desc, buf, MAX_LEN);
                    t->prio = prio;
                    t->done = 0;
                    get_fecha(t->fecha);
                    data_save(d);
                }
            }
            noecho(); curs_set(0);
        }

        else if (ch == 'K' && !items[selected].es_asig) {
            int ai = items[selected].asig_idx;
            int ti = items[selected].tema_idx;
            if (ti > 0) {
                Asignatura *a = &d->asigs[ai];
                Tema tmp = a->temas[ti];
                a->temas[ti] = a->temas[ti-1];
                a->temas[ti-1] = tmp;
                selected--;
                data_save(d);
            }
        }

        else if (ch == 'J' && !items[selected].es_asig) {
            int ai = items[selected].asig_idx;
            int ti = items[selected].tema_idx;
            Asignatura *a = &d->asigs[ai];
            if (ti < a->n_temas - 1) {
                Tema tmp = a->temas[ti];
                a->temas[ti] = a->temas[ti+1];
                a->temas[ti+1] = tmp;
                selected++;
                data_save(d);
            }
        }

    }

    endwin();
}
