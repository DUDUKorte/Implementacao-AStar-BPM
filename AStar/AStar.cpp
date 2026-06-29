/*
    Código que implementa o algorítmo de busca A* em labirintos para a matéria de IA desenvolvido pela dupla:
    . Rafael Loureiro Fockink
    . Eduardo Nogueira Korte

    Equipe do trabalho:
    . Rafael Loureiro Fockink,
    . Vitor Krieser Roehrs
    . Eduardo Nogueira Korte
    . Felipe Kuznik Thome
*/

// Aqui vou utilizar funções já implementadas
// pelos colegas da outra dupla para ler e imprimir os labirintos.

#include "AStar.h"

namespace AStar {

    // Funcção que calcula a heurística da distância do ponto atual até a saída
    int heuristica(Posicao atual, Posicao saida) {
        return abs(atual.x - saida.x) + abs(atual.y - saida.y);
    }

    std::vector<Posicao> reconstruir_caminho(const std::list<No>& fechada, No noFinal, Posicao entrada) {

        std::vector<Posicao> caminho;

        No atual = noFinal;
        caminho.push_back(atual.posicao);

        while (!atual.posicao.eh_igual(entrada)) {

            if (!atual.temPai) {
                break;
            }

            Posicao posPai = atual.pai;
            No noPai(Posicao{ -1, -1 });

            bool achouPai = obter_no_da_lista(fechada, posPai, noPai);

            if (!achouPai) {
                break;
            }

            atual = noPai;
            caminho.push_back(atual.posicao);
        }

        // inverte a ordem da lista para não precisar percorer o caminho de trás para frente
        reverse(caminho.begin(), caminho.end());

        return caminho;
    }


    // [Modularização/Interface]
    // AStar_init inicializa o algoritmo e prepara a lista aberta, lista fechada, 
    // posições de entrada/saída e métricas iniciais
    // Basicamente só copei o código do A* e adicionei a estrutura do Estado kk
    EstadoAStar Init(const Matriz& LABIRINTO, int numero_do_labirinto) {
        // [Dados] Aqui crio a estrutura para armazenar as informações e no final retornar ela
        EstadoAStar estado;

        // [Dados] Armazeno o labirinto dentro da estrutura do AStar
        estado.labirinto = &LABIRINTO;

        // [Dado] ínicio da medição do tempo de execução
        estado.inicio_tempo = std::chrono::high_resolution_clock::now();

        // [Dado] Armazeno informações do arquivo/Labirinto
        estado.dados.numero_labirinto = numero_do_labirinto;

        // [Inicialização A*] Inicializa a posição da entrada do labirinto
        estado.entrada.x = 0;
        estado.entrada.y = 1;

        // [Inicialização A*] Pega as dimensões do labirinto e inicializa a posição de saída do labirinto
        int n = LABIRINTO.size();
        int m = LABIRINTO[0].size();

        estado.saida.x = n - 1;
        estado.saida.y = m - 2;

        // [Dado] armazena o tam do labirinto e posições entrada/saída
        estado.dados.linhas = n;
        estado.dados.colunas = m;
        estado.dados.entrada = estado.entrada;
        estado.dados.saida = estado.saida;

        // [Inicialização A*] Calcula a heurística da entrada até a saída
        int hInicial = heuristica(estado.entrada, estado.saida);

        // [Inicialização A*] Cria o nó inicial
        estado.atual = No(estado.entrada, 0, hInicial);

        // [Dados] Conta o total de nós criados/gerados na árvore
        // Começa em 1 porque o nó inicial já foi criado
        estado.dados.nos_gerados = 1;

        // [Dado] Inicialização das métricas complementares
        estado.dados.nos_expandidos = 0;
        estado.dados.nos_visitados = 0;
        estado.dados.tamanho_max_lista_aberta = 1;
        estado.dados.tamanho_max_lista_fechada = 0;

        // [A*]
        // [Pseudocódigo] faça uma lista aberta contendo apenas o nó inicial
        // Lista aberta já foi criada dentro da estrutura
        estado.aberta.push_back(estado.atual);

        // [A*]
        // [Pseudocódigo] faça uma lista fechada vazia
        // Como a lista fechada está dentro da struct EstadoAStar, ela já começa vazia.

        // [Controle A*] Variável para controlar se o destino foi alcançado
        estado.destino_alcancado = false;
        estado.finalizado = false;

        return estado;
    }

