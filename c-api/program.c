#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/data-file.h"
#include "src/utils/data-utils.h"
#include "src/utils/cabecalho-utils.h"

const char* OP_NAME[] = {
    "create",
    "select",
    "modify",
    "insert",
    "delete",
};

const char* OP_DESC[] = {
    "Cria um novo arquivo (de dados ou de índice)",
    "Seleciona registros de um arquivo de dados com base em um critério",
    "Modifica um registro em um arquivo de dados",
    "Insere um registro em um arquivo de dados",
    "Remove registros de um arquivo de dados com base em um critério"
};

static int help_print_op(){
    printf("=================== HELP GUIDE ===================\n");
    for(int i = 0; i < 4; i++){
        printf("%s\t\t-- %s\n", OP_NAME[i], OP_DESC[i]);
    }

    return 0;
}

// TODO: Fazer interação direta dos comandos, somente o select e create são chamados com frequência e devem dar uma saída imediata,
// o restante só é chamado no final da execução
int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr, "Argumentos insuficientes!\n");
        return EXIT_FAILURE;
    }

    char* operation = argv[1];

    if(strcmp(operation, "help") == 0){
        help_print_op();
    }
    else if(strcmp(operation, "select") == 0){
        if(argc < 3){
            fprintf(stderr, "Argumentos insuficientes!\n");
            return EXIT_FAILURE;
        }

        char* data_filename = argv[2];
        FILE* data_fptr;

        if(!(data_fptr = fopen(data_filename, "rb")) || !check_status(data_fptr)){
            perror("Erro ao abrir o arquivo: ");
            return EXIT_FAILURE;
        }

        JOGADOR j_query = read_query();

        select_data(data_fptr, j_query);

        fclose(data_fptr);
    }
    else if(strcmp(operation, "delete")){
        if(argc < 4){
            fprintf(stderr, "Argumentos insuficientes!\n");
            return EXIT_FAILURE;
        }

        char* data_filename = argv[2];
        char* index_filename = argv[3];
        FILE* data_fptr, *index_fptr;

        if(!(data_fptr = fopen(data_filename, "r+b")) || !check_status(data_fptr) ||
        !(index_fptr = fopen(index_filename, "w"))){
            perror("Erro ao abrir o arquivo: ");
            return EXIT_FAILURE;
        }

        // JOGADOR j_query = read_query();
    }
    else{
        fprintf(stderr, "Operação %s inválida\n", operation);
    }

    return EXIT_SUCCESS;
}