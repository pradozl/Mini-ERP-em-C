#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cliente/cliente.h"
#include "pedido/pedido.h"
#include "produto/Produto.h"

WINDOW* janelaPrincipal;

int sair(FILE* fpC, FILE* fpP, FILE* fpPe) {
    fclose(fpC);
    fclose(fpP);
    fclose(fpPe);
    endwin();
    return 0;
}

int menuInicial() {
    char* escolhas[] = {"1) Manter Clientes", "2) Manter Produtos",
                        "3) Manter Pedidos", "4) Sair"};  // Escolhas

    /**
     * Inclusive, usar essa ideia pra realizarmos a listagem dos clientes,
     * produtos e pedidos. Ele lista todos os clientes, e a medida que a gente
     * vai "escolhendo" os logs, ele imprime individualmente do lado os dados.
     * Vou começar a já pensar nisso para o clientes.
     */

    int numEscolhas = sizeof(escolhas) / sizeof(char*);
    /**
     * Divido o tamanho dos arrays pelo tamanho do ponteiro char (descubro o
     * a quantidade de escolhas)
     */
    int tecla;
    int destaque = 0;  // O destaque começa no primeiro item
    int escolha = -1;  // A escolha começa em -1 pois nada foi escolhido

    clear();
    mvprintw(0, 0, "===== MENU INICIAL =====\n\n");
    mvprintw(2, 5,
             "Use as setas para navegar. Pressione ENTER para selecionar.");

    while (escolha == -1) {
        for (int i = 0; i < numEscolhas; i++) {
            int y = 5 + i;  // Imprimir na tela com separação
            int x = 5;

            if (i == destaque) {
                attron(A_REVERSE);
                /**
                 * Sempre que i especificamente é o valor que está escolhido
                 * utilizando a tecla, ele altera a cor da fonte pela cor do
                 * fundo escolhido
                 */
            }

            mvprintw(y, x, "%s",
                     escolhas[i]);  // Imprimive individualmente na tela
            attroff(A_REVERSE);
        }
        refresh();

        tecla = getch();

        switch (tecla) {
            case KEY_UP:
                destaque--;
                if (destaque < 0) {
                    destaque = numEscolhas - 1;
                }
                break;
            case KEY_DOWN:
                destaque++;
                if (destaque >= numEscolhas) {
                    destaque = 0;
                }
                break;
            case 10:
                escolha = destaque;
                break;
        }
    }

    return escolha;
}