    // [Modularização/Interface]
    // AStar_inc executa UMA iteração do algoritmo A*
    // Retorna true quando o algoritmo terminou e false quando ainda precisa continuar
    // Basicamente só copei o código do A* e adicionei a estrutura do Estado kk
    bool Increment(EstadoAStar& estado) {
        // Se o algoritmo já terminou, não faz mais nada.
        if (estado.finalizado) {
            return true;
        }

        // [A*]
        // [Pseudocódigo] enquanto (o nó de destino não foi alcançado):
        // Esta função representa uma única execução do corpo do while.

        // [Verifica Erro] Se a lista aberta ficou vazia, não existe caminho até a saída
        if (estado.aberta.empty()) {
            // [Dados] Finaliza medição do tempo
            auto fimTempo = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duracao = fimTempo - estado.inicio_tempo;

            // [Dados] Armazena métricas finais em caso de falha
            estado.dados.tempo_execucao_ms = duracao.count();
            estado.dados.resultado = FALHA;
            estado.dados.encontrou_solucao = false;
            estado.dados.lista_fechada = estado.fechada;
            estado.dados.lista_aberta_final = estado.aberta;
            estado.dados.nos_visitados = estado.fechada.size();
            estado.dados.tamanho_max_lista_fechada = std::max(estado.dados.tamanho_max_lista_fechada, (int)estado.fechada.size());

            // [Dados] Calcula fator de ramificação médio
            // Usa nos_expandidos para evitar divisão por zero.
            if (estado.dados.nos_expandidos > 0) {
                estado.dados.fator_ramificacao_medio = (double)(estado.dados.nos_gerados - 1) / estado.dados.nos_expandidos;
            }

            estado.finalizado = true;
            estado.destino_alcancado = false;

            //cout << "Terminou com FALHA: nao existe caminho ate a saida." << endl;
            //cout << "Total de nos gerados: " << estado.dados.nos_gerados << endl;
            //cout << "Total de nos expandidos: " << estado.dados.nos_expandidos << endl;
            //cout << "Tempo de execucao: " << fixed << setprecision(4)
            //    << estado.dados.tempo_execucao_ms << " ms" << endl;

            return true;
        }

        // [A*]
        // [Pseudocódigo] considere o nó com a menor pontuação f na lista aberta
        estado.atual = retirar_melhor_f(estado.aberta);

        //cout << "Atual: ("
        //    << estado.atual.posicao.x << ", "
        //    << estado.atual.posicao.y << ") "
        //    << "g=" << estado.atual.g << " "
        //    << "h=" << estado.atual.h << " "
        //    << "f=" << estado.atual.f << endl;

        // [A*]
        // [Pseudocódigo] se (este nó é o nó de destino):
        if (estado.atual.posicao.eh_igual(estado.saida)) {

            // [Dado] Finalizo a medição do tempo
            auto fimTempo = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duracao = fimTempo - estado.inicio_tempo;

            // [Dado] Armazena resultado e métricas principais
            estado.dados.tempo_execucao_ms = duracao.count();
            estado.dados.resultado = SUCESSO;
            estado.dados.encontrou_solucao = true;
            estado.dados.custo_solucao = estado.atual.g;

            // [Dado] Reconstroe e armazena o caminho para a interface
            estado.dados.caminho_solucao = reconstruir_caminho(estado.fechada, estado.atual, estado.entrada);
            estado.dados.tamanho_caminho = estado.dados.caminho_solucao.size();

            // [Dado] Calcula profundidade da solução
            if (estado.dados.tamanho_caminho > 0) {
                estado.dados.profundidade_solucao = estado.dados.tamanho_caminho - 1;
            }

            // [Dado] Armazena listas finais para análise/interface
            estado.dados.lista_fechada = estado.fechada;
            estado.dados.lista_aberta_final = estado.aberta;
            estado.dados.nos_visitados = estado.fechada.size();
            estado.dados.tamanho_max_lista_fechada = std::max(estado.dados.tamanho_max_lista_fechada, (int)estado.fechada.size());

            // [Dado] Calcula fator de ramificação médio
            if (estado.dados.nos_expandidos > 0) {
                estado.dados.fator_ramificacao_medio = (double)(estado.dados.nos_gerados - 1) / estado.dados.nos_expandidos;
            }

            // [Dado] Armazena que acabou com sucesso para não deixar mais incrementar
            estado.finalizado = true;
            estado.destino_alcancado = true;

            //cout << "\nTerminou com SUCESSO!!!!!!" << endl;
            //cout << "Total de nos gerados: " << estado.dados.nos_gerados << endl;
            //cout << "Total de nos expandidos: " << estado.dados.nos_expandidos << endl;
            //cout << "Total de nos visitados: " << estado.dados.nos_visitados << endl;
            //cout << "Custo da solucao: " << estado.dados.custo_solucao << endl;
            //cout << "Profundidade da solucao: " << estado.dados.profundidade_solucao << endl;
            //cout << "Tempo de execucao: " << fixed << setprecision(4)
            //    << estado.dados.tempo_execucao_ms << " ms" << endl;
            //cout << "Fator de ramificacao medio: " << fixed << setprecision(4)
            //    << estado.dados.fator_ramificacao_medio << endl;

            // [Representação visual A*] Printa o labirinto final com . mostrando o caminho
            //imprimir_labirinto_caminho(*estado.labirinto, estado.fechada, estado.atual, estado.entrada);

            return true;
        }

        // [A*]
        // [Pseudocódigo] senão:
        else {
            // [A*]
            // [Pseudocódigo] coloque o nó atual na lista fechada e observe todos os seus vizinhos
            estado.fechada.push_back(estado.atual);

            // [Dado] O nó atual será expandido, pois seus vizinhos serão analisados
            estado.dados.nos_expandidos++;

            // [Dado] Atualiza métricas da lista fechada
            estado.dados.nos_visitados = estado.fechada.size();
            estado.dados.tamanho_max_lista_fechada = std::max(estado.dados.tamanho_max_lista_fechada, (int)estado.fechada.size());

            // [Inicialização A*] Possiveis direções/vizinhos do nó atual
            // [Pseudocódigo] e observe todos os seus vizinhos
            std::vector<Posicao> direcoes = {
                {-1, 0}, // Cima
                {1,  0}, // Baixo
                {0, -1}, // Esquerda
                {0,  1}  // Direita
            };

            // [A*]
            // [Pseudocódigo] para (cada vizinho do nó atual):
            for (Posicao direcao : direcoes) {
                Posicao posVizinho;
                posVizinho.x = estado.atual.posicao.x + direcao.x;
                posVizinho.y = estado.atual.posicao.y + direcao.y;

                // [Verificações A*]
                // Se o vizinho não está dentro do labirinto ou é parede,
                // pula para o próximo vizinho
                if (!posVizinho.eh_valida(*estado.labirinto)) {
                    continue;
                }

                // [A*]
                // Calcula o novo valor de g do vizinho
                int novoG = estado.atual.g + 1;

                // [A*]
                // Calcula o novo valor de h do vizinho
                int novoH = heuristica(posVizinho, estado.saida);

                // [A*]
                // Verifica se esse vizinho está na lista fechada
                bool vizinhoEstaNaFechada = esta_na_lista(estado.fechada, posVizinho);

                // [A*]
                // Verifica se esse vizinho está na lista aberta
                bool vizinhoEstaNaAberta = esta_na_lista(estado.aberta, posVizinho);

                // [A*]
                // [Pseudocódigo] se (vizinho tem um valor g menor que o atual e está na lista fechada):
                if (vizinhoEstaNaFechada &&
                    substituir_se_g_menor(estado.fechada, posVizinho, novoG, novoH, estado.atual.posicao)) {

                    // [Pseudocódigo] - Substitua o vizinho pelo novo valor mais baixo de g
                    // Isso já foi feito dentro da função substituir_se_g_menor que também retorna a verificação
                    // [Pseudocódigo] - O nó atual agora é o pai do vizinho
                    // Isso também já foi feito dentro da função substituir_se_g_menor
                }

                // [A*]
                // [Pseudocódigo] caso contrário
                else {
                    // [A*]
                    // [Pseudocódigo] se (o valor atual de g for menor e esse vizinho estiver na lista aberta):
                    if (vizinhoEstaNaAberta &&
                        substituir_se_g_menor(estado.aberta, posVizinho, novoG, novoH, estado.atual.posicao)) {

                        // [Pseudocódigo] - Substitua o vizinho pelo novo valor mais baixo de g
                        // Isso já foi feito dentro da função substituir_se_g_menor que também retorna a verificação
                        // [Pseudocódigo] - Alterar o pai do vizinho para o nosso nó atual
                        // Isso também já foi feito dentro da função substituir_se_g_menor
                    }

                    // [A*]
                    // [Pseudocódigo] caso contrário
                    else {
                        // [A*]
                        // [Pseudocódigo] se esse vizinho não estiver nas duas listas:
                        if (!vizinhoEstaNaFechada && !vizinhoEstaNaAberta) {
                            // [A*]
                            // [Pseudocódigo] - Adicione-o à lista aberta e defina seu g.
                            No vizinho(posVizinho, novoG, novoH, estado.atual.posicao, true);
                            estado.aberta.push_back(vizinho);

                            // [Dados] Conta mais um nó gerado
                            estado.dados.nos_gerados++;
                        }
                    }
                }
            }

            // [Dados] Atualiza o maior tamanho atingido pela lista aberta
            estado.dados.tamanho_max_lista_aberta = std::max(estado.dados.tamanho_max_lista_aberta, (int)estado.aberta.size());
        }

        return false;
    }

