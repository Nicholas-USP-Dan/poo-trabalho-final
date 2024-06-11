/**
 * @file trab-1.c
 * @brief Arquivo do programa principal do projeto.
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/adts/dyn-array.h"
#include "src/utils/campo-utils.h"
#include "src/utils/data-utils.h"
#include "src/utils/cabecalho-utils.h"
#include "src/utils/funcoes_fornecidas.h"

#include "src/data-file.h"
#include "src/index-file.h"
#include "src/utils/removed-list.h"

/**
 * @brief Implementação da funcionalidade 1 - cria um novo arquivo de dados a partir de um arquivo csv
 * 
 * @retval -1 A funcionalidade terminou com erro
 * @retval 0 A funcionalidade terminou com sucesso
 */
static inline int func1(){
    char input_filename[200];
    char output_filename[200];
    int ret;

    FILE *data_fptr = NULL;
    FILE *csv_fptr = NULL;

    // Input do usuário
    scanf("%s", input_filename);
    scanf("%s", output_filename);

    // Abre os arquivos, se algum erro for detectado, retorna
    if(!(csv_fptr = fopen(input_filename, "r")) || !(data_fptr = fopen(output_filename, "wb"))){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        
        if(csv_fptr) fclose(csv_fptr);
        if(data_fptr) fclose(data_fptr);

        return -1;
    }

    ret = create_data_file(csv_fptr, data_fptr);
    
    // Fecha os arquivos
    fclose(csv_fptr);
    fclose(data_fptr);

    // Imprime o binarioNaTela do arquvo criado caso a funcionalidade tenha terminado sem erro ou uma mensagem
    // de erro caso contrário 
    ret == 0 ? binarioNaTela(output_filename) : 
                fprintf(stdout, "Falha no processamento do arquivo.\n");

    return ret;
}

/**
 * @brief Implemetação da funcionalidade 2 - mostra na tela todos os registros presentes no arquivo de dados
 * 
 * @note A função principal dessa funcionalidade é a mesma com o 3, possuindo apenas um critério vazio de busca
 * 
 * @retval -1 A funcionalidade terminou com erro
 * @retval 0 A funcionalidade terminou com sucesso
 */
static inline int func2(){
    char input_filename[200];
    int ret;

    FILE *data_fptr = NULL;

    // Input do usuário
    scanf("%s", input_filename);

    // Abre os arquivos, se algum erro for detectado, retorna
    if(!(data_fptr = fopen(input_filename, "rb")) || !check_status(data_fptr)){
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        if(data_fptr) fclose(data_fptr);

        return -1;
    }

    ret = select_data(data_fptr, jNil);

    // Fecha os arquivos
    fclose(data_fptr);

    // Caso algum erro seja encontrado, imprimir uma mensagem de erro
    if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");

    return ret;
}

/**
 * @brief Implementação da funcionalidade 3 - Filtra os registros do arquivo de dados por meio de um
 * critério de busca, por uma quantidade escolhida pelo usuário
 * 
 * @note Esta funcionalidade não altera o arquivo de dados
 * 
 * @retval -1 A funcionalidade terminou com erro
 * @retval 0 A funcionalidade terminou com sucesso
 */
int static inline func3(){
    char input_filename[200];
    int n;
    int ret;

    FILE *data_fptr = NULL;

    // Input do ususário
    scanf("%s", input_filename);
    scanf("%d", &n);

    // Abre os arquivos, se algum erro for encontrado, retorna
    if(!(data_fptr = fopen(input_filename, "rb")) || !check_status(data_fptr)){
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        if(data_fptr) fclose(data_fptr);

        return -1;
    }

    // Faz n buscas
    for(int i = 0; i < n; i++){
        // Lê o critério escrito pelo usuário
        JOGADOR j_query = read_query();

        printf("Busca %d\n\n", i+1);

        ret = select_data(data_fptr, j_query);
        // Imprime uma mensagem de erro caso um erro for encontrado
        if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");
    }

    // Fecha o arquivo
    fclose(data_fptr);

    return 0;
}

/**
 * @brief Implementação da funcionalidade 4 - Criação de um arquivo de índice
 * 
 * @details As funções principais desta funcionalidade estão separadas em carregar a lista de índices
 * (ler o arquivo de dados e criar uma lista (em memória primária)) e escrever esta lista de índices
 * no arquivo de índices
 * 
 * @retval -1 A funcionalidade terminou com erro
 * @retval 0 A funcionalidade terminou com sucesso
 */
int static inline func4(){
    char input_filename[200];
    char output_filename[200];
    int ret;

    FILE *data_fptr = NULL;
    FILE *index_fptr = NULL;

    // Input do usuário
    scanf("%s", input_filename);
    scanf("%s", output_filename);

    // Abre os arquivos, se algum erro for encontrado, retorna
    if(!(data_fptr = fopen(input_filename, "rb")) || !check_status(data_fptr) || 
    !(index_fptr = fopen(output_filename, "wb"))){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        
        if(data_fptr) fclose(data_fptr);
        if(index_fptr) fclose(index_fptr);

        return -1;
    }

    // Cria a lista de índice
    DYN_ARRAY *index_arr = generate_index(data_fptr);

    // Fecha o arquivo de dados (com a lista gerada, não é mais necessário ler do arquivo de dados)
    fclose(data_fptr);

    // Retorna se a lista não puder ser gerada
    if(!index_arr){
        return -1;
    }

    // Escreve a lista de índices no arquivo de índice
    ret = write_index(&index_arr, index_fptr);

    clear_dynarr(&index_arr);

    // Fecha o arquivo de índices
    fclose(index_fptr);

    // Imprime o binarioNaTela do arquivo de índices criado
    binarioNaTela(output_filename);

    return ret;
}

