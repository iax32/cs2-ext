#include "overlay_app.hpp"
#include "windows_api.hpp"
#include <iostream>
#include "type.h"
#include "offsets.hpp"
#include "sdk.hpp"
#include <vector>

#include <imgui.h>

namespace overlay {

    OverlayApp::OverlayApp(HINSTANCE instance)
        : screen_width(GetSystemMetrics(SM_CXSCREEN)),
          screen_height(GetSystemMetrics(SM_CYSCREEN)),
          instance_(instance)
    {
    }

    int OverlayApp::run() {
        screen_width = GetSystemMetrics(SM_CXSCREEN);
        screen_height = GetSystemMetrics(SM_CYSCREEN);

        window_.create(
            instance_,
            L"Minimal Transparent Overlay",
            screen_width,
            screen_height
        );
        graphics_.initialize(
            window_.handle(),
            static_cast<std::uint32_t>(window_.width()),
            static_cast<std::uint32_t>(window_.height())
        );
        imgui_.initialize(window_.handle(), graphics_.device(), graphics_.device_context());

        // MAIN ACTION HERE
        while (window_.process_messages()) {
            handle_hotkeys();
            window_.set_click_through(!menu_.open());
            render();
        }

        return 0;
    }

    void OverlayApp::handle_hotkeys() noexcept {
        static bool insert_was_down = false;
        const bool insert_is_down = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;

        if (insert_is_down && !insert_was_down) {
            menu_.open() = !menu_.open();
        }

        insert_was_down = insert_is_down;
    }

    // Call this inside your main ImGui render loop
    void RenderPlayerStatBoard(std::vector<Entity>& entities, const Entity& localPlayer)
    {
        // Control flags
        static bool onlyShowEnemies = false;

        ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Player Stats", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Checkbox("Only show enemies", &onlyShowEnemies);

            // ---------------------------
            // Teammates Table
            // ---------------------------
            if (!onlyShowEnemies)
            {
                if (ImGui::CollapsingHeader("Teammates", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    if (ImGui::BeginTable("TeammatesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                    {
                        ImGui::TableSetupColumn("Name");
                        ImGui::TableSetupColumn("Health");
                        ImGui::TableSetupColumn("Armor");
                        ImGui::TableHeadersRow();

                        for (const auto& entity : entities)
                        {
                            // Valid entity checks
                            if (entity.GetBase() == 0 || entity.GetBase() == localPlayer.GetBase())
                                continue;
                            if (entity.GetTeam() != localPlayer.GetTeam()) // only teammates
                                continue;
                            if (entity.GetHealth() == 0)
                                continue;

                            ImGui::TableNextRow();

                            ImGui::TableSetColumnIndex(0);
                            ImGui::TextUnformatted(entity.GetName().c_str());

                            ImGui::TableSetColumnIndex(1);
                            float healthPercent = entity.GetHealth() / 100.0f; // adjust max health
                            ImVec4 healthCol = healthPercent > 0.5f ? ImVec4(0, 1, 0, 1) :
                                healthPercent > 0.25f ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1);
                            ImGui::TextColored(healthCol, "%d", entity.GetHealth());

                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("%d", entity.GetArmor());
                        }
                        ImGui::EndTable();
                    }
                }
            }

            // ---------------------------
            // Enemies Table
            // ---------------------------
            if (ImGui::CollapsingHeader("Enemies", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginTable("EnemiesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
                    ImGui::TableSetupColumn("Name");
                    ImGui::TableSetupColumn("Health");
                    ImGui::TableSetupColumn("Armor");
                    ImGui::TableHeadersRow();

                    for (const auto& entity : entities)
                    {
                        if (entity.GetBase() == 0 || entity.GetBase() == localPlayer.GetBase())
                            continue;
                        if (entity.GetTeam() == localPlayer.GetTeam()) // only enemies
                            continue;
                        if (entity.GetHealth() == 0)
                            continue;

                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted(entity.GetName().c_str());

                        ImGui::TableSetColumnIndex(1);
                        float healthPercent = entity.GetHealth() / 100.0f;
                        ImVec4 healthCol = healthPercent > 0.5f ? ImVec4(0, 1, 0, 1) :
                            healthPercent > 0.25f ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1);
                        ImGui::TextColored(healthCol, "%d", entity.GetHealth());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%d", entity.GetArmor());
                    }
                    ImGui::EndTable();
                }
            }
        }
        ImGui::End();
    }

