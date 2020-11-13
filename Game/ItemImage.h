#pragma once

class ItemImage{
public:
	virtual ~ItemImage(){}

	virtual void Draw(const CVector2& pos, const CVector2& parentScale ) = 0;

protected:
	const CVector2 m_pivot{0.5f, 0.5f};
};

class ItemModelImage : public ItemImage{
public:
	ItemModelImage( const wchar_t* modelpath );

	void Draw( const CVector2& pos, const CVector2& parentScale ) override;

private:
	static float constexpr st_initialScale = 1 / 100.0f;
	SkinModel m_model;
};

class ItemSpriteImage : public ItemImage{
public:
	ItemSpriteImage( const wchar_t* spritePath );

	void Draw( const CVector2& pos, const CVector2& parentScale ) override;

private:
	static float constexpr st_initialScale = 0.24f;
	CSprite m_sprite;
};