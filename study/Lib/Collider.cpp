#include "Collider.h"
#include <math.h>

Collider* Collider::m_pColiderInstance = NULL;
//コンストラクタ
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
EasyCheck	：　ｘファイル当たり判定
引数		：	第1引数：自身の座標
：	第2引数：対象の座標
：	第3引数：対象のボックスの大きさ
戻り値		：	BOOL型　衝突しているか判定
******************************************************/
BOOL Collider::EasyCheck(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large)
{

	D3DXVECTOR3 checkVec = pos2 - pos;
	return D3DXVec3Length(&checkVec) < large;

	////点と箱との当たり判定を簡易的にする。
	//if (pos.x - large <posCheck.x && posCheck.x < pos.x + large &&
	//	pos.y - large <posCheck.y && posCheck.y < pos.y + large &&
	//	pos.z - large <posCheck.z && posCheck.z < pos.z + large) {
	//	return TRUE;
	//}
	//return FALSE;
}

/******************************************************
Distance		：　ｘファイル当たり判定
引数		：	第1引数：自身の座標
：	第2引数：対象の座標
：	第3引数：２点との距離
戻り値		：	BOOL型　衝突しているか判定
******************************************************/
BOOL Collider::Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large)
{
	//三次元の三平方の定理を用いて計算する。
	float x = pos2.x - pos.x;
	float y = pos2.y - pos.y;
	float z = pos2.z - pos.z;

	float distance = pow(x, 2) + pow(y, 2) + pow(z, 2);
	distance = sqrt(distance);

	return distance<large;
}

