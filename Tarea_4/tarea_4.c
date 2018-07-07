#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <estructuras.h>
#include <metodos.h>

#define length(x) (sizeof(x)/sizeof(x[0]))

typedef struct bola_numero bola;

int main(int argc, char** argv) {
    system("clear");
	int rank;
	int size;
    MPI_Init(&argc, &argv);

	//De aca sacamos el tamaño de los nodos que asignamos con el mpirun
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root;
    root=0;

    //Creamos el tipo de dato que enviaremos por mpi SOLO DE LA ESTRUCTURA BOLA_NUMERO
    int blocklengths_bola[2] = {1,1};
    MPI_Datatype types_bola[2] = {MPI_INT, MPI_CHAR};
    MPI_Datatype MPI_BOLA_NUMERO;
    MPI_Aint disps_bola[2];
    disps_bola[0] = offsetof( bola, numero );
    disps_bola[1] = offsetof( bola, color );

    MPI_Type_create_struct(2, blocklengths_bola, disps_bola, types_bola, &MPI_BOLA_NUMERO);
    MPI_Type_commit(&MPI_BOLA_NUMERO);


    //Defnimos este tipo de dato, para crear las bolas
    
    int cantidad_cartones=4*15;
    int lim_inferior=cantidad_cartones/size;

    bola vector_bolas[60];
    bola distribucion_bolas[30];

    int datos_cartones[cantidad_cartones];
    int datos_nodos[array_split(cantidad_cartones,size)[rank]-rank*lim_inferior];
    //Creamos los 4 cartones
    int carton_uno[3][5];
    int carton_dos[3][5];
    int carton_tres[3][5];
    int carton_cuatro[3][5];
    
    if(rank==root){
        llenar_vector_bolas(&vector_bolas);
        llenar_distribucion_bolas(&distribucion_bolas);
        crear_carton(&carton_uno,1,&datos_cartones);
        crear_carton(&carton_dos,2,&datos_cartones);
        crear_carton(&carton_tres,3,&datos_cartones);
        crear_carton(&carton_cuatro,4,&datos_cartones);

        for(int i=0;i < 30 ;i++){
            printf(" %d",distribucion_bolas[i].numero);
        }
        printf("\n");
        printf("\n");
        printf("Carton 1\n");
        for(int i=0;i < 3;i++){
            for(int j=0; j < 5;j++){
                printf(" %d",carton_uno[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Carton 2\n");

        for(int i=0;i < 3;i++){
            for(int j=0; j < 5;j++){
                printf(" %d",carton_dos[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Carton 3\n");
        
        for(int i=0;i < 3;i++){
            for(int j=0; j < 5;j++){
                printf(" %d",carton_tres[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Carton 4\n");
        
        for(int i=0;i < 3;i++){
            for(int j=0; j < 5;j++){
                printf(" %d",carton_cuatro[i][j]);
            }
            printf("\n");
        }
        
        printf("\n");
        printf("\n");



    }

    // Enviamos una copia a todos los nodos de las 30 bolas al azar que se lanzan en la mesa, para posteriormente marcar los cartones en cada nodo
    MPI_Bcast(&distribucion_bolas,30,MPI_BOLA_NUMERO,root,MPI_COMM_WORLD);
    MPI_Scatter(&datos_cartones,array_split(cantidad_cartones,size)[rank]-rank*lim_inferior,MPI_INT,&datos_nodos,array_split(cantidad_cartones,size)[rank]-rank*lim_inferior,MPI_INT,root,MPI_COMM_WORLD); 

    MPI_Finalize();
     return 0;

 }
