#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>

namespace Offsets
{
    namespace ClientDll
    {
        inline std::uintptr_t DwCSGOInput = 0;
        inline std::uintptr_t DwEntityList = 0;
        inline std::uintptr_t DwGameEntitySystem = 0;
        inline std::uintptr_t DwGameEntitySystemHighestEntityIndex = 0;
        inline std::uintptr_t DwGameRules = 0;
        inline std::uintptr_t DwGlobalVars = 0;
        inline std::uintptr_t DwGlowManager = 0;
        inline std::uintptr_t DwLocalPlayerController = 0;
        inline std::uintptr_t DwLocalPlayerPawn = 0;
        inline std::uintptr_t DwPlantedC4 = 0;
        inline std::uintptr_t DwPrediction = 0;
        inline std::uintptr_t DwSensitivity = 0;
        inline std::uintptr_t DwSensitivitySensitivity = 0;
        inline std::uintptr_t DwViewAngles = 0;
        inline std::uintptr_t DwViewMatrix = 0;
        inline std::uintptr_t DwViewRender = 0;
        inline std::uintptr_t DwWeaponC4 = 0;
    }

    namespace Engine2Dll
    {
        inline std::uintptr_t DwBuildNumber = 0;
        inline std::uintptr_t DwNetworkGameClient = 0;
        inline std::uintptr_t DwNetworkGameClientClientTickCount = 0;
        inline std::uintptr_t DwNetworkGameClientDeltaTick = 0;
        inline std::uintptr_t DwNetworkGameClientIsBackgroundMap = 0;
        inline std::uintptr_t DwNetworkGameClientLocalPlayer = 0;
        inline std::uintptr_t DwNetworkGameClientMaxClients = 0;
        inline std::uintptr_t DwNetworkGameClientServerTickCount = 0;
        inline std::uintptr_t DwNetworkGameClientSignOnState = 0;
        inline std::uintptr_t DwWindowHeight = 0;
        inline std::uintptr_t DwWindowWidth = 0;
    }

    namespace InputSystemDll
    {
        inline std::uintptr_t DwInputSystem = 0;
    }

    namespace MatchmakingDll
    {
        inline std::uintptr_t DwGameTypes = 0;
    }

    namespace SoundSystemDll
    {
        inline std::uintptr_t DwSoundSystem = 0;
        inline std::uintptr_t DwSoundSystemEngineViewData = 0;
    }

    inline bool Load(const std::string& filePath = "offsets.json")
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            return false;

        nlohmann::json json;
        file >> json;

        ClientDll::DwCSGOInput = json["client.dll"]["dwCSGOInput"].get<std::uintptr_t>();
        ClientDll::DwEntityList = json["client.dll"]["dwEntityList"].get<std::uintptr_t>();
        ClientDll::DwGameEntitySystem = json["client.dll"]["dwGameEntitySystem"].get<std::uintptr_t>();
        ClientDll::DwGameEntitySystemHighestEntityIndex = json["client.dll"]["dwGameEntitySystem_highestEntityIndex"].get<std::uintptr_t>();
        ClientDll::DwGameRules = json["client.dll"]["dwGameRules"].get<std::uintptr_t>();
        ClientDll::DwGlobalVars = json["client.dll"]["dwGlobalVars"].get<std::uintptr_t>();
        ClientDll::DwGlowManager = json["client.dll"]["dwGlowManager"].get<std::uintptr_t>();
        ClientDll::DwLocalPlayerController = json["client.dll"]["dwLocalPlayerController"].get<std::uintptr_t>();
        ClientDll::DwLocalPlayerPawn = json["client.dll"]["dwLocalPlayerPawn"].get<std::uintptr_t>();
        ClientDll::DwPlantedC4 = json["client.dll"]["dwPlantedC4"].get<std::uintptr_t>();
        ClientDll::DwPrediction = json["client.dll"]["dwPrediction"].get<std::uintptr_t>();
        ClientDll::DwSensitivity = json["client.dll"]["dwSensitivity"].get<std::uintptr_t>();
        ClientDll::DwSensitivitySensitivity = json["client.dll"]["dwSensitivity_sensitivity"].get<std::uintptr_t>();
        ClientDll::DwViewAngles = json["client.dll"]["dwViewAngles"].get<std::uintptr_t>();
        ClientDll::DwViewMatrix = json["client.dll"]["dwViewMatrix"].get<std::uintptr_t>();
        ClientDll::DwViewRender = json["client.dll"]["dwViewRender"].get<std::uintptr_t>();
        ClientDll::DwWeaponC4 = json["client.dll"]["dwWeaponC4"].get<std::uintptr_t>();

        Engine2Dll::DwBuildNumber = json["engine2.dll"]["dwBuildNumber"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClient = json["engine2.dll"]["dwNetworkGameClient"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClientClientTickCount = json["engine2.dll"]["dwNetworkGameClient_clientTickCount"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClientDeltaTick = json["engine2.dll"]["dwNetworkGameClient_deltaTick"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClientIsBackgroundMap = json["engine2.dll"]["dwNetworkGameClient_isBackgroundMap"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClientLocalPlayer = json["engine2.dll"]["dwNetworkGameClient_localPlayer"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClientMaxClients = json["engine2.dll"]["dwNetworkGameClient_maxClients"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClientServerTickCount = json["engine2.dll"]["dwNetworkGameClient_serverTickCount"].get<std::uintptr_t>();
        Engine2Dll::DwNetworkGameClientSignOnState = json["engine2.dll"]["dwNetworkGameClient_signOnState"].get<std::uintptr_t>();
        Engine2Dll::DwWindowHeight = json["engine2.dll"]["dwWindowHeight"].get<std::uintptr_t>();
        Engine2Dll::DwWindowWidth = json["engine2.dll"]["dwWindowWidth"].get<std::uintptr_t>();

        InputSystemDll::DwInputSystem = json["inputsystem.dll"]["dwInputSystem"].get<std::uintptr_t>();

        MatchmakingDll::DwGameTypes = json["matchmaking.dll"]["dwGameTypes"].get<std::uintptr_t>();

        SoundSystemDll::DwSoundSystem = json["soundsystem.dll"]["dwSoundSystem"].get<std::uintptr_t>();
        SoundSystemDll::DwSoundSystemEngineViewData = json["soundsystem.dll"]["dwSoundSystem_engineViewData"].get<std::uintptr_t>();

        return true;
    }
}