#pragma once

class World;

/// <summary>
/// �}�C���N���t�g�p�L�����N�^�[�R���g���[���[
/// </summary>
class MCCharaCon {
public:
	//������
	void Init(float width, float height, const CVector3& position);
	//���s
	const CVector3& Execute(CVector3& moveSpeed, float deltaTime = 1.0f / GetStandardFrameRate());

	//���̃L�����R���̓����蔻���`�悷�邩
	void SetIsDrawCollider(bool b) {
		if (b && GetIsDebugDraw()) {
			m_aabbReender = std::make_unique<GameObj::CAABBRender>();
			UpdateAABBRender();
		}
		else {
			m_aabbReender.reset();
		}
	}
	
	//���W�擾
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	//���W�ݒ�
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}

	//�u���b�N����]���ɉ����o��������ݒ�
	void SetOffset(float offset) {
		m_offset = offset;
	}
	
	//�W�����v����
	bool IsJump() const
	{
		return m_isJump;
	}
	//�n�ʂɂ��邩
	bool IsOnGround() const
	{		
		return m_isOnGround;
	}
	//�ǂɐڐG���Ă��邩�H
	bool IsContactWall() const
	{
		return m_isContactWall;
	}

private:
	//�`��AABB�̍X�V
	void UpdateAABBRender() {
		if (m_aabbReender) {
			CVector3 min = m_position - m_colSize, max = m_position + m_colSize;
			min.y = m_position.y;
			max.y = m_position.y + m_colSize.y;

			m_aabbReender->Init(min, max, {1.0f,0.0f,1.0f,1.0f});
		}
	}

private:
	bool				m_isInited = false;				//�������ς݁H

	CVector3 			m_position = CVector3::Zero();	//���W�B

	bool 				m_isJump = false;				//�W�����v���H
	bool				m_isOnGround = true;			//�n�ʂ̏�ɂ���H
	bool				m_isContactWall = false;		//�ǂɐڐG���Ă�H

	CVector3			m_colSize;						//�R���W�����̑傫��
	float				m_offset = 0.05f;				//�u���b�N����]���ɉ����o������

	std::unique_ptr<GameObj::CAABBRender> m_aabbReender;//AABB��`�悷����

	World* m_world = nullptr;							//���[���h�̃|�C���^
};
