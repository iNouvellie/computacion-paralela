from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_rank()
rank = comm.Get_rank()

root = 0

buf = 0
if rank == root:
    buf = 777

print "[%d]: Before Bcast, buf is %d\n"%(rank, buf)
buf = comm.bcast(buf, root=root)
print "[%d]: After Bcast, buf is %d\n"%(rank, buf)
