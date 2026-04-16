#include "d3d11_context.hpp"

#include <array>

#include "win32_error.hpp"

namespace overlay {

    D3D11Context::~D3D11Context() {
        cleanup();
    }

    void D3D11Context::initialize(HWND hwnd, std::uint32_t width, std::uint32_t height) {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
        swap_chain_desc.BufferCount = 2;
        swap_chain_desc.BufferDesc.Width = width;
        swap_chain_desc.BufferDesc.Height = height;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.OutputWindow = hwnd;
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.Windowed = TRUE;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        constexpr std::array feature_levels = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0
        };

        UINT create_device_flags = 0;
#ifdef _DEBUG
        create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        const auto result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            create_device_flags,
            feature_levels.data(),
            static_cast<UINT>(feature_levels.size()),
            D3D11_SDK_VERSION,
            &swap_chain_desc,
            &swap_chain_,
            &device_,
            nullptr,
            &device_context_
        );

        if (FAILED(result)) {
            throw Win32Error("Failed to create D3D11 device and swap chain.");
        }

        create_render_target();
    }

    void D3D11Context::resize(std::uint32_t width, std::uint32_t height) {
        if (!device_ || !swap_chain_) {
            return;
        }

        cleanup_render_target();

        const auto result = swap_chain_->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        if (FAILED(result)) {
            throw Win32Error("Failed to resize swap chain buffers.");
        }

        create_render_target();
    }

    void D3D11Context::begin_frame() const {
        constexpr float clear_color[4] = { 0.0F, 0.0F, 0.0F, 0.0F };
        device_context_->OMSetRenderTargets(1, &render_target_view_, nullptr);
        device_context_->ClearRenderTargetView(render_target_view_, clear_color);
    }

    void D3D11Context::present() const {
        swap_chain_->Present(1, 0);
    }

    ID3D11Device* D3D11Context::device() const noexcept {
        return device_;
    }

    ID3D11DeviceContext* D3D11Context::device_context() const noexcept {
        return device_context_;
    }

    void D3D11Context::create_render_target() {
        ID3D11Texture2D* back_buffer = nullptr;
        const auto result = swap_chain_->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
        if (FAILED(result)) {
            throw Win32Error("Failed to get swap chain back buffer.");
        }

        const auto rtv_result = device_->CreateRenderTargetView(back_buffer, nullptr, &render_target_view_);
        back_buffer->Release();

        if (FAILED(rtv_result)) {
            throw Win32Error("Failed to create render target view.");
        }
    }

    void D3D11Context::cleanup_render_target() noexcept {
        if (render_target_view_) {
            render_target_view_->Release();
            render_target_view_ = nullptr;
        }
    }

    void D3D11Context::cleanup() noexcept {
        cleanup_render_target();

        if (swap_chain_) {
            swap_chain_->Release();
            swap_chain_ = nullptr;
        }

        if (device_context_) {
            device_context_->Release();
            device_context_ = nullptr;
        }

        if (device_) {
            device_->Release();
            device_ = nullptr;
        }
    }

} // namespace overlay