#include "stdafx.h"
#include "Light.h"
#include "World.h"


void LightUtil::SpreadLight(World* world, char lightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight) {
	//弱すぎる光は伝搬しない
	if (lightPower < 1) {
		return;
	}
	for (int i = 0; i < 6; i++) {//6方向に伝搬(前後上下左右)
		//来た方向には戻らない
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//サンプル位置算出
		IntVector3 nowPos = pos + spreadDir[i];

		DW_ERRORBOX((lightPower < 0 || lightPower > LightUtil::LIGHT_POWER_MAX), "明るさレベルが範囲外です")

		//ライト情報記録
		char* light = nullptr;
		if (isSkyLight) {
			light = world->GetSkyLightData(nowPos.x, nowPos.y, nowPos.z);
		}
		else {
			light = world->GetLightData(nowPos.x, nowPos.y, nowPos.z);
		}

		//DW_ERRORBOX(light == nullptr, "です")

		if (light && *light < lightPower) {
			*light = lightPower;
		}
		else {
			//この方向への伝播は終わり(もう明るいから)
			continue;
		}

		//上下左右前後のブロックを照らす
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

		//ブロックをサンプル
		Block* block = world->GetBlock(nowPos.x, nowPos.y, nowPos.z);
		if (block) {//TODO なおかつ不透明ブロック
			//この方向への伝播は終わり(ブロックと衝突したから)		
		}
		else {
			//伝搬を続ける
			SpreadLight(world, lightPower - 1, nowPos, spreadDir[i], isSkyLight);
		}
	}
}

void SkyLight::CalcSkyLight(Chunk* chunk) {
	if (!m_world) {
		DW_ERRORBOX(true,"SkyLight::CalcSkyLight()\n初期化されてないよ...")
		return;
	}

	//上から照らす
	int lightHeight[Chunk::WIDTH][Chunk::WIDTH] = {};
	for (int x = 0; x < Chunk::WIDTH; x++) {
		for (int z = 0; z < Chunk::WIDTH; z++) {

			//初期化
			lightHeight[x][z] = Chunk::HEIGHT;

			for (int y = Chunk::HEIGHT-1; y >= 0; y--) {
				auto light = chunk->GetSkyLightData(x,y,z);
				auto block = chunk->GetBlock(x, y, z);
				if (block) {
					//ブロックに衝突(遮蔽されている)
					*light = 0;
					break;
				}
				else {
					//遮蔽されていない
					*light = LightUtil::LIGHT_POWER_MAX;
					lightHeight[x][z] = y;
				}
			}
		}
	}

	for (int x = 0; x < Chunk::WIDTH; x++) {
		for (int z = 0; z < Chunk::WIDTH; z++) {
			for (int y = Chunk::HEIGHT - 1; y >= lightHeight[x][z]; y--) {
				//上下左右前後のブロックを照らす
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

				//光の伝播
				LightUtil::SpreadLight(m_world, LightUtil::LIGHT_POWER_MAX, { x,y,z }, { 0,0,0 }, true);
			}
		}
	}
}