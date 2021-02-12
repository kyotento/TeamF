#pragma once
#include "Block.h"

class TNT;

class TNTTimer : public IGameObject {
	void Update()override;
	int m_bombCount = 0;
	TNT* m_tnt = nullptr;
	World* m_world = nullptr;

public:
	TNTTimer(TNT* tnt, World* world) : m_tnt(tnt),m_world(world){}
};

class TNT : public Block {
public:
	~TNT() {
		if (m_timer) {
			delete m_timer;
		}
	}
	bool OnClick(Player* player) override;

	void ChangeModelColor(bool isLight);

private:
	bool m_isStared = false;
	TNTTimer* m_timer = nullptr;
};