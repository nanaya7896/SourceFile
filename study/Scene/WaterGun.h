#pragma once
#include"../Global/Global.h"
#include"../Lib/DirectX/DInput.h"
#include"../Lib/Collider.h"
#include"../Zombie/Zombie.h"

#define GROUND	0
#define	HAVE	1
#define SHOT	2


#define	BULLET_MAX 64
#define BULLET_SPEED 5.0f
#define RECAST_TIME 0.2f

class WaterGun {

	//private�����o�ϐ�
private:

	static WaterGun* m_pWaterGunInstance;	//�C���X�^���X�ϐ�

	CDInput*				w_pInput;//�C���v�b�g�n��
									 //���S�C�̈ʒu
	D3DXVECTOR3 gunpos;

	//�e�̍\����
	struct Bullet
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rotate;
		int			deg;
		bool		used;
	};
	Bullet bullet[BULLET_MAX];

	int start = 0;
	int end = 0;

	//�e�̕ۑ�������
	int bulletnum = 0;
	//���S�C�̏��
	int state;
	//��]��
	D3DXVECTOR3 rotate;

	//�����Ă��邩�ǂ���
	BOOL floor;

	//���˒���
	BOOL schott;

	D3DXMATRIX				m_bullet;

public:
	WaterGun();
	~WaterGun();

	static void Create();//�C���X�^���X�𐶐�����
	static void Destory();//�C���X�^���X���폜

						  //�C���X�^���X���擾����
	static WaterGun* GetInstance() {
		return m_pWaterGunInstance;
	}

	void Init(CDInput* m_pInput);
	BOOL GetGun(D3DXVECTOR3 tmpPos);
	void DisGetGun();
	void Update(D3DXVECTOR3 tmpPos, D3DXVECTOR3 tmpRotate);
	void Draw();

	BOOL HitCheck(D3DXVECTOR3 checkPos);

	int wGetState()
	{
		return state;
	}

	void ResetWaterGun();
};
