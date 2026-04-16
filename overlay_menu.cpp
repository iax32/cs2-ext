#include "overlay_menu.hpp"

#include <imgui.h>

namespace overlay {

    void OverlayMenu::render() {
        if (!is_open_) {
            return;
        }

        ImGui::SetNextWindowSize({ 320.0F, 180.0F }, ImGuiCond_FirstUseEver);
        ImGui::Begin("Overlay Menu", &is_open_, ImGuiWindowFlags_NoCollapse);
        ImGui::TextUnformatted("Minimal transparent overlay");
        ImGui::Separator();
        ImGui::Checkbox("Show demo text", &show_demo_text_);
        ImGui::SliderFloat("Opacity", &menu_opacity_, 0.20F, 1.00F, "%.2f");
        ImGui::Text("Press INSERT to toggle this menu");
        ImGui::End();

        if (show_demo_text_) {
            ImGui::SetNextWindowBgAlpha(0.35F);
            ImGui::Begin(
                "Overlay Label",
                nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav
            );
            ImGui::TextUnformatted("Hello from ImGui + DirectX 11");
            ImGui::End();
        }
    }

    bool& OverlayMenu::open() noexcept {
        return is_open_;
    }

    bool OverlayMenu::click_through() const noexcept {
        return click_through_;
    }

    float OverlayMenu::opacity() const noexcept {
        return menu_opacity_;
    }

} // namespace overlay