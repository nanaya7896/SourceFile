float4x4	matWorld;				// ���[���h���W
float4x4	matWorldInverse;		// ���[���h���W�̋t�s��
float4x4	matVP;					// �ˉe��Ԃւ̍��W�ϊ�
float4x4	matLVP;					// ���C�g���猩���ˉe��Ԃւ̍��W�ϊ�
texture		Texture;				// �e�N�X�`���[
texture		ShadowBuffer;			// �V���h�E�o�b�t�@�[
bool		TextureNone = true;		// �e�N�X�`���[�̗L��(true:����)	
int			ObjectNumber;			// �I�u�W�F�N�g�̔ԍ�

float4		MaterialDiffuse;		// �F

// ���̋���
float4 Light_Diffuse = { 1.0f,1.0f,1.0f,0.0f };	// diffuse
float4 Light_Ambient = { 1.0f,1.0f,1.0f,0.0f };	// ambient
float4 Light_Specular= { 0.4f,0.4f,0.4f,0.0f }; // specular
// �������C�e�B���O
float4 SkyBlue	= { 0.53f,0.81f,1.0f,0.0f };
float4 Sienna	= { 0.63f,0.32f,0.18f,0.0f };
float4		DirectionalLight;		// ������
float4		ViewFrom;				// �J�����̈ʒu

