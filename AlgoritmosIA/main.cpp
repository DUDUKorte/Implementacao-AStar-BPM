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
#include <AStar.h>
#include <BPM.h>

struct ImGuiDemoDockspaceArgs
{
    bool                IsFullscreen = true;
    bool                KeepWindowPadding = false; // Keep WindowPadding to help understand that DockSpace() is a widget inside the window.
    ImGuiDockNodeFlags  DockSpaceFlags = ImGuiDockNodeFlags_None;
};

using Matriz = std::vector<std::vector<int>>;

void ImLabirinto(Matriz matriz, const std::list<AStar::No>& listaAberta, const std::vector<AStar::Posicao> listaFechada, const char *nomeLabirinto = "Labirinto") {
    // verificar se a matriz n tá vazia
    if (matriz.empty()) { return; }
    ImGui::Begin(nomeLabirinto);
    ImGui::Text("Labirinto A*");
    ImGui::Separator();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

    int linhas = matriz.size();
    int colunas = matriz[0].size();

    // Calcular o tamanho da celula
    float tamanho = ImGui::GetContentRegionAvail().x / colunas;

    for (AStar::No i : listaAberta) {
        matriz[i.posicao.x][i.posicao.y] = 2;
    }
    
    for (auto i : listaFechada) {
        matriz[i.x][i.y] = 3;
    }

    for (int y = 0; y < linhas; y++) {
        for (int x = 0; x < colunas; x++) {
            int valorCelula = matriz[y][x];

            ImVec2 p_min = ImVec2(cursor_pos.x + (x * tamanho), cursor_pos.y + (y * tamanho));
            ImVec2 p_max = ImVec2(p_min.x + tamanho, p_min.y + tamanho);

            ImU32 color;

            switch (valorCelula) {
            case 0:
                color = IM_COL32(40, 40, 40, 255); // caminho liberado, cinza
                break;
            case 1:
                color = IM_COL32(200, 200, 200, 255); // parede, branco escuro
                break;
            case 2:
                color = IM_COL32(0, 200, 0, 255); // caminho percorrido, verde
                break;
            case 3:
                color = IM_COL32(0, 0, 200, 255);
                break;
            default:
                color = IM_COL32(255, 0, 0, 255); // deu bosta na leitura, vermelho
                break;
            }

            draw_list->AddRectFilled(p_min, p_max, color);
            draw_list->AddRect(p_min, p_max, IM_COL32(20, 20, 20, 255)); //borda
        }
    }

    ImGui::Dummy(ImVec2(colunas * tamanho, linhas * tamanho));
    ImGui::End();
}

void ImLabirintoBPM(Matriz matriz, const std::vector<BPM::Posicao>& listaAberta, const std::vector<BPM::Posicao> listaFechada, const char* nomeLabirinto = "Labirinto") {
    // verificar se a matriz n tá vazia
    if (matriz.empty()) { return; }
    ImGui::Begin(nomeLabirinto);
    ImGui::Text("Labirinto BPM");
    ImGui::Separator();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

    int linhas = matriz.size();
    int colunas = matriz[0].size();

    // Calcular o tamanho da celula
    float tamanho = ImGui::GetContentRegionAvail().x / colunas;

    for (BPM::Posicao i : listaAberta) {
        matriz[i.x][i.y] = 2;
    }

    for (auto i : listaFechada) {
        matriz[i.x][i.y] = 3;
    }

    for (int y = 0; y < linhas; y++) {
        for (int x = 0; x < colunas; x++) {
            int valorCelula = matriz[y][x];

            ImVec2 p_min = ImVec2(cursor_pos.x + (x * tamanho), cursor_pos.y + (y * tamanho));
            ImVec2 p_max = ImVec2(p_min.x + tamanho, p_min.y + tamanho);

            ImU32 color;

            switch (valorCelula) {
            case 0:
                color = IM_COL32(40, 40, 40, 255); // caminho liberado, cinza
                break;
            case 1:
                color = IM_COL32(200, 200, 200, 255); // parede, branco escuro
                break;
            case 2:
                color = IM_COL32(0, 200, 0, 255); // caminho percorrido, verde
                break;
            case 3:
                color = IM_COL32(0, 0, 200, 255);
                break;
            default:
                color = IM_COL32(255, 0, 0, 255); // deu bosta na leitura, vermelho
                break;
            }

            draw_list->AddRectFilled(p_min, p_max, color);
            draw_list->AddRect(p_min, p_max, IM_COL32(20, 20, 20, 255)); //borda
        }
    }

    ImGui::Dummy(ImVec2(colunas * tamanho, linhas * tamanho));
    ImGui::End();
}

void ShowExampleAppDockSpace(bool* p_open)
{
    static int opt_demo_mode = 0;
    static bool opt_demo_mode_changed = false;
    static ImGuiDemoDockspaceArgs args;

    ImGuiDockNodeFlags dockspace_flags = args.DockSpaceFlags;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    // Fullscreen dockspace: practically the same as calling DockSpaceOverViewport();
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;

    // dockspace flags
    args.DockSpaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::Begin("Window with a DockSpace", p_open, window_flags);

    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // Submit the DockSpace widget inside our window
    // - Note that the id here is different from the one used by DockSpaceOverViewport(), so docking state won't get transfered between "Basic" and "Advanced" demos.
    // - If we made the ShowExampleAppDockSpaceBasic() calculate its own ID and pass it to DockSpaceOverViewport() the ID could easily match.
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();

    // Refocus our window to minimize perceived loss of focus when changing mode (caused by the fact that each use a different window, which would not happen in a real app)
    if (opt_demo_mode_changed)
        ImGui::SetNextWindowFocus();
}

