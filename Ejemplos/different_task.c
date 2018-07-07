#include <mpi.h>
int main(int argc, char **argv)
   {
      int my_id, root_process, ierr, num_procs;
      MPI_Status status;

      ierr = MPI_Init(&argc, &argv); 
      ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
      ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

      if( my_id == 0 ) {

         /* do some work as process 0 */
      }
      else if( my_id == 1 ) {

         /* do some work as process 1 */
      }
      else if( my_id == 2 ) {

         /* do some work as process 2 */ 
      } 
      else {

         /* do this work in any remaining processes */
      }
      /* Stop this process */

      ierr = MPI_Finalize();

      return 0;
   }
