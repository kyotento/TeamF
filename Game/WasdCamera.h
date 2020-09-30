#pragma once
class WasdCamera : public IGameObject{
public:
	void Update() override;

private:
	CVector3 m_lookVec{0, 0, -1.0f};
};

