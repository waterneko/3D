
//=============================================================================//
// 
// Name : Wavfile.cpp
// Describe : Wavファイル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/06
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Wavfile.h"				// Wavファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SWavfile::SWavfile()
// DESC: Wavファイル処理 コンストラクタ
//-----------------------------------------------------------------------------
SWavfile::SWavfile()
{
	byDataAudio = NULL;
}

//-----------------------------------------------------------------------------
// NAME: SWavfile::~SWavfile()
// DESC: Wavファイル処理 デストラクタ
//-----------------------------------------------------------------------------
SWavfile::~SWavfile()
{
	SAFE_DELETE(byDataAudio)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SWavfile::LoadWavFile(char* path, char* name)
// DESC: Wavファイル処理 デストラクタ
//-----------------------------------------------------------------------------
HRESULT SWavfile::LoadWavFile(char* path, char* name)
{
	HRESULT hr;
	HANDLE	hFile;
	DWORD	dwChunkSize;
	DWORD	dwChunkPosition;
	DWORD	dwFiletype;

	// ファイルのパス合并処理
	char FilePath[255];
	strcpy_s(FilePath, path);
	strcat_s(FilePath, name);

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));

	// サウンドデータファイルの生成
	hFile = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// ファイルポインタを先頭に移動
		MessageBox(NULL, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// WAVEファイルのチェック
	hr = FindChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(NULL, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// フォーマットチェック
	hr = FindChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// オーディオデータ読み込み
	hr = FindChunk(hFile, 'atad', &dwSizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	byDataAudio = (BYTE*)malloc(dwSizeAudio);
	hr = ReadChunkData(hFile, byDataAudio, dwSizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT FindChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
// DESC: Wavファイル処理 チャンク情報を探す
//-----------------------------------------------------------------------------
HRESULT SWavfile::FindChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SWavfile::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
// DESC: Wavファイル処理 チャンク情報を探す
//-----------------------------------------------------------------------------
HRESULT SWavfile::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
//*****************************************************************************//
// (4) グローバル変数:
//*****************************************************************************//

//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//

//*****************************************************************************//
// (6) 関数
//*****************************************************************************//