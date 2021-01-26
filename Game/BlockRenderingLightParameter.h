#pragma once

/// <summary>
/// ブロックレンダリングにおける光パラメータを扱う
/// </summary>
class BlockRenderingLightParameter : public GameObj::InstancingModel::IInstancesData {
private:
	//再確保
	void Reset(int instancingMaxNum);

public:
	void PreDraw(int instanceNum, int drawInstanceNum, const std::unique_ptr<bool[]>& drawInstanceMask)override;
	void AddDrawInstance(int instanceIndex, const CMatrix& SRTMatrix, const CVector3& scale, void* param)override;
	void SetInstanceMax(int instanceMax)override;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="instancingMaxNum">インスタンス最大数</param>
	BlockRenderingLightParameter(int instancingMaxNum);

private:
	//パラメータ
	//(ライティングの状態を表すビット列)
	StructuredBuffer<CMatrix>  m_paramsSB;
	std::unique_ptr<CMatrix[]> m_paramsCache;
	//インスタンス数
	int m_instanceMax = 0;
};