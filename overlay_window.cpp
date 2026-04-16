#include "overlay_window.hpp"

#include <imgui_impl_win32.h>

#include "win32_error.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hwnd,
    UINT msg,
    WPARAM w_param,
    LPARAM l_param
);

namespace overlay {

    OverlayWindow::~OverlayWindow() {
        destroy();
    }

    void OverlayWindow::create(HINSTANCE instance, std::wstring_view title, int width, int height) {
        instance_ = instance;
        width_ = width;
        height_ = height;

        WNDCLASSEXW window_class{};
        window_class.cbSize = sizeof(window_class);
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = &OverlayWindow::window_proc;
        window_class.hInstance = instance_;
        window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
        window_class.lpszClassName = L"MinimalTransparentOverlayWindow";

        if (!RegisterClassExW(&window_class)) {
            throw Win32Error("Failed to register window class.");
        }

        hwnd_ = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
            window_class.lpszClassName,
            title.data(),
            WS_POPUP,
            0,
            0,
            width_,
            height_,
            nullptr,
            nullptr,
            instance_,
            this
        );

        if (!hwnd_) {
            throw Win32Error("Failed to create overlay window.");
        }

        SetLayeredWindowAttributes(hwnd_, RGB(0, 0, 0), 255, LWA_ALPHA);

        MARGINS margins{ -1 };
        DwmExtendFrameIntoClientArea(hwnd_, &margins);

        ShowWindow(hwnd_, SW_SHOW);
        UpdateWindow(hwnd_);
    }

    void OverlayWindow::destroy() noexcept {
        if (hwnd_) {
            DestroyWindow(hwnd_);
            hwnd_ = nullptr;
        }
    }

    HWND OverlayWindow::handle() const noexcept {
        return hwnd_;
    }

    int OverlayWindow::width() const noexcept {
        return width_;
    }

    int OverlayWindow::height() const noexcept {
        return height_;
    }

    void OverlayWindow::set_click_through(bool enabled) const noexcept {
        auto ex_style = GetWindowLongPtrW(hwnd_, GWL_EXSTYLE);

        if (enabled) {
            ex_style |= WS_EX_TRANSPARENT;
        }
        else {
            ex_style &= ~static_cast<LONG_PTR>(WS_EX_TRANSPARENT);
        }

        SetWindowLongPtrW(hwnd_, GWL_EXSTYLE, ex_style);
    }

    bool OverlayWindow::process_messages() const noexcept {
        MSG msg{};
        while (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);

            if (msg.message == WM_QUIT) {
                return false;
            }
        }

        return true;
    }

    LRESULT CALLBACK OverlayWindow::window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param)) {
            return TRUE;
        }

        auto* self = reinterpret_cast<OverlayWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

        if (msg == WM_NCCREATE) {
            const auto* create_struct = reinterpret_cast<CREATESTRUCTW*>(l_param);
            self = reinterpret_cast<OverlayWindow*>(create_struct->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        }

        switch (msg) {
        case WM_SIZE:
            if (w_param != SIZE_MINIMIZED && self) {
                self->width_ = LOWORD(l_param);
                self->height_ = HIWORD(l_param);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcW(hwnd, msg, w_param, l_param);
        }
    }

} // namespace overlay