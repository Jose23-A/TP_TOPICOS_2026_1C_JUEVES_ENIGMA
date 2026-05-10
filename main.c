// Apellido: Albarracin, José Emanuel
// DNI: 40393967
// Usuario: Jose23-A
// Entrega: Sí

#include "header.h"
//#include "matrices.h"

const uint8_t textura_magenta[TAM_BLOQUE][TAM_BLOQUE] =
{
    {7, 7, 7, 7, 7, 7, 7, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 8, 8, 8, 8, 8, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

int main(int argc, char* argv[])
{

    // 1. Inicializar la biblioteca [cite: 1]
    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT.\n");
        return -1;
    }

    // 2. Crear la ventana con el tamaño lógico calculado [cite: 3]
    if (gbt_crear_ventana("Ficha Cayendo (Gravedad)", ANCHO_VENTANA, ALTO_VENTANA, ESCALA_VENTANA) != 0)
    {
        fprintf(stderr, "Error al crear la ventana.\n");
        gbt_cerrar(); // [cite: 1]
        return -1;
    }

    gbt_aplicar_paleta(paletaCGA, 16, GBT_FORMATO_888); // [cite: 3, 9]

    // 3. Temporizador para la gravedad: Cae cada 0.5 segundos [cite: 5]
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

    // Posición lógica inicial (ajustada para el ancho 4)
    int16_t ficha_columna = (TABLERO_ANCHO / 2) - 2;
    int16_t ficha_fila = 0;

    int corriendo = 1;
    printf("Programa iniciado. Ficha cayendo por gravedad.\n");
    printf("Pulse ESCAPE para salir.\n");

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
        if(gbt_tecla_presionada(GBTK_IZQUIERDA) && pieza_activa.x > 0)
        {
            pieza_activa.x--;
        }
        if(gbt_tecla_presionada(GBTK_DERECHA) && pieza_activa.x < 8)
        {
            pieza_activa.x++;
        }
        if(gbt_tecla_presionada(GBTK_q))
        {
            printf("presionaste Q");
            rotar_pieza(&pieza_activa, GIRO_Q); // Izquierda
        }
        if(gbt_tecla_presionada(GBTK_e))
        {
            printf("presionaste E");
            rotar_pieza(&pieza_activa, GIRO_E);  // Derecha
        }
        // B. Actualización Lógica (La Gravedad) [cite: 5]
        // Si se cumplió el tiempo (0.5s), movemos la pieza hacia abajo.
        if (gbt_temporizador_consumir(temp_gravedad))   // [cite: 5]
        {
/*
            // Verificamos si tocamos el fondo lógico.
            // (La pieza mide 2 bloques de alto, el tablero mide TABLERO_ALTO)
            if (pieza_activa.y < TABLERO_ALTO - pieza_activa.dimension)
                pieza_activa.y++;
            else
            {
                fijar_pieza(&pieza_activa, tablero_principal);
                crear_pieza(&pieza_activa);
            }
*/
            if (choca_abajo(&pieza_activa, tablero_principal) == 0)
            {
                // CAMINO LIBRE: La pieza baja una fila en el mundo lógico
                pieza_activa.y++;
            }
            else
            {
                // BLOQUEADO: La pieza tocó el fondo o algo más
                // 1. La dejamos "pintada" permanentemente en el tablero
                fijar_pieza(&pieza_activa, tablero_principal);

                // 2. Pedimos una nueva pieza al azar para que aparezca arriba
                crear_pieza(&pieza_activa);
            }

        }

        gbt_borrar_backbuffer(INDICE_NEGRO); // Limpiar pantalla en negro
        dibujar_tablero(tablero_principal, textura_magenta);
        dibujar_pieza(&pieza_activa, textura_magenta);
        gbt_volcar_backbuffer(); // Mostrar el frame dibujado
        gbt_esperar(16);
    }

    // Limpieza final
    gbt_temporizador_destruir(temp_gravedad);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}
