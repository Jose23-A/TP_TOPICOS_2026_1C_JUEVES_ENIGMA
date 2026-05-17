// Apellido: Albarracin, José Emanuel
// DNI: 40393967
// Usuario: Jose23-A
// Entrega: Sí

#include "header.h"
//#include "matrices.h"

int main(int argc, char* argv[])
{
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

    gbt_aplicar_paleta(paletaCGA, 16, GBT_FORMATO_888);

    // Temporizador para la gravedad: Cae cada 0.5 segundos
    tGBT_Temporizador* temp_gravedad = gbt_temporizador_crear(0.5);
    if (!temp_gravedad)
    {
        fprintf(stderr, "Error al crear temporizador.\n");
        return -1;
    }

    uint8_t tablero_principal[TABLERO_ALTO][TABLERO_ANCHO];

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

    int offset_x = (res_ancho - tablero_ancho_px) / 2;
    int offset_y = (res_alto - tablero_alto_px) / 2;

    t_tetromino pieza_activa;
    crear_pieza(&pieza_activa);

    /// --- Bucle Principal del Juego (Game Loop) ---
    while (corriendo)
    {
        // A. Procesar Entrada (solo para salir con ESC) [cite: 2]
        gbt_procesar_entrada();
        if (gbt_tecla_presionada(GBTK_ESCAPE))   // [cite: 2]
        {
            corriendo = 0;
            printf("Saliendo...\n");
        }
        // Dibujamos la pieza en sus coordenadas lógicas actuales
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
        // Actualización Lógica (La Gravedad)
        // Si se cumplió el tiempo (0.5s), movemos la pieza hacia abajo.
        if (gbt_temporizador_consumir(temp_gravedad))   // [cite: 5]
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

                // Pedimos una nueva pieza al azar para que aparezca arriba
                crear_pieza(&pieza_activa);
            }

        }

        gbt_borrar_backbuffer(INDICE_NEGRO); // Limpiar pantalla en negro
        dibujar_tablero(tablero_principal, offset_x, offset_y);
        dibujar_pieza(&pieza_activa, offset_x, offset_y);
        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_temporizador_destruir(temp_gravedad);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}
