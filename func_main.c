#include "header.h"

void inicializarSistema(t_sistema *sys, int argc, char* argv[])
{
    sys->estado_actual = ESTADO_MENU;
    sys->opcion_menu = 0;
    sys->sub_pantalla = 0;

    sys->res_ancho = RES_ANCHO_CGA;
    sys->res_alto = RES_ALTO_CGA;
    sys->escala = ESCALA_VENTANA;

    cargar_configuracion(sys);

    if (argc >= MIN_ARG)
    {
        if (strcmp(argv[POS_RES], "VGA") == 0)
        {
            sys->res_ancho = RES_ANCHO_VGA;
            sys->res_alto = RES_ALTO_VGA;
        }
        else if (strcmp(argv[POS_RES], "CGA") == 0)
        {
            sys->res_ancho = RES_ANCHO_CGA;
            sys->res_alto = RES_ALTO_CGA;
        }
        sys->escala = atoi(argv[POS_ESCALA]);
    }

    sys->centro_x = sys->res_ancho / MITAD;
    sys->centro_y = sys->res_alto / MITAD;

    sys->corriendo = 1;

}

void inicializarDatosJuego(t_datosJuego *dt, t_sistema *sys)
{
    inicializarTablero(dt);
    dt->contador_fichas = 0;
    dt->tiempo_caida_actual = 0.5;
    dt->en_fijacion = 0;

    dt->temp_gravedad = gbt_temporizador_crear(dt->tiempo_caida_actual);
    dt->temp_soft_drop = gbt_temporizador_crear(dt->tiempo_caida_actual/10);
    dt->temp_fijacion = gbt_temporizador_crear(dt->tiempo_caida_actual * 0.5f);

    dt->puntaje_total = 0;

    dt->tablero_ancho_px = TABLERO_ANCHO * TAM_BLOQUE;
    dt->tablero_alto_px = (TABLERO_ALTO - FILAS_INVISIBLES) * TAM_BLOQUE;

    dt->offset_x = (sys->res_ancho - dt->tablero_ancho_px) / MITAD;
    dt->offset_y = (sys->res_alto - dt->tablero_alto_px) / MITAD;

    crear_pieza(&dt->pieza_activa);
}

void inicializarTablero(t_datosJuego *dt)
{
    for(int f = 0; f < TABLERO_ALTO; f++)
    {
        for(int c = 0; c < TABLERO_ANCHO; c++)
        {
            dt->tablero_principal[f][c] = INDICE_TRANSP;
        }
    }
}

void controlar_menu_principal(t_sistema *sys)
{
    if (gbt_tecla_presionada(GBTK_ABAJO))
    {
        sys->opcion_menu++;
        if (sys->opcion_menu > 3) sys->opcion_menu = 0;
    }
    if (gbt_tecla_presionada(GBTK_ARRIBA))
    {
        sys->opcion_menu--;
        if (sys->opcion_menu < 0) sys->opcion_menu = 3;
    }
    if (gbt_tecla_presionada(GBTK_ENTER))
    {
        if (sys->opcion_menu == 0) // PLAY GAME
        {
            if (existe_partida()) sys->sub_pantalla = SUB_DECISION;
            else sys->sub_pantalla = SUB_NOMBRE;
        }
        else if (sys->opcion_menu == 1) sys->sub_pantalla = SUB_PUNTAJES;
        else if (sys->opcion_menu == 2) sys->sub_pantalla = SUB_CONFIG;
        else if (sys->opcion_menu == 3) sys->corriendo = 0;
    }
}

void dibujar_menu_principal(const t_sistema *sys)
{
    mi_gbt_dibujar_texto(sys->centro_x - 40, sys->centro_y + 30, "PLAY GAME", INDICE_NEGRO);
    mi_gbt_dibujar_texto(sys->centro_x - 40, sys->centro_y + 45, "PUNTAJES", INDICE_NEGRO);
    mi_gbt_dibujar_texto(sys->centro_x - 40, sys->centro_y + 60, "CONFIGURACION", INDICE_NEGRO);
    mi_gbt_dibujar_texto(sys->centro_x - 40, sys->centro_y + 75, "SALIR", INDICE_NEGRO);

    // El selector usa el dato guardado en el struct
    int y_selector = (sys->centro_y + 30) + (sys->opcion_menu * 15);
    mi_gbt_dibujar_texto(sys->centro_x - 55, y_selector, "X", INDICE_NEGRO);
}

