# -*- coding: utf-8 -*-
"""
Created on Thu Aug 28 15:23:59 2014

@author: paralela
"""

import time
starting_point=time.time() 



#Inicio codigo
from mpi4py import MPI

comm = MPI.COMM_WORLD  # comunicador entre dos procesadores #

rank =  comm.rank     # id procesador actual #
size =  comm.size     # tamaño procesador #


lista = []

if rank== 0 :
    lista = range(size) # genera lista de nodos tamaño size #

#procesador 0#

v= comm.scatter(lista,0) 
# como la lista es igual a la cantidad de procesadores , 
                         # asigna a cada procesador un numero , en el nodo 0 #
v=v*v 

variable_de_vuelta = comm.gather(v,0) # toma un elemento y lo deja en el nodo maestro#
suma = comm.reduce(v,op=MPI.SUM,root=0) # reduccion con la funcion suma, suma todos los v y los  manda al nodo maestro#
#fin procesador 0#
if rank ==0:
    lista = variable_de_vuelta
    print lista, 
    " y la suma es",suma
    



#Termino codigo    
    
if rank==0:
    elapsed_time=time.time()-starting_point
    elapsed_time_int = int(elapsed_time)
    elapsed_time_minutes = elapsed_time_int/60
    elapsed_time_seconds = elapsed_time_int%60
    print "Time: "+ str(elapsed_time_minutes) + ":" + str(elapsed_time_seconds)