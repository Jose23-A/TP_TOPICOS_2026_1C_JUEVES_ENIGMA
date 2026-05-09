#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"

#define TABLERO_ANCHO 10
#define TABLERO_ALTO 20

#define GIRO_Q -1
#define GIRO_E 1

#define TAM_BLOQUE 8
#define COLOR_AMARILLO 14
#define DIMENSION_PIEZA 2
#define DIMENSION_Z 3
#define DIMENSION_GRAL 4

#define ANCHO_VENTANA (TABLERO_ANCHO * TAM_BLOQUE)
#define ALTO_VENTANA  (TABLERO_ALTO * TAM_BLOQUE)
#define ESCALA_VENTANA 3

#define INDICE_NEGRO    0
#define INDICE_AZUL_C   9  // Azul claro para la J
#define INDICE_VERDE_C  10 // Verde claro para la S
#define INDICE_CIAN     11 // Cian para el palo (I)
#define INDICE_ROJO_C   12 // Rojo claro para la Z
#define INDICE_MAGENTA  13 // Magenta para la T
#define INDICE_AMARILLO 14 // Amarillo para la O
#define INDICE_NARANJA  6  // Marron/Naranja para la L
#define INDICE_TRANSP   15

#define DIMENSION_GENERAL 4 // TODAS las piezas ahora viven en una matriz de 4x4
#define CANT_COLORES 16

// 1. Declaramos que la paleta existe y tiene 16 elementos
extern tGBT_ColorRGB paletaCGA[CANT_COLORES];

void dibujar_cuadrado_base(uint16_t, uint16_t, const uint8_t [][TAM_BLOQUE]);
void dibujar_pieza(int16_t f_col, int16_t f_fila, uint8_t[][DIMENSION_GENERAL], const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE]);
void fijar_pieza(int16_t pos_columna, int16_t pos_fila, uint8_t pieza[][DIMENSION_GENERAL], uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO]);
void dibujar_tablero(const uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO], const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE]);
void cargar_nueva_pieza(const uint8_t pieza_origen[DIMENSION_GENERAL][DIMENSION_GENERAL], uint8_t pieza_destino[DIMENSION_GENERAL][DIMENSION_GENERAL]);
void rotar_pieza(uint8_t pieza[DIMENSION_GENERAL][DIMENSION_GENERAL], int sentido);

#endif // HEADER_H_INCLUDED
