#include "SoundManager.h"

//====================================
//関数名	:EventNotifyProc
//引数		:param:アドレス
//戻り値	:
//内容		:イベント管理用プロシージャ
//====================================
//DWORD WINAPI CSoundManager::EventNotifyProc(LPVOID param)
//{
//	CSoundManager* pSound = (CSoundManager*)param;
//
//	while (TRUE)
//	{
//		DWORD dwResult = WaitForMultipleObjects(
//			NUMSECONDARY + 1,	//オブジェクトのハンドル数
//			pSound->hEvent,		//オブジェクトハンドルの配列へのポインタ
//			FALSE,				//待機の種類(どれか一つがシグナル状態になったとき)
//			INFINITE			//タイムアウト時間をミリ秒単位(条件がみたされるまで待ち)
//			);
//		if (dwResult == NUMSECONDARY)
//		{
//			ExitThread(0);
//		}
//		else
//		{
//			pSound->pDSoundSecondaryBuffer[dwResult].SetPlay(TRUE);
//		}
//	}
//	return 0;
//}

//====================================
//関数名	:CSoundManager
//引数		:hWnd:ウィンドウハンドル
//戻り値	:
//内容		:コンストラクタ
//====================================
CSoundManager::CSoundManager(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
	
	//サウンドバッファ
	pDSoundBuffer = new CSoundBuffer[NUMEFFECTSOUND];
	//セカンダリバッファ
	pDSoundSecondaryBuffer = new CSoundBuffer[NUMSECONDARY];

	//停止時に発生するイベントを作成
	for (DWORD i = 0; i < NUMSECONDARY; i++)
	{
		hEvent[i] = ::CreateEvent(
			NULL,	//
			FALSE,	//
			FALSE,	//
			NULL	//
			);
	}
	//DWORD ThreadID;
	//hThread = CreateThread(
	//	NULL,				//
	//	0,					//
	//	EventNotifyProc,	//
	//	this,				//
	//	0,					//
	//	&ThreadID			//
	//	);
}

//====================================
//関数名	:~CSoundManager
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CSoundManager::~CSoundManager()
{
	DWORD i;



	for (i = 0; i < NUMEFFECTSOUND; i++)
	{
		pDSoundBuffer[i].Release();
	}
	if (pDSoundBuffer)
	{
		delete[] pDSoundBuffer;
	}

	for (i = 0; i < NUMSECONDARY; i++)
	{
		pDSoundSecondaryBuffer[i].Release();
	}
	if (pDSoundSecondaryBuffer)
	{
		delete[] pDSoundSecondaryBuffer;
	}
}

//====================================
//関数名	:LoadWave
//引数		:setPosition:格納番号
//			 fileName	:サウンドファイル名
//戻り値	:TRUE:成功　FALSE:失敗
//内容		:WAVEファイルの読み込み
//====================================
BOOL CSoundManager::LoadWave(DWORD setPosition, char *fileName)
{
	if (!pDSoundBuffer[setPosition].LoadWave(DirectSound, fileName))
	{
		return FALSE;
	}

	return TRUE;
}

//====================================
//関数名	:Play
//引数		:setPosition:演奏番号
//			 loop		:繰り返すか否か
//戻り値	:
//内容		:サウンド再生
//====================================
void CSoundManager::Play(DWORD setPosition, BOOL loop)
{
	DWORD i;

	if (setPosition >= NUMEFFECTSOUND)
	{
		//読み込んだ個数以外のサウンドが選択された
		return;
	}
	
	//セカンダリバッファに、同じサウンドがあるか確認
	if (pDSoundBuffer[setPosition].IsAlive())
	{
		for (i = 0; i < NUMSECONDARY; i++)
		{
			//現在再生されているものと現在入っているものが今読み込んだ数字と同じなら
			if (pDSoundSecondaryBuffer[i].GetPlay() && pDSoundSecondaryBuffer[i].GetNumPlay() == (int)setPosition)
			{
				//ループを抜ける
				break;
			}
		}
		//同じサウンドがみつかったので
		if (i < NUMSECONDARY)
		{
			//再生
			pDSoundSecondaryBuffer[i].Play(loop);
			pDSoundSecondaryBuffer[i].SetPlay(FALSE);
		}
		else
		{
			//同じサウンドが見つからなかったときの処理
			for (i = 0; i < NUMSECONDARY; i++)
			{
				//実行可能なセカンダリﾊﾞｯﾌｧを検索
				if (pDSoundSecondaryBuffer[i].GetPlay())
				{
					//実行可能なバッファを発見
					break;
				}
			}
			//実行可能なバッファがあったので以下の処理
			if (i < NUMSECONDARY)
			{
				//現在の再生番号を変更
				pDSoundSecondaryBuffer[i].SetNumPlay((int)setPosition);
				//
				ChangeWave(i, setPosition);
				//
				//Notify.SetNotify(pDSoundSecondaryBuffer[i], hEvent[i]);
				//
				pDSoundSecondaryBuffer[i].Play(loop);
				//
				pDSoundSecondaryBuffer[i].SetPlay(FALSE);
			}
		}
	}
}