    // [Modularização/Teste]
    // Função que executa o algortimo de Busca do A* com as funções modulares
    DadosUteis AStarMod(const Matriz& LABIRINTO, Posicao entrada, Posicao saida, int numero_do_labirinto) {
        EstadoAStar estado = Init(LABIRINTO, numero_do_labirinto);

        // Mantive a entrada e saída para a "Assinatura" ficar igual a da versão original
        entrada = estado.entrada;
        saida = estado.saida;

        while (!estado.finalizado) {
            Increment(estado);
        }

        return estado.dados;
    }

    void CalcularMedias(const Matriz& LABIRINTO, int quantidadeLabirintos){
        double somaNosGerados = 0;
        double somaNosExpandidos = 0;
        double somaTempo = 0;
        double somaCusto = 0;
        double somaProfundidade = 0;

        int executados = 0;

        for (int i = 1; i <= quantidadeLabirintos; i++) {

            Matriz labirinto = LABIRINTO;

            if (labirinto.empty())
                continue;

            Posicao entrada{};
            Posicao saida{};

            DadosUteis dados =
                AStarMod(labirinto, entrada, saida, i);

            somaNosGerados += dados.nos_gerados;
            somaNosExpandidos += dados.nos_expandidos;
            somaTempo += dados.tempo_execucao_ms;

            if (dados.encontrou_solucao) {
                somaCusto += dados.custo_solucao;
                somaProfundidade += dados.profundidade_solucao;
            }

            executados++;
        }

        std::cout << "\n========== MÉDIAS A* ==========\n";

        std::cout << "Labirintos analisados: "
            << executados << std::endl;

        std::cout << "Média nós gerados: "
            << somaNosGerados / executados << std::endl;

        std::cout << "Média nós expandidos: "
            << somaNosExpandidos / executados << std::endl;

        std::cout << "Média tempo (ms): "
            << somaTempo / executados << std::endl;

        std::cout << "Média custo solução: "
            << somaCusto / executados << std::endl;

        std::cout << "Média profundidade: "
            << somaProfundidade / executados << std::endl;
    }

