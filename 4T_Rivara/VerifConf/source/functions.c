// functions.c     15-11
// Funciona 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "base.h"
#include "functions.h"

extern void print_triangulo_info (char msg[], s_triangulo *triangulo);
extern void print_triangulos_info (s_triangulos *triangulos, int rank);

// =====================================================================================
/* Bisect Arista
 * Input: ID de 2 vertices y puntero hacia vertices
 * Output: Nuevo vertice que se encuentra en la mitad de los 2 vertices
 */
s_vertice *bisect_arista (int vertice1_id, int vertice2_id, s_vertices *vertices) {
	float delta_x, delta_y;
	
	s_vertice *vertice_new = (s_vertice*) malloc ( sizeof (s_vertice) );
	s_vertice *vertice1, *vertice2;

	/* Recuperar los vertices */
	vertice1 = get_vertice (vertice1_id, vertices);
	vertice2 = get_vertice (vertice2_id, vertices);
	if ( (vertice1 == NULL) || (vertice2 == NULL) ) {
		printf ("[bisect_arista] Error Vertice no Encontrado\n");
		return NULL;
	}
	/* Calcular el punto en X */
	if ( vertice1->x < vertice2->x ) {
		delta_x = vertice2->x - vertice1->x;
		vertice_new->x = vertice1->x + (delta_x/2);
	} else {
		delta_x = vertice1->x - vertice2->x;
		vertice_new->x = vertice2->x + (delta_x/2);
	}
	/* Calcular el punto en Y */
	if ( vertice1->y < vertice2->y ) {
		delta_y = vertice2->y - vertice1->y;
		vertice_new->y = vertice1->y + (delta_y/2);
	} else {
		delta_y = vertice1->y - vertice2->y;
		vertice_new->y = vertice2->y + (delta_y/2);
	}
	return vertice_new;
}
// ==================================================================================
/* Get Distancia Arista
 * Input: 2 ID de vertices y Punterio hacia los vertices
 * Output: float con la distancia entre los 2 vertices 
 */
float get_distancia_arista (int vertice1_id, int vertice2_id, s_vertices *vertices) {
	s_vertice *vertice1, *vertice2;
	float delta_x, delta_y, distancia;
	/* Recuperar los vertices */
	vertice1 = get_vertice (vertice1_id, vertices);
	vertice2 = get_vertice (vertice2_id, vertices);
	if ( (vertice1 == NULL) || (vertice2 == NULL) ) {
		printf ("[get_distancia_arista] Error Vertice no Encontrado\n");
		return -1;
	}
	delta_x = vertice1->x - vertice2->x;
	delta_y = vertice1->y - vertice2->y;
	distancia = sqrtf ( delta_x*delta_x + delta_y*delta_y );
	return distancia;
}
// ====================================================================================
/* Search Longest Arista in Triangulo
 * Input: Puntero hacia el triangulo en cuestion y puntero hacia los vertices
 * Output: Arreglo de 2 elementos conteniendo los 2 ID de los vertices de la arista mas larga del triangulo 
 */
int *search_longest_arista (s_triangulo *triangulo, s_vertices *vertices) {
	int *resultado = (int *) malloc ( sizeof (int) * 2 );
	float distancia[3]; 
	/* Calcular la distancia de la primera arista (entre los vertices 0 y 1) */
	distancia[0] = get_distancia_arista ( triangulo->vertice[0], triangulo->vertice[1], vertices);
	/* Calcular la distancia de la segunda arista (entre los vertices 1 y 2) */
	distancia[1] = get_distancia_arista ( triangulo->vertice[1], triangulo->vertice[2], vertices);
	/* Calcular la distancia de la tercera arista (entre los vertice 2 y 0) */
	distancia[2] = get_distancia_arista ( triangulo->vertice[2], triangulo->vertice[0], vertices);
	/* Ver cual arista es mas larga */
	if ( distancia[0] > distancia[1] ) {
		if ( distancia[0] > distancia[2] ) {
			resultado[0] = triangulo->vertice[0];
			resultado[1] = triangulo->vertice[1];
		} else {
			resultado[0] = triangulo->vertice[2];
			resultado[1] = triangulo->vertice[0];
		}	
	} else {
		if ( distancia[1] > distancia[2] ) {
			resultado[0] = triangulo->vertice[1];
			resultado[1] = triangulo->vertice[2];
		} else {
			resultado[0] = triangulo->vertice[2];
			resultado[1] = triangulo->vertice[0];
		}	
	}
	return resultado;
}
// =====================================================================================
/* Check Triangulo Conforme
 * Input: Puntero hacia el triangulo a revisar, puntero hacia los vertices
 * Output: TRUE o FALSE 
 */
