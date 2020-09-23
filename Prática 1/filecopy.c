#include "copiarArquivo.c"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("filecopy: Argumentos inválidos!\n Use: filecopy <arquivoorigem> <arquivodestino>\n");
        return 1;
    }
    
    char* arquivoOrigem = argv[1];
    char* arquivoDestino = argv[2];

    //imprimir sucesso
    int qtdTotalBytesEscritos = copiarArquivo(arquivoOrigem, arquivoDestino, "filecopy");
    
    printf("filecopy: foram copiados %d bytes do arquivo %s para o arquivo %s\n", qtdTotalBytesEscritos, arquivoOrigem, arquivoDestino);
    
    return 0;
}