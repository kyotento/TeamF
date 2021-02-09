#pragma once

/// <summary>
/// リスポーン地点をセーブ・ロード
/// </summary>
class RespawnPointFiler
{
public:
	bool Load( CVector3& respawnP);
	void Save(const CVector3& respawnP);
};

