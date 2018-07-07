from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

mean = None
lp = l1 = l2 = l3 = None

if rank == 0:
    l = [1.0, 4.0, 5.0, 20.0, 3.0, 2.0, 0.0, 1.0, 1.0]
    l1 = l[0:3]
    l2 = l[3:6]
    l3 = l[6:9]
    
    comm.send(l1, dest=1)
    comm.send(l2, dest=2)
    comm.send(l3, dest=3)

if rank == 1:
    lp = comm.recv(l1, source=0)
    
    mean = (lp[0] + lp[1] + lp[2]) / 3
    
    comm.send(mean, dest=0)
    
if rank == 2:
    lp = comm.recv(l2, source=0)
    
    mean = (lp[0] + lp[1] + lp[2]) / 3
    
    comm.send(mean, dest=0)
    
if rank == 3:
    lp = comm.recv(l3, source=0)
    
    mean = (lp[0] + lp[1] + lp[2]) / 3
    
    comm.send(mean, dest=0)

if rank == 0:
    
    mean0 = comm.recv(mean, source=1)
    mean1 = comm.recv(mean, source=2)
    mean2 = comm.recv(mean, source=3)
    
    mean_result = (mean0 + mean1 + mean2)/3
    print mean_result