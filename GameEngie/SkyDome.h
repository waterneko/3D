
//=============================================================================//
// 
// Name : SkyDome.h
// Describe : スカイドーム処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMESKY_H_
#define _GAMESKY_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "WorldTransform.h"			// ワールドトランスフォーム処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

#define	FVF_VERTEXSKY	(D3DFVF_XYZ | D3DFVF_TEX1)  // スカイドームFVF

#define	AUTOROTATION_SKYDOME		(0.000005f)				// スカイボックス自回転速度
#define	INIT_POSY_SKYDOME			(-4000.0f)				// スカイボックス初期化Y座標

#define	TEXTURE_SKYDOMENIGHT		("Data/Texture/Other/SkyDome/night.bmp")	// 夜テクスチャ
#define	TEXTURE_SKYDOMECLOUD		("Data/Texture/Other/SkyDome/cloud.bmp")	// 昼テクスチャ

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }
// スカイドームFVF構造体
struct VertexSkyDome
{
	D3DXVECTOR3 vtx;	// 頂点座標
	D3DXVECTOR2 tex;	// テクスチャ座標
};

// { クラス }

// スカイドームクラス
class GSkyDome : public WorldTransform
{
private:
	LPDIRECT3DTEXTURE9      pTexture;				// テクスチャへのポインター
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer;			// インデックスバッファー
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer; 			// 頂点バッファー
	LPDIRECT3DDEVICE9       pDevice;				// デバイスオブジェクト

	int     iNumLatitudes;							// 緯線の数
	int     iNumLongitudes;							// 経線の数
	int     iVertsPerLati;							// 緯線の頂点数
	int     iVertsPerLongi;							// 経線の頂点数
	int     iNumVertices;							// スフィアの頂点数
	float   fSkySphereRadius;						// スフィアの半径

													// スガイスフィアFVF構造体
	VertexSkyDome* 			VtxSkyDome; 			// スカイドームFVF構造体
public:
	bool					bUpdate;				// 更新フラグ
	bool					bRender;				// 描画フラグ
	bool                    bAutoRotation;			// 自動回転フラグ
public:
	GSkyDome(LPDIRECT3DDEVICE9 D3dDevice);
	~GSkyDome(void);
public:
	HRESULT LoadSkyTexture(char* Path);				// テクスチャを読込み
	HRESULT InitSkyDome(int nAlpha, int nBeta, float nRadius);	// スカイドーム初期化
	VOID UpdateSkyDome();							// スカイドーム更新
	VOID RenderSkyDome(bool bRenderFrame);			// スカイドーム描画
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif