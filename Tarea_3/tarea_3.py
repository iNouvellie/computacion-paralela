from mpi4py import MPI
import numpy as np
from time import time

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()


tiempo_inicial = time()
root=0
varianza=0.0
varianza_total=0.0
media=0
promedio_lista=0
if rank == root:

	#Tamano de la muestra
	n=1000000

	#no viene de la libreria numpy, genera una lista de numeros random y uniformes de tamano n -muestra-	
	rand = np.random.uniform(0,1,n)

	#np.mean saca el promedio de la lista rand creada anteriormente
	promedio_lista=np.mean(rand)

	#subdividimos las listas en listas mas pequenas, para enviarlas por scatter
	data = np.array_split(rand, size)
else:
	#este caso deja vacia nula la subdivision de las listas
	data=None

#aca distribuimos los arreglos a los distintos nodos
data_distribuida = comm.scatter(data, root=root)

#Les entregamos la informacion del promedio de la lista general a los nodos, para que puedan calcular la varianza
promedio_lista=comm.bcast(promedio_lista, root=root)

#Sacamos la varianza parcial de las listas, len nos sirve para determinar el largo de nuestro arreglo, ya que el tamano lo genera el array_split
for i in range(len(data_distribuida)):
	varianza=((promedio_lista-data_distribuida[i])**2)+varianza

varianza = comm.gather(varianza, root=root)

if rank == root:
	#Sumamos las varianzas parciales y llegando a nuesto resultado
	for i in range(size):
		varianza_total=varianza[i]+varianza_total
	varianza_total=varianza_total/n
	tiempo_final=time()
	tiempo_ejecucion = tiempo_final - tiempo_inicial
	print "La varianza es", varianza_total
	print "La media es", promedio_lista
	print "Tiempo de ejecucion", tiempo_ejecucion
