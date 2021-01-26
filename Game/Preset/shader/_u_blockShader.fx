#ifndef MC_BLOCK_SHADER_FX
#define MC_BLOCK_SHADER_FX

#include"model.fx"

//光パラメータ
StructuredBuffer<float4x4> LightingParam : register(t60);

//ブロックのGBufferへのレンダリング
PSOutput_RenderGBuffer PSMain_McBlockRenderGBuffer(PSInput In)
{
    PSOutput_RenderGBuffer Out = (PSOutput_RenderGBuffer) 0;

    AlbedoRender(In, Out);

	//αテスト
    if (Out.albedo.a > 0.5f)
    {
        Out.albedo.a = 1.0f; //半透明無効
    }
    else
    {
        discard; //描画しない
    }
    
    NormalRender(In, Out);
    
    //ライティング
    float lightScale = 0.0f;
    lightScale += saturate(Out.normal.x * -1.0 * LightingParam[In.instanceID]._m00);
    lightScale += saturate(Out.normal.x * LightingParam[In.instanceID]._m01);
    lightScale += saturate(Out.normal.y * -1.0 * LightingParam[In.instanceID]._m02);
    lightScale += saturate(Out.normal.y * LightingParam[In.instanceID]._m03);
    lightScale += saturate(Out.normal.z * -1.0 * LightingParam[In.instanceID]._m10);
    lightScale += saturate(Out.normal.z * LightingParam[In.instanceID]._m11);
    lightScale *= 2.8f;
    
    float lightScaleSky = 0.0f;
    lightScaleSky += saturate(Out.normal.x * -1.0 * LightingParam[In.instanceID]._m20);
    lightScaleSky += saturate(Out.normal.x * LightingParam[In.instanceID]._m21);
    lightScaleSky += saturate(Out.normal.y * -1.0 * LightingParam[In.instanceID]._m22);
    lightScaleSky += saturate(Out.normal.y * LightingParam[In.instanceID]._m23);
    lightScaleSky += saturate(Out.normal.z * -1.0 * LightingParam[In.instanceID]._m30);
    lightScaleSky += saturate(Out.normal.z * LightingParam[In.instanceID]._m31);
    lightScaleSky *= cb_t;
    
    Out.albedo.rgb *= max(lightScale, lightScaleSky);
    
    //上面は明るい
    //下面は暗い
    Out.albedo.rgb *= 1.0f + 0.2f * (Out.normal.y - 1.0f);
    
    //X方向面はちょっと暗い
    Out.albedo.rgb *= 1.0f - 0.4f * abs(Out.normal.x);   

    PosRender(In, Out);

    ParamRender(In, Out);
    Out.lightingParam.y = 0.0f;//ライティングせず!

    MotionRender(In, Out);

    TranslucentRender(In, Out);
    
    return Out;
}

#endif //MC_BLOCK_SHADER_FX