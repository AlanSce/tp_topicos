/*
Apellido: Scelza, Alan Daniel
DNI: 42647760
Usuario: AlanSce
Entrega: Sí

Apellido: Leal, Luciano Gaspar
DNI: 40514151
Usuario: Lucianoleal97
Entrega: Si


*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lib/GBT_v2026.1C.01/include/GBT/gbt.h"
#include "Funciones.h"


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
