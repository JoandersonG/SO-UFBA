#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int copiarArquivo(char* arquivoOrigem, char* arquivoDestino, char * programaInvocador) {

    //abrir arquivo de origem
    int fdOrigem = open(arquivoOrigem, O_RDONLY);
    if (fdOrigem == -1) {
        if(errno == EPERM) {
            printf("%s: não foi possível abrir o arquivo %s. Erro: Operação não permitida.\n", programaInvocador, arquivoOrigem);
        } else if (errno == ENOENT) {
            printf("%s: não foi possível abrir o arquivo %s. Erro: Arquivo ou diretório não encontrado.\n", programaInvocador, arquivoOrigem);
        }
        else if (errno == EACCES) {
            printf("%s: não foi possível abrir o arquivo %s. Erro: Acesso negado.\n", programaInvocador, arquivoOrigem);
        }else if (errno == EAGAIN) {
            printf("%s: não foi possível abrir o arquivo %s. Tente novamente.\n", programaInvocador, arquivoOrigem);
        } else {
            printf("%s: não foi possível abrir o arquivo %s: %s.\n", programaInvocador, arquivoOrigem, strerror(errno));
        }
        close(fdOrigem);
        exit(EXIT_FAILURE);
    }

    //criar arquivo destino:

    //flags
    //O_WRONLY: apenas escrever no arquivo
    //O_CREAT: criar se arquivo não existe
    //O_EXCL: associado ao O_CREAT, impede a criação do arquivo novo (pois isso pode dar problema ao copiar, caso o destino tenha mais dados que a origem)
    //S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH: são flags para permitir leitura e escrita do novo arquivo por qualquer um
    int fdDestino = open(arquivoDestino, O_WRONLY| O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fdDestino == -1) {
        if(errno == EEXIST) {
        printf("%s: não foi possível criar o arquivo %s. Erro: o arquivo especificado já existe.\n", programaInvocador, arquivoDestino);
        } else if (errno == ENOENT) {
            printf("%s: não foi possível criar o arquivo %s. Erro: Diretório não encontrado.\n", programaInvocador, arquivoDestino);
        } else if (errno == EACCES) {
            printf("%s: não foi possível criar o arquivo %s. Erro: Acesso negado.\n", programaInvocador, arquivoDestino);
        } else if (errno == EAGAIN) {
            printf("%s: não foi possível criar o arquivo %s. Tente novamente.\n", programaInvocador, arquivoDestino);
        }else {
            printf("%s: não foi possível criar o arquivo %s. %s.\n", programaInvocador, arquivoDestino, strerror(errno));
        }
        close(fdOrigem);
        exit(EXIT_FAILURE);
    }

    //declarando buffer
    int TAMANHO_BUFFER = 4096;//4KB
    char* bufferConteudoOrigem = (char*) calloc(TAMANHO_BUFFER, sizeof(char));

    int qtdBytesLidos = 0;
    int qtdTotalBytesEscritos = 0;
    do {

        //ler arquivo de origem:
        qtdBytesLidos = read(fdOrigem, bufferConteudoOrigem, TAMANHO_BUFFER);
        if(qtdBytesLidos == -1) {
            if(errno == EPERM) {
            printf("%s: não foi possível ler o arquivo %s. Erro: Operação não permitida.\n", programaInvocador, arquivoOrigem);
            } else if (errno == EACCES) {
                printf("%s: não foi possível ler o arquivo %s. Erro: Acesso negado.\n", programaInvocador, arquivoOrigem);
            } else if (errno == EAGAIN) {
                printf("%s: não foi possível ler o arquivo %s. Tente novamente.\n", programaInvocador, arquivoOrigem);
            } else if (errno == EISDIR) {
                printf("%s: não foi possível ler o arquivo %s. Erro: É um diretório.\n", programaInvocador, arquivoOrigem);
            } else {
                printf("%s: não foi possível ler o arquivo %s: %s.\n", programaInvocador, arquivoOrigem, strerror(errno));
            }

            close(fdOrigem);
            close(fdDestino);
            exit(EXIT_FAILURE);
        }

        //se dados foram lido:
        if(qtdBytesLidos > 0) {

            //escrever no arquivo de destino:
            int qtdBytesEscrita = write(fdDestino, bufferConteudoOrigem, strlen(bufferConteudoOrigem));
            if(qtdBytesEscrita  == -1) {
                if(errno == EPERM) {
                printf("%s: não foi possível escrever no arquivo %s. Erro: Operação não permitida.\n", programaInvocador, arquivoDestino);
                }  else if (errno == EAGAIN) {
                    printf("%s: não foi possível escrever o arquivo %s. Tente novamente.\n", programaInvocador, arquivoDestino);
                }   else if (errno == ENOMEM) {
                    printf("%s: não foi possível escrever o arquivo %s. Erro: memória insuficiente.\n", programaInvocador, arquivoDestino);
                } else {
                    printf("%s: não foi possível escrever o arquivo %s: %s.\n", programaInvocador, arquivoDestino, strerror(errno));
                }

                //liberar memória de buffer
                free(bufferConteudoOrigem);
                close(fdOrigem);
                close(fdDestino);
                exit(EXIT_FAILURE);
            }
            qtdTotalBytesEscritos += qtdBytesEscrita;
        }
        
        //limpar dados do buffer
        memset(bufferConteudoOrigem, 0x00, TAMANHO_BUFFER);

    } while (qtdBytesLidos > 0);
    
    
    //liberar memória de buffer
    free(bufferConteudoOrigem);

    //fechar aqruivos:
    close(fdOrigem);
    close(fdDestino);

    return qtdTotalBytesEscritos;
}