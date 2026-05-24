#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

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
#define TABLERO_X 124
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

#define FRAME_MS 16

extern tGBT_ColorRGB paletaNES[CANT_COLORES];
extern tGBT_ColorRGB paletaGameBoy[CANT_COLORES];
extern tGBT_ColorRGB paletaCGA[CANT_COLORES];

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
    uint8_t apoyada;
    float tiempo_apoyada;
} tPiezaActiva;

typedef enum {
    ESTADO_MENU,
    ESTADO_SUBMENU_JUEGO,
    ESTADO_DIFICULTAD,
    ESTADO_NOMBRE,
    ESTADO_CONFIG,
    ESTADO_CARGAR_CONFIG,
    ESTADO_CONFIG_SELECTOR,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_INSTRUCCIONES
} tEstadoJuego;

typedef struct {
    char nombre[32];
    uint8_t paleta;
    float velocidad_inicial;
    uint16_t ancho;
    uint16_t alto;

} tConfigGuardada;

typedef enum {
    CONFIG_GAMING = 0,
    CONFIG_ORIGINAL = 1
} tTipoConfiguracion;

typedef uint8_t tTablero[TABLERO_FILAS][TABLERO_COLUMNAS];
extern float velocidad_actual;
extern tGBT_ColorRGB paletaNES[CANT_COLORES];
extern const tGlifo5x7 fuente5x7[];
extern const uint8_t tetrominos[CANT_TETROMINOS][4][4][4];
extern const uint8_t colores_piezas[CANT_TETROMINOS];
extern const char nombres_piezas[CANT_TETROMINOS];

tConfiguracionVentana obtener_configuracion(tOpcionesArranque opciones);
uint8_t es_resolucion(const char *valor);
uint16_t normalizar_escala(int escala);
void aplicar_resolucion(tOpcionesArranque *opciones, const char *valor);
tOpcionesArranque leer_opciones_arranque(int argc, char *argv[]);

tRender crear_render(const tConfiguracionVentana *config);

void dibujar_rectangulo_px(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color);
void dibujar_rectangulo(const tRender *render, uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color);
void dibujar_marco_nes(const tRender *render, uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto);

const uint8_t *buscar_glifo(char caracter);
void dibujar_texto(const tRender *render, uint16_t x, uint16_t y, const char *texto, uint8_t color, uint16_t tam);
void dibujar_fondo_ladrillos(const tRender *render);

void dibujar_mino(const tRender *render, uint16_t x, uint16_t y, uint8_t color);
void dibujar_mino_tablero(const tRender *render, uint16_t columna, uint16_t fila_visible, uint8_t color);
void dibujar_muestra_tetromino(const tRender *render, uint16_t x, uint16_t y, const uint8_t forma[4][4], uint8_t color);
void dibujar_mino_chico(const tRender *render,uint16_t x,uint16_t y,uint8_t color);
void dibujar_muestra_tetromino_chico(const tRender *render,uint16_t x,uint16_t y,const uint8_t forma[4][4],uint8_t color);

void tablero_vaciar(tTablero tablero);
tPiezaActiva crear_pieza_aleatoria(void);

uint8_t puede_ubicar(const tTablero tablero, const tPiezaActiva *pieza, int16_t delta_fila, int16_t delta_columna);
uint8_t intentar_mover(tTablero tablero, tPiezaActiva *pieza, int16_t delta_fila, int16_t delta_columna, const char *origen);
uint8_t intentar_rotar(tTablero tablero, tPiezaActiva *pieza, int8_t direccion, const char *origen);

void reiniciar_partida(tTablero tablero, tPiezaActiva *pieza);

void fijar_pieza(tTablero tablero, const tPiezaActiva *pieza);
uint8_t eliminar_lineas(tTablero tablero);

void dibujar_submenu_juego(const tConfiguracionVentana *config, uint8_t opcion);
void dibujar_pausa(const tConfiguracionVentana *config);

void dibujar_tablero(const tRender *render, const tTablero tablero);
void dibujar_pieza_activa(const tRender *render, const tPiezaActiva *pieza);

void dibujar_estadisticas(const tRender *render,const uint32_t estadisticas[]);
void dibujar_pantalla_base(const tConfiguracionVentana *config,uint32_t score,uint32_t top_score,uint32_t lineas,const tPiezaActiva *siguiente_pieza,const uint32_t estadisticas[]);
void dibujar_instrucciones(const tConfiguracionVentana *config);

void dibujar_menu_principal(const tConfiguracionVentana *config, uint8_t opcion);

void dibujar_pantalla_dificultad(const tConfiguracionVentana *config,uint8_t opcion);

void dibujar_ingreso_nombre(const tConfiguracionVentana *config,const char *nombre);
void dibujar_cargar_config(tRender *render, tConfigGuardada *cfg, uint8_t opcion);
void dibujar_configuracion(tRender *render,tConfigGuardada *cfg);

uint8_t guardar_configuracion(const char *ruta,const tConfigGuardada *config);

uint8_t cargar_configuracion(const char *ruta,tConfigGuardada *config);
void dibujar_selector_configuracion(const tConfiguracionVentana *config, uint8_t opcion);

#endif // FUNCIONES_H_INCLUDED
