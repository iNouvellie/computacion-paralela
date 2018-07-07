#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include<time.h>

//Asignamos valores random a la lista entre 0 y 1 con un gran rango de 1000000 numeros 
void lista_aleatoria(int n, float l[]){
	for(int i = 0 ; i < n ; i++){	
		l[i]=(float)(rand()%1000000)/1000000;
	}
}

//Esta funcion retornara la cantidad de datos y los tomara el ultimo nodo 
int reajuste_asignacion(int numero_operaciones, int numero_procesos){
	int a,b;
	a=numero_operaciones/numero_procesos;
	b=a*numero_procesos;
	if(b == numero_operaciones){
		return 0;
	}else{
		return numero_operaciones-b;
	}
}

//Hacemos la raiz cuadrada a mano, ya que tuve problemas con la libreria math.h
double raiz_cuadrada(double numero,double aproximacion, int n)
{
	double aproximacionantes = aproximacion;
 
	//Utilizando la formula de Newton-Raphson
	aproximacion = aproximacion - (((aproximacion * aproximacion) - numero ) / ( 2 * aproximacion));
 
	//Verificas que la aproximación haya cambiado, y cuidamos nuestro contador, para que la recursión no se trabe
	if(aproximacion == aproximacionantes || n > 50)

	//Regresas la aproximación en caso de que el numero no cambie
	return aproximacion; 
 
	//Llamada recursiva para calcular una nueva aproximación
	return raiz_cuadrada(numero,aproximacion,++n);
}

int main(int argc, char** argv) {
	int rank;
	int size;
       	MPI_Init(&argc, &argv);

	//De aca sacamos el tamaño de los nodos que asignamos con el mpirun
	MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	clock_t t_ini, t_fin;
  	double secs;

	//Este es el promedio de la lista completa
	float promedio=0.0;
	
	int muestra=100;	
	
	//Generamos numeros aleatorios entre 0 y 1, el tamanho de la muestra es 100
	float l[muestra];

	//La Esta variable "sumatoria", es el argumento de la sumatoria de la varianza de la muestra
	float sumatoria=0.0;
	float varianza=0.0;
	float desviacion_estandar;
        const int root=0;
	float *sumas;
	char a;
	int reajuste=reajuste_asignacion(muestra,size);
	float sumas_reajustes=0.0;

	//Calculamos el tamanho total por subdivision de la lista, en el caso de tamanho 100 la subdivision es de 25
	int div_tamanho=muestra/size;
	
	//Nos ubicamos en el nodo maestro rank=0
        if(rank == root) {
		system("clear");
		t_ini = clock();

		//Aca le damos el tamaño de la lista que usaremos para guardar los datos en la raiz, usando el Gather esto es fundamental
		sumas=(int*)malloc(sizeof(int)*1*size);
		lista_aleatoria(muestra,&l);
		
		//Calculamos el promedio de la lista completa en el nodo maestro
		for(int i = 0 ; i < muestra ; i++){
			promedio=promedio+l[i];
		}

		//Es necesario castear a float los resultados, ya que si no los deja en enteros y trunca
		promedio=(float)(promedio)/muestra;
        }

	//Hacemos el bcast para enviar los datos a todos los nodos, en este caso enviamos el promedio
	//Tambien se hace enfasis en la direccion de la variable que se envia con el &, el tamaño de la variable, el tipo, los nodos y el comm
        MPI_Bcast(&promedio, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

	//En este caso, enviamos la lista, ya que la funcion que utilizamos para llenar la lista de numeros aleatorios la llamamos en el nodo maestro
	MPI_Bcast(&l,muestra,MPI_FLOAT,root,MPI_COMM_WORLD);
	
	//Hacemos la sumatoria, pero en forma paralela, definiendo los limites, en el caso de la lista 1, rank=0 recorrera de 0 a 24 tomando esos valores de la lista y restando el promedio total ya calculado. En el rank=1 parte de 25 y llegaria hasta 49 y asi sucesivamente
	for(int i = div_tamanho*rank  ; i < div_tamanho*(rank+1) ; i++){
		sumatoria=((l[i]-promedio)*(l[i]-promedio))+sumatoria;
	}  

	//Mostramos los nodos y los argumentos de la sumatoria que estamos calculando, para enviarla al nodo maestro 
	printf("%d %f\n",rank,sumatoria);

	//Lejos lo que mas me complico, fue entender esta estructura, ya que no sabia manejar la lista donde recepcionaba los datos que enviaba al nodo maestro con Gather, es simple la estructura, ya que define los datos que envias con referencia a la direccion de memoria, tamaño de la variable y tipo de dato de la variable que se envia. Despues hay que hubicar la lista donde vas a recepcionar los datos que se envian, el tamaño de dato que envia cada nodo el tipo de dato que recibe, el root y el coom. Los ultimos 5 datos los maneja la raiz unicamente
	MPI_Gather(&sumatoria, 1, MPI_FLOAT,sumas,1,MPI_FLOAT, root, MPI_COMM_WORLD);

	if(rank==root){

		//Aca revisaremos que los datos sean asignados al 100%. Un ejemplo de un dato no asignado es cuando tenemos 3 nodos, ya que dividiremos el numero de operaciones por el numero de procesadores 100/3 al el tipo de dato entero lo trunca, dejando la division de 33 operaciones por nodo, lo que en este caso particular dejaria 1 dato sin ser asignado a un nodo para que pueda operar con el.
		if(reajuste!=0){
			for(int i = muestra-reajuste  ; i < muestra ; i++){
				sumas_reajustes=((l[i]-promedio)*(l[i]-promedio))+sumas_reajustes;
			}
		}
		
		//Sumamos las sumatorias calculadas paralelamente en cada nodo y con esto podemos conocer la varianza
		//Hay que destacar que el reajuste aplicado sera en el nodo raiz y se agregara despues de la suma de los nodos en paralelo
		varianza=sumas_reajustes;
		//Sumamos las sumatorias calculadas paralelamente en cada nodo y con esto podemos conocer la varianza
		for(int i = 0 ; i < size ; i++){	
			varianza=sumas[i]+varianza;		
		}
		varianza=varianza/muestra;

		//La desviacion estandar seria la raiz de la varianza
		desviacion_estandar=raiz_cuadrada(varianza,1,0);
		t_fin = clock();
		printf("La varianza es %f\n",varianza);
		printf("La desviacion estandar es %f\n",desviacion_estandar);
		secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  		printf("%.16g milisegundos\n", secs);
		printf("Desea ver la muestra? (S/N)\n");
		scanf("%c",&a);
		if(a=='S'){
			for(int i = 0 ; i < muestra ; i++){
				printf(" %f",l[i]);
			}
		}else{
			if(a=='s'){
				for(int i = 0 ; i < muestra ; i++){
					printf(" %f",l[i]);
				}	
			}
		}
	}
        MPI_Finalize();
        return 0;
}
