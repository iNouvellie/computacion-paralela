program hello_world
include "/usr/local/include/mpif.h"
include "/usr/local/include/mpif-config.h"
integer ierr

call MPI_INIT ( ierr )
print *, "Hello world"
call MPI_FINALIZE ( ierr )

stop
end