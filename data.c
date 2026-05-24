#include <stdio.h>
#include <string.h>
#include <time.h>
#include "data.h"

void data_load(Data *d) {
    d->n_asigs = 0;
    d->pomo.minutes = 25;
    d->pomo.seconds = 0;
    d->pomo.is_work = 1;
    d->pomo.running = 0;
    d->pomo.work_time = 25;
    d->pomo.break_time = 5;

    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;

    char line[256];
    Asignatura *cur = NULL;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;

        if (line[0] == '[' && line[1] == 'A' && line[2] == 'S') {
            // Asignatura: [AS] nombre
            if (d->n_asigs >= MAX_ASIG) continue;
            cur = &d->asigs[d->n_asigs++];
            strncpy(cur->nombre, line + 5, MAX_LEN);
            cur->n_temas = 0;
            cur->expandida = 1;
        } else if (line[0] == ' ' && cur) {
            // Tema:   [A] [x] desc |fecha
            if (cur->n_temas >= MAX_TEMAS) continue;
            Tema *t = &cur->temas[cur->n_temas++];
            t->prio = line[3];
            t->done = (line[7] == 'x') ? 1 : 0;
            char *sep = strrchr(line, '|');
            if (sep) {
                strncpy(t->fecha, sep + 1, 11);
                *sep = 0;
            }
            char *desc_start = line + 9;
            while (*desc_start == ' ') desc_start++;
            strncpy(t->desc, desc_start, MAX_LEN);
            // quitar espacio final
            int len = strlen(t->desc);
            while (len > 0 && t->desc[len-1] == ' ') t->desc[--len] = 0;
        }
    }
    fclose(f);
}

void data_save(const Data *d) {
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) return;

    for (int i = 0; i < d->n_asigs; i++) {
        const Asignatura *a = &d->asigs[i];
        fprintf(f, "[AS] %s\n", a->nombre);
        for (int j = 0; j < a->n_temas; j++) {
            const Tema *t = &a->temas[j];
            fprintf(f, "  [%c] [%c] %s |%s\n",
                t->prio,
                t->done ? 'x' : ' ',
                t->desc,
                t->fecha);
        }
    }
    fclose(f);
}
