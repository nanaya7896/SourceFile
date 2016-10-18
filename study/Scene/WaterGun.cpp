#include"WaterGun.h"

//コンストラクタ
WaterGun::WaterGun() {

	//水鉄砲の状態を地上にしておく
	state = GROUND;
	start = 0;
}

//デストラクタ
WaterGun::~WaterGun() {

}

//====================================
//関数名	:Init
//引数		:
//戻り値	:
//内容		:コンストラクタ
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
//Update関数
void WaterGun::Update(D3DXVECTOR3 tmpPos, D3DXVECTOR3 tmpRotate)
{

	//設置状態かそうではないか
	if (GROUND == state) {
		rotate = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 2.0f);
		//取得を決める
		GetGun(tmpPos);
		return;
	}
	else {
		//銃を落とすかどうか
		DisGetGun();
	}

	//所持している状態から以下の通り

	//銃の位置をゆきちゃんの場所に更新
	gunpos = tmpPos;
	gunpos.y += 3.0f;
	rotate = tmpRotate;
	//リキャストタイム0.2秒
	static float waitTime = RECAST_TIME;

	//発射ボタンをおした時に
	if (w_pInput->KeyState(DIK_SPACE) || (w_pInput->IsButtonDown(CIRCLE_BUTTON))) {
		state = SHOT;

		//リキャストタイムが終わっていれば
		if (waitTime < 0.0f) {

			//現在位置と回転を配列に保存
			bullet[bulletnum].pos = gunpos;
			bullet[bulletnum].rotate = rotate;
			bullet[bulletnum].deg = 0;
			bullet[bulletnum].used = true;

			//カウントする
			bulletnum = (bulletnum + 1) % BULLET_MAX;
			//	MyOutputDebugString("%d\n",bulletnum);
			//リキャストタイム設定
			waitTime = RECAST_TIME;
		}
	}
	else {
		state = HAVE;
	}

	waitTime -= 0.1f;

}

void WaterGun::Draw() {

	//モデルのインスタンスを取得する
	CModelManager* pModelManager = CModelManager::GetInstance();

	//水鉄砲が置かれている状態を描画する
	if (GROUND == state) {
		pModelManager->Draw(10, gunpos, rotate, D3DXVECTOR3(0.3f, 0.3f, 0.3f), 1.0f);
	}

	//持っている状態を描画
	if (HAVE == state || SHOT == state) {



		for (int i = start; i < BULLET_MAX * 2; i++) {

			int num = i%BULLET_MAX;
			//使われていなければ、次へ
			if (bullet[num].used) {
				start = num;
				break;
			}
		}

		//for文で回す
		for (int i = start; i <BULLET_MAX * 2; i++) {

			int num = i%BULLET_MAX;

			if (num == bulletnum) {
				break;
			}

			if (!bullet[num].used) {
				continue;
			}

			//重力っぽいことをやってます
			double testnum = 0.0;
			if (90.0f > bullet[num].deg) {
				bullet[num].deg += 2;
				testnum = (bullet[num].deg % 360) / 180.0*D3DX_PI;

			}
			double tmp = 0.0;
			tmp = -0.5*sin(testnum);

			//弾の移動を設定する
			D3DXVECTOR3 myvec(0.0f, tmp, -(BULLET_SPEED));

			D3DXMATRIXA16 matWorld;
			D3DXMatrixRotationY(&matWorld, bullet[num].rotate.y);
			D3DXVECTOR3 myvec2;
			D3DXVec3TransformCoord(&myvec2, &myvec, &matWorld);

			//弾の位置を更新
			bullet[num].pos = bullet[num].pos + myvec2;

			//描画を行う
			pModelManager->Draw(11, bullet[num].pos, bullet[num].rotate, D3DXVECTOR3(10.0f, 10.0f, 10.0f), 1.0f);

			//壁のあたり判定で何かに当たれば、Falseにして使ってない状態にする
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
