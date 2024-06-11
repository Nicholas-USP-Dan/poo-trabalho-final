/**
 * @file data-utils.h
 * @brief Header file de algumas utilidades gerais do arquivo binário de dados
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <stdint.h>
#include <stdio.h>

#include "../data-file.h"

/**
 * Máscara de filtragem (representação em bits):
 * 
 * IDMASK   IDADEMASK   NOMEMASK    NACMASK   CLUBEMASK
 * 0        0           0           0         0
 */

#define IDMASK 0x01
#define IDADEMASK 0x02
#define NOMEMASK 0x04
#define NACMASK 0x08
#define CLUBEMASK 0x10

/**
 * @brief Nome das colunas como registrado nas especificações do projeto
 * 
 */
#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube"

/**
 * @brief Instância de um jogador vazio/nulo, representa uma busca "vazia"
 * 
 */
extern const JOGADOR jNil;

/**
 * @brief Array com todos os nomes dde campos válidos para realizar a filtragem
 */
extern const char *CAMPO_LIST[5];;

/**
 * @brief Função que limpa a memória alocada por um jogador (alocado pelos campos de string)
 * 
 * @param j Referência ao jogador a ter a memória limpa
 */
void free_jogador(JOGADOR *j);

/**
 * @brief Função que imprime, conforme os requisitos do projeto, os dados de um jogador
 * 
 * @details É imprimido o nome do jogador, a nacionalidade dele e o seu clube, ou seja, todas as strings do jogador.
 * 
 * @verbatim
    ----- EX DE SAÍDA -----
    Nome do Jogador: A. SMITH
    Nacionalidade do Jogador: ENGLAND
    Clube do jogador: E. UNAL
   @endverbatim
 * 
 * @param j Jogador a ser imprimido
 */
void print_jogador(const JOGADOR j);

/**
 * @brief Função que lê um registro no arquivo binário e retorna um objeto do tipo JOGADOR 
 * com os dados obtidos pela função.
 * 
 * @param fptr Ponteiro para o arquivo binário de dados.
 * 
 * @note Esta função considera que o ponteiro do arquivo está apontada para o campo id, logo os campos 
 * "removido", "tamanhoRegistro" e "Prox" devem ser pulados antes desta função ser chamada.
 */
JOGADOR read_jogador_data(FILE *fptr);

/**
 * @brief Lê uma linha (do stdin) com os campos de um jogador e retorna uma estrutura com todos os dados inseridos
 * 
 * @return [JOGADOR] Retorna um objeto do tipo JOGADOR guardando os dados dos campos a serem filtrados;
 * os campos não procurados são atribuídos com o mesmo valor do jogador nulo (jNil).
 */
JOGADOR read_query();

/**
 * @brief 
 * 
 * @return [JOGADOR] 
 */
JOGADOR read_new_jogador();

/**
 * @brief Calcula o tamanho que um registro jogador ocupa
 * 
 * @param j Jogador a ter o tamanho calculado
 * @return [int32_t] Retorna o tamnanho ocupado por um registro jogador
 */
int32_t get_reg_size(const JOGADOR j);

/**
 * @brief Verifica se um jogador passa pelo filtro (query)
 * 
 * @param j Jogador a ser testado
 * @param where Critério de filtragem
 * 
 * @retval 1 O jogador passou pela filtragem
 * @retval 0 O jogador não passou pela filtragem
 */
int pass_where(const JOGADOR j, const JOGADOR where);

/**
 * @brief Adiciona um jogador em um arquivo de dados (ignorando os campos "removido", "tamanhoRegistro" e "Prox")
 * 
 * @return [int] 
 */
int add_jogador_reg(const JOGADOR j);

/**
 * @brief Insere um registro em um arquivo binári na posição apontada pelo ponteiro do arquivo
 * 
 * @param j Jogador a ser inserido
 * @param data_fptr Ponteiro para o arquivo binário
 * 
 * @retval -1 Houve uma falha durante a inserção.
 * @retval 0 Inserção feita com sucesso.
 */
int append_reg(const JOGADOR j, FILE *data_fptr);

#endif