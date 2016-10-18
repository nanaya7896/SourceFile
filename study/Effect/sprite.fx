float4x4 matWorld;	//���[���h���W
float4x4 matVP;		//�ˉe�ϊ��ւ̍��W�ϊ�
texture m_pTexture;	//�e�N�X�`��
float4 Diffuse : COLOR0;//���_�̃f�B�t���[�Y�F


//�e�N�X�`������ǂ�����ĐF�����o�����ݒ�
sampler smp =sampler_state{
	texture=<m_pTexture>;
	MipFilter=NONE;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
	AddressU=WRAP;
	AddressV=WRAP;
};

//=====�\���̂̒�`=====//
//���͂̍\����
struct VS_INPUT{
	float4 pos 	: POSITION;	//���_�̈ʒu
	float4 Diffuse 	: COLOR0;	//���_�̃f�t���[�Y�F
	float2 uv 	:TEXCOORD0; 	//UV�l
};

//�o�͂̍\����
struct VS_OUTPUT{
	float4 pos 	: POSITION;	//���_�̈ʒu
	float4 Diffuse 	: COLOR0;	//���_�̃f�t���[�Y�F
	float2 uv 	: TEXCOORD0;	//UV�l
};

//���_shader
VS_OUTPUT VS_VERTEX(VS_INPUT Input){
	//�g�p����\����VS_OUT�����������Ă���
	VS_OUTPUT Output=(VS_OUTPUT)0;

	//���[���h���W�n
	float4 WorldPosition =mul(Input.pos,matWorld);

	//���_���W�ϊ�
	Output.pos=mul(WorldPosition,matVP);

	//���_�̐F
	Output.Diffuse=Input.Diffuse;

	//�e�N�X�`��UV�l���o��
	Output.uv=Input.uv;

	return Output;
}


//�s�N�Z���V�F�[�_�[
float4 PS_VERTEX(VS_OUTPUT Input) : COLOR0{
	
	float4 color=Input.Diffuse*tex2D(smp,Input.uv);
	//color.a*=alpha;
	return color;
}


//Main�ƂȂ�ӏ�
technique Tech{
	//�p�X�ɂ��e�N�j�b�N�̎�ނ𕪂���
	pass P0{
		//�����x����
		//�G�t�F�N�g�X�e�[�g
		//�J�����O�̐ݒ�
		CULLMODE=NONE;
		//�A���t�@�u�����h�̐ݒ�
		ALPHABLENDENABLE=TRUE;
		//
		SRCBLEND=SRCALPHA;
		//
		DESTBLEND=ONE;
		//
		ZENABLE=TRUE;
		//
		ZWRITEENABLE=TRUE;

		VertexShader=compile vs_2_0 VS_VERTEX();
		PixelShader=compile ps_2_0 PS_VERTEX();
	}
	pass P1{
		//�ʏ퍇��
		CULLMODE=NONE;
		ALPHABLENDENABLE=TRUE;
		SRCBLEND=SRCALPHA;
		DESTBLEND=INVSRCALPHA;
		ZENABLE=FALSE;
		ZWRITEENABLE=FALSE;
		VertexShader=compile vs_2_0 VS_VERTEX();
		PixelShader=compile ps_2_0 PS_VERTEX();
	}
    pass P2
    {
        CULLMODE = NONE;
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = INVDESTCOLOR;
        DESTBLEND = ONE;
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE
;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }

}