from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

if rank == 0:
    n = 1000*size
    rand = np.random.uniform(0,1,n)
    data = np.array_split(rand, size)
else:
    data = None
data_chunck = comm.scatter(data, root=0)

avg = np.mean(data_chunck)

avg = comm.gather(avg, root=0)

if rank == 0:
    avg = np.array(avg)
    print "gather es: ", avg
    print "el promedio de los promedios es: ", np.mean(avg)
