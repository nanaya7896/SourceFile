float4x4 matWorld : WORLD; // ���[���h���W
float4x4 matWorldInverse; // ���[���h���W�̋t�s��
float4x4 matVP; // �ˉe��Ԃւ̍��W�ϊ�
float4x4 matLVP; // ���C�g���猩���ˉe��Ԃւ̍��W�ϊ�
bool titleLight;
float4 LightPosition;//=float4(-500000.0f, -5000.0f, 0.0f, 0.0f); // �������W

texture Texture; // �e�N�X�`���[
//�}�e���A���F
float4 MaterialDiffuse : MATERIALDIFFUSE;
float4 MaterialAmbient : MATERIALAMBIENT;
float4 ambient : AMBIENT;

//:LIGHTDIRECTION;

sampler TextureSample = sampler_state
{
    Texture = <Texture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//���_���͍\����
struct VS_INPUT
{
    //���W
    float4 Pos : POSITION;
    //�@��
    float3 Normal : NORMAL;
    //UV���W
    float2 TexUV : TEXCOORD0;    
};

//���_�o�͍\����
struct VS_OUTPUT
{
    //���_
    float4 Pos : POSITION;
    //
    float4 Col : COLOR0;
    //UV���W
    float2 TexUV : TEXCOORD0;
    //�@��
    float3 Normal : TEXCOORD1;
    //������
    float3 Light : TEXCOORD2;
};

//�������@�����v�Z���A���C�e�B���O�����߂�
float CalcLightingDot(float4 nor)
{
    //�@�������[���h���W�ɕϊ����A���s�ړ����Ƃ̍������߂�
    //���K���A���C�g�Ƃ̖@���̓��ς����߂�
    float3 N = mul(nor, matWorld).xyz;
    float3 BaseN = mul(float4(0, 0, 0, 1), matWorld).xyz;
    N = normalize(N - BaseN);
    return dot(-LightPosition.xyz, N);
}

//���_���̐ݒ�
VS_OUTPUT VS_VERTEX(VS_INPUT In)
{
    //VS_OUTPUT�̏�����
    VS_OUTPUT Out = (VS_OUTPUT) 0;

    //
    //float4 LightDirection = LightPosition;
    // ���[���h���W�n�ւ̕ϊ�
    float4 WorldPosition = mul(In.Pos, matWorld);
    //�@��
    float4 WorldNormal = float4(In.Normal,0.0);
    // ���_���W�ϊ�
    Out.Pos = mul(WorldPosition, matVP);

    //Out.Pos = mul(In.Pos, Out.Pos);
    //�@���̌v�Z
    Out.Normal = mul(float4(In.Normal,0), matWorld).xyz;
    //���[���h���W�ƃ��C�g�̈ʒu�̍���
    Out.Light = LightPosition.xyz-WorldPosition.xyz;
    //UV���W
    Out.TexUV = In.TexUV;
    //�����o�[�g����
    Out.Col.rgb = MaterialDiffuse.rgb * CalcLightingDot(float4(In.Normal,0))+ MaterialAmbient.rgb * ambient.rgb;
   // Out.Col.a = MaterialDiffuse.a;
	
    return Out;
}

//���_���̐ݒ�
VS_OUTPUT VS_VERTEXLIGHT(VS_INPUT In)
{
    //VS_OUTPUT�̏�����
    VS_OUTPUT Out = (VS_OUTPUT) 0;

    //
    //float4 LightDirection = LightPosition;
    // ���[���h���W�n�ւ̕ϊ�
    float4 WorldPosition = mul(In.Pos, matWorld);
    //�@��
    float4 WorldNormal = float4(In.Normal, 0.0);
    // ���_���W�ϊ�
    Out.Pos = mul(WorldPosition, matVP);

    //Out.Pos = mul(In.Pos, Out.Pos);
    //�@���̌v�Z
    Out.Normal = mul(float4(In.Normal, 0), matWorld).xyz;
    //���[���h���W�ƃ��C�g�̈ʒu�̍���
    Out.Light = LightPosition.xyz; // -WorldPosition.xyz;
    //UV���W
    Out.TexUV = In.TexUV;
    //�����o�[�g����
    Out.Col.rgb = MaterialDiffuse.rgb * CalcLightingDot(float4(In.Normal, 0)) + MaterialAmbient.rgb * ambient.rgb;
   // Out.Col.a = MaterialDiffuse.a;
	
    return Out;
}

VS_OUTPUT VS_VERTEXWORLD(VS_INPUT In)
{
	 VS_OUTPUT Out = (VS_OUTPUT) 0;
    // ���[���h���W�n�ւ̕ϊ�s
    float4 WorldPosition = mul(In.Pos, matWorld);
    //
    float4 WorldNormal = float4(In.Normal,0.0);
    // ���_���W�ϊ�
    Out.Pos = mul(WorldPosition, matVP);	

    return Out;    
}

//�s�N�Z���V�F�[�_�[
float4 PS_VERTEX(VS_OUTPUT In) : COLOR
{
    //�@���E���C�g���K��
    float3 N= normalize(In.Normal);
    float3 L = normalize(In.Light);
float3 diffuse;
if(titleLight){
    //�f�B�t���[�Y�F�̌���
diffuse = (max(dot(N, L), 0.0f) * 10.8f + 0.5f);
}else{
diffuse = (max(dot(N, L), 0.0f) * 0.8f + 0.5f);
}
    
    //
    float4 output = float4(diffuse, 1.0f) * tex2D(TextureSample, In.TexUV);
    
    //�����x��ݒ�
    output.a = MaterialDiffuse.a;
    //output.a = 1.0f;
    return output; //* tex2D(TextureSample, In.TexUV);
}

//�s�N�Z���V�F�[�_�[
float4 PS_COLORVERTEX(VS_OUTPUT In) : COLOR
{
    //�@���E���C�g���K��
    float3 N = normalize(In.Normal);
    float3 L = normalize(In.Light);
    //�f�B�t���[�Y�F�̌���
    float3 diffuse = (max(dot(N, L), 0.0f) * 0.8f + 0.5f);  
    //
    float4 output = float4(diffuse, 1.0f) * tex2D(TextureSample, In.TexUV);
    output.r = MaterialDiffuse.r;
    output.g = MaterialDiffuse.g;
    output.b = MaterialDiffuse.b;
    //�����x��ݒ�
    output.a = MaterialDiffuse.a;
    //output.a = 0.5f;
    return output; //* tex2D(TextureSample, In.TexUV);
}



technique ShaderTechnique
{
    //���߂Ȃ�
    Pass P0
    {
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = false;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
    //���ߏ�������
    Pass P1
    {
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
    //���̐F�̕\��
    Pass P2
    {
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_COLORVERTEX();

    }
    //Light�𓮂���
    Pass P3
    {    
	    ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEXLIGHT();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
}