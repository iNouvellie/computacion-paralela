# -*- coding: utf-8 -*-
"""
Created on Thu Aug 28 15:23:59 2014

@author: paralela
"""

import time
starting_point=time.time() 


from mpi4py import MPI

comm = MPI.COMM_WORLD  # comunicador entre dos procesadores #

rank =  comm.rank     # id procesador actual #
size =  comm.size     # tamaño procesador #

if rank == 0:
    #Mandar a cada procesador un número
    for i in range(size):
        comm.send(i,dest=i)

data=comm.recv(source=0)
data=data+rank
comm.send(data,dest=0)


if rank==0:
    rec=[]
    for i in range(size):
        r=comm.recv(source=i)
        rec.append(r)
    print "He recibido el ", rec
    
    

    elapsed_time=time.time()-starting_point
    elapsed_time_int = int(elapsed_time)
    elapsed_time_minutes = elapsed_time_int/60
    elapsed_time_seconds = elapsed_time_int%60
    print "Time: "+ str(elapsed_time_minutes) + ":" + str(elapsed_time_seconds)