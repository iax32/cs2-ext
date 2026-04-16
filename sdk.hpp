#pragma once
#include "windows_api.hpp"
#include "offsets.hpp"

constexpr int BONE_STRIDE = 32;
constexpr int MAX_INDEX = 27;
constexpr size_t BONE_COUNT = size_t(MAX_INDEX) + 1;
struct BoneConnection {
	int a, b;
};
constexpr std::array<BoneConnection, 16> kBoneConnections{ {{6, 5}, {5, 4}, {4, 0}, {4, 8}, {8, 9}, {9, 11}, {4, 13}, {13, 14}, {14, 16}, {4, 2}, {0, 22}, {0, 25}, {22, 23}, {23, 24}, {25, 26}, {26, 27}} };

const std::uintptr_t GetPawnBase(std::uintptr_t pPlayerController, std::uintptr_t pEntityList)
{
	std::uintptr_t pawnHandle = WinApi::Read<uintptr_t>(pPlayerController + 0x90C);
	std::uintptr_t listentry2 = WinApi::Read<uintptr_t>(
		pEntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

	return WinApi::Read<uintptr_t>(listentry2 + 0x70 * (pawnHandle & 0x1FF));
}

class Entity
{
private:
	std::uintptr_t m_pBase;
	std::uintptr_t m_pController;
	std::uint32_t m_iHealth;
	std::uint32_t m_iArmor;
	std::uint32_t m_iTeam;
	char m_szName[32];


	Vector3 m_vecOrigin;
	Vector2 m_vecOriginPx;

	Vector3 m_vecHead;
	Vector2 m_vecHeadPx;

	std::uintptr_t m_pGameSceneNode;
	std::uintptr_t m_pSceneNode;

	ImVec2 m_feet2D;
	ImVec2 m_head2D;

	std::array<Vector3, BONE_COUNT> m_vecBones{ };
	std::array<Vector2, BONE_COUNT> M_vecBones2d{ };
	std::array<uint8_t, BONE_COUNT> m_vecVisibleBones{ };

public:
	void Update()
	{
		m_iHealth = WinApi::Read<std::uint32_t>(m_pBase + 0x354);
		m_iArmor = WinApi::Read<std::uint32_t>(m_pBase + 0x358);
		WinApi::ReadString(m_pController + 0x6F8, m_szName, sizeof(m_szName));
		m_iTeam = WinApi::Read<std::uint32_t>(m_pBase + 0x3F3);
		m_vecOrigin = WinApi::Read<Vector3>(m_pBase + 0x1588);

		m_pGameSceneNode = WinApi::Read<std::uintptr_t>(m_pBase + 0x338);
		m_pSceneNode = WinApi::Read<std::uintptr_t>(m_pGameSceneNode + 0x160 + 0x80);
		m_vecHead = WinApi::Read<Vector3>(m_pSceneNode + (6 * 32));
	}

	std::string GetName() const
	{
		return std::string(m_szName);
	}

	Vector2 GetHeadPosPx() const
	{
		return m_vecHeadPx;
	}

	Vector2 GetFeetPosPx() const
	{
		return m_vecOriginPx;
	}

	std::array<Vector3, BONE_COUNT> GetBones() const
	{
		return m_vecBones;
	}

	std::array<Vector2, BONE_COUNT> GetBones2D() const
	{
		return M_vecBones2d;
	}

	std::array<uint8_t, BONE_COUNT> GetVisibleBones() const
	{
		return m_vecVisibleBones;
	}

	void ReadBones(Matrix matrix)
	{
		std::array<std::uint8_t, BONE_STRIDE* BONE_COUNT> raw{ };
		WinApi::ReadInto(m_pSceneNode, raw.data(), raw.size());


		for (size_t bi = 0; bi < BONE_COUNT; ++bi) {
			std::memcpy(&m_vecBones[bi], raw.data() + bi * BONE_STRIDE, sizeof(Vector3));
		}
	}

	BOOL Render(Matrix matrix, int window_width, int window_height)
	{
		if (!WorldToScreen(m_vecOrigin, m_vecOriginPx, matrix.mvpMatrix))
			return FALSE;
		if (!WorldToScreen(m_vecHead, m_vecHeadPx, matrix.mvpMatrix))
			return FALSE;


		for (size_t bi = 0; bi < BONE_COUNT; ++bi) {
			m_vecVisibleBones[bi] = WorldToScreen(m_vecBones[bi], M_vecBones2d[bi], matrix.mvpMatrix);
		}

		m_feet2D = NDCToScreen(m_vecOriginPx, window_width, window_height);
		m_head2D = NDCToScreen(m_vecHeadPx, window_width, window_height);

		return TRUE;
	}

	void SetBase(std::uintptr_t pBase)
	{
		m_pBase = pBase;
	}

	void SetController(std::uintptr_t pController)
	{
		m_pController = pController;
	}

	std::uintptr_t GetBase() const
	{
		return m_pBase;
	}

	std::uintptr_t GetController() const
	{
		return m_pController;
	}

	std::uint32_t GetHealth() const
	{
		return m_iHealth;
	}

	std::uint32_t GetArmor() const
	{
		return m_iArmor;
	}

	std::uint32_t GetTeam() const
	{
		return m_iTeam;
	}


};

