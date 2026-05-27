#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "GBT/gbt.h"

#define TABLERO_ANCHO 10
#define TABLERO_ALTO 24
#define FILAS_INVISIBLES 4

#define CENTRO_TABLERO (TABLERO_ANCHO / 2) - 2
#define MITAD 2

#define RES_ANCHO_CGA 320
#define RES_ALTO_CGA  200

#define RES_ANCHO_VGA 640
#define RES_ALTO_VGA 480

#define POS_RES 1

#define ANCHO_VENTANA (TABLERO_ANCHO * TAM_BLOQUE)
#define ALTO_VENTANA  (TABLERO_ALTO * TAM_BLOQUE)
#define ESCALA_VENTANA 3
#define POS_ESCALA 2

#define MIN_ARG 3

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
#define MOVIO 1
#define NO_MOVIO 0

#define INDICE_NEGRO    0
#define INDICE_PIEL_MEDIA  3 // Para la madera a color
#define INDICE_GRIS_MED    4 // Para la madera en grises (ajustá si usás otro gris)
#define INDICE_ROJO_OSC    5 // Para la zona pintada a color
#define INDICE_SOMBRA_OSC  5
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

#define SUB_MENU_PPAL   0
#define SUB_NOMBRE      1
#define SUB_PUNTAJES    2
#define SUB_CONFIG      3
#define SUB_DECISION    4

#define SLAM_DUNK_HEIGHT 104
#define SLAM_DUNK_WIDTH 270

#define MAX_JUGADORES 100

typedef struct {
    uint8_t matriz[DIMENSION_PIEZA][DIMENSION_PIEZA];
    int dimension;
    int x;
    int y;
    uint8_t color;
} t_tetromino;

typedef struct{
    int estado_actual;
    int corriendo;
    int res_ancho;
    int res_alto;
    int escala;
    int opcion_menu;
    int sub_pantalla;
    int paleta_tipo;
    float velocidad_base;
    int opcion_config;
    int pausado;
    int opcion_pausa;
    int opcion_gameover;
    int centro_x;
    int centro_y;
    int opcion_decision;
} t_sistema;

typedef struct{
    char nombre[15];
    int largo_nombre;
    int puntaje;
} t_jugador;

typedef struct {
    t_tetromino pieza_activa;
    uint8_t tablero_principal[TABLERO_ALTO][TABLERO_ANCHO];
    int tablero_ancho_px;
    int tablero_alto_px;
    int puntaje_total;
    int contador_fichas;
    float tiempo_caida_actual;
    int offset_x;
    int offset_y;
    tGBT_Temporizador* temp_gravedad;
    tGBT_Temporizador* temp_soft_drop;
    tGBT_Temporizador* temp_fijacion;
    int en_fijacion;
    char nombre_jugador[15];
} t_datosJuego;

// 1. Declaramos que la paleta existe y tiene 16 elementos
extern tGBT_ColorRGB paletaCGA[CANT_COLORES];
extern tGBT_ColorRGB paletaGameBoy[CANT_COLORES];
extern tGBT_ColorRGB paletaBlancoYNegro[CANT_COLORES];

void copiar_matriz(uint8_t destino[DIMENSION_PIEZA][DIMENSION_PIEZA], const uint8_t origen[DIMENSION_PIEZA][DIMENSION_PIEZA]);
void mi_gbt_dibujar_texto_16(int x_pantalla, int y_pantalla, const char* texto, uint8_t color);

void dibujar_cuadrado_base(uint16_t x_pantalla, uint16_t y_pantalla, uint8_t color_pieza, int);
void dibujar_pieza(t_tetromino *p, int offset_x, int offset_y, int);

void fijar_pieza(t_tetromino *p, uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO]);
void dibujar_tablero(const uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO], int offset_x, int offset_y, int);

void cargar_nueva_pieza(const uint8_t pieza_origen[DIMENSION_PIEZA][DIMENSION_PIEZA], uint8_t pieza_destino[DIMENSION_PIEZA][DIMENSION_PIEZA]);
int rotar_pieza(t_tetromino *p, int sentido, uint8_t tablero[][TABLERO_ANCHO]);

void crear_pieza(t_tetromino *p);
int limpiar_lineas(uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO]);

void dibujar_marco_tablero(int offset_x, int offset_y, uint8_t color_marco);
void inicializarSistema(t_sistema *sys, int argc, char* argv[]);

void inicializarDatosJuego(t_datosJuego *dt, t_sistema *sys);
void inicializarTablero(t_datosJuego *dt);

void movimientosPiezas(t_datosJuego *dt);
void actualizar_juego(t_datosJuego *dt, t_sistema *sys);

void controlar_menu_principal(t_sistema *sys);
void dibujar_menu_principal(const t_sistema *sys);

void controlar_pantalla_decision(t_sistema *sys, t_datosJuego *dt, t_jugador *player);

void controlar_ingreso_nombre(t_sistema *sys, t_jugador *player);
void dibujar_ingreso_nombre(t_sistema *sys, t_jugador *player);

void controlar_pantalla_puntajes(t_sistema *sys);
void dibujar_pantalla_puntajes(t_sistema *sys);

void controlar_pantalla_configuracion(t_sistema *sys);
void dibujar_pantalla_configuracion(t_sistema *sys);

void controlar_menu_pausa(t_sistema *sys, t_datosJuego *dt, t_jugador *player);
void dibujar_menu_pausa(const t_sistema *sys);

void controlar_pantalla_gameover(t_sistema *sys, t_datosJuego *dt, t_jugador *player);
void dibujar_pantalla_gameover(const t_sistema *sys);

void cargar_configuracion(t_sistema *sys);
void guardar_configuracion(t_sistema *sys);

void guardar_puntaje(t_jugador *nuevo_jugador);

void guardar_partida(const t_datosJuego *dt);
int existe_partida();

void dibujar_slamdunk(int x_inicial, int y_inicial);
void dibujar_cancha_fondo(int res_ancho, int res_alto, int offset_x, int offset_y, int ancho_tablero, int paleta_tipo);

void mi_gbt_dibujar_texto(int x_pantalla, int y_pantalla, const char* texto, uint8_t color);
int cargar_partida(t_datosJuego *dt);

void dibujar_pantalla_decision(const t_sistema *sys);
void dibujar_score(t_datosJuego *dt);

int choca_izquierda (t_tetromino *p, uint8_t tablero[][TABLERO_ANCHO]);
int choca_derecha (t_tetromino *p, uint8_t tablero[][TABLERO_ANCHO]);

int choca_abajo(t_tetromino *p, uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO]);
int es_posicion_valida(t_tetromino *p, uint8_t tablero[][TABLERO_ANCHO]);

void dibujar_bloque_marco_basket(uint16_t x_pantalla, uint16_t y_pantalla);

#endif // HEADER_H_INCLUDED