int check_triangulo_conforme (s_triangulo *triangulo, s_vertices *vertices) {
	s_vertice *vertice;
	int status = TRUE;

	/* Revisar si esta conforme en el vertice 0 - 1 */
	vertice = bisect_arista (triangulo->vertice[0], triangulo->vertice[1], vertices);	
	if ( get_vertice_id ( vertice, vertices ) != 0 )
		status = FALSE;     // es un nuevo vertice
	/* Revisar si esta conforme en el vertice 1 - 2 */
	vertice = bisect_arista (triangulo->vertice[1], triangulo->vertice[2], vertices);	
	if ( get_vertice_id ( vertice, vertices ) != 0 )
		status = FALSE;
	/* Revisar si esta conforme en el vertice 2 - 0 */
	vertice = bisect_arista (triangulo->vertice[2], triangulo->vertice[0], vertices);	
	if ( get_vertice_id ( vertice, vertices ) != 0 )
		status = FALSE;
	free (vertice); 
	return status;
}
// ====================================================================================
/* Bisect Triangulo
 * Input: ID del triangulo, puntero hacia triangulos, puntero hacia vertices
 * Output: Arreglo con ID de los 2 triangulos nuevos
 */
int *bisect_triangulo (int trianguloID, s_triangulos *triangulos, s_vertices *vertices, int rank) {
  int i, vertice_new_id, *vertices_t, *triangulo_new_id;
  	
	s_triangulo *triangulo_t, *triangulo_new[2];
	s_vertice *vertice_new;
	
	triangulo_t = get_triangulo (trianguloID, triangulos);  // Cargar el triangulo 
	if ( triangulo_t == NULL )      
		return NULL;
	/* Buscar la arista mas larga */
	vertices_t = search_longest_arista (triangulo_t, vertices);
	/* Bisectar la arista */
	vertice_new = bisect_arista (vertices_t[0], vertices_t[1], vertices);
	/* Agregar el vertice a la coleccion de vertices */
	vertice_new_id = add_vertice (vertice_new, vertices);
	/* Crear los 2 triangulos nuevos */
	triangulo_new[0] = (s_triangulo *) malloc ( sizeof (s_triangulo) );
	triangulo_new[1] = (s_triangulo *) malloc ( sizeof (s_triangulo) );
	/* Dejarlos activados */
	triangulo_new[0]->status = 0x00; triangulo_new[0]->status = ENABLED_BIT;
	triangulo_new[1]->status = 0x00; triangulo_new[1]->status = ENABLED_BIT;
	/* Los nuevos triangulos quedan en la particion del original */
	triangulo_new[0]->particionID = triangulo_t->particionID;
	triangulo_new[1]->particionID = triangulo_t->particionID;
	/* Definir de que triangulo aparecen */
	triangulo_new[0]->rootID = triangulo_t->rootID;
	triangulo_new[1]->rootID = triangulo_t->rootID;	
	triangulo_new[0]->parentID = trianguloID;
	triangulo_new[1]->parentID = trianguloID;
	/* Ver que vertice queda en comun para los triangulos, aparte del vertice nuevo */
	for (i = 0; i < 3; i++)  
		/* Caso que el vertice i del triangulo original no este en la arista mas larga */
		if ( (triangulo_t->vertice[i] != vertices_t[0]) && (triangulo_t->vertice[i] != vertices_t[1]) ) {
			triangulo_new[0]->vertice[0] = triangulo_t->vertice[i];
			triangulo_new[0]->vertice[1] = vertices_t[0];
			triangulo_new[0]->vertice[2] = vertice_new_id;
	
			triangulo_new[1]->vertice[0] = triangulo_t->vertice[i];
			triangulo_new[1]->vertice[1] = vertices_t[1];
			triangulo_new[1]->vertice[2] = vertice_new_id;
		}
	/* Desactivar el triangulo original */
	triangulo_t->status &= ~ENABLED_BIT;
	/* Agregar los 2 triangulos nuevos a la coleccion de triangulos */
	triangulo_new_id = (int *) malloc ( sizeof (int) * 2 );
	triangulo_new_id[0] = add_triangulo (triangulo_new[0], triangulos);
	triangulo_new_id[1] = add_triangulo (triangulo_new[1], triangulos);

	return triangulo_new_id;
}
// ===============================================================================================
/* Search Triangulo By Vertices
 * Input : Puntero hacia arreglo con triangulos, VerticeA, VerticeB y Triangulo Actual 
 * Output: Numero + 1 del triangulo cuya arista sea igual.
 * 		  Retorna 0 si no se encuentra ningun triangulo
 */
