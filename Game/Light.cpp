#include "stdafx.h"
#include "Light.h"
#include "World.h"

void LightUtil::SpreadLight(World* world, char lightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight) {
	//�シ������͓`�����Ȃ�
	if (lightPower < 1) {
		return;
	}

	DW_ERRORBOX((lightPower < 0 || lightPower > LightUtil::LIGHT_POWER_MAX), "���邳���x�����͈͊O�ł�")

	for (int i = 0; i < 6; i++) {//6�����ɓ`��(�O��㉺���E)
		//���������ɂ͖߂�Ȃ�
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//�T���v���ʒu�Z�o
		IntVector3 nowPos = pos + spreadDir[i];

		//�u���b�N���T���v��
		Block* block = world->GetBlock(nowPos);
		if (block && block->GetBlockType() != enCube_None) {//TODO �Ȃ����s�����u���b�N
			//���̕����ւ̓`�d�͏I���(�u���b�N�ƏՓ˂�������)		
		}
		else {		

			//�T���v���ʒu�̃��C�g���擾
			char* light = nullptr;
			if (isSkyLight) {
				light = world->GetSkyLightData(nowPos.x, nowPos.y, nowPos.z);
			}
			else {
				light = world->GetLightData(nowPos.x, nowPos.y, nowPos.z);
			}

			//���C�g���X�V
			if (light && *light < lightPower) {
				*light = lightPower;
			}
			else {
				//���̕����ւ̓`�d�͏I���(�������邢����)
				continue;
			}

			//�㉺���E�O��̃u���b�N�̃��C�e�B���O���X�V
			for (int sb = 0; sb < 6; sb++) {
				IntVector3 samplePos = nowPos + spreadDir[sb];
				Block* block = world->GetBlock(samplePos);
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
		
			//�`���𑱂���
			SpreadLight(world, lightPower - 1, nowPos, spreadDir[i], isSkyLight);
		}
	}
}

int LightUtil::SpreadDark(World* world, char oldLightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight)
{
	if (oldLightPower <= 0) {
		return 0;
	}
	std::list<std::unique_ptr<IntVector3>> refleshList;
	SpreadDarkInner(world, oldLightPower, pos, fromDir, isSkyLight, refleshList);

	int count = 0;
	bool reflesh = true;
	while(reflesh){
		reflesh = false;
		count++;
		for (auto& ppos : refleshList) {
			IntVector3 pos = *(ppos.get());

			char* light = nullptr;
			if (isSkyLight) {
				light = world->GetSkyLightData(pos);
			}
			else {
				light = world->GetLightData(pos);
			}

			if (light) {
				char maxlight = 0;
				//6�����𒲂ׂ�
				for (int i2 = 0; i2 < 6; i2++) {
					//�T���v�����O
					IntVector3 sampPos = pos + spreadDir[i2];
					char* sampLight = nullptr;
					if (isSkyLight) {
						sampLight = world->GetSkyLightData(sampPos);
					}
					else {
						sampLight = world->GetLightData(sampPos);
					}
					if (sampLight) {
						maxlight = max(maxlight, *sampLight - 1);
					}
				}
			
				if (*light != maxlight) {
					reflesh = true;
				}
				//if (count == 1 || *light != maxlight) {
					//�㉺���E�O��̃u���b�N�̃��C�e�B���O���X�V
					for (int sb = 0; sb < 6; sb++) {
						IntVector3 samplePos = pos + spreadDir[sb];
						Block* block = world->GetBlock(samplePos);
						if (block) {
							if (isSkyLight) {
								//TODO
								if (sb < 4) {
									block->SetLightingData(sb, 0, *light);
								}
								else {
									block->SetLightingData(sb - 4, 1, *light);
								}
							}
							else {
								if (sb < 4) {
									block->SetLightingData(sb, 0, *light);
								}
								else {
									block->SetLightingData(sb - 4, 1, *light);
								}
							}
						}
					}
				//}
				*light = maxlight;
			}
		}
	}
	return count;
}
void LightUtil::SpreadDarkInner(World* world, char oldLightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight, std::list<std::unique_ptr<IntVector3>>& refleshList)
{
	char newOldLightPower[6] = { -1,-1 ,-1 ,-1 ,-1 ,-1 };
	for (int i = 0; i < 6; i++) {//6�����ɓ`��(�O��㉺���E)
		//���������ɂ͖߂�Ȃ�
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//�T���v���ʒu�Z�o
		IntVector3 nowPos = pos + spreadDir[i];

		//�u���b�N�̂���Ƃ���͖���
		Block* block = world->GetBlock(nowPos);
		if (block && block->GetBlockType() != enCube_None) {//TODO �Ȃ����s�����u���b�N
			continue;
		}

		//�T���v���ʒu�̃��C�g���擾
		char* light = nullptr;
		if (isSkyLight) {
			light = world->GetSkyLightData(nowPos);
		}
		else {
			light = world->GetLightData(nowPos);
		}

		if (light && oldLightPower > *light && *light > 0){//�����\������
			refleshList.emplace_back(std::make_unique<IntVector3>(nowPos.x, nowPos.y, nowPos.z));
		}
		else {
			continue;
		}	

		//���C�g���X�V
		newOldLightPower[i] = *light;
		*light = 0;
	}
	for (int i = 0; i < 6; i++) {
		if (newOldLightPower[i] > 0) {			
			SpreadDarkInner(world, newOldLightPower[i], pos + spreadDir[i], spreadDir[i], isSkyLight, refleshList);
		}
	}
}

void LightUtil::RefleshLight(World* world, const IntVector3& pos) {
	//�u���b�N�̃��C�e�B���O���X�V
	Block* block = world->GetBlock(pos);
	if (block) {
		for (int sb = 0; sb < 6; sb++) {
			IntVector3 samplePos = pos + spreadDir[sb]*-1;
			char* light = world->GetLightData(samplePos);
			//if (isSkyLight) {
			//	//TODO
			//	if (sb < 4) {
			//		block->SetLightingData(sb, 0, *light);
			//	}
			//	else {
			//		block->SetLightingData(sb - 4, 1, *light);
			//	}
			//}
			//else {
				if (sb < 4) {
					block->SetLightingData(sb, 0, *light);
				}
				else {
					block->SetLightingData(sb - 4, 1, *light);
				}
			//}
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