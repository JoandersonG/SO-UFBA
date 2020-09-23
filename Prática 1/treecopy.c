#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include<stdbool.h>
#include <stdlib.h>
#include "copiarArquivo.c"


struct QuantidadeCopias
{
    int qtdBytes;
    int qtdDiretorios;
    int qtdArquivos;
};

struct QuantidadeCopias copiarDiretorio(char* dirOrigemPath, char* dirDestinoPath) {

    //contador para quantidade de bytes e diretórios
    struct QuantidadeCopias qtdDirAndBytes;
    qtdDirAndBytes.qtdArquivos = 0;
    qtdDirAndBytes.qtdBytes = 0;
    qtdDirAndBytes.qtdDiretorios = 0;

    //abrir diretório origem
    DIR *dirOrigem = opendir(dirOrigemPath);
    if(dirOrigem == NULL) {
        if(errno == EACCES) {
            printf("treecopy: não foi possível abrir o diretório %s. Erro: Permissão negada.\n", dirOrigemPath);
        } else if(errno == ENFILE) {
            printf("treecopy: não foi possível abrir o diretório %s. Erro: Muitos arquivos abertos no sistema no momento.\n", dirOrigemPath);
        } else if(errno == ENOENT) {
            printf("treecopy: não foi possível abrir o diretório %s. Erro: Diretório não existe.\n", dirOrigemPath);
        } else if(errno == ENOMEM) {
            printf("treecopy: não foi possível abrir o diretório %s. Erro: Memória insuficiente para completar a operação.\n", dirOrigemPath);
        } else {
            printf("treecopy: não foi possível abrir o diretório %s. Erro: %s.\n", dirOrigemPath, strerror(errno));
        }

        closedir(dirOrigem);
        exit(EXIT_FAILURE);
    }

    //criar diretório destino
    int mkdirResult = mkdir(dirDestinoPath, S_IRWXG | S_IRWXO | S_IRWXU); //permissions for read, write and execute for everyone
    if (mkdirResult == -1) {

        if(errno == EACCES) {
            printf("treecopy: não foi possível criar o diretório %s. Erro: Permissão de escrita ou busca negada.\n", dirDestinoPath);
        } else if(errno == EEXIST) {
            printf("treecopy: não foi possível criar o diretório %s. Erro: Diretório já existe.\n", dirDestinoPath);
        } else if(errno == ENAMETOOLONG) {
            printf("treecopy: não foi possível criar o diretório %s. Erro: Caminho com nome muito longo.\n", dirDestinoPath);
        } else if(errno == ENOSPC) {
            printf("treecopy: não foi possível criar o diretório %s. Erro: Permissão de escrita ou busca negada.\n", dirDestinoPath);
        } else if(errno == ENOTDIR) {
            printf("treecopy: não foi possível criar o diretório %s. Erro: Um componente usado como diretório não é, de fato, um diretório.\n", dirDestinoPath);
        } else if(errno == ENOENT) {
            printf("treecopy: não foi possível criar o diretório %s. Erro: Diretório especificado não existe.\n", dirDestinoPath);
        } else {
            printf("treecopy: não foi possível criar o diretório %s. Erro: %s.\n", dirDestinoPath, strerror(errno));
        }

        closedir(dirOrigem);
        exit(EXIT_FAILURE);
    }