Matriz LoadMaze(int altura, int largura) {
    std::string nomeDoArquivo = std::format("data/maze_{0}x{1}.csv", altura, largura);

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
    std::string nomeDoArquivo = std::format("data/maze_{0}x{1}.csv", altura_labirinto, largura_labirinto);
    
    // Pega um entre os 5 labirintos aleatórios no arquivo
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(1, 5);
    int numeroAlvo = distr(gen);

    // Carrega o primeiro labirinto
    Matriz LABIRINTO = lerLabirinto(nomeDoArquivo, numeroAlvo);

    if (!LABIRINTO.empty()) {
        std::cout << "Matriz \"" << numeroAlvo << "\" carregada com sucesso!\n" << std::endl;
        imprimirMatriz(LABIRINTO);
        std::cout << "--------------------------------------------------------------------\n" << std::endl;
    } else {
        return 1;
    }

    // DadosUteis dados_AStar = AStar(LABIRINTO, entrada, saida, nomeDoArquivo, numeroAlvo);

    // incializa o A*
    AStar::EstadoAStar estado = AStar::Init(LABIRINTO, 1);

    std::vector<AStar::Posicao>& aStarListaFechada = estado.dados.caminho_solucao;
    std::list<AStar::No>& aStarListaAberta = estado.fechada;

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
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
    //io.ConfigDpiScaleFonts = true;        // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    //io.ConfigDpiScaleViewports = true;    // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool auto_run_aStar = false;
    bool auto_run_BPM = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;

    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        // [If using SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your SDL_AppEvent() function]
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Verificações dos algoritmos para rodar automático
        if (auto_run_aStar) {
            AStar::Increment(estado);
        }
        if (auto_run_BPM) {
            BPM::Iterate(LABIRINTO, metricas);
        }

        // Setup ImGui Dockspace.
        ShowExampleAppDockSpace(&show_demo_window);

        // Mostra as janelas de cada labirinto utilizando o ImGui
        ImLabirinto(LABIRINTO, aStarListaAberta, aStarListaFechada, "Labirinto A*");
        ImLabirintoBPM(LABIRINTO, metricas.nos_explorados, metricas.caminho_solucao, "Labirinto BPM");

        // Mostra janela principal de controles
        {
            ImGui::Begin("Controles");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Controles"); ImGui::Separator();
            ImGui::SeparatorText("Labirinto");
            if (ImGui::SliderInt("Altura", &altura_labirinto, 5, 99)) {
                LABIRINTO = LoadMaze(altura_labirinto, largura_labirinto);
                estado = AStar::Init(LABIRINTO, 1);
                metricas = BPM::Init(LABIRINTO);
            }
            if(ImGui::SliderInt("Largura", &largura_labirinto, 5, 99)){
                LABIRINTO = LoadMaze(altura_labirinto, largura_labirinto);
                estado = AStar::Init(LABIRINTO, 1);
                metricas = BPM::Init(LABIRINTO);
            }
            if (largura_labirinto * altura_labirinto > 1600) {
                ImGui::TextColored(ImVec4(0.9, 0.2, 0.2, 255), "CUIDADO!!!!!");
                ImGui::TextColored(ImVec4(0.9, 0.2, 0.2, 255), "Valores muito altos podem afetar o desempenho!");
            }
            if (ImGui::Button("Recarregar Labirinto")) {
                LABIRINTO = LoadMaze(altura_labirinto, largura_labirinto);
                estado = AStar::Init(LABIRINTO, 1);
                metricas = BPM::Init(LABIRINTO);
            }

            ImGui::SeparatorText("Controles Step-By-Step");
            if (ImGui::Button("Incrementar Ambos")) {
                AStar::Increment(estado);
                BPM::Iterate(LABIRINTO, metricas);
            }
            ImGui::SameLine();

            if (ImGui::Button("Incrementar A*")) {
                AStar::Increment(estado);
            }
            ImGui::SameLine();

            if (ImGui::Button("Incrementar BPM")) {
                BPM::Iterate(LABIRINTO, metricas);
            }

            ImGui::SeparatorText("Controles Automáticos");
            if (ImGui::Button("RUN!")) {
                auto_run_aStar = true;
                auto_run_BPM = true;
            }
            ImGui::SameLine();

            if (ImGui::Button("MT RUN!")) {
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

            if (ImGui::Button("Reset")) {
                estado = AStar::Init(LABIRINTO, 10);
                metricas = BPM::Init(LABIRINTO);
            }

            ImGui::SeparatorText("Controles indivduais");
            ImGui::Checkbox("Auto A*", &auto_run_aStar);
            ImGui::SameLine();
            ImGui::Checkbox("Auto BPM", &auto_run_BPM);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
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