sampler TextureSampler =
sampler_state
{
	Texture = <Texture>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler ShadowBufferSampler = 
sampler_state
{
	Texture = <ShadowBuffer>;
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = BORDER;
	AddressV = BORDER;
};

struct VS_INPUT_XFILE
{
	float4	Position	:	POSITION;	// ���_�̈ʒu
	float3  Normal		:	NORMAL;		// ���_�̖@��
	float2	TextureUV	:	TEXCOORD0;	// �e�N�X�`���[UV�l
};

struct VS_OUTPUT_XFILE
{
	float4	Position	:	POSITION;	// ���_�̈ʒu
	float4	Color		:	COLOR0;		// ���_�̐F
	float4	Specular	:	COLOR1;		// �X�y�L�����[�F
	float2	TextureUV	:	TEXCOORD0;	// �e�N�X�`���[UV�l
	float4	Light		:	TEXCOORD1;	// ���C�g�̈ʒu
};

struct SHADOWMAP_VS_OUTPUT
{
	float4	Position	:	POSITION;	// ���_�̈ʒu
	float2	Depth		:	TEXCOORD0;	// ���C�g����̋���
};

VS_OUTPUT_XFILE VS_XFILE( VS_INPUT_XFILE Input )
{
    VS_OUTPUT_XFILE Output = (VS_OUTPUT_XFILE)0;
	
	// ���[���h���W�n
	float4 WorldPosition = mul(Input.Position,matWorld);
	float4 WorldNormal = float4(Input.Normal,0.0);
    // ���_���W�ϊ�
	Output.Position = mul(WorldPosition,matVP);

	// �e�N�X�`���[UV�l���o��
	Output.TextureUV = Input.TextureUV;

	// ���_�̐F�̌���
	// ���C�g�����x�N�g��
	float4 DirectionToLight = normalize(mul(normalize(-DirectionalLight),matWorldInverse));
	// �J���������x�N�g��
	float4 DirectionToViewFrom = normalize(ViewFrom - WorldPosition);

	float4 Diffuse = Light_Diffuse * MaterialDiffuse * max(0,dot(
		DirectionToLight.xyz,WorldNormal.xyz));		 // �g�U���ˌ�
	float4 Ambient = Light_Ambient * MaterialDiffuse;// ����
	Output.Specular = Light_Specular * pow(max(0,dot(DirectionToLight.xyz,normalize(WorldNormal.xyz))),10);
	float HemiSphereValue = (dot(normalize(WorldNormal), float4(0.0f,1.0f,0.0f,0.0f)) + 1.0f) * 0.5f;
	float4 HemiSphere = lerp( Sienna, SkyBlue, HemiSphereValue ) * 0.2f;

	Output.Color = saturate(Diffuse + Ambient + HemiSphere);

	Output.Color.a = 1.0f;

	// ��������݂����_�̈ʒu�̌v�Z
	Output.Light = mul(mul(Input.Position,matWorld),matLVP);

    return Output;
}

float4 PS_XFILE( VS_OUTPUT_XFILE Input ) : COLOR0
{
    float4 Color;

    if (TextureNone)
    {
        Color = Input.Color + Input.Specular;
    }
    else
    {
        Color = Input.Color * tex2D(TextureSampler, Input.TextureUV) + Input.Specular;
    }

	// �V���h�E�}�b�v����l�̎擾
	float2 ShadowUV = 0.5f * Input.Light.xy 
				/ Input.Light.w + float2(0.5f,0.5f);
	ShadowUV.y = 1.0f - ShadowUV.y;
	ShadowUV.x = ShadowUV.x + 0.5f / 1280.0f;
	ShadowUV.y = ShadowUV.y + 0.5f / 720.0f;

	float ShadowValue = tex2D(ShadowBufferSampler, ShadowUV).r;

	// �������璸�_�܂ł̋������v�Z
	float Depth = Input.Light.z / Input.Light.w;

	// �V���h�E�}�b�v�̐[�x�̒l�Ɣ�r
	// �e�ɂȂ��Ă���
	if(Depth - ShadowValue > 0.000065f)
	{
		// �e�ɂȂ��Ă���
		if (ObjectNumber == int(tex2D(ShadowBufferSampler, ShadowUV).g) && int(tex2D(ShadowBufferSampler, ShadowUV).g) != 0)
		{
			Color = Color * 0.95f;
			Color.a = 1.0f;
		}
		else
		{
			if (int(tex2D(ShadowBufferSampler, ShadowUV).g) != 0)
			{
				Color = Color * 0.5f;
				Color.a = 1.0f;
			}
		}
	}
    Color.a = 1.0f;
    return Color;
}

float4 PS_XFILE2( VS_OUTPUT_XFILE Input ) : COLOR0
{
    float4 Color;

    if (TextureNone)
    {
        Color = Input.Color + Input.Specular;
    }
    else
    {
        Color = Input.Color * tex2D(TextureSampler, Input.TextureUV) + Input.Specular;
    }

	// �V���h�E�}�b�v����l�̎擾
	float2 ShadowUV = 0.5f * Input.Light.xy 
				/ Input.Light.w + float2(0.5f,0.5f);
	ShadowUV.y = 1.0f - ShadowUV.y;
	ShadowUV.x = ShadowUV.x + 0.5f / 1280.0f;
	ShadowUV.y = ShadowUV.y + 0.5f / 720.0f;

	float ShadowValue = tex2D(ShadowBufferSampler, ShadowUV).r;

	// �������璸�_�܂ł̋������v�Z
	float Depth = Input.Light.z / Input.Light.w;

	// �V���h�E�}�b�v�̐[�x�̒l�Ɣ�r
	// �e�ɂȂ��Ă���
	if(Depth - ShadowValue > 0.000065f)
	{
		// �e�ɂȂ��Ă���
		if (ObjectNumber == int(tex2D(ShadowBufferSampler, ShadowUV).g) && int(tex2D(ShadowBufferSampler, ShadowUV).g) != 0)
		{
			Color = Color * 0.95f;
			Color.a = 1.0f;
		}
		else
		{
			if (int(tex2D(ShadowBufferSampler, ShadowUV).g) != 0)
			{
				Color = Color * 0.5f;
				Color.a = 1.0f;
			}
		}
	}
	//Color = float4(Color.xyz+Light_Ambient.xyz * Light_Ambient.w, //Color.w);
	Color.a=0.7f;
    return Color;
}

// ----- �V���h�E�}�b�v�֘A -----
SHADOWMAP_VS_OUTPUT ShadowMapVS(SHADOWMAP_VS_OUTPUT Input)
{
	SHADOWMAP_VS_OUTPUT Output = (SHADOWMAP_VS_OUTPUT)0;	// �o�̓f�[�^

	// ���W�ϊ�
	float4 WorldPosition = mul(Input.Position,matWorld);
	Output.Position = mul(Output.Position,matLVP);
	// �ʒu���̏o��
	Output.Depth.xy = Output.Position.zw;

    return Output;
}

float4 ShadowMapPS(SHADOWMAP_VS_OUTPUT Input) :	COLOR
{
	// �s�N�Z���̐[�x�����v�Z���ďo��(0�`1�ɐ��K��)
	float4 Color = float4(0,0,0,0);
	Color.r = Input.Depth.x / Input.Depth.y;
	Color.g = float(ObjectNumber);

	return Color;
}

technique ShaderTechnique
{
	pass P0
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;
		CULLMODE=NONE;
		VertexShader = compile vs_2_0 VS_XFILE();
		pixelShader  = compile ps_2_0 PS_XFILE();
	}
	pass P1
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		VertexShader = compile vs_2_0 ShadowMapVS();
		pixelShader  = compile ps_2_0 ShadowMapPS();
	}
	pass P2
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = TRUE;
		
		VertexShader = compile vs_2_0 VS_XFILE();
		pixelShader  = compile ps_2_0 PS_XFILE2();	
	}
}

