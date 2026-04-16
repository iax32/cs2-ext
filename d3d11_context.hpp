#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>

#include <cstdint>

namespace overlay {

    class D3D11Context final {
    public:
        D3D11Context() = default;
        D3D11Context(const D3D11Context&) = delete;
        D3D11Context& operator=(const D3D11Context&) = delete;
        D3D11Context(D3D11Context&&) = delete;
        D3D11Context& operator=(D3D11Context&&) = delete;

        ~D3D11Context();

        void initialize(HWND hwnd, std::uint32_t width, std::uint32_t height);
        void resize(std::uint32_t width, std::uint32_t height);
        void begin_frame() const;
        void present() const;

        [[nodiscard]] ID3D11Device* device() const noexcept;
        [[nodiscard]] ID3D11DeviceContext* device_context() const noexcept;

    private:
        void create_render_target();
        void cleanup_render_target() noexcept;
        void cleanup() noexcept;

    private:
        IDXGISwapChain* swap_chain_ = nullptr;
        ID3D11Device* device_ = nullptr;
        ID3D11DeviceContext* device_context_ = nullptr;
        ID3D11RenderTargetView* render_target_view_ = nullptr;
    };

} // namespace overlay