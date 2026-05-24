/*
Apellido: Scelza, Alan Daniel
DNI: 42647760
Usuario: AlanSce
Entrega: Sí

Apellido: Leal, Luciano Gaspar
DNI: 40514151
Usuario: Lucianoleal97
Entrega: Si

Apellido: Buccini, Alejandro Gabriel
DNI: 40378757
Usuario: Arthillery
Entrega: Si
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "lib/GBT_v2026.1C.01/include/GBT/gbt.h"
#include "Funciones.h"

float velocidad_actual = CAIDA_INICIAL_SEGUNDOS; //variable global, la necesito en memoria para el control de la pantalla base
tConfigGuardada config_guardada;
uint8_t paleta_actual = 0;

int main(int argc, char *argv[])
{
    tOpcionesArranque opciones = leer_opciones_arranque(argc, argv);

    tConfiguracionVentana config = obtener_configuracion(opciones);

    tRender render = crear_render(&config);

    tTablero tablero;

    tPiezaActiva pieza;
    tPiezaActiva siguiente_pieza;

    tEstadoJuego estado = ESTADO_MENU;

    uint8_t opcion_menu = 0;
    uint8_t opcion_submenu = 0;
    uint8_t opcion_dificultad = 0;
    uint8_t hay_partida = 0;

    uint32_t score = 0;
    uint32_t top_score = 0;
    uint32_t lineas_totales = 0;

    uint32_t estadisticas_piezas[CANT_TETROMINOS] = {0};

    char nombre_jugador[32] = "JUGADOR";
    strcpy(config_guardada.nombre, "JUGADOR");

    config_guardada.paleta = 0;

    config_guardada.velocidad_inicial =CAIDA_INICIAL_SEGUNDOS;

    config_guardada.ancho = config.ancho;
    config_guardada.alto = config.alto;


    float velocidad_base = CAIDA_INICIAL_SEGUNDOS;
    uint32_t piezas_caidas = 0;

    if (gbt_iniciar() != 0) {

        fprintf(stderr, "Error al iniciar GBT: %s\n",
                gbt_obtener_log());

        return -1;
    }

    if (gbt_crear_ventana(config.nombre,config.ancho,config.alto,config.escala) != 0) {

        fprintf(stderr,
                "Error al crear la ventana: %s\n",
                gbt_obtener_log());

        gbt_cerrar();

        return -1;
    }

tGBT_ColorRGB *paleta_a_usar = paletaNES;

  /*  if (config_guardada.paleta == 1) {
        paleta_a_usar = paletaGameBoy;
    }

    else if (config_guardada.paleta == 2) {
        paleta_a_usar = paletaCGA;
    }*/

    if (gbt_aplicar_paleta(
            paleta_a_usar,
            CANT_COLORES,
            GBT_FORMATO_888) != 0) {

        fprintf(stderr,
                "Error al aplicar la paleta: %s\n",
                gbt_obtener_log());

        gbt_destruir_ventana();

        gbt_cerrar();

        return -1;
    }

    tGBT_Temporizador *temporizador_caida =
        gbt_temporizador_crear(CAIDA_INICIAL_SEGUNDOS);

    if (!temporizador_caida) {

        fprintf(stderr,
                "Error al crear temporizador de caida: %s\n",
                gbt_obtener_log());

        gbt_destruir_ventana();

        gbt_cerrar();

        return -1;
    }

    srand((unsigned int) time(NULL));

    tablero_vaciar(tablero);

  //  pieza = crear_pieza_aleatoria();
 //   siguiente_pieza = crear_pieza_aleatoria();

   // estadisticas_piezas[pieza.tipo]++;

    uint8_t corriendo = 1;

    while (corriendo) {

        gbt_procesar_entrada();

        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if (estado == ESTADO_MENU) {

            if (tecla == GBTK_ABAJO) {

                opcion_menu = (opcion_menu + 1) % 3;
            }

            if (tecla == GBTK_ARRIBA) {

                if (opcion_menu == 0) {
                    opcion_menu = 2;
                } else {
                    opcion_menu--;
                }
            }

            if (tecla == GBTK_ENTER) {

                if (opcion_menu == 0) {

                    estado = ESTADO_SUBMENU_JUEGO;
                }

                else if (opcion_menu == 1) {

                    estado = ESTADO_INSTRUCCIONES;
                }

                else {

                    corriendo = 0;
                }
            }


            dibujar_menu_principal(&config, opcion_menu);

            gbt_volcar_backbuffer();

            gbt_esperar(16);

            continue;
        }

        if (estado == ESTADO_SUBMENU_JUEGO) {

            if (tecla == GBTK_ABAJO) {

                if (opcion_submenu < 3) {
                    opcion_submenu++;
                }
            }

            if (tecla == GBTK_ARRIBA) {

                if (opcion_submenu > 0) {
                    opcion_submenu--;
                }
            }

            if (tecla == GBTK_ENTER) {

                switch (opcion_submenu) {

                    case 0:

                        if (hay_partida) {

                            estado = ESTADO_JUGANDO;
                        }

                        break;

                    case 1:

                        estado = ESTADO_DIFICULTAD;

                        break;

                    case 2:

                        estado = ESTADO_CARGAR_CONFIG;

                        break;

                    case 3:

                        estado = ESTADO_MENU;

                        break;
                }
            }

            if (tecla == GBTK_q) {

                estado = ESTADO_MENU;
            }

            dibujar_submenu_juego(&config, opcion_submenu);

            gbt_volcar_backbuffer();

            gbt_esperar(16);

            continue;
        }

        if (estado == ESTADO_DIFICULTAD) {

            if (tecla == GBTK_ARRIBA && opcion_dificultad > 0) {

                opcion_dificultad--;
            }

            if (tecla == GBTK_ABAJO && opcion_dificultad < 2) {

                opcion_dificultad++;
            }

            if (tecla == GBTK_ENTER) {

                if (opcion_dificultad == 0) {

                    velocidad_base = 0.8f;
                }

                else if (opcion_dificultad == 1) {

                    velocidad_base = 0.6f;
                }

                else {

                    velocidad_base = 0.4f;
                }

                velocidad_actual = velocidad_base;

                gbt_temporizador_destruir(temporizador_caida);

                temporizador_caida = gbt_temporizador_crear(velocidad_actual);

                if (score > top_score) {top_score = score;}

                score = 0;
                lineas_totales = 0;
                piezas_caidas = 0;

                for (int i = 0; i < CANT_TETROMINOS; i++) {

                    estadisticas_piezas[i] = 0;
                }

                tablero_vaciar(tablero);

                pieza = crear_pieza_aleatoria();
                siguiente_pieza = crear_pieza_aleatoria();

                estadisticas_piezas[pieza.tipo]++;

                hay_partida = 1;
                strcpy(nombre_jugador, "");
                estado = ESTADO_NOMBRE;
            }

            if (tecla == GBTK_q) {

                estado = ESTADO_MENU;
            }
            dibujar_pantalla_dificultad(&config, opcion_dificultad);

            gbt_volcar_backbuffer();

            gbt_esperar(16);

            continue;
        }

        if (estado == ESTADO_NOMBRE) {

            if (tecla >= GBTK_a && tecla <= GBTK_z) {

                size_t len = strlen(nombre_jugador);

                if (len < 15) {

                    nombre_jugador[len] =
                        'A' + (tecla - GBTK_a);

                    nombre_jugador[len + 1] = '\0';
                }
            }

            if (tecla == GBTK_RETROCESO) {

                size_t len = strlen(nombre_jugador);

                if (len > 0) {

                    nombre_jugador[len - 1] = '\0';
                }
            }

            if (tecla == GBTK_ENTER) {

                FILE *archivo =fopen("estadisticas_jugador.txt", "a");

                if (archivo) {

                    fprintf(archivo,"Jugador: %s\n",nombre_jugador);

                    fclose(archivo);
                }


                estado = ESTADO_JUGANDO;
            }

            dibujar_ingreso_nombre(&config,nombre_jugador);

            gbt_volcar_backbuffer();

            gbt_esperar(16);

            continue;
        }

        if (estado == ESTADO_INSTRUCCIONES) {

            dibujar_instrucciones(&config);

            if (tecla == GBTK_q) {

                estado = ESTADO_MENU;
            }

            gbt_volcar_backbuffer();

            gbt_esperar(16);

            continue;
        }

        if (estado == ESTADO_PAUSA) {

            if (tecla == GBTK_p) {

                estado = ESTADO_JUGANDO;
            }

            dibujar_pantalla_base(&config,score,top_score,lineas_totales,&siguiente_pieza,estadisticas_piezas);

            dibujar_tablero(&render, tablero);

            dibujar_pieza_activa(&render, &pieza);

            dibujar_pausa(&config);

            gbt_volcar_backbuffer();

            gbt_esperar(16);

            continue;
        }
        if (estado == ESTADO_CARGAR_CONFIG) {

            static uint8_t opcion_config = 0; // 0 = gaming, 1 = original

            if (tecla == GBTK_ARRIBA) {
                opcion_config = 0;
            }

            if (tecla == GBTK_ABAJO) {
                opcion_config = 1;
            }

            if (tecla == GBTK_ENTER) {

                const char *archivo = NULL;

                if (opcion_config == 0) {
                    archivo = "config_gaming.dat";
                } else {
                    archivo = "config_original.dat";
                }

                if (cargar_configuracion(archivo, &config_guardada)) {

                    strcpy(nombre_jugador, config_guardada.nombre);

                    velocidad_base = config_guardada.velocidad_inicial;
                    velocidad_actual = velocidad_base;

                    gbt_temporizador_destruir(temporizador_caida);
                    temporizador_caida = gbt_temporizador_crear(velocidad_actual);

                    tGBT_ColorRGB *paleta_a_usar = paletaNES;

                    if (config_guardada.paleta == 1) {
                        paleta_a_usar = paletaGameBoy;
                    } else if (config_guardada.paleta == 2) {
                        paleta_a_usar = paletaCGA;
                    }

                    gbt_aplicar_paleta(
                        paleta_a_usar,
                        CANT_COLORES,
                        GBT_FORMATO_888
                    );

                    /*tablero_vaciar(tablero);

                    pieza = crear_pieza_aleatoria();
                    siguiente_pieza = crear_pieza_aleatoria();

                    estadisticas_piezas[pieza.tipo]++;

                    score = 0;
                    lineas_totales = 0;
                    piezas_caidas = 0;

                    hay_partida = 1;*/

                    estado = ESTADO_MENU;

                  //  opcion_config = 0; // reset
                }
                else {
                    printf("Error: no se pudo cargar %s\n", archivo);
                    estado = ESTADO_SUBMENU_JUEGO;
                }
            }

            if (tecla == GBTK_q) {
                estado = ESTADO_SUBMENU_JUEGO;
            }

            dibujar_cargar_config(&render, &config_guardada, opcion_config);
            gbt_volcar_backbuffer();
            gbt_esperar(16);

            continue;
        }

            if (estado == ESTADO_CONFIG) {

                if (tecla == GBTK_IZQUIERDA) {

                    if (config_guardada.paleta > 0) {
                        config_guardada.paleta--;
                    }
                }

                if (tecla == GBTK_DERECHA) {

                    if (config_guardada.paleta < 2) {
                        config_guardada.paleta++;
                    }
                }

                if (tecla == GBTK_ARRIBA) {

                    config_guardada.velocidad_inicial += 0.1f;
                }

                if (tecla == GBTK_ABAJO) {

                    if (config_guardada.velocidad_inicial > 0.2f) {
                        config_guardada.velocidad_inicial -= 0.1f;
                    }
                }

                if (tecla == GBTK_ENTER) {

                    strcpy(config_guardada.nombre, nombre_jugador);

                    guardar_configuracion(
                        "config.dat",
                        &config_guardada);

                    velocidad_base =
                        config_guardada.velocidad_inicial;

                    velocidad_actual =
                        velocidad_base;

                    gbt_temporizador_destruir(
                        temporizador_caida);

                    temporizador_caida =
                        gbt_temporizador_crear(
                            velocidad_actual);

                    estado = ESTADO_JUGANDO;
                }

                if (tecla == GBTK_q) {

                    estado = ESTADO_MENU;
                }

                dibujar_configuracion(&render,&config_guardada);

                gbt_volcar_backbuffer();

                gbt_esperar(16);

                continue;
        }
        if (estado == ESTADO_JUGANDO) {

            if (tecla == GBTK_p) {

                estado = ESTADO_PAUSA;

                continue;
            }

            if (tecla == GBTK_q) {

                FILE *archivo =fopen("estadisticas_jugador.txt", "a");

                if (archivo) {

                    fprintf(archivo,"Jugador: %s Score: %u Lineas: %u\n",nombre_jugador,score,lineas_totales);

                    fclose(archivo);
                }

                estado = ESTADO_MENU;

                continue;
            }

             else if (tecla == GBTK_IZQUIERDA) {

                if (intentar_mover(
                        tablero,
                        &pieza,
                        0,
                        -1,
                        "tecla izquierda")) {

                    pieza.tiempo_apoyada = 0;

                    if (!puede_ubicar(tablero, &pieza, 1, 0)) {
                        pieza.apoyada = 1;
                    }

                    else {
                        pieza.apoyada = 0;
                    }
                }
            }

            else if (tecla == GBTK_DERECHA) {

                if (intentar_mover(
                        tablero,
                        &pieza,
                        0,
                        1,
                        "tecla derecha")) {

                    pieza.tiempo_apoyada = 0;

                    if (!puede_ubicar(tablero, &pieza, 1, 0)) {
                        pieza.apoyada = 1;
                    }

                    else {
                        pieza.apoyada = 0;
                    }
                }
            }

            else if (tecla == GBTK_ABAJO) {

                if (intentar_mover(
                        tablero,
                        &pieza,
                        1,
                        0,
                        "tecla abajo")) {

                    score += 1;

                    pieza.tiempo_apoyada = 0;
                    pieza.apoyada = 0;
                }
            }

            else if (tecla == GBTK_ARRIBA) {

                if (intentar_rotar(
                        tablero,
                        &pieza,
                        1,
                        "tecla arriba")) {

                    pieza.tiempo_apoyada = 0;

                    if (!puede_ubicar(tablero, &pieza, 1, 0)) {
                        pieza.apoyada = 1;
                    }

                    else {
                        pieza.apoyada = 0;
                    }
                }
            }

            else if (tecla == GBTK_z) {

                if (intentar_rotar(
                        tablero,
                        &pieza,
                        -1,
                        "tecla z")) {

                    pieza.tiempo_apoyada = 0;

                    if (!puede_ubicar(tablero, &pieza, 1, 0)) {
                        pieza.apoyada = 1;
                    }

                    else {
                        pieza.apoyada = 0;
                    }
                }
            }

            if (gbt_temporizador_consumir(temporizador_caida)) {

                if (intentar_mover(
                        tablero,
                        &pieza,
                        1,
                        0,
                        "timer")) {

                    pieza.apoyada = 0;
                    pieza.tiempo_apoyada = 0;
                }

                else {

                    pieza.apoyada = 1;
                }
            }

            if (pieza.apoyada) {

                pieza.tiempo_apoyada += (FRAME_MS / 1000.0f);

                float lock_delay = velocidad_actual * 0.5f;

                if (pieza.tiempo_apoyada >= lock_delay) {

                    fijar_pieza(tablero, &pieza);

                    piezas_caidas++;

                    pieza.apoyada = 0;
                    pieza.tiempo_apoyada = 0;

                    if (piezas_caidas % 10 == 0) {

                        velocidad_actual *= 0.97f;

                        gbt_temporizador_destruir(temporizador_caida);

                        temporizador_caida =
                            gbt_temporizador_crear(
                                velocidad_actual);
                    }

                    uint8_t lineas =
                        eliminar_lineas(tablero);

                    if (lineas > 0) {

                        uint32_t multiplicador =
                            (uint32_t)(1.0f / velocidad_actual);

                        if (multiplicador < 1) {
                            multiplicador = 1;
                        }

                        uint32_t puntos =
                            (lineas * (100 + ((lineas - 1) * 10)))
                            * multiplicador;

                        score += puntos;

                        lineas_totales += lineas;

                        printf(
                            "[lineas] eliminadas=%d puntos=%u score=%u\n",
                            lineas,
                            puntos,
                            score);
                    }

                    pieza = siguiente_pieza;

                    estadisticas_piezas[pieza.tipo]++;

                    siguiente_pieza =
                        crear_pieza_aleatoria();

                    if (!puede_ubicar(
                            tablero,
                            &pieza,
                            0,
                            0)) {

                        printf("[game] game over\n");

                        FILE *archivo =
                            fopen(
                                "estadisticas_jugador.txt",
                                "a");

                        if (archivo) {

                            fprintf(
                                archivo,
                                "GAME OVER - Jugador: %s "
                                "Score: %u "
                                "Lineas: %u\n",
                                nombre_jugador,
                                score,
                                lineas_totales);

                            fclose(archivo);
                        }

                        hay_partida = 0;

                        estado = ESTADO_MENU;
                    }
                }
            }

            dibujar_pantalla_base(
                &config,
                score,
                top_score,
                lineas_totales,
                &siguiente_pieza,
                estadisticas_piezas
            );

            dibujar_tablero(&render, tablero);

            dibujar_pieza_activa(&render, &pieza);

            gbt_volcar_backbuffer();

            gbt_esperar(16);
        }
    }

    gbt_temporizador_destruir(temporizador_caida);

    gbt_destruir_ventana();

    gbt_cerrar();

    return 0;
}
