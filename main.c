// Apellido: Albarracin, José Emanuel
// DNI: 40393967
// Usuario: Jose23-A
// Entrega: Sí

#include "header.h"
//#include "matrices.h"

int main(int argc, char* argv[])
{
    t_datosJuego dt;
    t_sistema sys;
    t_jugador player;

    inicializarSistema(&sys, argc, argv);

    //cargar_configuracion(&sys);

    player.nombre[0] = '\0';
    player.largo_nombre = 0;

    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT.\n");
        return -1;
    }

    if (gbt_crear_ventana("Ficha Cayendo (Gravedad)", sys.res_ancho, sys.res_alto, sys.escala) != 0)
    {
        fprintf(stderr, "Error al crear la ventana.\n");
        gbt_cerrar(); // [cite: 1]
        return -1;
    }

    gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);

    inicializarDatosJuego(&dt, &sys);

    if (!dt.temp_gravedad)
    {
        fprintf(stderr, "Error al crear temporizador.\n");
        return -1;
    }


    if (!dt.temp_soft_drop)
    {
        fprintf(stderr, "Error al crear temporizador de soft drop.\n");
        return -1;
    }



    srand(time(NULL));


    printf("Programa iniciado. Ficha cayendo por gravedad.\n");
    printf("Pulse ESCAPE para salir.\n");



    /// --- Bucle Principal del Juego (Game Loop) ---
    while (sys.corriendo)
    {
        // A. Procesar Entrada (solo para salir con ESC)
        gbt_procesar_entrada();
        if (gbt_tecla_presionada(GBTK_ESCAPE))
        {
            sys.corriendo = 0;
            printf("Saliendo...\n");
        }

        sys.centro_x = sys.res_ancho / MITAD;
        sys.centro_y = sys.res_alto / MITAD;

        dt.offset_x = sys.centro_x - (dt.tablero_ancho_px / MITAD);
        dt.offset_y = sys.centro_y - (dt.tablero_alto_px / MITAD);

        gbt_borrar_backbuffer(INDICE_GRIS_CL);

        switch (sys.estado_actual)
        {
            case ESTADO_MENU:
                gbt_borrar_backbuffer(INDICE_GRIS_CL);
                dibujar_slamdunk(sys.centro_x - (SLAM_DUNK_WIDTH / MITAD), sys.centro_y - 90);

                switch (sys.sub_pantalla)
                {
                    case SUB_MENU_PPAL:
                        controlar_menu_principal(&sys);
                        dibujar_menu_principal(&sys);
                        break;

                    case SUB_NOMBRE:
                        controlar_ingreso_nombre(&sys, &player);
                        dibujar_ingreso_nombre(&sys, &player);
                        break;

                    case SUB_DECISION:
                        controlar_pantalla_decision(&sys, &dt, &player);
                        dibujar_pantalla_decision(&sys);
                        break;

                    case SUB_PUNTAJES:
                        controlar_pantalla_puntajes(&sys);
                        dibujar_pantalla_puntajes(&sys);
                        break;

                    case SUB_CONFIG:
                        controlar_pantalla_configuracion(&sys);
                        dibujar_pantalla_configuracion(&sys);
                        break;
                }
                break;

            case ESTADO_JUGANDO:

                gbt_borrar_backbuffer(INDICE_GRIS_CL);

                movimientosPiezas(&dt);

                actualizar_juego(&dt, &sys);

                if(gbt_tecla_presionada(GBTK_p))
                {
                    sys.estado_actual = ESTADO_PAUSA;
                    sys.opcion_pausa = 0;
                }
                dibujar_marco_tablero(dt.offset_x, dt.offset_y, INDICE_GRIS_CL);
                dibujar_tablero(dt.tablero_principal, dt.offset_x, dt.offset_y, sys.paleta_tipo);
                dibujar_pieza(&dt.pieza_activa, dt.offset_x, dt.offset_y, sys.paleta_tipo);

                dibujar_score(&dt);

                break;

            case ESTADO_PAUSA:

                gbt_borrar_backbuffer(INDICE_GRIS_CL);

                controlar_menu_pausa(&sys, &dt, &player);

                dibujar_marco_tablero(dt.offset_x, dt.offset_y, INDICE_GRIS_CL);
                //dibujar_tablero(dt.tablero_principal, dt.offset_x, dt.offset_y);
                //dibujar_pieza(&dt.pieza_activa, dt.offset_x, dt.offset_y);

                dibujar_menu_pausa(&sys);
                break;

             case ESTADO_GAMEOVER:
                gbt_borrar_backbuffer(INDICE_GRIS_CL);

                controlar_pantalla_gameover(&sys, &dt, &player);

                dibujar_pantalla_gameover(&sys);
                break;
        }


        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(dt.temp_gravedad);
    gbt_temporizador_destruir(dt.temp_soft_drop);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}
