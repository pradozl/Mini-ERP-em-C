#include "pedido.h"

Pedido listaPedidos[maxPedidos];
int totalPedidos = 0;

void obterDataAtual(char *destino, int tamanho){
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    
    snprintf(destino, tamanho, "%02d/%02d/%04d",tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
}

void obterDadosProduto(FILE *fpP, int codigoProduto, char *descricaoDestino, double *precoDestino){
    rewind(fpP);
    
    char linha[1024];
    int idTemp, estoqueTemp;
    char descTemp[maxDescricao];
    double precoTemp;

    while(fgets(linha, sizeof(linha), fpP) != NULL){
        if(sscanf(linha, "%d ; %[^;] ; %lf ; %d", 
                  &idTemp, descTemp, &precoTemp, &estoqueTemp) == 4){
            
            if(idTemp == codigoProduto){
                strncpy(descricaoDestino, descTemp, maxDescricao - 1);
                descricaoDestino[maxDescricao - 1] = '\0';
                *precoDestino = precoTemp;
                return;
            }
        }
    }
    *precoDestino = 0.0;
    strcpy(descricaoDestino, "Produto não Encontrado");
}

int codigoPedidoJaExiste(FILE* fpPe, int code){
    rewind(fpPe);

    int codigoPedido;
    int codigoCliente;
    int codigoProduto;
    int quantidade;
    char data[maxData];
    char descricao[maxDescricao];
    double valorTotal;
    char status[maxStatus];

   while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c", &codigoPedido, &codigoCliente, &codigoProduto,
                &quantidade, data,descricao, &valorTotal, status) == 8){
        if(codigoPedido == code && codigoPedido != 0){ 
            return 1;
        }
    }
    return 0;
}

void cadastrarPedido(FILE *fpPe, FILE *fpC, FILE *fpP){
    clear();

    Pedido pedido;
    double precoUnitario = 0.0;
    echo();

    mvprintw(0, 0, "===== CADASTRO DE PEDIDO =====\n\n");

    mvprintw(3, 0, "Digite o código do pedido:\n");
    refresh();
    echo();
    scanw("%d", &pedido.codigoPedido);
    noecho();

    if(pedido.codigoPedido == 0){ 
        noecho();
        mvprintw(LINES - 22, 0, "Erro: O código do pedido não pode ser 0!\n");
        flushinp();
        getch();
        return;
    }

    if(codigoPedidoJaExiste(fpPe, pedido.codigoPedido)){
        noecho();
        mvprintw(LINES - 22, 0, "Erro: Já existe este código de pedido!\n");
        flushinp();
        getch();
        return;
    }

    mvprintw(5, 0, "Digite o código do cliente:\n");
    refresh();
    echo();
    scanw("%d", &pedido.codigoCliente);
    noecho();
    
    if(!codigoClienteJaExiste(fpC, pedido.codigoCliente)){
        noecho();
        mvprintw(LINES - 22, 0, "Erro: Cliente não encontrado!\n");
        flushinp();
        getch();
        return;
    }

    mvprintw(7, 0, "Digite o código do Produto:\n");
    refresh();
    echo();
    scanw("%d", &pedido.codigoProduto);
    noecho();
    
    if(!codigoProdutoJaExiste(fpP, pedido.codigoProduto)){
        noecho();
        mvprintw(LINES - 22, 0, "Erro: Produto não encontrado!\n");
        flushinp();
        getch();
        return;
    }

    mvprintw(9, 0, "Digite a quantidade de itens:\n");
    refresh();
    echo();
    scanw("%d", &pedido.quantidade);
    noecho();

    obterDadosProduto(fpP, pedido.codigoProduto, pedido.descricao, &precoUnitario);
    pedido.valorTotal = precoUnitario * pedido.quantidade;

    obterDataAtual(pedido.data, maxData);
    strcpy(pedido.status, "Pendente");

    noecho();
    mvprintw(11, 0, "Descrição do Produto: %s", pedido.descricao);
    mvprintw(12, 0, "Valor Unitário: R$ %.2f", precoUnitario);
    mvprintw(13, 0, "Valor Total: R$ %.2f", pedido.valorTotal);
    
    fprintf(fpPe, "%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n", pedido.codigoPedido, pedido.codigoCliente,pedido.codigoProduto, pedido.quantidade, 
        pedido.data, pedido.descricao, pedido.valorTotal, pedido.status);

    fflush(fpPe);

    mvprintw(LINES - 2, 0, "\nPedido cadastrado com sucesso!\n");
    refresh();
    flushinp();
    getch(); 
}