int manterClientes() {

    int tecla;
    int escolha = -1;
    int destaque = 0;
    

    char *escolhas[] = {
        "1) Cadastrar Clientes",
        "2) Consultar Clientes",
        "3) Remover Clientes",
        "4) Listar Clientes",
        "5) Voltar"
    };

    int numOpcoes = sizeof(escolhas) / sizeof(char*);

    while (escolha == -1) {
        clear();

        mvprintw(0, 0,"===== MANTER CLIENTES =====\n\n");

        for (int i = 0; i < numOpcoes; i++) {
            int y = 5 + i;  // Imprimir na tela com separação
            int x = 5;

            if (i == destaque) {
                attron(A_REVERSE);
            }

            mvprintw(y, x, "%s",
                     escolhas[i]);
            attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();

        switch (tecla) {
            case KEY_UP:
            destaque--;
            if (destaque < 0) {
                destaque = numOpcoes - 1;
            }
            break;
            case KEY_DOWN:
            destaque++;
            if (destaque >= numOpcoes){
                destaque = 0;
            }
            break;
            case 10:
            case '\r':
            case KEY_ENTER:
            escolha = destaque;
            break;
        }
    }

    flushinp();
    return escolha + 1;
}
int manterProdutos() {
int tecla;
    int escolha = -1;
    int destaque = 0;

char *escolhas[] = {
        "1) Cadastrar Produto",
        "2) Consultar Produto",
        "3) Remover Produto",  
        "4) Listar Produto",   
        "5) Voltar"
    };

    int numOpcoes = sizeof(escolhas) / sizeof(char*);

    while (escolha == -1) {
        clear();

        mvprintw(0, 0,"===== MANTER PRODUTOS =====\n\n");
        mvprintw(2, 5, "Use as setas para navegar. Pressione ENTER para selecionar.\n");

        for (int i = 0; i < numOpcoes; i++) {
            int y = 5 + i; 
            int x = 5;

            if (i == destaque) {
                attron(A_REVERSE);
            }

            mvprintw(y, x, "%s", escolhas[i]);
            attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();

        switch (tecla) {
            case KEY_UP:
            destaque--;
            if (destaque < 0) {
                destaque = numOpcoes - 1;
            }
            break;
            case KEY_DOWN:
            destaque++;
            if (destaque >= numOpcoes){
                destaque = 0;
            }
            break;
            case 10:
            case '\r':
            case KEY_ENTER:
            escolha = destaque;
            break;
        }
    }

    flushinp();
    return escolha + 1; 
}

/* ======MENU INTERATIVO DO MANTER PEDIDOS====== */

int manterPedidos() {
    int tecla;
    int escolha = -1;
    int destaque = 0;

/* ======LISTA DE OPÇÕES====== */

char *escolhas[] = {
        "1) Cadastrar Pedido",
        "2) Consultar Pedidos",
        "3) Atualizar Pedido", 
        "4) Remover Pedidos",  
        "5) Listar Pedidos",   
        "6) Voltar"
    };

    int numOpcoes = sizeof(escolhas) / sizeof(char*);

    while (escolha == -1) {
        clear();

        mvprintw(0, 0,"===== MANTER PEDIDOS =====\n\n");
        mvprintw(2, 5, "Use as setas para navegar. Pressione ENTER para selecionar.\n");

        for (int i = 0; i < numOpcoes; i++) {
            int y = 5 + i; 
            int x = 5;

            if (i == destaque) {
                attron(A_REVERSE); /* Serve para inverter a cor do texto, pois irá ligar o destaque, isso "ao colocar" a setinha em cima */
            }

            mvprintw(y, x, "%s", escolhas[i]);
            attroff(A_REVERSE); /* Serve para invertar a cor do texto, pois irá ligar o destaque */
        }

        refresh();

        tecla = getch();

        switch (tecla) {
            case KEY_UP:
            destaque--;
            if (destaque < 0) { /* Se passar do início, volta para o fim */ 
                destaque = numOpcoes - 1;
            }
            break;
            case KEY_DOWN:
            destaque++;
            if (destaque >= numOpcoes){ /* Se passar do fim, volta para o início */
                destaque = 0;
            }
            break;

            /* Opções de "Enters" para cada tipo de sistema */

            case 10: /* Linux */
            case '\r': /* Windows */
            case KEY_ENTER: /* Ncurses */
            escolha = destaque;
            break;
        }
    }

    flushinp(); /* Serve para limpar o buffer do teclado, pois assim evitar ENTER duplo,
                tava dando erro de alocação de memória em alguns testes que fiz */ 

    return escolha + 1; /* Retorna a escolha, no caso 1 a 6 e não 0 a 5 */ 
}

int main() {
    FILE* fpClientes = fopen("clientes.csv", "a+");
    FILE* fpProdutos = fopen("produtos.csv", "a+");
    FILE* fpPedidos = fopen("pedidos.csv", "a+");

    if (fpClientes == NULL || fpProdutos == NULL || fpPedidos == NULL) {
        printf(
            "Erro fatal: Não foi possível abrir os arquivos .csv "
            "necessários.\n");
        return EXIT_FAILURE;
    }

    initscr();
    start_color();
    clear();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));

    int escolhaMenu;

    do {
        escolhaMenu = menuInicial();

        switch (escolhaMenu) {
            case 0: {
                int clienteValor;
                do {
                    clienteValor = manterClientes();
                    switch (clienteValor) {
                        case 1:
                            cadastrarClientes(fpClientes);
                            break;
                        case 2:
                            consultarClientes(fpClientes);
                            break;
                        case 3:
                            fpClientes = deletarClientes(fpClientes);
                            break;
                        case 4:
                            listarClientes(fpClientes);
                            break;
                        case 5:
                            break;
                        default:
                            printw("Opção Inválida!");
                            refresh();
                            getch();
                            break;
                    }
                } while (clienteValor != 5);
                break;
            }
            case 1:
                int produtoValor;
                do {
                    produtoValor = manterProdutos();
                    switch (produtoValor) {
                        case 1:
                            cadastrarProdutos(fpProdutos);
                            break;
                        case 2:
                            consultarProdutos(fpProdutos);
                            break;
                        case 3:
                            deletarProdutos(fpProdutos);
                            fpProdutos = fopen(ARQUIVO_PRODUTOS, "a+");
                            if (fpProdutos == NULL) {
                                printf(
                                    "ERRO CRÍTICO: Não foi possível reabrir o "
                                    "arquivo após a deleção.\n");
                                exit(1);
                            }
                            break;
                        case 4:
                            listarProdutos(fpProdutos);
                            break;
                        case 5:
                            break;
                        default:
                            printw(
                                "Código inválido! Tente novamente um código "
                                "válido.");
                            refresh();
                            getch();
                            break;
                    }

                } while (produtoValor != 5);
                break;
            case 2:
                int pedidoValor;
                do {
                    pedidoValor = manterPedidos();
                    switch (pedidoValor) {
                        case 1:
                            cadastrarPedido(fpPedidos, fpClientes, fpProdutos);
                            break;
                        case 2:
                            consultarPedido(fpPedidos, fpClientes);
                            break;
                        case 3:
                            atualizarPedido(fpPedidos, fpClientes, fpProdutos);
                            fpPedidos = fopen("pedidos.csv", "a+");
                            if (fpPedidos == NULL) {
                                printw("ERRO CRÍTICO: Nao foi possivel reabrir o arquivo de pedidos apos atualizacao.");
                                exit(1);
                            }
                            break;
                        case 4:
                            fpPedidos = deletarPedido(fpPedidos, fpClientes);
                            break;
                        case 5:
                            listarPedidos(fpPedidos);
                            break;
                        case 6:
                            break;
                        default:
                            printw(
                                "Código Inválido! Tente novamente um código "
                                "válido.");
                            refresh();
                            getch();
                            break;
                    }

                } while (pedidoValor != 6);
                break;
            case 3:
                clear();
                mvprintw(10, 5, "Saindo do programa...\n");
                refresh();
                break;
            default:
                printw("Opção inválida!\n");
                refresh();
                getch();
                break;
        }

    } while (escolhaMenu != 3);

    printw("\nPressione qualquer tecla para sair...");
    getch();

    refresh();
    sair(fpClientes, fpProdutos, fpPedidos);
    return EXIT_SUCCESS;
}