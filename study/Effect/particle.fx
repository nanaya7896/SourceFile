//float4 color = (1,1,1, 1);
//�f�B�t���[�Y�F
float4 matDiffuse : MATERIALDIFFUSE;
//�A���r�G���g�F
float4 matAmbient : MATERIALAMBIENT;
float4 ambient : AMBIENT;
float4 emmisive : EMMISIVE;
//�s��
float4x4 world;
float4x4 view;
float4x4 proj;
//���C�g
float3 dir : LIGHTDIRECTION;

//�e�N�X�`��
texture tex;

//�e�N�X�`���̐ݒ�
sampler smp = sampler_state
{
    texture = <tex>;
    //�e�N�X�`�����WU,V�͈̔͊O�̈ʒu�ɂ�����W��0.0�܂���1.0�ɂ���
    AddressU = CLAMP;
    AddressV = CLAMP;
    //���`���
    //�~�b�v���x��
    MipFilter = LINEAR;
    //�k���̃T���v�����O
    MinFilter = LINEAR;
    //�g��̃T���v�����O
    MagFilter = LINEAR;
};


// ���_�V�F�[�_�[����
struct VS_INPUT
{
    //���W
    float3 pos : POSITION;
    //�|�C���^�T�C�Y
    float size : PSIZE;
    //�F
    float4 color : COLOR0;
    float4 unused : COLOR1;
    //�@��
    float4 normal : NORMAL;
    float2 tex : TEXCOORD0;
   
};

// ���_�V�F�[�_�[�o��
struct VS_OUTPUT
{
    float4 pos : POSITION;
    float size : PSIZE;
    float4 color : COLOR0;
    float2 tex : TEXCOORD0;

};

// �s�N�Z���V�F�[�_�[����
struct PS_INPUT
{
    float4 Col : COLOR0;
    float2 TexUV : TEXCOORD0;
};

VS_OUTPUT VS_VERTEX(VS_INPUT In)
{
    // VS_OUTPUT��������
    VS_OUTPUT Out = (VS_OUTPUT) 0;
    //���W�ϊ�
    Out.pos = float4(In.pos, 1);
    Out.pos = mul(Out.pos, world);
    Out.pos = mul(Out.pos, view);

    float d = length(Out.pos.xyz);
    Out.pos = mul(Out.pos, proj);
    
     //�@���̕ϊ��E���K��
    float3 N = mul(In.normal, Out.pos);
    N = normalize(N);
    //�@���ƃ��C�g�̓��ς��v�Z���A���˗��Ƃ��ăf�B�q���[�Y�F�Ɋ|�����킹��
    Out.color.rgb = matDiffuse.rgb * max(dot(-dir, N), 0);
    //+matAmbient.rgb * ambient.rgb;
    //�A���t�@�����ƃe�N�X�`����UV���W�͂��̂܂܏o��
    Out.color.a = matDiffuse.a;
    Out.tex = In.tex;
    Out.size = In.size;

    Out.color = matDiffuse;

    return Out;
}

float4 PS_VERTEX(PS_INPUT In) : COLOR0
{
    return tex2D(smp, In.TexUV) * In.Col+emmisive; //* emmisive;
    //out_color =tex2D(smp, in_tex)*in_color; //* color;
}

technique tec
{

    Pass P0
    {
  // �T�C�Y�͎����Ōv�Z����̂ŃX�v���C�g�̐�����������
        ALPHABLENDENABLE = true;
        SPECULARENABLE = true;
        PointSpriteEnable = true;
        Ditherenable = true;
        ZENABLE = true;
        ZWRITEENABLE = false;
        SrcBlend = SRCALPHA;
        DestBlend = ONE;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
      
    }
    Pass P1
    {
        ALPHABLENDENABLE = true;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
}