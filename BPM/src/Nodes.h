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

namespace BPM {

    ///////////////////////////////////////////////
    ////// ESTRUTURA PARA SABER NOSSA POSIÇÃO /////
    ///////////////////////////////////////////////

    struct Posicao {

        // x = linha, y = coluna 
        int x, y;
        // para conseguir comparar duas structs (saber que chegou no final)
        inline bool operator==(const Posicao& outra) const { return x == outra.x && y == outra.y; }
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

    // Função que testa se o nó atual é o nó da saida
    inline bool eh_objectivo(Posicao estado, Posicao saida) { return estado == saida; }


}
