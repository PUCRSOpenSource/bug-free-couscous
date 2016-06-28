#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

/*#define DEBUG 1*/
/*#define BUBBLE 1*/
#define THREADS 16

#define ROWS 10000
#define COLUMNS 100000
#define WORKTAG 1
#define DIETAG 2
#define CHUNK 8

int vet[ROWS][COLUMNS];

void
bs (int n, int * vetor)
{
  int c =0, d, troca, trocou =1;

  while ((c < (n-1)) & trocou )
    {
      trocou = 0;
      for (d = 0 ; d < n - c - 1; d++)
        if (vetor[d] > vetor[d+1])
          {
            troca      = vetor[d];
            vetor[d]   = vetor[d+1];
            vetor[d+1] = troca;
            trocou = 1;
          }
      c++;
    }

#ifdef DEBUG
  for ( c = 0 ; c < n ; c++ )
    printf("%03d ", vetor[c]);
  printf("\n");
#endif
}


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
  int sent = 0;
  int received = 0;
  while ( sent < ROWS )
    {
      for (rank = 1; rank < proc_n && sent < ROWS; rank++)
        {
          MPI_Send(vet[sent], CHUNK * COLUMNS, MPI_INT, rank, WORKTAG, MPI_COMM_WORLD);
          sent += CHUNK;
        }
      for (rank = 1; rank < proc_n && received < ROWS; rank++)
        {
          MPI_Recv(vet[received], CHUNK * COLUMNS, MPI_INT, rank, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          received += CHUNK;
        }
    }

  //Kill the slaves
  i = 1;
  while (i < proc_n)
    {
      MPI_Send(0, 0, MPI_INT, i++, DIETAG, MPI_COMM_WORLD);
    }


  t2 = MPI_Wtime();
  fprintf(stderr, "Time: %fs\n\n", t2-t1);

  return 0;
}

int
slave (void)
{
  int proc_n;
  int my_rank;

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int work[CHUNK][COLUMNS];

  //Receive and work until it dies
  while (1)
    {
      MPI_Recv(work, CHUNK * COLUMNS, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if (status.MPI_TAG == DIETAG)
        {
          return 0;
        }

      int i;
      omp_set_num_threads(THREADS);
#pragma omp parallel for
      for (i = 0; i < CHUNK; i++)
        {
#ifdef BUBBLE
          bs(COLUMNS, work[i]);
#else
          qsort(work[i], COLUMNS, sizeof(int), compare);
#endif
        }

      MPI_Send(work, CHUNK * COLUMNS, MPI_INT, 0, 0, MPI_COMM_WORLD);
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
