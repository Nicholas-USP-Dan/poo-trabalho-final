/**
 * @file select-data.c
 * @brief Implementação da funcionalidade 2 & 3 como definido no projeto
 * 
 * O tipo de jogador é utilizado para representar uma busca filtrada (a estrutura contém todos os campos de um registro);
 * Como definido nas especificações do projeto, o id de cada jogador é único, assim, em buscas que filtram o id de um jogador,
 * É possível sair da busca assim que achar-se um registro com o mesmo id que o filtrado
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

// TODO: Fazer iteração contando que os registros podem não estar preenchidos completamente
int select_data(FILE *data_fptr, JOGADOR where){ // Seria legal se essa função retornasse algo como um JSON para ser impresso depois 
    int filter_count = 0;

    // Pular o cabecalho
    fseek(data_fptr, HEADER_END_OFFSET, SEEK_SET);
    // fseek(data_fptr, HEADER_END_OFFSET-NRO_REGARQ_OFFSET, SEEK_CUR);

    // Lê registros até ler todos os registros válidos
    while(1){
        long reg_offset = ftell(data_fptr);
        unsigned char rem = get_campoc(data_fptr);

        if(feof(data_fptr)){
            break;
        }

        int32_t reg_size = get_campo32(data_fptr);

        // Registro esta removido, mover para o proximo
        if(rem == '1'){    
            fseek(data_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        // Pular tamanhoRegistro e Prox
        fseek(data_fptr, 8, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);

        // Registro passou pelo filtro
        if(pass_where(j, where)){
            filter_count++;

            print_jogador(j);
            printf("\n");
        }

        free_jogador(&j);

        // Caso o filtro tenha um campo id, quando o jogador com esse id for encontrado, terminar busca
        if(where.id == j.id){
            break;
        }

        fseek(data_fptr, reg_offset + reg_size, SEEK_SET);
    }

    // Imprimir: "Registro inexistente." caso nenhum registro passar pelo filtro
    if(filter_count <= 0){
        printf("Registro inexistente.\n\n");
    }

    return 0;
}