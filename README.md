# bug-free-couscous

* [Giovanni Cupertino](http://www.github.com/GiovanniCuper)
* [Matthias Nunes](http://www.github.com/execb5)

---

# Enunciado 3o Trabalho: MPI & OpenMP

O objetivo do trabalho é implementar uma versão paralela híbrida da ordenação de
vários vetores em um saco de trabalho (vetor de vetores) usando as ferramentas
MPI e OpenMP.

Para a exploração do paralelismo sem memória compartilhada deverá ser utilizada
uma versão adaptada do modelo mestre/escravo implementado com MPI no primeiro
trabalho, com o propósito de distribuir os vetores a serem ordenados nos
diferentes nós dos cluster. Pra isso será usado apenas um processo pesado MPI
por nó. A ordem original dos vetores no saco deve ser mantida.

Para a exploração do paralelismo com memória compartilhada dentro de cada nó
deverá ser implementada uma ordenação de vetores seguindo o modelo workpool em
OpenMP (todas as threads ordenam e o controle é feito por uma estrutura de
dados, sem mestre).  Deve ser utilizada pelo menos uma thread para cada núcleo
do nó e o programa deve receber como entrada o algoritmo de ordenação a ser
utilizado (quick sort - função da linguagem C - ou o bubble sort - código
abaixo).

Após implementado, o programa deve ser executado em quatro nós do cluster
atlantica na ordenação de 10.000 vetores de 100.000 elementos. Cada grupo (de
dois integrantes) deve entregar um relatório em .pdf de uma página com a análise
dos resultados e uma página com o código (seguir modelo proposto).

Os itens para avaliação são:

- foco na análise dos resultados se utilizando das métricas de speed-up e
  eficiência
- cálculo do speed up e da eficiência para o caso de teste e número de
  processadores;
- análise das diferentes formas de alocar o processo mestre do MPI
- comparação do fator de aceleração (speed-up) entre os diferentes algoritmos de
  ordenação (quick sort e bubble sort)
- comparação do fator de aceleração (speed-up) da versão MPI pura (modelo
  mestre/escravo) e justificativa de ter ficado melhor/pior
- análise do número de horas máquina usadas pelo grupo no LAD durante o
  desenvolvimento do trabalho (feita pelo professor).

```c
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define BAG_SIZE   7       // trabalho final com o valor 14
#define ARRAY_SIZE 10      // trabalho final com o valor 10.000

void bs(int n, int * vetor)
{
    int c =0, d, troca, trocou =1;

    while (c < (n-1) & trocou )
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

int main()
{
    int work_bag[BAG_SIZE][ARRAY_SIZE];
    int i,j;

    for (i=0 ; i<BAG_SIZE; i++)              /* init work bag */
        for (j=0 ; j<ARRAY_SIZE; j++)
            {
            if (i%2 == 0)
                work_bag[i][j] = ARRAY_SIZE-j;
            else
                work_bag[i][j] = j+1;
            }

    for (i=0 ; i<BAG_SIZE; i++)
        {
        #ifdef DEBUG
        printf("Vetor [%02d]: ", i);
        #endif
        bs(ARRAY_SIZE, &work_bag[i][0]);
        }

    return 0;
}
```
