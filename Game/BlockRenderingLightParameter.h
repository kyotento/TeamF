#pragma once

/// <summary>
/// �u���b�N�����_�����O�ɂ�������p�����[�^������
/// </summary>
class BlockRenderingLightParameter : public GameObj::InstancingModel::IInstancesData {
private:
	//�Ċm��
	void Reset(int instancingMaxNum);

public:
	void PreDraw(int instanceNum, int drawInstanceNum, const std::unique_ptr<bool[]>& drawInstanceMask)override;
	void AddDrawInstance(int instanceIndex, const CMatrix& SRTMatrix, const CVector3& scale, void* param)override;
	void SetInstanceMax(int instanceMax)override;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="instancingMaxNum">�C���X�^���X�ő吔</param>
	BlockRenderingLightParameter(int instancingMaxNum);

private:
	//�p�����[�^
	//(���C�e�B���O�̏�Ԃ�\���r�b�g��)
	StructuredBuffer<CMatrix>  m_paramsSB;
	std::unique_ptr<CMatrix[]> m_paramsCache;
	//�C���X�^���X��
	int m_instanceMax = 0;
};