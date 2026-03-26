#ifndef CLIENTE_H
#define CLIENTE_H


#define maxNome 30
#define maxRazao 40
#define maxEmail 50
#define maxDescricao 100
#define maxEndereco 40
#define maxData 11
#define tamCPF 12
#define tamCNPJ 15
#define tamTelefone 10

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char rua[maxEndereco];
    char setor[maxEndereco];
    char cidade[maxEndereco];
    char estado[maxEndereco];
} Endereco;

typedef struct {
    int identificador;
    char nome[maxNome];
    char telefone[tamTelefone];
    char email[maxEmail];
    Endereco endereco;
    char cpf[tamCPF];
    char razaoSocial[maxRazao];
    char cnpj[tamCNPJ];
} Cliente;

int validaCPF(char cpf[tamCPF]);
int validaCNPJ(char cnpj[tamCNPJ]);
int codigoClienteJaExiste(FILE* fp, int code);
void cadastrarClientes(FILE* fp);
void consultarClientes(FILE* fp);
FILE* deletarClientes(FILE* fp);
int listarClientes(FILE* fp);

#endif