void listarPedidos(FILE *fpPe){
    clear();
    rewind(fpPe);
    
    Pedido pedido;
    
    printw("\n\n=====LISTA DE PEDIDOS=====\n\n");
    
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c", &pedido.codigoPedido, &pedido.codigoCliente,
        &pedido.codigoProduto,&pedido.quantidade, pedido.data, pedido.descricao, &pedido.valorTotal, pedido.status) == 8){       
        if(pedido.codigoPedido != 0){
            printw("Código do Pedido: %d | Cliente: %d | Produto: %d | Descrição: %s | Quantidade: %d | Valor: %.2f | Data: %s | Status: %s\n",
                    pedido.codigoPedido, pedido.codigoCliente, pedido.codigoProduto, pedido.descricao, pedido.quantidade, 
                    pedido.valorTotal, pedido.data, pedido.status);
        }
    }
            
    refresh();
    flushinp();
    getch();
}

int consultarPedido(FILE *fpPe, FILE *fpC){
    rewind(fpPe);
    
    int codigoCliente;
    
    clear();
    mvprintw(0, 0, "===== CONSULTAR PEDIDO =====");
    mvprintw(3, 5, "Informe o código do Cliente: ");
    
    echo();
    scanw("%d", &codigoCliente);
    noecho(); 
    
    if(!codigoClienteJaExiste(fpC, codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        flushinp();
        getch();
        return 0;
    }
    
    rewind(fpPe);
    Pedido pedido;
    PedidoMenu *pedidosDoCliente = NULL;
    int totalPedidosCliente = 0;
    
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c", &pedido.codigoPedido, &pedido.codigoCliente,
        &pedido.codigoProduto,&pedido.quantidade, pedido.data, pedido.descricao, &pedido.valorTotal, pedido.status) == 8){
        if(pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            totalPedidosCliente++;
        }
    }

    if(totalPedidosCliente == 0){
        printw("\nErro: Nenhum Pedido Encontrado para este cliente!\n");
        flushinp();
        getch();
        return 0;
    }
    
    pedidosDoCliente = (PedidoMenu*)malloc(totalPedidosCliente * sizeof(PedidoMenu));
    
    if (pedidosDoCliente == NULL) {
        mvprintw(5, 5, "Erro fatal: Falha ao alocar memória!");
        refresh();
        flushinp();
        getch();
        return -1;
    }

    rewind(fpPe);
    int i = 0;
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c",&pedido.codigoPedido, &pedido.codigoCliente, &pedido.codigoProduto,
        &pedido.quantidade,pedido.data, pedido.descricao, &pedido.valorTotal, pedido.status) == 8){
        if(pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            pedidosDoCliente[i].codigoPedido = pedido.codigoPedido;
            pedidosDoCliente[i].codigoCliente = pedido.codigoCliente;
            pedidosDoCliente[i].quantidade = pedido.quantidade;
            strncpy(pedidosDoCliente[i].descricao, pedido.descricao, maxDescricao);
            i++;
        }
    }
    
    int tecla;
    int codigoPedidoSelecionado = -1;
    int destaque = 0;
    
    while(codigoPedidoSelecionado == -1){
        clear();
        mvprintw(0, 0, "===== PEDIDOS DO CLIENTE %d =====", codigoCliente);
        mvprintw(2, 5, "Use as setas para navegar. Pressione ENTER para selecionar.");

        for (int j = 0; j < totalPedidosCliente; j++) {
            int x = 5;
            int y = 5 + j;

            if (j == destaque) {
                attron(A_REVERSE);
            }

            mvprintw(y, x, "Cód: %d | Desc: %s | Qtd: %d",
                     pedidosDoCliente[j].codigoPedido, pedidosDoCliente[j].descricao, pedidosDoCliente[j].quantidade);
            
            attroff(A_REVERSE);
        }
        
        refresh();
        tecla = getch();

        switch (tecla) {
            case KEY_UP:
                destaque = (destaque - 1 + totalPedidosCliente) % totalPedidosCliente;
                break;
            case KEY_DOWN:
                destaque = (destaque + 1) % totalPedidosCliente;
                break;
            case 10:       
            case KEY_ENTER: 
                codigoPedidoSelecionado = pedidosDoCliente[destaque].codigoPedido;
                break;
        }
    }
    
    free(pedidosDoCliente);
    clear();
    
    rewind(fpPe);
    
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %10[^;] ; %99[^;] ; %lf ; %19[^\n]%*c",&pedido.codigoPedido, &pedido.codigoCliente, &pedido.codigoProduto,
        &pedido.quantidade,pedido.data, pedido.descricao, &pedido.valorTotal, pedido.status) == 8){
        if(pedido.codigoPedido == codigoPedidoSelecionado){
            printw("\n=====DETALHES DO PEDIDO %d=====\n", pedido.codigoPedido);
            printw("Cliente: %d\nProduto: %d\nQuantidade: %d\nData: %s\nStatus: %s\nValor: %.2f\nDescrição: %s\n",
                    pedido.codigoCliente, pedido.codigoProduto, pedido.quantidade,
                    pedido.data, pedido.status, pedido.valorTotal, pedido.descricao);
            break;
        }
    }

    printw("\n\nPressione qualquer tecla para continuar...");
    refresh();
    flushinp();
    getch();
    return 1;
}