// Verifica si hay una partida guardada sin cargarla
int existe_partida()
{
    FILE *fp = fopen("partida.dat", "rb");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}

// Lógica de control para la nueva pantalla
void controlar_pantalla_decision(t_sistema *sys, t_datosJuego *dt, t_jugador *player)
{
    if (gbt_tecla_presionada(GBTK_ABAJO) || gbt_tecla_presionada(GBTK_ARRIBA))
        sys->opcion_decision = !sys->opcion_decision;

    if (gbt_tecla_presionada(GBTK_ENTER))
    {
        if (sys->opcion_decision == 0) // CONTINUAR
        {
            cargar_partida(dt); // Carga explícita

            strcpy(player->nombre, dt->nombre_jugador);
            player->largo_nombre = strlen(player->nombre);

            sys->estado_actual = ESTADO_JUGANDO;
        }
        else // NUEVA PARTIDA
        {
            remove("partida.dat"); // Borramos la vieja
            inicializarDatosJuego(dt, sys); // Arranca de cero

            player->nombre[0] = '\0';
            player->largo_nombre = 0;

            sys->sub_pantalla = 1; // Vamos al ingreso de nombre
            sys->estado_actual = ESTADO_MENU;
        }
    }
}

// Dibujado de la nueva pantalla
void dibujar_pantalla_decision(const t_sistema *sys)
{
    mi_gbt_dibujar_texto(sys->centro_x - 100, sys->centro_y + 30, "PARTIDA GUARDADA DETECTADA", INDICE_NEGRO);
    mi_gbt_dibujar_texto(sys->centro_x - 54, sys->centro_y + 55, "CONTINUAR", INDICE_NEGRO);
    mi_gbt_dibujar_texto(sys->centro_x - 58, sys->centro_y + 70, "NUEVA PARTIDA", INDICE_NEGRO);

    int pos_x = (sys->opcion_decision == 0) ? sys->centro_x - 70 : sys->centro_x - 73;
    int pos_y = (sys->opcion_decision == 0) ? sys->centro_y + 55 : sys->centro_y + 70;
    mi_gbt_dibujar_texto(pos_x, pos_y, "X", INDICE_NEGRO);
}

void controlar_ingreso_nombre(t_sistema *sys, t_jugador *player)
{
    if (player->largo_nombre < 10)
    {
        for (int i = 0; i < 26; i++)
        {
            if (gbt_tecla_presionada(GBTK_a + i))
            {
                player->nombre[player->largo_nombre] = 'A' + i;
                player->largo_nombre++;
                player->nombre[player->largo_nombre] = '\0';
                break;
            }
        }
    }

    if (gbt_tecla_presionada(GBTK_RETROCESO) && player->largo_nombre > 0)
    {
        player->largo_nombre--;
        player->nombre[player->largo_nombre] = '\0';
    }

    if (gbt_tecla_presionada(GBTK_ENTER) && player->largo_nombre > 0)
    {
        sys->estado_actual = ESTADO_JUGANDO;
    }
}

void dibujar_ingreso_nombre(t_sistema *sys, t_jugador *player)
{
    mi_gbt_dibujar_texto((sys->res_ancho - 153) / MITAD, sys->centro_y + 25, "INGRESE SU NOMBRE", INDICE_NEGRO);

    if (player->largo_nombre == 0)
    {
        mi_gbt_dibujar_texto((sys->res_ancho - 72) / MITAD, sys->centro_y + 40, "________", INDICE_NEGRO);
    }
    else
    {
        int ancho_nombre_px = player->largo_nombre * 9;
        mi_gbt_dibujar_texto((sys->res_ancho - ancho_nombre_px) / MITAD, sys->centro_y + 40, player->nombre, INDICE_NEGRO);
    }
    mi_gbt_dibujar_texto((sys->res_ancho - 225) / MITAD, sys->centro_y + 65, "PRESIONE ENTER PARA EMPEZAR", INDICE_NEGRO);
}

void controlar_pantalla_puntajes(t_sistema *sys)
{
    if (gbt_tecla_presionada(GBTK_m))
    {
        sys->sub_pantalla = 0;
    }
}

