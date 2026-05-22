#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lib/GBT_v2026.1C.01/include/GBT/gbt.h"
#include "Funciones.h"

 tGBT_ColorRGB paletaNES[CANT_COLORES] = {
    {0x00, 0x00, 0x00}, // 0: Negro
    {0x24, 0x18, 0x8F}, // 1: Azul NES
    {0x00, 0x88, 0x00}, // 2: Verde
    {0x00, 0xA8, 0xA8}, // 3: Cian
    {0xB8, 0x00, 0x20}, // 4: Rojo
    {0xA8, 0x00, 0xA8}, // 5: Magenta
    {0xA8, 0x58, 0x00}, // 6: Marron
    {0xB8, 0xB8, 0xB8}, // 7: Gris claro
    {0x50, 0x50, 0x50}, // 8: Gris oscuro
    {0x30, 0x30, 0xFF}, // 9: Azul brillante
    {0x50, 0xF0, 0x50}, // 10: Verde brillante
    {0xC8, 0xFF, 0xFF}, // 11: Cian muy claro
    {0xFF, 0x30, 0x30}, // 12: Rojo brillante
    {0xFF, 0x60, 0xFF}, // 13: Magenta brillante
    {0xFF, 0xF0, 0x50}, // 14: Amarillo
    {0xFF, 0xFF, 0xFF}  // 15: Blanco / transparente
};

 const tGlifo5x7 fuente5x7[] = {
    {' ', {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {'-', {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00}},
    {'0', {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E}},
    {'1', {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}},
    {'2', {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F}},
    {'3', {0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E}},
    {'4', {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02}},
    {'5', {0x1F, 0x10, 0x10, 0x1E, 0x01, 0x01, 0x1E}},
    {'6', {0x0E, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x0E}},
    {'7', {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08}},
    {'8', {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}},
    {'9', {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x0E}},
    {'A', {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}},
    {'C', {0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F}},
    {'E', {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F}},
    {'I', {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F}},
    {'L', {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F}},
    {'N', {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11}},
    {'O', {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}},
    {'P', {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10}},
    {'R', {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11}},
    {'S', {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E}},
    {'T', {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {'V', {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04}},
    {'X', {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11}},
    {'Y', {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}},
    {'G', {0x0E, 0x10, 0x10, 0x17, 0x11, 0x11, 0x0E}},
    {'J', {0x1F, 0x02, 0x02, 0x02, 0x12, 0x12, 0x0C}},
    {'U', {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}},
    {'<', {0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02}},
    {'>', {0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08}},
    {'^', {0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00}},
    {'Q', {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D}}
};

 const uint8_t tetrominos[CANT_TETROMINOS][4][4][4] = {
    { // I
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}
    },
    { // J
        {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}
    },
    { // L
        {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
        {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}
    },
    { // O
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
    },
    { // S
        {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
        {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}
    },
    { // T
        {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}
    },
    { // Z
        {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
        {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}
    }
};

 const uint8_t colores_piezas[CANT_TETROMINOS] = {
    COLOR_CIAN_BRILLANTE,
    COLOR_AZUL,
    COLOR_GRIS_CLARO,
    14,
    2,
    5,
    COLOR_ROJO
};

 const char nombres_piezas[CANT_TETROMINOS] = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};

 tConfiguracionVentana obtener_configuracion(tOpcionesArranque opciones)
{
    if (opciones.resolucion == RESOLUCION_CGA) {
        return (tConfiguracionVentana) {"Tetris NES 1989 - CGA", RES_CGA_ANCHO, RES_CGA_ALTO, opciones.escala};
    }

    return (tConfiguracionVentana) {"Tetris NES 1989 - VGA", RES_VGA_ANCHO, RES_VGA_ALTO, opciones.escala};
}

 uint8_t es_resolucion(const char *valor)
{
    return strcmp(valor, "cga") == 0 || strcmp(valor, "vga") == 0;
}

 uint16_t normalizar_escala(int escala)
{
    if (escala < ESCALA_MINIMA) {
        return ESCALA_MINIMA;
    }

    if (escala > ESCALA_MAXIMA) {
        return ESCALA_MAXIMA;
    }

    return (uint16_t) escala;
}

 void aplicar_resolucion(tOpcionesArranque *opciones, const char *valor)
{
    if (strcmp(valor, "cga") == 0) {
        opciones->resolucion = RESOLUCION_CGA;
    } else if (strcmp(valor, "vga") == 0) {
        opciones->resolucion = RESOLUCION_VGA;
    }
}

 tOpcionesArranque leer_opciones_arranque(int argc, char *argv[])
{
    tOpcionesArranque opciones = {RESOLUCION_VGA, ESCALA_DEFAULT};

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--res") == 0 && i + 1 < argc) {
            aplicar_resolucion(&opciones, argv[++i]);
        } else if (strcmp(argv[i], "--scale") == 0 && i + 1 < argc) {
            opciones.escala = normalizar_escala(atoi(argv[++i]));
        } else if (es_resolucion(argv[i])) {
            aplicar_resolucion(&opciones, argv[i]);
        } else {
            opciones.escala = normalizar_escala(atoi(argv[i]));
        }
    }

    return opciones;
}

 tRender crear_render(const tConfiguracionVentana *config)
{
    tRender render;

    render.escala = config->ancho / LIENZO_BASE_ANCHO;
    render.offset_y = (config->alto - (LIENZO_BASE_ALTO * render.escala)) / 2;

    return render;
}

 void dibujar_rectangulo_px(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color)
{
    for (uint16_t fila = 0; fila < alto; fila++) {
        for (uint16_t col = 0; col < ancho; col++) {
            gbt_dibujar_pixel(x + col, y + fila, color);
        }
    }
}

 void dibujar_rectangulo(const tRender *render, uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color)
{
    if (x >= LIENZO_BASE_ANCHO || y >= LIENZO_BASE_ALTO) {
        return;
    }

    if (x + ancho > LIENZO_BASE_ANCHO) {
        ancho = LIENZO_BASE_ANCHO - x;
    }

    if (y + alto > LIENZO_BASE_ALTO) {
        alto = LIENZO_BASE_ALTO - y;
    }

    dibujar_rectangulo_px(
        x * render->escala,
        render->offset_y + (y * render->escala),
        ancho * render->escala,
        alto * render->escala,
        color
    );
}

 void dibujar_marco_nes(const tRender *render, uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto)
{
    dibujar_rectangulo(render, x, y, ancho, alto, COLOR_GRIS_OSCURO);
    dibujar_rectangulo(render, x + 2, y + 2, ancho - 4, alto - 4, COLOR_CIAN_BRILLANTE);
    dibujar_rectangulo(render, x + 4, y + 4, ancho - 8, alto - 8, COLOR_GRIS_CLARO);
    dibujar_rectangulo(render, x + 6, y + 6, ancho - 12, alto - 12, COLOR_NEGRO);
    dibujar_rectangulo(render, x + 2, y + alto - 4, ancho - 4, 2, COLOR_GRIS_OSCURO);
    dibujar_rectangulo(render, x + ancho - 4, y + 2, 2, alto - 4, COLOR_GRIS_OSCURO);
}

 const uint8_t *buscar_glifo(char caracter)
{
    for (uint16_t i = 0; i < sizeof(fuente5x7) / sizeof(fuente5x7[0]); i++) {
        if (fuente5x7[i].caracter == caracter) {
            return fuente5x7[i].filas;
        }
    }

    return fuente5x7[0].filas;
}

 void dibujar_texto(const tRender *render, uint16_t x, uint16_t y, const char *texto, uint8_t color, uint16_t tam)
{
    uint16_t cursor_x = x;

    while (*texto) {
        const uint8_t *glifo = buscar_glifo(*texto);

        for (uint16_t fila = 0; fila < 7; fila++) {
            for (uint16_t col = 0; col < 5; col++) {
                if (glifo[fila] & (1 << (4 - col))) {
                    dibujar_rectangulo(render, cursor_x + (col * tam), y + (fila * tam), tam, tam, color);
                }
            }
        }

        cursor_x += 6 * tam;
        texto++;
    }
}

void dibujar_fondo_ladrillos(const tRender *render)
{
    gbt_borrar_backbuffer(COLOR_NEGRO);

    for (uint16_t y = 0; y < LIENZO_BASE_ALTO +10; y += 12) {

        uint16_t offset_x = ((y / 12) % 2) ? 10 : 0;

        for (uint16_t x = 0; x < LIENZO_BASE_ANCHO; x += 20) {

            dibujar_rectangulo(render, x + offset_x, y, 18, 10, COLOR_GRIS_OSCURO);

            dibujar_rectangulo(render, x + offset_x, y, 18, 1, COLOR_CIAN_BRILLANTE);

            dibujar_rectangulo(render, x + offset_x, y + 9, 18, 1, COLOR_NEGRO);

            dibujar_rectangulo(render, x + offset_x, y, 1, 10, COLOR_CIAN_BRILLANTE);

            dibujar_rectangulo(render, x + offset_x + 17, y, 1, 10, COLOR_NEGRO);
        }
    }
}

 void dibujar_mino(const tRender *render, uint16_t x, uint16_t y, uint8_t color)
{
    dibujar_rectangulo(render, x, y, 8, 8, COLOR_BLANCO);
    dibujar_rectangulo(render, x + 1, y + 1, 6, 6, color);
    dibujar_rectangulo(render, x + 2, y + 2, 2, 2, COLOR_BLANCO);
}

 void dibujar_mino_tablero(const tRender *render, uint16_t columna, uint16_t fila_visible, uint8_t color)
{
    uint16_t x = TABLERO_X + (columna * TAM_CELDA);
    uint16_t y = TABLERO_Y + (fila_visible * TAM_CELDA);

    dibujar_rectangulo(render, x, y, TAM_CELDA, TAM_CELDA, COLOR_BLANCO);
    dibujar_rectangulo(render, x + 1, y + 1, TAM_CELDA - 1, TAM_CELDA - 1, color);
    dibujar_rectangulo(render, x + 2, y + 2, 2, 2, COLOR_BLANCO);
}

 void dibujar_muestra_tetromino(const tRender *render, uint16_t x, uint16_t y, const uint8_t forma[4][4], uint8_t color)
{
    for (uint16_t fila = 0; fila < 4; fila++) {
        for (uint16_t col = 0; col < 4; col++) {
            if (forma[fila][col]) {
                dibujar_mino(render, x + (col * 8), y + (fila * 8), color);
            }
        }
    }
}

 void tablero_vaciar(tTablero tablero)
{
    for (uint16_t fila = 0; fila < TABLERO_FILAS; fila++) {
        for (uint16_t col = 0; col < TABLERO_COLUMNAS; col++) {
            tablero[fila][col] = 0;
        }
    }
}

 tPiezaActiva crear_pieza_aleatoria(void)
{
    tPiezaActiva pieza;

    pieza.tipo = (tTipoPieza)(rand() % CANT_TETROMINOS);
    pieza.rotacion = 0;
    pieza.fila = 0;
    pieza.columna = 3;

    printf("[pieza] nueva pieza %c en fila=%d columna=%d\n",
           nombres_piezas[pieza.tipo], pieza.fila, pieza.columna);

    return pieza;
}

 uint8_t puede_ubicar(const tTablero tablero, const tPiezaActiva *pieza, int16_t delta_fila, int16_t delta_columna)
{
    const uint8_t (*forma)[4] = tetrominos[pieza->tipo][pieza->rotacion];

    for (int16_t fila = 0; fila < 4; fila++) {
        for (int16_t col = 0; col < 4; col++) {
            if (!forma[fila][col]) {
                continue;
            }

            int16_t tablero_fila = pieza->fila + fila + delta_fila;
            int16_t tablero_col = pieza->columna + col + delta_columna;

            if (tablero_col < 0 || tablero_col >= TABLERO_COLUMNAS) {
                return 0;
            }

            if (tablero_fila >= TABLERO_FILAS) {
                return 0;
            }

            if (tablero_fila >= 0 && tablero[tablero_fila][tablero_col] != 0) {
                return 0;
            }
        }
    }

    return 1;
}

 uint8_t intentar_mover(tTablero tablero, tPiezaActiva *pieza, int16_t delta_fila, int16_t delta_columna, const char *origen)
{
    if (puede_ubicar(tablero, pieza, delta_fila, delta_columna)) {
        pieza->fila += delta_fila;
        pieza->columna += delta_columna;
        printf("[movimiento] %s movio pieza %c a fila=%d columna=%d\n",
               origen, nombres_piezas[pieza->tipo], pieza->fila, pieza->columna);
        return 1;
    }

    printf("[bloqueo] %s no pudo mover pieza %c desde fila=%d columna=%d delta=(%d,%d)\n",
           origen, nombres_piezas[pieza->tipo], pieza->fila, pieza->columna, delta_fila, delta_columna);

    return 0;
}

 uint8_t intentar_rotar(tTablero tablero, tPiezaActiva *pieza, int8_t direccion, const char *origen)
{
    uint8_t rotacion_anterior = pieza->rotacion;

    printf("rotacion anterior=%d nueva=%d\n",
       rotacion_anterior,
       pieza->rotacion);

    if (direccion > 0) {
        pieza->rotacion = (pieza->rotacion + 1) % 4;
    } else {
        pieza->rotacion = (pieza->rotacion + 3) % 4;
    }

    if (puede_ubicar(tablero, pieza, 0, 0)) {
        printf("[rotacion] %s roto pieza %c a rotacion=%d fila=%d columna=%d\n",
               origen, nombres_piezas[pieza->tipo], pieza->rotacion, pieza->fila, pieza->columna);
        return 1;
    }

    pieza->rotacion = rotacion_anterior;
    printf("[bloqueo] %s no pudo rotar pieza %c en fila=%d columna=%d\n",
           origen, nombres_piezas[pieza->tipo], pieza->fila, pieza->columna);

    return 0;
}

 void fijar_pieza(tTablero tablero, const tPiezaActiva *pieza)
{
    const uint8_t (*forma)[4] = tetrominos[pieza->tipo][pieza->rotacion];
    uint8_t color = colores_piezas[pieza->tipo];

    for (int16_t fila = 0; fila < 4; fila++) {
        for (int16_t col = 0; col < 4; col++) {
            if (!forma[fila][col]) {
                continue;
            }

            int16_t tablero_fila = pieza->fila + fila;
            int16_t tablero_col = pieza->columna + col;

            if (tablero_fila >= 0 && tablero_fila < TABLERO_FILAS &&
                tablero_col >= 0 && tablero_col < TABLERO_COLUMNAS) {
                tablero[tablero_fila][tablero_col] = color;
            }
        }
    }

    printf("[fijacion] pieza %c fijada en fila=%d columna=%d\n",
           nombres_piezas[pieza->tipo], pieza->fila, pieza->columna);
}

 void dibujar_tablero(const tRender *render, const tTablero tablero)
{
    for (uint16_t fila = TABLERO_FILAS_OCULTAS; fila < TABLERO_FILAS; fila++) {
        for (uint16_t col = 0; col < TABLERO_COLUMNAS; col++) {
            if (tablero[fila][col] != 0) {
                dibujar_mino_tablero(render, col, fila - TABLERO_FILAS_OCULTAS, tablero[fila][col]);
            }
        }
    }
}

 void dibujar_pieza_activa(const tRender *render, const tPiezaActiva *pieza)
{
    const uint8_t (*forma)[4] = tetrominos[pieza->tipo][pieza->rotacion];
    uint8_t color = colores_piezas[pieza->tipo];

    for (int16_t fila = 0; fila < 4; fila++) {
        for (int16_t col = 0; col < 4; col++) {
            if (!forma[fila][col]) {
                continue;
            }

            int16_t tablero_fila = pieza->fila + fila;
            int16_t tablero_col = pieza->columna + col;

            if (tablero_fila >= TABLERO_FILAS_OCULTAS &&
                tablero_fila < TABLERO_FILAS &&
                tablero_col >= 0 &&
                tablero_col < TABLERO_COLUMNAS) {
                dibujar_mino_tablero(render, tablero_col, tablero_fila - TABLERO_FILAS_OCULTAS, color);
            }
        }
    }
}

 void dibujar_estadisticas(const tRender *render)
{
    static const uint8_t pieza_t[4][4] = {
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    static const uint8_t pieza_z[4][4] = {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    static const uint8_t pieza_o[4][4] = {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    static const uint8_t pieza_l[4][4] = {
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    };
    static const uint8_t pieza_s[4][4] = {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    static const uint8_t pieza_j[4][4] = {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    };
    static const uint8_t pieza_i[4][4] = {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    const uint8_t (*piezas[7])[4] = {pieza_t, pieza_z, pieza_o, pieza_l, pieza_s, pieza_j, pieza_i};
    const char *valores[7] = {"002", "004", "002", "002", "003", "003", "002"};

    dibujar_texto(render, 28, 62, "STATISTICS", COLOR_CIAN_BRILLANTE, 1);

    for (uint16_t i = 0; i < 7; i++) {
        uint16_t y = 78 + (i * 15);
        dibujar_muestra_tetromino(render, 32, y, piezas[i], i == 2 ? COLOR_AZUL_BRILLANTE : COLOR_AZUL);
        dibujar_texto(render, 72, y + 3, valores[i], COLOR_ROJO, 1);
    }
}

 void dibujar_pantalla_base(const tConfiguracionVentana *config)
{
    static const uint8_t pieza_next[4][4] = {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    tRender render = crear_render(config);

    dibujar_fondo_ladrillos(&render);

    dibujar_marco_nes(&render, 12, 14, 94, 30);
    dibujar_texto(&render, 28, 25, "A-TYPE", COLOR_CIAN_BRILLANTE, 2);

    dibujar_marco_nes(&render, 8, 54, 102, 134);
    dibujar_estadisticas(&render);

    dibujar_marco_nes(&render, 112, 8, 112, 24);
    dibujar_texto(&render, 115, 15, "LINES-000", COLOR_CIAN_BRILLANTE, 2);

    dibujar_marco_nes(&render, 118, 32, 82, 156);

    dibujar_marco_nes(&render, 226, 8, 86, 70);
    dibujar_texto(&render, 236, 20, "TOP", COLOR_CIAN_BRILLANTE, 2);
    dibujar_texto(&render, 232, 34, "010000", COLOR_CIAN_BRILLANTE, 2);
    dibujar_texto(&render, 232, 48, "SCORE", COLOR_CIAN_BRILLANTE, 2);
    dibujar_texto(&render, 232, 62, "000000", COLOR_CIAN_BRILLANTE, 2);

    dibujar_marco_nes(&render, 224, 84, 66, 48);
    dibujar_texto(&render, 230, 91, "NEXT", COLOR_CIAN_BRILLANTE, 2);
    dibujar_muestra_tetromino(&render, 242, 108, pieza_next, COLOR_AZUL);

    dibujar_marco_nes(&render, 224, 140, 66, 36);
    dibujar_texto(&render, 229, 148, "LEVEL", COLOR_CIAN_BRILLANTE, 2);
    dibujar_texto(&render, 252, 162, "00", COLOR_AZUL, 2);
    dibujar_texto(&render, 66, 176,"Q -> MAIN MENU",COLOR_ROJO,2);
}
void dibujar_menu_principal(const tConfiguracionVentana *config, uint8_t opcion)
{
    tRender render = crear_render(config);

    dibujar_fondo_ladrillos(&render);

    dibujar_marco_nes(&render, 60, 32, 220, 146);

    dibujar_texto(&render, 95, 48, "TETRIS", COLOR_CIAN_BRILLANTE, 3);

    if (opcion == 0) {
        dibujar_texto(&render, 92, 88, "> JUGAR", COLOR_AZUL, 2);
    } else {
        dibujar_texto(&render, 92, 88, "  JUGAR", COLOR_ROJO, 2);
    }

    if (opcion == 1) {
        dibujar_texto(&render, 92, 116, "> INSTRUCCIONES", COLOR_AZUL, 2);
    } else {
        dibujar_texto(&render, 92, 116, "  INSTRUCCIONES", COLOR_ROJO, 2);
    }

    if (opcion == 2) {
        dibujar_texto(&render, 92, 144, "> SALIR", COLOR_AZUL, 2);
    } else {
        dibujar_texto(&render, 92, 144, "  SALIR", COLOR_ROJO, 2);
    }
}
void dibujar_submenu_juego(const tConfiguracionVentana *config, uint8_t opcion)
{
    tRender render = crear_render(config);

    dibujar_fondo_ladrillos(&render);

    dibujar_marco_nes(&render, 60, 32, 220, 146);

    dibujar_texto(&render, 78, 58, "SELECCION", COLOR_CIAN_BRILLANTE, 2);

    // 0 -> CONTINUAR
    if (opcion == 0) {
        dibujar_texto(&render, 72, 88, "> CONTINUAR", COLOR_AZUL, 2);
    } else {
        dibujar_texto(&render, 72, 88, "  CONTINUAR", COLOR_ROJO, 2);
    }

    // 1 -> PARTIDA NUEVA
    if (opcion == 1) {
        dibujar_texto(&render, 72, 116, "> PARTIDA NUEVA", COLOR_AZUL, 2);
    } else {
        dibujar_texto(&render, 72, 116, "  PARTIDA NUEVA", COLOR_ROJO, 2);
    }

    // 2 -> MENU PRINCIPAL
    if (opcion == 2) {
        dibujar_texto(&render, 72, 144, "> MENU PRINC", COLOR_AZUL, 2);
    } else {
        dibujar_texto(&render, 72, 144, "  MENU PRINC", COLOR_ROJO, 2);
    }
}
void dibujar_instrucciones(const tConfiguracionVentana *config)
{
    tRender render = crear_render(config);

    dibujar_fondo_ladrillos(&render);

    dibujar_marco_nes(&render, 28, 18, 264, 164);

    dibujar_texto(&render, 68, 28, "INSTRUCCIONES", COLOR_CIAN_BRILLANTE, 2);

    // Flechas
    dibujar_texto(&render, 138, 58, "^", COLOR_AZUL, 2);
    dibujar_texto(&render, 112, 82, "<", COLOR_AZUL, 2);
    dibujar_texto(&render, 164, 82, ">", COLOR_AZUL, 2);
    dibujar_texto(&render, 138, 106, "V", COLOR_AZUL, 2);

    dibujar_texto(&render, 190, 58, "ROTAR", COLOR_CIAN_BRILLANTE, 1);
    dibujar_texto(&render, 190, 82, "MOVER", COLOR_CIAN_BRILLANTE, 1);
    dibujar_texto(&render, 190, 106, "BAJAR", COLOR_CIAN_BRILLANTE, 1);

    // ENTER
    dibujar_marco_nes(&render, 46, 122, 70, 26);
    dibujar_texto(&render, 56, 130, "ENTER", COLOR_AZUL, 1);

    dibujar_texto(&render, 128, 130, "INICIAR", COLOR_CIAN_BRILLANTE, 1);

    // Q
    dibujar_marco_nes(&render, 46, 152, 36, 20);
    dibujar_texto(&render, 58, 158, "Q", COLOR_AZUL, 1);

    dibujar_texto(&render, 94, 158, "MENU", COLOR_CIAN_BRILLANTE, 1);
}

uint8_t eliminar_lineas(tTablero tablero)
{
    uint8_t lineas_eliminadas = 0;

    for (int fila = TABLERO_FILAS - 1; fila >= 0; fila--) {

        uint8_t completa = 1;

        // Verificar si la fila esta completa
        for (int col = 0; col < TABLERO_COLUMNAS; col++) {

            if (tablero[fila][col] == 0) {
                completa = 0;
                break;
            }
        }

        // Si la fila esta completa
        if (completa) {

            lineas_eliminadas++;

            // Bajar todas las filas superiores
            for (int y = fila; y > 0; y--) {

                for (int x = 0; x < TABLERO_COLUMNAS; x++) {
                    tablero[y][x] = tablero[y - 1][x];
                }
            }

            // Vaciar fila superior
            for (int x = 0; x < TABLERO_COLUMNAS; x++) {
                tablero[0][x] = 0;
            }

            /*
             IMPORTANTE:
             Repetimos la misma fila porque ahora
             cayo una nueva desde arriba.
            */
            fila++;
        }
    }

    return lineas_eliminadas;
}
void dibujar_pausa(const tConfiguracionVentana *config)
{
    tRender render = crear_render(config);

    dibujar_marco_nes(&render, 96, 80, 128, 40);

    dibujar_texto(&render, 118, 94, "PAUSA", COLOR_ROJO, 2);
}


void reiniciar_partida(tTablero tablero, tPiezaActiva *pieza)
{
    tablero_vaciar(tablero);
    *pieza = crear_pieza_aleatoria();
}