int search_triangulo_by_vertices (s_triangulos *triangulos, int vertice_a, int vertice_b, int n) {
	int i, t;
	s_triangulo *triangulo;
	int encontrado = FALSE;

	t = 0;  	/* Suponiendo que no encuentra ninguno */
	/* Recorrer todos los triangulos */
	for (i = 0; i < triangulos->size && encontrado == FALSE; i++) {
		/* Ver si algun triangulo que no sea el pedido contiene esa arista */
		if ( i != n ) {			
			triangulo = get_triangulo (i, triangulos);  /* Recuperar el triangulo i */
			if ( triangulo != NULL ) {
				/* Arista 0-1 */
				if ( ( triangulo->vertice[0] == vertice_a ) && ( triangulo->vertice[1] == vertice_b ) ) {
					/* El triangulo encontrado tiene los vertices en la arista 0-1 */
					t = i + 1;
					encontrado = TRUE;
				}
				/* Arista 1-0 */
				if ( ( triangulo->vertice[1] == vertice_a ) && ( triangulo->vertice[0] == vertice_b ) ) {
					/* El triangulo encontrado tiene los vertices en la arista 1-0 */
					t = i + 1;
					encontrado = TRUE;
				}	
				/* Arista 1-2 */
				if ( ( triangulo->vertice[1] == vertice_a ) && ( triangulo->vertice[2] == vertice_b ) ) {
					/* El triangulo encontrado tiene los vertices en la arista 1-2 */
					t = i + 1;
					encontrado = TRUE;
				}	
				/* Arista 2-1 */
				if ( ( triangulo->vertice[2] == vertice_a ) && ( triangulo->vertice[1] == vertice_b ) ) {
					/* El triangulo encontrado tiene los vertices en la arista 2-1 */
					t = i + 1;
					encontrado = TRUE;
				}	
				/* Arista 2-0 */
				if ( ( triangulo->vertice[2] == vertice_a ) && ( triangulo->vertice[0] == vertice_b ) ) {
					/* El triangulo encontrado tiene los vertices en la arista 2-0 */
					t = i + 1;
					encontrado = TRUE;
				}	
				/* Arista 0-2 */
				if ( ( triangulo->vertice[0] == vertice_a ) && ( triangulo->vertice[2] == vertice_b ) ) {
					/* El triangulo encontrado tiene los vertices en la arista 0-2 */
					t = i + 1;
					encontrado = TRUE;
				}
			}
		}
	} 
	return t;
}
// ======================================================================================================
void search_vertices_status (s_triangulos *triangulos, s_vertices *vertices) {
	int i, t;

	s_triangulo *triangulo, *triangulo_t;
	s_vertice *vertice1, *vertice2;	
	/* Recorrer todos los triangulos */
	for (i = 0; i < triangulos->size; i++) {
		/* Apuntar el triangulo al triangulo actual */
		triangulo = get_triangulo (i, triangulos);
		if ( triangulo != NULL ) {
			/* Ver si existe otro triangulo que tenga una de las 3 aristas */	
			/* Arista 0-1 */
			{	t = search_triangulo_by_vertices (triangulos, triangulo->vertice[0], triangulo->vertice[1], i);
				/* Recuperar los vertices */
				vertice1 = get_vertice(triangulo->vertice[0], vertices);
				vertice2 = get_vertice(triangulo->vertice[1], vertices);
				if ( (vertice1 != NULL) && (vertice2 != NULL) ) {
					/* Ver si es una arista frontera */
					if (t == 0) {
						vertice1->status |= FRONTERA_BIT;
						vertice2->status |= FRONTERA_BIT;
					} else {						
						t--;	/* Corregir la variable devuelta para que parta desde 0 */							
						triangulo_t = get_triangulo (t, triangulos); 	/* Recuperar el triangulo */	
						if ( triangulo_t != NULL ) {             
							/* Ver si el triangulo esta en la misma particion */							
							if ( triangulo_t->particionID != triangulo->particionID ) {
								vertice1->status |= INTERFAZ_BIT;
								vertice2->status |= INTERFAZ_BIT;
							}
						}
					}
				}
			}
			/* Arista 1-2 */
			{	t = search_triangulo_by_vertices (triangulos, triangulo->vertice[1], triangulo->vertice[2], i);	
				/* Recuperar los vertices */
				vertice1 = get_vertice(triangulo->vertice[1], vertices);
				vertice2 = get_vertice(triangulo->vertice[2], vertices);	
				if ( (vertice1 != NULL) && (vertice2 != NULL) ) {
					/* Ver si es una arista frontera */
					if (t == 0) {
						vertice1->status |= FRONTERA_BIT;
						vertice2->status |= FRONTERA_BIT;
					} else {						
						t--;		/* Corregir la variable devuelta para que parta desde 0 */
						triangulo_t = get_triangulo (t, triangulos);	/* Recuperar el triangulo */		
						if ( triangulo_t != NULL ) {
							/* Ver si el triangulo esta en la misma particion */
							if ( triangulo_t->particionID != triangulo->particionID ) {
								vertice1->status |= INTERFAZ_BIT;
								vertice2->status |= INTERFAZ_BIT;
							}
						}
					}
				}
			}
			/* Arista 2-0 */
			{	t = search_triangulo_by_vertices (triangulos, triangulo->vertice[2], triangulo->vertice[0], i);
				/* Recuperar los vertices */
				vertice1 = get_vertice(triangulo->vertice[2], vertices);
				vertice2 = get_vertice(triangulo->vertice[0], vertices);		
				if ( (vertice1 != NULL) && (vertice2 != NULL) ) {
					/* Ver si es una arista frontera */
					if (t == 0) {
						vertice1->status |= FRONTERA_BIT;
						vertice2->status |= FRONTERA_BIT;
					} else {					
						t--;	/* Corregir la variable devuelta para que parta desde 0 */						
						triangulo_t = get_triangulo (t, triangulos);	/* Recuperar el triangulo */	
						if ( triangulo_t != NULL ) {
							/* Ver si el triangulo esta en la misma particion */
							if ( triangulo_t->particionID != triangulo->particionID ) {
								vertice1->status |= INTERFAZ_BIT;
								vertice2->status |= INTERFAZ_BIT;
							}
						}
					}
				}
			}
		}
	}
}
// =======================================================================================
/* Search Triangulo Seleccionado
 * Input: Lista de Triangulos
 * Output: ID+1 del primer triangulo encontrado seleccionado */
