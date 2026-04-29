# AGENTS

## Contexto Del Proyecto

- Trabajo practico de Topicos de Programacion, primer cuatrimestre 2026.
- Objetivo: desarrollar un videojuego equivalente a Tetris usando la biblioteca GBT.
- Version de referencia elegida: **Tetris - Nintendo NES - 1989**.
- Biblioteca grafica y de soporte: **GBT - Gran Biblioteca de Topicos**.
- El desarrollo debe avanzar de forma incremental: no implementar todo el juego de una vez.

## Requisitos Centrales De La Consigna

- Pantalla de presentacion grafica.
- Documentacion para compilar, ejecutar y usar el juego, incluyendo linkeo de bibliotecas y ubicacion de archivos.
- Resoluciones logicas obligatorias:
  - CGA: `320x200`
  - VGA: `640x480`
- Argumentos a `main` para elegir resolucion logica y factor de escala de ventana.
- Sistema de puntuacion visible en interfaz grafica.
- Dificultad con velocidad de caida variable:
  - Caida inicial configurable.
  - Aumenta 3% cada 10 fichas caidas.
  - Velocidad de fijacion igual al 50% de la velocidad de caida.
  - Mayor velocidad debe otorgar mas puntos.
- Tetromino actual aleatorio sin tendencia hacia tipos concretos.
- Solicitud grafica del nombre del jugador, no por consola.
- Estadisticas por nombre de jugador.
- Texto grafico con al menos dos fuentes de distinto tamano.
- Todos los tetrominos originales: `I`, `J`, `L`, `O`, `S`, `T`, `Z`.
- Rotacion hacia izquierda y derecha.
- Menu grafico con configuraciones perdurables mediante archivos:
  - Paleta.
  - Resolucion logica.
  - Velocidad de caida.
- Pausa, pantalla de salida y opcion de jugar nuevamente al perder.

## Modalidad Clasica

- Tablero de 20 filas visibles por 10 columnas.
- Considerar filas invisibles para generar el tetromino actual.
- Paleta de color propia para el modo clasico.
- Movimiento horizontal o rotacion no permitidos si la pieza se sale del tablero.

## Opcionales Para Promocionar

- Seleccion entre modo Clasico y modo DX.
- Fuente no monoespaciada.
- Tablero mediante array de punteros a filas, limpiando lineas por intercambio de punteros.
- Implementacion propia de primitivas de TDA Vector necesarias.
- Guardado binario del estado actual de partida.
- Cheat, por ejemplo ralentizar el tiempo.

## Modalidad DX

- Tablero clasico o redimensionable en ancho de 8 a 16 casilleros.
- Tablero circular para movimiento/rotacion en bordes.
- Piezas adicionales:
  - `x`: 1 mino.
  - `c`: 5 minos en forma de c.
  - `p`: 5 minos en forma de p.
  - `*`: una letra elegida por el grupo.

## Requisitos Tecnicos

- Uso de CPU menor al 10% en computadoras de laboratorio.
- Uso maximo de RAM: 256 MB.
- Solucion principal matricial.
- Para promocion, preferir matriz dinamica con array de punteros a filas y limpieza por reordenamiento de punteros.
- Se valora uso de memoria dinamica, punteros a funciones, recursividad, investigacion y creatividad grafica.

## Decisiones Vigentes

- Basarse como primera referencia en la libreria GBT que ofrece la cátedra para solucionar los problemas.
- La resolucion por defecto es VGA `640x480`.
- El arranque actual acepta resolucion y escala por argumentos.
- El layout inicial se inspira en Tetris NES 1989: fondo de ladrillos, marcos con bisel, estadisticas a la izquierda, tablero central y paneles de informacion a la derecha.
- Ya existe una primera fuente bitmap `5x7` dibujada con primitivas de GBT. Falta agregar un segundo set de fuente para cumplir completamente la consigna.
- Por ahora `main.c` concentra el arranque grafico para mantener simple la base.
- Cuando crezcan las funcionalidades, separar en modulos:
  - `pantalla.c/.h`
  - `juego.c/.h`
  - `tablero.c/.h`
  - `tetromino.c/.h`
  - `fuente.c/.h`
  - `configuracion.c/.h`
  - `estadisticas.c/.h`
- Mantener documentacion en `docs/*.md` cada vez que se agregue una funcionalidad o decision relevante.

## Estado Actual

- Implementado:
  - Ventana GBT.
  - Paleta base CGA.
  - Resolucion logica CGA/VGA.
  - VGA como resolucion por defecto.
  - Factor de escala por argumentos.
  - Pantalla base con estilo NES.
  - Textos bitmap para `A-TYPE`, `STATISTICS`, `LINES`, `TOP`, `SCORE`, `NEXT` y `LEVEL`.
  - Tablero logico de 22 filas por 10 columnas, con 2 filas ocultas.
  - Pieza activa aleatoria separada del tablero fijo.
  - Caida automatica con `tGBT_Temporizador`.
  - Movimiento horizontal y bajada manual con teclado.
  - Rotacion horaria con flecha arriba y antihoraria con `Z`.
  - Fijacion de piezas cuando no pueden seguir bajando.
  - Logs por consola para timer, teclas, movimientos, rotaciones, bloqueos y fijaciones.
  - Salida con `Escape`.
- Pendiente:
  - Pantalla de presentacion real.
  - Menu.
  - Nombre de jugador por interfaz grafica.
  - Limpieza de filas completas.
  - Puntuacion.
  - Dificultad.
  - Segundo set de fuente grafica.
  - Persistencia de configuracion y estadisticas.

## Uso Actual

```bash
TETRIS
TETRIS vga
TETRIS cga
TETRIS vga 2
TETRIS cga 3
TETRIS --res vga --scale 2
TETRIS --res cga --scale 3
```
