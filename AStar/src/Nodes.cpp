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

#include "Nodes.h"

namespace AStar {
	bool Posicao::eh_valida(const Matriz& LABIRINTO) const{
        int n = LABIRINTO.size();
        int m = LABIRINTO[0].size();

        // Verifica se está dentro dos limites da matriz
        if (x < 0 || x >= n || y < 0 || y >= m) {
            return false;
        }

        // Verifica se é caminho livre, se não é parede
        if (LABIRINTO[x][y] != 0) {
            return false;
        }

        return true;
	}

    // Funcao para pegar o nó com o menor f na lista aberta
    No retirar_melhor_f(std::list<No>& aberta) {

        // Assumo que o primeiro nó é o melhor inicialmente
        // utilizo o tipo auto pq a função .beguin() retorna
        // um iterador, quase um ponteiro...
        auto melhor = aberta.begin();

        // Percorre todos os nós da lista aberta
        for (auto it = aberta.begin(); it != aberta.end(); it++) {
            // Se o nó atual tiver um f menor que o melhor até agora, ele vira o novo melhor.
            if (it->f < melhor->f)
                melhor = it;
        }

        // Copia o melhor nó
        No escolhido = *melhor;
        // remove ele da lista aberta
        aberta.erase(melhor);

        return escolhido;
    }

    // Função que verifica se o No/Posicao está na lista
    bool esta_na_lista(const std::list<No>& lista, Posicao posicao) {
        for (No no : lista) {
            if (no.posicao.eh_igual(posicao)) {
                return true;
            }
        }

        return false;
    }

    // Função que substitui o nó pelo novo valor mais baixo de g,
    // caso o novo caminho seja melhor
    bool substituir_se_g_menor(std::list<No>& lista, Posicao posicao, int novoG, int novoH, Posicao novoPai) {
        for (No& no : lista) {
            if (no.posicao.eh_igual(posicao)) {

                // Verifica se o novo g é menor que o g armazenado
                if (novoG < no.g) {
                    no.g = novoG;
                    no.h = novoH;
                    no.f = no.g + no.h;

                    // O nó atual agora é o pai do vizinho
                    no.pai = novoPai;
                    no.temPai = true;

                    return true;
                }

                return false;
            }
        }

        return false;
    }

    // Função que busca um nó em uma lista pela posição, parecido com a 
    // função de esta na lista mas essa busca por uma posição específica
    bool obter_no_da_lista(const std::list<No>& lista, Posicao posicao, No& noEncontrado) {
        for (No no : lista) {
            if (no.posicao.eh_igual(posicao)) {
                noEncontrado = no;
                return true;
            }
        }

        return false;
    }

}