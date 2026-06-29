/*
    Código que implementa o algorítmo de busca BPM em labirintos para a matéria de IA desenvolvido pela dupla:
    . Vitor Krieser Roehrs
    . Felipe Kuznik Thome

    Equipe do trabalho:
    . Rafael Loureiro Fockink,
    . Vitor Krieser Roehrs
    . Eduardo Nogueira Korte
    . Felipe Kuznik Thome
*/

#include "BPM.h"

namespace BPM {
    // Uma matriz para saber nos visitados, com 'n' linhas com 'm' colunas, tudo falso
    std::vector<std::vector<bool>> visitados;
    No* raiz;
    No* noEstado;
    std::vector<No*> fila;

    // Entrada do labirinto, saida do labirinto e nos criados pelo algoritmo
    Posicao entrada = { 0, 1 }; // entrada fixa
    Posicao saida;            // 
    int nosCriados = 0;       // Nos criados é global

    //////////////////////////////////////////////////////////////////////////
    //////////// HEURISTICA (Distancia Euclidiana ao Quadrado) ///////////////
    //////////////////////////////////////////////////////////////////////////

    double calcularHeuristica(Posicao atual, Posicao destino) {
        // Calculo de distancia simples
        int dx = atual.x - destino.x;
        int dy = atual.y - destino.y;

        return (dx * dx) + (dy * dy);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////
    //////////// OBTER SUCESSORES DO NÓ ATUAL ///////////////
    /////////////////////////////////////////////////////////

    std::vector<No*> obter_sucessores(No* noAtual, const Matriz& labirinto, const std::vector<std::vector<bool>>& visitados) {
        std::vector<No*> sucessores;            // Vetor de sucessores
        int linhas = labirinto.size();     // tamanho das linhas do labirinto 
        int colunas = labirinto[0].size(); // tamanho das colunas do labirinto

        // Movimentos permitidos, baixo, cima, direita e esquerda 
        int dx[] = { 1, -1, 0, 0 };
        int dy[] = { 0, 0, 1, -1 };

        // Verifica as 4 posições possiveis
        for (int i = 0; i < 4; i++) {
            int novoX = noAtual->estado.x + dx[i];
            int novoY = noAtual->estado.y + dy[i];

            // Testa se não estamos fora do labirinto, uma posição invalida
            if (novoX >= 0 && novoX < linhas && novoY >= 0 && novoY < colunas) {

                // Ele é um caminho possivel e nunca foi visitado
                if (labirinto[novoX][novoY] == 0 && visitados[novoX][novoY] == false) {
                    No* novoNo = new No({ novoX, novoY }, noAtual);
                    nosCriados++;                 // +1 nó criado
                    sucessores.push_back(novoNo); // coloca o novo nó no vetor de sucessores, logo ele é uma opção
                }
            }
        }
        return sucessores;
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    //////////// ORDENAÇÃO DA FILA (PRIORIDADE) ///////////////
    ///////////////////////////////////////////////////////////


    void ordenar_fila(std::vector<No*>& fila) {
        // Usando ordenação sort para ordenar a fila para quem tiver a menor distancia ser o primeiro a ser escolhido
        std::sort(fila.begin(), fila.end(), [](No* a, No* b) {
            return calcularHeuristica(a->estado, saida) < calcularHeuristica(b->estado, saida);
            });
    }
    ///////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    //////////// ALGORITMO DE BUSCA DO PRIMEIRO MELHOR ///////////////
    //////////////////////////////////////////////////////////////////

    DadosUteis Init(const Matriz& labirinto) {
        DadosUteis nova_metricas;
        visitados.clear();
        fila.clear();

        // Linha e coluna da matriz
        int n = labirinto.size();
        int m = labirinto[0].size();
        saida = { n - 1, m - 2 }; // Saida sempre fixa

        entrada = { 0, 1 };

        visitados.assign(n, std::vector<bool>(m, false));

        // Metricas que serão guardadas
        nova_metricas.linhas = n;
        nova_metricas.colunas = m;
        nova_metricas.custo_solucao = -1;
        nova_metricas.profundidade_solucao = -1;
        nova_metricas.tamanho_caminho = 0;
        nova_metricas.encontrou_solucao = false;
        nova_metricas.resultado = FALHA;
        nosCriados = 0;
        nova_metricas.caminho_solucao.clear();
        nova_metricas.nos_expandidos = 0;
        nova_metricas.tamanho_max_fila = 0;
        nova_metricas.entrada = entrada;
        nova_metricas.saida = saida;

        // Inicia nossa arvore 
        raiz = new No(entrada, nullptr);
        nosCriados++; // O nó raiz
        noEstado = raiz; // Estado atual -> no raiz

        return nova_metricas;
    }

    Resultado Iterate(const Matriz& labirinto, DadosUteis& Metricas) {
        if (Metricas.resultado == SUCESSO) {
            return SUCESSO;
        }
        // Nó atual foi visitado
        visitados[noEstado->estado.x][noEstado->estado.y] = true;

        Metricas.nos_expandidos++;                           // +1 nó que existiu
        Metricas.nos_explorados.push_back(noEstado->estado); // Guarda para no futuro a interface

        // Esse meu nó é o meu no objetivo?
        if (eh_objectivo(noEstado->estado, saida)) {
            // Sim?  então sucesso, novo nó e ultimo
            No* atual = noEstado;

            // Enquanto o o atual não for nulo, irá percorrer todos os nós até chegar em um que não tem pai, ou seja, a raiz 
            while (atual != nullptr) {
                Metricas.caminho_solucao.push_back(atual->estado); // É parte da solução
                atual = atual->pai;
            }

            // Inverte para ficar do início ao fim, pois ele foi guardado de forma inversa
            reverse(Metricas.caminho_solucao.begin(), Metricas.caminho_solucao.end());

            Metricas.custo_solucao = Metricas.caminho_solucao.size() - 1;
            Metricas.profundidade_solucao = Metricas.caminho_solucao.size() - 1;
            Metricas.tamanho_caminho = Metricas.caminho_solucao.size();

            // SUCESSO!!!
            Metricas.resultado = SUCESSO;
            Metricas.encontrou_solucao = true;
            Metricas.nos_gerados = nosCriados;

            return SUCESSO;
        }
        else {
            // Não foi achado ainda a saida, então procuramos os proximos sucessores usando a heuristica
            std::vector<No*> sucessores = obter_sucessores(noEstado, labirinto, visitados);
            // Inserimos na fila o nó escolhido
            fila.insert(fila.begin(), sucessores.begin(), sucessores.end());
            // Ordena a fila pois ela deve pegar o com mais "prioridade"
            ordenar_fila(fila);
        }

        // Medição do pico de uso da fila em tempo real
        if ((int)fila.size() > Metricas.tamanho_max_fila) {
            Metricas.tamanho_max_fila = fila.size();
        }

        // Caso a fila fique vazia e não encontrou saida, logicamente falhou!
        if (fila.empty()) {
            Metricas.resultado = FALHA;
            Metricas.encontrou_solucao = false;
            Metricas.nos_gerados = nosCriados;
            return FALHA;
        }

        // Pega o primeiro Elemento da fila, e volta para testar se ele é nosso nó desejado!
        noEstado = fila.front();
        // Deleta o primeiro elemento da fila
        fila.erase(fila.begin());
    }

    Resultado buscaPrimeiroMelhor(const Matriz& labirinto, DadosUteis& Metricas) {
        // Sempre verdade até dentro do codigo forçar sair
        Init(labirinto);
        while (Metricas.resultado != SUCESSO) {
            Iterate(labirinto, Metricas);
        }
        return Metricas.resultado;
    }

    void calcularMediasBPM(const Matriz& LABIRINTO, int quantidadeLabirintos) {
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

            DadosUteis metricas;

            metricas.numero_labirinto = i;

            auto inicio =
                std::chrono::high_resolution_clock::now();

            Resultado res =
                buscaPrimeiroMelhor(labirinto, metricas);

            auto fim =
                std::chrono::high_resolution_clock::now();

            metricas.tempo_execucao_ms =
                std::chrono::duration<double, std::milli>(fim - inicio).count();

            if (res == SUCESSO) {
                metricas.custo_solucao =
                    metricas.caminho_solucao.size() - 1;

                metricas.profundidade_solucao =
                    metricas.caminho_solucao.size() - 1;
            }

            somaNosGerados += metricas.nos_gerados;
            somaNosExpandidos += metricas.nos_expandidos;
            somaTempo += metricas.tempo_execucao_ms;

            if (metricas.encontrou_solucao) {
                somaCusto += metricas.custo_solucao;
                somaProfundidade += metricas.profundidade_solucao;
            }

            executados++;
        }

        std::cout << "\n========== MÉDIAS BPM ==========\n";

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



}