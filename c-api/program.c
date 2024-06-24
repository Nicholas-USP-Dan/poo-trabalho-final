#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "src/adts/dyn-array.h"
#include "src/index-file.h"
#include "src/data-file.h"
#include "src/utils/data-utils.h"
#include "src/utils/cabecalho-utils.h"
#include "src/utils/removed-list.h"

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
    else if(strcmp(operation, "create") == 0){
        if(argc < 5){
            fprintf(stderr, "Argumentos insuficientes!\n");
            return EXIT_FAILURE;
        }

        char *csv_filename = argv[2];
        char *data_filename = argv[3];
        char *index_filename = argv[4];

        FILE *csv_fptr, *data_fptr, *index_fptr;
        if(!(csv_fptr = fopen(csv_filename, "r")) || 
        !(data_fptr = fopen(data_filename, "wb+")) ||
        !(index_fptr = fopen(index_filename, "wb"))){
            perror("Erro ao abrir os arquivos");
        }

        if(create_data_file(csv_fptr, data_fptr) == -1){
            perror("Erro durante a criação do arquivo de dados: ");
            fclose(csv_fptr);
            fclose(index_fptr);
            fclose(data_fptr);
            return EXIT_FAILURE;
        }

        DYN_ARRAY *index_arr;

        if(!(index_arr = generate_index(data_fptr))){
            perror("Erro durante a geração do índice");
            return EXIT_FAILURE;
        }

        if(write_index(&index_arr, index_fptr) == -1){
            perror("Erro durante a criação do arquivo de dados: ");
            fclose(csv_fptr);
            fclose(index_fptr);
            fclose(data_fptr);
            return EXIT_FAILURE;
        }

        fclose(index_fptr);
        fclose(csv_fptr);
        fclose(data_fptr);
        return EXIT_SUCCESS;
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

        JOGADOR j_query = read_new_jogador();

        select_data(data_fptr, j_query);

        fclose(data_fptr);
    }
    else if(strcmp(operation, "edit") == 0){
        if(argc < 4){
            fprintf(stderr, "Argumentos insuficientes!\n");
            return EXIT_FAILURE;
        }

        char* data_filename = argv[2];
        char* index_filename = argv[3];
        FILE *data_fptr, *index_fptr;

        if(!(data_fptr = fopen(data_filename, "rb+")) || !check_status(data_fptr) ||
        !(index_fptr = fopen(index_filename, "rb+")) || !check_status(index_fptr)){
            perror("Erro ao abrir o arquivo: ");
            return EXIT_FAILURE;
        }

        DYN_ARRAY *index_arr = load_index(index_fptr);
        REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);

        int32_t edit_id;
        sscanf(argv[4], "%" PRId32, &edit_id);

        // JOGADOR j_query = read_query();
        JOGADOR new_j = read_new_jogador();

        // select_data(data_fptr, j_query);

        // Deleta usando somente o id
        JOGADOR old_j_query = jNil;
        old_j_query.id = edit_id;

        int quant_rem;
        delete_data(data_fptr, old_j_query, &quant_rem, &rem_list, &index_arr);

        int quant_ins;
        insert_data(data_fptr, new_j, &quant_ins, &rem_list, &index_arr);

        write_rem_list(&rem_list, data_fptr);
        write_index(&index_arr, index_fptr);

        fclose(data_fptr);
        fclose(index_fptr);
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