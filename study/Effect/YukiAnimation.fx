// GLOBALS //
#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 26
#endif
int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x3 amPalette[MATRIX_PALETTE_SIZE_DEFAULT]: WORLDMATRIXARRAY;

float4x4		matWorld;			// ���[���h���W�s��
float4x4		matView;			// �r���[�s��
float4x4		matProjection;		// �v���W�F�N�V�����s��
float4x4		matCombinedTrans;	// �����ςݍs��

float4			MaterialDiffuse;	// diffuse
float4			MaterialAmbient;	// ambient
float4			MaterialSpecular;	// specular

texture			Texture;			// �e�N�X�`��
bool			TextureNone = true;	// �e�N�X�`���̗L��(true:���� false:�e�N�X�`���L��)

int				NumBones;			// �{�[����(�e�����󂯂�{�[���̐�)(���[�v���鐔����)

float4			DirectionalLight;	// ������
float4			ViewPosition;		// �J�����̈ʒu

/*�r���t������*/
// �X���C�_�[�p�ϐ�
bool setSlider = false;
float uv_left;
float uv_top;
float uv_width;
float uv_height;
/*�ȏ�*/

// Light //
float4 LightDiffuse = { 1.0, 1.0, 1.0, 0.0 }; // diffuse
float4 LightAmbient = { 1.0, 1.0, 1.0, 0.0 }; // ambient
float4 LightSpecular = { 1.0, 1.0, 1.0, 0.0 }; // specular

// TYPEDEFS //
sampler TextureSampler = sampler_state
{
	Texture = < Texture >; // <���������ɓ���ϐ�>�͏�̕ϐ��Ɠ���
	MipFilter = NONE; // �݂�Ȃ̃A�C�h���~�~���b�v(���^)
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_INPUT_SKIN
{
	float4 Position			: POSITION;		// ���_
	float3 Normal			: NORMAL;		// �@��
	float2 TextureUV		: TEXCOORD0;	// �e�N�X�`��UV�l
	float3 BlendWeights		: BLENDWEIGHT;  // �E�F�C�g
	float4 BlendIndices		: BLENDINDICES; // �C���f�b�N�X
};

struct VS_INPUT_XFILE
{
	float4 Position			: POSITION;		// ���_
	float3 Normal			: NORMAL;		// �@��
	float2 TextureUV		: TEXCOORD0;	// �e�N�X�`��UV�l
};
struct VS_OUTPUT
{
	float4 Position			: POSITION;		// ���_
	float2 TextureUV		: TEXCOORD0;
	float3 DirectionToView	: TEXCOORD1;	// ���_���W�n�ɂ�����J���������x�N�g��
	float3 DirectionToLight	: TEXCOORD2;	// ���_���W�n�ɂ����郉�C�g�����x�N�g��
	float3 Normal			: TEXCOORD3;	// ���_���W�n�ɂ�����@���x�N�g��
	float3 ReflectVector	: TEXCOORD4;	// ���_���W�n�ɂ����锽�˃x�N�g��
};

//
// ������:�X�g���[�� cpp��������Ƃ��Ă�����Ă���
// ������:�V�F�[�_�[���ň���������Ă���
//
VS_OUTPUT VS_SKIN(VS_INPUT_SKIN Input, uniform int NumBones)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	//
	// ���_�Ɩ@���̃u�����h�v�Z
	//
	float3 BlendPosition = 0.0;
	float3 BlendNormal = 0.0;
	float3 Normal = normalize(Input.Normal);
	float LastWeight = 0.0;
	float Weight;

	float afBlendWeights[3] = (float[3]) Input.BlendWeights;
	int aIndices[4] = (int[4]) D3DCOLORtoUBYTE4(Input.BlendIndices);

	for (int Bone = 0; (Bone < 3) && (Bone < NumBones - 1); Bone++)
	{
		LastWeight += afBlendWeights[Bone];
		BlendPosition += mul(Input.Position, amPalette[aIndices[Bone]]) * afBlendWeights[Bone];
		BlendNormal += mul(float4(Normal, 0.0), amPalette[aIndices[Bone]]) * afBlendWeights[Bone];
	}
	LastWeight = 1.0 - LastWeight;
	BlendPosition += mul(Input.Position, amPalette[aIndices[NumBones - 1]]) * LastWeight;
	BlendNormal += mul(float4(Normal, 0.0), amPalette[aIndices[NumBones - 1]]) * LastWeight;
	
	//
	// ���[���h���W�n�ɕϊ�
	//
	float4 WorldPosition = float4(BlendPosition, 1.0);
	WorldPosition = mul(WorldPosition, matWorld);
	
	//
	// �r���[���W�n�ɕϊ�
	//
	Output.Position = mul(WorldPosition, matView);
	// �v���W�F�N�V�������W�n�ɕϊ�
	Output.Position = mul(Output.Position, matProjection);

	// ���_���W�n�ɂ�����@���x�N�g��
	float3 WorldNormal = mul(Input.Normal, (float3x3)matCombinedTrans);
	WorldNormal = mul(WorldNormal, (float3x3)matWorld);
	Output.Normal = normalize(mul(float4(WorldNormal, 0.0), matView).xyz);

	// ���C�g�����x�N�g��
	float4 DirectionToLight = -DirectionalLight;
	// ���_���W�n�ɂ����郉�C�g�����x�N�g��
	Output.DirectionToLight = normalize(mul(DirectionToLight, matView).xyz);

	// �J���������x�N�g��
	float4 DirectionToViewPosition = ViewPosition - WorldPosition;
	// ���_���W�n�ɂ�����J���������x�N�g��
	Output.DirectionToView = normalize(mul(DirectionToViewPosition, matView).xyz);

	/// ���˃x�N�g��
	float3 ReflectVector = -DirectionToViewPosition.xyz + 2.0
		* max(0.0, dot(WorldNormal, DirectionToViewPosition.xyz)) * WorldNormal;

	// �e�N�X�`��UV�l
	Output.TextureUV = Input.TextureUV;
	return Output;
}

