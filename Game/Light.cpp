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
		if (block && block->GetIsOpacity()) {
			//この方向への伝播は終わり(不透明ブロックと衝突したから)		
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
								char power = *light;
								auto lightptr = world->GetSkyLightData(samplePos);
								if (lightptr) {
									power = max(power, *lightptr);
								}
								
								if (sb < 4) {
									block->SetLightingData(sb, 2, power);
								}
								else {
									block->SetLightingData(sb - 4, 3, power);
								}
							}
							else {
								char power = *light;
								auto lightptr = world->GetLightData(samplePos);
								if (lightptr) {
									power = max(power, *lightptr);
								}

								if (sb < 4) {
									block->SetLightingData(sb, 0, power);
								}
								else {
									block->SetLightingData(sb - 4, 1, power);
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
	for (int i = 0; i < 6; i++) {//6方向に伝搬(前後上下左右)
		//来た方向には戻らない
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//サンプル位置算出
		IntVector3 nowPos = pos + spreadDir[i];

		//ブロックのあるところは無視
		Block* block = world->GetBlock(nowPos);
		if (block && block->GetIsOpacity()) {
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
			SpreadDarkInner(world, newOldLightPower[i], pos + spreadDir[i], spreadDir[i], isSkyLight, refleshList, nest + 1);
		}
	}
}

void SkyLight::CalcSkyLight(Chunk* chunk) {
	if (!m_world) {
		DW_ERRORBOX(true,"SkyLight::CalcSkyLight()\n初期化されてないよ...")
		return;
	}

	//スカイライト計算済みフラグ
	chunk->CalcedSkyLight();

	//上から照らす
	int lightHeight[Chunk::WIDTH][Chunk::WIDTH] = {};
	for (int x = 0; x < Chunk::WIDTH; x++) {
		for (int z = 0; z < Chunk::WIDTH; z++) {
			//初期化
			lightHeight[x][z] = Chunk::HEIGHT;
			bool isShadow = false;
			for (int y = Chunk::HEIGHT-1; y >= 0; y--) {
				auto light = chunk->GetSkyLightData(x, y, z);
				DW_ERRORBOX(!light, "スカイライトがない")
				if (isShadow == false) {
					auto block = chunk->GetBlock(x, y, z);
					if (block && block->GetIsOpacity()) {
						DW_ERRORBOX(y == Chunk::HEIGHT - 1, "しとはといは")
						//ブロックに衝突(遮蔽されている)
						*light = 0;
						isShadow = true;
					}
					else {
						//遮蔽されていない
						*light = LightUtil::LIGHT_POWER_MAX;
						lightHeight[x][z] = y;
					}
				}
				else {
					//遮蔽
					*light = 0;
				}
			}
		}
	}

	for (int x = 0; x < Chunk::WIDTH; x++) {
		for (int z = 0; z < Chunk::WIDTH; z++) {
			for (int y = Chunk::HEIGHT - 1; y >= lightHeight[x][z]; y--) {
				//ワールドのブロック単位座標算出
				IntVector3 worldBlockPos = { chunk->CalcWorldCoordX(x), y, chunk->CalcWorldCoordZ(z) };

				//上下左右前後のブロックを照らす
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

				//光の伝播
				LightUtil::SpreadLight(m_world, LightUtil::LIGHT_POWER_MAX - 1, worldBlockPos, { 0,0,0 }, true);
			}
		}
	}

	//周りのチャンクからの伝播
	{
		IntVector3 pos = IntVector3::Zero();
		int x = chunk->GetX();
		int z = chunk->GetZ();

		for (int ix = 0; ix < 2; ix++) {
			if (ix == 0) {
				pos.x = x * Chunk::WIDTH - 1;
			}
			else {
				pos.x = x * Chunk::WIDTH + Chunk::WIDTH;
			}
			for (int iy = 0; iy < Chunk::HEIGHT; iy++) {
				pos.y = iy;
				for (int iz = 0; iz < Chunk::WIDTH; iz++) {
					pos.z = z * Chunk::WIDTH + iz;
					
					char* skylight = m_world->GetSkyLightData(pos);
					if (skylight && *skylight > 1) {
						LightUtil::SpreadLight(m_world, *skylight - 1, pos, IntVector3::Zero(), true);
					}
				}
			}
		}

		for (int ix = 0; ix < 2; ix++) {
			if (ix == 0) {
				pos.z = z * Chunk::WIDTH - 1;
			}
			else {
				pos.z = z * Chunk::WIDTH + Chunk::WIDTH;
			}
			for (int iy = 0; iy < Chunk::HEIGHT; iy++) {
				pos.y = iy;
				for (int iz = 0; iz < Chunk::WIDTH; iz++) {
					pos.x = x * Chunk::WIDTH + iz;
					
					char* skylight = m_world->GetSkyLightData(pos);
					if (skylight && *skylight > 1) {
						LightUtil::SpreadLight(m_world, *skylight - 1, pos, IntVector3::Zero(), true);
					}
				}
			}
		}
	}
}

void SkyLight::CalcSkyLightThisPosition(IntVector3 pos, bool isBlock) {
	//これより上にブロックがあるか検索
	if (pos.y != Chunk::HEIGHT - 1) {//一番上じゃない
		IntVector3 sampPos = { pos.x, pos.y + 1, pos.z };
		auto light = m_world->GetSkyLightData(sampPos);
		if (light && *light < LightUtil::LIGHT_POWER_MAX) {//一個上のライトが最大値なら遮蔽はない
			return;//遮蔽あればリターン
		}
	}

	if (isBlock) {//指定位置にブロックが置かれた

		//遮蔽されてないならブロックにぶつかるまで自分より下を暗くする
		for (int y = pos.y - 1; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetIsOpacity()) {//すでに遮蔽されている
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				*light = 0;
			}
		}

		//負の伝播(上で更新したところの)
		for (int y = pos.y - 1; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetIsOpacity()) {
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				char oldPower = LightUtil::LIGHT_POWER_MAX;
				//負の伝播
				int count = LightUtil::SpreadDark(m_world, oldPower, sampPos, { 0,0,0 }, true);				
			}
		}

		//周囲からの伝播と描画更新
		for (int y = pos.y - 1; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetIsOpacity()) {
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				//TODO いっかいでいい?
				for (int sb = 0; sb < 6; sb++) {
					//周りから光を伝搬させる
					auto pos = sampPos + LightUtil::spreadDir[sb];
					//スカイ
					auto samplight = m_world->GetSkyLightData(pos);
					if (samplight) {
						LightUtil::SpreadLight(m_world, *samplight - 1, pos, LightUtil::spreadDir[sb] * -1, true);
					}
				}
				//skyLightPower = *skyLight;			

				//自分と周囲のライティング描画更新
				if (block) {
					block->RefleshDrawLighting(m_world, sampPos, -1, *light);
				}
				else {
					//上下左右前後のブロックのライティング
					for (int sb = 0; sb < 6; sb++) {
						IntVector3 samplePos = sampPos + LightUtil::spreadDir[sb];
						Block* block = m_world->GetBlock(samplePos);
						if (block) {
							char power = *light;
							auto lightptr = m_world->GetSkyLightData(samplePos);
							if (lightptr) {
								power = max(power, *lightptr);
							}

							if (sb < 4) {
								block->SetLightingData(sb, 2, power);
							}
							else {
								block->SetLightingData(sb - 4, 3, power);
							}
						}
					}
				}
			}
		}

	}
	else {//指定位置からブロックが消えた	

		//遮蔽されてないならブロックにぶつかるまで自分から下を照らす
		for (int y = pos.y; y >= 0; y--) {
			IntVector3 sampPos = { pos.x, y, pos.z };

			auto block = m_world->GetBlock(sampPos);
			if (block && block->GetIsOpacity()) {
				break;
			}

			auto light = m_world->GetSkyLightData(sampPos);
			if (light) {
				*light = LightUtil::LIGHT_POWER_MAX;

				//自分と周囲のライティング描画更新
				if (block) {
					block->RefleshDrawLighting(m_world, sampPos, -1, *light);
				}
				else {
					//上下左右前後のブロックのライティング
					for (int sb = 0; sb < 6; sb++) {
						IntVector3 samplePos = sampPos + LightUtil::spreadDir[sb];
						Block* block = m_world->GetBlock(samplePos);
						if (block) {
							char power = *light;
							auto lightptr = m_world->GetSkyLightData(samplePos);
							if (lightptr) {
								power = max(power, *lightptr);
							}

							if (sb < 4) {
								block->SetLightingData(sb, 2, power);
							}
							else {
								block->SetLightingData(sb - 4, 3, power);
							}
						}
					}
				}

				//光の伝播
				LightUtil::SpreadLight(m_world, LightUtil::LIGHT_POWER_MAX - 1, sampPos, { 0,0,0 }, true);
			}
		}

	}
}
