#include "stdafx.h"
#include "Light.h"
#include "World.h"

void LightUtil::SpreadLight(World* world, char lightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight) {
	//弱すぎる光は伝搬しない
	if (lightPower < 1) {
		return;
	}

	DW_ERRORBOX((lightPower < 0 || lightPower > LightUtil::LIGHT_POWER_MAX), "明るさレベルが範囲外です")

	for (int i = 0; i < 6; i++) {//6方向に伝搬(前後上下左右)
		//来た方向には戻らない
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//サンプル位置算出
		IntVector3 nowPos = pos + spreadDir[i];

		//ブロックをサンプル
		Block* block = world->GetBlock(nowPos);
		if (block && block->GetBlockType() != enCube_None) {//TODO なおかつ不透明ブロック
			//この方向への伝播は終わり(ブロックと衝突したから)		
		}
		else {		

			//サンプル位置のライト情報取得
			char* light = nullptr;
			if (isSkyLight) {
				light = world->GetSkyLightData(nowPos.x, nowPos.y, nowPos.z);
			}
			else {
				light = world->GetLightData(nowPos.x, nowPos.y, nowPos.z);
			}

			//ライト情報更新
			if (light && *light < lightPower) {
				*light = lightPower;
			}
			else {
				//この方向への伝播は終わり(もう明るいから)
				continue;
			}

			//上下左右前後のブロックのライティングを更新
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
		
			//伝搬を続ける
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
				//6方向を調べる
				for (int i2 = 0; i2 < 6; i2++) {
					//サンプリング
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
					//上下左右前後のブロックのライティングを更新
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
	for (int i = 0; i < 6; i++) {//6方向に伝搬(前後上下左右)
		//来た方向には戻らない
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//サンプル位置算出
		IntVector3 nowPos = pos + spreadDir[i];

		//ブロックのあるところは無視
		Block* block = world->GetBlock(nowPos);
		if (block && block->GetBlockType() != enCube_None) {//TODO なおかつ不透明ブロック
			continue;
		}

		//サンプル位置のライト情報取得
		char* light = nullptr;
		if (isSkyLight) {
			light = world->GetSkyLightData(nowPos);
		}
		else {
			light = world->GetLightData(nowPos);
		}

		if (light && oldLightPower > *light && *light > 0){//減衰可能性あり
			refleshList.emplace_back(std::make_unique<IntVector3>(nowPos.x, nowPos.y, nowPos.z));
		}
		else {
			continue;
		}	

		//ライト情報更新
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
	//ブロックのライティングを更新
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