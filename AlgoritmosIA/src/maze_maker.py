'''
    Código gerador de labirinto para a matéria de IA implementado pelos alunos:
    . Rafael Loureiro Fockink,
    . Vitor Krieser Roehrs
    . Eduardo Nogueira Korte
    . Felipe Kuznik Thome

'''

from random import shuffle, randrange
import os


'''
////////////////////////////////////////////////////////////////////
/////////// GERA NO MÁXIMO 44X44 /////////////////////////////////////
/////////// !!!!!!!!!!!!!!!!!! /////////////////////////////////////
////////////////////////////////////////////////////////////////////
'''

# definição das dimensões do labirinto
largura = 3
altura = 3


# caminho de saída do arquivo contendo o labirinto
# Cada labirinto terá o nome no formato de suas dimensoes para facilitar a identificação e escolha deles.
end_saida = ("labirintos/")
end_saida += f"maze_{largura}x{altura}.csv"

'''
    FUNÇÃO PEGA DE REFERÊNCIA E ADAPTADA PARA GERAR O LABIRINTO EM FORMATO BINÁRIO (0s e 1s)
    O código original foi retirado do seguinte site:
    https://rosettacode.org/wiki/Maze_generation#Python
'''
# Função para gerar um labirinto 
def gerar_labirinto(largura=16, altura=8):

    # MATRIZ DE VISITAÇÃO
    # 0 = célula ainda não visitada
    # 1 = célula já visitada
    #
    # A última coluna e última linha extras servem como "bordas"
    # para evitar erro de índice.
    visitado = [[0] * largura + [1] for _ in range(altura)] + [[1] * (largura + 1)]

    # PAREDES VERTICAIS
    #
    # Cada célula começa com uma parede vertical "|"
    #
    # Exemplo:
    # |  |  |  |
    #
    paredes_verticais = [["|  "] * largura + ['|'] for _ in range(altura)] + [[]]

    # PAREDES HORIZONTAIS
    #
    # Cada célula começa com uma parede horizontal "--"
    #
    # Exemplo:
    # +--+--+--+
    #
    paredes_horizontais = [["+--"] * largura + ['+'] for _ in range(altura + 1)]

    # Adicionando entrada e saída para o labirinto
    paredes_horizontais[0][0] = "+  "  # Entrada no canto superior esquerdo
    paredes_horizontais[altura][largura - 1] = "+  "  # Saída no canto inferior direito

    # FUNÇÃO RECURSIVA UTILIZANDO DFS
    # Responsável por "caminhar" pelo labirinto
    def caminhar(x, y):

        # Marca a célula atual como visitada
        visitado[y][x] = 1

        # Lista de direções possíveis:
        # esquerda, baixo, direita, cima
        vizinhos = [
            (x - 1, y),
            (x, y + 1),
            (x + 1, y),
            (x, y - 1)
        ]

        # Embaralha as direções
        # Isso faz o labirinto ficar aleatório
        shuffle(vizinhos)

        # Percorre os vizinhos
        for (novo_x, novo_y) in vizinhos:

            # Se já visitou essa célula, ignora
            if visitado[novo_y][novo_x]:
                continue

            # Se mudou o eixo Y:
            # remove parede horizontal
            if novo_x == x:
                paredes_horizontais[max(y, novo_y)][x] = "+  "

            # Se mudou o eixo X:
            # remove parede vertical
            if novo_y == y:
                paredes_verticais[y][max(x, novo_x)] = "   "

            # Continua explorando recursivamente
            caminhar(novo_x, novo_y)

    # Começa em uma posição aleatória
    caminhar(randrange(largura), randrange(altura))

    # Monta a string final do labirinto
    labirinto = ""

    # Gera pares entre as linhas de paredes horizontais e verticais
    for linha_horizontal, linha_vertical in zip(paredes_horizontais, paredes_verticais):

        # Junta os caracteres das paredes horizontais
        labirinto += ''.join(linha_horizontal) + '\n'

        # Junta os caracteres das paredes verticais
        labirinto += ''.join(linha_vertical) + '\n'

    return labirinto

def gerar_labirinto_binario(largura=16, altura=8):
    # Dimensão real da matriz
    #
    # Cada célula do labirinto ocupa:
    # - 1 posição para o caminho
    # - paredes entre células
    #
    # Exemplo: 3x3:
    # célula parede célula parede célula
    #
    linhas = altura * 2 + 1
    colunas = largura * 2 + 1

    # Inicializa tudo como parede (1)
    labirinto = [[1 for _ in range(colunas)] for _ in range(linhas)]

    # Controle de células visitadas
    visitado = [[False for _ in range(largura)] for _ in range(altura)]

    # FUNçÂO RECURSIVA UTILIZANDO DFS
    def caminhar(x, y):

        # Marca como visitado
        visitado[y][x] = True

        # Converte coordenada da célula
        # para coordenada da matriz real
        matriz_x = x * 2 + 1
        matriz_y = y * 2 + 1

        # Marca o caminho como livre
        labirinto[matriz_y][matriz_x] = 0

        # Direções possíveis
        direcoes = [
            (-1, 0),  # esquerda
            (1, 0),   # direita
            (0, -1),  # cima
            (0, 1)    # baixo
        ]

        # Embaralha as direções para ficar aleatorio
        shuffle(direcoes)

        # percorre as direções
        for dx, dy in direcoes:

            novo_x = x + dx
            novo_y = y + dy

            # Verifica limites
            if not (0 <= novo_x < largura and 0 <= novo_y < altura):
                continue

            # Ignora células já visitadas
            if visitado[novo_y][novo_x]:
                continue

            # Remove parede entre células
            parede_x = matriz_x + dx
            parede_y = matriz_y + dy

            labirinto[parede_y][parede_x] = 0

            # Continua exploração
            caminhar(novo_x, novo_y)

    # Começa em posição aleatória
    inicio_x = randrange(largura)
    inicio_y = randrange(altura)

    caminhar(inicio_x, inicio_y)

    # Entrada
    labirinto[0][1] = 0

    # Saída
    labirinto[linhas - 1][colunas - 2] = 0

    # Retorna a matriz do labirinto
    return labirinto

