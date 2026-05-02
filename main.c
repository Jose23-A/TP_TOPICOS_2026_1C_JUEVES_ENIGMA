// Apellido: Albarracin, José Emanuel
// DNI: 40393967
// Usuario: Jose23-A
// Entrega: Sí

#include "header.h"

const uint8_t pieza_cuadrada[DIMENSION_PIEZA][DIMENSION_PIEZA] =
{
    {INDICE_AMARILLO, INDICE_AMARILLO},
    {INDICE_AMARILLO, INDICE_AMARILLO}
};

const uint8_t pieza_z[DIMENSION_Z][DIMENSION_Z] =
{
    {INDICE_TRANSP, INDICE_MAGENTA, INDICE_TRANSP},
    {INDICE_TRANSP, INDICE_MAGENTA, INDICE_MAGENTA},
    {INDICE_TRANSP, INDICE_TRANSP, INDICE_MAGENTA}
};

const uint8_t textura_magenta[TAM_BLOQUE][TAM_BLOQUE] = {
    {7, 7, 7, 7, 7, 7, 7, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 5, 5, 5, 5, 5, 8, 0},
    {7, 8, 8, 8, 8, 8, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

tGBT_ColorRGB paletaCGA[CANT_COLORES] =
{

    /// 0-15: Colores CGA (16 colores)
    {0x00, 0x00, 0x00}, // 0:   Negro
    {0x00, 0x00, 0xAA}, // 1:   Azul
    {0x00, 0xAA, 0x00}, // 2:   Verde
    {0x00, 0xAA, 0xAA}, // 3:   Cian
    {0xAA, 0x00, 0x00}, // 4:   Rojo
    {0xAA, 0x00, 0xAA}, // 5:   Magenta
    {0xAA, 0x55, 0x00}, // 6:   Marron
    {0xAA, 0xAA, 0xAA}, // 7:   Gris claro
    {0x55, 0x55, 0x55}, // 8:   Gris oscuro
    {0x55, 0x55, 0xFF}, // 9:   Azul brillante
    {0x55, 0xFF, 0x55}, // 10:  Verde brillante
    {0x55, 0xFF, 0xFF}, // 11:  Cian brillante
    {0xFF, 0x55, 0x55}, // 12:  Rojo brillante
    {0xFF, 0x55, 0xFF}, // 13:  Magenta brillante
    {0xFF, 0xFF, 0x55}, // 14:  Amarillo
    {0xFF, 0xFF, 0xFF}  // 15:  Usado como transparente por GBT
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

    // --- Estado de la Ficha ---
    // Posición lógica inicial (en la grilla de 10x20)
    int16_t ficha_columna = TABLERO_ANCHO / 2 - 1; // Centrada horizontalmente
    int16_t ficha_fila = 0;                        // Arriba del todo

    int corriendo = 1;
    printf("Programa iniciado. Ficha cayendo por gravedad.\n");
    printf("Pulse ESCAPE para salir.\n");

    // --- Bucle Principal del Juego (Game Loop) [cite: 1] ---
    while (corriendo)
    {
        // A. Procesar Entrada (solo para salir con ESC) [cite: 2]
        gbt_procesar_entrada();
        if (gbt_tecla_presionada(GBTK_ESCAPE))   // [cite: 2]
        {
            corriendo = 0;
            printf("Saliendo...\n");
        }

        // B. Actualización Lógica (La Gravedad) [cite: 5]
        // Si se cumplió el tiempo (0.5s), movemos la pieza hacia abajo.
        if (gbt_temporizador_consumir(temp_gravedad))   // [cite: 5]
        {

            // Verificamos si tocamos el fondo lógico.
            // (La pieza mide 2 bloques de alto, el tablero mide TABLERO_ALTO)
            if (ficha_fila < TABLERO_ALTO - 2)
            {
                ficha_fila++; // Cae una fila lógicamente
                printf("Ficha cayó a la fila: %d\n", ficha_fila);
            }
            else
            {
                // Se detiene al llegar al fondo en este ejemplo simple.
                // Resetear pieza arriba para demo continua: ficha_fila = 0;
            }
        }

        // C. Dibujado (Renderizado) [cite: 3]
        gbt_borrar_backbuffer(INDICE_NEGRO); // Limpiar pantalla en negro [cite: 3]

        // Dibujamos la pieza en sus coordenadas lógicas actuales
        dibujar_pieza(ficha_columna, ficha_fila, pieza_z, textura_magenta);

        gbt_volcar_backbuffer(); // Mostrar el frame dibujado [cite: 3]

        // D. Control de Fotogramas (FPS) [cite: 1]
        // Mantenemos el bucle a una velocidad constante (~60 FPS) [cite: 1].
        gbt_esperar(16);
    }

    // 4. Limpieza final [cite: 5, 3, 1]
    gbt_temporizador_destruir(temp_gravedad);
    gbt_destruir_ventana();
    gbt_cerrar();

    return 0;
}
