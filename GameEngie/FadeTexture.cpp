
//=============================================================================//
// 
// Name : FadeTexture.cpp
// Describe : フェードテクスチャ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "FadeTexture.h"				// フェードテクスチャ処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: フェードテクスチャ処理 コンストラクタ
//-----------------------------------------------------------------------------
GFadeTexture::GFadeTexture(LPDIRECT3DDEVICE9 D3dDevice) : BScreenTexture(D3dDevice)
{
	pDevice = D3dDevice;

	bUpdate = true;
	bRender = true;

	iFadeState = FS_Ready;

	vSize = D3DXVECTOR2(0.0f, 0.0f);
	vScreenPosition = D3DXVECTOR2(0.0f, 0.0f);

	pBlackTexture = NULL;
	pWhiteTexture = NULL;

	ScreenVtx[NUM_VERTEX_SCREENTEXTURE];
}

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::~BScreenTexture()
// DESC: フェードテクスチャ処理 デストラクタ
//-----------------------------------------------------------------------------
GFadeTexture::~GFadeTexture()
{
	SAFE_RELEASE(pBlackTexture)
	SAFE_RELEASE(pWhiteTexture)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GFadeTexture::InitFadeTexture()
// DESC: フェードテクスチャ処理 フェードテクスチャ初期化
//-----------------------------------------------------------------------------
HRESULT GFadeTexture::InitFadeTexture()
{
	D3DXCreateTextureFromFile(pDevice, PATH_BLACKFADE, &pBlackTexture);
	D3DXCreateTextureFromFile(pDevice, PATH_WHITEFADE, &pWhiteTexture);

	InitScreenTexture(D3DXVECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT), D3DXVECTOR2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GFadeTexture::UpdateFadeTexture()
// DESC: フェードテクスチャ処理 フェードテクスチャ更新
//-----------------------------------------------------------------------------
VOID GFadeTexture::UpdateFadeTexture()
{
	if (bUpdate)
	{
		// フェード処理
		if (iFadeState == FS_FadeIn)
		{
			if (iAlpha < 255)
			{
				iAlpha += SPEED_FADE;
			}
		}
		else if (iFadeState == FS_FadeOut)
		{
			if (iAlpha > 0)
			{
				iAlpha -= SPEED_FADE;
			}
		}

		// アルファ値更新
		UpdateAlpha();
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GFadeTexture::RenderFadeTexture(bool black)
// DESC: フェードテクスチャ処理 フェードテクスチャ描画
//-----------------------------------------------------------------------------
VOID GFadeTexture::RenderFadeTexture(bool black)
{
	if (bRender)
	{
		if (black)
		{
			RenderScreenTexture(pBlackTexture);
		}
		else
		{
			RenderScreenTexture(pWhiteTexture);
		}
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