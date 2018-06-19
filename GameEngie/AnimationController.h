
//=============================================================================//
// 
// Name : AnimationController.h
// Describe : アニメーションコントローラー処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _ANIMATIONCONTROLLER_H_
#define _ANIMATIONCONTROLLER_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "AnimXfile.h"				// アニメーションxファイル処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define AMIMATION_FRAMETIME 	( 0.0125f )		// 毎フレームアニメーション時間( 60フレーム / AnimTicksPerSecond )
#define SMOOTH_FRAMETIME		( 0.1f )		// 滑らかなアニメーション時間
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }
// アニメーションコントローラークラス
class BAnimController
{
private:
	LPDIRECT3DDEVICE9			pDevice;					// デバイスオブジェクト

	LPD3DXANIMATIONCONTROLLER	pAnimController;			// アニメーションコントローラー
	LPD3DXANIMATIONSET	 		pAnimSet;					// アニメーションセット
	D3DXTRACK_DESC  			TrackDesc[2];				// アニメーショントラック情報

	int							iCurrentTrack;				// 今のトラック番号
	int							iAnimIndex;					// アニメーションセット番号
	int							iAnimMaxNum;				// アニメーションセット総数
	int							iAnimFrame;					// アニメーションフレーム時間

	float						fShiftTime;					// シフトするのにかかる時間
	float						fCurWeightTime;				// アニメーション合成を使用した時間
public:
	bool						bAnimBlend;					// 合成フラグ
	bool						bAnimation;					// アニメーションフラグ
public:
	BAnimController(LPDIRECT3DDEVICE9 D3dDevice);			// コンストラクタ
	~BAnimController();										// デストラクタ
public:
	// [アニメーション]設置
	HRESULT InitAnimController(SAnimXfile* pAnimxfile);		// アニメーションコントローラー初期化
	VOID UpdateAnimController(FLOAT time);					// アニメーション更新
	VOID ChangeAnimation(bool Smooth, int aniIndex);		// アニメーション切り替え

	// [取得関数]
	UINT GetAnimIndex() { return iAnimIndex; };									// 今のアニメーション番号を取得する
	DWORD GetAnimIndexByName(char sString[]);									// 名でアニメーション番号を取得する

	int GetAnimFrame() { return iAnimFrame; }; 									// アニメーションフレーム数を取得する
	int GetCurrentTrack() { return iCurrentTrack; };							// 今のトラック番号を取得する
	DOUBLE GetAnimCurrentTime() { return pAnimController->GetTime(); };	// 今のアニメーション時間を取得する

	LPD3DXANIMATIONCONTROLLER GetAnimController() { return pAnimController; };	// アニメーションを取得する
	D3DXTRACK_DESC GetTrackInfo(int n) { return TrackDesc[n]; };				// アニメーショントラックを取得する
private:
	BOOL SetAnimationToTrack(int aniInx, int track);							// アニメーショントラックセット処理
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif