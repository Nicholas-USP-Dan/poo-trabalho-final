/**
 * @file funcoes_fornecidas.h
 * @brief Header file de funções fornecidas para o projeto
 * 
 * @author Cristina Dutra de Aguiar (cdac@icmc.usp.br)
 * @version 1.0
 * 
 */

#ifndef FUNC_FORN
#define FUNC_FORN

/**
 * @brief Função que imprime uma hash na tela de um arquivo binário de dados. Use essa função para comparação no run.codes.
 * 
 * @note Lembre-se de ter fechado (fclose) o arquivo anteriormente. Ela vai abrir de novo para leitura e depois fechar 
 * (você não vai perder pontos por isso se usar ela).
 * 
 * @param nomeArquivoBinario Nome do arquivo a ser hasheado.
 */
void binarioNaTela(char *nomeArquivoBinario);

/**
 * @brief Use essa função para ler um campo string delimitado entre aspas (").
 * 
 * @details
 * @verbatim
 *	Chame ela na hora que for ler tal campo. Por exemplo:
 *
 *	A entrada está da seguinte forma:
 *		nomeDoCampo "MARIA DA SILVA"
 *
 *	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
 *		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
 *		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
 * @endverbatim
 * 
 * @note A função lê strings normalmente (como scanf("%s")) caso uma entrada não tenha aspas

 * @param [out] str String a ser escrita (str já deve ter espaço alocado)
 */
void scan_quote_string(char *str);

#endif