#pragma once

class ItemImage{
public:
	ItemImage(bool isModel, const std::filesystem::path& filepath );

	void Draw(const CVector2& pos, const CVector2& parentScale );

private:
	static float constexpr st_initialScale = 0.24f;
	const CVector2 m_pivot{0.5f, 0.5f};
	CSprite m_sprite;
};