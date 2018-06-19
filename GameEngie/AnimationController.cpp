//=============================================================================//
// 
// Name :  AnimationController.cpp
// Describe : アニメーションコントローラー処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "AnimationController.h"				// アニメーションコントローラー処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BAnimController::DAnimController(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: アニメーションコントローラー処理 コンストラクタ
//-----------------------------------------------------------------------------
BAnimController::BAnimController(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	bAnimation = true;

	iCurrentTrack = 0;
	iAnimIndex = 0;
	iAnimMaxNum = 0;
	iAnimFrame = 0;

	bAnimBlend = false;
	fShiftTime = SMOOTH_FRAMETIME;
	fCurWeightTime = 0.0f;

	pAnimController = NULL;
	pAnimSet = NULL;
	ZeroMemory(&TrackDesc[0], sizeof(TrackDesc[0]));
	ZeroMemory(&TrackDesc[1], sizeof(TrackDesc[1]));
}

//-----------------------------------------------------------------------------
// NAME: BAnimController::~BAnimController()
// DESC: アニメーションコントローラー処理 デストラクタ
//-----------------------------------------------------------------------------
BAnimController::~BAnimController()
{
	SAFE_RELEASE(pAnimController)
		//SAFE_RELEASE(pAnimSet)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT BAnimController::InitAnimController(DAnimXfile* pAnimxfile)	
// DESC: アニメーションコントローラー処理 アニメーションコントローラー初期化
//-----------------------------------------------------------------------------
HRESULT BAnimController::InitAnimController(SAnimXfile* animxfile)
{
	// アニメーションコントロールをコピーする
	animxfile->pOrigAnimController->CloneAnimationController(
		animxfile->pOrigAnimController->GetMaxNumAnimationOutputs(),
		animxfile->pOrigAnimController->GetNumAnimationSets(),
		animxfile->pOrigAnimController->GetMaxNumTracks(),
		animxfile->pOrigAnimController->GetMaxNumEvents(),
		&pAnimController);

	iAnimMaxNum = animxfile->pOrigAnimController->GetNumAnimationSets();

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BAnimController::UpdateAnimController(FLOAT time)
// DESC: アニメーションコントローラー処理 アニメーション更新
//-----------------------------------------------------------------------------
VOID BAnimController::UpdateAnimController(FLOAT time)
{
	// 合成中か否かを判定
	if (bAnimBlend)
	{
		fCurWeightTime += time;
		if (fCurWeightTime <= fShiftTime)
		{
			// 合成中。ウェイトを算出
			FLOAT Weight = fCurWeightTime / fShiftTime;
			// ウェイトを登録
			pAnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
			pAnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
		}
		else
		{
			// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
			pAnimController->SetTrackWeight(0, 1.0f);       // 現在のアニメーション
			pAnimController->SetTrackEnable(1, false);      // 前のアニメーションを無効にする
			fCurWeightTime = 0.0f;
			bAnimBlend = false;
		}
	}

	// 時間を更新
	if (bAnimation)
	{
		pAnimController->AdvanceTime(time, NULL);
	}
	else
	{
		pAnimController->AdvanceTime(0, NULL);
	}

	// トラック情報更新
	pAnimController->GetTrackDesc(0, &TrackDesc[0]);
	pAnimController->GetTrackDesc(1, &TrackDesc[1]);

	// 時間更新(4800フレームを1秒，60/4800 = 0.0125)
	iAnimFrame++;
}

//-----------------------------------------------------------------------------
// NAME: VOID BAnimController::ChangeAnimation(bool Smooth, int aniIndex)	
// DESC: アニメーションコントローラー処理 アニメーション切り替え
//-----------------------------------------------------------------------------
VOID BAnimController::ChangeAnimation(bool Smooth, int aniIndex)
{
	if (iAnimIndex != aniIndex)
	{
		int iNewTrack = 1;

		// アニメーションインデックス番号処理
		if (aniIndex > iAnimMaxNum - 1)
		{
			aniIndex = 0;
		}
		else if (aniIndex < 0)
		{
			aniIndex = iAnimMaxNum - 1;
		}

		if (Smooth)
		{
			// <1.現在のトラック処理>
			// 現在のトラック情報を取得する
			D3DXTRACK_DESC TD;
			pAnimController->GetTrackDesc(iCurrentTrack, &TD);

			// 現在のアニメーションを取得し、そして取得したアニメーションをトラック1に移行
			if (SetAnimationToTrack(iAnimIndex, iNewTrack))
			{
				// 現在のトラックの設定値をトラック1に移行
				pAnimController->SetTrackDesc(iNewTrack, &TD);
			}

			// <2.新しいトラック処理>
			// 新しいアニメーションを取得し、そして取得したアニメーションをトラック0に移行
			if (SetAnimationToTrack(aniIndex, iCurrentTrack))
			{
				// トラック0のポジションの設定
				pAnimController->SetTrackPosition(iCurrentTrack, 0.0f);
				// トラック0のスピードの設定
				pAnimController->SetTrackSpeed(iCurrentTrack, 1.0f);
			}

			// トラックの合成を許可(有効のトラックを自動的に合成)
			pAnimController->SetTrackEnable(iCurrentTrack, true);
			pAnimController->SetTrackEnable(iNewTrack, true);

			bAnimBlend = true;
		}
		else
		{
			// <1.新しいトラック処理>
			// 新しいアニメーションを取得し、そして取得したアニメーションをトラック0に移行
			if (SetAnimationToTrack(aniIndex, iCurrentTrack))
			{
				// トラック0のポジションの設定
				pAnimController->SetTrackPosition(iCurrentTrack, 0.0f);
				// トラック0のウェイトの設定
				pAnimController->SetTrackWeight(iCurrentTrack, 1.0f);
				// トラック0のスピードの設定
				pAnimController->SetTrackSpeed(iCurrentTrack, 1.0f);
			}

			// トラックの合成を禁止
			pAnimController->SetTrackEnable(iCurrentTrack, true);
			pAnimController->SetTrackEnable(iNewTrack, false);

			bAnimBlend = false;

		}
		// ウェイト時間を初期化
		fCurWeightTime = 0.0f;

		// 現在のアニメーション番号を切り替え
		iAnimIndex = aniIndex;

		// 時間リセット
		pAnimController->ResetTime();
		iAnimFrame = 0;
	}
}

//-----------------------------------------------------------------------------
// NAME: DWORD BAnimController::GetAnimIndexByName(char sString[])
// DESC: スキンメッシュ処理 名でアニメーションセットを取得する
//-----------------------------------------------------------------------------
DWORD BAnimController::GetAnimIndexByName(char sString[])
{
	HRESULT hr;
	LPD3DXANIMATIONSET pAS;
	DWORD dwRet = -1;

	for (DWORD i = 0; i < pAnimController->GetNumAnimationSets(); ++i)
	{
		hr = pAnimController->GetAnimationSet(i, &pAS);
		if (FAILED(hr))
			continue;

		if (pAS->GetName() &&
			!strncmp(pAS->GetName(), sString, min(strlen(pAS->GetName()), strlen(sString))))
		{
			dwRet = i;
			pAS->Release();
			break;
		}
		pAS->Release();
	}
	return dwRet;
}

//-----------------------------------------------------------------------------
// NAME: BOOL BAnimController::SetAnimationToTrack(int aniInx, int track)
// DESC: スキンメッシュ処理 アニメーショントラックセット処理
//-----------------------------------------------------------------------------
BOOL BAnimController::SetAnimationToTrack(int aniInx, int track)
{
	HRESULT hr;

	// アニメーションコントローラーから指定のアニメーションを取得する
	hr = pAnimController->GetAnimationSet(aniInx, &pAnimSet);
	if (FAILED(hr))
		return false;
	// 取得したアニメーションを指定のトラックに移行する
	hr = pAnimController->SetTrackAnimationSet(track, pAnimSet);
	if (FAILED(hr))
		return false;
	// アニメーションセット解放
	pAnimSet->Release();

	return true;
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