FILE* atualizarPedido(FILE *fpPe, FILE *fpC, FILE *fpP){
    int codigoCliente, mod, totalPedidosCliente = 0, opc = -1, escolhaModificacao = -1;
    Pedido pedido, pedidoModificado;
    PedidoMenu *pedidosDoCliente = NULL; 
    double novoPrecoUnitario = 0.0;
    
    clear();
    mvprintw(0, 0, "===== ATUALIZAR PEDIDO =====");
    mvprintw(3, 5, "Informe o código do Cliente: ");
    
    echo(); 
    scanw("%d", &codigoCliente);
    noecho();
    
    if(!codigoClienteJaExiste(fpC, codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return fpPe; 
    }

    rewind(fpPe);

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c",&pedido.codigoPedido, &pedido.codigoCliente, &pedido.codigoProduto,
        &pedido.quantidade, pedido.data, pedido.descricao, &pedido.valorTotal, pedido.status) == 8){
        if(pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            totalPedidosCliente++;
        }
    }

    if(totalPedidosCliente == 0){
        printw("\nErro: Nenhum Pedido Encontrado para este cliente!\n");
        flushinp();
        getch();
        return fpPe;
    }
    
    pedidosDoCliente = (PedidoMenu*)malloc(totalPedidosCliente * sizeof(PedidoMenu));

    if (pedidosDoCliente == NULL) {
        mvprintw(5, 5, "Erro fatal: Falha ao alocar memória!");
        getch();
        return fpPe;
    }

    rewind(fpPe);
    int i = 0;
    
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c",&pedido.codigoPedido, &pedido.codigoCliente, &pedido.codigoProduto,
        &pedido.quantidade,pedido.data, pedido.descricao, &pedido.valorTotal, pedido.status) == 8){
        if(pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            pedidosDoCliente[i].codigoPedido = pedido.codigoPedido;
            pedidosDoCliente[i].codigoCliente = pedido.codigoCliente;
            pedidosDoCliente[i].quantidade = pedido.quantidade;
            strncpy(pedidosDoCliente[i].descricao, pedido.descricao, maxDescricao);
            i++;
        }
    }

    int tecla;
    int destaque = 0;
    
    while(opc == -1){
        clear();
        mvprintw(0, 0, "===== SELECIONE O PEDIDO A MODIFICAR (Cliente: %d) =====", codigoCliente);
        mvprintw(2, 5, "Use as setas (cima/baixo) para navegar. ENTER para selecionar.");

        for (int j = 0; j < totalPedidosCliente; j++) {
            int x = 5;
            int y = 5 + j;

            if (j == destaque) {
                attron(A_REVERSE);
            }

            mvprintw(y, x, "Cód: %d | Desc: %s | Qtd: %d",pedidosDoCliente[j].codigoPedido, pedidosDoCliente[j].descricao,
                pedidosDoCliente[j].quantidade);
            
            attroff(A_REVERSE);
        }
        
        refresh();
        tecla = getch();

        switch (tecla) {
            case KEY_UP:
                destaque = (destaque - 1 + totalPedidosCliente) % totalPedidosCliente;
                break;
            case KEY_DOWN:
                destaque = (destaque + 1) % totalPedidosCliente;
                break;
            case 10: case KEY_ENTER:
                opc = pedidosDoCliente[destaque].codigoPedido;
                break;
        }
    }
    
    clear();
    rewind(fpPe);
    int pedidoEncontrado = 0;

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c",&pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,
        &pedido.quantidade,pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
        if (pedido.codigoPedido == opc && pedido.codigoCliente == codigoCliente) {
            pedidoModificado = pedido;
            pedidoEncontrado = 1;
            break;
        }
    }
    
    if(!pedidoEncontrado){
        printw("\nErro: Pedido não encontrado ou não pertence ao cliente (Código: %d)!\n", opc);
        flushinp();
        getch();
        if (pedidosDoCliente != NULL) free(pedidosDoCliente);
        return fpPe; 
    }
    
    char *opcoesModificacao[] = {
        "1) Codigo do Produto",
        "2) Quantidade",
        "3) Descricao",
        "4) Status"
    };
    int numOpcoesMod = 4;
    destaque = 0;
    
    while(escolhaModificacao == -1){
        clear();
        mvprintw(0, 0, "===== MODIFICAR PEDIDO %d =====", opc);
        mvprintw(2, 5, "Qual campo deseja modificar? Use as setas e ENTER.");
        
        for (int k = 0; k < numOpcoesMod; k++) {
            int x = 5;
            int y = 5 + k;

            if (k == destaque) {
                attron(A_REVERSE);
            }

            mvprintw(y, x, "%s", opcoesModificacao[k]);
            attroff(A_REVERSE);
        }
        
        refresh();
        tecla = getch();

        switch (tecla) {
            case KEY_UP:
                destaque = (destaque - 1 + numOpcoesMod) % numOpcoesMod;
                break;
            case KEY_DOWN:
                destaque = (destaque + 1) % numOpcoesMod;
                break;
            case 10: case KEY_ENTER:
                escolhaModificacao = destaque + 1;
                break;
        }
    }
    
    clear();
    mod = escolhaModificacao; 

    switch(mod){
        case 1:
            mvprintw(0, 0, "Novo código do produto:\n");
            echo();
            scanw("%d", &pedidoModificado.codigoProduto);
            noecho();
            obterDadosProduto(fpP, pedidoModificado.codigoProduto, pedidoModificado.descricao, &novoPrecoUnitario);
            pedidoModificado.valorTotal = novoPrecoUnitario * pedidoModificado.quantidade;
            break;
        case 2:
            mvprintw(0, 0, "Nova quantidade:\n");
            echo();
            scanw("%d", &pedidoModificado.quantidade);
            noecho();
            obterDadosProduto(fpP, pedidoModificado.codigoProduto, pedidoModificado.descricao, &novoPrecoUnitario);
            pedidoModificado.valorTotal = novoPrecoUnitario * pedidoModificado.quantidade;
            break;
        case 3:
            mvprintw(0, 0, "Nova descrição:\n");
            echo();
            scanw("%s", pedidoModificado.descricao); 
            noecho();
            break;
        case 4:
            mvprintw(0, 0, "Novo status:\n");
            echo();
            scanw("%s", pedidoModificado.status);
            noecho();
            break;
    }

    if (mod >= 1 && mod <= 4) {
    obterDataAtual(pedidoModificado.data, maxData);
    }

    FILE *fpTemp = fopen("pedidos_temp.csv", "w");
    if (fpTemp == NULL) {
        printw("\nErro: Nao foi possivel criar arquivo temporario para atualizacao.\n");
        flushinp();
        getch();
        if (pedidosDoCliente != NULL) free(pedidosDoCliente);
        return fpPe; 
    }
    
    rewind(fpPe); 
    
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
        if (pedido.codigoPedido == opc) {
            fprintf(fpTemp, "%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n", 
                     pedidoModificado.codigoPedido, pedidoModificado.codigoCliente, 
                     pedidoModificado.codigoProduto, pedidoModificado.quantidade, 
                     pedidoModificado.data, pedidoModificado.descricao, 
                     pedidoModificado.valorTotal, pedidoModificado.status);
        } else {
            fprintf(fpTemp, "%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n", 
                     pedido.codigoPedido, pedido.codigoCliente, 
                     pedido.codigoProduto, pedido.quantidade, 
                     pedido.data, pedido.descricao, pedido.valorTotal, pedido.status);
        }
    }
    
    fclose(fpPe); 
    fclose(fpTemp);

    int resultado = 0;

    if (remove(ARQUIVO_PEDIDOS) == 0 && rename("pedidos_temp.csv", ARQUIVO_PEDIDOS) == 0) {
        resultado = 1;
        clear();
        mvprintw(0, 0, "\nAlteração feita com sucesso!\n");
        mvprintw(2, 0, "Pressione qualquer tecla para retornar ao menu.");
        refresh();
        getch(); 
    } else {
        clear();
        mvprintw(0, 0, "ERRO: Falha ao finalizar a atualização do arquivo.");
        getch();
    }
    if (pedidosDoCliente != NULL) free(pedidosDoCliente);
    return fpPe;
}

