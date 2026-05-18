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
        copiar_matriz(p->matriz, pieza_S);
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
        break;
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
            return 1;
        }

        prueba.x -= 4;
        if (es_posicion_valida(&prueba, tablero)) {
            copiar_matriz(p->matriz, prueba.matriz);
            p->x = prueba.x;
            return 1;
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

int limpiar_lineas(uint8_t tablero[TABLERO_ALTO][TABLERO_ANCHO])
{
    int lineas_borradas = 0;

    // Recorremos desde la última fila (abajo) hacia la de arriba
    for (int fila = TABLERO_ALTO - 1; fila >= 0; fila--)
    {
        int linea_llena = 1;

        // Comprobamos si toda la fila tiene pelotas pintadas
        for (int col = 0; col < TABLERO_ANCHO; col++)
        {
            if (tablero[fila][col] == INDICE_TRANSP)
            {
                linea_llena = 0; // Encontramos un hueco libre
                break;
            }
        }

        // Si la línea no tiene huecos, la borramos
        if (linea_llena)
        {
            lineas_borradas++;

            // Desplazamos todas las filas superiores una posición hacia abajo
            for (int fila_arriba = fila; fila_arriba > 0; fila_arriba--)
            {
                for (int col = 0; col < TABLERO_ANCHO; col++)
                {
                    tablero[fila_arriba][col] = tablero[fila_arriba - 1][col];
                }
            }

            // Vaciamos la fila de arriba de todo para limpiar basura
            for (int col = 0; col < TABLERO_ANCHO; col++)
            {
                tablero[0][col] = INDICE_TRANSP;
            }

            // [!] Como todo bajó, volvemos a evaluar la misma fila en la próxima vuelta
            fila++;
        }
    }

    return lineas_borradas;
}

void dibujar_marco_tablero(int offset_x, int offset_y, uint8_t color_marco)
{
    // 1. Dibujamos las paredes izquierda y derecha
    for (int fila = 0; fila < TABLERO_ALTO; fila++)
    {
        int pixel_y = offset_y + (fila * TAM_BLOQUE);

        // Bloque pegado a la izquierda (un bloque antes del inicio del tablero)
        dibujar_cuadrado_base(offset_x - TAM_BLOQUE, pixel_y, color_marco);

        // Bloque pegado a la derecha (justo donde termina el ancho del tablero)
        dibujar_cuadrado_base(offset_x + (TABLERO_ANCHO * TAM_BLOQUE), pixel_y, color_marco);
    }

    // 2. Dibujamos el piso (va desde la esquina izquierda a la derecha)
    for (int col = -1; col <= TABLERO_ANCHO; col++)
    {
        int pixel_x = offset_x + (col * TAM_BLOQUE);
        int pixel_y = offset_y + (TABLERO_ALTO * TAM_BLOQUE);

        dibujar_cuadrado_base(pixel_x, pixel_y, color_marco);
    }
}

void dibujar_digito_8x8(int x_pantalla, int y_pantalla, int numero, uint8_t color)
{
    if (numero < 0 || numero > 9) return;

    for (int y = 0; y < 8; y++)
    {
        uint8_t fila_bits = fuente_numeros[numero][y];

        for (int x = 0; x < 8; x++)
        {
            // Escaneamos el bit correspondiente de izquierda a derecha
            if ((fila_bits & (0x80 >> x)) != 0)
            {
                gbt_dibujar_pixel(x_pantalla + x, y_pantalla + y, color);
            }
        }
    }
}

void dibujar_numero_entero(int x_pantalla, int y_pantalla, int valor, uint8_t color)
{
    char buffer[12];
    // Convertimos el número a una cadena de texto (String) clásica de C
    sprintf(buffer, "%d", valor);

    int posicion_x = x_pantalla;
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        int digito = buffer[i] - '0'; // Convertimos el caracter char a un int puro
        dibujar_digito_8x8(posicion_x, y_pantalla, digito, color);
        posicion_x += 9; // Dejamos 8 píxeles del número + 1 píxel de espacio libre
    }
}

void dibujar_caracter_8x8(int x_pantalla, int y_pantalla, char caracter, uint8_t color)
{
    // 1. Si es un espacio en blanco, no dibujamos nada y pasamos de largo
    if (caracter == ' ') return;

    // 2. Si es un número (del '0' al '9')
    if (caracter >= '0' && caracter <= '9')
    {
        int indice_num = caracter - '0';
        for (int y = 0; y < 8; y++)
        {
            uint8_t bits = fuente_numeros[indice_num][y];
            for (int x = 0; x < 8; x++)
            {
                if ((bits & (0x80 >> x)) != 0)
                    gbt_dibujar_pixel(x_pantalla + x, y_pantalla + y, color);
            }
        }
    }
    // 3. Si es una letra mayúscula (de la 'A' a la 'Z')
    else if (caracter >= 'A' && caracter <= 'Z')
    {
        int indice_letra = caracter - 'A';
        for (int y = 0; y < 8; y++)
        {
            uint8_t bits = fuente_letras[indice_letra][y];
            for (int x = 0; x < 8; x++)
            {
                if ((bits & (0x80 >> x)) != 0)
                    gbt_dibujar_pixel(x_pantalla + x, y_pantalla + y, color);
            }
        }
    }
}

