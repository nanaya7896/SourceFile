//���_�V�F�[�_�[�o��
struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float4 Col : COLOR0;
};

// ���_�V�F�[�_�[
VS_OUTPUT VS_MainOut()
{

	return;
}

// �s�N�Z���V�F�[�_�[
float4 PS_main(float4 Col : COLOR0) : COLOR0
{

	// ��ɔ�(1, 0, 0, 1)��Ԃ�
	return Col;
}

technique _ColorEdit
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS_MainOut();
		PixelShader = compile vs_2_0 PS_main();

	}
}