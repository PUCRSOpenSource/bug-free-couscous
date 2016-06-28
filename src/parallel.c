#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 10000
#define COLUMNS 100000
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

  //Seed the slaves
  for (rank = 1; rank < proc_n; rank++)
    {
      MPI_Send(vet[work], COLUMNS * (ROWS / (proc_n - 1)), MPI_INT, rank, WORKTAG, MPI_COMM_WORLD);
      work += ROWS / (proc_n - 1);
    }

  //Receive a result from any slave and dispatch a new work request
  int save_path = 0;
  i = 1;
  while (i < proc_n)
    {
      /*MPI_Recv(vet[save_path], COLUMNS * (ROWS / (proc_n - 1)), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);*/
      /*MPI_Send(0, 0, MPI_INT, status.MPI_SOURCE, DIETAG, MPI_COMM_WORLD);*/
      MPI_Recv(vet[save_path], COLUMNS * (ROWS / (proc_n - 1)), MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Send(0, 0, MPI_INT, i, DIETAG, MPI_COMM_WORLD);
      save_path += ROWS / (proc_n - 1);
      i++;
    }


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

  /*int** work = malloc((COLUMNS * ROWS / (proc_n - 1)) * sizeof(int));*/
  int work[ROWS / (proc_n - 1)][COLUMNS];

  //Receive and work until it dies
  while (1)
    {
      MPI_Recv(work, COLUMNS * (ROWS / (proc_n - 1)), MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if (status.MPI_TAG == DIETAG)
        {
          return 0;
        }


      int i;
#pragma omp parallel for
      for (i = 0; i < ROWS / (proc_n - 1); i++)
        {
          qsort(work[i], COLUMNS, sizeof(int), compare);
        }
#pragma omp barrier

      MPI_Send(work, COLUMNS * (ROWS / (proc_n - 1)), MPI_INT, 0, 0, MPI_COMM_WORLD);
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
