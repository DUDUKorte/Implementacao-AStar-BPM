#pragma once
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

#include <AStar.h>
#include <BPM.h>

// Argumentos do dockspace (ImGui)
struct ImGuiDemoDockspaceArgs
{
    bool                IsFullscreen = true;
    bool                KeepWindowPadding = false; // Keep WindowPadding to help understand that DockSpace() is a widget inside the window.
    ImGuiDockNodeFlags  DockSpaceFlags = ImGuiDockNodeFlags_None;
};

// Usando nome matriz ao invés desse vector gigante...
using Matriz = std::vector<std::vector<int>>;

// Widget Do Labirinto do A* (ImGui)
void ImLabirintoAStar(Matriz matriz, const std::list<AStar::No>& caminhoExplorado, const std::vector<AStar::Posicao> caminhoSolucao, const char* nomeLabirinto, AStar::EstadoAStar* estado);

// Widget Do Labirinto do BPM (ImGui)
void ImLabirintoBPM(Matriz matriz, const std::vector<BPM::Posicao>& caminhoExplorado, const std::vector<BPM::Posicao> caminhoSolucao, const char* nomeLabirinto, BPM::DadosUteis* metricas);

// Função base de exemplo de dockspace do ImGui
// Foi adapatada para nossas necessidades
void ShowExampleAppDockSpace(bool* p_open);