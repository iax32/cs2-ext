#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>

namespace overlay {

    class ImGuiLayer final {
    public:
        ImGuiLayer() = default;
        ImGuiLayer(const ImGuiLayer&) = delete;
        ImGuiLayer& operator=(const ImGuiLayer&) = delete;
        ImGuiLayer(ImGuiLayer&&) = delete;
        ImGuiLayer& operator=(ImGuiLayer&&) = delete;

        ~ImGuiLayer();

        void initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
        void shutdown() noexcept;
        void begin_frame() const;
        void end_frame() const;
    };

} // namespace overlay