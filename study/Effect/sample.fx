//�O���[�o���ϐ�
float4x4 g_mWorldViewProjection;	//���W�ϊ��s��
texture  g_Texture;			//�e�N�X�`��

//�e�N�X�`���̃T���v�����O���@
sampler MeshTextureSampler =
sampler_state
{
	Texture = <g_Texture>;//�Ώۂ̃e�N�X�`��(�O������󂯎��܂�)
	MinFilter = LINEAR; //�k�����̃T���v�����O(LINEAR�����`�⊮)
	MagFilter = LINEAR; //�g�厞
	MipFilter = NONE;   //�~�b�v�}�b�v

	//�e�N�X�`���A�h���b�V���O���[�h
	AddressU = Clamp;	//�iClanp��0�`1�ȊO�̍��W�̎��ɒ[�����̃s�N�Z�����Ђ��̂΂��j
	AddressV = Clamp;

};

//���_�V�F�[�_�̏o�͏o�͒�`
struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float4 Diffuse : COLOR0;
	float2 TexUV : TEXCOORD0;
};

//���_�V�F�[�_�����i��ɍ��W�ϊ��j
VS_OUTPUT RenderSceneVS(
	float4 Pos : POSITION,
	float4 aDiffuse : COLOR0,
	float2 vTexCoord0 : TEXCOORD0 )
{
	VS_OUTPUT Out;

	Out.Pos = mul(Pos, g_mWorldViewProjection);
	Out.TexUV = vTexCoord0;
	Out.Diffuse = aDiffuse;

	return Out;
}

//�s�N�Z���V�F�[�_�o�͒�`
struct PS_OUTPUT
{
    float4 RGB : COLOR0;
};

//�s�N�Z���V�F�[�_�����i�e�N�X�`���F�ɒ��_�F�������j
PS_OUTPUT RenderScenePS( VS_OUTPUT In )
{
	PS_OUTPUT Out;

	Out.RGB = tex2D( MeshTextureSampler, In.TexUV ) * In.Diffuse;

	return Out;
}

//�s�N�Z���V�F�[�_����(�e�N�X�`���F�ɒ��_�F���������ă��m�N����)
PS_OUTPUT RenderScenePSMono( VS_OUTPUT In )
{
	PS_OUTPUT Out;

	//�e�N�X�`���̃s�N�Z���F�ɒ��_�F�����������F
	float4 color = tex2D( MeshTextureSampler, In.TexUV ) * In.Diffuse;

	//���m�N�����ɂ���Ƃ���RGB�̕��S�����B
	float4 tomono = float4( 0.298912, 0.586611, 0.114478, 0.0 );

	//�e�N�X�`���̃s�N�Z���F�ɒ��_�F����������
	Out.RGB = dot( color, tomono );

	//�A���t�@�l�����͌��̃e�N�X�`���ɖ߂�
	Out.RGB.w = color.w;

	return Out;
}

//�e�N�j�b�N�ƃp�X
//�����̃e�N�j�b�N��p�X���`�����ăV�F�[�_�v���O�����������؂�ւ��邱�Ƃ��ł��܂��B
technique RenderScene
{
	//�J���[�\���̏ꍇ
	pass P0
	{
		VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader = compile ps_2_0 RenderScenePS();
	}
	//���m�N���\���̏ꍇ
	pass P1
	{
		VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader = compile ps_2_0 RenderScenePSMono();
	}
}
