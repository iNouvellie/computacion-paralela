#include <stdio.h>
#include <time.h>

typedef struct bola_numero bola;

int no_repetir_numero_bolas(int numero_aleatorio, int tamano_vector,bola lista[]){
	for(int i=0;i < tamano_vector;i++){
		if(numero_aleatorio == (lista[i].numero-1)){
			return 0;
		}
	}	
	return 1;
}

int no_repetir_numero_carton_vector(int numero_aleatorio, int tamano_vector,int lista[]){
	for(int i=0;i < tamano_vector;i++){
		if(numero_aleatorio == (lista[i]-1)){
			return 0;
		}
	}	
	return 1;
}

void llenar_vector_bolas(bola vector_bolas[60]){
	for(int i=0;i<60;i++){
		//1 al 14 amarillas
		if(i<14){
			vector_bolas[i].color='y';
			vector_bolas[i].numero=i;
		}else{
			//15 al 29 azules
			if(i<29){
				vector_bolas[i].color='b';
				vector_bolas[i].numero=i;
			}else{
				//30 al 44 rojo
				if(i<44){
					vector_bolas[i].color='r';
					vector_bolas[i].numero=i;
				}else{
					//45 al 60 verde
					if(i<60){
						vector_bolas[i].color='g';
						vector_bolas[i].numero=i;
					}
				}
			}
		}
	}
}

void llenar_distribucion_bolas(bola lista[]){
	srand(time(0));
	int numero_aleatorio;
	for(int i=0;i < 30; i++){
		numero_aleatorio=rand()%59;
		while(no_repetir_numero_bolas(numero_aleatorio,i,lista)==0){
			numero_aleatorio=rand()%59;
		}
		if(numero_aleatorio < 14){
			lista[i].numero=numero_aleatorio+1;
			lista[i].color='y';
		}else{
			if(numero_aleatorio < 29){
				lista[i].numero=numero_aleatorio+1;
				lista[i].color='b';
			}else{
				if(numero_aleatorio < 44){
					lista[i].numero=numero_aleatorio+1;
					lista[i].color='r';
				}else{
					if(numero_aleatorio < 60){
						lista[i].numero=numero_aleatorio+1;
						lista[i].color='g';
					}
				}
			}
		}
	}
}

void crear_carton(int (*m)[5], int num_carton, int lista[]){
	srand(time(0)*num_carton+num_carton);
	int numero_aleatorio;
	int contador=0;
	if(num_carton==1){
		contador=0;
	}else{
		if(num_carton==2){
			contador=15;
		}else{
			if(num_carton==3){
				contador=30;
			}else{
				if(num_carton=4){
					contador=45;
				}
			}
		}
	}
	for(int i=0;i < 3;i++){
		for(int j=0; j < 5;j++){
			numero_aleatorio=rand()%59;
			while(no_repetir_numero_carton_vector(numero_aleatorio,contador,lista)==0){
				numero_aleatorio=completar_aleatorio(numero_aleatorio);
			}
			m[i][j]=numero_aleatorio+1;

			//Llenamos el vector
			lista[contador]=numero_aleatorio+1;
			contador=contador+1;
		}
	}
}

//Retorna los limites superiores del vector
int *array_split(int total_tamano_cartones,int cantidad_nodos){
	int *tamano_distribuido=malloc(cantidad_nodos*sizeof(int));
	int cociente_sin_truncar=total_tamano_cartones/cantidad_nodos;
	int truncamiento=cociente_sin_truncar*cantidad_nodos;
	int diferencial=total_tamano_cartones-truncamiento;
	
	for(int i=0;i<cantidad_nodos;i++){
		if(i==(cantidad_nodos-1)){
			tamano_distribuido[i]=cociente_sin_truncar*(i+1)+diferencial;
		}else{
			tamano_distribuido[i]=cociente_sin_truncar*(i+1);
		}
	}
	return tamano_distribuido;
}

int completar_aleatorio(int n){
	if(n==59){
		return 0;
	}else{
		return n+1;
	}
}
