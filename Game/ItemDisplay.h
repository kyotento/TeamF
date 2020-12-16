#pragma once
#include "Inventory.h"

/// <summary>
/// アイテムディスプレイクラス
/// </summary>
/// アイテムを表示するクラスですよねこれ
/// 
class Player;
class GameCamera;
class ItemDisplay : public IGameObject
{
public:
	ItemDisplay();
	~ItemDisplay();
	bool Start()override;
	void Update()override;
	/// <summary>
	/// プレイヤーから座標をもらう。
	/// </summary>
	/// <param name="selItem">プレイヤーのポジション。</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// プレイヤーから方向ももらうじぇ。
	/// </summary>
	/// <param name="selItem">プレイヤーの回転。</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 頭に合わせて上下させるよう
	/// <summary>
	void SetHeadRot(const CQuaternion& h_rot)
	{
		m_headRot = h_rot;
	}
	/// <summary>
	/// インベントリーの番号取得。
	/// </summary>
	void SetSelectNum(int num)
	{
		m_playerSelectNum = num;
	}
	/// <summary>
	/// アイテムが切り替わりを設定するよ。
	/// </summary>
	void SetChangeItemFlag(bool flag)
	{
		m_isItemChangeFlag = flag;
	}
	/// <summary>
	/// 切り替え終わった事を伝える。
	/// </summary>
	/// <returns></returns>
	bool GetEndChangeFlag()
	{
		return m_isItemChangeFlag;
	}
private:

	enum EnType
	{
		enHand,
		enBlock,
		enTool
	};

	/// <summary>
	/// モデルの生成。
	/// </summary>
	void InitModel();
	/// <summary>
	/// 追従
	/// </summary>
	void Follow();
	/// モデルの切り替え
	/// </summary>
	void Switching();
	/// <summary>
	/// アイテムを斜め上に表示するために。
	/// </summary>
	void Rotation();
	/// <summary>
	/// カメラのモードに合わせて表示させるための関数。
	/// </summary>
	void CameraModeChangeToDisplay();
	/// <summary>
	/// 切り替えたときに再度生成。
	/// </summary>
	void BuildAgain();
	/// <summary>
	/// 手の回転処理
	/// </summary>
	void HandRotation();
	/// <summary>
	/// ブロック系の回転処理
	/// </summary>
	void BlockRotation();
	/// <summary>
	/// ツール系の回転処理
	/// </summary>
	void ToolRotation();
	/// <summary>
	/// アイテムをインベントリに合わせて切り替える。
	/// </summary>
	void SwitchItemType();
private:
	GameObj::CSkinModelRender* m_skinModelRender = nullptr;	//モデル。
	CVector3 m_position = CVector3::Zero();					//モデルを表示する座標。
	CVector3 m_forward = CVector3::Zero();					//前方。
	CVector3 m_scale = CVector3::One();						//大きさ。

	CQuaternion m_rotation = CQuaternion::Identity();		//モデルを回転させる。
	CQuaternion m_headRot = CQuaternion::Identity();

	float m_mullFornt = 25.0f;								//アイテムを前後させる変数。
	float m_mullCrossProduct = 45.0f;							//アイテムの左右移動させるの変数。
	float m_radianY = 0.0f;									//Y軸の回転(ラジアン)
	float m_radianXZ = 0.0f;								//XZ軸の回転(ラジアン)

	int m_cameraDisplayMode = 0;							//カメラのモード。
	int m_playerSelectNum = 0;								//プレイヤーが持っている番号

	bool m_isItemChangeFlag = false;						//持ってるもの切り替えたかどうか。

	Player* m_player = nullptr;								//プレイヤーのインスタンス。
	GameCamera* m_gameCamera = nullptr;						//ゲームカメライェア。

	EnType type;											//タイプ
};

