#include "pa2m.h"
#include "src/hash.h"
#include <stdlib.h>

void prueba_hash_crear_y_destruir()
{
	pa2m_nuevo_grupo("Pruebas de creación y destrucción de hash");

	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash != NULL,
		     "Se crea una tabla de hash con capacidad inicial");
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "La cantidad de elementos es 0 al inicio");

	hash_destruir(hash);
	pa2m_afirmar(true, "Se destruye la tabla de hash correctamente");
}

void prueba_hash_insertar()
{
	pa2m_nuevo_grupo("Pruebas de inserción en el hash");

	hash_t *hash = hash_crear(3);

	int valor1 = 10, valor2 = 20, valor3 = 30;
	pa2m_afirmar(hash_insertar(hash, "clave1", &valor1, NULL),
		     "Se inserta clave1 en el hash");
	pa2m_afirmar(hash_cantidad(hash) == 1, "La cantidad de elementos es 1");

	pa2m_afirmar(hash_insertar(hash, "clave2", &valor2, NULL),
		     "Se inserta clave2 en el hash");
	pa2m_afirmar(hash_cantidad(hash) == 2, "La cantidad de elementos es 2");

	pa2m_afirmar(hash_insertar(hash, "clave3", &valor3, NULL),
		     "Se inserta clave3 en el hash");
	pa2m_afirmar(hash_cantidad(hash) == 3, "La cantidad de elementos es 3");

	int nuevo_valor = 40;
	int *elemento_reemplazado = NULL;
	pa2m_afirmar(hash_insertar(hash, "clave2", &nuevo_valor,
				   (void **)&elemento_reemplazado),
		     "Se reemplaza valor de clave2");
	pa2m_afirmar(*elemento_reemplazado == valor2,
		     "Elemento reemplazado es el valor antiguo");

	hash_destruir(hash);
}

void prueba_hash_buscar()
{
	pa2m_nuevo_grupo("Pruebas de búsqueda en el hash");

	hash_t *hash = hash_crear(5);
	int valor = 100;

	hash_insertar(hash, "claveX", &valor, NULL);

	int *encontrado = hash_buscar(hash, "claveX");
	pa2m_afirmar(encontrado && *encontrado == valor,
		     "Se encuentra el elemento asociado a 'claveX'");

	pa2m_afirmar(hash_buscar(hash, "claveNoExistente") == NULL,
		     "Buscar clave inexistente devuelve NULL");

	hash_destruir(hash);
}

void prueba_hash_contiene()
{
	pa2m_nuevo_grupo("Pruebas de verificación de existencia en el hash");

	hash_t *hash = hash_crear(5);
	int valor = 200;
	hash_insertar(hash, "claveA", &valor, NULL);

	pa2m_afirmar(hash_contiene(hash, "claveA"), "Hash contiene 'claveA'");
	pa2m_afirmar(!hash_contiene(hash, "claveB"),
		     "Hash no contiene 'claveB'");

	hash_destruir(hash);
}

void prueba_hash_quitar()
{
	pa2m_nuevo_grupo("Pruebas de eliminación en el hash");

	hash_t *hash = hash_crear(5);
	int valor = 300;
	hash_insertar(hash, "claveEliminar", &valor, NULL);

	int *eliminado = hash_quitar(hash, "claveEliminar");
	pa2m_afirmar(
		eliminado && *eliminado == valor,
		"Se elimina 'claveEliminar' y devuelve el elemento correcto");

	pa2m_afirmar(hash_quitar(hash, "claveInexistente") == NULL,
		     "Intentar eliminar clave inexistente devuelve NULL");

	hash_destruir(hash);
}

bool aplicar_funcion_iteracion(char *clave, void *valor, void *contador)
{
	(*(size_t *)contador)++;
	return true;
}

void prueba_hash_iterar()
{
	pa2m_nuevo_grupo("Pruebas de iteración en el hash");

	hash_t *hash = hash_crear(5);
	int valor1 = 1, valor2 = 2, valor3 = 3;
	hash_insertar(hash, "clave1", &valor1, NULL);
	hash_insertar(hash, "clave2", &valor2, NULL);
	hash_insertar(hash, "clave3", &valor3, NULL);

	size_t contador = 0;
	size_t iterados =
		hash_iterar(hash, aplicar_funcion_iteracion, &contador);

	pa2m_afirmar(iterados == contador,
		     "La cantidad de elementos iterados es correcta");
	pa2m_afirmar(contador == hash_cantidad(hash),
		     "Se iteraron todos los elementos");

	hash_destruir(hash);
}

void prueba_hash_destruir_todo()
{
	pa2m_nuevo_grupo("Pruebas de destrucción completa del hash");

	hash_t *hash = hash_crear(5);
	int *valor1 = malloc(sizeof(int));
	int *valor2 = malloc(sizeof(int));
	*valor1 = 400;
	*valor2 = 500;

	hash_insertar(hash, "clave1", valor1, NULL);
	hash_insertar(hash, "clave2", valor2, NULL);

	hash_destruir_todo(hash, free);
	pa2m_afirmar(
		true,
		"Se destruye la tabla de hash con destructor aplicado a los elementos");
}

int main()
{
	prueba_hash_crear_y_destruir();
	prueba_hash_insertar();
	prueba_hash_buscar();
	prueba_hash_contiene();
	prueba_hash_quitar();
	prueba_hash_iterar();
	prueba_hash_destruir_todo();

	return pa2m_mostrar_reporte();
}
