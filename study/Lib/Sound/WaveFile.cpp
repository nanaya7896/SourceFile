#include "WaveFile.h"

//====================================
//関数名	:CWaveFile
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CWaveFile::CWaveFile()
{
	m_hmmio = NULL;
	m_pwfx = NULL;
}
//====================================
//関数名	:~CWaveFile
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CWaveFile::~CWaveFile()
{
	Close();
}

//====================================
//関数名	:Open
//引数		:pFileName:ファイル名
//戻り値	:TRUE:成功、FALSE:失敗
//内容		:Open
//====================================
BOOL CWaveFile::Open(char* pFileName)
{
	MMCKINFO		ckIn;
	PCMWAVEFORMAT	pcmWaveFormat;

	m_hmmio = mmioOpen(pFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (m_hmmio == NULL)
		return FALSE;

	// 入力ファイルを'RIFF'チャンクへ移動
	if (mmioDescend(m_hmmio, &m_ckRiff, NULL, 0) != 0)
		return FALSE;

	// 入力ファイルがWAVEファイルであるか確認
	if ((m_ckRiff.ckid != FOURCC_RIFF)
		|| (m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return FALSE;

	// 入力ファイル内で'fmt'チャンクを検索
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK) != 0)
		return FALSE;

	// 'fmt'チャンクのサイズが少なくとも<PCMWAVEFORMAT>
	// 同程度のサイズであることを想定
	if (ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT))
		return FALSE;

	// <wfex>に'fmt'チャンクを読み取る
	if (mmioRead(m_hmmio, (HPSTR)&pcmWaveFormat,
		sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
		return FALSE;

	// フォーマットが'PCM'の場合
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM) {
		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
		if (NULL == m_pwfx)
			return FALSE;
		// ウェーブフォーマットのコピー
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = 0;		// プライマリバッファは0
	}
	else {
		// フォーマットが'PCM'以外の場合
		WORD cbExtraBytes = 0L;
		if (mmioRead(m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return FALSE;
		m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX) + cbExtraBytes];
		if (NULL == m_pwfx)
			return FALSE;
		// ウェーブフォーマットのコピー
		memcpy(m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat));
		m_pwfx->cbSize = cbExtraBytes;
		// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
		if (mmioRead(m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize)) + sizeof(WORD)),
			cbExtraBytes) != cbExtraBytes) {
			SAFE_DELETE(m_pwfx);
			return FALSE;
		}
	}
	// 'fmt'チャンクの外へ入力ファイルを移動
	if (mmioAscend(m_hmmio, &ckIn, 0) != 0) {
		SAFE_DELETE(m_pwfx);
		return FALSE;
	}

	return TRUE;
}


//====================================
//関数名	:StartRead
//引数		:
//戻り値	:TRUE:成功、FALSE:失敗
//内容		:読み込み場所をdataに移動
//====================================
BOOL CWaveFile::StartRead()
{
	if (mmioSeek(m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)
		return FALSE;

	// 入力ファイル内で'data'チャンクを検索
	m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (0 != mmioDescend(m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK))
		return FALSE;	// データチャンク無し

	return TRUE;
}

//====================================
//関数名	:Read
//引数		:pBuffer					格納バッファ
//			;dwSizeToRead				読み込むサイズ
//			:pdwSizeRead				読み込んだサイズ格納用アドレス
//戻り値	:TRUE:成功、FALSE:失敗
//内容		:情報の読み込み
//====================================
BOOL CWaveFile::Read(LPVOID pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead)
{
	*pdwSizeRead = 0;
	MMIOINFO	mmioinfoIn; // current status of m_hmmio

							// 入力バッファの直接アクセスするため、ファイル情報取得
	if (mmioGetInfo(m_hmmio, &mmioinfoIn, 0) != 0)
		return FALSE;

	UINT cbDataIn = dwSizeToRead;
	if (cbDataIn > m_ck.cksize)
		cbDataIn = m_ck.cksize;

	m_ck.cksize -= cbDataIn;

	for (DWORD cT = 0; cT < cbDataIn; cT++) {
		// バッファの終端に達したら、バッファにデータを読み込む
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead) {
			if ((mmioAdvance(m_hmmio, &mmioinfoIn, MMIO_READ) != 0)
				|| (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead))
				return FALSE;
		}

		// Actual copy.
		*((BYTE*)pBuffer + cT) = *((BYTE*)mmioinfoIn.pchNext);
		mmioinfoIn.pchNext++;
	}

	// バッファの状態を更新し、次の入力に備える
	if (mmioSetInfo(m_hmmio, &mmioinfoIn, 0) != 0)
		return FALSE;

	if (pdwSizeRead != NULL)
		*pdwSizeRead = cbDataIn;

	return TRUE;
}

//====================================
//関数名	:Close
//引数		:
//戻り値	:TRUE:成功、FALSE:失敗
//内容		:ファイルを閉じる
//====================================
BOOL CWaveFile::Close()
{
	delete[](char*)m_pwfx;

	if (m_hmmio != NULL) {
		if (FAILED(mmioClose(m_hmmio, 0)))
			return FALSE;
	}

	return TRUE;
}

