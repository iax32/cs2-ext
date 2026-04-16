#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "d3d11_context.hpp"
#include "imgui_layer.hpp"
#include "overlay_menu.hpp"
#include "overlay_window.hpp"

namespace overlay {

    class OverlayApp final {
    public:
        explicit OverlayApp(HINSTANCE instance);
        int screen_width;
        int screen_height;
        int run();

    private:
        void handle_hotkeys() noexcept;
        void render();
        

    private:
        HINSTANCE instance_ = nullptr;
        OverlayWindow window_{};
        D3D11Context graphics_{};
        ImGuiLayer imgui_{};
        OverlayMenu menu_{};
    };

} // namespace overlay