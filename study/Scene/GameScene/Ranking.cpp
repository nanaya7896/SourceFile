#include "Ranking.h"

//====================================
//関数名	:CRanking
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CRanking::CRanking()
{
	for (int i = 0; i < 3; i++) {
		rank[i] = 0;
	}
	if ((fp = fopen("ranking.txt", "r")) != NULL) {
		
		char tmp[6] = "";
		int i = 0;
		while (fgets(tmp, 5, fp) != NULL && i<3) {
			rank[i] = atoi(tmp);
			i++;
		}
	}
	fclose(fp);
}

//====================================
//関数名	:~CRanking
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CRanking::~CRanking()
{
	if ((fp = fopen("ranking.txt", "w")) != NULL)
	{
		for (int i = 0; i < 3; i++) 
		{
			fprintf(fp, "%d\n", rank[i]);
		}
	}
	fclose(fp);
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新
//====================================
void CRanking::Update()
{

}

//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void CRanking::Draw()
{
	//インスタンス生成
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(26, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int tmp = rank[j] / (int)pow(10, i);
			tmp %= 10;
			int x = (WINDOW_WIDTH / 20.0f) *(9.3f - (float)i);
			pTextureManager->Draw(tmp, D3DXVECTOR3(x, 380.0f+(130.0f*j), 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		}
	}
}

void CRanking::RankinCheck()
{
	int score;
	score  = CScore::GetInstnce()->GetScore(0);
	score += CScore::GetInstnce()->GetScore(1) * 10;
	score += CScore::GetInstnce()->GetScore(2) * 100;

	if (rank[0] < score) {
		rank[2] = rank[1];
		rank[1] = rank[0];
		rank[0] = score;
	}
	else if (rank[1] < score) {
		rank[2] = rank[1];
		rank[1] = score;
	}
	else if (rank[2] < score) {
		rank[2] = score;
	}
}
