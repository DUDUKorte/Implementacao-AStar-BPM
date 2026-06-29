#pragma once
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

#include "src/Nodes.h"

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <chrono>

namespace AStar {
	using Matriz = std::vector<std::vector<int>>;

    // Cria o tipo Resultado que pode ser SUCESSO ou FALHA
	enum Resultado { SUCESSO, FALHA };

	struct DadosUteis {
        // identificação
        std::string nome_arquivo;
        int numero_labirinto;

        // Dimensões do labirinto
        int linhas = 0;
        int colunas = 0;

        // Entrada e saída (são fixas mas é bom armazenar)
        Posicao entrada;
        Posicao saida;

        // Resultado da execução
        Resultado resultado = FALHA;
        bool encontrou_solucao = false;

        // Métricas principais
        int nos_gerados = 0; // Nós criados/adicionados a àrvore de busca
        int nos_expandidos = 0; // Nós removidos da aberta e tiveram seus vizinhos analizados
        int nos_visitados = 0; // Tamanho da lista fechada
        // nosVisitados = tamanho da lista fechada (equivale aos nós expandidos)
        double tempo_execucao_ms = 0.0;

        // Métricas "complementares"
        int custo_solucao = -1; // Valor g do nó final
        int profundidade_solucao = -1; // Quantidades de passos no caminho
        int tamanho_caminho = 0; // Quantidade de células no caminho
        int tamanho_max_lista_aberta = 0;
        int tamanho_max_lista_fechada = 0;
        double fator_ramificacao_medio = 0.0;
        // ramificação = nós gerados / nós expandidos

        // Dados uteis para a interface e analize
        std::list<No> lista_fechada;
        std::list<No> lista_aberta_final;
        std::vector<Posicao> caminho_solucao;
	};

    // [Modularização/Interface] 
    // Estado interno do A* para permitir execução passo a passo.
    // Aqui ficam as variáveis que antes estavam locais dentro da função AStar
    struct EstadoAStar {
        const Matriz* labirinto = nullptr;

        std::list<No> aberta;
        std::list<No> fechada;

        Posicao entrada;
        Posicao saida;

        No atual = No(Posicao{ -1, -1 });

        DadosUteis dados;

        bool destino_alcancado = false;
        bool finalizado = false;

        std::chrono::high_resolution_clock::time_point inicio_tempo;
    };

    // Funcção que calcula a heurística da distância do ponto atual até a saída
    int heuristica(Posicao atual, Posicao saida);

    std::vector<Posicao> reconstruir_caminho(const std::list<No>& fechada, No noFinal, Posicao entrada);

    // [Modularização/Interface]
    // AStar_init inicializa o algoritmo e prepara a lista aberta, lista fechada, 
    // posições de entrada/saída e métricas iniciais
    // Basicamente só copei o código do A* e adicionei a estrutura do Estado kk
    EstadoAStar Init(const Matriz& LABIRINTO, int numero_do_labirinto);

    // [Modularização / Interface]
    // AStar_inc executa UMA iteração do algoritmo A*
    // Retorna true quando o algoritmo terminou e false quando ainda precisa continuar
    // Basicamente só copei o código do A* e adicionei a estrutura do Estado kk
    bool Increment(EstadoAStar& estado);

    // [Modularização/Teste]
    // Função que executa o algortimo de Busca do A* com as funções modulares
    DadosUteis AStarMod(const Matriz& LABIRINTO, Posicao entrada, Posicao saida, int numero_do_labirinto);


    void CalcularMedias(const Matriz& LABIRINTO, int quantidadeLabirintos);
}