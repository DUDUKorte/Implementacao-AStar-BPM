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

#include <vector>
#include <list>

namespace AStar {
    using Matriz = std::vector<std::vector<int>>;

	class Posicao {
    public:
        int x, y;

        inline bool eh_igual(const Posicao& outra) const { return x == outra.x && y == outra.y; };

        bool eh_valida(const Matriz& LABIRINTO) const;
	};

	class No {
	public:
		Posicao posicao;
        // guarda a posição do pai
        Posicao pai;
        bool temPai;

        // custo do início até o nó atual
        int g;
        // estimativa até o destino
        int h;
        // prioridade na busca
        int f;

        No(Posicao pos, int gValor = 0, int hValor = 0, Posicao posPai = Posicao{ -1, -1 }, bool temPaiValor = false) :
            posicao(pos), g(gValor), h(hValor), f(g + h), pai(posPai), temPai(temPaiValor) {};
	};

    // Funcao para pegar o nó com o menor f na lista aberta
    No retirar_melhor_f(std::list<No>& aberta);

    // Função que verifica se o No/Posicao está na lista
    bool esta_na_lista(const std::list<No>& lista, Posicao posicao);

    // Função que substitui o nó pelo novo valor mais baixo de g,
    // caso o novo caminho seja melhor
    bool substituir_se_g_menor(std::list<No>& lista, Posicao posicao, int novoG, int novoH, Posicao novoPai);

    // Função que busca um nó em uma lista pela posição, parecido com a 
    // função de esta na lista mas essa busca por uma posição específica
    bool obter_no_da_lista(const std::list<No>& lista, Posicao posicao, No& noEncontrado);
}