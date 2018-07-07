//  base.c  15-11
// Funciona

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "base.h"
#include "functions.h"

/* Triangulos */
// ===========================================================================================
void init_triangulos (s_triangulo *data_array, int size_array, s_triangulos *triangulos) {
	triangulos->size = size_array;
	triangulos->size_array = size_array;
	triangulos->data_array = data_array;
	triangulos->data_list = NULL;
}
// ===========================================================================================
int add_triangulo (s_triangulo *triangulo, s_triangulos *triangulos) {
	int id;
	/* Ver si existe un triangulo con los mismos vertices */
	/* Si existe, devolver su ID y no agregar uno nuevo */
	id = get_triangulo_id (triangulo, triangulos);
	if ( id > 0 ) {
		id--;
		return id;
	}
	/* Setear el status basico */
	triangulo->status = 0x00;
	triangulo->status |= ENABLED_BIT;
	triangulos->size++;
	s_triangulo_node *triangulo_node_t, *triangulo_node_new;
	/* Crear el nuevo nodo */
	triangulo_node_new = (s_triangulo_node *) malloc ( sizeof ( s_triangulo_node) );
	triangulo_node_new->next = NULL;
	triangulo_node_new->triangulo = triangulo;
	/* Apuntar al comienzo de la lista */
	triangulo_node_t = triangulos->data_list;	
	/* Ver si es el primer nodo de la lista */
	if ( triangulo_node_t == NULL ) {
		triangulos->data_list = triangulo_node_new;
	} else {
		/* Si no es el primer nodo, buscar cual es el ultimo actual */
		while (triangulo_node_t->next != NULL) {
			triangulo_node_t = triangulo_node_t->next;
		}
		/* Agregar el nuevo nodo a la lista */
		triangulo_node_t->next = triangulo_node_new;
	}
	return (triangulos->size - 1);
}
// =============================================================================
/* Get Triangulo ID
 * Input: Puntero hacia el triangulo a buscar, puntero hacia los triangulos
 * Output: ID+1 del triangulo
 */
int get_triangulo_id (s_triangulo *triangulo, s_triangulos *triangulos) {
	int i;
	s_triangulo *triangulo_t;

   for (i = 0; i < triangulos->size; i++) {
		triangulo_t = get_triangulo (i, triangulos);
		if ( triangulo_t != NULL ) {
			if ( (triangulo_t->vertice[0] == triangulo->vertice[0]) &&
				   (triangulo_t->vertice[1] == triangulo->vertice[1]) &&
			 	   (triangulo_t->vertice[2] == triangulo->vertice[2]) ) {
				return i+1;
			}
		}
	}
	return 0;
}
// ====================================================================================
/* Get Triangulo
 * Input: ID del triangulo y puntero a triangulos
 * Output: Puntero a triangulo requerido
 * Nota: El ID parte en 0
 */
