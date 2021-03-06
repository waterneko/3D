
//=============================================================================//
// 
// Name : Sound.h
// Describe : サウンド処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }
// ソースボイス構造体
struct SourceVoice
{
	XAUDIO2_BUFFER			XA2buffer;				// XAUDIO2バッファ
	IXAudio2SourceVoice*	pSourceVoice = NULL;	// ソースボイス
};

// { クラス }

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//
HRESULT InitSound();						// サウンド初期化
VOID UninitSound();							// サウンド解放処理
IXAudio2* GetXaudio2();						// XAudio2オブジェクトへのポインターを取得する
IXAudio2MasteringVoice*	GetMasterVoice();	// マスターボイスを取得する

#endif