VS_OUTPUT VS_XFILE(VS_INPUT_XFILE Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// ���[���h���W�n�ɕϊ�
	float4 WorldPosition = mul(Input.Position, matCombinedTrans);
	WorldPosition = mul(WorldPosition, matWorld);
	// �r���[���W�n�ɕϊ�
	Output.Position = mul(WorldPosition, matView);
	// �v���W�F�N�V�������W�n�ɕϊ�
	Output.Position = mul(Output.Position, matProjection);

	// ���_���W�n�ɂ�����@���x�N�g��
	float3 WorldNormal = mul(Input.Normal, (float3x3)matCombinedTrans);
	WorldNormal = mul(WorldNormal, (float3x3)matWorld);
	Output.Normal = normalize(mul(float4(WorldNormal, 0.0), matView).xyz);

	// ���C�g�����x�N�g��
	float4 DirectionToLight = -DirectionalLight;
	// ���_���W�n�ɂ����郉�C�g�����x�N�g��
	Output.DirectionToLight = normalize(mul(DirectionToLight, matView).xyz);

	// �J���������x�N�g��
	float4 DirectionToViewPosition = ViewPosition - WorldPosition;
	// ���_���W�n�ɂ�����J���������x�N�g��
	Output.DirectionToView = normalize(mul(DirectionToViewPosition, matView).xyz);

	/// ���˃x�N�g��
	float3 ReflectVector = -DirectionToViewPosition.xyz + 2.0
		* max(0.0, dot(WorldNormal, DirectionToViewPosition.xyz)) * WorldNormal;

	// �e�N�X�`��UV�l
	Output.TextureUV = Input.TextureUV;

	return Output;
}

float4 PS_XFILE(VS_OUTPUT Input) : COLOR0
{
	float4 Color;

	// �g�U���ˌ�(�n�[�t�����o�[�g)
	float DiffuseAngle = dot(Input.Normal, Input.DirectionToLight);
	// �n�[�t�����o�[�g�ɕύX
	float HalfLambert = DiffuseAngle * 0.5 + 0.5;
	HalfLambert = clamp(HalfLambert * HalfLambert, 0.0, 1.0);
	float4 Diffuse;
	if (!TextureNone)
		Diffuse = LightDiffuse * MaterialDiffuse * HalfLambert;
	else
		Diffuse = LightDiffuse * MaterialDiffuse * DiffuseAngle;

		// ����
		float4 Ambient = LightAmbient * MaterialAmbient;

		// ���ˌ�
		float3 HalfVector = normalize(Input.DirectionToView + Input.DirectionToLight);
		float3 Specular = LightSpecular.rbg * saturate(pow(max(0.0, dot(HalfVector, Input.Normal)), MaterialSpecular.a) * MaterialSpecular.rbg);

		Color = Diffuse + Ambient + float4(Specular, 0.0);
	if (!TextureNone)
		Color *= tex2D(TextureSampler, Input.TextureUV);
	Color += float4(Specular, 0.0);
	Color.a = MaterialDiffuse.a;
	Color = saturate(Color);

	return Color;
}

VertexShader vsArray[4] =
{
	compile vs_2_0 VS_SKIN(1),
	compile vs_2_0 VS_SKIN(2),
	compile vs_2_0 VS_SKIN(3),
	compile vs_2_0 VS_SKIN(4)
};

technique ShaderTechnique
{
	pass P0
	{
		VertexShader = ( vsArray[ NumBones ]);
		PixelShader = compile ps_2_0 PS_XFILE();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS_XFILE();
		PixelShader = compile ps_2_0 PS_XFILE();
	}
}