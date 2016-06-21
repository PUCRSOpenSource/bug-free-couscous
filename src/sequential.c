#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 1000
#define COLUMNS 100000

int vet[ROWS][COLUMNS];

int
compare (const void* a, const void* b)
{
  return *((const int*) a) - *((const int*) b);
}

int
main (int argc, const char* argv[])
{
  time_t start, stop;

  start = clock();

  int i, j, k = COLUMNS;
  for (i = 0; i < ROWS; i++)
    {
      for (j = 0; j < COLUMNS; j++)
        {
          vet[i][j] = k;
          k--;
        }
      k = COLUMNS;
    }

  for (i = 0; i < ROWS; i++)
    {
      qsort(vet[i], COLUMNS, sizeof(int), compare);
    }

  stop = clock();

  float diff = ((float)(stop - start) / 1000000.F) * 1000;
  printf("Time: %.0fms\n\n", diff);

  return 0;
}
