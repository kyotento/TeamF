#ifndef MC_BLOCK_SHADER_FX
#define MC_BLOCK_SHADER_FX

#include"model.fx"

//���p�����[�^
StructuredBuffer<float4x4> LightingParam : register(t60);

//�u���b�N��GBuffer�ւ̃����_�����O
PSOutput_RenderGBuffer PSMain_McBlockRenderGBuffer(PSInput In)
{
    PSOutput_RenderGBuffer Out = (PSOutput_RenderGBuffer) 0;

    AlbedoRender(In, Out);

	//���e�X�g
    if (Out.albedo.a > 0.5f)
    {
        Out.albedo.a = 1.0f; //����������
    }
    else
    {
        discard; //�`�悵�Ȃ�
    }
    
    NormalRender(In, Out);
    
    //���C�e�B���O
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
    
    //��ʂ͖��邢
    //���ʂ͈Â�
    Out.albedo.rgb *= 1.0f + 0.2f * (Out.normal.y - 1.0f);
    
    //X�����ʂ͂�����ƈÂ�
    Out.albedo.rgb *= 1.0f - 0.4f * abs(Out.normal.x);   

    PosRender(In, Out);

    ParamRender(In, Out);
    Out.lightingParam.y = 0.0f;//���C�e�B���O����!

    MotionRender(In, Out);

    TranslucentRender(In, Out);
    
    return Out;
}

#endif //MC_BLOCK_SHADER_FX