void dibujar_pantalla_puntajes(t_sistema *sys)
{
    // El título arranca justo debajo del logo
    mi_gbt_dibujar_texto(sys->centro_x - 54, sys->centro_y + 20, "TOP 5 SCORES", INDICE_NEGRO);

    FILE *fp = fopen("puntaje.dat", "rb");
    t_jugador j_aux;
    int puesto = 1;
    int offset_y = 40; // Empezamos a listar más abajo

    if (fp != NULL)
    {
        while (puesto <= 3 && fread(&j_aux, sizeof(t_jugador), 1, fp) == 1)
        {
            char txt_puesto[5];
            char txt_score[15];

            sprintf(txt_puesto, "%d", puesto);
            sprintf(txt_score, "%d", j_aux.puntaje);

            // Columna Izquierda: El número de puesto
            mi_gbt_dibujar_texto(sys->centro_x - 95, sys->centro_y + offset_y, txt_puesto, INDICE_NEGRO);
            // Columna Central: El Nombre
            mi_gbt_dibujar_texto(sys->centro_x - 65, sys->centro_y + offset_y, j_aux.nombre, INDICE_NEGRO);
            // Columna Derecha: El Puntaje (ALINEADO A LA DERECHA)
            int ancho_score = strlen(txt_score) * 9;
            mi_gbt_dibujar_texto((sys->centro_x + 65) - ancho_score, sys->centro_y + offset_y, txt_score, INDICE_NEGRO);

            offset_y += 15;
            puesto++;
        }
        fclose(fp);
    }

    // Cartel si el archivo recién se borró o no hay nadie
    if (puesto == 1)
    {
        mi_gbt_dibujar_texto(sys->centro_x - 58, sys->centro_y + 50, "SIN REGISTROS", INDICE_NEGRO);
    }

    // El botón de volver queda al fondo, visible en CGA y VGA
    mi_gbt_dibujar_texto(sys->centro_x - 94, sys->centro_y + 85, "PULSE 'M' PARA VOLVER", INDICE_NEGRO);
}

void controlar_pantalla_configuracion(t_sistema *sys)
{
    if (gbt_tecla_presionada(GBTK_ABAJO))
    {
        sys->opcion_config++;
        if (sys->opcion_config > 3) sys->opcion_config = 0; // 0:Paleta, 1:Res, 2:Vel, 3:Guardar
    }
    if (gbt_tecla_presionada(GBTK_ARRIBA))
    {
        sys->opcion_config--;
        if (sys->opcion_config < 0) sys->opcion_config = 3;
    }

    // Modificar valores con las flechas laterales
    if (gbt_tecla_presionada(GBTK_DERECHA) || gbt_tecla_presionada(GBTK_IZQUIERDA))
    {
        switch (sys->opcion_config)
        {
            case 0: // Alternar Paleta
                sys->paleta_tipo = (sys->paleta_tipo == 0) ? 1 : 0;
                if (sys->paleta_tipo == 0)
                    gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);
                else
                    gbt_aplicar_paleta(paletaBlancoYNegro, CANT_COLORES, GBT_FORMATO_888);
                break;

            case 1: // Alternar Resolución (CGA / VGA)
                if (sys->res_ancho == RES_ANCHO_CGA) {
                    sys->res_ancho = RES_ANCHO_VGA;
                    sys->res_alto = RES_ALTO_VGA;
                } else {
                    sys->res_ancho = RES_ANCHO_CGA;
                    sys->res_alto = RES_ALTO_CGA;
                }

                sys->centro_x = sys->res_ancho / MITAD;
                sys->centro_y = sys->res_alto / MITAD;

                // Recreamos la ventana en vivo para aplicar la resolución
                gbt_destruir_ventana();
                gbt_crear_ventana("Ficha Cayendo", sys->res_ancho, sys->res_alto, sys->escala);
                if (sys->paleta_tipo == 0)
                    gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);
                else
                    gbt_aplicar_paleta(paletaBlancoYNegro, CANT_COLORES, GBT_FORMATO_888);
                break;

            case 2: // Alternar Velocidad Base de caída
                if (sys->velocidad_base == 0.5f)      sys->velocidad_base = 0.3f; // Rápido
                else if (sys->velocidad_base == 0.3f) sys->velocidad_base = 0.7f; // Lento
                else                                  sys->velocidad_base = 0.5f; // Normal
                break;
        }
    }

    if (gbt_tecla_presionada(GBTK_ENTER) && sys->opcion_config == 3)
    {
        guardar_configuracion(sys);
        sys->sub_pantalla = 0; // Regresa al menú principal
    }
}

