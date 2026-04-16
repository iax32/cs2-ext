#include "imgui_layer.hpp"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "theme.hpp"
#include "win32_error.hpp"

namespace overlay {

    ImGuiLayer::~ImGuiLayer() {
        shutdown();
    }

    void ImGuiLayer::initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ApplyMinimalDarkTheme();

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 10.0F;
        style.FrameRounding = 6.0F;
        style.Colors[ImGuiCol_WindowBg].w = 0.92F;

        if (!ImGui_ImplWin32_Init(hwnd)) {
            throw Win32Error("Failed to initialize ImGui Win32 backend.");
        }

        if (!ImGui_ImplDX11_Init(device, device_context)) {
            throw Win32Error("Failed to initialize ImGui DX11 backend.");
        }
    }

    void ImGuiLayer::shutdown() noexcept {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();

        if (ImGui::GetCurrentContext()) {
            ImGui::DestroyContext();
        }
    }

    void ImGuiLayer::begin_frame() const {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end_frame() const {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

} // namespace overlay