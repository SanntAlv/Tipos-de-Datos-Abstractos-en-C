#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "src/hash.h"
#include "src/csv.h"

#define SEPARADOR ';'
#define TAMAÑO_INICIAL 50

struct pokemon {
	char *nombre;
	char tipo;
	int fuerza;
	int destreza;
	int resistencia;
};

char *copiar_nombre(const char *nombre)
{
	size_t longitud = strlen(nombre) + 1;
	char *nombre_copiado = malloc(longitud);
	if (!nombre_copiado)
		return NULL;
	memcpy(nombre_copiado, nombre, longitud);
	return nombre_copiado;
}

bool parsear_nombre(const char *str, void *ctx)
{
	struct pokemon *p = ctx;
	p->nombre = copiar_nombre(str);
	return p->nombre != NULL;
}

bool parsear_tipo(const char *str, void *ctx)
{
	struct pokemon *p = ctx;
	p->tipo = str[0];
	return true;
}

bool parsear_fuerza(const char *str, void *ctx)
{
	struct pokemon *p = ctx;
	p->fuerza = atoi(str);
	return true;
}

bool parsear_destreza(const char *str, void *ctx)
{
	struct pokemon *p = ctx;
	p->destreza = atoi(str);
	return true;
}

bool parsear_resistencia(const char *str, void *ctx)
{
	struct pokemon *p = ctx;
	p->resistencia = atoi(str);
	return true;
}

bool agregar_pokemon_desde_archivo(hash_t *pokedex, const char *nombre_archivo)
{
	struct archivo_csv *archivo =
		abrir_archivo_csv(nombre_archivo, SEPARADOR);
	if (!archivo) {
		printf("Error al abrir el archivo CSV.\n");
		return false;
	}

	bool (*funciones[5])(const char *,
			     void *) = { parsear_nombre, parsear_tipo,
					 parsear_fuerza, parsear_destreza,
					 parsear_resistencia };
	size_t columnas = 5;
	struct pokemon *p;

	while (true) {
		p = malloc(sizeof(struct pokemon));
		if (!p) {
			printf("Error al asignar memoria para el Pokémon.\n");
			cerrar_archivo_csv(archivo);
			return false;
		}

		void *ctx[5] = { p, p, p, p, p };
		if (leer_linea_csv(archivo, columnas, funciones, ctx) == 0) {
			free(p);
			break;
		}

		if (!hash_insertar(pokedex, p->nombre, p, NULL)) {
			printf("Error al agregar el Pokémon al hash.\n");
			free(p);
			cerrar_archivo_csv(archivo);
			return false;
		}
	}

	cerrar_archivo_csv(archivo);
	return true;
}

void liberar_pokemon(void *pokemon)
{
	struct pokemon *p = pokemon;
	free(p->nombre);
	free(p);
}

bool imprimir_pokemon(char *clave, void *dato, void *ctx)
{
	struct pokemon *p = dato;
	if (!p)
		return false;

	printf("Nombre: %s, Tipo: %c, Fuerza: %d, Destreza: %d, Resistencia: %d\n",
	       p->nombre, p->tipo, p->fuerza, p->destreza, p->resistencia);
	return true;
}

void limpiar_buffer(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

int mostrar_menu()
{
	int opcion, validar_opcion;
	printf("\nSeleccione una opción:\n");
	printf("1. Buscar un Pokémon por nombre.\n");
	printf("2. Listar todos los Pokémon.\n");
	printf("3. Salir del programa.\n");
	printf("Opción: ");
	validar_opcion = scanf("%d", &opcion);

	if (validar_opcion != 1) {
		limpiar_buffer();
		return -1;
	}
	return opcion;
}

void buscar_pokemon(hash_t *pokedex)
{
	char nombre[50];
	printf("Ingrese el nombre del Pokémon: ");
	if (scanf("%49s", nombre) != 1) {
		printf("\nError al leer el nombre.\n");
		return;
	}

	struct pokemon *encontrado = hash_buscar(pokedex, nombre);

	if (encontrado) {
		printf("Pokémon encontrado: Nombre: %s, Tipo: %c, Fuerza: %d, Destreza: %d, Resistencia: %d\n",
		       encontrado->nombre, encontrado->tipo, encontrado->fuerza,
		       encontrado->destreza, encontrado->resistencia);
	} else {
		printf("Pokémon no encontrado, recuerde que el nombre inicia con letra mayuscula\nPor ejemplo: Pikachu.\n");
	}
}

void listar_pokemon(hash_t *pokedex)
{
	printf("Listado de Pokémon:\n");
	hash_iterar(pokedex, imprimir_pokemon, NULL);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Uso: %s archivo_pokemon.csv\n", argv[0]);
		return -1;
	}

	hash_t *pokedex = hash_crear(TAMAÑO_INICIAL);
	if (!pokedex) {
		printf("Error al crear el hash para la Pokedex\n");
		return 1;
	}

	if (!agregar_pokemon_desde_archivo(pokedex, argv[1])) {
		hash_destruir(pokedex);
		return 1;
	}

	int opcion;
	do {
		opcion = mostrar_menu();

		switch (opcion) {
		case 1:
			buscar_pokemon(pokedex);
			opcion = 3;
			break;
		case 2:
			listar_pokemon(pokedex);
			opcion = 3;
			break;
		case 3:
			printf("Saliendo del programa...\n");
			break;
		default:
			printf("\nOpción no válida. Por favor, ingrese 1, 2 o 3.\n");
			break;
		}
	} while (opcion != 3);

	hash_destruir_todo(pokedex, liberar_pokemon);
	return 0;
}