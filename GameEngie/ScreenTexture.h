
//=============================================================================//
// 
// Name : ScreenTexture.h
// Describe : スクリーンテクスチャ(頂点バッファ)処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SCREENTEXTURE_H_
#define _SCREENTEXTURE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_SCREENTEXTURE		(100)					// 最大平面数

#define	FVF_VERTEX_SCREENTEXTURE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)  // UIFVF

// UI用
#define	NUM_VERTEX_SCREENTEXTURE		(4)		// 頂点数
#define	NUM_POLYGON_SCREENTEXTURE		(2)		// ポリゴン数
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }
// スクリーン頂点FVF構造体
struct ScreenVertex
{
	D3DXVECTOR3 vtx;	// 頂点座標
	float		rhw;	// スクリーン座標フラグ
	D3DCOLOR	diffuse;// ディフェーズ
	D3DXVECTOR2 tex;	// テクスチャ座標
};

// { クラス }
// スクリーン頂点クラス
class BScreenTexture
{
protected:
	LPDIRECT3DDEVICE9			pDevice; 								// デバイスオブジェクト

	int							iAlpha;									// アルファ値

	D3DXVECTOR2					vSize;									// サイズ
	D3DXVECTOR2					vScreenPosition;						// スクリーン座標

	ScreenVertex				ScreenVtx[NUM_VERTEX_SCREENTEXTURE]; 	// スクリーン頂点FVF構造体
public:
	BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice);			// コンストラクタ
	~BScreenTexture();										// デストラクタ
public:
	HRESULT InitScreenTexture(D3DXVECTOR2 size, D3DXVECTOR2 position);	// スクリーン頂点初期化
	VOID UpdateScreenTexture();											// スクリーン頂点更新
	VOID RenderScreenTexture(LPDIRECT3DTEXTURE9 texture);				// スクリーン頂点描画

	VOID UpdateAlpha();									// 透明度更新

	// [設置関数]
	VOID SetCenterPosition(D3DXVECTOR2 pos);			// 中心座標設置
	VOID SetDiffuse(D3DCOLOR d);						// 頂点ディフェーズ設置
	VOID SetAlpha(int alpha);							// 頂点透明度設置

	VOID SetVertexPosition(int n, D3DXVECTOR2 pos);		// 特定頂点座標設置
	VOID SetVertexTextureUV(int n, D3DXVECTOR2 pos);	// 特定頂点テクスチャUV座標設置
	VOID SetVertexDiffuse(int n, D3DCOLOR d);			// 特定頂点頂点ディフェーズ設置
	VOID SetVertexAlpha(int n,int alpha);				// 特定頂点透明度設置

	VOID MoveByVector(D3DXVECTOR2 move);				// 頂点移動
	VOID MoveAlpha(int movealpha);						// 頂点透明度増やす/減る

	VOID MoveVertexByVector(int n, D3DXVECTOR2 pos);	// 特定頂点座標設置
	VOID MoveVertexTextureUV(int n, D3DXVECTOR2 pos);	// 特定頂点テクスチャUV座標設置

	// [取得関数]
	D3DXVECTOR2 GetPosition() { return vScreenPosition; };			// スクリーン座標を取得する
	D3DXVECTOR2 GetSize() { return vSize; };						// サイズを取得する
	int GetAlphaValue() { return iAlpha; };							// 全体透明度を取得する

	D3DXVECTOR2 GetVertexPosition(int n) { return D3DXVECTOR2(ScreenVtx[n].vtx.x, ScreenVtx[n].vtx.y); };// 特定頂点テクスチャ座標を取得する
	D3DXVECTOR2 GetVertexTextureUV(int n) { return ScreenVtx[n].tex; };				// 特定頂点テクスチャUV座標を取得する
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif