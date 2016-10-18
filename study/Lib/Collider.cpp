#include "Collider.h"
#include <math.h>

Collider* Collider::m_pColiderInstance = NULL;
//�R���X�g���N�^
Collider::Collider()
{
	num = 0;
}

Collider::~Collider() {
}

void Collider::Create() {

	if (!m_pColiderInstance) {
		m_pColiderInstance = new Collider();
	}
}

void Collider::Destory() {
	SAFE_DELETE(m_pColiderInstance);
}

/******************************************************
EasyCheck	�F�@���t�@�C�������蔻��
����		�F	��1�����F���g�̍��W
�F	��2�����F�Ώۂ̍��W
�F	��3�����F�Ώۂ̃{�b�N�X�̑傫��
�߂�l		�F	BOOL�^�@�Փ˂��Ă��邩����
******************************************************/
BOOL Collider::EasyCheck(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large)
{

	D3DXVECTOR3 checkVec = pos2 - pos;
	return D3DXVec3Length(&checkVec) < large;

	////�_�Ɣ��Ƃ̓����蔻����ȈՓI�ɂ���B
	//if (pos.x - large <posCheck.x && posCheck.x < pos.x + large &&
	//	pos.y - large <posCheck.y && posCheck.y < pos.y + large &&
	//	pos.z - large <posCheck.z && posCheck.z < pos.z + large) {
	//	return TRUE;
	//}
	//return FALSE;
}

/******************************************************
Distance		�F�@���t�@�C�������蔻��
����		�F	��1�����F���g�̍��W
�F	��2�����F�Ώۂ̍��W
�F	��3�����F�Q�_�Ƃ̋���
�߂�l		�F	BOOL�^�@�Փ˂��Ă��邩����
******************************************************/
BOOL Collider::Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large)
{
	//�O�����̎O�����̒藝��p���Čv�Z����B
	float x = pos2.x - pos.x;
	float y = pos2.y - pos.y;
	float z = pos2.z - pos.z;

	float distance = pow(x, 2) + pow(y, 2) + pow(z, 2);
	distance = sqrt(distance);

	return distance<large;
}

BOOL Collider::Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large, float *get, D3DXVECTOR3 *r_pos)
{
	//�O�����̎O�����̒藝��p���Čv�Z����B
	float x = pos2.x - pos.x;
	float y = pos2.y - pos.y;
	float z = pos2.z - pos.z;

	float distance = pow(x, 2) + pow(y, 2) + pow(z, 2);
	distance = sqrt(distance);

	float rad = atan2(z, x);
	//float deg = rad * 180 / D3DX_PI;

	if (distance < large) {
		float x2 = sin(rad) * large;
		float z2 = cos(rad) * large;

		*r_pos = D3DXVECTOR3(pos2.x - z2, pos.y, pos2.z - x2);
	}

	*get = rad;
	return distance<large;
}

/********************************************************


*********************************************************/

/******************************************************
SetObj		�F�@���t�@�C���̓����蔻��̏�����́E�i�[
����		�F	��1�����F���W
�F	��2�����F���b�V�����
******************************************************/
bool Collider::SetObj(D3DXVECTOR3 pos, LPD3DXMESH pmesh) {

	pMesh[num].mesh = pmesh;
	pMesh[num].pos = pos;

	num++;
	return true;
}

