// VerifConformidad.c    06-12
//  Para verificar la conformidad de las mallas generadas

#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <malloc.h>

#include "base.h"
#include "functions.h"

#include "read_files.h"

#define MAX 25

int main() {
   //  Definicion de datos
   s_vertices *vertices = NULL; //  lista de vertices
   s_triangulos *triangulos = NULL;   // lista de triangulos
   s_triangulo *triangulo = NULL;

   char msg[256];
   int i,ii, n_particiones, sw = 0;

	vertices = (s_vertices *) malloc ( sizeof (s_vertices) );
	triangulos = (s_triangulos *) malloc ( sizeof (s_triangulos) );
	char a[MAX],b[MAX], c[MAX];
 
  FILE *archtri, *archvert, *archpart;
  printf("\n Ingrese nombre del archivo triangulos: "); scanf("%s",a);
  if ((archtri = fopen (a, "r")) == NULL){
	 fprintf (stderr, "\nERROR: No es posible abrir el archivo de triangulos %s\n", a); getch();
	 exit(0);
  }
  printf("\n Ingrese nombre del archivo vertices: "); scanf("%s",b);
  if ((archvert = fopen (b, "r")) == NULL){
    fprintf (stderr, "\nERROR: No es posible abrir el archivo de vertices %s\n", b); getch();
    exit(0);
  }
  printf("\n Ingrese nombre del archivo particiones: "); scanf("%s",c);
  if ((archpart = fopen (c, "r")) == NULL){
    fprintf (stderr, "\nERROR: No es posible abrir el archivo de particiones %s\n", c); getch();
    exit(0);
  }
	read_vertices (vertices, b);
//    system("PAUSE");
	read_triangulos (triangulos, a);
//    system("PAUSE");
	n_particiones = read_particiones(triangulos, c);
//    system("PAUSE");
//printf("\n POR AQUI....\n ");
	// Loop mientras hayan triangulos en la malla original 
	for ( i = 0; i < triangulos->size; i++ ) {
		triangulo = get_triangulo(i, triangulos);

/////////////////////////////////////// DEBUG //////////////////////////////////////////////////
//		sprintf (msg,"Revisando Triangulo %d",i+1);
//		print_triangulo_info(msg, triangulo);
//		for( ii = 0; ii < 3; ii++ ) {
//			sprintf (msg,"Vertice %llu: ",triangulo->vertice[ii]+1);
//			print_vertice_info(msg, get_vertice(triangulo->vertice[ii], vertices));
//		} //  Fin del for 
/////////////////////////////////////// DEBUG //////////////////////////////////////////////////

		if (triangulo != NULL)  
			if ( (check_triangulo_conforme (triangulo, vertices) == FALSE ) )  {         
				sprintf (msg,"Triangulo No-conforme %d",i+1);
				print_triangulo_info(msg, triangulo);
				//printf("\n Y AQUI....\n ");
				for( ii = 0; ii < 3; ii++ ) {
					sprintf (msg,"Vertice %llu: ",triangulo->vertice[ii]+1);
					print_vertice_info(msg, get_vertice(triangulo->vertice[ii], vertices));
				} //  Fin del for 
				sw = 1;
			} // Fin del if ( (check_triangulo_conforme (triangulo, vertices) == FALSE ) )
	} // fin del FOR para revisar todos los triangulos
	if (sw == 0)
		printf("Triangulos Conformes\n");
	//printf("\n POR ACA....\n ");	
    system("PAUSE");
	return 0;
}
