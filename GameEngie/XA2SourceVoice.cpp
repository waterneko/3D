
//=============================================================================//
// 
// Name : XA2SourceVoice.cpp
// Describe : XAudio2ソースボイス処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "XA2SourceVoice.h"				// XAudio2ソースボイス処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BXA2SourceVoice::BXA2SourceVoice()
// DESC: XAudio2ソースボイス処理 コンストラクタ
//-----------------------------------------------------------------------------
BXA2SourceVoice::BXA2SourceVoice()
{
	bLoop = true;

	pSourceVoice = NULL;
}

//-----------------------------------------------------------------------------
// NAME: BXA2SourceVoice::~BXA2SourceVoice()
// DESC: XAudio2ソースボイス処理 デストラクタ
//-----------------------------------------------------------------------------
BXA2SourceVoice::~BXA2SourceVoice()
{
	if (pSourceVoice)
	{
		// 一時停止
		pSourceVoice->Stop(0);

		// ソースボイスの破棄
		pSourceVoice->DestroyVoice();
		pSourceVoice = NULL;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT InitSourceVoice(SWavfile* wavfile, bool loop)
// DESC: XAudio2ソースボイス処理 ソースボイスの初期化
//-----------------------------------------------------------------------------
HRESULT BXA2SourceVoice::InitSourceVoice(SWavfile* wavfile, bool loop)
{
	HRESULT hr;

	if (GetMasterVoice() != NULL)
	{
		bLoop = loop;

		// ソースボイスの生成
		hr = GetXaudio2()->CreateSourceVoice(&pSourceVoice, &(wavfile->wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(NULL, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}

		memset(&XA2buffer, 0, sizeof(XAUDIO2_BUFFER));
		XA2buffer.Flags = XAUDIO2_END_OF_STREAM;			// 再生終わったら次のバッファ再生しない
		XA2buffer.AudioBytes = wavfile->dwSizeAudio;		// オーディオ データのサイズ
		XA2buffer.pAudioData = wavfile->byDataAudio;		// オーディオ データへのポインター
		XA2buffer.PlayBegin = 0;							// 再生始動位置
		XA2buffer.PlayLength = 0;							// 再生される領域の長さ　0 = 全部再生
		XA2buffer.LoopBegin = 0;							// ループ始動位置 LoopBegin < PlayBegin + PlayLength
		XA2buffer.LoopLength = 0;							// ループ領域の長さ PlayBegin < LoopBegin + LoopLength < PlayBegin + PlayLength

		// ループ回数 0 = ループしない ，XAUDIO2_LOOP_INFINITE = 無限ループ
		if (bLoop)
		{
			XA2buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else
		{
			XA2buffer.LoopCount = 0;
		}

		// オーディオバッファの登録
		pSourceVoice->SubmitSourceBuffer(&XA2buffer);
	}
	else
	{
		MessageBox(NULL, "マスターボイス未生成！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::PlaySourceVoice()
// DESC: XAudio2ソースボイス処理 セグメント再生
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::PlaySourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	pSourceVoice->GetState(&xa2state);

	// 再生
	pSourceVoice->Start(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::RestartSourceVoice()
// DESC: XAudio2ソースボイス処理 セグメント初期再生
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::RestartSourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	pSourceVoice->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{
		// 一時停止
		pSourceVoice->Stop(0);

		// オーディオバッファの削除
		pSourceVoice->FlushSourceBuffers();

		// オーディオバッファの再登録
		pSourceVoice->SubmitSourceBuffer(&XA2buffer);
	}

	// 再生
	pSourceVoice->Start(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::PauseSourceVoice()
// DESC: XAudio2ソースボイス処理 セグメント一時停止
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::PauseSourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	pSourceVoice->GetState(&xa2state);

	// 再生中 
	if (xa2state.BuffersQueued != 0)
	{
		// 一時停止
		pSourceVoice->Stop(0);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::PauseSourceVoice()
// DESC: XAudio2ソースボイス処理 セグメント停止
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::StopSourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	pSourceVoice->GetState(&xa2state);

	// 再生中 
	if (xa2state.BuffersQueued != 0)
	{
		// 一時停止
		pSourceVoice->Stop(0);

		// オーディオバッファの削除
		pSourceVoice->FlushSourceBuffers();
	}
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