# Implementacion

## Estado inicial

El proyecto apunta a implementar un Tetris inspirado en **Tetris - Nintendo NES - 1989** usando la biblioteca GBT.

La primera base implementada abre una ventana, aplica una paleta estilo NES/CGA y dibuja una pantalla inicial con:

- Fondo de ladrillos inspirado en Tetris NES.
- Area de estadisticas a la izquierda.
- Area central para el tablero.
- Paneles a la derecha para `TOP`, `SCORE`, `NEXT` y `LEVEL`.
- Panel superior para `LINES`.

Ya existe una primera demo jugable minima: cae una pieza aleatoria, el usuario puede moverla horizontalmente, bajarla manualmente, rotarla y fijarla al tocar la base u otra pieza. Todavia no se implementaron limpieza de filas, puntaje real ni pantalla de game over definitiva.

## Resoluciones logicas

La consigna pide soportar:

- CGA: `320x200`
- VGA: `640x480`

La resolucion default es **VGA**.

## Argumentos de arranque

La consigna pide poder ejecutar el juego con argumentos a `main` para elegir resolucion logica y factor de escala de ventana.

Uso actual soportado:

```bash
TETRIS
TETRIS vga
TETRIS cga
TETRIS vga 2
TETRIS cga 3
TETRIS --res vga --scale 2
TETRIS --res cga --scale 3
```

Si no se pasa argumento, se usa `vga` con escala `1`.

La escala se limita al rango `1..6` para evitar valores extremos.

## Pantalla base

La pantalla actual no es todavia la pantalla de presentacion definitiva ni el juego terminado. Es una base grafica inicial para validar:

- Creacion de ventana.
- Resolucion logica.
- Escala de ventana.
- Paleta.
- Dibujo de primitivas simples mediante `gbt_dibujar_pixel`.
- Texto bitmap propio.
- Marcos con estilo NES.
- Loop principal con `gbt_procesar_entrada`, `gbt_volcar_backbuffer` y `gbt_esperar`.

## Texto grafico

Se agrego una primera fuente bitmap interna de `5x7` pixeles, escalable por software. Actualmente se usa con tamano `2` para aproximar el estilo grueso del Tetris NES.

La consigna pide al menos dos sets de fuentes de distintos tamanos. Por ahora solo esta implementada la base `5x7`; mas adelante se puede agregar una fuente `8x16` o una variante grande especifica para titulos.

## Tablero Y Pieza Activa

La demo actual separa dos conceptos:

- `tTablero`: matriz logica de `22 x 10`.
- `tPiezaActiva`: pieza que esta cayendo, guardada aparte con tipo, rotacion, fila y columna.

El tablero tiene:

- 20 filas visibles.
- 2 filas ocultas superiores para generar tetrominos.
- Celdas `0` para vacio.
- Celdas con indice de color para minos ya fijados.
- Celdas visibles de `7x7` pixeles alineadas contra las paredes internas del marco central.

La pieza activa no se escribe en el tablero mientras cae. En cada frame se dibujan:

- Interfaz NES.
- Tablero fijo.
- Pieza activa encima.

Cuando la pieza no puede bajar mas, se copian sus minos al tablero y se genera una nueva pieza aleatoria.

## Temporizador Y Logs

La caida automatica usa `tGBT_Temporizador` con una duracion inicial de `0.6` segundos para que la demo se vea rapido.

Se agregaron logs por consola para:

- Consumo del timer.
- Teclas presionadas.
- Movimiento exitoso.
- Movimiento bloqueado.
- Rotacion exitosa o bloqueada.
- Fijacion de pieza.
- Fin de demo si no hay lugar para generar una nueva pieza.

## Archivo principal

Por ahora la implementacion vive en `main.c` para mantener el arranque simple. Cuando la logica crezca, conviene separar responsabilidades en archivos como:

- `pantalla.c` / `pantalla.h`
- `juego.c` / `juego.h`
- `tablero.c` / `tablero.h`
- `tetromino.c` / `tetromino.h`
- `fuente.c` / `fuente.h`
- `configuracion.c` / `configuracion.h`
- `estadisticas.c` / `estadisticas.h`
- `entrada.c` / `entrada.h`
