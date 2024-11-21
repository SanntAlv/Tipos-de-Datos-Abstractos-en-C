#include "csv.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_INICIAL 1024

struct archivo_csv {
	char separador;
	FILE *archivo;
};

//Abre un archivo csv, lo prepara para leer y se define el separador
//retorna un puntero al archivo en caso de exito
struct archivo_csv *abrir_archivo_csv(const char *nombre_archivo,
				      char separador)
{
	struct archivo_csv *archivo = malloc(sizeof(struct archivo_csv));
	if (!archivo) {
		return NULL;
	}
	archivo->archivo = fopen(nombre_archivo, "r");
	if (!archivo->archivo) {
		free(archivo);
		return NULL;
	}
	archivo->separador = separador;
	return archivo;
}

size_t leer_linea_csv(struct archivo_csv *archivo, size_t columnas,
		      bool (*funciones[])(const char *, void *), void *ctx[])
{
	if (archivo == NULL) {
		return 0;
	}

	size_t capacidad_buffer = BUFFER_INICIAL;
	char *linea = malloc(capacidad_buffer);
	if (!linea) {
		return 0; // Si no se puede asignar memoria, retorna 0
	}

	// Lee la línea del archivo, redimensionando el buffer si es necesario
	size_t longitud = 0;
	int c;
	while ((c = fgetc(archivo->archivo)) != EOF && c != '\n') {
		if (longitud + 1 >= capacidad_buffer) {
			// Redimensiona el buffer si es necesario
			capacidad_buffer *= 2;
			char *nuevo_buffer = realloc(linea, capacidad_buffer);

			if (!nuevo_buffer) {
				free(linea); // Libera memoria en caso de fallo
				return 0;
			}

			linea = nuevo_buffer;
		}

		linea[longitud++] = (char)c;
	}

	linea[longitud] = '\0'; // Asegura que la línea es un string válido

	if (longitud == 0) {
		free(linea); // Si no se leyó nada, libera y retorna 0
		return 0;
	}

	size_t columnas_leidas = 0;
	size_t inicio_columna = 0;
	size_t longitud_columna = 0;
	size_t columna_actual = 0;
	bool dentro_columna = false;

	for (size_t i = 0; linea[i] != '\0' && columna_actual < columnas; i++) {
		if (linea[i] == archivo->separador || linea[i] == '\n') {
			// Encontramos el delimitador o el fin de línea, es el final de una columna
			if (dentro_columna) {
				linea[inicio_columna + longitud_columna] =
					'\0'; // Termina la columna actual

				// Verifica si la función para la columna no es NULL
				if (funciones[columna_actual] != NULL) {
					if (funciones[columna_actual](
						    &linea[inicio_columna],
						    ctx[columna_actual])) {
						columnas_leidas++;
					}
				}

				columna_actual++;
				dentro_columna = false;
			}
			inicio_columna =
				i +
				1; // La siguiente columna comienza después del delimitador
			longitud_columna = 0;
		} else {
			// Si no es un delimitador, estamos dentro de una columna
			if (!dentro_columna) {
				dentro_columna =
					true; // Inicia una nueva columna
				inicio_columna = i;
			}
			longitud_columna++; // Incrementa la longitud de la columna
		}
	}

	// Si no termina en delimitador
	if (dentro_columna && columna_actual < columnas) {
		linea[inicio_columna + longitud_columna] = '\0';

		// Verifica si la función para la columna no es NULL
		if (funciones[columna_actual] != NULL) {
			if (funciones[columna_actual](&linea[inicio_columna],
						      ctx[columna_actual])) {
				columnas_leidas++;
			}
		}
	}

	free(linea);
	return columnas_leidas;
}

void cerrar_archivo_csv(struct archivo_csv *archivo)
{
	if (archivo->archivo != NULL) {
		fclose(archivo->archivo);
		archivo->archivo =
			NULL; //se marca el archivo como cerrado para evitar que se intente cerrar nuevamente
	}
	free(archivo);
}