#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "GBT/gbt.h"

#define TABLERO_ANCHO 10
#define TABLERO_ALTO 20
#define CENTRO_TABLERO (TABLERO_ANCHO / 2) - 2
#define MITAD 2

#define RES_ANCHO_CGA 320
#define RES_ALTO_CGA  200

#define RES_ANCHO_VGA 640
#define RES_ALTO_VGA 480

#define ANCHO_VENTANA (TABLERO_ANCHO * TAM_BLOQUE)
#define ALTO_VENTANA  (TABLERO_ALTO * TAM_BLOQUE)
#define ESCALA_VENTANA 3

#define TAM_BLOQUE 8
#define DIMENSION_PIEZA 4

#define I 1
#define O 2
#define T 3
#define S 4
#define Z 5
#define J 6
#define L 7

#define GIRO_Q -1
#define GIRO_E 1

#define INDICE_NEGRO    0
#define INDICE_GRIS_CL  7  // Gris Claro
#define INDICE_GRIS_OS  8  // Gris Oscuro
#define INDICE_AZUL_C   9  // Azul claro para la J
#define INDICE_VERDE_C  10 // Verde claro para la S
#define INDICE_CIAN     11 // Cian para el palo (I)
#define INDICE_ROJO_C   12 // Rojo claro para la Z
#define INDICE_MAGENTA  13 // Magenta para la T
#define INDICE_AMARILLO 14 // Amarillo para la O
#define INDICE_NARANJA  6  // Marron/Naranja para la L
#define INDICE_TRANSP   15
#define INDICE_RELLENO  99

#define CANT_COLORES 16

#define SIN_MOV_DER 1
#define CON_MOV_DER 0
#define SIN_MOV_IZQ 1
#define CON_MOV_IZQ 0

// --- ESTADOS DEL JUEGO ---
#define ESTADO_MENU      0
#define ESTADO_JUGANDO   1
#define ESTADO_PAUSA     2
#define ESTADO_GAMEOVER  3

#define SLAM_DUNK_HEIGHT 104
#define SLAM_DUNK_WIDTH 270

typedef struct {
    uint8_t matriz[DIMENSION_PIEZA][DIMENSION_PIEZA];
    int dimension;
    int x;
    int y;
    uint8_t color;
} t_tetromino;

typedef struct{
    char nombre[11];
    int puntaje;
} t_jugador;

// 1. Declaramos que la paleta existe y tiene 16 elementos
extern tGBT_ColorRGB paletaCGA[CANT_COLORES];

void dibujar_cuadrado_base(uint16_t x_pantalla, uint16_t y_pantalla, uint8_t color_pieza);
void dibujar_pieza(t_tetromino *p, int offset_x, int offset_y);
void fijar_pieza(t_tetromino *p, uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO]);
void dibujar_tablero(const uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO], int offset_x, int offset_y);
void cargar_nueva_pieza(const uint8_t pieza_origen[DIMENSION_PIEZA][DIMENSION_PIEZA], uint8_t pieza_destino[DIMENSION_PIEZA][DIMENSION_PIEZA]);
int rotar_pieza(t_tetromino *p, int sentido, uint8_t tablero[][TABLERO_ANCHO]);
void crear_pieza(t_tetromino *p);
int limpiar_lineas(uint8_t [][TABLERO_ANCHO]);
void dibujar_marco_tablero(int offset_x, int offset_y, uint8_t color_marco);

#endif // HEADER_H_INCLUDED
