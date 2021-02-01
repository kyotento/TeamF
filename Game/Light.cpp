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
						if (sb < 4) {
							block->Lighting(sb, 2, lightPower);
						}
						else {
							block->Lighting(sb - 4, 3, lightPower);
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
	SpreadDarkInner(world, oldLightPower, pos, fromDir, isSkyLight, refleshList, 0);

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
								if (sb < 4) {
									block->SetLightingData(sb, 2, *light);
								}
								else {
									block->SetLightingData(sb - 4, 3, *light);
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
void LightUtil::SpreadDarkInner(World* world, char oldLightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight, std::list<std::unique_ptr<IntVector3>>& refleshList, int nest)
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
			SpreadDarkInner(world, newOldLightPower[i], pos + spreadDir[i], spreadDir[i], isSkyLight, refleshList, nest + 1);
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
				auto light = chunk->GetSkyLightData(x, y, z);
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
				//���[���h�̃u���b�N�P�ʍ��W�Z�o
				IntVector3 worldBlockPos = { chunk->CalcWorldCoordX(x), y, chunk->CalcWorldCoordZ(z) };

				//�㉺���E�O��̃u���b�N���Ƃ炷
				for (int sb = 0; sb < 6; sb++) {
					IntVector3 samplePos = worldBlockPos + LightUtil::spreadDir[sb];
					Block* block = m_world->GetBlock(samplePos);
					if (block) {
						if (sb < 4) {
							block->Lighting(sb, 2, LightUtil::LIGHT_POWER_MAX);
						}
						else {
							block->Lighting(sb - 4, 3, LightUtil::LIGHT_POWER_MAX);
						}
					}
				}

				//���̓`�d
				LightUtil::SpreadLight(m_world, LightUtil::LIGHT_POWER_MAX - 1, worldBlockPos, { 0,0,0 }, true);
			}
		}
	}
}

void SkyLight::CalcSkyLightThisPosition(IntVector3 pos, bool isBlock) {
	//�������Ƀu���b�N�����邩����
	/*for (int y = pos.y + 1; y < Chunk::HEIGHT; y++) {
		IntVector3 sampPos = { pos.x, y, pos.z };
		auto block = m_world->GetBlock(sampPos);
		if (block) {
			return;//����Ȃ�I���
		}
	}*/
	if (pos.y != Chunk::HEIGHT - 1) {//��ԏザ��Ȃ�
		IntVector3 sampPos = { pos.x, pos.y + 1, pos.z };
		auto light = m_world->GetSkyLightData(sampPos);
		if (!light || *light < LightUtil::LIGHT_POWER_MAX) {//���̃��C�g���ő�l�Ȃ�Օ��͂Ȃ�
			return;
		}
	}

	if (isBlock) {//�w��ʒu�Ƀu���b�N���u���ꂽ

		//�Օ�����ĂȂ��Ȃ�u���b�N�ɂԂ���܂Ŏ�����艺���Â�����
		for (int y = pos.y - 1; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetBlockType() != enCube_None) {//TODO �s�����u���b�N
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				*light = 0;
			}
		}

		//���̓`�d
		for (int y = pos.y - 1; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetBlockType() != enCube_None) {//TODO �s�����u���b�N
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				char oldPower = LightUtil::LIGHT_POWER_MAX;
				//���̓`�d
				int count = LightUtil::SpreadDark(m_world, oldPower, sampPos, { 0,0,0 }, true);				
			}
		}



		//���͂���̓`�d�ƕ`��X�V
		for (int y = pos.y - 1; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetBlockType() != enCube_None) {//TODO �s�����u���b�N
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				//TODO ���������ł���?
				for (int sb = 0; sb < 6; sb++) {
					//���肩�����`��������
					auto pos = sampPos + LightUtil::spreadDir[sb];
					//�X�J�C
					auto samplight = m_world->GetSkyLightData(pos);
					if (samplight) {
						LightUtil::SpreadLight(m_world, *samplight - 1, pos, LightUtil::spreadDir[sb] * -1, true);
					}
				}
				//skyLightPower = *skyLight;			

				//�����Ǝ��͂̃��C�e�B���O�`��X�V
				if (block) {
					block->RefleshDrawLighting(m_world, sampPos, -1, *light);
				}
				else {
					//�㉺���E�O��̃u���b�N�̃��C�e�B���O
					for (int sb = 0; sb < 6; sb++) {
						IntVector3 samplePos = sampPos + LightUtil::spreadDir[sb];
						Block* block = m_world->GetBlock(samplePos);
						if (block) {
							if (sb < 4) {
								block->SetLightingData(sb, 2, *light);
							}
							else {
								block->SetLightingData(sb - 4, 3, *light);
							}
						}
					}
				}
			}
		}

	}
	else {//�w��ʒu����u���b�N��������	
		//�Օ�����ĂȂ��Ȃ�u���b�N�ɂԂ���܂Ŏ������牺���Ƃ炷
		for (int y = pos.y; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetBlockType() != enCube_None) {//TODO �s�����u���b�N
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				*light = LightUtil::LIGHT_POWER_MAX;

				//�����Ǝ��͂̃��C�e�B���O�`��X�V
				if (block) {
					block->RefleshDrawLighting(m_world, sampPos, -1, *light);
				}
				else {
					//�㉺���E�O��̃u���b�N�̃��C�e�B���O
					for (int sb = 0; sb < 6; sb++) {
						IntVector3 samplePos = sampPos + LightUtil::spreadDir[sb];
						Block* block = m_world->GetBlock(samplePos);
						if (block) {
							if (sb < 4) {
								block->SetLightingData(sb, 2, *light);
							}
							else {
								block->SetLightingData(sb - 4, 3, *light);
							}
						}
					}
				}

				//���̓`�d
				LightUtil::SpreadLight(m_world, LightUtil::LIGHT_POWER_MAX - 1, sampPos, { 0,0,0 }, true);
			}
		}
	}
}