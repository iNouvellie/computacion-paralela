from mpi4py import MPI
import random
import math
from time import time

#Generamos una funcion para crear una lista con numeros aleatorios que toman valor de 0 a 1
def lista_aleatorios(n):
	lista=[0] *n
	for i in range(n):
		lista[i]=random.random()
	return lista

comm = MPI.COMM_WORLD

#Conocemos el numero de nodos totales
size = comm.Get_size()
rank = comm.Get_rank()

root = 0

#Tamano de la muestra
muestra=100

#Este es el promedio de la lista completa
promedio=0

#La Esta variable "sumatoria", es el argumento de la sumatoria de la varianza de la muestra
sumatoria=0
varianza=0
sumas_reajuste=0.0

#Tiempos agregados
tiempo_inicial=0
tiempo_final=0
tiempo_ejecucion=0

#Generamos numeros aleatorios entre 0 y 1, el tamanho de la muestra es 100
l=lista_aleatorios(muestra)

#Calculamos el tamanho total por subdivision de la lista, en el caso de tamanho 100 la subdivision es de 25 
div_tamano = muestra / size

reajuste=0

#Nos ubicamos en el nodo maestro rank=0
if rank == root:
	tiempo_inicial=time()

	#sacamos el promedio total en el nodo maestro
	for i in range(muestra):
		promedio=promedio+l[i]
	promedio=promedio / muestra

#Enviamos el promedio a todos los nodos con bcast
promedio = comm.bcast(promedio, root=root)

#Enviamos la lista generada a todos los nodos (no es necesario ya que la lista la generamos afuera del nodo maestro)
l = comm.bcast(l,root=root)

#Hacemos la sumatoria, pero en forma paralela, definiendo los limites, en el caso de la lista 1, rank=0 recorrera de 0 a 24 tomando esos valores de la lista y restando el promedio total ya calculado. En el rank=1 parte de 25 y llegaria hasta 49 y asi sucesivamente
for i in range(div_tamano*rank, div_tamano*(rank+1)):
	#Seria la resta al cuadrado del valor de esa posicion con el promedio
	sumatoria=((l[i]-promedio)*(l[i]-promedio))+sumatoria

#Aca enviamos todas las sumatorias calculadas en cada nodo al nodo maestro o rank=0
sumatoria = comm.gather(sumatoria, root=root)

if rank == root:
	
	reajuste=muestra-div_tamano*size
	#Sumamos las sumatorias calculadas paralelamente en cada nodo y con esto podemos conocer la varianza
	if reajuste > 0:
		for i in range(muestra-reajuste,muestra):
			sumas_reajuste=((l[i]-promedio)*(l[i]-promedio))+sumas_reajuste
	varianza=sumas_reajuste
	for i in range(size):
		varianza=sumatoria[i]+varianza
	varianza=varianza/muestra
	
	#La desviacion estandar seria la raiz de la varianza
	desviacion_estandar=math.sqrt(varianza)
	tiempo_final=time()
	tiempo_ejecucion=tiempo_final-tiempo_inicial
	print "La varianza es", varianza
	print "La desviacion estandar es", desviacion_estandar
	print "El tiempo de ejecucion es ", tiempo_ejecucion
	print "Desea ver la muestra? (S/N)"
	respuesta=raw_input()
	if respuesta == "S":
		print "La muestra es la siguiente:", l
	elif respuesta == "s":
		print "La muestra es la siguiente:", l
