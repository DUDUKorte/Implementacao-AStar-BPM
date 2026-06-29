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

#include "Widgets.h"

// Widget Do Labirinto do A* (ImGui)
void ImLabirintoAStar(Matriz matriz, const std::list<AStar::No>& caminhoExplorado, const std::vector<AStar::Posicao> caminhoSolucao, const char* nomeLabirinto = "Labirinto", AStar::EstadoAStar* estado = nullptr) {
    // verificar se a matriz n tá vazia
    if (matriz.empty()) { return; }

    // Cria a janela
    ImGui::Begin(nomeLabirinto);
    ImGui::Text("Labirinto A*");
    ImGui::Separator();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

    int linhas = matriz.size();
    int colunas = matriz[0].size();

    // Calcular o tamanho da celula
    float tamanho = ImGui::GetContentRegionAvail().x / colunas;

    // Determina o caminho que o A* percorreu e marca com um número correspondente
    if (estado != nullptr && !estado->finalizado) {
        for (AStar::No i : caminhoExplorado) {
            matriz[i.posicao.x][i.posicao.y] = 2; // caminho percorrido
        }
    }

    if (estado != nullptr && estado->finalizado) {
        for (auto i : caminhoSolucao) {
            matriz[i.x][i.y] = 3; // caminho encontrado
        }
    }

    // Determina a cor e desenha todas as células do labirinto
    for (int y = 0; y < linhas; y++) {
        for (int x = 0; x < colunas; x++) {
            int valorCelula = matriz[y][x];

            // Tamanho mínimo e máximo de cada célula
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
                color = IM_COL32(0, 0, 200, 255); // caminho encontrado, azul
                break;
            default:
                color = IM_COL32(255, 0, 0, 255); // deu ruim na leitura, vermelho
                break;
            }
            // Adiciona os elementos na lista do imgui
            draw_list->AddRectFilled(p_min, p_max, color);
            draw_list->AddRect(p_min, p_max, IM_COL32(20, 20, 20, 255)); //borda
        }
    }
    // Dummy pra permitir scroll vertial
    ImGui::Dummy(ImVec2(colunas * tamanho, linhas * tamanho));
    ImGui::End();
}

// Widget Do Labirinto do BPM (ImGui)
void ImLabirintoBPM(Matriz matriz, const std::vector<BPM::Posicao>& caminhoExplorado, const std::vector<BPM::Posicao> caminhoSolucao, const char* nomeLabirinto = "Labirinto", BPM::DadosUteis* metricas = nullptr) {
    // verificar se a matriz n tá vazia
    if (matriz.empty()) { return; }

    // Cria a janela
    ImGui::Begin(nomeLabirinto);
    ImGui::Text("Labirinto BPM");
    ImGui::Separator();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

    int linhas = matriz.size();
    int colunas = matriz[0].size();

    // Calcular o tamanho da celula
    float tamanho = ImGui::GetContentRegionAvail().x / colunas;

    // Determina o caminho que o A* percorreu e marca com um número correspondente
    if (metricas != nullptr && !metricas->encontrou_solucao) {
        for (BPM::Posicao i : caminhoExplorado) {
            matriz[i.x][i.y] = 2; // Caminho percorrido
        }
    }

    if (metricas != nullptr && metricas->encontrou_solucao) {
        for (auto i : caminhoSolucao) {
            matriz[i.x][i.y] = 3; // Caminho encontrado
        }
    }

    // Determina a cor e desenha todas as células do labirinto
    for (int y = 0; y < linhas; y++) {
        for (int x = 0; x < colunas; x++) {
            int valorCelula = matriz[y][x];

            // Tamanho mínimo e máximo de cada célula
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
                color = IM_COL32(0, 0, 200, 255); // caminho encontrado, azul
                break;
            default:
                color = IM_COL32(255, 0, 0, 255); // deu ruim na leitura, vermelho
                break;
            }
            // Adiciona os elementos na lista do imgui
            draw_list->AddRectFilled(p_min, p_max, color);
            draw_list->AddRect(p_min, p_max, IM_COL32(20, 20, 20, 255)); //borda
        }
    }

    // Dummy pra permitir scroll vertial
    ImGui::Dummy(ImVec2(colunas * tamanho, linhas * tamanho));
    ImGui::End();
}

// Função base de exemplo de dockspace do ImGui
// Foi adapatada para nossas necessidades
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