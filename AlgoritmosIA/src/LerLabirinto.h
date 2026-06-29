/*
    Biblioteca externa para ler labirinto dos arquivos elaborada pela dupla:
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


using Matriz = std::vector<std::vector<int>>; // Matriz que conterá o labirinto

// Função que lê labirinto dos arquivos.
Matriz lerLabirinto(const std::string& nomeArquivo, int numeroDesejado);

// Função que escreve o labirinto lido
void imprimirMatriz(const Matriz& matriz);