//====================================
//関数名	:Stop
//引数		:setPosition:演奏番号
//戻り値	:
//内容		:サウンドを止める
//====================================
void CSoundManager::Stop(DWORD setPosition)
{
	DWORD i;

	if (setPosition >=NUMEFFECTSOUND)
	{
		return;
	}
	//セカンダリバッファのどこで再生されているか検索
	for (i = 0; i < NUMSECONDARY; i++)
	{
		if (pDSoundSecondaryBuffer[i].GetNumPlay() == (int)setPosition)
		{
			break;
		}
	}
		if (i < NUMSECONDARY)
		{
			pDSoundSecondaryBuffer[i].Stop();
			pDSoundSecondaryBuffer[i].SetPlay(TRUE);
		}
	
}

//====================================
//関数名	:StopAllSound
//引数		:
//戻り値	:
//内容		:全てのBGMを止める
//====================================
void CSoundManager::StopAllSound()
{
	for (DWORD i = 0; i < NUMSECONDARY; i++)
	{
		if (pDSoundSecondaryBuffer[i].GetNumPlay() != -1)
		{
			pDSoundSecondaryBuffer[i].Stop();
			pDSoundSecondaryBuffer[i].SetPlay(TRUE);
			pDSoundSecondaryBuffer[i].SetNumPlay(-1);
		}
	}
}

//====================================
//関数名	:ChangeWave
//引数		:dwDest	:格納番号
//			 dwSrc	:演奏番号
//戻り値	:TRUE:成功 失敗:FALSE
//内容		:WAVE情報をセカンダリバッファにコピー
//====================================
BOOL CSoundManager::ChangeWave(DWORD dwDest, DWORD dwSrc)
{
	if (dwDest >= NUMSECONDARY || dwSrc >= NUMEFFECTSOUND)
	{
		return FALSE;
	}

	pDSoundSecondaryBuffer[dwDest].Release();

	if (!pDSoundSecondaryBuffer[dwDest].Create(DirectSound, &pDSoundBuffer[dwSrc].GetSetWaveFormat(), pDSoundBuffer[dwSrc].GetBufferBytes()))
	{
		return FALSE;
	}

	//空バッファにバッファデータを書き込む
	LPVOID pFirstDest, pSecondDest, pFirstSrc, pSecondSrc;
	DWORD dwFirstDestBufferBytes, dwSecondDestBufferBytes;
	DWORD dwFirstSrcBufferBytes, dwSecondSrcBufferBytes;

	pDSoundSecondaryBuffer[dwDest]->Lock(
		0,
		pDSoundSecondaryBuffer[dwDest].GetBufferBytes(),
		&pFirstDest,
		&dwFirstDestBufferBytes,
		&pSecondDest,
		&dwSecondDestBufferBytes,
		0L
		);

	pDSoundBuffer[dwSrc]->Lock(
		0,
		pDSoundBuffer[dwSrc].GetBufferBytes(),
		&pFirstSrc,
		&dwFirstSrcBufferBytes,
		&pSecondSrc,
		&dwSecondSrcBufferBytes,
		0L
		);

	//プライマリバッファの中身をコピー
	memcpy(pFirstDest, pFirstSrc, dwFirstSrcBufferBytes);

	//もしあふれたブロックがあったら
	if (dwSecondDestBufferBytes)
	{
		memcpy(pSecondDest, pSecondSrc, dwSecondDestBufferBytes);
	}
	pDSoundBuffer[dwSrc]->Unlock(pFirstSrc, dwFirstSrcBufferBytes, pSecondSrc, dwSecondSrcBufferBytes);
	pDSoundSecondaryBuffer[dwDest]->Unlock(pFirstDest, dwFirstDestBufferBytes, pSecondDest, dwSecondDestBufferBytes);

	return TRUE;
}
