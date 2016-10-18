#include"WaterGun.h"

//�R���X�g���N�^
WaterGun::WaterGun() {

	//���S�C�̏�Ԃ�n��ɂ��Ă���
	state = GROUND;
	start = 0;
}

//�f�X�g���N�^
WaterGun::~WaterGun() {

}

//====================================
//�֐���	:Init
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
void WaterGun::Init(CDInput* m_pInput) {

	w_pInput = m_pInput;
	gunpos = D3DXVECTOR3(125.0f, 13.25f, -130.0f);


}

BOOL WaterGun::GetGun(D3DXVECTOR3 tmpPos) 
{

	Collider* pColManager = Collider::GetInstance();
	BOOL check = pColManager->EasyCheck(tmpPos, gunpos, 5.0f);
	if (check) {
		state = HAVE;
		return TRUE;
	}

	return FALSE;
}

void WaterGun::DisGetGun() {
	if (w_pInput->KeyDown(DIK_G) || (w_pInput->IsButtonDown(X_MARK_BUTTON)))
	{
		state = GROUND;
	}
}
//Update�֐�
void WaterGun::Update(D3DXVECTOR3 tmpPos, D3DXVECTOR3 tmpRotate)
{

	//�ݒu��Ԃ������ł͂Ȃ���
	if (GROUND == state) {
		rotate = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 2.0f);
		//�擾�����߂�
		GetGun(tmpPos);
		return;
	}
	else {
		//�e�𗎂Ƃ����ǂ���
		DisGetGun();
	}

	//�������Ă����Ԃ���ȉ��̒ʂ�

	//�e�̈ʒu���䂫�����̏ꏊ�ɍX�V
	gunpos = tmpPos;
	gunpos.y += 3.0f;
	rotate = tmpRotate;
	//���L���X�g�^�C��0.2�b
	static float waitTime = RECAST_TIME;

	//���˃{�^��������������
	if (w_pInput->KeyState(DIK_SPACE) || (w_pInput->IsButtonDown(CIRCLE_BUTTON))) {
		state = SHOT;

		//���L���X�g�^�C�����I����Ă����
		if (waitTime < 0.0f) {

			//���݈ʒu�Ɖ�]��z��ɕۑ�
			bullet[bulletnum].pos = gunpos;
			bullet[bulletnum].rotate = rotate;
			bullet[bulletnum].deg = 0;
			bullet[bulletnum].used = true;

			//�J�E���g����
			bulletnum = (bulletnum + 1) % BULLET_MAX;
			//	MyOutputDebugString("%d\n",bulletnum);
			//���L���X�g�^�C���ݒ�
			waitTime = RECAST_TIME;
		}
	}
	else {
		state = HAVE;
	}

	waitTime -= 0.1f;

}

void WaterGun::Draw() {

	//���f���̃C���X�^���X���擾����
	CModelManager* pModelManager = CModelManager::GetInstance();

	//���S�C���u����Ă����Ԃ�`�悷��
	if (GROUND == state) {
		pModelManager->Draw(10, gunpos, rotate, D3DXVECTOR3(0.3f, 0.3f, 0.3f), 1.0f);
	}

	//�����Ă����Ԃ�`��
	if (HAVE == state || SHOT == state) {



		for (int i = start; i < BULLET_MAX * 2; i++) {

			int num = i%BULLET_MAX;
			//�g���Ă��Ȃ���΁A����
			if (bullet[num].used) {
				start = num;
				break;
			}
		}

		//for���ŉ�
		for (int i = start; i <BULLET_MAX * 2; i++) {

			int num = i%BULLET_MAX;

			if (num == bulletnum) {
				break;
			}

			if (!bullet[num].used) {
				continue;
			}

			//�d�͂��ۂ����Ƃ�����Ă܂�
			double testnum = 0.0;
			if (90.0f > bullet[num].deg) {
				bullet[num].deg += 2;
				testnum = (bullet[num].deg % 360) / 180.0*D3DX_PI;

			}
			double tmp = 0.0;
			tmp = -0.5*sin(testnum);

			//�e�̈ړ���ݒ肷��
			D3DXVECTOR3 myvec(0.0f, tmp, -(BULLET_SPEED));

			D3DXMATRIXA16 matWorld;
			D3DXMatrixRotationY(&matWorld, bullet[num].rotate.y);
			D3DXVECTOR3 myvec2;
			D3DXVec3TransformCoord(&myvec2, &myvec, &matWorld);

			//�e�̈ʒu���X�V
			bullet[num].pos = bullet[num].pos + myvec2;

			//�`����s��
			pModelManager->Draw(11, bullet[num].pos, bullet[num].rotate, D3DXVECTOR3(10.0f, 10.0f, 10.0f), 1.0f);

			//�ǂ̂����蔻��ŉ����ɓ�����΁AFalse�ɂ��Ďg���ĂȂ���Ԃɂ���
			if (Collider::GetInstance()->RayCast(bullet[num].pos, bullet[num].rotate)) {
				bullet[num].used = false;
			}
		}
	}
}


BOOL WaterGun::HitCheck(D3DXVECTOR3 checkPos)
{
	for (int i = 0; i < bulletnum; i++) {

		if (Collider::GetInstance()->EasyCheck(bullet[i].pos, checkPos, 5.0f)&& bullet[i].used) {
			bullet[i].used = false;
			return TRUE;
		}
	}

	return FALSE;
}

void WaterGun::ResetWaterGun()
{
	state = GROUND;
	start = 0;
	gunpos = D3DXVECTOR3(125.0f, 13.25f, -130.0f);
}
