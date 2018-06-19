
//=============================================================================//
// 
// Name : ScreenGauge.h
// Describe : スクリーンゲージ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SCREENGAUGE_H_
#define _SCREENGAUGE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "ScreenTexture.h"			// スクリーンテクスチャ処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_GAUGETEXTURE		(100)						// 最大スクリーンゲージ数

#define	PATH_GAUGEMASK	("Data/Texture/Other/Gauge/GauguMask.png")
#define	PATH_GAUGE		("Data/Texture/Other/Gauge/GauguTexture.png")

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }
// スクリーンゲージクラス
class GScreenGauge
{
private:
	LPDIRECT3DDEVICE9			pDevice; 				// デバイスオブジェクト

	int							iAlpha;					// アルファ値

	float						fMaxValue;				// 最大値
	float						fCurrentValue;			// 当前値

	D3DXVECTOR2					vSize;					// サイズ
	D3DXVECTOR2					vScreenPosition;		// スクリーン座標

	LPDIRECT3DTEXTURE9			pGaugeTexture;			// ゲージテクスチャ
	LPDIRECT3DTEXTURE9			pMaskTexture;			// 下のテクスチャ

	BScreenTexture*				pMask;					// ゲージのテクスチャクラス
	BScreenTexture*				pGauge;					// 下のテクスチャクラス
public:
	bool						bUpdate;				// 更新フラグ
	bool						bRender;				// 描画フラグ

	bool						bGaugeMove;				// ゲージ移動フラグ
	float						fTargetValue;			// 目標値
public:
	GScreenGauge(LPDIRECT3DDEVICE9 D3dDevice);			// コンストラクタ
	~GScreenGauge();									// デストラクタ
public:
	HRESULT InitGaugeTexture(D3DXVECTOR2 size, D3DXVECTOR2 position, UINT type);	// スクリーンゲージ初期化
	VOID UpdateGaugeTexture(float percent);												// スクリーンゲージ更新
	VOID RenderGaugeTexture();														// スクリーンゲージ描画

	VOID InitGauge(bool max, float maxvalue);										// ゲージ初期化
	VOID GaugeMove(float percent);													// ゲージ移動

	VOID SetMaxValue(float value);													// ゲージ最大値設定
	VOID SetCurrentValue(float value);												// ゲージ当前値設定
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif