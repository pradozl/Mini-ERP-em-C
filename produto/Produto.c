#include "Produtos.h"

int codigoProdutoJaExiste(FILE* fp, int code) {
    rewind(fp);

    char linha[1024];
    int identificador;


    while (fgets(linha, sizeof(linha), fp) != NULL) {
        if (sscanf(linha, "%d", &identificador) == 1) {
            if(identificador == code) {
                return 1;
            }
        }
    }
    return 0;
}

void cadastrarProdutos(FILE* fp) {
	Produtos produto;

	int codigo_existe;

	do {
		clear();
		printw("===== CADASTRO DE PRODUTO =====\n\n");

		printw("Digite um codigo identificador para o produto: ");
        echo();
		scanw("%d", &produto.identificador);
		getch();

		codigo_existe = codigoProdutoJaExiste(fp, produto.identificador);

		if (codigo_existe == 1) {
			clear();
			printw("ERRO: O codigo %d ja esta cadastrado!\n", produto.identificador);
			printw("Por favor, pressione qualquer tecla e digite um codigo diferente.\n");
			refresh();
			getch();
		}

	} while (codigo_existe == 1);

	clear();

	printw("===== CADASTRO DE PRODUTO (Cod: %d) =====\n\n", produto.identificador);

	printw("Digite a descricao do produto: ");
	echo();
	getnstr(produto.descricao, maxDescricao - 1);

	printw("Digite o preco do produto: ");
    echo();
	scanw("%lf", &produto.preco);
	getch();

	printw("Digite quantos itens tem no estoque: ");
	echo();
    scanw("%d", &produto.estoque);
	getch();


	fprintf(fp, "%d ; %s ; %.2lf ; %d\n", produto.identificador, produto.descricao, produto.preco, produto.estoque);
	fflush(fp);

	printw("\nProduto cadastrado com sucesso!");
	refresh();
	getch();
}

void consultarProdutos(FILE* fp) {
	Produtos produto;
	int codigo;
	int opcao = 1;

	clear();
	do {

        int encontrado = 0;

		printw("===== CONSULTAR PRODUTOS =====\n\n");

		printw("Digite o codigo para ser consultado: ");
		echo();
        scanw("%d", &codigo);

		refresh();

		clear();

		printw("===== CONSULTAR PRODUTOS =====\n\n");

		rewind(fp);

        char buffer[MAX_LINE];


		while (fscanf(fp, "%d ; %99[^;] ; %lf ; %d", &produto.identificador, produto.descricao, &produto.preco, &produto.estoque) == 4) {
			if (codigo == produto.identificador) {
				printw("%d,%s,%lf,%d", produto.identificador, produto.descricao,
				       produto.preco, produto.estoque);
				encontrado++;
			}
		}

		if (encontrado == 0) {
            printw("Nao existe qualquer produto com este identificador!\nDeseja tentar novamente com outro identificador?");
        } else {
            printw("\n\nConsulta concluída. Deseja fazer outra consulta?");
        }
        
        printw("\n1 - Sim, tentar novamente\n2 - Nao, continuar o programa\n");
        scanw("%d", &opcao);
        getch();
        refresh();

    } while (opcao == 1);
}

void deletarProdutos(FILE* fp_origem) {
    
    FILE *fp_temp;
    Produtos produto;
    int codigo_alvo;
    int confirmacao;
    int encontrado = 0;
    char buffer[MAX_LINE];
    
    clear();
    printw("\n\n===== DELETAR PRODUTO =====\n\n");
    printw("Digite o codigo do produto a ser deletado: ");
    echo();
    scanw("%d", &codigo_alvo);
    getch();
    
    rewind(fp_origem);
    
    fp_temp = fopen(TEMP_FILE, "w");
    if (fp_temp == NULL) {
        printw("\nERRO ao criar arquivo temporário.");
        refresh(); getch(); return;
    }
    
    while (fscanf(fp_origem, "%d ; %99[^;] ; %lf ; %d", 
                  &produto.identificador, produto.descricao, &produto.preco, &produto.estoque) == 4) 
    {
        
        if (produto.identificador == 0) {
            continue; 
        }
        
        if (codigo_alvo == produto.identificador) {
            encontrado = 1;
            
            clear();
            printw("\n\n===== PRODUTO ENCONTRADO =====");
            printw("\nCodigo: %d\nDescricao: %s\nPreço: %.2lf\nEstoque: %d", 
                   produto.identificador, produto.descricao, produto.preco, produto.estoque);

            printw("\n\n===== CONFIRMAR EXCLUSAO =====");
            printw("\n1 - Sim\n2 - Nao\n==============================\n");
            scanw("%d", &confirmacao);
            getch();
            
            if(confirmacao == 1) {
                printw("\nExclusão confirmada. Linha sera pulada na reescrita.");
            } else {
                printw("\nExclusão negada. Produto sera mantido.");
                fprintf(fp_temp, "%d ; %s ; %.2lf ; %d\n", 
                        produto.identificador, produto.descricao, produto.preco, produto.estoque);
            }
            refresh(); getch(); 
            
        } else {
            fprintf(fp_temp, "%d ; %s ; %.2lf ; %d\n", 
                    produto.identificador, produto.descricao, produto.preco, produto.estoque);
        }
    }
    
    fclose(fp_origem); 
    fflush(fp_temp);
    fclose(fp_temp);

    if (encontrado) {
        if (remove(ARQUIVO_PRODUTOS) == 0) {
            if (rename(TEMP_FILE, ARQUIVO_PRODUTOS) == 0) {
                printw("\n\nOperação concluída. Arquivo atualizado.");
            } else {
                printw("\nERRO CRÍTICO: Falha ao renomear o arquivo temporário.");
            }
        } else {
            printw("\nERRO CRÍTICO: Falha ao remover o arquivo original.");
        }
    } else {
        printw("\n\nProduto com código %d não encontrado.", codigo_alvo);
        remove(TEMP_FILE);
    }
    
    refresh(); 
    getch();
}

void listarProdutos(FILE *fp) {
    clear();
    rewind(fp);
    
    int ehVazio = 1; 
    char linha[1024];
    
    Produtos produto;

    printw("\n\n===== LISTA DE PRODUTOS =====\n\n");

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        if(sscanf(linha, "%d ; %[^;] ; %lf ; %d", 
           &produto.identificador, produto.descricao, &produto.preco, &produto.estoque) == 4) {
            
            if(produto.identificador != 0) {
                ehVazio = 0;
                printw("ID: %d | %s | R$ %.2f | Qtd: %d\n", 
                       produto.identificador, produto.descricao, produto.preco, produto.estoque);
            }
        }
    }

    if (ehVazio) {
        printw("Nenhum produto cadastrado!\n");
    }

    printw("\nPressione qualquer tecla para voltar...");
    refresh();
    getch();
}