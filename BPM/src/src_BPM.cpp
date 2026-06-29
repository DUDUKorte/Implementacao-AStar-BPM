/*
    Código que implementa o algorítmo de Busca do Primeiro Melhor em labirintos para a matéria de IA desenvolvido pela dupla:
    . Felipe Kuznik Thome
    . Vitor Krieser Roehrs

    Equipe do trabalho:
    . Rafael Loureiro Fockink,
    . Vitor Krieser Roehrs
    . Eduardo Nogueira Korte
    . Felipe Kuznik Thome
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include "../../AlgoritmosIA/src/LerLabirinto.h"

using namespace std;

using Matriz = vector<vector<int>>; // Matriz que ficará o labirinto

namespace BuscaPrimeiroMelhor {

    struct DadosUteis; // Dados para a interface


    ///////////////////////////////////////////////
    ////// ESTRUTURA PARA SABER NOSSA POSIÇÃO /////
    ///////////////////////////////////////////////

    struct Posicao {

        // x = linha, y = coluna 
        int x, y;
        // para conseguir comparar duas structs (saber que chegou no final)
        bool operator==(const Posicao& outra) const {
            return x == outra.x && y == outra.y;
        }
    };
    ///////////////////////////////////////////////////////////////////


            ///////////////////////////////////////////////
            //////////// NÓ DA NOSSA ARVORE ///////////////
            ///////////////////////////////////////////////

    struct No {

        Posicao estado; // Para saber a posição de onde esta no labirinto
        No* pai; // Nó pai, o gerador do nó atual, onde a raiz tem pai null

        // Construtor do nó
        No(Posicao est, No* p = nullptr) : estado(est), pai(p) {}
    };
    /////////////////////////////////////////////////////////////////////


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


    // Função que testa se o nó atual é o nó da saida
    bool eh_objectivo(Posicao estado) {

        return estado == saida;

    }


    /////////////////////////////////////////////////////////
    //////////// OBTER SUCESSORES DO NÓ ATUAL ///////////////
    /////////////////////////////////////////////////////////

    vector<No*> obter_sucessores(No* noAtual, const Matriz& labirinto, const vector<vector<bool>>& visitados) {


        vector<No*> sucessores;            // Vetor de sucessores
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

    void ordenar_fila(vector<No*>& fila) {

        // Usando ordenação sort para ordenar a fila para quem tiver a menor distancia ser o primeiro a ser escolhido
        sort(fila.begin(), fila.end(), [](No* a, No* b) {
            return calcularHeuristica(a->estado, saida) < calcularHeuristica(b->estado, saida);
            });
    }
    ///////////////////////////////////////////////////////////////////////////


    // Resultado da nossa busca
    enum Resultado { SUCESSO, FALHA };

    // Dados para serem utilizados na interface
    struct DadosUteis {

        // Identificação
        string nome_arquivo;
        int numero_labirinto;

        // Dimensões do labirinto
        int linhas = 0;
        int colunas = 0;

        // Entrada e saida
        Posicao entrada;
        Posicao saida;

        // Resultado da execução
        Resultado resultado = FALHA;
        bool encontrou_solucao = false;

        // Métricas principais
        int nos_gerados = 0;
        int nos_expandidos = 0;
        double tempo_execucao_ms = 0.0;

        // Métricas Complementares
        int custo_solucao = -1;
        int profundidade_solucao = -1;
        int tamanho_caminho = 0;
        int tamanho_max_fila = 0;

        // Dados uteis para interface e análise 
        vector<Posicao> caminho_solucao;
        vector<Posicao> nos_explorados;

    };


    //////////////////////////////////////////////////////////////////
    //////////// ALGORITMO DE BUSCA DO PRIMEIRO MELHOR ///////////////
    //////////////////////////////////////////////////////////////////
    // 
    // Uma matriz para saber nos visitados, com 'n' linhas com 'm' colunas, tudo falso
    vector<vector<bool>> visitados;
    No* raiz;
    No* noEstado;
    vector<No*> fila;

    DadosUteis Init(const Matriz& labirinto) {
        DadosUteis nova_metricas;
        visitados.clear();
        fila.clear();

        // Linha e coluna da matriz
        int n = labirinto.size();
        int m = labirinto[0].size();
        saida = { n - 1, m - 2 }; // Saida sempre fixa

        entrada = { 0, 1 };

        visitados.assign(n, vector<bool>(m, false));

        // Metricas que serão guardadas
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
        if (eh_objectivo(noEstado->estado)) {
            // Sim?  então sucesso, novo nó e ultimo
            No* atual = noEstado;

            // Enquanto o o atual não for nulo, irá percorrer todos os nós até chegar em um que não tem pai, ou seja, a raiz 
            while (atual != nullptr) {
                Metricas.caminho_solucao.push_back(atual->estado); // É parte da solução
                atual = atual->pai;
            }

            // Inverte para ficar do início ao fim, pois ele foi guardado de forma inversa
            reverse(Metricas.caminho_solucao.begin(), Metricas.caminho_solucao.end());

            // SUCESSO!!!
            Metricas.resultado = SUCESSO;
            Metricas.encontrou_solucao = true;
            Metricas.nos_gerados = nosCriados;

            return SUCESSO;
        }
        else {
            // Não foi achado ainda a saida, então procuramos os proximos sucessores usando a heuristica
            vector<No*> sucessores = obter_sucessores(noEstado, labirinto, visitados);
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
        while (Metricas.resultado != SUCESSO){
            Iterate(labirinto, Metricas);
        }
        return Metricas.resultado;
    }


    void calcularMediasBPM(const string& arquivo, int quantidadeLabirintos) {

        double somaNosGerados = 0;
        double somaNosExpandidos = 0;
        double somaTempo = 0;
        double somaCusto = 0;
        double somaProfundidade = 0;

        int executados = 0;

        for (int i = 1; i <= quantidadeLabirintos; i++) {

            Matriz labirinto = lerLabirinto(arquivo, i);

            if (labirinto.empty())
                continue;

            DadosUteis metricas;

            metricas.nome_arquivo = arquivo;
            metricas.numero_labirinto = i;

            auto inicio =
                chrono::high_resolution_clock::now();

            Resultado res =
                buscaPrimeiroMelhor(labirinto, metricas);

            auto fim =
                chrono::high_resolution_clock::now();

            metricas.tempo_execucao_ms =
                chrono::duration<double, milli>(fim - inicio).count();

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

        cout << "\n========== MÉDIAS BPM ==========\n";

        cout << "Labirintos analisados: "
            << executados << endl;

        cout << "Média nós gerados: "
            << somaNosGerados / executados << endl;

        cout << "Média nós expandidos: "
            << somaNosExpandidos / executados << endl;

        cout << "Média tempo (ms): "
            << somaTempo / executados << endl;

        cout << "Média custo solução: "
            << somaCusto / executados << endl;

        cout << "Média profundidade: "
            << somaProfundidade / executados << endl;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////





            //////////////////////////////////////////////////////////////////
            //////////// FUNÇÃO PRINCIPAL QUE VAI UNIR TUDO //////////////////
            //////////////////////////////////////////////////////////////////
    //        
    //int main() {
    //    
    //    // Nome e labirinto alvo
    //    string nomeArquivo = "labirintos/maze_35x35.csv"; 
    //
    //    calcularMediasBPM(nomeArquivo, 5);
    //
    //
    //    /*
    //    int numeroAlvo = 1;
    //
    //    // Le o labirinto do arquivo
    //    Matriz LABIRINTO = lerLabirinto(nomeArquivo, numeroAlvo);
    //    
    //    // Metricas que serão usadas
    //    DadosUteis metricas;
    //    
    //    // Testa se o labirinto existe
    //    if (!LABIRINTO.empty()) {
    //        metricas.nome_arquivo = nomeArquivo;
    //        metricas.numero_labirinto = numeroAlvo;
    //        metricas.linhas  = LABIRINTO.size();       
    //        metricas.colunas = LABIRINTO[0].size();
    //
    //        cout << "Matriz \"" << numeroAlvo << "\" carregada com sucesso!\n" << endl;
    //        imprimirMatriz(LABIRINTO);
    //        cout << "--------------------------------------------------------------------\n" << endl;
    //    } else {
    //        return 1; 
    //    }
    //    
    //    // Inicio do tempo
    //    auto tempo_inicio = chrono::high_resolution_clock::now();
    //    
    //    // O algoritmo do Busca Pelo Primeiro Melhor
    //    Resultado res = buscaPrimeiroMelhor(LABIRINTO, metricas);
    //    
    //    // Final do tempo
    //    auto tempo_fim = chrono::high_resolution_clock::now();
    //    
    //    // Tempo total utilizado no algoritmo de busca
    //    metricas.tempo_execucao_ms = chrono::duration<double, milli>(tempo_fim - tempo_inicio).count();
    //
    //    // Caso tenha sido um sucesso, armazena os dados nas metricas
    //    if (res == SUCESSO) {
    //        metricas.custo_solucao        = metricas.caminho_solucao.size() - 1;
    //        metricas.profundidade_solucao = metricas.caminho_solucao.size() - 1; 
    //        metricas.tamanho_caminho      = metricas.caminho_solucao.size();     
    //        
    //        cout << "Sucesso! Caminho encontrado ate a Saida." << endl;
    //        cout << "Coordenadas do caminho: ";
    //        for (const auto& pos : metricas.caminho_solucao) {
    //            cout << "(" << pos.x << ", " << pos.y << ") ";
    //        }
    //        cout << "\n\n";
    //        
    //        for (const auto& pos : metricas.caminho_solucao) {
    //            LABIRINTO[pos.x][pos.y] = 2;
    //        }
    //        
    //        cout << "Labirinto com caminho marcado (. = caminho):\n";
    //        imprimirMatriz(LABIRINTO);
    //    } else {
    //        // Caso não tenha sido um sucesso, as metricas dirão que deu errado
    //        metricas.custo_solucao        = -1;
    //        metricas.profundidade_solucao = -1;
    //        metricas.tamanho_caminho      = 0;
    //        
    //        cout << "Falha! Nao existe caminho possivel." << endl;
    //        imprimirMatriz(LABIRINTO);
    //    }
    //
    //    
    //    // Impressão de estastisticas da execução
    //    cout << "\n====================================================================" << endl;
    //    cout << "                    ESTATÍSTICAS DA EXECUÇÃO                        " << endl;
    //    cout << "====================================================================" << endl;
    //    cout << "Arquivo Lido:            " << metricas.nome_arquivo << " (Labirinto nº " << metricas.numero_labirinto << ")" << endl;
    //    cout << "Dimensões:               " << metricas.linhas << "x" << metricas.colunas << endl;
    //    cout << "Entrada / Saída:         (" << metricas.entrada.x << "," << metricas.entrada.y << ") -> (" << metricas.saida.x << "," << metricas.saida.y << ")" << endl;
    //    cout << "Situação Final:          " << (metricas.encontrou_solucao ? "Solucionado" : "Sem solução") << endl;
    //    cout << "Tempo de Execução:       " << metricas.tempo_execucao_ms << " ms" << endl;
    //    cout << "Total de Nós Gerados:    " << metricas.nos_gerados << " nós na árvore" << endl;
    //    cout << "Total de Nós Expandidos: " << metricas.nos_expandidos << " nós processados" << endl;
    //    cout << "Tamanho Máximo da Fila:  " << metricas.tamanho_max_fila << " nós guardados simultaneamente" << endl;
    //    cout << "Custo do Caminho:        " << metricas.custo_solucao << " passos" << endl;
    //    cout << "Profundidade Solução:    " << metricas.profundidade_solucao << endl;
    //    cout << "Tamanho Vetor Caminho:   " << metricas.tamanho_caminho << " coordenadas" << endl;
    //    cout << "====================================================================" << endl;
    //    */
    //
    //   
    //    return 0;
    //}
}