s_triangulo *get_triangulo (int trianguloID, s_triangulos *triangulos) {
	int i;
	s_triangulo_node *triangulo_node;
   //char msg[256];

	/* Ver si el ID esta dentro del rango aceptable */
	if ( trianguloID >= triangulos->size) {
		return NULL;
	}
	/* Ver si se tiene que recuperar el triangulo desde el arreglo de la lista */
	/* Recuperarlo del array */
	if ( trianguloID < triangulos->size_array ) {
		if ( triangulos->data_array[trianguloID].status & ENABLED_BIT )
			return &triangulos->data_array[trianguloID];
		else
			return NULL;
	}
	/* Recuperarlo de la lista */
	if ( trianguloID >= triangulos->size_array ) {
		i = triangulos->size_array;
		triangulo_node = triangulos->data_list;
		while (i != trianguloID) {
			triangulo_node = triangulo_node->next;
			i++;
		}	
		if (triangulo_node->triangulo->status & ENABLED_BIT)
			return triangulo_node->triangulo;
		else
			return NULL;
	}
	printf ("[get_triangulo] ERROR, ID Desconocido: %llu\n", trianguloID);
	return NULL;
}
// ===============================================================================
/* Vertices */
void init_vertices (s_vertice *data_array, int size_array, s_vertices *vertices) {
	vertices->size = size_array;
	vertices->size_array = size_array;
	vertices->data_array = data_array;
	vertices->data_list = NULL;
}
// =====================================================================================
/* Seria bueno revisar si es frontera o interfaz al agregarlo */
int add_vertice (s_vertice *vertice, s_vertices *vertices) {
	int id;	
	/* Ver si existe el vertice, para no agregarlo 2 veces */
	id = get_vertice_id (vertice, vertices);
	if ( id > 0 ) {
		id--;
		return id;
	}
	/* Setear el status basico */
	vertice->status = 0x00;
	vertice->status |= ENABLED_BIT;
	vertices->size++;
	s_vertice_node *vertice_node_t, *vertice_node_new;
	/* Crear el nuevo vertice */
	vertice_node_new = (s_vertice_node *) malloc ( sizeof ( s_vertice_node) );
	vertice_node_new->next = NULL;
	vertice_node_new->vertice = vertice;

	// aqui verificar si es FRONTERA O INTERFAZ

	/* Apuntar al comienzo de la lista */
	vertice_node_t = vertices->data_list;
	/* Ver si es el primer vertice de la lista */
	if ( vertice_node_t == NULL ) {
		vertices->data_list = vertice_node_new;
	} else {
		/* Si no es el primer vertice, buscar cual es el ultimo actual */	
		while (vertice_node_t->next != NULL) {
			vertice_node_t = vertice_node_t->next;
		}
		/* Agregar el nuevo vertice a la lista */
		vertice_node_t->next = vertice_node_new;
	}
	return (vertices->size - 1);
}
// ==============================================================================
/* Get Vertice
 * Input: ID del vertice y puntero a vertices
 * Output: Puntero a vertice requerido
 * Nota: El ID parte en 0
 */
s_vertice *get_vertice (int verticeID, s_vertices *vertices) {
	int i;
	s_vertice_node *vertice_node;
	/* Ver si el ID esta dentro del rango aceptable */
	if ( verticeID >= vertices->size ) {
		printf ("[get_vertice] ERROR, ID fuera del rango aceptable: %llu\n", verticeID);
		return NULL;
	}
	/* Ver si se tiene que recuperar el vertice desde el arreglo O desde la lista */
	/* Recuperarlo del array */
	if ( verticeID < vertices->size_array ) {
		if (vertices->data_array[verticeID].status & ENABLED_BIT)
			return &vertices->data_array[verticeID];
		else
			return NULL;
	}
	/* Recuperarlo de la lista */
	if ( verticeID >= vertices->size_array ) {
		i = vertices->size_array;
		vertice_node = vertices->data_list;
		while (i != verticeID) {
			vertice_node = vertice_node->next;
			i++;
		}
		if (vertice_node->vertice->status & ENABLED_BIT)
			return vertice_node->vertice;
		else
			return NULL;
	}
	printf ("[get_vertice] ERROR, ID Desconocido: %llu\n", verticeID);
	return NULL;
}
// ==============================================================================
/* Get Vertice ID
 * Input: Puntero hacia el vertice a buscar, puntero hacia los vertices
 * Output: ID+1 del vertice
 */
int get_vertice_id (s_vertice *vertice, s_vertices *vert) {
	int i;
	s_vertice *vertice_t;

	for (i = 0; i < vert->size; i++) {
		vertice_t = get_vertice (i, vert);
		if ( vertice_t != NULL ) {
			if ( vertice_t->status & ENABLED_BIT ) {
				if ( (vertice_t->x == vertice->x) && (vertice_t->y == vertice->y) ) {
					return i+1;
				}
			}
		}
	}
	return 0;
}
// ===============================================================================
