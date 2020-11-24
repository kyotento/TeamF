#pragma once

class Title : public IGameObject
{
public:
	Title();
	~Title();

	bool Start()override;
	void Update()override;


private:

	CSpriteRender* m_spriteRender = nullptr;

};

