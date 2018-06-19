
//=============================================================================//
// 
// Name : Wavfile.h
// Describe : Wavファイル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _WAVFILE_H_
#define _WAVFILE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_WAVFILE			(50)						// 最大Wavファイル数
#define	PATH_BGM_WAV			("Data/Sound/BGM/")			// WAVファイルパスBGM
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }
// wavファイルクラス
class SWavfile
{
public:
	WAVEFORMATEXTENSIBLE	wfx;							// フォーマット
	BYTE*					byDataAudio;					// オーディオデータ
	DWORD					dwSizeAudio;					// オーディオデータサイズ
public:
	SWavfile();												// コンストラクタ
	~SWavfile();											// デストラクタ
public:
	HRESULT LoadWavFile(char* path, char* name);
private:
	HRESULT FindChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);		// チャンク情報を探す
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);		// チャンクデータの読込み
};
//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif