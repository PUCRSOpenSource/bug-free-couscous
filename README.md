# super-octo-goggles

O objetivo do trabalho é implementar, usando a biblioteca MPI, uma versão
paralela seguindo o modelo mestre escravo, de um programa que ordena vários
vetores usando o algortimo Quick Sort (usar a versão padrão do C qsort()). Após
implementado, o programa deve ser executado no cluster atlântica com 2 nós (16 e
32 processadores no total - com e sem HT) para realização das medições de
desempenho com 1000 vetores no saco de trabalho, cada um com 100.000 elementos
(sem os prints de tela). Cada grupo (de dois integrantes) deve entregar um
relatório em .pdf de uma página com a análise dos resultados e uma página com o
código (seguir modelo proposto).

O mestre ficará responsável pela gerência do saco de trabalho (uma matriz),
distribuindo os vetores para os escravos e recebendo de volta os vetores
ordenados e guardando-os novamente no saco (a recepção deve ser por ordem de
chegada, ou seja, de quem ficar pronto primeiro). A ordem original dos vetores
no saco deve ser mantida. Os escravos por sua vez, receberão os vetores a serem
ordenados, realizarão a ordenação com o algoritmo fornecido e retornarão o vetor
ordenada para o mestre. A iniciativa deve ser dos escravos, ou seja, o s
escravos pedem trabalho ao mestre que vai atendendo estas demandas até que o
saco esteja vazio. Neste momento são enviadas mensagens de suicídio aos
escravos. O programa SPMD deve funcionar para qualquer numero de escravos para
permitir uma análise da curva do Speed-UP e Eficiência.

![Image](https://github.com/execb5/super-octo-goggles/raw/master/data/MS.gif)

Figura 1: Funcionamento do modelo mestre escravo na ordenação de vários vetores
(saco de trabalho)

Os itens para avaliação são:

* execução da versão sequencial;
* implementação da versão paralela SPMD do algoritmo em C e MPI seguindo o
	modelo mestre/escravo;
* medição dos tempos de execução para a versão sequencial em uma máquina
	qualquer do aluno ou laboratório e da versão paralela (usando 2 nós exclusivos
	da máquina gates totalizando 16 e 32 processos - cada nó possui 8
	processadores capazes de executar 16 threads);
* cálculo do speed up e da eficiência para o caso de teste e diferentes números
	de processadores;
* análise do balanceamento da carga na execução do programa paralelo;
* clareza do código (utilização de comentários e nomes de variáveis adequadas);
* relatório no formato .pdf com duas páginas (coluna dupla), um apara a análise
	dos resultados e outra para o código, seguindo as recomendações fornecidas no
	moodle (submissão em sala de entrega moodle até as 17:30 do dia da entrega -
	duas semanas);
* análise do número de horas máquina usadas pelo grupo no LAD durante o
	desenvolvimento do trabalho.

	Last modified: Tuesday, 5 April 2016, 5:10 PM