    //ler o diretório origem
    struct dirent *sd;
    while((sd = readdir(dirOrigem)) != NULL) {

        if(sd == NULL) {
            if(errno == EBADF) {
                printf("treecopy: não foi possível ler o diretório %s. Erro: .\n", dirOrigemPath);
            }
            closedir(dirOrigem);
            exit(EXIT_FAILURE);
        }
    
        //se for '.' ou '..', ignoro pois são os diretórios pai e atual
        bool isDirAtualOuPai = true;
        for(int i = 0; i < strlen(sd->d_name); i++) {
            if(sd->d_name[0] != '.') {
                isDirAtualOuPai = false;
                break;
            }
        }
        if(isDirAtualOuPai) {
            continue; //ignoro '.' ou '..'
        } 

        //se for um arquivo
        if (sd->d_type == 8) {

            //recuperando o caminho do arquivo de origem
            int sizeOfArrayCaminhoArquivoOrigem = (strlen(dirOrigemPath) + strlen(sd->d_name)) + 10;
            char caminhoArquivoOrigem[sizeOfArrayCaminhoArquivoOrigem];
            caminhoArquivoOrigem[0] = '\0';
            strcat(caminhoArquivoOrigem, dirOrigemPath);
            strcat(caminhoArquivoOrigem,"/");
            strcat(caminhoArquivoOrigem,sd->d_name);

            //recuperando o caminho do arquivo de destino
            int sizeOfArrayCaminhoArquivoDestino = (strlen(dirDestinoPath) + strlen(sd->d_name)) + 10;
            char caminhoArquivoDestino[sizeOfArrayCaminhoArquivoDestino];
            caminhoArquivoDestino[0] = '\0';
            strcat(caminhoArquivoDestino, dirDestinoPath);
            strcat(caminhoArquivoDestino,"/");
            strcat(caminhoArquivoDestino,sd->d_name);

            //copiar dados para diretório destino
            int qtdBytes = copiarArquivo(caminhoArquivoOrigem, caminhoArquivoDestino, "treecopy");

            //atualizar contador da quantidade de arquivos copiados
            qtdDirAndBytes.qtdBytes += qtdBytes;
            if(qtdBytes > 0) {//se não copiou o arquivo, então não conta +1 arquivo
                qtdDirAndBytes.qtdArquivos++;
            }
        }

        //se for um diretório
        else if (sd->d_type == 4) {
            
            //recuperar caminho de origem
            int sizeOfArrayCaminhoDiretorioOrigem = (strlen(dirOrigemPath) + strlen(sd->d_name)) + 10;
            char caminhoDiretorioOrigem[sizeOfArrayCaminhoDiretorioOrigem];
            caminhoDiretorioOrigem[0] = '\0';
            strcat(caminhoDiretorioOrigem, dirOrigemPath);
            strcat(caminhoDiretorioOrigem,"/");
            strcat(caminhoDiretorioOrigem,sd->d_name);

            //recuperar caminho de destino
            int sizeOfArrayCaminhoDiretorioDestino = (strlen(dirDestinoPath) + strlen(sd->d_name)) + 10;
            char caminhoDiretorioDestino[sizeOfArrayCaminhoDiretorioDestino];
            caminhoDiretorioDestino[0] = '\0';
            strcat(caminhoDiretorioDestino, dirDestinoPath);
            strcat(caminhoDiretorioDestino,"/");
            strcat(caminhoDiretorioDestino,sd->d_name);

            //copiar recursivamente os itens
            struct QuantidadeCopias qtdCopiasRecursivo = copiarDiretorio(caminhoDiretorioOrigem, caminhoDiretorioDestino);
            qtdDirAndBytes.qtdBytes += qtdCopiasRecursivo.qtdBytes;
            qtdDirAndBytes.qtdDiretorios += qtdCopiasRecursivo.qtdDiretorios;
            qtdDirAndBytes.qtdDiretorios++;
            qtdDirAndBytes.qtdArquivos += qtdCopiasRecursivo.qtdArquivos;
        }

        //se for algo mais
        else {
            printf("treecopy: não foi possível copiar o diretório %s. Erro: Arquivos inválidos presentes no diretório de origem %s.\n", dirOrigemPath);
            closedir(dirOrigem);
            exit(EXIT_FAILURE);
        }

    }

    //fechar diretório
    closedir(dirOrigem);
    
    return qtdDirAndBytes;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("treecopy: Argumentos inválidos!\n Use: treecopy <diretorioorigem> <diretoriodestino>\n");
        exit(EXIT_FAILURE);
    }
    
    char* dirOrigem = argv[1];
    char* dirDestino = argv[2];

    struct QuantidadeCopias qtdCopias = copiarDiretorio(dirOrigem, dirDestino);
    
    //imprimir resultado
    printf("treecopy: foram copiados %d diretórios, %d arquivos e %d bytes de %s para %s.\n", qtdCopias.qtdDiretorios, qtdCopias.qtdArquivos, qtdCopias.qtdBytes, dirOrigem, dirDestino);
    
    return 0;
}