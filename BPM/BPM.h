#pragma once
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

#include "src/Nodes.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>

namespace BPM {
	using Matriz = std::vector<std::vector<int>>;

    // Resultado da nossa busca
    enum Resultado { SUCESSO, FALHA };

    // Dados para serem utilizados na interface
    struct DadosUteis {

        // Identificação
        std::string nome_arquivo;
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
        std::vector<Posicao> caminho_solucao;
        std::vector<Posicao> nos_explorados;

    };

    //////////////////////////////////////////////////////////////////////////
    //////////// HEURISTICA (Distancia Euclidiana ao Quadrado) ///////////////
    //////////////////////////////////////////////////////////////////////////

    double calcularHeuristica(Posicao atual, Posicao destino);

    /////////////////////////////////////////////////////////
    //////////// OBTER SUCESSORES DO NÓ ATUAL ///////////////
    /////////////////////////////////////////////////////////

    std::vector<No*> obter_sucessores(No* noAtual, const Matriz& labirinto, const std::vector<std::vector<bool>>& visitados);

    ///////////////////////////////////////////////////////////
    //////////// ORDENAÇÃO DA FILA (PRIORIDADE) ///////////////
    ///////////////////////////////////////////////////////////

    void ordenar_fila(std::vector<No*>& fila);

    //////////////////////////////////////////////////////////////////
    //////////// ALGORITMO DE BUSCA DO PRIMEIRO MELHOR ///////////////
    //////////////////////////////////////////////////////////////////

    DadosUteis Init(const Matriz& labirinto);

    Resultado Iterate(const Matriz& labirinto, DadosUteis& Metricas);

    Resultado buscaPrimeiroMelhor(const Matriz& labirinto, DadosUteis& Metricas);

    void calcularMediasBPM(const Matriz& LABIRINTO, int quantidadeLabirintos);

}