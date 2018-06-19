
//=============================================================================//
// 
// Name : FadeTexture.h
// Describe : フェードテクスチャ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _FADETEXTURE_H_
#define _FADETEXTURE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "ScreenTexture.h"			// スクリーンテクスチャ処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_FADETEXTURE		(100)						// 最大フェードテクスチャ数

#define	PATH_BLACKFADE	("Data/Texture/Other/Fade/black.png")	// 黒テクスチャファイルパス
#define	PATH_WHITEFADE	("Data/Texture/Other/Fade/white.png")	// 白テクスチャファイルパス

#define	SPEED_FADE				(4)							// フェード速度
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// フェード状態
enum FadeState
{
	FS_Ready = 0,
	FS_FadeIn,
	FS_FadeOut
};
// { 構造体 }

// { クラス }
// フェードテクスチャクラス
class GFadeTexture : public BScreenTexture
{
private:
	LPDIRECT3DDEVICE9			pDevice; 				// デバイスオブジェクト

	LPDIRECT3DTEXTURE9			pBlackTexture;			// 黒テクスチャ
	LPDIRECT3DTEXTURE9			pWhiteTexture;			// 白テクスチャ
public:
	bool						bUpdate;				// 更新フラグ
	bool						bRender;				// 描画フラグ
	UINT						iFadeState;				// フェード状態
public:
	GFadeTexture(LPDIRECT3DDEVICE9 D3dDevice);			// コンストラクタ
	~GFadeTexture();									// デストラクタ
public:
	HRESULT InitFadeTexture();							// フェードテクスチャ初期化
	VOID UpdateFadeTexture();							// フェードテクスチャ更新
	VOID RenderFadeTexture(bool black);					// フェードテクスチャ描画
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif