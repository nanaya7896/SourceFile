#include "WG.h"

Cwg::Cwg()
{
	gunpos = { 125.0f, 13.25f, -130.0f };
	m_State = Land;
	start = 0;

}

Cwg::~Cwg()
{
}

void Cwg::WG_ResetState()
{
	gunpos = { 125.0f, 13.25f, -130.0f };
	m_State = Land;
	start = 0;
}

BOOL Cwg::PutWeapon(D3DXVECTOR3 pos)
{
	if (m_State == Have)
	{
		m_State = Land;
		gunpos = pos;
		return TRUE;
	}

	return FALSE;
}

BOOL Cwg::GetWeapon(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 dis = pos - gunpos;
	rotate = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 2.0f);
	if (abs(D3DXVec3Length(&dis)) < 3.0f)
	{
		m_State = Have;
		return TRUE;
	}
	return FALSE;
}

void Cwg::Update(D3DXVECTOR3 pos,D3DXVECTOR3 tmpRo)
{
	gunpos = pos;
	gunpos.y += 3.0f;
	rotate = tmpRo;
	//MyOutputDebugString("�S�C�X�V\n");
}

void Cwg::Attack()
{
	//���L���X�g�^�C��0.2�bw
	static float waitTime = RECAST_TIME;
	//���L���X�g�^�C�����I����Ă����
	if (waitTime < 0.0f) 
	{
		m_State = Shot;
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
	else 
	{
		m_State = Have;
	}
//	MyOutputDebugString("���S�C�𔭎�\n");
	waitTime -= 0.1f;
}

void Cwg::Draw()
{
	//���f���̃C���X�^���X���擾����
	CModelManager* pModelManager = CModelManager::GetInstance();
	if (m_State ==Land)
	{
		pModelManager->Draw(10, gunpos, rotate, D3DXVECTOR3(0.3f, 0.3f, 0.3f), 1.0f);
		return;
	}

	if (m_State == Have || m_State == Shot)
	{
		for (int i = start; i < BULLET_MAX * 2; i++) 
		{

			int num = i%BULLET_MAX;
			//�g���Ă��Ȃ���΁A����
			if (bullet[num].used)
			{
				start = num;
				break;
			}
		}
		//for���ŉ�
		for (int i = start; i <BULLET_MAX * 2; i++)
		{

			int num = i%BULLET_MAX;

			if (num == bulletnum) 
			{
				break;
			}

			if (!bullet[num].used)
			{
				continue;
			}

			//�d�͂��ۂ����Ƃ�����Ă܂�
			double testnum = 0.0;
			if (90.0f > bullet[num].deg)
			{
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

BOOL Cwg::HitCheck(D3DXVECTOR3 checkPos)
{
	for (int i = 0; i < bulletnum; i++) 
	{

		if (Collider::GetInstance()->EasyCheck(bullet[i].pos, checkPos, 5.0f) && bullet[i].used) 
		{
			bullet[i].used = false;
			return TRUE;
		}
	}
	return FALSE;
}


