# Trabalho Final de Programação Orientada a Objetos

Este projeto envolve a comunicação de uma interface gráfica escrita em Java com um programa em C que manipule um arquivo de registros com jogadores da FIFA2017 à FIFA2023.

> O programa em C foi desenvolvido conforme as especificações do projeto da disciplina SCC0215 - Organização de Arquivos.

A comunicação entre os dois programas é intermediado por um servidor local em Python, onde a interface em Java envia sockets para esse servidor (permitindo com que múltiplas instâncias da interface sejam executadas ao mesmo tempo) e o servidor agenda os comandos recebidos para o programa em C.

A cada conexão, um novo arquivo é criado onde somente ela pode modificar o arquivo, logo não havendo conflitos sobre a manipulação de um mesmo arquivo.

## Características

* As alterações são aplicadas somente ao fim da interação, logo caso o usuário abra um outro arquivo sem salvar as alterações, elas são perdidas

## Configuração

**Dependências**:
* Java (coloque alguma versão)
* Python (coloque alguma versão)
* GCC (coloque alguma versão)
* [Make](https://www.gnu.org/software/make/)

Para compilar o programa em C, execute os comandos:

```
cd c-api
make build
```

Para rodar o server em Python, execute o comando:



## Estrutura do repositório

* [c-api/](c-api/) - diretório do programa em C que manipula os arquivos de registros