void guardar_configuracion(t_sistema *sys)
{
    FILE *fp = fopen("config.dat", "wb");
    if (fp == NULL)
        return;

    fwrite(&(sys->res_ancho), sizeof(int), 1, fp);
    fwrite(&(sys->res_alto), sizeof(int), 1, fp);
    fwrite(&(sys->escala), sizeof(int), 1, fp);
    fwrite(&(sys->paleta_tipo), sizeof(int), 1, fp);
    fwrite(&(sys->velocidad_base), sizeof(float), 1, fp);
    fclose(fp);
}

void cargar_configuracion(t_sistema *sys)
{
    FILE *fp = fopen("config.dat", "rb");
    if (fp == NULL)
        return;

    fread(&(sys->res_ancho), sizeof(int), 1, fp);
    fread(&(sys->res_alto), sizeof(int), 1, fp);
    fread(&(sys->escala), sizeof(int), 1, fp);
    fread(&(sys->paleta_tipo), sizeof(int), 1, fp);
    fread(&(sys->velocidad_base), sizeof(float), 1, fp);
    fclose(fp);
}

void dibujar_pantalla_configuracion(t_sistema *sys)
{
    int pos_res_x = 160;

    mi_gbt_dibujar_texto((sys->res_ancho - 117) / MITAD, sys->centro_y + 18, "CONFIGURACION", INDICE_NEGRO);

    // Textos de Opciones
    mi_gbt_dibujar_texto(40, sys->centro_y + 35, "PALETA:", INDICE_NEGRO);
    mi_gbt_dibujar_texto(pos_res_x, sys->centro_y + 35, (sys->paleta_tipo == 0) ? "SLAM DUNK" : "RETRO", INDICE_NEGRO);

    mi_gbt_dibujar_texto(40, sys->centro_y + 50, "RESOLUCION:", INDICE_NEGRO);
    mi_gbt_dibujar_texto(pos_res_x, sys->centro_y + 50, (sys->res_ancho == RES_ANCHO_VGA) ? "VGA" : "CGA", INDICE_NEGRO);

    mi_gbt_dibujar_texto(40, sys->centro_y + 65, "VEL. INICIAL:", INDICE_NEGRO);
    if (sys->velocidad_base == 0.3f)      mi_gbt_dibujar_texto(pos_res_x, sys->centro_y + 65, "RAPIDO", INDICE_NEGRO);
    else if (sys->velocidad_base == 0.7f) mi_gbt_dibujar_texto(pos_res_x, sys->centro_y + 65, "LENTO", INDICE_NEGRO);
    else                                  mi_gbt_dibujar_texto(pos_res_x, sys->centro_y + 65, "NORMAL", INDICE_NEGRO);

    mi_gbt_dibujar_texto((sys->res_ancho - 144) / MITAD, sys->centro_y + 85, "GUARDAR CONFIG", INDICE_NEGRO);

    // Posicionador del Selector X
    int y_selector = (sys->centro_y + 35) + (sys->opcion_config * 15);
    if (sys->opcion_config != 3)
        mi_gbt_dibujar_texto(25, y_selector, "X", INDICE_NEGRO);
    else
        mi_gbt_dibujar_texto(230, y_selector + 5, "X", INDICE_NEGRO);
}

void controlar_menu_pausa(t_sistema *sys, t_datosJuego *dt, t_jugador *player)
{
    if (gbt_tecla_presionada(GBTK_ABAJO) || gbt_tecla_presionada(GBTK_ARRIBA))
    {
        sys->opcion_pausa = !sys->opcion_pausa;
    }

    if (gbt_tecla_presionada(GBTK_ENTER))
    {
        if (sys->opcion_pausa == 0)
        {
            sys->pausado = 0;
            sys->estado_actual = ESTADO_JUGANDO;
        }
        else
        {
            player->puntaje = dt->puntaje_total;
            guardar_puntaje(player);

            strcpy(dt->nombre_jugador, player->nombre);
            guardar_partida(dt);

            sys->pausado = 0;
            sys->estado_actual = ESTADO_MENU;
            sys->sub_pantalla = 0;
        }
    }
}

void dibujar_menu_pausa(const t_sistema *sys)
{
    mi_gbt_dibujar_texto_16(sys->centro_x - 22, sys->centro_y - 10, "PAUSA", INDICE_NEGRO);

    mi_gbt_dibujar_texto(sys->centro_x - 27, sys->centro_y + 15, "VOLVER", INDICE_NEGRO);
    mi_gbt_dibujar_texto(sys->centro_x - 22, sys->centro_y + 30, "SALIR", INDICE_NEGRO);

    int posicion_x, posicion_y;
    if (sys->opcion_pausa == 0) // VOLVER
    {
        posicion_x = (sys->centro_x - 27) - 15;
        posicion_y = sys->centro_y + 15;
    }
    else // SALIR
    {
        posicion_x = (sys->centro_x - 22) - 15;
        posicion_y = sys->centro_y + 30;
    }

    mi_gbt_dibujar_texto(posicion_x, posicion_y, "X", INDICE_NEGRO);
}

