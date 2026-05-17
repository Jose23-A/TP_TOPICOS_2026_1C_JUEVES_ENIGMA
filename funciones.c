#include "header.h"
#include "matrices.h"

void copiar_matriz(uint8_t destino[DIMENSION_PIEZA][DIMENSION_PIEZA], uint8_t origen[DIMENSION_PIEZA][DIMENSION_PIEZA])
{
    for (int f = 0; f < DIMENSION_PIEZA; f++)
    {
        for (int c = 0; c < DIMENSION_PIEZA; c++)
        {
            destino[f][c] = origen[f][c];
        }
    }
}

void crear_pieza(t_tetromino *p)
{
    p->y = 0;
    p->x = CENTRO_TABLERO;

    int tipo_aleatorio = (rand() % 7) + 1;

    // 1:I, 2:O, 3:T, 4:S, 5:Z, 6:J, 7:L
    switch(tipo_aleatorio)
    {
    case I:
        p->dimension = 4;
        copiar_matriz(p->matriz, pieza_I);
        p->color = INDICE_CIAN;
        break;
    case O:
        p->dimension = 2;
        copiar_matriz(p->matriz, pieza_O);
        p->color = INDICE_AMARILLO;
        break;
    case T:
        p->dimension = 3;
        copiar_matriz(p->matriz, pieza_T);
        p->color = INDICE_MAGENTA;
        break;
    case S:
        p->dimension = 3;
        copiar_matriz(p->matriz, pieza_T);
        p->color = INDICE_VERDE_C;
        break;
    case Z:
        p->dimension = 3;
        copiar_matriz(p->matriz, pieza_Z);
        p->color = INDICE_ROJO_C;
        break;
    case J:
        p->dimension = 3;
        copiar_matriz(p->matriz, pieza_J);
        p->color = INDICE_AZUL_C;
        break;
    case L:
        p->dimension = 3;
        copiar_matriz(p->matriz, pieza_L);
        p->color = INDICE_NARANJA;
    default: // Por defecto cargamos una T para no romper nada
        //p->dimension = 3;
        //copiar_matriz(p->matriz, pieza_T);
        //p->color = INDICE_VERDE_C;
        break;
    }
}

//Pinta el cuadrado del tetromino
void dibujar_cuadrado_base(uint16_t x_pantalla, uint16_t y_pantalla, uint8_t color_pieza)
{
    for (int y = 0; y < TAM_BLOQUE; y++)
    {
        for (int x = 0; x < TAM_BLOQUE; x++)
        {
            uint8_t color_pixel = textura_molde[y][x];

            if (color_pixel == INDICE_RELLENO)
                color_pixel = color_pieza;

            gbt_dibujar_pixel(x_pantalla + x, y_pantalla + y, color_pixel);
        }
    }
}

void dibujar_tablero(const uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO], int offset_x, int offset_y)
{
    for (int fila = 0; fila < TABLERO_ALTO; fila++)
    {
        for (int col = 0; col < TABLERO_ANCHO; col++)
        {
            if (tablero[fila][col] != INDICE_TRANSP)
            {
                int pixel_x = offset_x + col * TAM_BLOQUE;
                int pixel_y = offset_y + fila * TAM_BLOQUE;

                dibujar_cuadrado_base(pixel_x, pixel_y, tablero[fila][col]);
            }
        }
    }
}

void dibujar_pieza(t_tetromino *p, int offset_x, int offset_y)
{
    int pixel_x, pixel_y;

    for (int fila = 0; fila < DIMENSION_PIEZA; fila++)
    {
        for (int col = 0; col < DIMENSION_PIEZA; col++)
        {
            if (p->matriz[fila][col] != INDICE_TRANSP)
            {
                pixel_x = offset_x + (p->x + col) * TAM_BLOQUE;
                pixel_y = offset_y + (p->y + fila) * TAM_BLOQUE;

                dibujar_cuadrado_base(pixel_x, pixel_y, p->matriz[fila][col]);
            }
        }
    }
}

void fijar_pieza(t_tetromino *p, uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO])
{
    for (int fila = 0; fila < DIMENSION_PIEZA; fila++)
    {
        for (int col = 0; col < DIMENSION_PIEZA; col++)
        {
            if (p->matriz[fila][col] != INDICE_TRANSP)
            {
                tablero[p->y + fila][p->x + col] = p->matriz[fila][col];
            }
        }
    }
}

void cargar_nueva_pieza(const uint8_t pieza_origen[DIMENSION_PIEZA][DIMENSION_PIEZA], uint8_t pieza_destino[DIMENSION_PIEZA][DIMENSION_PIEZA])
{
    for(int f = 0; f < DIMENSION_PIEZA; f++)
    {
        for(int c = 0; c < DIMENSION_PIEZA; c++)
        {
            pieza_destino[f][c] = pieza_origen[f][c];
        }
    }
}

