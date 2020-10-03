#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//método que calcula e exibe todos os primos de 2 até attr
void *calculaPrimo(void* attr) {

    int numMaxTestar = *((int *) attr);
    
    //agora crio o array com os valores até o topo
    int numParaTestar[numMaxTestar+1];
    for (int i = 0; i < numMaxTestar+1; i++)
    {
        numParaTestar[i] = i;
    }


    numParaTestar[1] = 0;
    for (int i = 2; i < sqrt( (double) numMaxTestar); i++)
    {
        if ( i == 0) continue; //ignoro caso não haja número na posição

        //removo todos os múltiplos de i
        for (int j = i + 1; j < numMaxTestar + 1; j++)//+1?
        {
            if (numParaTestar[j] % i == 0) { // é múltiplo de i, então não é primo
                numParaTestar[j] = 0;// removo
            }
        }        
    }
    
    //imprimindo no terminal os primos
    if (numMaxTestar >= 2) {
        //imprimindo o primeiro primo (sem a vírgula)
        printf("2");
    }
    for (int i = 3; i <= numMaxTestar; i++)
    {
        if ( numParaTestar[i] != 0) {
            printf(", %d", numParaTestar[i]);
        }
    }
    printf("\n");

    return NULL;
}

int main(int argc, char * argv[]) {
    
    //testa se entrada é válida
    if (argc < 2) {
        printf("Erro: informe um número para calcular seu primo.\n");
        exit(1);
    }
    int numMaxTestar = atoi(argv[1]);

    //identificador da thread que calculará os primos.
    pthread_t thread1;
    int errCode = pthread_create(&thread1, NULL, calculaPrimo, &numMaxTestar);

    if (errCode != 0) {//erro
        
    }

    //espero a finalização do thread filho.
    int errThreadJoin = pthread_join(thread1, NULL);

    if (errThreadJoin != 0) {
        
    }

    return 0;
}