void controlar_pantalla_gameover(t_sistema *sys, t_datosJuego *dt, t_jugador *player)
{
    if (gbt_tecla_presionada(GBTK_ABAJO) || gbt_tecla_presionada(GBTK_ARRIBA))
    {
        sys->opcion_gameover = !sys->opcion_gameover;
    }

    if (gbt_tecla_presionada(GBTK_ENTER))
    {
        // 1. Ahora sí lee el puntaje real de la partida que acaba de terminar
        player->puntaje = dt->puntaje_total;
        guardar_puntaje(player);

        if (sys->opcion_gameover == 0) // VOLVER A JUGAR
        {
            inicializarDatosJuego(dt, sys);
            sys->estado_actual = ESTADO_JUGANDO;
        }
        else // SALIR AL MENU
        {
            // 2. Limpiamos el juego viejo para que la próxima partida nazca de cero
            inicializarDatosJuego(dt, sys);
            sys->estado_actual = ESTADO_MENU;
            sys->sub_pantalla = 0;
        }
    }
}

void dibujar_pantalla_gameover(const t_sistema *sys)
{
    dibujar_slamdunk((sys->res_ancho - SLAM_DUNK_WIDTH) / MITAD, sys->centro_y - 90);
    mi_gbt_dibujar_texto_16(sys->centro_x - 40, sys->centro_y + 20, "GAME OVER", INDICE_NEGRO);

    mi_gbt_dibujar_texto(sys->centro_x - 63, sys->centro_y + 45, "VOLVER A JUGAR", INDICE_NEGRO);
    mi_gbt_dibujar_texto(sys->centro_x - 58, sys->centro_y + 60, "SALIR AL MENU", INDICE_NEGRO);

    int posicion_x, posicion_y;
    if (sys->opcion_gameover == 0) // VOLVER A JUGAR
    {
        posicion_x = (sys->centro_x - 63) - 15;
        posicion_y = sys->centro_y + 45;
    }
    else // SALIR AL MENU
    {
        posicion_x = (sys->centro_x - 58) - 15;
        posicion_y = sys->centro_y + 60;
    }

    mi_gbt_dibujar_texto(posicion_x, posicion_y, "X", INDICE_NEGRO);
}

void movimientosPiezas(t_datosJuego *dt)
{
    int movio=NO_MOVIO;
    if(gbt_tecla_presionada(GBTK_IZQUIERDA) && !choca_izquierda(&(dt->pieza_activa), dt->tablero_principal))
    {
        dt->pieza_activa.x--;
        movio = MOVIO;
    }
    if(gbt_tecla_presionada(GBTK_DERECHA) && !choca_derecha(&(dt->pieza_activa), dt->tablero_principal))
    {
        dt->pieza_activa.x++;
        movio = MOVIO;
    }
    if(gbt_tecla_presionada(GBTK_q))
    {
        rotar_pieza(&(dt->pieza_activa), GIRO_Q, dt->tablero_principal); // Izquierda
        movio = MOVIO;
    }
    if(gbt_tecla_presionada(GBTK_e))
    {
        rotar_pieza(&(dt->pieza_activa), GIRO_E, dt->tablero_principal);  // Derecha
        movio = MOVIO;
    }

    if (movio == MOVIO && dt->en_fijacion == 1)
    {
        gbt_temporizador_destruir(dt->temp_fijacion);
        dt->temp_fijacion = gbt_temporizador_crear(dt->tiempo_caida_actual * 0.5f);
    }

    if(gbt_tecla_sostenida(GBTK_ABAJO))
    {
        if (gbt_temporizador_consumir(dt->temp_soft_drop))
        {
            if (choca_abajo(&(dt->pieza_activa), dt->tablero_principal) == 0)
            {
                dt->pieza_activa.y++;
                dt->puntaje_total += 1;
            }
        }
    }
}