int rotar_pieza(t_tetromino *p, int sentido, uint8_t tablero[][TABLERO_ANCHO])
{
    if (p->dimension == 2)
        return 0;

    t_tetromino prueba = *p;

    for (int i=0; i<DIMENSION_PIEZA; i++)
    {
        for (int j=0; j<DIMENSION_PIEZA; j++)
        {
            prueba.matriz[i][j]=INDICE_TRANSP;
        }
    }


    for (int f = 0; f < p->dimension; f++)
    {
        for (int c = 0; c < p->dimension; c++)
        {
            if (sentido == 1)
            {
                prueba.matriz[c][p->dimension - 1 - f] = p->matriz[f][c];
            }
            else
            {
                prueba.matriz[p->dimension - 1 - c][f] = p->matriz[f][c];
            }
        }
    }

    if (es_posicion_valida(&prueba, tablero)) {
        copiar_matriz(p->matriz, prueba.matriz);
        return 1;
    }

    prueba.x++;
    if (es_posicion_valida(&prueba, tablero)) {
        copiar_matriz(p->matriz, prueba.matriz);
        p->x = prueba.x;
        return 1;
    }

    prueba.x -= 2;
    if (es_posicion_valida(&prueba, tablero)) {
        copiar_matriz(p->matriz, prueba.matriz);
        p->x = prueba.x;
        return 1;
    }

    // --- INTENTOS EXCLUSIVOS PARA LA PIEZA 'I' ---
    if (p->dimension == 4)
    {
        prueba.x += 3;
        if (es_posicion_valida(&prueba, tablero)) {
            copiar_matriz(p->matriz, prueba.matriz);
            p->x = prueba.x;
            return;
        }

        prueba.x -= 4;
        if (es_posicion_valida(&prueba, tablero)) {
            copiar_matriz(p->matriz, prueba.matriz);
            p->x = prueba.x;
            return;
        }
    }

    return 0;
}

int es_posicion_valida(t_tetromino *p, uint8_t tablero[][TABLERO_ANCHO])
{
    for (int f = 0; f < DIMENSION_PIEZA; f++)
    {
        for (int c = 0; c < DIMENSION_PIEZA; c++)
        {

            if (p->matriz[f][c] != INDICE_TRANSP)
            {
                int fila_real = p->y + f;
                int col_real = p->x + c;

                // 1. Paredes y piso
                if (col_real < 0 || col_real >= TABLERO_ANCHO || fila_real >= TABLERO_ALTO)
                {
                    return 0;
                }

                // 2. Otras piezas (evitando leer basura arriba del tablero)
                if (fila_real >= 0 && tablero[fila_real][col_real] != INDICE_TRANSP)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Devuelve 1 si hay choque, 0 si hay camino libre
int choca_abajo(t_tetromino *p, uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO])
{
    for (int f = 0; f < DIMENSION_PIEZA; f++)
    {
        for (int c = 0; c < DIMENSION_PIEZA; c++)
        {

            // Solo nos interesan los bloques que tienen color (ignoramos el "aire")
            if (p->matriz[f][c] != INDICE_TRANSP)
            {

                // Calculamos a dónde iría este bloque si cae una posición más (y + 1)
                int futura_fila = p->y + f + 1;
                int actual_col = p->x + c;

                // 1. ¿Tocamos el piso del tablero?
                if (futura_fila >= TABLERO_ALTO)
                {
                    return 1; // ¡Choque!
                }

                // 2. ¿Tocamos otra pieza que ya estaba en el tablero?
                if (tablero[futura_fila][actual_col] != INDICE_TRANSP)
                {
                    return 1; // ¡Choque!
                }
            }
        }
    }
    return 0; // Todo libre, puede seguir cayendo
}

int choca_derecha (t_tetromino *p, uint8_t tablero[][TABLERO_ANCHO])
{
    int col_der, fila;
    for(int j=DIMENSION_PIEZA-1; j>=0; j--)
    {
        for(int i=0; i<DIMENSION_PIEZA; i++)
        {
            col_der = p->x + j +1;
            fila = p->y+i;

            if(p->matriz[i][j] != INDICE_TRANSP && col_der > TABLERO_ANCHO - 1)
                return SIN_MOV_DER;

            if( p->matriz[i][j] != INDICE_TRANSP && tablero[fila][col_der] != INDICE_TRANSP)
                return SIN_MOV_DER;
        }
    }

    return CON_MOV_DER;
}

int choca_izquierda (t_tetromino *p, uint8_t tablero[][TABLERO_ANCHO])
{
    int col_izq, fila;
    for(int j=0; j<DIMENSION_PIEZA; j++)
    {
        for(int i=0; i<DIMENSION_PIEZA; i++)
        {
            col_izq = p->x + j - 1;
            fila = p->y+i;

            if( p->matriz[i][j] != INDICE_TRANSP && col_izq < 0)
                return SIN_MOV_IZQ;

            if( p->matriz[i][j] != INDICE_TRANSP && tablero[fila][col_izq] != INDICE_TRANSP)
                return SIN_MOV_IZQ;
        }
    }

    return CON_MOV_IZQ;
}