BOOL Collider::Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large, float *get, D3DXVECTOR3 *r_pos)
{
	//三次元の三平方の定理を用いて計算する。
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
SetObj		：　ｘファイルの当たり判定の情報を解析・格納
引数		：	第1引数：座標
：	第2引数：メッシュ情報
******************************************************/
bool Collider::SetObj(D3DXVECTOR3 pos, LPD3DXMESH pmesh) {

	pMesh[num].mesh = pmesh;
	pMesh[num].pos = pos;

	num++;
	return true;
}

/******************************************************
GroundCol	：　床の当たり判定を行う
引数		：	第1引数：現在の位置
：	第2引数：対象のメッシュ情報
：	第3引数：どのぐらい離れているか数字
******************************************************/
BOOL Collider::GroundCol(D3DXVECTOR3 pos, D3DXVECTOR3 rota, int *Hitnum, float *numreturn) {

	//水の流れに合わせる（仮置き）
	static float m_WaterRot = 0.0f;
	static int beforeGround = 0;
	//m_WaterRot -= 0.001f;

	*numreturn = -99.0f;					//戻り値の初期化(-99.0)
	BOOL  bIsHit = FALSE;					//衝突しているかの初期化
	float fLandDistance = 0;				//衝突した時の距離
	D3DXVECTOR3 checkpos;					//対象の座標を挿入する変数
	D3DXMATRIX matWorld, matPos, matRota;	//行列


											//メッシュ毎に確認するためfor文で回す
	for (int i = 0; i < num; i++) {

		//特殊処理
		//スライダー系統は飛ばす
		if (i == 2 || i == 5 || i == 7 || i==10 || i==11 || i==13) {
			continue;
		}
		//現在の位置が橋ならば飛ばす
		if (beforeGround == 12 && i == 1) {
			continue;
		}

		//水のメッシュ（１）の場合のみ回転しているため、その修正を行うため特殊処理
		if (i == 1) {

			//平行移動の行列変換
			D3DXMatrixTranslation(
				&matPos,	// 演算結果の行列へのアドレス
				pos.x,	// X座標のオフセット
				pos.y,	// X座標のオフセット
				pos.z);	// X座標のオフセット

						//回転の行列変換
			D3DXMatrixRotationYawPitchRoll(
				&matRota,	// 演算結果の行列へのアドレス
				m_WaterRot,	// Y軸を中心とするヨー(ラジアン単位)
				rota.x,		// X軸を中心とするピッチ(ラジアン単位) 
				rota.z);	// Z軸を中心とするロール(ラジアン単位)

							//行列同士を掛け合わせる（平行行列から回転行列にかける
			matWorld = matPos * matRota;

			//行列変換を座標に適応
			D3DXVec3TransformCoord(
				&checkpos,
				&D3DXVECTOR3(0, 0, 0),
				&matWorld);

		}
		else {
			//判定を行うために、座標移動した逆数計算して当たり判定を調節
			checkpos = pos - D3DXVECTOR3(pMesh[i].pos.x, pMesh[i].pos.y, pMesh[i].pos.z);
		}

		//許容しても良い高さを決める
		float dis = 10;
		//スライダーの時は例外で厳しくする
		if (i == 3) {
			dis = 5;
		}
		//橋
		if (i == 12) {
			dis = 1;
		}

		//足元からレイを上に飛ばし、衝突した場所との距離を取得
		D3DXIntersect(pMesh[i].mesh,			//対象のメッシュ情報
			&checkpos,							//レイを発射するための座標
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f),		//レイを飛ばす向き
			&bIsHit,							//メッシュにあたったか
			NULL,
			NULL,
			NULL,
			&fLandDistance,						//あたったメッシュとの距離
			NULL,
			NULL);

		//規定値より、高さを修正する
		//fLandDistance += pMesh[i].pos.y;

		//デバッグ用（まだ残してくれてたら嬉しいです）
		/*		if(bIsHit)
		MyOutputDebugString("%d==%f\n",i, fLandDistance);*/
		//規定値より想定内であるか
		if (bIsHit				&&	//なにかにヒットしているかどうか
			fLandDistance <  dis&&	//規定値より上か
			fLandDistance > -dis)	//規定値より下か
		{

			//座標を再修正したあとに距離がさらに開いていれば、処理を行う
			if (*numreturn < fLandDistance) {

				//水のみ波をさらに調整して格納
				/*if (1 == i) {
				fLandDistance += sinf(m_WaterRot);
				}*/

				//結果を格納する
				*numreturn = fLandDistance;
				*Hitnum = beforeGround = i;
			}
		}

		//少し上に上昇させる
		checkpos.y += PLAYER_HEIGHT;

		//頭上からレイを下に飛ばし、衝突した場所との距離を取得
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

		//高さに合わせてｙ移動
		//fLandDistance -= pMesh[i].pos.y;

		//頭上位置より足元に再修正
		fLandDistance -= PLAYER_HEIGHT;
		fLandDistance = -(fLandDistance);

		/*if (bIsHit)
		MyOutputDebugString("%d==%f\n", i, fLandDistance);*/
		//規定値より、想定外の高さかを判定（想定外であれば、壁があるもしくは、崖である
		if (bIsHit &&
			fLandDistance <  dis&&
			fLandDistance > -dis
			)
		{
			//上から下なのでマイナスに変換したあとに距離が高ければ、処理を行う
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
	//どこにでもダメであれば、
	if (*numreturn == -99) {
		*numreturn = 0.0f;

		//この先は移動としておかしい行動になることをTrueとして返す
		return true;
	}
	//問題なければfalse
	return false;
}

/******************************************************
GroundCol	：　床の当たり判定を行う
引数		：	第1引数：現在の位置
：	第2引数：対象のメッシュ情報
：	第3引数：戻り値：距離の数値を導入
******************************************************/
BOOL Collider::RayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate)
{

	BOOL  bIsHit = FALSE;		//衝突しているかの初期化
	
	float fLandDistance = 0.0f;	//衝突した時の距離

								//回転率を計算する
	D3DXVECTOR3 checkrota(-sin(rotate.y), 0.0f, -cos(rotate.y));

	//レイキャストの発射位置を少し上に調整
	pos.y += PLAYER_HEIGHT;

	//床（0)と水(1)は床の判定とさせるので、スルーする
	for (int i = 2; i < num; i++) {
		if (i == 2 || i == 5 || i == 7 || i == 10 || i == 11 || i == 13) {
			continue;
		}
			D3DXVECTOR3 Movepos(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 checkpos;

			checkpos = pos - D3DXVECTOR3(pMesh[i].pos.x, pMesh[i].pos.y, pMesh[i].pos.z);
		//現在位置からレイを正面に飛ばし、衝突した場所との距離を取得
		D3DXIntersect(pMesh[i].mesh,				//対象のメッシュ情報
			&checkpos,								//レイを発射するための座標
			&checkrota,							//レイを飛ばす向き
			&bIsHit,							//メッシュにあたったか
			NULL,
			NULL,
			NULL,
			&fLandDistance,						//あたったメッシュとの距離
			NULL,
			NULL);

		//デバッグ用（まだ残してくれてたら嬉しいです）
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

	BOOL  bIsHit = FALSE;		//衝突しているかの初期化
	*HitNum = 999.9f;
	float fLandDistance = 0.0f;	//衝突した時の距離

								//回転率を計算する
	D3DXVECTOR3 checkrota(-sin(rotate.y), 0.0f, -cos(rotate.y));
	D3DXVECTOR3 checkpos;
	//レイキャストの発射位置を少し上に調整
	pos.y += PLAYER_HEIGHT;

	//床（0)と水(1)は床の判定とさせるので、スルーする
	for (int i = 2; i < num; i++) {
		if (i == 2 || i == 5 || i == 7 || i == 10 || i == 11 || i == 13) {
			continue;
		}
		
			D3DXVECTOR3 Movepos(0.0f, 0.0f, 0.0f);
				
			checkpos = pos- D3DXVECTOR3(pMesh[i].pos.x, pMesh[i].pos.y, pMesh[i].pos.z);
	
		//現在位置からレイを正面に飛ばし、衝突した場所との距離を取得
		D3DXIntersect(pMesh[i].mesh,				//対象のメッシュ情報
			&checkpos,								//レイを発射するための座標
			&checkrota,							//レイを飛ばす向き
			&bIsHit,							//メッシュにあたったか
			NULL,
			NULL,
			NULL,
			&fLandDistance,						//あたったメッシュとの距離
			NULL,
			NULL);

		//デバッグ用（まだ残してくれてたら嬉しいです）
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

	BOOL  bIsHit = FALSE;		//衝突しているかの初期化
	float fLandDistance = 0.0f;	//衝突した時の距離

								//回転率を計算する
	D3DXVECTOR3 checkrota(-sin(rotate.y), 0.0f, -cos(rotate.y));

	//レイキャストの発射位置を少し上に調整
	pos.y += 6;

	pos -= D3DXVECTOR3(pMesh[objnum].pos.x, pMesh[objnum].pos.y, pMesh[objnum].pos.z);

	//現在位置からレイを正面に飛ばし、衝突した場所との距離を取得
	D3DXIntersect(pMesh[objnum].mesh,				//対象のメッシュ情報
		&pos,								//レイを発射するための座標
		&checkrota,							//レイを飛ばす向き
		&bIsHit,							//メッシュにあたったか
		NULL,
		NULL,
		NULL,
		&fLandDistance,						//あたったメッシュとの距離
		NULL,
		NULL);

	*HitNum = fLandDistance;

	if (bIsHit &&fLandDistance < 5.0f) {
		return TRUE;
	}


	return FALSE;
}
