#include "DirectSound.h"

//====================================
//関数名	:CDirectSound
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CDirectSound::CDirectSound()
{
	m_pDSound = 0;
}

//====================================
//関数名	:~CDirectSound
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CDirectSound::~CDirectSound()
{
	RelaseObject();
}

//====================================
//関数名	:Create
//引数		:hWnd:ウィンドウハンドル
//			 freq:周波数
//戻り値	:TRUE:成功 FALSE:失敗
//内容		:サウンドインターフェイスの作成
//====================================
BOOL CDirectSound::Create(HWND hWnd, int freq)
{
	HRESULT hr;
	//DirectSoundインタフェースの取得
	if (FAILED(hr = DirectSoundCreate(NULL, &m_pDSound, NULL)))
	{
		return FALSE;
	}
	//=====協調レベルの設定=====//
	//DSSCL_NORMAL		:標準協調レベル
	//DSSCL_PRIORITY	:優先協調レベル
	//DSSCL_WRITEPRIMARY:書き込み優先協調レベル
	if (FAILED(hr = m_pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return FALSE;
	}


	//プライマリーバッファの作成
	return Primary.CreatePrimary(*this, freq);

	//return TRUE;
}

//====================================
//関数名	:RelaseObject
//引数		:
//戻り値	:
//内容		:サウンドインターフェイスの解放
//====================================
void CDirectSound::RelaseObject()
{
	//もしサウンドインタフェースが存在したら
	if (m_pDSound)
	{
		Primary.Release();
		m_pDSound->Release();
		m_pDSound = NULL;
	}
}

//====================================
//関数名	:SpeakerConfig
//引数		:hr:HRESULT型
//戻り値	:
//内容		:スピーカーを調査
//====================================
DWORD CDirectSound::SpeakerConfig(HRESULT hr)
{
	//=====スピーカーの識別=====//
	DWORD dwSpeakerConfig;
	hr = m_pDSound->GetSpeakerConfig(&dwSpeakerConfig);
	//成功した場合
	if (SUCCEEDED(hr))
	{
		switch ((DSSPEAKER_CONFIG(dwSpeakerConfig)))
		{
		case DSSPEAKER_5POINT1_BACK:
			//サブウーファー付きの5サラウンド・スピーカー
			break;
		case DSSPEAKER_5POINT1_SURROUND:
			//ホームシアター向けのサブウーファー付き５サラウンド・スピーカー
			break;
		case DSSPEAKER_DIRECTOUT:
			//スピーカー用の構成は行われない
			break;
		case DSSPEAKER_HEADPHONE:
			//ヘッドホン
			break;
		case DSSPEAKER_MONO:
			//モノラルスピーカー
			break;
		case DSSPEAKER_QUAD:
			//4チャンネルスピーカー
			break;
		case DSSPEAKER_STEREO:
			//ステレオスピーカー
			//ジオメトリ構成(２つのスピーカーの配置)
			switch (DSSPEAKER_GEOMETRY(dwSpeakerConfig))
			{
			case DSSPEAKER_GEOMETRY_WIDE:
				//リスナーを中心として20度の角度
				break;
			case DSSPEAKER_GEOMETRY_NARROW:
				//リスナーを中心として10度の角度
				break;
			case DSSPEAKER_GEOMETRY_MIN:
				//リスナーを中心として5度の角度
				break;
			case DSSPEAKER_GEOMETRY_MAX:
				//リスナーを中心として180度の角度
				break;
			}
			break;
		case DSSPEAKER_SURROUND:
			//サラウンドスピーカー
			break;
		case DSSPEAKER_7POINT1_WIDE:
			//サブウーファー付きの7サラウンドスピーカー
			break;
		case DSSPEAKER_7POINT1_SURROUND:
			//ホームシアター向けのサブウーファー付き７サラウンドスピーカー
			break;
		}
	}
	return dwSpeakerConfig;
}

//======================================CSoundBufferクラス======================================//

//====================================
//関数名	:CSoundBuffer
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CSoundBuffer::CSoundBuffer()
{
	buffer = NULL;
	Volume = 0;
	Pan = 0;
	Frequency = 0;
	bPlay = TRUE;
	NumPlay = -1;
}

//====================================
//関数名	:~CSoundBuffer
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CSoundBuffer::~CSoundBuffer()
{
	if (buffer)
	{
		buffer->Release();
	}

}

//====================================
//関数名	:Create
//引数		:dSound			:ダイレクトサウンドのアドレス
//			 pFormat		:WAVEFORMATEX構造体
//			 dwBufferBytes	:バッファサイズ
//戻り値	:TRUE:成功 FALSE:失敗
//内容		:サウンドバッファの作成
//====================================
BOOL CSoundBuffer::Create(CDirectSound& dSound, WAVEFORMATEX *pFormat, DWORD dwBufferBytes)
{
	HRESULT hr;
	DSBUFFERDESC dsbd;

	//DSBUFFERDESC構造体の設定
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags =
		DSBCAPS_GETCURRENTPOSITION2 |
		DSBCAPS_GLOBALFOCUS |
		DSBCAPS_LOCDEFER |
		DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLFREQUENCY |
		DSBCAPS_CTRLPOSITIONNOTIFY;
	dsbd.dwBufferBytes = dwBufferBytes;
	dsbd.lpwfxFormat = pFormat;
	//情報のバックアップ
	BufferBytes = dwBufferBytes;
	memcpy(&Wfx,pFormat,sizeof(WAVEFORMATEX));
	
	//サウンドバッファの作成
	if (FAILED(hr = dSound->CreateSoundBuffer(&dsbd, &buffer, NULL)))
	{
		return FALSE;
	}

	//情報の取得
	buffer->GetVolume(&Volume);
	buffer->GetPan(&Pan);
	buffer->GetFrequency(&Frequency);


	return TRUE;
}

//====================================
//関数名	:CreatePrimary
//引数		:dSound			:ダイレクトサウンドのアドレス
//			 dwPrimaryFreq	:周波数
//戻り値	:TRUE:成功 FALSE:失敗
//内容		:プライマリバッファの作成
//====================================
BOOL CSoundBuffer::CreatePrimary(CDirectSound &dSound, DWORD dwPrimaryFreq)
{
	HRESULT hr;

	//DSBUFFERDESC構造体の設定
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags =
		DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLPAN |
		DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	//サウンドバッファの作成
	if (FAILED(hr = dSound->CreateSoundBuffer(&dsbd, &buffer, NULL)))
	{
		return FALSE;
	}

	//WAVEフォーマットの設定
	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	//プライマリバッファのWaveフォーマットを設定
	//優先協調レベル以上の協調レベルが設定されている必要がある。
	//WAVE_FORMAT_PCM:１チャンネルPCMまたは2チャンネルPCM
	//WAVE_FORMAT_EXTENSIBLE:WAVEFORMATEXTENSIBLEを使用
	//
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	//チャンネル数 1:モノラル2:ステレオ
	//2チャンネル(ステレオ)
	wfx.nChannels = 2;
	//11.025kHz:11025 22.05kHz:22050(ヘルツ単位で指定)
	//サンプリングレート
	wfx.nSamplesPerSec = dwPrimaryFreq;
	
	//1サンプル当たりのビット数
	//WAVE_FORMAT_PCMの場合:｢8｣または｢16｣
	//16ビット
	wfx.wBitsPerSample = 16;

	//ブロックアライメント
	//WAVE_FORMAT_PCMの場合:nChannels*wBitsPerSample/8に等しい
	wfx.nBlockAlign = 4;

	//1秒間に転送するバイト数
	//WAVE_FORMAT_PCMの場合:nSamplesPerSec*nBlockAlignに等しい
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if (FAILED(hr = buffer->SetFormat(&wfx)))
	{
		//設定に失敗
		return FALSE;
	}

	buffer->GetVolume(&Volume);
	buffer->GetPan(&Pan);
	buffer->GetFrequency(&Frequency);

	return TRUE;
}

//====================================
//関数名	:Release
//引数		:
//戻り値	:
//内容		:バッファの解放
//====================================
HRESULT CSoundBuffer::Release()
{

	if (buffer)
	{
		HRESULT hr;

		hr= buffer->Release();
	}
	buffer = NULL;

	return S_OK;
}

//====================================
//関数名	:Restore
//引数		:
//戻り値	:
//内容		:バッファの復元
//====================================
HRESULT CSoundBuffer::Restore()
{
	HRESULT hr;
	if (buffer)
	{
		hr= buffer->Restore();
	}

	return S_OK;
}

//====================================
//関数名	:LoadWave
//引数		:dSound:ダイレクトサウンドのアドレス
//			 pFileName:サウンドファイル名
//戻り値	:TRUE:読み込み成功 FALSE:失敗
//内容		:Waveファイルの読み込み
//====================================
BOOL CSoundBuffer::LoadWave(CDirectSound &dSound, char *pFileName)
{
	CWaveFile file;
	
	//もしファイルが開けない、
	if (!file.Open(pFileName) || !file.StartRead())
	{
		return FALSE;
	}

	//バッファがあればリリース
	Release();

	//新しくWAVEサイズのバッファで作成
	if (!Create(dSound, file.GetFormat(), file.CkSize()))
	{
		return FALSE;
	}

	//バッファにWAVEデータに書き込む
	LPVOID pFirstDest, pSecondDest;
	DWORD dwFirstDestBufferBytes, dwSecondDestBufferBytes;

	HRESULT hr = buffer->Lock(
		0,							//オフセット値をバイト単位
		file.CkSize(),				//ロックされるバッファ部分のサイズをバイト単位
		&pFirstDest,				//最初にロックされた部分へのポインタ
		&dwFirstDestBufferBytes,	//ブロックのバイト数
		&pSecondDest,				//あふれたブロックのロックされた部分へのポインタ
		&dwSecondDestBufferBytes,	//あふれたブロックのバイト数
		0L
		);

	if (hr == DSERR_BUFFERLOST)
	{
		//デバイスの復元
		Restore();
		hr = buffer->Lock(
			0,							// オフセット値をバイト単位
			file.CkSize(),				// ロックされるバッファ部分のサイズをバイト単位
			&pFirstDest,				// 最初にロックされた部分へのポインタ
			&dwFirstDestBufferBytes,	// ブロックのバイト数
			&pSecondDest,				// 溢れたブロックのロックされた部分へのポインタ
			&dwSecondDestBufferBytes,	// 溢れたブロックのバイト数
			0L);
	}

	//成功した場合
	if (SUCCEEDED(hr))
	{
		BOOL ReaderErr = FALSE;
		DWORD dwSizeRead;

		if (!file.Read((LPBYTE)pFirstDest, dwFirstDestBufferBytes, &dwSizeRead) || dwFirstDestBufferBytes != dwSizeRead)
		{
			ReaderErr = TRUE;
		}

		if (dwSecondDestBufferBytes)
		{
			if (!file.Read((LPBYTE)pSecondDest, dwSecondDestBufferBytes, &dwSizeRead) || dwSecondDestBufferBytes != dwSizeRead)
			{
				ReaderErr = TRUE;
			}
		}
		//ロックは早めに解除する
		hr = buffer->Unlock(
			pFirstDest,
			dwFirstDestBufferBytes,
			pSecondDest,
			dwSecondDestBufferBytes
			);

		if (ReaderErr)
		{
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

//======================================CSoundBufferNotifyクラス======================================//

//====================================
//関数名	:CSoundBufferNotify
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CSoundBufferNotify::CSoundBufferNotify()
{
	m_pDSNotify = NULL;
}

//====================================
//関数名	:~CSoundBufferNotify
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CSoundBufferNotify::~CSoundBufferNotify()
{
	
}

//====================================
//関数名	:SetNotify
//引数		:buffer:サウンドバッファ
//			 hEvent:演奏終了時に発生するイベントハンドル
//戻り値	:TRUE:成功 FALSE:失敗
//内容		:演奏終了通知イベントセット
//====================================
BOOL CSoundBufferNotify::SetNotify(CSoundBuffer &buffer, HANDLE hEvent )
{
	HRESULT hr;

	//インターフェイスを指すポインタを取得する
	hr = buffer->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&m_pDSNotify);
	if (FAILED(hr))
	{
		//インターフェイス取得に失敗
		return FALSE;
	}

	//
	DSBPOSITIONNOTIFY PositionNotify;

	//再生終了を指定
	PositionNotify.dwOffset = DSBPN_OFFSETSTOP;
	PositionNotify.hEventNotify = hEvent;
	hr = m_pDSNotify->SetNotificationPositions(
		1,				//DSBPOSITIONNOTIFY構造体の数
		&PositionNotify	//DSBPOSITIONNOTIFY構造体のアドレス
		);

	SAFE_RELEASE(m_pDSNotify);
	if (hr == S_OK)
	{
		return TRUE;
	}
	return FALSE;
}
