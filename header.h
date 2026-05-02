#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"

#define TABLERO_ANCHO 10
#define TABLERO_ALTO 20

#define TAM_BLOQUE 8
#define COLOR_AMARILLO 14
#define DIMENSION_PIEZA 2
#define DIMENSION_Z 3

#define ANCHO_VENTANA (TABLERO_ANCHO * TAM_BLOQUE)
#define ALTO_VENTANA  (TABLERO_ALTO * TAM_BLOQUE)
#define ESCALA_VENTANA 3

#define INDICE_NEGRO    0
#define INDICE_MAGENTA  5
#define INDICE_AMARILLO 14
#define INDICE_TRANSP 15
#define CANT_COLORES 16

// 1. Declaramos que la paleta existe y tiene 16 elementos
extern tGBT_ColorRGB paletaCGA[CANT_COLORES];

void dibujar_cuadrado_base(uint16_t, uint16_t, const uint8_t [][TAM_BLOQUE]);
void dibujar_pieza(int16_t f_col, int16_t f_fila, uint8_t[][DIMENSION_Z], const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE]);

#endif // HEADER_H_INCLUDED
