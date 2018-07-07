from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

if rank == 0:
    data = [(i+1)**2 for i in range(size)]
    print data
else:
    data = None
d = comm.scatter(data, root=0)

d += 1

data = comm.gather(d, root=0)

if rank == 0:
    print data

