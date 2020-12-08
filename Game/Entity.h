//! @file
#pragma once

class World;
//! @brief ブロックではない、動物や、アイテム、矢などの基底クラス。
//! @author Takayama
class Entity : public IGameObject{
public:
	Entity(World* world);

	virtual ~Entity();

	//! @brief 座標を取得。
	virtual CVector3 GetPos() const = 0;

	//! @brief 座標を設定。
	virtual void SetPos( const CVector3& pos ) = 0;

protected:

	World* m_world = nullptr;

};

