/**
 * @file trab-int.c
 * @brief Arquivo do programa principal do projeto.
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "src/utils/data-utils.h"
#include "src/utils/cabecalho-utils.h"
#include "src/utils/funcoes_fornecidas.h"

#include "src/data-file.h"

static inline int func1(){
    char input_filename[200];
    char output_filename[200];
    int ret;

    FILE *data_fptr = NULL;
    FILE *csv_fptr = NULL;

    scanf("%s", input_filename);
    scanf("%s", output_filename);

    // Abertura dos arquivos
    if(!(csv_fptr = fopen(input_filename, "r")) || !(data_fptr = fopen(output_filename, "wb"))){
        if(csv_fptr) fclose(csv_fptr);
        if(data_fptr) fclose(data_fptr);

        fprintf(stdout, "Falha no processamento do arquivo.\n");
        return -1;
    }

    ret = create_data_file(csv_fptr, data_fptr);
    
    fclose(csv_fptr);
    fclose(data_fptr);

    ret == 0 ? binarioNaTela(output_filename) : fprintf(stdout, "Falha no processamento do arquivo.\n");

    return ret;
}

static inline int func2(){
    char input_filename[200];
    int ret;

    FILE *data_fptr = NULL;

    scanf("%s", input_filename);

    if(!(data_fptr = fopen(input_filename, "rb")) || !check_status(data_fptr)){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        return -1;
    }

    ret = select_data(data_fptr, jNil);

    fclose(data_fptr);

    if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");

    return ret;
}

int static inline func3(){
    char input_filename[200];
    int n;
    int ret;

    FILE *data_fptr = NULL;

    scanf("%s", input_filename);
    scanf("%d", &n);

    if(!(data_fptr = fopen(input_filename, "rb")) || !check_status(data_fptr)){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        return -1;
    }

    for(int i = 0; i < n; i++){
        JOGADOR j_query = read_query();
        printf("Busca %d\n\n", i+1);

        ret = select_data(data_fptr, j_query);

        if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");

        fseek(data_fptr, 0, SEEK_SET);
    }

    fclose(data_fptr);

    return 0;
}

int main(){
    // Le um caractere no stdin para verificar qual operacao realizar
    char op;
    scanf("%c", &op);

    int ret;

    // Ramificacao para cada operacao
    switch(op){
        case '1':
            ret = func1();
            break;
        case '2':
            ret = func2();
            break;
        case '3':
            ret = func3();
            break; 
        default:
            fprintf(stdout, "Funcionalidade invalida.\n");
            ret = -1;
            break;
    }

    return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}