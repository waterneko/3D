
//=============================================================================//
// 
// Name : Xfile.h
// Describe : xファイル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _XFILE_H_
#define _XFILE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_XFILE		(100)					// 最大xファイル数

#define	PATH_X_YUKARI		("Data/Xfile/yukari/")	// xファイルパスyukari
#define	PATH_X_CASTLE		("Data/Xfile/Castle/")	// xファイルパスCastle
#define	PATH_X_ROOM			("Data/Xfile/Room/")	// xファイルパスRoom
#define	PATH_X_SLOPE		("Data/Xfile/Slope/")	// xファイルパスSlope
#define	PATH_X_CORRIDOR		("Data/Xfile/Corridor/")// xファイルパスCorridor
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// シェーディングタイプ

// { 構造体 }

// { クラス }

// xファイルクラス
class SXfile
{
private:
	LPDIRECT3DDEVICE9		pDevice;						// デバイスオブジェクト
	LPD3DXMESH				pMesh;  						// メッシュへのポインター
public:
	D3DMATERIAL9*			pMeshMaterials; 				// マテリアルへのポインター
	LPDIRECT3DTEXTURE9*		pMeshTextures;  				// テクスチャへのポインター
	DWORD					dwNumMaterials;					// マテリアルの数
public:
	SXfile(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~SXfile();												// デストラクタ
public:
	HRESULT LoadXfile(char* path, char* name);				// xファイル読み込み

	// [取得関数]
	LPD3DXMESH GetMesh() { return pMesh; };					// メッシュを取得する
	DWORD GetMaterialsNum() { return dwNumMaterials; };		// マテリアルを取得する

};
//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif