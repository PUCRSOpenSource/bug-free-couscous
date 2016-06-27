#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 40
#define COLUMNS 10
#define WORKTAG 1
#define DIETAG 2

int vet[ROWS][COLUMNS];

int
compare (const void* a, const void* b)
{
  return *((const int*) a) - *((const int*) b);
}

int
master (void)
{
  double t1,t2;
  t1 = MPI_Wtime();

  int proc_n;
  int rank;
  int work = 0;

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  //Populate the matrix
  int i, j, k;
  for (i = 0; i < ROWS; i++)
    {
      k = COLUMNS;
      for (j = 0; j < COLUMNS; j++)
        {
          vet[i][j] = k;
          k--;
        }
    }

  printf("Starting seeding the slaves\n");

  //Seed the slaves
  for (rank = 1; rank < proc_n; rank++)
    {
      printf("Sending to rank %d\n", rank);
      MPI_Send(vet[work], COLUMNS * ROWS / (proc_n - 1), MPI_INT, rank, WORKTAG, MPI_COMM_WORLD);
      printf("Sent to rank %d\n", rank);
      work += ROWS / (proc_n - 1);
      printf("work is %d\n", work);
    }

  printf("Finished seeding the slaves\n");

  //Receive a result from any slave and dispatch a new work request
  int save_path = 0;
  i = 1;
  while (i < proc_n)
    {
      /*MPI_Recv(vet[save_path], COLUMNS, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);*/
      MPI_Recv(vet[save_path], COLUMNS * ROWS / (proc_n - 1), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      /*MPI_Send(vet[work], COLUMNS, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);*/
      printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHH\n");
      MPI_Send(0, 0, MPI_INT, status.MPI_SOURCE, DIETAG, MPI_COMM_WORLD);
      printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOHHHHHHHHHHHH\n");
      /*work++;*/
      save_path = ROWS / (proc_n - 1);
      /*save_path++;*/
      i++;
      /*printf("Acabei de mandar o processo %d se matar", status.MPI_SOURCE);*/
    }

  //Receive last results
  /*for (rank = 1; rank < proc_n; rank++)*/
    /*{*/
      /*MPI_Recv(vet[save_path], COLUMNS, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);*/
      /*save_path++;*/
    /*}*/

  //Kill all the slaves
  /*for (rank = 1; rank < proc_n; rank++)*/
    /*{*/
      /*MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);*/
    /*}*/

  t2 = MPI_Wtime();
  fprintf(stderr, "Time: %fs\n\n", t2-t1);

  return 0;
}

int
slave (void)
{
  int proc_n;

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  int* work = malloc((COLUMNS * ROWS / (proc_n - 1)) * sizeof(int));
  int* beginning = work;

  //Receive and work until it dies
  while (1)
    {
      MPI_Recv(beginning, COLUMNS * ROWS / (proc_n - 1), MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if (status.MPI_TAG == DIETAG)
        {
          printf("MEU MESTRE MANDOU EU ME MATAR, SOCORR\n");
          free(work);
          return 0;
        }

      int i;
      for (i = 0; i < ROWS / (proc_n - 1); i++)
	{
	  printf("Gonna quicksort it\n");
	  qsort(work += COLUMNS, COLUMNS, sizeof(int), compare);
	  /*work += COLUMNS;*/
	  printf(" and work is gonna be %d\n", work);
	}

      printf("Begginning is %d work is %d and their difference is %d\n", beginning, work, work - beginning);
      MPI_Send(beginning, COLUMNS * ROWS / (proc_n - 1), MPI_INT, 0, 0, MPI_COMM_WORLD);
      printf("ENVIEI PRO MEU MESTRE\n");
    }

  return 1;
}

int
main (int argc, char** argv)
{
  int my_rank;
  int proc_n;

  MPI_Init(&argc , &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  if ( my_rank == 0 )
    master();
  else
    slave();

  MPI_Finalize();

  return 0;
}
