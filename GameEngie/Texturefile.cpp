
//=============================================================================//
// 
// Name : Texturefile.cpp
// Describe : テクスチャファイル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Texturefile.h"				// テクスチャファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: STexturefile::STexturefile(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: テクスチャファイル処理 コンストラクタ
//-----------------------------------------------------------------------------
STexturefile::STexturefile(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	vSize = D3DXVECTOR2(0.0f, 0.0f);
	pTexture = NULL;
}

//-----------------------------------------------------------------------------
// NAME: STexturefile::~STexturefile()
// DESC: テクスチャファイル処理 デストラクタ
//-----------------------------------------------------------------------------
STexturefile::~STexturefile()
{
	SAFE_RELEASE(pTexture)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT STexturefile::LoadTexturefile(char* path ,char* name);
// DESC: テクスチャファイル処理 テクスチャファイル読み込み
//-----------------------------------------------------------------------------
HRESULT STexturefile::LoadTexturefile(char* path ,char* name)
{
	// ファイルのパス合并処理
	char FilePath[255];
	strcpy_s(FilePath, path);
	strcat_s(FilePath, name);

	if (FAILED(D3DXCreateTextureFromFileEx(pDevice,
		FilePath,						// ファイルパス
		D3DX_DEFAULT,					// 画像ファイルから幅を取得
		D3DX_DEFAULT,					// 画像ファイルから高さを取得
		D3DX_DEFAULT,					// 要求されるミップ レベルの数
		0,								// 確保するメモリの場所を指定するオプション
		D3DFMT_A1R5G5B5,				// テクスチャのピクセルフォーマット
		D3DPOOL_MANAGED,				// テクスチャメモリの位置と管理に関するフラグ
		D3DX_FILTER_TRIANGLE,			// テクスチャピクセル拡大縮小の補間計算法
		D3DX_FILTER_TRIANGLE,			// ミップマップに対して拡大縮小の補間計算法
		D3DCOLOR_RGBA(0, 0, 0, 255),	// テクスチャ完全に透明にする色を指定します
		NULL,							// 画像の情報を格納するD3DXIMAGE_INFO構造体へのポインタ
		NULL,							// 格納する 256 色パレット(256フォーマットの場合にのみ使われる)
		&pTexture)))					// テクスチャへのポインター
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "警告！", MB_OK);
		return E_FAIL;
	}

	// テクスチャサイズを取得
	D3DSURFACE_DESC desc;
	pTexture->GetLevelDesc(0, &desc);

	vSize.x = (float)desc.Width;
	vSize.y = (float)desc.Height;

	return S_OK;
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
