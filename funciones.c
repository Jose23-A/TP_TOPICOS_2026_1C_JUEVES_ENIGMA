#include "header.h"

void dibujar_cuadrado_base(uint16_t x_pantalla, uint16_t y_pantalla, const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE])
{
    for (int y = 0; y < TAM_BLOQUE; y++)
    {
        for (int x = 0; x < TAM_BLOQUE; x++)
        {
            uint8_t color_pixel = textura[y][x];

                gbt_dibujar_pixel(x_pantalla + x, y_pantalla + y, color_pixel);
        }
    }
}

void dibujar_tablero(const uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO], const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE])
{
    for (int fila = 0; fila < TABLERO_ALTO; fila++)
    {
        for (int col = 0; col < TABLERO_ANCHO; col++)
        {
            if (tablero[fila][col] != INDICE_TRANSP)
            {
                int pixel_x = col * TAM_BLOQUE;
                int pixel_y = fila * TAM_BLOQUE;
                dibujar_cuadrado_base(pixel_x, pixel_y, textura);
            }
        }
    }
}

void dibujar_pieza(int16_t pos_columna, int16_t pos_fila, uint8_t pieza[][DIMENSION_GENERAL], const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE])
{
    for (int fila = 0; fila < DIMENSION_GENERAL; fila++)
    {
        for (int col = 0; col < DIMENSION_GENERAL; col++)
        {
            uint8_t color = pieza[fila][col];
            if (color != INDICE_TRANSP)
            {
                int pixel_x = (pos_columna + col) * TAM_BLOQUE;
                int pixel_y = (pos_fila + fila) * TAM_BLOQUE;
                dibujar_cuadrado_base(pixel_x, pixel_y, textura);
            }
        }
    }
}

void fijar_pieza(int16_t pos_columna, int16_t pos_fila, uint8_t pieza[][DIMENSION_GENERAL], uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO])
{
    for (int fila = 0; fila < DIMENSION_GENERAL; fila++)
    {
        for (int col = 0; col < DIMENSION_GENERAL; col++)
        {
            if (pieza[fila][col] != INDICE_TRANSP)
            {
                tablero[pos_fila + fila][pos_columna + col] = pieza[fila][col];
            }
        }
    }
}

void cargar_nueva_pieza(const uint8_t pieza_origen[DIMENSION_GENERAL][DIMENSION_GENERAL], uint8_t pieza_destino[DIMENSION_GENERAL][DIMENSION_GENERAL])
{
    for(int f = 0; f < DIMENSION_GENERAL; f++) {
        for(int c = 0; c < DIMENSION_GENERAL; c++) {
            pieza_destino[f][c] = pieza_origen[f][c];
        }
    }
}

void rotar_pieza(uint8_t pieza[DIMENSION_GENERAL][DIMENSION_GENERAL], int sentido)
{
    uint8_t temporal[DIMENSION_GENERAL][DIMENSION_GENERAL];

    // Primero, hacemos la rotación matemática y la guardamos en un temporal
    // para no pisar los datos de nuestra propia matriz mientras la leemos
    for (int f = 0; f < DIMENSION_GENERAL; f++)
    {
        for (int c = 0; c < DIMENSION_GENERAL; c++)
        {
            if (sentido == 1) {
                // Rotar a la DERECHA (Horario)
                temporal[c][DIMENSION_GENERAL - 1 - f] = pieza[f][c];
            } else {
                // Rotar a la IZQUIERDA (Antihorario)
                temporal[DIMENSION_GENERAL - 1 - c][f] = pieza[f][c];
            }
        }
    }

    // Finalmente, volcamos la matriz rotada de nuevo en nuestra pieza
    for (int f = 0; f < DIMENSION_GENERAL; f++)
    {
        for (int c = 0; c < DIMENSION_GENERAL; c++)
        {
            pieza[f][c] = temporal[f][c];
        }
    }
}
