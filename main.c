// Apellido: Albarracin, José Emanuel
// DNI: 40393967
// Usuario: Jose23-A
// Entrega: Sí

#include "header.h"
//#include "matrices.h"

int main(int argc, char* argv[])
{
    int estado_actual = ESTADO_MENU; // El juego arranca en el menú principal

    int contador_fichas = 0;
    float tiempo_caida_actual = 0.5;

    char nombre_jugador[11] = "";
    int largo_nombre = 0;

    // Resolucion por defecto
    int res_ancho = RES_ANCHO_CGA;
    int res_alto = RES_ALTO_CGA;
    int escala = ESCALA_VENTANA;

    if (argc >= 3)
    {
        if (strcmp(argv[1], "VGA") == 0)
        {
            res_ancho = RES_ANCHO_VGA;
            res_alto = RES_ALTO_VGA;
        }
        else if (strcmp(argv[1], "CGA") == 0)
        {
            res_ancho = RES_ANCHO_CGA;
            res_alto = RES_ALTO_CGA;
        }
        escala = atoi(argv[2]);
    }

    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT.\n");
        return -1;
    }

    if (gbt_crear_ventana("Ficha Cayendo (Gravedad)", res_ancho, res_alto, escala) != 0)
    {
        fprintf(stderr, "Error al crear la ventana.\n");
        gbt_cerrar(); // [cite: 1]
        return -1;
    }

    gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);

    // Temporizador para la gravedad: Cae cada 0.5 segundos
    tGBT_Temporizador* temp_gravedad = gbt_temporizador_crear(tiempo_caida_actual);
    if (!temp_gravedad)
    {
        fprintf(stderr, "Error al crear temporizador.\n");
        return -1;
    }

    tGBT_Temporizador* temp_soft_drop = gbt_temporizador_crear(tiempo_caida_actual/10);
    if (!temp_soft_drop)
    {
        fprintf(stderr, "Error al crear temporizador de soft drop.\n");
        return -1;
    }

    uint8_t tablero_principal[TABLERO_ALTO][TABLERO_ANCHO];
    int puntaje_total = 0;

    for(int f = 0; f < TABLERO_ALTO; f++)
    {
        for(int c = 0; c < TABLERO_ANCHO; c++)
        {
            tablero_principal[f][c] = INDICE_TRANSP;
        }
    }

    srand(time(NULL));

    int corriendo = 1;
    printf("Programa iniciado. Ficha cayendo por gravedad.\n");
    printf("Pulse ESCAPE para salir.\n");

    int tablero_ancho_px = TABLERO_ANCHO * TAM_BLOQUE;
    int tablero_alto_px = TABLERO_ALTO * TAM_BLOQUE;

    int offset_x = (res_ancho - tablero_ancho_px) / MITAD;
    int offset_y = (res_alto - tablero_alto_px) / MITAD;

    t_tetromino pieza_activa;
    crear_pieza(&pieza_activa);

    /// --- Bucle Principal del Juego (Game Loop) ---
    while (corriendo)
    {
        // A. Procesar Entrada (solo para salir con ESC)
        gbt_procesar_entrada();
        if (gbt_tecla_presionada(GBTK_ESCAPE))
        {
            corriendo = 0;
            printf("Saliendo...\n");
        }

        gbt_borrar_backbuffer(INDICE_GRIS_CL);

        switch (estado_actual)
        {
            case ESTADO_MENU:
                // --- PANTALLA DE INICIO ---
                gbt_borrar_backbuffer(INDICE_GRIS_CL);

                int img_x = (res_ancho - SLAM_DUNK_WIDTH) / MITAD;
                dibujar_slamdunk(img_x, 10);


                // "INGRESE SU NOMBRE" tiene 17 letras -> 17 * 9 = 153 px de ancho
                mi_gbt_dibujar_texto((res_ancho - 153) / MITAD, 130, "INGRESE SU NOMBRE", INDICE_NEGRO);

                // El nombre o la guía se centran dinámicamente según el largo actual
                if (largo_nombre == 0)
                {
                    // "________" tiene 8 caracteres -> 8 * 9 = 72 px
                    mi_gbt_dibujar_texto((res_ancho - 72) / MITAD, 90, "________", INDICE_NEGRO);
                }
                else
                {
                    // Centramos el nombre según la cantidad de letras que tenga tipeadas
                    int ancho_nombre_px = largo_nombre * 9;
                    mi_gbt_dibujar_texto((res_ancho - ancho_nombre_px) / MITAD, 140, nombre_jugador, INDICE_NEGRO);
                }

                // "PRESIONE ENTER PARA JUGAR" tiene 25 letras -> 25 * 9 = 225 px de ancho
                mi_gbt_dibujar_texto((res_ancho - 130) / MITAD, 160, "PRESIONE ENTER", INDICE_NEGRO);
                mi_gbt_dibujar_texto((res_ancho - 100) / MITAD, 168, "PARA JUGAR", INDICE_NEGRO);

                // --- LÓGICA DE TIPEO (Mismo código de antes) ---
                if (largo_nombre < 10)
                {
                    for (int i = 0; i < 26; i++)
                    {
                        if (gbt_tecla_presionada(GBTK_a + i))
                        {
                            nombre_jugador[largo_nombre] = 'A' + i;
                            largo_nombre++;
                            nombre_jugador[largo_nombre] = '\0';
                            break;
                        }
                    }
                }

                // --- LÓGICA DE BORRADO ---
                if (gbt_tecla_presionada(GBTK_RETROCESO) && largo_nombre > 0)
                {
                    largo_nombre--;
                    nombre_jugador[largo_nombre] = '\0';
                }

                if (gbt_tecla_presionada(GBTK_ENTER))
                {
                    estado_actual = ESTADO_JUGANDO;
                }
                break;

            case ESTADO_JUGANDO:

                gbt_borrar_backbuffer(INDICE_NEGRO);

                if(gbt_tecla_presionada(GBTK_IZQUIERDA) && !choca_izquierda(&pieza_activa, tablero_principal))
                {
                    pieza_activa.x--;
                }
                if(gbt_tecla_presionada(GBTK_DERECHA) && !choca_derecha(&pieza_activa, tablero_principal))
                {
                    pieza_activa.x++;
                }
                if(gbt_tecla_presionada(GBTK_q))
                {
                    rotar_pieza(&pieza_activa, GIRO_Q, tablero_principal); // Izquierda
                }
                if(gbt_tecla_presionada(GBTK_e))
                {
                    rotar_pieza(&pieza_activa, GIRO_E, tablero_principal);  // Derecha
                }

                if(gbt_tecla_sostenida(GBTK_ABAJO))
                {
                    if (gbt_temporizador_consumir(temp_soft_drop))
                    {
                        if (choca_abajo(&pieza_activa, tablero_principal) == 0)
                        {
                            pieza_activa.y++;
                            puntaje_total += 1;
                        }
                    }
                }

                // Actualización Lógica (La Gravedad)
                // Si se cumplió el tiempo (0.5s), movemos la pieza hacia abajo.
                if (gbt_temporizador_consumir(temp_gravedad))
                {

                    if (choca_abajo(&pieza_activa, tablero_principal) == 0)
                    {
                        // La pieza baja una fila
                        pieza_activa.y++;
                    }
                    else
                    {
                        // La pieza tocó el fondo o algo más
                        // La dejamos "pintada" permanentemente en el tablero
                        fijar_pieza(&pieza_activa, tablero_principal);

                        int cant_lineas = limpiar_lineas(tablero_principal);
                        if (cant_lineas > 0)
                        {
                            // Sistema de puntos clásico (más líneas juntas = más puntos)
                            if (cant_lineas == 1) puntaje_total += 100;
                            else if (cant_lineas == 2) puntaje_total += 300;
                            else if (cant_lineas == 3) puntaje_total += 500;
                            else if (cant_lineas == 4) puntaje_total += 800; // ¡Un Tetris!

                            printf("¡SLAM DUNK! Borraste %d linea(s). Puntaje: %d\n", cant_lineas, puntaje_total);
                        }
                        contador_fichas++;
                        if (contador_fichas % 10 == 0)
                        {
                            // Reducimos el tiempo un 3% (multiplicar por 0.97)
                            tiempo_caida_actual = tiempo_caida_actual * 0.97f;

                            // Destruimos el timer viejo y creamos el nuevo acelerado
                            gbt_temporizador_destruir(temp_gravedad);
                            temp_gravedad = gbt_temporizador_crear(tiempo_caida_actual);
                        }
                        // Pedimos una nueva pieza al azar para que aparezca arriba
                        crear_pieza(&pieza_activa);

                        if (es_posicion_valida(&pieza_activa, tablero_principal) == 0)
                        {
                            estado_actual = ESTADO_GAMEOVER;
                            printf("GAME OVER - Te llenaste de pelotas\n");
                            guardar_puntaje(nombre_jugador, puntaje_total);
                        }
                    }

                }

                if(gbt_tecla_presionada(GBTK_p))
                {
                    estado_actual = ESTADO_PAUSA;
                    printf("PAUSA\n");
                }
                dibujar_marco_tablero(offset_x, offset_y, INDICE_GRIS_CL);
                dibujar_tablero(tablero_principal, offset_x, offset_y);
                dibujar_pieza(&pieza_activa, offset_x, offset_y);

                int velocidad_ms = (int)(tiempo_caida_actual * 1000);

                mi_gbt_dibujar_texto(offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, offset_y + 10, "SCORE", INDICE_GRIS_CL);

                char txt_score[12];
                sprintf(txt_score, "%d", puntaje_total);
                mi_gbt_dibujar_texto(offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, offset_y + 22, txt_score, INDICE_GRIS_CL);

                mi_gbt_dibujar_texto(offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, offset_y + 45, "VEL ms", INDICE_GRIS_CL);
                char txt_vel[12];
                sprintf(txt_vel, "%d", (int)(tiempo_caida_actual * 1000));
                mi_gbt_dibujar_texto(offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, offset_y + 57, txt_vel, INDICE_GRIS_CL);

                break;

            case ESTADO_PAUSA:
                // --- PANTALLA DE PAUSA ---
                // Dibujamos el juego de fondo (sin la gravedad, por eso está pausado)
                dibujar_marco_tablero(offset_x, offset_y, INDICE_GRIS_CL);
                dibujar_tablero(tablero_principal, offset_x, offset_y);
                dibujar_pieza(&pieza_activa, offset_x, offset_y);

                mi_gbt_dibujar_texto(offset_x + 25, offset_y + 80, "PAUSA", INDICE_GRIS_CL);

                if (gbt_tecla_presionada(GBTK_p)) {
                    estado_actual = ESTADO_JUGANDO;
                    printf("Reanudando\n");
                }
                break;

             case ESTADO_GAMEOVER:
                dibujar_marco_tablero(offset_x, offset_y, INDICE_GRIS_CL);
                dibujar_tablero(tablero_principal, offset_x, offset_y);

                printf("PULSÁ 'R' PARA REINICIAR LA PARTIDA\n");

                if (gbt_tecla_presionada(GBTK_r))
                {
                     // 1. Vaciamos el tablero completo
                     for(int f = 0; f < TABLERO_ALTO; f++) {
                         for(int c = 0; c < TABLERO_ANCHO; c++) {
                             tablero_principal[f][c] = INDICE_TRANSP;
                         }
                     }
                     // 2. Reseteamos las variables de juego
                     puntaje_total = 0;
                     contador_fichas = 0;
                     tiempo_caida_actual = 0.5;

                     // 3. Recreamos el temporizador con la velocidad inicial
                     gbt_temporizador_destruir(temp_gravedad);
                     temp_gravedad = gbt_temporizador_crear(tiempo_caida_actual);

                     // 4. Creamos una pieza limpia y volvemos al ruedo
                     crear_pieza(&pieza_activa);
                     estado_actual = ESTADO_JUGANDO;
                }
                break;
        }


        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(temp_gravedad);
    gbt_temporizador_destruir(temp_soft_drop);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}
