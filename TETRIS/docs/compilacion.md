# Compilacion Y Ejecucion

## Entorno Esperado

El proyecto esta preparado para trabajarse con Code::Blocks y GBT, siguiendo el entorno de la materia.

La biblioteca GBT se encuentra dentro del proyecto en:

```text
TETRIS/lib/
```

El archivo de proyecto actual es:

```text
TETRIS/TETRIS.cbp
```

## Configuracion De GBT

Para compilar correctamente, Code::Blocks debe poder encontrar:

- Los headers de GBT.
- La biblioteca compilada de GBT.
- Las dependencias que GBT necesite segun el entorno de laboratorio.

El ejemplo provisto por la catedra esta en:

```text
TETRIS/lib/GBT_v2026.1C.01/ejemplo/
```

Ese ejemplo sirve como referencia para configurar includes, linker y ejecucion.

## Ejecucion Actual

Sin argumentos:

```bash
TETRIS
```

Abre VGA `640x480` con escala `1`.

Con resolucion:

```bash
TETRIS vga
TETRIS cga
```

Con resolucion y escala:

```bash
TETRIS vga 2
TETRIS cga 3
```

Forma explicita:

```bash
TETRIS --res vga --scale 2
TETRIS --res cga --scale 3
```

## Controles Actuales

- `Escape`: cerrar la ventana.
- `Flecha izquierda`: mover pieza activa a la izquierda.
- `Flecha derecha`: mover pieza activa a la derecha.
- `Flecha abajo`: bajar pieza activa manualmente.
- `Flecha arriba`: rotar pieza activa hacia la derecha.
- `Z`: rotar pieza activa hacia la izquierda.

## Logs Actuales

La demo imprime informacion por consola para depuracion:

- `[timer]`: cuando se consume el temporizador de caida.
- `[tecla]`: cuando el usuario presiona una tecla.
- `[movimiento]`: cuando una pieza pudo moverse.
- `[rotacion]`: cuando una pieza pudo rotar.
- `[bloqueo]`: cuando una pieza no pudo moverse.
- `[fijacion]`: cuando una pieza aterriza y se copia al tablero.

## Nota

Esta documentacion se va a completar a medida que se agreguen pantalla de presentacion, menu, juego, configuraciones persistentes y estadisticas.
