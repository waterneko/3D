
//=============================================================================//
// 
// Name : ScreenTexture.cpp
// Describe : スクリーンテクスチャ(頂点バッファ)処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "ScreenTexture.h"				// スクリーンテクスチャ処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: スクリーンテクスチャ処理 コンストラクタ
//-----------------------------------------------------------------------------
BScreenTexture::BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iAlpha = 255;
	vSize = D3DXVECTOR2(0.0f, 0.0f);
	vScreenPosition = D3DXVECTOR2(0.0f, 0.0f);

	ScreenVtx[NUM_VERTEX_SCREENTEXTURE];
}

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::~BScreenTexture()
// DESC: スクリーンテクスチャ処理 デストラクタ
//-----------------------------------------------------------------------------
BScreenTexture::~BScreenTexture()
{

}

//-----------------------------------------------------------------------------
// NAME: HRESULT BScreenTexture::InitScreenTexture(D3DXVECTOR2 size, D3DXVECTOR2 position)
// DESC: スクリーンテクスチャ処理 UI初期化
//-----------------------------------------------------------------------------
HRESULT BScreenTexture::InitScreenTexture(D3DXVECTOR2 size, D3DXVECTOR2 position)
{
	vSize = size;
	vScreenPosition = position;
	iAlpha = 255;

	// 中心座標設置
	SetCenterPosition(vScreenPosition);

	// 反射光の設定
	SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, iAlpha));

	ScreenVtx[0].rhw = 1.0f;
	ScreenVtx[1].rhw = 1.0f;
	ScreenVtx[2].rhw = 1.0f;
	ScreenVtx[3].rhw = 1.0f;
	// テクスチャ座標の設定
	ScreenVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ScreenVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ScreenVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ScreenVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::UpdateScreenTexture()
// DESC: スクリーンテクスチャ処理 スクリーンテクスチャ更新
//-----------------------------------------------------------------------------
VOID BScreenTexture::UpdateScreenTexture()
{
	// 透明度更新
	UpdateAlpha();
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::RenderScreenTexture(LPDIRECT3DTEXTURE9 texture)
// DESC: スクリーンテクスチャ処理 スクリーンテクスチャ描画
//-----------------------------------------------------------------------------
VOID BScreenTexture::RenderScreenTexture(LPDIRECT3DTEXTURE9 texture)
{
	// 頂点FVFで描画する
	pDevice->SetFVF(FVF_VERTEX_SCREENTEXTURE);
	// テクスチャを設定する
	pDevice->SetTexture(0, texture);
	// UIレタリング
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON_SCREENTEXTURE, ScreenVtx, sizeof(ScreenVertex));
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::UpdateAlpha()
// DESC: スクリーンテクスチャ処理 透明度更新
//-----------------------------------------------------------------------------
VOID BScreenTexture::UpdateAlpha()
{
	static int iCurrentAlpha = 255;

	if (iCurrentAlpha != iAlpha)
	{
		if (iAlpha > 255)
		{
			SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, 255));
			iAlpha = 255;
		}
		else if (iAlpha < 0)
		{
			SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, 0));
			iAlpha = 0;
		}
		else
		{
			SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, iAlpha));
		}

		// 現在のアルファ値更新
		iCurrentAlpha = iAlpha;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetCenterPosition(D3DXVECTOR2 pos)
// DESC: スクリーンテクスチャ処理 座標設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetCenterPosition(D3DXVECTOR2 pos)
{
	vScreenPosition = pos;
	// 頂点座標の設定
	ScreenVtx[0].vtx = D3DXVECTOR3(-vSize.x / 2, -vSize.y / 2, 0.0f);
	ScreenVtx[1].vtx = D3DXVECTOR3(vSize.x / 2, -vSize.y / 2, 0.0f);
	ScreenVtx[2].vtx = D3DXVECTOR3(-vSize.x / 2, vSize.y / 2, 0.0f);
	ScreenVtx[3].vtx = D3DXVECTOR3(vSize.x / 2, vSize.y / 2, 0.0f);

	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].vtx += D3DXVECTOR3(vScreenPosition.x, vScreenPosition.y, 0.0f);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetDiffuse()
// DESC: スクリーンテクスチャ処理 頂点ディフェーズ設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetDiffuse(D3DCOLOR d)
{
	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].diffuse = d;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetAlpha(int alpha)
// DESC: スクリーンテクスチャ処理 頂点透明度設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetAlpha(int alpha)
{
	iAlpha = alpha;
	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, iAlpha);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetVertexPosition(int n, D3DXVECTOR2 pos)
// DESC: スクリーンテクスチャ処理 特定頂点座標設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetVertexPosition(int n, D3DXVECTOR2 pos)
{
	ScreenVtx[n].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetVertexTextureUV(int n, D3DXVECTOR2 pos)
// DESC: スクリーンテクスチャ処理 特定頂点テクスチャUV座標設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetVertexTextureUV(int n, D3DXVECTOR2 pos)
{
	ScreenVtx[n].tex = pos;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetVertexDiffuse(int n, D3DXCOLOR d)
// DESC: スクリーンテクスチャ処理 特定頂点頂点ディフェーズ設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetVertexDiffuse(int n, D3DCOLOR d)
{
	ScreenVtx[n].diffuse = d;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveByVector(D3DXVECTOR2 move)
// DESC: スクリーンテクスチャ処理 頂点移動
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveByVector(D3DXVECTOR2 move)
{
	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].vtx += D3DXVECTOR3(move.x, move.y, 0.0f);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveAlpha(int movealpha)
// DESC: スクリーンテクスチャ処理 頂点透明度増やす/減る
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveAlpha(int movealpha)
{
	iAlpha += movealpha;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveVertexByVector(int n, D3DXVECTOR2 move)
// DESC: スクリーンテクスチャ処理 特定頂点座標設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveVertexByVector(int n, D3DXVECTOR2 move)
{
	ScreenVtx[n].vtx += D3DXVECTOR3(move.x, move.y, 0.0f);
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveVertexTextureUV(int n, D3DXVECTOR2 move)
// DESC: スクリーンテクスチャ処理 特定頂点テクスチャUV座標設置
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveVertexTextureUV(int n, D3DXVECTOR2 move)
{
	ScreenVtx[n].tex += move;
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