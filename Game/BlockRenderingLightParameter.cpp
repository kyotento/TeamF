#include "stdafx.h"
#include "BlockRenderingLightParameter.h"
#include "DemolisherWeapon/Graphic/Model/SkinModelShaderConst.h"

void BlockRenderingLightParameter::Reset(int instancingMaxNum) {
	m_instanceMax = instancingMaxNum;

	//ストラクチャーバッファ初期化
	m_paramsSB.Init(instancingMaxNum);

	//インデックス配列の確保
	m_paramsCache = std::make_unique<CMatrix[]>(instancingMaxNum);
}

BlockRenderingLightParameter::BlockRenderingLightParameter(int instancingMaxNum) {
	Reset(instancingMaxNum);
}

void BlockRenderingLightParameter::PreDraw(int instanceNum, int drawInstanceNum, const std::unique_ptr<bool[]>& drawInstanceMask) {
	//カリングされてないもののみコピー
	int drawNum = 0;
	for (int i = 0; i < instanceNum; i++) {
		if (drawInstanceMask[i]) {
			m_paramsSB.GetData()[drawNum] = m_paramsCache[i];
			drawNum++;
		}
	}
	//StructuredBufferを更新
	m_paramsSB.UpdateSubresource();
	//シェーダーリソースにセット
	GetGraphicsEngine().GetD3DDeviceContext()->PSSetShaderResources(
		enSkinModelSRVReg_Free2, 1, m_paramsSB.GetAddressOfSRV()
	);
}

void BlockRenderingLightParameter::AddDrawInstance(int instanceIndex, const CMatrix& SRTMatrix, const CVector3& scale, void* param) {
	m_paramsCache[instanceIndex] = *(CMatrix*)param;
}

void BlockRenderingLightParameter::SetInstanceMax(int instanceMax) {
	if (instanceMax > m_instanceMax) {
		Reset(instanceMax);
	}
}