FILE* deletarPedido(FILE *fpPe, FILE *fpC){
    rewind(fpPe);
    
    int codigoCliente, opc, encontro = 0;
    Pedido pedido;
    
    clear();
    mvprintw(0, 0, "===== EXCLUIR PEDIDO =====");
    mvprintw(3, 5, "Informe o código do Cliente dono do pedido:");
    refresh();
    echo();
    scanw("%d", &codigoCliente);
    noecho();
    
    if(!codigoClienteJaExiste(fpC,codigoCliente)){
        printw("\nErro: Cliente não encontrado!\n");
        getch();
        return fpPe;
    }
    
    clear();
    printw("\n=====PEDIDOS DO CLIENTE %d=====\n", codigoCliente);
    
    rewind(fpPe);

    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
        if(pedido.codigoCliente == codigoCliente && pedido.codigoPedido != 0){
            printw("\nCódigo do Pedido: %d | Descrição: %s | Quantidade: %d\n",
                     pedido.codigoPedido,pedido.descricao,pedido.quantidade);
            encontro = 1;
        }
    }
    if(encontro == 0){
        printw("\nErro: Nenhum Pedido Encontrado!\n");
        flushinp();
        getch();
        return fpPe;
    }
    
    printw("\nInforme o código do pedido que você deseja desfazer:\n");
    refresh();
    echo();
    scanw("%d", &opc);
    noecho();

    FILE *fpTemp = fopen("pedidos_temp.csv", "w"); 
    if (fpTemp == NULL) {
        printw("\nErro: Nao foi possivel criar arquivo temporario.\n");
        flushinp();
        getch();
        return fpPe;
    }
    
    rewind(fpPe);
    int pedidoDeletado = 0;
    
    while(fscanf(fpPe, "%d ; %d ; %d ; %d ; %[^;] ; %[^;] ; %lf ; %[^\n]%*c",
                 &pedido.codigoPedido,&pedido.codigoCliente,&pedido.codigoProduto,&pedido.quantidade,
                 pedido.data,pedido.descricao,&pedido.valorTotal,pedido.status) == 8){
        if (pedido.codigoPedido != opc) {
            fprintf(fpTemp, "%d ; %d ; %d ; %d ; %s ; %s ; %.2f ; %s\n", 
                    pedido.codigoPedido, pedido.codigoCliente, 
                    pedido.codigoProduto, pedido.quantidade, 
                    pedido.data, pedido.descricao, pedido.valorTotal, pedido.status);
        } else {
            pedidoDeletado = 1; 
        }
    }

    fclose(fpPe);
    fclose(fpTemp);

    remove(ARQUIVO_PEDIDOS);
    rename("pedidos_temp.csv",ARQUIVO_PEDIDOS);

    fpPe = fopen(ARQUIVO_PEDIDOS, "a+");
    if (fpPe == NULL) {
         printw("\nERRO CRÍTICO: Nao foi possivel reabrir o arquivo de pedidos.\n");
         refresh();
         getch();
         exit(1); 
    }
    if (pedidoDeletado) {
        printw("\nO pedido %d foi desfeito com sucesso!\n", opc);
    } else {
         printw("\nErro: Pedido %d nao encontrado!\n", opc);
    }

    refresh();
    flushinp();
    getch();
    return fpPe;
}