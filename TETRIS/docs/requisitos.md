# Requisitos Del Trabajo Practico

Este documento resume la consigna que guia el desarrollo del Tetris.

## Objetivo

Desarrollar un videojuego equivalente a Tetris con interfaz grafica usando GBT, aplicando estructuras vistas en la cursada: matrices, memoria dinamica, TDAs, punteros, archivos, temporizadores, teclado y graficos.

La version elegida como referencia visual y funcional es **Tetris - Nintendo NES - 1989**.

## Funcionalidades Minimas

- Pantalla de presentacion.
- Documentacion para compilar, ejecutar y usar el juego.
- Resoluciones logicas:
  - CGA `320x200`
  - VGA `640x480`
- Sistema de puntuacion visible en la interfaz.
- Dificultad con velocidad de caida, velocidad de fijacion y bonificacion por mayor velocidad.
- Seleccion aleatoria pareja del tetromino actual.
- Nombre del jugador solicitado desde interfaz grafica, no desde consola.
- Estadisticas por nombre de jugador.
- Texto grafico con al menos dos fuentes de distinto tamano.
- Argumentos a `main` para resolucion logica y escala de ventana.
- Todos los tetrominos originales.
- Rotacion hacia izquierda y derecha.
- Menu grafico con configuraciones persistentes por archivo.
- Pausa, pantalla de salida y opcion de jugar nuevamente al perder.

## Puntuacion

Debe contemplar:

- Puntos por limpiar una fila.
- Bonificacion por limpiar 2, 3 o 4 filas al mismo tiempo.
- Puntos por cada casillero que el jugador baja manualmente.
- Mayor puntuacion cuando la velocidad de juego es mayor.

## Dificultad

Debe existir al menos una dificultad donde:

- La velocidad de caida aumenta un 3% cada 10 fichas caidas.
- Ejemplo: `1000 ms`, luego `970 ms`, luego aproximadamente `941 ms`.
- La velocidad de fijacion es el 50% de la velocidad de caida.
- La tolerancia de fijacion se reinicia si la pieza sigue moviendose.
- La duracion de la caida debe verse en la interfaz.

## Tetris Clasico

- Tablero de 20 filas visibles por 10 columnas.
- Filas invisibles adicionales para generar el tetromino actual.
- Paleta propia para el modo clasico.
- Si un movimiento horizontal o rotacion saca la pieza del tablero, se debe rechazar.

## Opcionales Para Promocionar

- Modo Clasico y modo DX.
- Fuente no monoespaciada.
- Tablero con array de punteros a filas y limpieza por intercambio de punteros.
- Implementacion propia de primitivas necesarias de TDA Vector.
- Guardado binario del estado actual de partida.
- Cheat.

## Tetris DX

- Tablero redimensionable de 8 a 16 columnas.
- Tablero circular para movimiento y rotacion en bordes.
- Piezas extra:
  - `x`
  - `c`
  - `p`
  - `*`

## Requisitos Tecnicos

- CPU menor al 10% en computadoras de laboratorio.
- RAM maxima: 256 MB.
- Solucion principal matricial.
- Para promocion: matriz dinamica con punteros a filas y limpieza por reordenamiento de punteros.

