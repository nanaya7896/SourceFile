#include "AbstractWeapon.h"

CAbstractWeapon::CAbstractWeapon()
{

}

CAbstractWeapon::~CAbstractWeapon()
{

}



void CAbstractWeapon::Update(D3DXVECTOR3 pos, D3DXVECTOR3 rotate)
{
	//MyOutputDebugString("����͊��N���X�̃A�b�v�f�[�g�ł�\n");
}


void CAbstractWeapon::Attack()
{
	//MyOutputDebugString("����͊��N���X�U���ł�\n");
}

void CAbstractWeapon::SetPos(D3DXVECTOR3 pos)
{
}

void CAbstractWeapon::SetRot(D3DXVECTOR3 rot)
{
}

int CAbstractWeapon::GetState()
{
	return 0;
}

BOOL CAbstractWeapon::HitCheck(D3DXVECTOR3 checkPos)
{
	return FALSE;
}
