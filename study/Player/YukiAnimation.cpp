#include "YukiAnimation.h"

// �R���X�g���N�^
YukiAnimation::YukiAnimation(LPDIRECT3DDEVICE9 pd3dDevice, char* pFileName)
{
	d3d_device9_ = pd3dDevice;

	animation_ = nullptr;
	effect_ = nullptr;
	position_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	angle_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale_ = D3DXVECTOR3(1.2f, 1.2f, 1.2f);
	animation_time_ = 0.025f;

	Initialize(pFileName);
}
// �f�X�g���N�^
YukiAnimation::~YukiAnimation()
{
	SAFE_DELETE(animation_);
	SAFE_RELEASE(effect_);
}

// ���s
void YukiAnimation::Update()
{
	animation_->SetTime(animation_time_);
}
// �`��
void YukiAnimation::Draw()
{
	D3DXMATRIX matWorld, matPosition, matRotation, matView, matProjection, matScaling;

	CCamera* pCamera = CCamera::GetInstance();
	matView = pCamera->getView();
	matProjection = pCamera->getProj();
	D3DXVECTOR4 vViewPosition(pCamera->GetViewPosition(), 1.0f);
	D3DXVECTOR4 vDirectionalLight(pCamera->GetDirectionalLight(), 1.0f);

	D3DXMatrixTranslation(
		&matPosition,	// ���Z���ʂ̍s��ւ̃A�h���X
		position_.x,	// X���W�̃I�t�Z�b�g
		position_.y,	// X���W�̃I�t�Z�b�g
		position_.z);	// X���W�̃I�t�Z�b�g
						// ���[�A�s�b�`�A���[�����w�肵�čs����쐬
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,	// ���Z���ʂ̍s��ւ̃A�h���X
		angle_.y,		// Y���𒆐S�Ƃ��郈�[(���W�A���P��)
		angle_.x,		// X���𒆐S�Ƃ���s�b�`(���W�A���P��) 
		angle_.z);	// Z���𒆐S�Ƃ��郍�[��(���W�A���P��)


	matWorld = matRotation * matPosition;

	if (effect_)
	{
		// �e�N�j�b�N�̑I��
		effect_->SetTechnique("ShaderTechnique");

		// �т���
		effect_->Begin(0, 0); // �p�X���w��
	
							  //�X�P�[���ϊ�
		D3DXMatrixScaling(&matScaling, scale_.x, scale_.y, scale_.z);

								// ���s�ړ��s����쐬
		D3DXMatrixTranslation(&matPosition, position_.x, position_.y, position_.z);
		// ���[�A�s�b�`�A���[�����w�肵�čs����쐬
		D3DXMatrixRotationYawPitchRoll(
			&matRotation,	// ���Z���ʂ̍s��ւ̃A�h���X
			angle_.y,		// Y���𒆐S�Ƃ��郈�[(���W�A���P��)
			angle_.x,		// X���𒆐S�Ƃ���s�b�`(���W�A���P��) 
			angle_.z);	// Z���𒆐S�Ƃ��郍�[��(���W�A���P��)

		matWorld = matScaling * matRotation * matPosition;

		// ���[���h�s����w��
		// �������͐�΃V�F�[�_�[�̂ق��Ő錾������ƈꏏ�ɂ���
		// ���Ȃ��Ⴋ��񂫂��ς�[
		effect_->SetMatrix("matWorld", &matWorld);

		// �r���[�s����w��
		effect_->SetMatrix("matView", &matView);

		// �v���W�F�N�V�����s����w��
		effect_->SetMatrix("matProjection", &matProjection);

		// �J�����̈ʒu���w��
		effect_->SetVector("ViewPosition", &vViewPosition);

		// ���C�g�������w��
		effect_->SetVector("DirectionalLight", &vDirectionalLight);

		// �V�F�[�_�ݒ���O���{�ɍX�V
		effect_->CommitChanges();

		animation_->Draw();

		effect_->EndPass();
	}
}

// ����������
BOOL YukiAnimation::Initialize(char* pFileName)
{
	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;

	// �V�F�[�_�̓ǂݍ���
	hr = D3DXCreateEffectFromFile(
		d3d_device9_,	// �f�o�C�X
		"Effect\\YukiAnimation.fx",	// �t�@�C����
		NULL,			// �v���v���Z�b�T��`�ւ̃|�C���^
		NULL,			// �I�v�V�����̃C���^�[�t�F�C�X�|�C���^ 
		0,				// �R���p�C���I�v�V����
		NULL,			// �G�t�F�N�g�v�[���I�u�W�F�N�g�ւ̃|�C���^
		&effect_,		// �G�t�F�N�g�I�u�W�F�N�g�ւ̃|�C���^ (�f�o�C�X���񂾂�Ƃ��Ɏ��ʂ�)
		&pErrorMsgs);	// �G���[����ьx���̃��X�g���܂ރo�b�t�@
	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	DWORD Time = 20; // 20�~���b
	animation_ = new CAnimMesh(d3d_device9_, Time, effect_);

	animation_->LoadFromFile(pFileName);

	return TRUE;
}