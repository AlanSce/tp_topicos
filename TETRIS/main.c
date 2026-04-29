/*
Apellido: Scelza, Alan Daniel
DNI: 42647760
Usuario: AlanSce
Entrega: Sí



*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "GBT/gbt.h"

#define ESCALA_DEFAULT 1
#define ESCALA_MINIMA 1
#define ESCALA_MAXIMA 6
#define CANT_COLORES 16

#define RES_CGA_ANCHO 320
#define RES_CGA_ALTO 200
#define RES_VGA_ANCHO 640
#define RES_VGA_ALTO 480

#define LIENZO_BASE_ANCHO 320
#define LIENZO_BASE_ALTO 200

#define TABLERO_COLUMNAS 10
#define TABLERO_FILAS_VISIBLES 20
#define TABLERO_FILAS_OCULTAS 2
#define TABLERO_FILAS (TABLERO_FILAS_VISIBLES + TABLERO_FILAS_OCULTAS)
#define TAM_CELDA 7
#define TABLERO_X 118
#define TABLERO_Y 38
#define CAIDA_INICIAL_SEGUNDOS 0.6

#define COLOR_NEGRO 0
#define COLOR_AZUL 1
#define COLOR_ROJO 4
#define COLOR_GRIS_CLARO 7
#define COLOR_GRIS_OSCURO 8
#define COLOR_AZUL_BRILLANTE 9
#define COLOR_CIAN_BRILLANTE 11
#define COLOR_BLANCO 15

typedef enum {
    RESOLUCION_CGA,
    RESOLUCION_VGA
} tResolucionLogica;

typedef struct {
    const char *nombre;
    uint16_t ancho;
    uint16_t alto;
    uint16_t escala;
} tConfiguracionVentana;

typedef struct {
    tResolucionLogica resolucion;
    uint16_t escala;
} tOpcionesArranque;

typedef struct {
    uint16_t escala;
    uint16_t offset_y;
} tRender;

typedef struct {
    char caracter;
    uint8_t filas[7];
} tGlifo5x7;

typedef enum {
    PIEZA_I,
    PIEZA_J,
    PIEZA_L,
    PIEZA_O,
    PIEZA_S,
    PIEZA_T,
    PIEZA_Z,
    CANT_TETROMINOS
} tTipoPieza;

typedef struct {
    tTipoPieza tipo;
    uint8_t rotacion;
    int16_t fila;
    int16_t columna;
} tPiezaActiva;

typedef uint8_t tTablero[TABLERO_FILAS][TABLERO_COLUMNAS];

static tGBT_ColorRGB paletaNES[CANT_COLORES] = {
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

static const tGlifo5x7 fuente5x7[] = {
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
    {'Y', {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}}
};

static const uint8_t tetrominos[CANT_TETROMINOS][4][4][4] = {
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

static const uint8_t colores_piezas[CANT_TETROMINOS] = {
    COLOR_CIAN_BRILLANTE,
    COLOR_AZUL,
    COLOR_GRIS_CLARO,
    14,
    2,
    5,
    COLOR_ROJO
};

static const char nombres_piezas[CANT_TETROMINOS] = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};

static tConfiguracionVentana obtener_configuracion(tOpcionesArranque opciones)
{
    if (opciones.resolucion == RESOLUCION_CGA) {
        return (tConfiguracionVentana) {"Tetris NES 1989 - CGA", RES_CGA_ANCHO, RES_CGA_ALTO, opciones.escala};
    }

    return (tConfiguracionVentana) {"Tetris NES 1989 - VGA", RES_VGA_ANCHO, RES_VGA_ALTO, opciones.escala};
}

static uint8_t es_resolucion(const char *valor)
{
    return strcmp(valor, "cga") == 0 || strcmp(valor, "vga") == 0;
}

static uint16_t normalizar_escala(int escala)
{
    if (escala < ESCALA_MINIMA) {
        return ESCALA_MINIMA;
    }

    if (escala > ESCALA_MAXIMA) {
        return ESCALA_MAXIMA;
    }

    return (uint16_t) escala;
}

static void aplicar_resolucion(tOpcionesArranque *opciones, const char *valor)
{
    if (strcmp(valor, "cga") == 0) {
        opciones->resolucion = RESOLUCION_CGA;
    } else if (strcmp(valor, "vga") == 0) {
        opciones->resolucion = RESOLUCION_VGA;
    }
}

static tOpcionesArranque leer_opciones_arranque(int argc, char *argv[])
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

static tRender crear_render(const tConfiguracionVentana *config)
{
    tRender render;

    render.escala = config->ancho / LIENZO_BASE_ANCHO;
    render.offset_y = (config->alto - (LIENZO_BASE_ALTO * render.escala)) / 2;

    return render;
}

static void dibujar_rectangulo_px(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color)
{
    for (uint16_t fila = 0; fila < alto; fila++) {
        for (uint16_t col = 0; col < ancho; col++) {
            gbt_dibujar_pixel(x + col, y + fila, color);
        }
    }
}

static void dibujar_rectangulo(const tRender *render, uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color)
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

static void dibujar_marco_nes(const tRender *render, uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto)
{
    dibujar_rectangulo(render, x, y, ancho, alto, COLOR_GRIS_OSCURO);
    dibujar_rectangulo(render, x + 2, y + 2, ancho - 4, alto - 4, COLOR_CIAN_BRILLANTE);
    dibujar_rectangulo(render, x + 4, y + 4, ancho - 8, alto - 8, COLOR_GRIS_CLARO);
    dibujar_rectangulo(render, x + 6, y + 6, ancho - 12, alto - 12, COLOR_NEGRO);
    dibujar_rectangulo(render, x + 2, y + alto - 4, ancho - 4, 2, COLOR_GRIS_OSCURO);
    dibujar_rectangulo(render, x + ancho - 4, y + 2, 2, alto - 4, COLOR_GRIS_OSCURO);
}

static const uint8_t *buscar_glifo(char caracter)
{
    for (uint16_t i = 0; i < sizeof(fuente5x7) / sizeof(fuente5x7[0]); i++) {
        if (fuente5x7[i].caracter == caracter) {
            return fuente5x7[i].filas;
        }
    }

    return fuente5x7[0].filas;
}

static void dibujar_texto(const tRender *render, uint16_t x, uint16_t y, const char *texto, uint8_t color, uint16_t tam)
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

static void dibujar_fondo_ladrillos(const tRender *render)
{
    gbt_borrar_backbuffer(COLOR_NEGRO);

    for (uint16_t y = 0; y < LIENZO_BASE_ALTO; y += 12) {
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

static void dibujar_mino(const tRender *render, uint16_t x, uint16_t y, uint8_t color)
{
    dibujar_rectangulo(render, x, y, 8, 8, COLOR_BLANCO);
    dibujar_rectangulo(render, x + 1, y + 1, 6, 6, color);
    dibujar_rectangulo(render, x + 2, y + 2, 2, 2, COLOR_CIAN_BRILLANTE);
}

static void dibujar_mino_tablero(const tRender *render, uint16_t columna, uint16_t fila_visible, uint8_t color)
{
    uint16_t x = TABLERO_X + (columna * TAM_CELDA);
    uint16_t y = TABLERO_Y + (fila_visible * TAM_CELDA);

    dibujar_rectangulo(render, x, y, TAM_CELDA, TAM_CELDA, COLOR_BLANCO);
    dibujar_rectangulo(render, x + 1, y + 1, TAM_CELDA - 1, TAM_CELDA - 1, color);
    dibujar_rectangulo(render, x + 2, y + 2, 2, 2, COLOR_CIAN_BRILLANTE);
}

static void dibujar_muestra_tetromino(const tRender *render, uint16_t x, uint16_t y, const uint8_t forma[4][4], uint8_t color)
{
    for (uint16_t fila = 0; fila < 4; fila++) {
        for (uint16_t col = 0; col < 4; col++) {
            if (forma[fila][col]) {
                dibujar_mino(render, x + (col * 8), y + (fila * 8), color);
            }
        }
    }
}

static void tablero_vaciar(tTablero tablero)
{
    for (uint16_t fila = 0; fila < TABLERO_FILAS; fila++) {
        for (uint16_t col = 0; col < TABLERO_COLUMNAS; col++) {
            tablero[fila][col] = 0;
        }
    }
}

static tPiezaActiva crear_pieza_aleatoria(void)
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

static uint8_t puede_ubicar(const tTablero tablero, const tPiezaActiva *pieza, int16_t delta_fila, int16_t delta_columna)
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

static uint8_t intentar_mover(tTablero tablero, tPiezaActiva *pieza, int16_t delta_fila, int16_t delta_columna, const char *origen)
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

static uint8_t intentar_rotar(tTablero tablero, tPiezaActiva *pieza, int8_t direccion, const char *origen)
{
    uint8_t rotacion_anterior = pieza->rotacion;

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

static void fijar_pieza(tTablero tablero, const tPiezaActiva *pieza)
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

static void dibujar_tablero(const tRender *render, const tTablero tablero)
{
    for (uint16_t fila = TABLERO_FILAS_OCULTAS; fila < TABLERO_FILAS; fila++) {
        for (uint16_t col = 0; col < TABLERO_COLUMNAS; col++) {
            if (tablero[fila][col] != 0) {
                dibujar_mino_tablero(render, col, fila - TABLERO_FILAS_OCULTAS, tablero[fila][col]);
            }
        }
    }
}

static void dibujar_pieza_activa(const tRender *render, const tPiezaActiva *pieza)
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

static void dibujar_estadisticas(const tRender *render)
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

    dibujar_texto(render, 28, 62, "STATISTICS", COLOR_BLANCO, 1);

    for (uint16_t i = 0; i < 7; i++) {
        uint16_t y = 78 + (i * 15);
        dibujar_muestra_tetromino(render, 32, y, piezas[i], i == 2 ? COLOR_AZUL_BRILLANTE : COLOR_AZUL);
        dibujar_texto(render, 72, y + 3, valores[i], COLOR_ROJO, 1);
    }
}

static void dibujar_pantalla_base(const tConfiguracionVentana *config)
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
    dibujar_texto(&render, 28, 25, "A-TYPE", COLOR_BLANCO, 2);

    dibujar_marco_nes(&render, 8, 54, 102, 134);
    dibujar_estadisticas(&render);

    dibujar_marco_nes(&render, 112, 8, 112, 24);
    dibujar_texto(&render, 115, 15, "LINES-000", COLOR_BLANCO, 2);

    dibujar_marco_nes(&render, 112, 32, 82, 156);

    dibujar_marco_nes(&render, 226, 8, 86, 70);
    dibujar_texto(&render, 236, 20, "TOP", COLOR_BLANCO, 2);
    dibujar_texto(&render, 232, 34, "010000", COLOR_BLANCO, 2);
    dibujar_texto(&render, 232, 48, "SCORE", COLOR_BLANCO, 2);
    dibujar_texto(&render, 232, 62, "000000", COLOR_BLANCO, 2);

    dibujar_marco_nes(&render, 224, 84, 66, 48);
    dibujar_texto(&render, 230, 91, "NEXT", COLOR_BLANCO, 2);
    dibujar_muestra_tetromino(&render, 242, 108, pieza_next, COLOR_AZUL);

    dibujar_marco_nes(&render, 224, 140, 66, 36);
    dibujar_texto(&render, 229, 148, "LEVEL", COLOR_BLANCO, 2);
    dibujar_texto(&render, 252, 162, "00", COLOR_BLANCO, 2);
}

int main(int argc, char *argv[])
{
    tOpcionesArranque opciones = leer_opciones_arranque(argc, argv);
    tConfiguracionVentana config = obtener_configuracion(opciones);
    tRender render = crear_render(&config);
    tTablero tablero;
    tPiezaActiva pieza;

    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if (gbt_crear_ventana(config.nombre, config.ancho, config.alto, config.escala) != 0) {
        fprintf(stderr, "Error al crear la ventana: %s\n", gbt_obtener_log());
        gbt_cerrar();
        return -1;
    }

    if (gbt_aplicar_paleta(paletaNES, CANT_COLORES, GBT_FORMATO_888) != 0) {
        fprintf(stderr, "Error al aplicar la paleta: %s\n", gbt_obtener_log());
        gbt_destruir_ventana();
        gbt_cerrar();
        return -1;
    }

    tGBT_Temporizador *temporizador_caida = gbt_temporizador_crear(CAIDA_INICIAL_SEGUNDOS);
    if (!temporizador_caida) {
        fprintf(stderr, "Error al crear temporizador de caida: %s\n", gbt_obtener_log());
        gbt_destruir_ventana();
        gbt_cerrar();
        return -1;
    }

    srand((unsigned int) time(NULL));
    tablero_vaciar(tablero);
    pieza = crear_pieza_aleatoria();

    uint8_t corriendo = 1;

    while (corriendo) {
        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if (tecla == GBTK_ESCAPE) {
            printf("[tecla] ESCAPE\n");
            corriendo = 0;
        } else if (tecla == GBTK_IZQUIERDA) {
            printf("[tecla] IZQUIERDA\n");
            intentar_mover(tablero, &pieza, 0, -1, "tecla izquierda");
        } else if (tecla == GBTK_DERECHA) {
            printf("[tecla] DERECHA\n");
            intentar_mover(tablero, &pieza, 0, 1, "tecla derecha");
        } else if (tecla == GBTK_ABAJO) {
            printf("[tecla] ABAJO\n");
            intentar_mover(tablero, &pieza, 1, 0, "tecla abajo");
        } else if (tecla == GBTK_ARRIBA) {
            printf("[tecla] ARRIBA\n");
            intentar_rotar(tablero, &pieza, 1, "tecla arriba");
        } else if (tecla == GBTK_z) {
            printf("[tecla] Z\n");
            intentar_rotar(tablero, &pieza, -1, "tecla z");
        } else if (tecla != GBTK_DESCONOCIDA) {
            printf("[tecla] codigo=%d\n", tecla);
        }

        if (gbt_temporizador_consumir(temporizador_caida)) {
            printf("[timer] caida consumida, intento bajar pieza %c\n", nombres_piezas[pieza.tipo]);

            if (!intentar_mover(tablero, &pieza, 1, 0, "timer")) {
                fijar_pieza(tablero, &pieza);
                pieza = crear_pieza_aleatoria();

                if (!puede_ubicar(tablero, &pieza, 0, 0)) {
                    printf("[game] no hay espacio para nueva pieza, termina la demo\n");
                    corriendo = 0;
                }
            }
        }

        dibujar_pantalla_base(&config);
        dibujar_tablero(&render, tablero);
        dibujar_pieza_activa(&render, &pieza);

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(temporizador_caida);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}
