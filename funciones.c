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

void dibujar_pieza(int16_t pos_columna, int16_t pos_fila, uint8_t pieza[][DIMENSION_Z], const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE])
{
    for (int fila = 0; fila < DIMENSION_Z; fila++)
    {
        for (int col = 0; col < DIMENSION_Z; col++)
        {

            uint8_t color = pieza[fila][col];

                int pixel_x = (pos_columna + col) * TAM_BLOQUE;
                int pixel_y = (pos_fila + fila) * TAM_BLOQUE;

                dibujar_cuadrado_base(pixel_x, pixel_y, textura);
        }
    }
}