/******************************************************
GroundCol	�F�@���̓����蔻����s��
����		�F	��1�����F���݂̈ʒu
�F	��2�����F�Ώۂ̃��b�V�����
�F	��3�����F�ǂ̂��炢����Ă��邩����
******************************************************/
BOOL Collider::GroundCol(D3DXVECTOR3 pos, D3DXVECTOR3 rota, int *Hitnum, float *numreturn) {

	//���̗���ɍ��킹��i���u���j
	static float m_WaterRot = 0.0f;
	static int beforeGround = 0;
	//m_WaterRot -= 0.001f;

	*numreturn = -99.0f;					//�߂�l�̏�����(-99.0)
	BOOL  bIsHit = FALSE;					//�Փ˂��Ă��邩�̏�����
	float fLandDistance = 0;				//�Փ˂������̋���
	D3DXVECTOR3 checkpos;					//�Ώۂ̍��W��}������ϐ�
	D3DXMATRIX matWorld, matPos, matRota;	//�s��


											//���b�V�����Ɋm�F���邽��for���ŉ�
	for (int i = 0; i < num; i++) {

		//���ꏈ��
		//�X���C�_�[�n���͔�΂�
		if (i == 2 || i == 5 || i == 7 || i==10 || i==11 || i==13) {
			continue;
		}
		//���݂̈ʒu�����Ȃ�Δ�΂�
		if (beforeGround == 12 && i == 1) {
			continue;
		}

		//���̃��b�V���i�P�j�̏ꍇ�̂݉�]���Ă��邽�߁A���̏C�����s�����ߓ��ꏈ��
		if (i == 1) {

			//���s�ړ��̍s��ϊ�
			D3DXMatrixTranslation(
				&matPos,	// ���Z���ʂ̍s��ւ̃A�h���X
				pos.x,	// X���W�̃I�t�Z�b�g
				pos.y,	// X���W�̃I�t�Z�b�g
				pos.z);	// X���W�̃I�t�Z�b�g

						//��]�̍s��ϊ�
			D3DXMatrixRotationYawPitchRoll(
				&matRota,	// ���Z���ʂ̍s��ւ̃A�h���X
				m_WaterRot,	// Y���𒆐S�Ƃ��郈�[(���W�A���P��)
				rota.x,		// X���𒆐S�Ƃ���s�b�`(���W�A���P��) 
				rota.z);	// Z���𒆐S�Ƃ��郍�[��(���W�A���P��)

							//�s�񓯎m���|�����킹��i���s�s�񂩂��]�s��ɂ�����
			matWorld = matPos * matRota;

			//�s��ϊ������W�ɓK��
			D3DXVec3TransformCoord(
				&checkpos,
				&D3DXVECTOR3(0, 0, 0),
				&matWorld);

		}
		else {
			//������s�����߂ɁA���W�ړ������t���v�Z���ē����蔻��𒲐�
			checkpos = pos - D3DXVECTOR3(pMesh[i].pos.x, pMesh[i].pos.y, pMesh[i].pos.z);
		}

		//���e���Ă��ǂ����������߂�
		float dis = 10;
		//�X���C�_�[�̎��͗�O�Ō���������
		if (i == 3) {
			dis = 5;
		}
		//��
		if (i == 12) {
			dis = 1;
		}

		//�������烌�C����ɔ�΂��A�Փ˂����ꏊ�Ƃ̋������擾
		D3DXIntersect(pMesh[i].mesh,			//�Ώۂ̃��b�V�����
			&checkpos,							//���C�𔭎˂��邽�߂̍��W
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f),		//���C���΂�����
			&bIsHit,							//���b�V���ɂ���������
			NULL,
			NULL,
			NULL,
			&fLandDistance,						//�����������b�V���Ƃ̋���
			NULL,
			NULL);

		//�K��l���A�������C������
		//fLandDistance += pMesh[i].pos.y;

		//�f�o�b�O�p�i�܂��c���Ă���Ă���������ł��j
		/*		if(bIsHit)
		MyOutputDebugString("%d==%f\n",i, fLandDistance);*/
		//�K��l���z����ł��邩
		if (bIsHit				&&	//�Ȃɂ��Ƀq�b�g���Ă��邩�ǂ���
			fLandDistance <  dis&&	//�K��l���ォ
			fLandDistance > -dis)	//�K��l��艺��
		{

			//���W���ďC���������Ƃɋ���������ɊJ���Ă���΁A�������s��
			if (*numreturn < fLandDistance) {

				//���̂ݔg������ɒ������Ċi�[
				/*if (1 == i) {
				fLandDistance += sinf(m_WaterRot);
				}*/

				//���ʂ��i�[����
				*numreturn = fLandDistance;
				*Hitnum = beforeGround = i;
			}
		}

		//������ɏ㏸������
		checkpos.y += PLAYER_HEIGHT;

		//���ォ�烌�C�����ɔ�΂��A�Փ˂����ꏊ�Ƃ̋������擾
		D3DXIntersect(pMesh[i].mesh,
			&checkpos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bIsHit,
			NULL,
			NULL,
			NULL,
			&fLandDistance,
			NULL,
			NULL);

		//�����ɍ��킹�Ă��ړ�
		//fLandDistance -= pMesh[i].pos.y;

		//����ʒu��葫���ɍďC��
		fLandDistance -= PLAYER_HEIGHT;
		fLandDistance = -(fLandDistance);

		/*if (bIsHit)
		MyOutputDebugString("%d==%f\n", i, fLandDistance);*/
		//�K��l���A�z��O�̍������𔻒�i�z��O�ł���΁A�ǂ�����������́A�R�ł���
		if (bIsHit &&
			fLandDistance <  dis&&
			fLandDistance > -dis
			)
		{
			//�ォ�牺�Ȃ̂Ń}�C�i�X�ɕϊ��������Ƃɋ�����������΁A�������s��
			if (*numreturn < fLandDistance) {

				/*if (1 == i) {
				fLandDistance += sinf(m_WaterRot);
				}*/

				*numreturn = fLandDistance;
				*Hitnum = beforeGround = i;
			}
		}
	}

//	MyOutputDebugString("%d==%f\n", *Hitnum, *numreturn);
	//�ǂ��ɂł��_���ł���΁A
	if (*numreturn == -99) {
		*numreturn = 0.0f;

		//���̐�͈ړ��Ƃ��Ă��������s���ɂȂ邱�Ƃ�True�Ƃ��ĕԂ�
		return true;
	}
	//���Ȃ����false
	return false;
}