    VOID line(const ImVec2& point1, const ImVec2 point2, ImU32 color, const FLOAT line_width) {
        ImGui::GetBackgroundDrawList()->AddLine(point1, point2, color, line_width);
    }

    // Visual Settings
    namespace Visuals {
        inline bool bESP = true;
        inline bool bBox = true;
        inline bool bHealthBar = true;
        inline bool bSnapline = false;
        inline bool bHeadDot = true;
        inline float fMaxDistance = 200.0f;
    }

    // Aim Settings
    namespace Aim {
        inline bool bAimbot = false;
        inline float fFOV = 5.0f;
        inline float fSmooth = 2.0f;
        inline int iBone = 0;                // 0 = Head, 1 = Neck, 2 = Chest, 3 = Pelvis
        inline bool bVisibleCheck = true;
        inline bool bTriggerbot = false;
    }

    // Misc Settings
    namespace Misc {
        inline bool bBunnyHop = false;
        inline bool bAutoStrafe = false;
        inline bool bNoFlash = true;
        inline float fFlashAlpha = 0.3f;
        inline bool bRadar = true;
        inline bool bSpectatorList = false;
    }

    void RenderMenu()
    {
        // Create a small, fixed window for the menu
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Cheat Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
        {
            if (ImGui::BeginTabBar("MainTabs"))
            {
                // ------- VISUAL TAB -------
                if (ImGui::BeginTabItem("Visual"))
                {
                    ImGui::Checkbox("ESP Enabled", &Visuals::bESP);
                    ImGui::Checkbox("Box ESP", &Visuals::bBox);
                    ImGui::Checkbox("Health Bar", &Visuals::bHealthBar);
                    ImGui::Checkbox("Snaplines", &Visuals::bSnapline);
                    ImGui::Checkbox("Head Dot", &Visuals::bHeadDot);
                    ImGui::SliderFloat("ESP Max Distance", &Visuals::fMaxDistance, 0.0f, 500.0f, "%.1f");

                    ImGui::EndTabItem();
                }

                // ------- AIM TAB -------
                if (ImGui::BeginTabItem("Aim"))
                {
                    ImGui::Checkbox("Aimbot Enabled", &Aim::bAimbot);
                    ImGui::SliderFloat("FOV", &Aim::fFOV, 1.0f, 180.0f, "%.1f");
                    ImGui::SliderFloat("Smooth", &Aim::fSmooth, 1.0f, 20.0f, "%.1f");

                    const char* bones[] = { "Head", "Neck", "Chest", "Pelvis" };
                    ImGui::Combo("Bone", &Aim::iBone, bones, IM_ARRAYSIZE(bones));

                    ImGui::Checkbox("Visibility Check", &Aim::bVisibleCheck);
                    ImGui::Checkbox("Triggerbot", &Aim::bTriggerbot);

                    ImGui::EndTabItem();
                }

                // ------- MISC TAB -------
                if (ImGui::BeginTabItem("Misc"))
                {
                    ImGui::Checkbox("Bunny Hop", &Misc::bBunnyHop);
                    ImGui::Checkbox("Auto Strafe", &Misc::bAutoStrafe);
                    ImGui::Checkbox("No Flash", &Misc::bNoFlash);
                    ImGui::SliderFloat("Flash Alpha", &Misc::fFlashAlpha, 0.0f, 1.0f, "%.2f");
                    ImGui::Checkbox("Radar Hack", &Misc::bRadar);
                    ImGui::Checkbox("Spectator List", &Misc::bSpectatorList);

                    ImGui::EndTabItem();
                }

                // ------- LINEUPS TAB -------
                if (ImGui::BeginTabItem("Lineups"))
                {
                    ImGui::Text("Lineup Manager");
                    ImGui::Separator();

                    static char lineupName[64] = "";
                    ImGui::InputText("Name", lineupName, IM_ARRAYSIZE(lineupName));

                    if (ImGui::Button("Save Current Position"))
                    {
                        // Save player position/orientation to file/array
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Load Selected"))
                    {
                        // Load the chosen lineup
                    }

                    ImGui::BeginChild("LineupList", ImVec2(0, 150), true);
                    // Placeholder list of saved lineups
                    ImGui::Selectable("Mid to B smoke", false);
                    ImGui::Selectable("A long flash", false);
                    ImGui::Selectable("CT spawn molly", false);
                    ImGui::EndChild();

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }

    void OverlayApp::render() {
        graphics_.begin_frame();
        imgui_.begin_frame();

        ImGui::SetNextWindowBgAlpha(menu_.opacity());
        menu_.render();

        DWORD iProcessId = NULL;
        std::uintptr_t pModuleBase = NULL;

        bool opened_process = false;
        if (opened_process == false)
        {
            iProcessId = WinApi::GetProcessId(L"cs2.exe");
            pModuleBase = WinApi::GetModuleBaseAddress(iProcessId, L"client.dll");

            if (iProcessId) {
                hProcess = OpenProcess(PROCESS_VM_READ, FALSE, iProcessId);
                if (hProcess) {
                    opened_process = true;
                }
            }
            else {
                std::cerr << "Failed to find process ID." << std::endl;
            }
            if (!opened_process) {
                std::cerr << "Failed to open process." << std::endl;
            }
        }

        uintptr_t entitylist = 0;
        uintptr_t entitylistEntry = 0;

        entitylist = WinApi::Read<uintptr_t>(pModuleBase + Offsets::ClientDll::DwEntityList);
        entitylistEntry = WinApi::Read<std::uintptr_t>(entitylist + 0x10);

        Matrix matrix{ };
        matrix = WinApi::Read<Matrix>(pModuleBase + Offsets::ClientDll::DwViewMatrix);

		Entity localPlayer{ };
		localPlayer.SetBase(WinApi::Read<std::uintptr_t>(pModuleBase + Offsets::ClientDll::DwLocalPlayerPawn));
		localPlayer.Update();

		std::vector<Entity> entities(64);
		std::vector<Entity>* p_vEntities = &entities;

        if (pModuleBase != 0 && entitylist != 0 && entitylistEntry != 0)
        {
            for (std::size_t i{ 0 }; i < entities.size(); i++)
            {
                Entity &entityRef = entities[i];

                entityRef.SetController(WinApi::Read<uintptr_t>(entitylistEntry + i * 0x70));
                if (!entityRef.GetController())
                    continue;

                std::uintptr_t pawnHandle = WinApi::Read<std::uintptr_t>(entityRef.GetController() + 0x90C);
                std::uintptr_t listentry2 = WinApi::Read<std::uintptr_t>(
                    entitylist + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

                entityRef.SetBase(GetPawnBase(entityRef.GetController(), entitylist));
                entityRef.Update();


                if (entityRef.GetBase() == localPlayer.GetBase())
                    continue;

                if (localPlayer.GetTeam() == entityRef.GetTeam())
                    continue;

                if (entityRef.GetHealth() == 0)
                    continue;

                entityRef.ReadBones(matrix);
                if (!entityRef.Render(matrix, this->screen_width, this->screen_height))
                    continue;

                for (auto [bi, bj] : kBoneConnections) {
                    if (!entityRef.GetVisibleBones()[bi] || !entityRef.GetVisibleBones()[bj])
                        continue;

                    ImVec2 boneOnePx = NDCToScreen(entityRef.GetBones2D()[bi], this->screen_width, this->screen_height);
                    ImVec2 boneTwoPx = NDCToScreen(entityRef.GetBones2D()[bj], this->screen_width, this->screen_height);


                    line(boneOnePx, boneTwoPx, ImColor(3, 232, 244), 2.f);


                }



            }
        }

        RenderPlayerStatBoard(entities, localPlayer);
		RenderMenu();

        imgui_.end_frame();
        graphics_.present();
    }

} // namespace overlay