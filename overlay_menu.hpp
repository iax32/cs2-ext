#pragma once

namespace overlay {

    class OverlayMenu final {
    public:
        void render();

        [[nodiscard]] bool& open() noexcept;
        [[nodiscard]] bool click_through() const noexcept;
        [[nodiscard]] float opacity() const noexcept;

    private:
        bool is_open_ = true;
        bool show_demo_text_ = true;
        bool click_through_ = true;
        float menu_opacity_ = 1.0F;
    };

} // namespace overlay