int search_triangulo_seleccionado (s_triangulos *triangulos, int particion ) {
	int i;

	s_triangulo *triangulo = NULL;

	for ( i = 0; i < triangulos->size; i++ ) {
		triangulo = get_triangulo (i, triangulos);
		if ( triangulo != NULL ) {
			if ( ( particion == -1 ) || ( triangulo->particionID == (uint) particion ) ) {
				if ( triangulo->status & SELECTED_BIT ) {
					return i+1;
				}
			}
		}
	}
	return 0;
}
// =====================================================================================
/* Print Vertice Info
 * Input: String con texto a mostrar y vertice
 * Output: Imprime informacion en pantalla y no retorna nada 
 */
void print_vertice_info (char msg[], s_vertice *vertice) {
	int i = 0;

	printf ("%s Tipo: ", msg);
	/* Imprimir el Status del vertice */
	{  if ( vertice->status & FRONTERA_BIT ) {
			printf ("F");
			i++;
		} 
		if ( vertice->status & INTERFAZ_BIT ) {
			if (i == 1)
				printf ("|");	
			printf ("I");
			i++;
		} 	
		if ( i == 0 )
			printf ("N");   // no es frontera ni interfaz
		if ( i == 1 )
			printf (" ");
	}
	printf (" Ena: ");
	if ( vertice->status & ENABLED_BIT )
		printf ("Si ");
	else
		printf ("No ");
	if ( vertice->x > 0 )
		printf (" X: %.3f", vertice->x);
	else if ( vertice->x < 0 )
		printf (" X: %.3f", vertice->x);
	else
		printf (" X: 0.000");
	if ( vertice->y > 0 )
		printf (" Y: %.3f\n", vertice->y);
	else if ( vertice->y < 0)
		printf (" Y: %.3f\n", vertice->y);
	else
		printf (" Y: 0.000\n");
}
// ========================================================================================
void print_vertices_info (s_vertices *vertices, int rank) {
	int i, k=1;
	char msg[256];
  
	s_vertice *vertice;

	for (i = 0; i < vertices->size; i++) {
	  sprintf (msg, "[nodo %d] Vertice: %llu (%llu)", rank, k, i+1);	  
	  vertice = get_vertice (i, vertices);		
	  if ( vertice != NULL )
			print_vertice_info (msg, vertice);
     k++;			
	}
}
// ====================================================================================================
/* Print Triangulo Info
 * Input: String con texto a mostrar y triangulo
 * Output: Imprime informacion en pantalla y no retorna nada 
 */
