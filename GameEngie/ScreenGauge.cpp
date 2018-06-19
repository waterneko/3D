
//=============================================================================//
// 
// Name : ScreenGauge.cpp
// Describe : スクリーンゲージ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "ScreenGauge.h"				// スクリーンゲージ処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: GScreenGauge::GScreenGauge(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: スクリーンゲージ処理 コンストラクタ
//-----------------------------------------------------------------------------
GScreenGauge::GScreenGauge(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	bUpdate = true;
	bRender = true;
	bGaugeMove = true;

	fMaxValue = 0.0f;
	fCurrentValue = 0.0f;
	fTargetValue = 0.0f;

	iAlpha = 255;
	vSize = D3DXVECTOR2(0.0f, 0.0f);
	vScreenPosition = D3DXVECTOR2(0.0f, 0.0f);

	pGaugeTexture = NULL;
	pMaskTexture = NULL;
	// {スクリーンテクスチャ}
	pMask = new BScreenTexture(pDevice);
	pGauge = new BScreenTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: GScreenGauge::~GScreenGauge()
// DESC: スクリーンゲージ処理 デストラクタ
//-----------------------------------------------------------------------------
GScreenGauge::~GScreenGauge()
{
	// {スクリーンテクスチャの解放}
	SAFE_DELETE(pMask)
	SAFE_DELETE(pGauge)

	SAFE_RELEASE(pGaugeTexture)
	SAFE_RELEASE(pMaskTexture)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GScreenGauge::InitGaugeTexture(D3DXVECTOR2 size, D3DXVECTOR2 position, UINT type)
// DESC: スクリーンゲージ処理 スクリーンゲージ初期化
//-----------------------------------------------------------------------------
HRESULT GScreenGauge::InitGaugeTexture(D3DXVECTOR2 size, D3DXVECTOR2 position, UINT type)
{
	switch (type)
	{
	case 0:
		D3DXCreateTextureFromFile(pDevice, PATH_GAUGE, &pGaugeTexture);
		D3DXCreateTextureFromFile(pDevice, PATH_GAUGEMASK, &pMaskTexture);
		break;
	case 1:
		break;
	}

	vSize = size;
	vScreenPosition = position;

	// {スクリーンテクスチャ初期化}
	pMask->InitScreenTexture(vSize, vScreenPosition);
	pGauge->InitScreenTexture(vSize, vScreenPosition);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::UpdateGaugeTexture(float percent)
// DESC: スクリーンゲージ処理 スクリーンゲージ更新
//-----------------------------------------------------------------------------
VOID GScreenGauge::UpdateGaugeTexture(float percent)
{
	if (bUpdate)
	{
		// アルファ値更新
		pMask->SetAlpha(iAlpha);
		pGauge->SetAlpha(iAlpha);

		pMask->UpdateAlpha();
		pGauge->UpdateAlpha();

		// 目標値チェック
		if (fTargetValue < 0.0f)
		{
			fTargetValue = 0.0f;
		}
		else if (fTargetValue > fMaxValue)
		{
			fTargetValue = fMaxValue;
		}

		// ゲージ移動
		if (bGaugeMove)
		{
			// ゲージ増加
			if (fCurrentValue < fTargetValue)
			{
				if (fCurrentValue + fMaxValue * (percent / 100.0f) <= fTargetValue)
				{
					GaugeMove(percent);
				}
			}
			// ゲージ減少
			else if (fCurrentValue > fTargetValue)
			{
				if (fCurrentValue - fMaxValue * (percent / 100.0f) >= fTargetValue)
				{
					GaugeMove(-percent);
				}
			}

		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::RenderGaugeTexture()
// DESC: スクリーンゲージ処理 スクリーンゲージ描画
//-----------------------------------------------------------------------------
VOID GScreenGauge::RenderGaugeTexture()
{
	if (bRender)
	{
		pMask->RenderScreenTexture(pMaskTexture);
		pGauge->RenderScreenTexture(pGaugeTexture);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::InitGauge(bool max, float maxvalue)
// DESC: スクリーンゲージ処理 ゲージ初期化
//-----------------------------------------------------------------------------
VOID GScreenGauge::InitGauge(bool max, float maxvalue)
{
	// ゲージ最大値設定
	SetMaxValue(maxvalue);

	if (!max)
	{
		// ゲージ値ゼロ処理
		fCurrentValue = 0.0f;
		fTargetValue = 0.0f;

		// 頂点移動(ゲージゼロ)
		pGauge->SetVertexPosition(1, D3DXVECTOR2(vScreenPosition.x - vSize.x / 2.0f, vScreenPosition.y - vSize.y / 2.0f));
		pGauge->SetVertexPosition(3, D3DXVECTOR2(vScreenPosition.x - vSize.x / 2.0f, vScreenPosition.y + vSize.y / 2.0f));

		// UV座標移動(ゲージゼロ)
		pGauge->SetVertexTextureUV(1, D3DXVECTOR2(0.0f, 0.0f));
		pGauge->SetVertexTextureUV(3, D3DXVECTOR2(0.0f, 1.0f));
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::SetMaxValue(float max)
// DESC: スクリーンゲージ処理 ゲージ最大値設定
//-----------------------------------------------------------------------------
VOID GScreenGauge::SetMaxValue(float max)
{
	if (max > 0)
	{
		fMaxValue = max;
		fCurrentValue = fMaxValue;
		fTargetValue = fMaxValue;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::SetMaxValue(float max)
// DESC: スクリーンゲージ処理 ゲージ当前値設定
//-----------------------------------------------------------------------------
VOID GScreenGauge::SetCurrentValue(float value)
{
	if ( (value <= fMaxValue) && (value >= 0.0f) )
	{
		fCurrentValue = value;
	}
	else if (value < 0.0f)
	{
		fCurrentValue = 0.0f;
	}
	else
	{
		fCurrentValue = fMaxValue;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::SetMaxValue(float percent)
// DESC: スクリーンゲージ処理 ゲージ移動
//-----------------------------------------------------------------------------
VOID GScreenGauge::GaugeMove(float percent)
{
	float fPercent;
	fPercent = (fCurrentValue / fMaxValue);

	if ((fPercent >= 0.0f) && (fPercent <= 1.0f))
	{
		// 頂点移動
		pGauge->MoveVertexByVector(1, D3DXVECTOR2(vSize.x / 100.0f * percent, 0.0f));
		pGauge->MoveVertexByVector(3, D3DXVECTOR2(vSize.x / 100.0f * percent, 0.0f));

		// UV座標移動
		pGauge->MoveVertexTextureUV(1, D3DXVECTOR2(1.0f / 100.0f * percent, 0.0f));
		pGauge->MoveVertexTextureUV(3, D3DXVECTOR2(1.0f / 100.0f * percent, 0.0f));

		// 当前値再計算
		fCurrentValue += fMaxValue * (percent / 100.0f);
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