/******************************************************
GroundCol	�F�@���̓����蔻����s��
����		�F	��1�����F���݂̈ʒu
�F	��2�����F�Ώۂ̃��b�V�����
�F	��3�����F�߂�l�F�����̐��l�𓱓�
******************************************************/
BOOL Collider::RayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate)
{

	BOOL  bIsHit = FALSE;		//�Փ˂��Ă��邩�̏�����
	
	float fLandDistance = 0.0f;	//�Փ˂������̋���

								//��]�����v�Z����
	D3DXVECTOR3 checkrota(-sin(rotate.y), 0.0f, -cos(rotate.y));

	//���C�L���X�g�̔��ˈʒu��������ɒ���
	pos.y += PLAYER_HEIGHT;

	//���i0)�Ɛ�(1)�͏��̔���Ƃ�����̂ŁA�X���[����
	for (int i = 2; i < num; i++) {
		if (i == 2 || i == 5 || i == 7 || i == 10 || i == 11 || i == 13) {
			continue;
		}
			D3DXVECTOR3 Movepos(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 checkpos;

			checkpos = pos - D3DXVECTOR3(pMesh[i].pos.x, pMesh[i].pos.y, pMesh[i].pos.z);
		//���݈ʒu���烌�C�𐳖ʂɔ�΂��A�Փ˂����ꏊ�Ƃ̋������擾
		D3DXIntersect(pMesh[i].mesh,				//�Ώۂ̃��b�V�����
			&checkpos,								//���C�𔭎˂��邽�߂̍��W
			&checkrota,							//���C���΂�����
			&bIsHit,							//���b�V���ɂ���������
			NULL,
			NULL,
			NULL,
			&fLandDistance,						//�����������b�V���Ƃ̋���
			NULL,
			NULL);

		//�f�o�b�O�p�i�܂��c���Ă���Ă���������ł��j
		/*	if(bIsHit)
		MyOutputDebugString("%d==%f\n",i, fLandDistance);*/

		if (bIsHit &&fLandDistance < 5.0f) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Collider::RayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate, float * HitNum)
{

	BOOL  bIsHit = FALSE;		//�Փ˂��Ă��邩�̏�����
	*HitNum = 999.9f;
	float fLandDistance = 0.0f;	//�Փ˂������̋���

								//��]�����v�Z����
	D3DXVECTOR3 checkrota(-sin(rotate.y), 0.0f, -cos(rotate.y));
	D3DXVECTOR3 checkpos;
	//���C�L���X�g�̔��ˈʒu��������ɒ���
	pos.y += PLAYER_HEIGHT;

	//���i0)�Ɛ�(1)�͏��̔���Ƃ�����̂ŁA�X���[����
	for (int i = 2; i < num; i++) {
		if (i == 2 || i == 5 || i == 7 || i == 10 || i == 11 || i == 13) {
			continue;
		}
		
			D3DXVECTOR3 Movepos(0.0f, 0.0f, 0.0f);
				
			checkpos = pos- D3DXVECTOR3(pMesh[i].pos.x, pMesh[i].pos.y, pMesh[i].pos.z);
	
		//���݈ʒu���烌�C�𐳖ʂɔ�΂��A�Փ˂����ꏊ�Ƃ̋������擾
		D3DXIntersect(pMesh[i].mesh,				//�Ώۂ̃��b�V�����
			&checkpos,								//���C�𔭎˂��邽�߂̍��W
			&checkrota,							//���C���΂�����
			&bIsHit,							//���b�V���ɂ���������
			NULL,
			NULL,
			NULL,
			&fLandDistance,						//�����������b�V���Ƃ̋���
			NULL,
			NULL);

		//�f�o�b�O�p�i�܂��c���Ă���Ă���������ł��j
	/*		if(bIsHit)
		MyOutputDebugString("%d==%f\n",i, fLandDistance);*/
		if (bIsHit&& fLandDistance < *HitNum) {
			*HitNum = fLandDistance;
		}

		if (bIsHit &&fLandDistance < 5.0f) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Collider::DirectRayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate, int objnum, float * HitNum)
{

	BOOL  bIsHit = FALSE;		//�Փ˂��Ă��邩�̏�����
	float fLandDistance = 0.0f;	//�Փ˂������̋���

								//��]�����v�Z����
	D3DXVECTOR3 checkrota(-sin(rotate.y), 0.0f, -cos(rotate.y));

	//���C�L���X�g�̔��ˈʒu��������ɒ���
	pos.y += 6;

	pos -= D3DXVECTOR3(pMesh[objnum].pos.x, pMesh[objnum].pos.y, pMesh[objnum].pos.z);

	//���݈ʒu���烌�C�𐳖ʂɔ�΂��A�Փ˂����ꏊ�Ƃ̋������擾
	D3DXIntersect(pMesh[objnum].mesh,				//�Ώۂ̃��b�V�����
		&pos,								//���C�𔭎˂��邽�߂̍��W
		&checkrota,							//���C���΂�����
		&bIsHit,							//���b�V���ɂ���������
		NULL,
		NULL,
		NULL,
		&fLandDistance,						//�����������b�V���Ƃ̋���
		NULL,
		NULL);

	*HitNum = fLandDistance;

	if (bIsHit &&fLandDistance < 5.0f) {
		return TRUE;
	}


	return FALSE;
}
