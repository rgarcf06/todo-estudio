Estoy desarrollando un gestor de estudio en C llamado "estudio" en WSL Ubuntu.
Ruta del proyecto: ~/Programming/todo-estudio

Estructura:
- main.c, data.c, data.h, ui.c, ui.h, Makefile
- Binario instalado en /usr/local/bin/estudio
- Datos en: /home/roberto/Programming/todo-estudio/estudio.txt

Stack: C + ncurses + gcc + make + git

Formato de estudio.txt:
[AS] nombre_asignatura
  [PRIO] [ESTADO] descripción |dd/mm/yyyy
Donde PRIO = A/M/B, ESTADO = ' ' o 'x'
Posiciones: prio=line[3], estado=line[7]
La descripción se lee saltando espacios tras line[9] con while(*p==' ')p++

Funcionalidades implementadas:
- TUI con ncurses
- Asignaturas colapsables con tab, indicador v/>
- Añadir asignatura con 'a', tema con 't'
- Completar/descompletar tema con Enter
- Editar nombre y prioridad con 'e' (edición inline con cursor)
- Borrar con 'd' + confirmación s/n
- Mover temas con J/K (mayúscula) dentro de la misma asignatura
- Nombres largos se truncan para no romper el layout
- Prioridades con colores: rojo=alta, amarillo=media, cyan=baja, verde=completada, azul=seleccionada
- Fecha de creación visible a la derecha
- Contador de temas completados en el título
- Navegación con j/k y flechas
- Acceso directo en escritorio Windows que lanza el programa
- Icono pixel art naranja (icono.svg, icono.ico)

Repo GitHub: https://github.com/rgarcf06/todo-estudio

Continúa ayudándome a desarrollar el proyecto. Responde de forma breve y precisa,
sin saludos ni resúmenes. Usa código cuando sea más claro que texto.
