// GLOBALS //
float4x4		matWorld;			// ���[���h���W�s��
float4x4		matView;			// �r���[�s��
float4x4		matProjection;		// �v���W�F�N�V�����s��

float4			MaterialDiffuse;	// diffuse
float4			MaterialAmbient;	// ambient
float4			MaterialSpecular;	// specular

texture			Texture;			// �e�N�X�`��
bool			TextureNone = true;	// �e�N�X�`���̗L��(true:���� false:�e�N�X�`���L��)

float4			DirectionalLight;	// ������
float4			ViewPosition;		// �J�����̈ʒu

// Light //
float4 LightDiffuse  = { 1.0, 1.0, 1.0, 0.0 }; // diffuse
float4 LightAmbient  = { 1.0, 1.0, 1.0, 0.0 }; // ambient
float4 LightSpecular = { 1.0, 1.0, 1.0, 0.0 }; // specular

// TYPEDEFS //
sampler TextureSampler = sampler_state
{
	Texture = < Texture > ; // <���������ɓ���ϐ�>�͏�̕ϐ��Ɠ���
	MipFilter = NONE; // �݂�Ȃ̃A�C�h���~�~���b�v(���^)
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_INPUT
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

VS_OUTPUT VS_XFILE(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// ���[���h���W�n�ɕϊ�
	float4 WorldPosition = mul(Input.Position, matWorld);
	// �r���[���W�n�ɕϊ�
	Output.Position = mul(WorldPosition, matView);
	// �v���W�F�N�V�������W�n�ɕϊ�
	Output.Position = mul(Output.Position, matProjection);

	// ���_���W�n�ɂ�����@���x�N�g��
	float3 WorldNormal = mul(Input.Normal, (float3x3)matWorld);
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

technique ShaderTechnique
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS_XFILE();
		PixelShader  = compile ps_2_0 PS_XFILE();
	}
}