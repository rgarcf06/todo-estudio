Estoy desarrollando un gestor de estudio en C llamado "estudio" en WSL Ubuntu.
Ruta del proyecto: ~/Programming/todo-estudio

Estructura:
- main.c, data.c, data.h, ui.c, ui.h, Makefile
- Binario: ./estudio (no instalado en PATH aún)

Stack: C + ncurses + gcc + make + git

Formato de estudio.txt:
[AS] nombre_asignatura
  [PRIO] [ESTADO] descripción |dd/mm/yyyy
Donde PRIO = A/M/B, ESTADO = ' ' o 'x'
Posiciones en línea de tema: prio=line[3], estado=line[7]

Funcionalidades implementadas:
- TUI con ncurses
- Asignaturas colapsables con v/> 
- Añadir asignatura con 'a', tema con 't'
- Completar/descompletar tema con Enter
- Editar nombre y prioridad con 'e'
- Borrar con 'd' + confirmación s/n
- Prioridades con colores: rojo=alta, amarillo=media, cyan=baja, verde=completada, azul=seleccionada
- Fecha de creación visible a la derecha
- Contador de temas completados en el título
- Navegación con j/k y flechas, Tab para colapsar/expandir

Repo GitHub: https://github.com/rgarcf06/todo-estudio

Continúa ayudándome a desarrollar el proyecto. Responde de forma breve y precisa,
sin saludos ni resúmenes. Usa código cuando sea más claro que texto.
