// ���[���h�r���[�v���W�F�N�V�����ϊ��s��
float4x4 mWVP : WORLDVIEWPROJECTION;
float4 matDiffuse : MATERIALDIFFUSE;
//���_�V�F�[�_�[�o��
struct VS_OUTPUT
{
    float4 Pos  :   POSITION;
    float4 Col : COLOR0;
};


// ���_�V�F�[�_�[
VS_OUTPUT VS_main(float4 pos : POSITION)
{
    //VS_OUTPUT��������
    VS_OUTPUT Out = (VS_OUTPUT) 0;
    //���W�ϊ�
    Out.Pos = mul(pos, mWVP);
    Out.Col = matDiffuse;
	// �s��Ƀx�N�g�����|�����킹�Ē��_��ϊ�
    return Out;
}

// �s�N�Z���V�F�[�_�[
float4 PS_main(float4 Col : COLOR0) : COLOR0
{

	// ��ɔ�(1, 0, 0, 1)��Ԃ�
    return Col;
}

// �e�N�j�b�N
technique TShader
{
    pass P0
    {
        ZFUNC = LESS;
        CULLMODE = NONE;
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        ALPHABLENDENABLE = FALSE;
        SHADEMODE = FLAT;

        VertexShader = compile vs_2_0 VS_main();
        PixelShader = compile ps_2_0 PS_main();
    }
}