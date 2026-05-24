Proyecto: gestor estudio C, WSL Ubuntu. Ruta: ~/Programming/todo-estudio

Stack: C + ncurses + gcc + make + git
Archivos: main.c data.c data.h ui.c ui.h Makefile
Binario: /usr/local/bin/estudio
Datos: ~/Programming/todo-estudio/estudio.txt

Formato estudio.txt:
[AS] nombre
  [PRIO] [ESTADO] desc |dd/mm/yyyy
prio=line[3] estado=line[7] desc=line[9] (skip spaces)

Features:
- TUI ncurses
- Asigs colapsables (tab, v/>)
- a:nueva asig, t:nuevo tema, e:editar, d:borrar(s/n), enter:done
- J/K mover temas, j/k navegar
- Colores: rojo=alta, amarillo=media, cyan=baja, verde=done, azul=selected
- Fecha creación a la derecha, contador done/total en título
- Pomodoro flotante (esquina inf-der): 25/5min, p:pausa, r:reset, flash al cambiar fase
- No persiste estado pomo. Arranca 25:00 pausado
- Icono pixel art naranja (icono.svg, icono.ico)
- Acceso directo Windows
- P: asignar tema a pomodoro
- Historial pomo en ~/.estudio/pomo.log (append al cambiar fase)

Repo: https://github.com/rgarcf06/todo-estudio

Modo: caveman. Sin saludos ni resúmenes. Código > texto.
