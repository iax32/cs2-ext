#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dwmapi.h>

#include <string_view>

namespace overlay {

    class OverlayWindow final {
    public:
        OverlayWindow() = default;
        OverlayWindow(const OverlayWindow&) = delete;
        OverlayWindow& operator=(const OverlayWindow&) = delete;
        OverlayWindow(OverlayWindow&&) = delete;
        OverlayWindow& operator=(OverlayWindow&&) = delete;

        ~OverlayWindow();

        void create(HINSTANCE instance, std::wstring_view title, int width, int height);
        void destroy() noexcept;

        [[nodiscard]] HWND handle() const noexcept;
        [[nodiscard]] int width() const noexcept;
        [[nodiscard]] int height() const noexcept;

        void set_click_through(bool enabled) const noexcept;
        [[nodiscard]] bool process_messages() const noexcept;

    private:
        static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

    private:
        HINSTANCE instance_ = nullptr;
        HWND hwnd_ = nullptr;
        int width_ = 1280;
        int height_ = 720;
    };

} // namespace overlay