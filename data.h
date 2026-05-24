#ifndef DATA_H
#define DATA_H

#include <time.h>

#define MAX_ASIG   20
#define MAX_TEMAS  50
#define MAX_LEN   128
#define FILE_NAME "/home/roberto/Programming/todo-estudio/estudio.txt"
#define POMO_LOG "/home/roberto/Programming/todo-estudio/pomo.log"

typedef struct {
    char desc[MAX_LEN];
    char prio;       // A, M, B
    int  done;       // 0 o 1
    char fecha[12];  // dd/mm/yyyy
} Tema;

typedef struct {
    char  nombre[MAX_LEN];
    Tema  temas[MAX_TEMAS];
    int   n_temas;
    int   expandida;
} Asignatura;

typedef struct {
    int minutes, seconds, is_work, running;
    char tarea[MAX_LEN];
    int work_time;   // default 25
    int break_time;  // default 5
} Pomodoro;

typedef struct {
    Asignatura asigs[MAX_ASIG];
    int n_asigs;
    Pomodoro pomo;
} Data;

    

void data_load(Data *d);
void data_save(const Data *d);

#endif