void actualizar_juego(t_datosJuego *dt, t_sistema *sys)
{
    if (choca_abajo(&(dt->pieza_activa), dt->tablero_principal))
    {
        // Si es el primer frame que toca el piso, arrancamos el reloj limpio
        if (dt->en_fijacion == 0)
        {
            dt->en_fijacion = 1;
            gbt_temporizador_destruir(dt->temp_fijacion);
            dt->temp_fijacion = gbt_temporizador_crear(dt->tiempo_caida_actual * 0.5f);
        }
        else
        {
            // Si ya estaba tocando el piso, esperamos que pase el tiempo
            if (gbt_temporizador_consumir(dt->temp_fijacion))
            {
                fijar_pieza(&(dt->pieza_activa), dt->tablero_principal);
                dt->en_fijacion = 0; // Se pegó, apagamos la bandera para la próxima pieza

                int cant_lineas = limpiar_lineas(dt->tablero_principal);
                if (cant_lineas > 0)
                {
                    int nivel_actual = (dt->contador_fichas / 10) + 1;
                    int puntos_base = 0;

                    if (cant_lineas == 1) dt->puntaje_total += 100;
                    else if (cant_lineas == 2) dt->puntaje_total += 300;
                    else if (cant_lineas == 3) dt->puntaje_total += 500;
                    else if (cant_lineas == 4) dt->puntaje_total += 800;
                    printf("¡SLAM DUNK! Borraste %d linea(s). Puntaje: %d\n", cant_lineas, dt->puntaje_total);

                    dt->puntaje_total += (puntos_base * nivel_actual);
                }

                dt->contador_fichas++;
                if (dt->contador_fichas % 10 == 0)
                {
                    dt->tiempo_caida_actual = dt->tiempo_caida_actual * 0.97f;
                    gbt_temporizador_destruir(dt->temp_gravedad);
                    dt->temp_gravedad = gbt_temporizador_crear(dt->tiempo_caida_actual);
                }

                crear_pieza(&(dt->pieza_activa));

                if (es_posicion_valida(&(dt->pieza_activa), dt->tablero_principal) == 0)
                {
                    sys->estado_actual = ESTADO_GAMEOVER;
                    printf("GAME OVER - Te llenaste de pelotas\n");
                }
            }
        }
    }
    else
    {
        // Está en el aire. Apagamos la bandera y consumimos la gravedad normal.
        dt->en_fijacion = 0;
        gbt_temporizador_consumir(dt->temp_fijacion);

        if (gbt_temporizador_consumir(dt->temp_gravedad))
        {
            dt->pieza_activa.y++;
        }
    }
}

void guardar_partida(const t_datosJuego *dt)
{
    FILE *fp = fopen("partida.dat", "wb");
    if (fp != NULL)
    {
        // Guardamos todo el bloque de datos de la partida
        fwrite(dt, sizeof(t_datosJuego), 1, fp);
        fclose(fp);
        printf("Partida guardada exitosamente.\n");
    }
}

int cargar_partida(t_datosJuego *dt)
{
    FILE *fp = fopen("partida.dat", "rb");
    if (fp == NULL)
    {
        return 0; // No hay ninguna partida guardada previamente
    }

    fread(dt, sizeof(t_datosJuego), 1, fp);
    fclose(fp);

    // ¡CRÍTICO! Los punteros guardados apuntan a basura. Recreamos los timers en la nueva memoria:
    dt->temp_gravedad = gbt_temporizador_crear(dt->tiempo_caida_actual);
    dt->temp_soft_drop = gbt_temporizador_crear(dt->tiempo_caida_actual / 10.0f);
    dt->temp_fijacion = gbt_temporizador_crear(dt->tiempo_caida_actual * 0.5f);

    // Borramos el archivo una vez usado para que no se pueda restaurar infinitamente
    remove("partida.dat");

    return 1; // Carga exitosa
}

void dibujar_score(t_datosJuego *dt)
{

    mi_gbt_dibujar_texto_16(dt->offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, dt->offset_y + 10, "SCORE", INDICE_NEGRO);

    char txt_score[12];
    sprintf(txt_score, "%d", dt->puntaje_total);
    mi_gbt_dibujar_texto_16(dt->offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, dt->offset_y + 22, txt_score, INDICE_NEGRO);

    mi_gbt_dibujar_texto(dt->offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, dt->offset_y + 45, "VEL ms", INDICE_NEGRO);
    char txt_vel[12];
    sprintf(txt_vel, "%d", (int)(dt->tiempo_caida_actual * 1000));
    mi_gbt_dibujar_texto_16(dt->offset_x + (TABLERO_ANCHO * TAM_BLOQUE) + 15, dt->offset_y + 57, txt_vel, INDICE_NEGRO);
}