    // Função que executa o algoritmo de busca do A* do tipo Dados Uteis para retornar os resultados,
    // métricas e dados para interface e analise
    DadosUteis AStarObsolete(const Matriz& LABIRINTO, Posicao entrada, Posicao saida, std::string nome_do_arquivo, int numero_do_labirinto) {

        // [Dados] Aqui crio a estrutura para armazenar as informações e no final retornar ela
        DadosUteis dados;

        // [Dado] ínicio da mediçãodo tempo de execução
        auto inicioTempo = std::chrono::high_resolution_clock::now();

        // [Dado] Armazeno informações do arquivo/Labirinto
        dados.nome_arquivo = nome_do_arquivo;
        dados.numero_labirinto = numero_do_labirinto;

        // [Inicialização A*] Inicializa a posição da entrada do labirinto
        entrada.x = 0;
        entrada.y = 1;

        // [Inicialização A*] Pega as dimensões do labirinto e inicializa a posição de saída do labirinto
        int n = LABIRINTO.size();
        int m = LABIRINTO[0].size();

        saida.x = n - 1;
        saida.y = m - 2;

        // [Dado] armazena o tam do labirinto e posições entrada/saída
        dados.linhas = n;
        dados.colunas = m;
        dados.entrada = entrada;
        dados.saida = saida;

        // [Inicialização A*] Calcula a heurística da entrada até a saída
        int hInicial = heuristica(entrada, saida);

        // [Inicialização A*] Cria o nó inicial
        No atual(entrada, 0, hInicial);

        // [Dados] Conta o total de nós criados/gerados na árvore
        // Começa em 1 porque o nó inicial já foi criado
        dados.nos_gerados = 1;

        // [Dado] Inicialização das métricas complementares
        dados.nos_expandidos = 0;
        dados.nos_visitados = 0;
        dados.tamanho_max_lista_aberta = 1;
        dados.tamanho_max_lista_fechada = 0;

        // [A*]
        // [Pseudocódigo] faça uma lista aberta contendo apenas o nó inicial
        std::list<No> aberta;
        aberta.push_back(atual);

        // [A*]
        // [Pseudocódigo] faça uma lista fechada vazia
        std::list<No> fechada;

        // [Controle A*] Variável para controlar se o destino foi alcançado
        bool destinoAlcancado = false;

        // [A*]
        // [Pseudocódigo] enquanto (o nó de destino não foi alcançado):
        while (!destinoAlcancado) {

            // [Verifica Erro] Se a lista aberta ficou vazia, não existe caminho até a saída
            if (aberta.empty()) {
                // [Dados] Finaliza medição do tempo
                auto fimTempo = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duracao = fimTempo - inicioTempo;

                // [Dados] Armazena métrocas finais em caso de falha
                dados.tempo_execucao_ms = duracao.count();
                dados.resultado = FALHA;
                dados.encontrou_solucao = false;
                dados.lista_fechada = fechada;
                dados.lista_aberta_final = aberta;
                dados.nos_visitados = fechada.size();
                dados.tamanho_max_lista_fechada = std::max(dados.tamanho_max_lista_fechada, (int)fechada.size());

                // [Dados] Calcula fator de ramificação médio
                if (dados.nos_gerados > 0) {
                    dados.fator_ramificacao_medio = (double)(dados.nos_gerados - 1) / dados.nos_expandidos;
                }

                //cout << "Terminou com FALHA: nao existe caminho ate a saida." << endl;
                //cout << "Total de nos gerados: " << dados.nos_gerados << endl;
                //cout << "Total de nos expandidos: " << dados.nos_expandidos << endl;
                //cout << "Tempo de execucao: " << fixed << setprecision(4)
                //    << dados.tempo_execucao_ms << " ms" << endl;

                return dados;

            }

            // [A*]
            // [Pseudocódigo] considere o nó com a menor pontuação f na lista aberta
            atual = retirar_melhor_f(aberta);

            //cout << "Atual: ("
            //    << atual.posicao.x << ", "
            //    << atual.posicao.y << ") "
            //    << "g=" << atual.g << " "
            //    << "h=" << atual.h << " "
            //    << "f=" << atual.f << endl;

            // [A*]
            // [Pseudocódigo] se (este nó é o nó de destino):
            if (atual.posicao.eh_igual(saida)) {

                // [Dado] Finalizo a medição do tempo
                auto fimTempo = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duracao = fimTempo - inicioTempo;

                // [Dado] Armazena resultado e métricas principais
                dados.tempo_execucao_ms = duracao.count();
                dados.resultado = SUCESSO;
                dados.encontrou_solucao = true;
                dados.custo_solucao = atual.g;

                // [Dado] Reconstroe e armazena o caminho apra a interface
                dados.caminho_solucao = reconstruir_caminho(fechada, atual, entrada);
                dados.tamanho_caminho = dados.caminho_solucao.size();

                // [Dado] Calcula profundidade da solução
                if (dados.tamanho_caminho > 0) {
                    dados.profundidade_solucao = dados.tamanho_caminho - 1;
                }

                // [Dado] Armazena listas finais para análise/interface
                dados.lista_fechada = fechada;
                dados.lista_aberta_final = aberta;
                dados.nos_visitados = fechada.size();

                // [Dado] Calcula fator deramificação médio
                if (dados.nos_expandidos > 0) {
                    dados.fator_ramificacao_medio = (double)(dados.nos_gerados - 1) / dados.nos_expandidos;
                }


                //cout << "\nTerminou com SUCESSO!!!!!!" << endl;
                //cout << "Total de nos gerados: " << dados.nos_gerados << endl;
                //cout << "Total de nos expandidos: " << dados.nos_expandidos << endl;
                //cout << "Total de nos visitados: " << dados.nos_visitados << endl;
                //cout << "Custo da solucao: " << dados.custo_solucao << endl;
                //cout << "Profundidade da solucao: " << dados.profundidade_solucao << endl;
                //cout << "Tempo de execucao: " << fixed << setprecision(4)
                //    << dados.tempo_execucao_ms << " ms" << endl;
                //cout << "Fator de ramificacao medio: " << fixed << setprecision(4)
                //    << dados.fator_ramificacao_medio << endl;


                // [Função auxiliar]
                // [Representação visual A*] Printa o labirinto final com . mostrando o caminho
                //imprimir_labirinto_caminho(LABIRINTO, fechada, atual, entrada);

                destinoAlcancado = true;

                return dados;
            }

            // [A*]
            // [Pseudocódigo] senão:
            else {

                // [A*]
                // [Pseudocódigo] coloque o nó atual na lista fechada e observe todos os seus vizinhos
                fechada.push_back(atual);

                // [Dado] O nó atual será expandido, pois seus vizinhos serão "adentadros"
                dados.nos_expandidos++;

                // [Dado] Atualiza métricas da lista fechada
                dados.nos_visitados = fechada.size();
                dados.tamanho_max_lista_fechada = std::max(dados.tamanho_max_lista_fechada, (int)fechada.size());

                // [Inicialização A*] Possiveis direções/vizinhos do nó atual
                // [Pseudocódigo] e observe todos os seus vizinhos
                std::vector<Posicao> direcoes = {
                    {-1, 0}, // Cima
                    {1,  0}, // Baixo
                    {0, -1}, // Esquerda
                    {0,  1}  // Direita
                };

                // [A*]
                // [Pseudocódigo] para (cada vizinho do nó atual):
                for (Posicao direcao : direcoes) {

                    Posicao posVizinho;

                    posVizinho.x = atual.posicao.x + direcao.x;
                    posVizinho.y = atual.posicao.y + direcao.y;

                    // [Verificações A*]
                    // Se o vizinho não está dentro do labirinto ou é parede,
                    // pula para o próximo vizinho
                    if (!posVizinho.eh_valida(LABIRINTO)) {
                        continue;
                    }

                    // [A*]
                    // Calcula o novo valor de g do vizinho
                    int novoG = atual.g + 1;

                    // [A*]
                    // Calcula o novo valor de h do vizinho
                    int novoH = heuristica(posVizinho, saida);

                    // [A*]
                    // Verifica se esse vizinho está na lista fechada
                    bool vizinhoEstaNaFechada = esta_na_lista(fechada, posVizinho);

                    // [A*]
                    // Verifica se esse vizinho está na lista aberta
                    bool vizinhoEstaNaAberta = esta_na_lista(aberta, posVizinho);

                    // [A*]
                    // [Pseudocódigo] se (vizinho tem um valor g menor que o atual e está na lista fechada):
                    if (vizinhoEstaNaFechada &&
                        substituir_se_g_menor(fechada, posVizinho, novoG, novoH, atual.posicao)) {

                        // [Pseudocódigo] - Substitua o vizinho pelo novo valor mais baixo de g
                        // Isso já foi feito dentro da função substituir_se_g_menor que também retorna a verificação

                        // [Pseudocódigo] - O nó atual agora é o pai do vizinho
                        // Isso também já foi feito dentro da função substituir_se_g_menor
                    }

                    // [A*]
                    // [Pseudocódigo] caso contrário
                    else {

                        // [A*]
                        // [Pseudocódigo] se (o valor atual de g for menor e esse vizinho estiver na lista aberta):
                        if (vizinhoEstaNaAberta &&
                            substituir_se_g_menor(aberta, posVizinho, novoG, novoH, atual.posicao)) {

                            // [Pseudocódigo] - Substitua o vizinho pelo novo valor mais baixo de g
                            // Isso já foi feito dentro da função substituir_se_g_menor que também retorna a verificação

                            // [Pseudocódigo] - Alterar o pai do vizinho para o nosso nó atual
                            // Isso também já foi feito dentro da função substituir_se_g_menor
                        }

                        // [A*]
                        // [Pseudocódigo] caso contrário
                        else {

                            // [A*]
                            // [Pseudocódigo] se esse vizinho não estiver nas duas listas:
                            if (!vizinhoEstaNaFechada && !vizinhoEstaNaAberta) {

                                // [A*]
                                // [Pseudocódigo] - Adicione-o à lista aberta e defina seu g.
                                No vizinho(posVizinho, novoG, novoH, atual.posicao, true);
                                aberta.push_back(vizinho);

                                // [Dados] Conta mais um nó gerado
                                dados.nos_gerados++;
                            }
                        }
                    }
                }

                // [Dados] Atualiza o maior tamanho atingido pela lista aberta
                dados.tamanho_max_lista_aberta = std::max(dados.tamanho_max_lista_aberta, (int)aberta.size());
            }
        }

        // [Dados] Retorno que NÃO É PARA OCORRER
        dados.resultado = FALHA;
        dados.encontrou_solucao = false;

        return dados;
    }

}