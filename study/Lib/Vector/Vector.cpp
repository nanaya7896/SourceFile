#include "Vector.h"

CVector::CVector()
{
}

CVector::~CVector()
{
}

D3DXVECTOR2 CVector::InitVector2()
{
	return D3DXVECTOR2(0,0);
}

D3DXVECTOR3 CVector::InitVector3()
{
	return D3DXVECTOR3(0,0,0);
}
