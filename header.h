#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GBT/gbt.h"

#define TABLERO_ANCHO 10
#define TABLERO_ALTO 20
#define CENTRO_TABLERO (TABLERO_ANCHO / 2) - 2

#define GIRO_Q -1
#define GIRO_E 1

#define TAM_BLOQUE 8
#define DIMENSION_PIEZA 4

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

#define CANT_COLORES 16

typedef struct {
    uint8_t matriz[DIMENSION_PIEZA][DIMENSION_PIEZA];
    int dimension;
    int x;
    int y;
    uint8_t color;
} t_tetromino;

// 1. Declaramos que la paleta existe y tiene 16 elementos
extern tGBT_ColorRGB paletaCGA[CANT_COLORES];

void dibujar_cuadrado_base(uint16_t, uint16_t, const uint8_t [][TAM_BLOQUE]);
void dibujar_pieza(t_tetromino *p, const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE]);
void fijar_pieza(t_tetromino *p, uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO]);
void dibujar_tablero(const uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO], const uint8_t textura[TAM_BLOQUE][TAM_BLOQUE]);
void cargar_nueva_pieza(const uint8_t pieza_origen[DIMENSION_PIEZA][DIMENSION_PIEZA], uint8_t pieza_destino[DIMENSION_PIEZA][DIMENSION_PIEZA]);
void rotar_pieza(t_tetromino *p, int sentido);

#endif // HEADER_H_INCLUDED
