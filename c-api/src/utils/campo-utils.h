/**
 * @file campo-utils.h
 * @brief Header file para funcionalidades dos campos de um registro.
 *
 * Este arquivo contém as declarações de várias utilidades para ler e escrever
 * campos de um registro em um arquivo binário de dados.
 *
 * @note Quando uma função desta biblioteca no geral retorna -1, isso significa
 * que houve um erro durante a execução de uma das funções nativas do stdio,
 * logo é possível extrair o erro obtido através da variável errno.
 *
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 *
 * @version 2.0
 */

#ifndef CAMP_UTILS_H
#define CAMP_UTILS_H

#include <stdint.h>
#include <stdio.h>

/**
 * @brief Tamanho das strings alocadas de tamanho não conhecido (ex. strings
 * para leitura de dados)
 */
#define BUFFER_SIZE 200

/**
 * @brief Função que lê um campo de char (string de 1 byte) na posição apontada no
 * arquivo.
 *
 * @param fp Ponteiro para o arquivo binário de dados.
 * @return [unsigned char] Retorna o valor lido pela função.
 */
unsigned char get_campoc(FILE *fp);

/**
 * @brief Função que lê um campo de int de 32 bits (4 bytes) na posição apontada
 * no arquivo.
 *
 * @param fp Ponteiro para o arquivo binário de dados
 * @return [int32_t] Retorna o valor lido pela função
 */
int32_t get_campo32(FILE *fp);

/**
 * @brief Função que lê um campo de int de 64 bits (8 bytes) na posição apontada
 * no arquivo.
 *
 * @param fp Ponteiro para o arquivo binário de dados.
 * @return [int64_t] Retorna o valor lido pela função.
 */
int64_t get_campo64(FILE *fp);

/**
 * @brief Função que lê um campo de string (tamanho variável) na posição
 * apontada pelo arquivo.
 *
 * @details Todas as strings no arquivo binário possuem seu tamanho escrito
 * antes delas, assim essa função lê este tamanho, aloca dinamicamente um espaço
 * com tamanho+1 (para o caractere nulo) para a string, e escreve neste espaço a
 * string na posição apontada no arquivo.
 *
 * @param fp Ponteiro para o arquivo binário de dados
 * @return [char*] Retorna uma string alocada dinamicamente com charactere nulo
 * no fim.
 */
char *get_campo_str(FILE *fp);

/**
 * @brief Atribui um char (string de 1 byte) no espaço apontado no arquivo.
 *
 * @param c Char a ser inserido.
 * @param fp Ponteiro para o arquivo binário de dados.
 *
 * @retval -1 Houve uma falha durante a atribuição do char no arquivo.
 * @retval 0 Atribuição realizada com sucesso.
 */
int set_campoc(const unsigned char c, FILE *fp);

/**
 * @brief Atribui um inteiro de 32 bits (4 bytes) no espaço apontado no arquivo.
 *
 * @param val Inteiro de 4 bytes a ser inserido.
 * @param fp Ponteiro para o arquivo binário de dados.
 *
 * @retval -1 Houve uma falha durante a atribuição do char no arquivo.
 * @retval 0 Atribuição realizada com sucesso.
 */
int set_campo32(const int32_t val, FILE *fp);

/**
 * @brief Atribui um inteiro de 64 bits (8 bytes) no espaço apontado no arquivo.
 *
 * @param val Inteiro de 8 bytes a ser inserido.
 * @param fp Ponteiro para o arquivo binário de dados.
 *
 * @retval -1 Houve uma falha durante a atribuição do char no arquivo.
 * @retval 0 Atribuição realizada com sucesso.
 */
int set_campo64(const int64_t val, FILE *fp);

/**
 * @brief Atribui uma string (de tamanho variável) no espaço apontado no
 * arquivo.
 *
 * @details Além de escrever a string em si, a função escreve o comprimento da
 * string no arquivo, assim tirando a necessidade do caractere nulo no fim da
 * string.
 *
 * @param str String a ser inserida no campo.
 * @param fp Ponteiro para o arquivo binário de dados.
 *
 * @retval -1 Houve uma falha durante a atribuição do char no arquivo.
 * @retval 0 Atribuição realizada com sucesso.
 */
int set_campo_str(const char *str, FILE *fp);

#endif
