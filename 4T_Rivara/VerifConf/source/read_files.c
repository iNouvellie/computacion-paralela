// read_files.c    26-11
#include <stdio.h>
#include <stdlib.h>

#include "base.h"
// ===============================================================
/* Read Vertices
 * Input: Puntero hacia arreglo con Vertices 
 * Output: Numero de Vertices encontrados
 */
void read_vertices (s_vertices *vertices, char archivo[]) {
	FILE *file;
	int i, trash;
	s_vertice *vertices_temp;
	long size_array;

	file = fopen (archivo, "r");
	
	/* Determinar cuantos vertices contiene */
	fscanf (file, "%ld %d", &size_array, &trash);
	vertices_temp = (s_vertice *) malloc ( sizeof (s_vertice) * size_array); 

	for (i = 0; i < size_array; i++) {
		fscanf (file, "%d %f %f", &trash, &(vertices_temp[i].x), &(vertices_temp[i].y));
 		vertices_temp[i].status = 0x00;
		vertices_temp[i].status |= ENABLED_BIT;
	}
	init_vertices (vertices_temp, size_array, vertices);
	fclose (file);
}
// =================================================================================
/* Read Triangulos
 * Input: Puntero hacia arreglo con triangulos 
 * Output: Numero de triangulos encontrados
 */
void read_triangulos (s_triangulos *triangulos, char archivo[]) {
	FILE *file;
	int i,	trash;
	
	s_triangulo *triangulos_temp;
	
  long size_array;
	file = fopen (archivo, "r");
	// Determinar cuantos triangulos contiene 
	fscanf (file, "%ld %d", &size_array, &trash);
	triangulos_temp = (s_triangulo *) malloc ( sizeof (s_triangulo) * size_array); 

	for (i = 0; i < size_array; i++) {
		fscanf (file, "%d %d %d %d", &trash, &triangulos_temp[i].vertice[0], &triangulos_temp[i].vertice[1], &triangulos_temp[i].vertice[2]);
    //fscanf (file,"%d %d %d", &triangulos_temp[i].vertice[0], &triangulos_temp[i].vertice[1], &triangulos_temp[i].vertice[2]);
		/* Hacer que los vertices partan en 0 */
		triangulos_temp[i].vertice[0]--;
		triangulos_temp[i].vertice[1]--;
		triangulos_temp[i].vertice[2]--;

		// Setear los otros valores iniciales del triangulo 
		triangulos_temp[i].particionID	= 0;
		triangulos_temp[i].status	= 0x00;
		triangulos_temp[i].status	|= ENABLED_BIT;
		triangulos_temp[i].parentID	= i;
		triangulos_temp[i].rootID	= i;
	}
	init_triangulos (triangulos_temp, size_array, triangulos);
	fclose (file);
}

// ======================================================================================
/* Read Particiones
 * Input: Puntero hacia arreglo con triangulos, numero de triangulos 
 * Output: Numero de particiones encontradas
 */
unsigned int read_particiones (s_triangulos *triangulos, char archivo[]) {
	FILE *file;
	uint max_part = 0;
	int i, trash;
	long size_array;
	s_triangulo *triangulo_t;	

	file = fopen (archivo, "r");
	fscanf (file, "%ld %d", &size_array, &trash);

	for (i = 0; i < triangulos->size; i++) {
		uint seleccionado;
		triangulo_t = get_triangulo(i, triangulos);
		fscanf (file, "%u %u",&triangulo_t->particionID, &seleccionado);
		if ( seleccionado == 1 ) {
			triangulo_t->status |= SELECTED_BIT;
		}
      // Determinar cual es la particion con el ID mas alto */
		if ( max_part < triangulo_t->particionID ) {
			max_part = triangulo_t->particionID;
		}
	}
	fclose (file);
	// Retornar el numero de particiones encontradas
	return max_part + 1;
}
