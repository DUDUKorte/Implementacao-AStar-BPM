/*
    Código que implementa uma interface gráfica interativa Integrando os dois algoritmos 
    para a matéria de IA desenvolvido pela dupla:
    . Rafael Loureiro Fockink
    . Eduardo Nogueira Korte

    Equipe do trabalho:
    . Rafael Loureiro Fockink
    . Vitor Krieser Roehrs
    . Eduardo Nogueira Korte
    . Felipe Kuznik Thome
*/

// Imgui includes
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <imgui_internal.h>

// std includes
#include <iostream>
#include <stdio.h>
#include <format>
#include <thread>
#include <random>

#include "src/LerLabirinto.h"
#include "src/Widgets.h"
#include <AStar.h>
#include <BPM.h>

// Função que recarrega o Labirinto com base nos valores fornecidos
Matriz LoadMaze(int altura, int largura) {
    // Monta o nome do arquivo do labirinto escolhido
    std::string nomeDoArquivo = std::format("data/maze_{0}x{1}.csv", largura, altura);

    // Pega um entre os 5 labirintos aleatórios no arquivo
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(1, 5);
    int numeroAlvo = distr(gen);

    return lerLabirinto(nomeDoArquivo, numeroAlvo);
}

int main() {
    // Configurações iniciais do primeiro labirinto
    int altura_labirinto = 40;
    int largura_labirinto = 40;

    // Carrega o primeiro labirinto
    Matriz LABIRINTO = LoadMaze(altura_labirinto, largura_labirinto);

    // incializa o A*
    AStar::EstadoAStar estado = AStar::Init(LABIRINTO, 1);

    // Inicializar o BPM
    BPM::DadosUteis metricas = BPM::Init(LABIRINTO);


    // Inicializar a configurar ImGui e SDL Renderer
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return 1;
	}

    // Create window with SDL_Renderer graphics context
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (renderer == nullptr)
    {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Variáveis de estados
    bool enable_dockspace = true;
    bool auto_run_aStar = false;
    bool auto_run_BPM = false;

    // Main loop
    bool done = false;

    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Verificações dos algoritmos para rodar automático
        if (auto_run_aStar) {
            AStar::Increment(estado);
        }
        if (auto_run_BPM) {
            BPM::Iterate(LABIRINTO, metricas);
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Setup ImGui Dockspace.
        ShowExampleAppDockSpace(&enable_dockspace);

        // Mostra as janelas de cada labirinto utilizando o ImGui
        ImLabirintoAStar(LABIRINTO, estado.fechada, estado.dados.caminho_solucao, "Labirinto A*", &estado);
        ImLabirintoBPM(LABIRINTO, metricas.nos_explorados, metricas.caminho_solucao, "Labirinto BPM", &metricas);

        // Mostra janela principal de controles
        {
            ImGui::Begin("Controles");
            ImGui::Text("Controles"); ImGui::Separator();

            ImGui::SeparatorText("Labirinto");
            // Slider da altura
            if (ImGui::SliderInt("Altura", &altura_labirinto, 5, 99)) {
                LABIRINTO = LoadMaze(altura_labirinto, largura_labirinto);
                estado = AStar::Init(LABIRINTO, 1);
                metricas = BPM::Init(LABIRINTO);
            }
            // Slider da largura
            if(ImGui::SliderInt("Largura", &largura_labirinto, 5, 99)){
                LABIRINTO = LoadMaze(altura_labirinto, largura_labirinto);
                estado = AStar::Init(LABIRINTO, 1);
                metricas = BPM::Init(LABIRINTO);
            }
            // Aviso se colocar valores muito altos
            if (largura_labirinto * altura_labirinto > 4225) {
                ImGui::TextColored(ImVec4(0.9, 0.2, 0.2, 255), "CUIDADO!!!!!");
                ImGui::TextColored(ImVec4(0.9, 0.2, 0.2, 255), "Valores muito altos podem afetar o desempenho!");
            }
            // Botão pra recarregar o labirinto
            if (ImGui::Button("Recarregar Labirinto")) {
                LABIRINTO = LoadMaze(altura_labirinto, largura_labirinto);
                estado = AStar::Init(LABIRINTO, 1);
                metricas = BPM::Init(LABIRINTO);
            }

            ImGui::SeparatorText("Controles Step-By-Step");
            // Botão que incrementa os dois algoritmos
            if (ImGui::Button("Incrementar Ambos")) {
                AStar::Increment(estado);
                BPM::Iterate(LABIRINTO, metricas);
            }
            ImGui::SameLine();
            // Botão que incrementa apenas o A*
            if (ImGui::Button("Incrementar A*")) {
                AStar::Increment(estado);
            }
            ImGui::SameLine();
            // Botão que incrementpa apenas o BPM
            if (ImGui::Button("Incrementar BPM")) {
                BPM::Iterate(LABIRINTO, metricas);
            }

            ImGui::SeparatorText("Controles Automáticos");
            // Botão que roda os dois algoritmos automatiacmente ao mesmo tempo
            if (ImGui::Button("RUN!")) {
                auto_run_aStar = true;
                auto_run_BPM = true;
            }
            ImGui::SameLine();
            // Botão que roda os dois algoritmos em MultiThread para comparar tempo de execução
            if (ImGui::Button("MT RUN!")) {
                auto_run_aStar = false;
                auto_run_BPM = false;
                std::thread astar_thread([&LABIRINTO, &estado]() {
                    estado = AStar::Init(LABIRINTO, 1);
                    auto inicioTempo = std::chrono::high_resolution_clock::now();
                    while (!estado.finalizado) {
                        AStar::Increment(estado);
                    }
                    auto fimTempo = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> duracao = fimTempo - inicioTempo;
                    estado.dados.tempo_execucao_ms = duracao.count();
                });

                std::thread bpm_thread([&LABIRINTO, &metricas]() {
                    metricas = BPM::Init(LABIRINTO);
                    auto inicioTempo = std::chrono::high_resolution_clock::now();
                    while (metricas.resultado != BPM::SUCESSO) {
                        BPM::Iterate(LABIRINTO, metricas);
                    }
                    auto fimTempo = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> duracao = fimTempo - inicioTempo;
                    metricas.tempo_execucao_ms = duracao.count();
                });

                astar_thread.detach();
                bpm_thread.detach();
            }

            // Botão para resetar os algoritmos sem mudar o labirinto
            if (ImGui::Button("Reset")) {
                estado = AStar::Init(LABIRINTO, 10);
                metricas = BPM::Init(LABIRINTO);
            }

            // Opções manuais pra deixar cada algoritmo rodando automático
            ImGui::SeparatorText("Controles indivduais");
            ImGui::Checkbox("Auto A*", &auto_run_aStar);
            ImGui::SameLine();
            ImGui::Checkbox("Auto BPM", &auto_run_BPM);

            ImGui::Separator();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            if(ImGui::Button("Sair")){
                done = true;
            }
            ImGui::End();
        }

        // Janela de informações do A*
        ImGui::Begin("Info. A*");
        //ImGui::SeparatorText("Informações do A*");
        ImGui::Text("Informações do A*"); ImGui::Separator();
        ImGui::Text("Colunas: %d", estado.dados.colunas);
        ImGui::Text("Linhas: %d", estado.dados.linhas);
        ImGui::Text("Custo Solução: %d", estado.dados.custo_solucao);
        ImGui::Text("Encontrou Solução: %d", estado.dados.encontrou_solucao);
        ImGui::Text("Entrada: (%d, %d)", estado.dados.entrada.x, estado.dados.entrada.y);
        ImGui::Text("Fator Ramificação: %lf", estado.dados.fator_ramificacao_medio);
        ImGui::Text("Nome Arquivo: %s", estado.dados.nome_arquivo);
        ImGui::Text("Nós expandidos: %d", estado.dados.nos_expandidos);
        ImGui::Text("Nós gerados: %d", estado.dados.nos_gerados);
        ImGui::Text("Nós visitados: %d", estado.dados.nos_visitados);
        ImGui::Text("Número labirinto: %d", estado.dados.numero_labirinto);
        ImGui::Text("Profundiade solução %d", estado.dados.profundidade_solucao);
        ImGui::Text("Resultado %s", estado.dados.resultado == AStar::SUCESSO ? "SUCESSO" : "FALHA");
        ImGui::Text("Saída: (%d, %d)", estado.dados.saida.x, estado.dados.saida.y);
        ImGui::Text("Tamanho Caminho: %d", estado.dados.tamanho_caminho);
        ImGui::Text("Tamanho Max Lista Aberta: %d", estado.dados.tamanho_max_lista_aberta);
        ImGui::Text("Tamanho Max Lista Fechada: %d", estado.dados.tamanho_max_lista_fechada);
        ImGui::Text("Tempo de execução: %.0lf ms", estado.dados.tempo_execucao_ms);
        ImGui::End();

        // Janela de informações do BPM
        ImGui::Begin("Info. BPM");
        ImGui::Text("Informações do BPM"); ImGui::Separator();
        ImGui::Text("Colunas: %d", metricas.colunas);
        ImGui::Text("Linhas: %d", metricas.linhas);
        ImGui::Text("Custo Solução: %d", metricas.custo_solucao);
        ImGui::Text("Encontrou Solução: %d", metricas.encontrou_solucao);
        ImGui::Text("Entrada: (%d, %d)", metricas.entrada.x, metricas.entrada.y);
        //ImGui::Text("Fator Ramificação: %lf", metricas.fator_ramificacao_medio);
        ImGui::Text("Nome Arquivo: %s", metricas.nome_arquivo);
        ImGui::Text("Nós expandidos: %d", metricas.nos_expandidos);
        ImGui::Text("Nós visitados: %d", metricas.nos_explorados.size());
        ImGui::Text("Nós gerados: %d", metricas.nos_gerados);
        ImGui::Text("Número labirinto: %d", metricas.numero_labirinto);
        ImGui::Text("Profundiade solução %d", metricas.profundidade_solucao);
        ImGui::Text("Resultado %s", metricas.resultado == BPM::SUCESSO ? "SUCESSO" : "FALHA");
        ImGui::Text("Saída: (%d, %d)", metricas.saida.x, metricas.saida.y);
        ImGui::Text("Tamanho Caminho: %d", metricas.tamanho_caminho);
        ImGui::Text("Tamanho Max Lista Aberta: %d", metricas.tamanho_max_fila);
        //ImGui::Text("Tamanho Max Lista Fechada: %d", metricas.tamanho_max_lista_fec);
        ImGui::Text("Tempo de execução: %.0lf ms", metricas.tempo_execucao_ms);
        ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(renderer, 0.3, 0.3, 0.3, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }


    // Cleanup
    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}