int static inline func5(){
    char input_filename[200];
    char index_filename[200];
    int n;
    int ret;

    FILE *data_fptr = NULL;
    FILE *index_fptr = NULL;

    // Leitura de input do usuário
    scanf("%s", input_filename);
    scanf("%s", index_filename);

    // Abre os arquivos, se algum erro for detectado, retorna
    if(!(data_fptr = fopen(input_filename, "r+b")) || !check_status(data_fptr) || 
    !(index_fptr = fopen(index_filename, "wb"))){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        
        if(data_fptr) fclose(data_fptr);
        if(index_fptr) fclose(index_fptr);

        return -1;
    }

    // Leitura da quantidade de remoções a serem realizadas
    scanf("%d", &n);

    // Setta os status do arquivo de dados e de índice como '0' (instável)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);
    set_campoc('0', data_fptr);
    fseek(index_fptr, 0, SEEK_SET);
    set_campoc('0', index_fptr);

    // Array contendo os índices do arquivo de dados
    DYN_ARRAY *index_arr = generate_index(data_fptr);

    // Array com a lista de registros removidos
    REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);

    // Variável que mantém a quantidade de elementos removidos na operação
    int quant_rem = 0;

    for (int i = 0; i < n; i++){
        JOGADOR j_query = read_query();
        ret = delete_data(data_fptr, j_query, &quant_rem, &rem_list, &index_arr);
        if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");

        free_jogador(&j_query);
    }

    // Escrever as estruturas em memória secundária
    write_index(&index_arr, index_fptr);
    write_rem_list(&rem_list, data_fptr);

    // Atualiza os campos com a quantidade de registros no cabeçalho do arquivo de dados
    update_nro_reg(-quant_rem, data_fptr);

    // Setta os status do arquivo de dados e de índice como '1' (estável)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);
    set_campoc('1', data_fptr);
    fseek(index_fptr, 0, SEEK_SET);
    set_campoc('1', index_fptr);

    // Limpeza de memória das estruturas utilizadas (lista de índices e de removidos)
    clear_dynarr(&index_arr);
    clear_rem_list(&rem_list);

    // Fecha os arquivos
    fclose(data_fptr);
    fclose(index_fptr);

    // Imprime os bináriosNaTela dos arquivos
    binarioNaTela(input_filename);
    binarioNaTela(index_filename);

    return 0;
}

int static inline func6(){
    char input_filename[200];
    char index_filename[200];
    int n;
    int ret;

    FILE *data_fptr = NULL;
    FILE *index_fptr = NULL;

    // Leitura de input do usuário
    scanf("%s", input_filename);
    scanf("%s", index_filename);

    // Abre os arquivos, se algum erro for detectado, retorna
    if(!(data_fptr = fopen(input_filename, "r+b")) || !check_status(data_fptr) || 
    !(index_fptr = fopen(index_filename, "wb"))){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        
        if(data_fptr) fclose(data_fptr);
        if(index_fptr) fclose(index_fptr);

        return -1;
    }

    // Leitura da quantidade de remoções a serem realizadas
    scanf("%d", &n);

    // Setta os status do arquivo de dados e de índice como '0' (instável)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);
    set_campoc('0', data_fptr);
    fseek(index_fptr, 0, SEEK_SET);
    set_campoc('0', index_fptr);

    // Array contendo os índices do arquivo de dados
    DYN_ARRAY *index_arr = generate_index(data_fptr);

    // Array com a lista de registros removidos
    REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);

    // Variável que mantém a quantidade de elementos removidos na operação
    int quant_ins = 0;

    for (int i = 0; i < n; i++){
        // JOGADOR j_query = read_query();
        JOGADOR j_query = read_new_jogador();
        
        ret = insert_data(data_fptr, j_query, &quant_ins, &rem_list, &index_arr);
        if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");

        free_jogador(&j_query);
    }

    // Escrever as estruturas em memória secundária
    write_index(&index_arr, index_fptr);
    write_rem_list(&rem_list, data_fptr);

    // Atualiza os campos com a quantidade de registros no cabeçalho do arquivo de dados
    update_nro_reg(quant_ins, data_fptr);

    // Setta os status do arquivo de dados e de índice como '1' (estável)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);
    set_campoc('1', data_fptr);
    fseek(index_fptr, 0, SEEK_SET);
    set_campoc('1', index_fptr);

    // Limpeza de memória das estruturas utilizadas (lista de índices e de removidos)
    clear_dynarr(&index_arr);
    clear_rem_list(&rem_list);

    // Fecha os arquivos
    fclose(data_fptr);
    fclose(index_fptr);

    // Imprime os bináriosNaTela dos arquivos
    binarioNaTela(input_filename);
    binarioNaTela(index_filename);

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
        case '4':
            ret = func4();
            break;
        case '5':
            ret = func5();
            break;
        case '6':
            ret = func6();
            break;
        default:
            fprintf(stdout, "Funcionalidade invalida.\n");
            ret = -1;
            break;
    }

    return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}