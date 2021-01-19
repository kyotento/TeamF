#include "stdafx.h"
#include "Light.h"
#include "World.h"


void LightUtil::SpreadLight(World* world, char lightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight) {
	//�シ������͓`�����Ȃ�
	if (lightPower < 1) {
		return;
	}
	for (int i = 0; i < 6; i++) {//6�����ɓ`��(�O��㉺���E)
		//���������ɂ͖߂�Ȃ�
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//�T���v���ʒu�Z�o
		IntVector3 nowPos = pos + spreadDir[i];

		DW_ERRORBOX((lightPower < 0 || lightPower > LightUtil::LIGHT_POWER_MAX), "���邳���x�����͈͊O�ł�")

		//���C�g���L�^
		char* light = nullptr;
		if (isSkyLight) {
			light = world->GetSkyLightData(nowPos.x, nowPos.y, nowPos.z);
		}
		else {
			light = world->GetLightData(nowPos.x, nowPos.y, nowPos.z);
		}

		//DW_ERRORBOX(light == nullptr, "�ł�")

		if (light && *light < lightPower) {
			*light = lightPower;
		}
		else {
			//���̕����ւ̓`�d�͏I���(�������邢����)
			continue;
		}

		//�㉺���E�O��̃u���b�N���Ƃ炷
		for (int sb = 0; sb < 6; sb++) {
			IntVector3 samplePos = nowPos + spreadDir[sb];
			Block* block = world->GetBlock(samplePos.x, samplePos.y, samplePos.z);
			if (block) {
				if (isSkyLight) {
					//TODO
					if (sb < 4) {
						block->Lighting(sb, 0, lightPower);
					}
					else {
						block->Lighting(sb - 4, 1, lightPower);
					}
				}
				else {
					if (sb < 4) {
						block->Lighting(sb, 0, lightPower);
					}
					else {
						block->Lighting(sb - 4, 1, lightPower);
					}
				}
			}
		}

		//�u���b�N���T���v��
		Block* block = world->GetBlock(nowPos.x, nowPos.y, nowPos.z);
		if (block) {//TODO �Ȃ����s�����u���b�N
			//���̕����ւ̓`�d�͏I���(�u���b�N�ƏՓ˂�������)		
		}
		else {
			//�`���𑱂���
			SpreadLight(world, lightPower - 1, nowPos, spreadDir[i], isSkyLight);
		}
	}
}

void SkyLight::CalcSkyLight(Chunk* chunk) {
	if (!m_world) {
		DW_ERRORBOX(true,"SkyLight::CalcSkyLight()\n����������ĂȂ���...")
		return;
	}

	//�ォ��Ƃ炷
	int lightHeight[Chunk::WIDTH][Chunk::WIDTH] = {};
	for (int x = 0; x < Chunk::WIDTH; x++) {
		for (int z = 0; z < Chunk::WIDTH; z++) {

			//������
			lightHeight[x][z] = Chunk::HEIGHT;

			for (int y = Chunk::HEIGHT-1; y >= 0; y--) {
				auto light = chunk->GetSkyLightData(x,y,z);
				auto block = chunk->GetBlock(x, y, z);
				if (block) {
					//�u���b�N�ɏՓ�(�Օ�����Ă���)
					*light = 0;
					break;
				}
				else {
					//�Օ�����Ă��Ȃ�
					*light = LightUtil::LIGHT_POWER_MAX;
					lightHeight[x][z] = y;
				}
			}
		}
	}

	for (int x = 0; x < Chunk::WIDTH; x++) {
		for (int z = 0; z < Chunk::WIDTH; z++) {
			for (int y = Chunk::HEIGHT - 1; y >= lightHeight[x][z]; y--) {
				//�㉺���E�O��̃u���b�N���Ƃ炷
				for (int sb = 0; sb < 6; sb++) {
					IntVector3 samplePos = { x,y,z }; samplePos += LightUtil::spreadDir[sb];
					Block* block = m_world->GetBlock(samplePos.x, samplePos.y, samplePos.z);
					if (block) {
						if (sb < 4) {
							block->Lighting(sb, 0, LightUtil::LIGHT_POWER_MAX);
						}
						else {
							block->Lighting(sb - 4, 1, LightUtil::LIGHT_POWER_MAX);
						}
					}
				}

				//���̓`�d
				LightUtil::SpreadLight(m_world, LightUtil::LIGHT_POWER_MAX, { x,y,z }, { 0,0,0 }, true);
			}
		}
	}
}