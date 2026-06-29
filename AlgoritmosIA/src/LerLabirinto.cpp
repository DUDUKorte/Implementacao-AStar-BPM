/*
    Implementação da biblioteca externa para ler labirinto dos arquivos elaborada pela dupla:
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
#include "LerLabirinto.h"

using Matriz = std::vector<std::vector<int>>; // Matriz que conterá o labirinto

    ///////////////////////////////////////////////
    ////// FUNÇÃO APENAS PARA LER O LABIRINTO /////
    ///////////////////////////////////////////////
    
Matriz lerLabirinto(const std::string& nomeArquivo, int numeroDesejado) {
    
    // Abre o arquivo e inicia a matriz que irá conter o labirinto
    std::ifstream arquivo(nomeArquivo);
    Matriz matrizEncontrada;

    // Teste para ver se o arquivo conseguiu abrir
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        // Retorna a Matriz (FALHA)
        return matrizEncontrada;
    }

    // Variavel de controle
    std::string linha;
    
    // Enquanto continuar lendo uma linha e armazenar na variavel linha continue executando
    while (getline(arquivo, linha)) {

        // Teste para ver se não é um espaço em branco, se for, pular até achar um caracter
        if (linha.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        
        // Encontrar nome da matriz, que sempre sera "LABIRINTO" e linha essa que conterá o numero dela
        if (linha.find("LABIRINTO") != std:: string::npos) {
            
            std::stringstream ss(linha); // Variavel de controle para armazenar as linhas
            std::string nome;            // Nome para controle 
            int numero;             // Numero para controle e testar se é o numero desejado
            
            // Lê o nome e numero
            if (ss >> nome >> numero) {
                
                // Verifica se o numero desejado é o numero encontrado
                if (numero == numeroDesejado) {
                    
                    // Enquanto continuar lendo uma linha e armazenar na variavel linha continue executando
                    while (getline(arquivo, linha)) {
                       
                        // Ele irá parar de ler se encontrar um Labirinto (significa que chegou em outro labirinto)
                        // OU
                        // Se encontrar se a linha estiver totalmente vazia
                        if (linha.find("LABIRINTO") != std::string::npos || linha.find_first_not_of(" \t\r\n") == std::string::npos) {
                            break;
                        }
                        
                        std::vector<int> linhaMatriz;      // Lista tempóraria das linhas
                        std::stringstream ssMatriz(linha); // Variavel de controle para ler as linhas
                        std::string valorTexto;            // Varivel de controle do valor lido
                        
                        // Ler todas as linhas até chegar em um espaço branco
                        while (getline(ssMatriz, valorTexto, ',')) {
                            if (valorTexto.find_first_not_of(" \t\r\n") == std::string::npos) continue;
                            // String para inteiros
                            linhaMatriz.push_back(stoi(valorTexto));
                        }
                        
                        // Testa se a linha não esta vazia, então vai armazena-la
                        if (!linhaMatriz.empty()) {
                            matrizEncontrada.push_back(linhaMatriz);
                        }
                    }
                  
                    // Fecha o arquivo 
                    arquivo.close();
                    // Retorna a Matriz (SUCESSO)
                    return matrizEncontrada;
                }
            }
        }
    }

    // Se não encontrar o numero desejado ele encerra a procura
    // Fecha o arquivo 
    arquivo.close();
    std::cout << "Labirinto com o numero " << numeroDesejado << " nao foi encontrado." << std::endl;
    // Retorna a Matriz (FALHA)
    return matrizEncontrada;
}

///////////////////////////////////////////////////////////////////////////////////
////// FUNÇÃO APENAS PARA IMPRIMIR O LABIRINTO (POSSIVELMENTE IREMOS EXCLUIR) /////
///////////////////////////////////////////////////////////////////////////////////

void imprimirMatriz(const Matriz& matriz) {
    for (const auto& linha : matriz) {
        for (int valor : linha) {
            if (valor == 2) {
                std::cout << "* ";
            } else {
                std::cout << valor << " ";
            }
        }
        std::cout << std::endl;
    }
}
///////////////////////////////////////////////////////////////////////////////////