void print_triangulo_info (char msg[], s_triangulo *triangulo) {
	if ( triangulo == NULL ) {
		printf ("%s Part ? Sel ? Ena: ? v1=? v2=? v3=?\n", msg);
	} else {
		printf ("%s Part %d Sel ",msg, triangulo->particionID+1);
		if ( triangulo->status & SELECTED_BIT )
			printf ("Si ");
		else
			printf ("No ");	
		printf ("Ena ");
    if ( triangulo->status & ENABLED_BIT )
		   printf ("Si ");
    else
		   printf ("No ");

		printf ("Vert %llu %llu %llu ", triangulo->vertice[0]+1, triangulo->vertice[1]+1, triangulo->vertice[2]+1);
		printf ("Padre %llu Raiz %llu\n", triangulo->parentID+1, triangulo->rootID+1);
	}
}
// =======================================================================================
void print_triangulos_info (s_triangulos *triangulos, int rank) {
	int i, k=1;
	char msg[256];	
   
	s_triangulo *triangulo;

	for (i = 0; i < triangulos->size; i++) {
      sprintf (msg, "[nodo %d] Triangulo: %llu (%llu)",rank, k, i+1 );     
		triangulo = get_triangulo (i, triangulos);
		if ( triangulo != NULL ) {
			print_triangulo_info (msg, triangulo);
         k++;			
      }
	} 
}
// ====================================================================================
/* Bisect Triangulo Conformar
 * Input: ID del triangulo, puntero hacia triangulos, puntero hacia vertices
 * Output: Arreglo con ID de los 2 triangulos nuevos
 */
int *bisect_triangulo_conf (int trianguloID, int vertices_t0, int vertices_t1, s_triangulos *triangulos, s_vertices *vertices, int rank) {
	int i, vertice_new_id, *triangulo_new_id;
	s_triangulo *triangulo_t, *triangulo_new[2];
	s_vertice *vertice_new;

	/* Cargar el triangulo */
	triangulo_t = get_triangulo (trianguloID, triangulos);
	if ( triangulo_t == NULL )      
		return NULL;

	/* Bisectar la arista */
	vertice_new = bisect_arista (vertices_t0, vertices_t1, vertices);
	/* Agregar el vertice a la coleccion de vertices */
	vertice_new_id = add_vertice (vertice_new, vertices);

	/* Crear los 2 triangulos nuevos */
	triangulo_new[0] = (s_triangulo *) malloc ( sizeof (s_triangulo) );
	triangulo_new[1] = (s_triangulo *) malloc ( sizeof (s_triangulo) );
	/* Dejarlos activados */
	triangulo_new[0]->status = 0x00; triangulo_new[0]->status = ENABLED_BIT;
	triangulo_new[1]->status = 0x00; triangulo_new[1]->status = ENABLED_BIT;
	/* Los nuevos triangulos quedan en la particion del original */
	triangulo_new[0]->particionID = triangulo_t->particionID;
	triangulo_new[1]->particionID = triangulo_t->particionID;
	/* Definir de que triangulo aparecen */
	triangulo_new[0]->rootID = triangulo_t->rootID;
	triangulo_new[1]->rootID = triangulo_t->rootID;	
	triangulo_new[0]->parentID = trianguloID;
	triangulo_new[1]->parentID = trianguloID;
	/* Ver que vertice queda en comun para los triangulos, aparte del vertice nuevo */
	for (i = 0; i < 3; i++) 
		/* Caso que el vertice i del triangulo original no este en la arista mas larga */
		if ( (triangulo_t->vertice[i] != vertices_t0) && (triangulo_t->vertice[i] != vertices_t1) ) {
			triangulo_new[0]->vertice[0] = triangulo_t->vertice[i];
			triangulo_new[0]->vertice[1] = vertices_t0;
			triangulo_new[0]->vertice[2] = vertice_new_id;
	
			triangulo_new[1]->vertice[0] = triangulo_t->vertice[i];
			triangulo_new[1]->vertice[1] = vertices_t1;
			triangulo_new[1]->vertice[2] = vertice_new_id;
		}
	/* Desactivar el triangulo original */
   triangulo_t->status &= ~ENABLED_BIT;

	/* Agregar los 2 triangulos nuevos a la coleccion de triangulos */
	triangulo_new_id = (int *) malloc ( sizeof (int) * 2 );
	triangulo_new_id[0] = add_triangulo (triangulo_new[0], triangulos);
	triangulo_new_id[1] = add_triangulo (triangulo_new[1], triangulos);
 
	return triangulo_new_id;
}
