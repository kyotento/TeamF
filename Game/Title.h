#pragma once

class TitleSelect;
class Title : public IGameObject
{
public:
	Title();
	~Title();

	bool Start()override;
	void Update()override;


private:

	GameObj::CSpriteRender* m_spriteRender = nullptr;

//	TitleSelect* m_titleSelect = nullptr;		//タイトルのボタン。
	std::unique_ptr<TitleSelect> m_titleSelect2;
};