# Função para imprimir o labirinto binário
def imprimir_labirinto_binario(labirinto):
    for linha in labirinto:
        print(" ".join(map(str, linha)))

# Função para converter o labirinto binário para uma visualização em ASCII
def converter_labirinto_visualizacao(labirinto_binario):
    labirinto_visual = ""

    for linha in labirinto_binario:
        for celula in linha:
            if celula == 1:
                labirinto_visual += "++"
            else:
                labirinto_visual += "  "
        labirinto_visual += "\n"

    return labirinto_visual

# Função para escrever o labirinto no arquivo
def armazena_labirinto(end_saida, modo, labirinto):
     
    # Cria o diretório labirintos se não existir
    os.makedirs('labirintos', exist_ok = True) 

    # Abre ou cira o arquivo e depois salva
    with open(end_saida, modo) as arquivo:        
        for linha in labirinto:
            arquivo.write(','.join(map(str, linha)) + '\n')
    
# Função que gera N labirintos de N tamanhos
def geracoes_labirintos(quantidade_por_tamanho=5, tamanho_inicial=4, tamanho_final=20, apenas_quadrados=True):
    """
    Gera vários labirintos e armazena
    vários no MESMO arquivo usando append.

   
    Parâmetros:
    ----------

    quantidade_por_tamanho : int
        Quantidade de labirintos gerados para cada tamanho

    tamanho_inicial : int
        Menor tamanho do labirinto

    tamanho_final : int
        Maior tamanho do labirinto

    apenas_quadrados : bool
        True  -> gera apenas NxN
        False -> gera retangulares também

    """

    # Cria pasta caso não exista
    os.makedirs("labirintos", exist_ok=True)

    # Apenas quadrados
    if apenas_quadrados:

        for tamanho in range(tamanho_inicial, tamanho_final + 1):

            largura = tamanho
            altura = tamanho

            caminho_arquivo = (f"labirintos/"f"maze_{largura}x{altura}.csv")

            # Limpa o arquivo antes
            open(caminho_arquivo, 'w').close()

            for numero_labirinto in range(1,quantidade_por_tamanho + 1):

                maze = gerar_labirinto_binario(largura, altura)

                with open(caminho_arquivo, 'a') as arquivo:

                    # Cabeçalho
                    arquivo.write(f"LABIRINTO {numero_labirinto}\n")

                    # Escreve o labirinto
                    for linha in maze:arquivo.write(','.join(map(str, linha)) + '\n')

                    # Separador
                    arquivo.write("\n")

                print(f"Adicionado labirinto "f"{numero_labirinto} em "f"{caminho_arquivo}")

    # Retangulares também
    else:

        for largura in range(tamanho_inicial, tamanho_final + 1):

            for altura in range(tamanho_inicial, tamanho_final + 1):

                caminho_arquivo = (f"labirintos/"f"maze_{largura}x{altura}.csv")

                # Limpa arquivo antes
                open(caminho_arquivo, 'w').close()

                for numero_labirinto in range(1, quantidade_por_tamanho + 1):

                    maze = gerar_labirinto_binario(largura, altura)

                    with open(caminho_arquivo, 'a') as arquivo:

                        arquivo.write(f"LABIRINTO {numero_labirinto}\n")

                        for linha in maze:
                            arquivo.write(','.join(map(str, linha)) + '\n')

                        arquivo.write("\n")

                    print(f"Adicionado labirinto "f"{numero_labirinto} em "f"{caminho_arquivo}")

# Função que gera N labirintos de um tamanho especifico
def gerar_n_labirintos (quantidade_por_tamanho=5, linhas=5, colunas=5):
    """
    Gera N labirintos de tamanho MxN e armazena
    vários no MESMO arquivo usando append.
    """

    # Cria pasta caso não exista
    os.makedirs("labirintos", exist_ok=True)

    caminho_arquivo = (f"labirintos/"f"maze_{linhas}x{colunas}.csv")

    # Limpa o arquivo antes
    open(caminho_arquivo, 'w').close()

    for numero_labirinto in range(1,quantidade_por_tamanho + 1):

        maze = gerar_labirinto_binario(linhas, colunas)

        with open(caminho_arquivo, 'a') as arquivo:

            # Cabeçalho
            arquivo.write(f"LABIRINTO {numero_labirinto}\n")

            # Escreve o labirinto
            for linha in maze:arquivo.write(','.join(map(str, linha)) + '\n')

            # Separador
            arquivo.write("\n")

        print(f"Adicionado labirinto "f"{numero_labirinto} em "f"{caminho_arquivo}")


if __name__ == "__main__":
    
    # Gera e imprime o labirinto utilizando a função de referência
    print(gerar_labirinto(largura, altura))

    # Gera o labirinto em formato binário
    maze = gerar_labirinto_binario(largura, altura)

    # Imprime o labirinto binário no console
    imprimir_labirinto_binario(maze)
    print('\n')
    
    # Armazena o labirinto binário no arquivo
    armazena_labirinto(end_saida, 'a', maze)

    # Converte o labirinto binário para uma visualização em ASCII e imprime
    labirinto_visual = converter_labirinto_visualizacao(maze)
    print(labirinto_visual)

    # Gera 5 labirintos quadrados de 5 até 20
    # geracoes_labirintos(5, 5, 20, True)