void mi_gbt_dibujar_texto(int x_pantalla, int y_pantalla, const char* texto, uint8_t color)
{
    int pos_x = x_pantalla;

    for (int i = 0; texto[i] != '\0'; i++)
    {
        // Pasamos el caracter actual a la función de dibujo individual
        dibujar_caracter_8x8(pos_x, y_pantalla, texto[i], color);

        // Desplazamos la X para el siguiente caracter (8 píxeles + 1 de espacio)
        pos_x += 9;
    }
}
// Esta función convierte un color hexadecimal arbitrario de la matriz
// al color CGA de 16 colores más cercano de tu paleta.
// Esta función convierte un color hexadecimal arbitrario de la matriz
// al color CGA de 16 colores más cercano de tu paleta.
// ESTA VERSIÓN REFINADA ELIMINA EL RUIDO Y PECAS DE LA PIEL.
/*
uint8_t mapear_color_cga(uint8_t color_orig)
{
    switch(color_orig)
    {
        // Negros y sombras profundas (Pelo de Rukawa y bordes)
        case 0x00: case 0x20: case 0x24: case 0x40: case 0x44: case 0x49:
            return 0; // CGA Negro

        // Grises oscuros / Sombras secundarias
        case 0x60: case 0x64: case 0x65: case 0x68: case 0x69:
            return 8; // CGA Gris Oscuro

        // Tonos de piel oscuros (Cuellos y sombras de las caras)
        case 0x8d: case 0x8e: case 0x91: case 0x92:
            return 7; // CGA Marrón / Dark Yellow (ideal para piel oscura)

        // --- SECCIÓN REFINADA PARA ELIMINAR PECAS ---
        // Tonos de piel medios y medios-claros. Consolidamos un rango mayor
        // para unificar la piel y eliminar las pecas amarillas/naranjas.
        // Agregamos tonos que podrían estar creando ruido (ej: 0xd2, 0xd6).
        case 0xad: case 0xae: case 0xb1: case 0xb2: case 0xb6:
        case 0xd2: case 0xd6: case 0xf2: case 0xf6: case 0xfb:
            return 7; // CGA Gris Claro (piel beige lisa, sin speckles)

        // Rojos oscuros (Camisetas y sombras del pelo)
        case 0x80: case 0x84: case 0x85: case 0x89:
            return 4; // CGA Rojo

        // Rojos brillantes (¡El pelo de Hanamichi!)
        case 0xa4: case 0xa9: case 0xc4: case 0xc5: case 0xc8: case 0xc9: case 0xcd: case 0xe4: case 0xe5: case 0xe8: case 0xe9: case 0xed:
            return 12; // CGA Rojo Claro

        default:
            return 7; // Si se nos escapó algún color raro de la piel, lo pintamos de gris piel
    }
}
*/
uint8_t mapear_color_cga(uint8_t color_orig)
{
    switch(color_orig)
    {
        case 0x00: case 0x20: case 0x24: case 0x40: case 0x44: case 0x49:
            return 0; // Al Negro puro

        case 0x60: case 0x64: case 0x65: case 0x68: case 0x69:
            return 1; // Al Gris azulado (sombras)

        case 0x8d: case 0x8e: case 0x91: case 0x92:
            return 2; // A Piel oscura

        case 0xad: case 0xae: case 0xb1: case 0xb2: case 0xb6:
        case 0xd2: case 0xd6: case 0xf2: case 0xf6: case 0xfb:
            return 3; // A Piel media/clara lisa (mata las pecas)

        case 0x80: case 0x84: case 0x85: case 0x89:
            return 5; // A Rojo oscuro

        case 0xa4: case 0xa9: case 0xc4: case 0xc5: case 0xc8: case 0xc9: case 0xcd: case 0xe4: case 0xe5: case 0xe8: case 0xe9: case 0xed:
            return 6; // A Rojo vivo (Hanamichi)

        default:
            return 7; // Fondo / Gris claro
    }
}
void dibujar_slamdunk(int x_inicial, int y_inicial)
{
    long indice = 0;

    for (int y = 0; y < SLAM_DUNK_HEIGHT; y++)
    {
        for (int x = 0; x < SLAM_DUNK_WIDTH; x++)
        {
            uint8_t color_original = Slam_dunk[indice];

            // Si el pixel no es el fondo (suponiendo que 0xFF es el fondo blanco de la foto)


                // Pasamos el color hexadecimal raro por nuestro traductor CGA
                uint8_t color_cga = mapear_color_cga(color_original);

                gbt_dibujar_pixel(x_inicial + x, y_inicial + y, color_cga);


            indice++;
        }
    }
}

int guardar_puntaje(t_jugador *jugador)
{
    t_jugador guardado;
    FILE *fp = fopen("puntaje.dat", "ab");

    if(fp == NULL)
    {
        printf("Error al guardar puntaje\n");
        return -1;
    }

    fread(&guardado, sizeof(t_jugador), 1, fp);
    while(!feof(fp) && jugador->puntaje > guardado.puntaje && strcmpi(jugador->nombre, guardado.nombre)==0)
    {

        fread(jugador, sizeof(jugador), 1, fp);
    }

        fwrite(jugador, sizeof(t_jugador), 1, fp);

    fclose(fp);
}
