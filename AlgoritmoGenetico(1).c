#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>


int main(void) {
  /* declaracao das variaveis */
  const int mask = 63; // 63 = 0b111111 //Mask criada para fazer o crossing over entre os inteiros
  long long int tam_populacao, tam_melhores, max_geracoes, melhor_solucao = INT_MAX, max_number = 100;
  long long int a, b, c, d, e, f; //Coeficientes da equacao de 5 grau
  double taxa_mutacao;
  

  /*inicializacao das variaveis*/
  printf("Valor do Tamanho da populacao:");
  scanf("%lld", &tam_populacao);

  long long int populacao[tam_populacao]; //valores de   x
  long long int fx[tam_populacao]; //valores de f(x),    sendo x um valor da populacao
  
  tam_melhores = tam_populacao*0.2; //será armazenado no vetor melhores, os 20% melhores
  long long int melhores[tam_melhores];

  printf("\nValor maximo de geracoes:");
  scanf("%lld", &max_geracoes);

  printf("\nValor taxa de mutacao:");
  scanf("%lf", &taxa_mutacao);

  printf("\nValor dos coeficientes da equacao de 5 grau ax^5 + bx^4 + cx^3 + dx^2 + ex + f: ");
  scanf("%lld %lld %lld %lld %lld %lld", &a, &b, &c, &d, &e, &f);
  
  
  srand(time(NULL));
  /*gerar aleatoriamente os valores iniciais da populacao, com um rand que varia de acordo com o tempo*/
  for(int i = 0;i < tam_populacao; i++){
    populacao[i] = (rand()%100) - max_number;
    //ao fazer isso, limitamos o possível valor de rand, para que populacao[i] <= max_number
  }

  /*laco das geracoes*/
  for(int i = 0; i < max_geracoes ; i++){ 

    /*avalia populacao para seu problema */

    for(int j = 0; j < tam_populacao; j++){
      long long int x = populacao[j];
      
      fx[j] = a*pow(x,5) + b*pow(x,4) + c*pow(x,3) +         d*pow(x,2) + e*x + f; //valor de fitness da            populacao. Quanto mais próximo de 0, melhor
      
      if(llabs(fx[j]) < melhor_solucao){ //se o atual         f(x) é mais próximo do que a antiga melhor             solucao, armazena ele.
        melhor_solucao = llabs(fx[j]); 
      }
    }

    //sort dos valores da populacao e f(x), a fim de deixar os x com f(x) mas próximo de 0 no inicio da array
    for(int j = 0; j < tam_populacao; j++){
      for(int w = j + 1; w < tam_populacao; w++){
        if(llabs(fx[w]) < llabs(fx[j])){
          int tmp = populacao[w];
          populacao[w] = populacao[j];
          populacao[j] = tmp;
          tmp = fx[w];
          fx[w] = fx[j];
          fx[j] = tmp;
        }
      }
    }
    /*verificar se a melhor solucao esta aceitavel (se sim, encerrar o laco)*/
    if (llabs(melhor_solucao) <= 5) break; 
    
    /*selecionar os melhores individuos*/
    for(int j = 0; j < tam_melhores; j++){ //pega os melhores 20% da populacao e troca com os 20% piores;
      /*melhores[j] = populacao[j];*/
      populacao[tam_populacao - (j+1)] = populacao[j];
      fx[tam_populacao - (j+1)] = fx[j];
    }
    /*aplicar o cruzamento entre os 80% piores da populacao */

    //esq representa o melhor valor entre os 80%, e o dir representa o pior valor entre os 80%
    int esq = tam_melhores, dir = tam_populacao - 1;

    //Representam a primeira e segunda metade dos dois inteiros que estão sendo cruzados
    int tmp_front[2];
    int tmp_back[2];

    while (dir > esq){ 
      long long int x1, x2;

      tmp_front[0] = populacao[esq] & ~mask;
      tmp_front[1] = populacao[dir] & ~mask;
      tmp_back[0] = populacao[esq] & mask;
      tmp_back[1] = populacao[dir] & mask;

      populacao[esq] = tmp_front[0] | tmp_back[1];
      populacao[dir] = tmp_front[1] | tmp_back[0];

      x1 = populacao[esq], x2 = populacao[dir];

      //atualiza os valores de f(x) de acordo com os novos valores pós crossing-over
      fx[esq] = a*pow(x1,5) + b*pow(x1,4) + c*pow(x1,3) + d*pow(x1,2) + e*x1 + f; 
      fx[dir] = a*pow(x2,5) + b*pow(x2,4) + c*pow(x2,3) + d*pow(x2,2) + e*x2 + f; 
    
      esq++;
      dir--;
    }
  
    /*aplicar mutacao*/  
    for(int j = 0; j < tam_populacao; j++){

      double r = ((double) rand() / (RAND_MAX)); //           gera um valor entre 0 e 1

      if(r <= taxa_mutacao){ // a mutacao ocorre se o           r se encontra no intervalo [0, taxa_mutacao]

        int mutation = 1; // 1 = 0b1 
        int tmp = ((int) rand()%6) + 1; // shift de 1          a 6
        mutation = mutation << tmp;
        populacao[j] = populacao[j] ^ mutation;
      
        //atualizando o valor f(populacao[j])
        long long int x = populacao[j];
        fx[j] = a*pow(x,5) + b*pow(x,4) + c*pow(x,3) + d*pow(x,2) + e*x + f; 
      }
    }

    //sort para deixar novamente os melhores no inicio     do vetor
    for(int j = 0; j < tam_populacao; j++){
      for(int w = j + 1; w < tam_populacao; w++){
        if(llabs(fx[w]) < llabs(fx[j])){
          int tmp = populacao[w];
          populacao[w] = populacao[j];
          populacao[j] = tmp;
          tmp = fx[w];
          fx[w] = fx[j];
          fx[j] = tmp;
        }
      }
    }
  }

  /*imprimir a melhor solucao*/
  if(melhor_solucao == 0){ //caso uma raiz seja         encontrada
    printf("Solucao encontrada:\n");
  }else{
    printf("Nao foi encontrada a solucao, a solucao        mais proxima foi:\n");
  }
  printf("Valor de x:%lld\nValor de f(x):%lld \n",       populacao[0], melhor_solucao);

  return 0;
}