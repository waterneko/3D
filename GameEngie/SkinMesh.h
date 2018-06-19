
//=============================================================================//
// 
// Name : SkinMesh.h
// Describe : スキンメッシュ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SKINMESH_H_
#define _SKINMESH_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "AnimXfile.h"				// アニメーションxファイル処理
#include "AnimationController.h"	// アニメーションコントローラー処理
#include "WorldTransform.h"			// ワールドトランスフォーム処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_SKINMESH			(30)			// 最大スキンメッシュ数
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }
// スキンメッシュクラス
class BSkinMesh : public WorldTransform
{
private:
	LPDIRECT3DDEVICE9			pDevice;					// デバイスオブジェクト

	SAnimXfile*					pAnimXfile;					// アニメーションxファイルへのポインター
public:
	bool		bUpdate;									// 更新フラグ
	bool		bRender;									// 描画フラグ

	BAnimController*			pAnimController;			//  アニメーションコントローラークラスへのポインター
public:
	BSkinMesh(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~BSkinMesh();											// デストラクタ
public:
	HRESULT InitSkinMesh(SAnimXfile* pAnimxfile);			// スキンメッシュ初期化
	VOID UpdateAnimation(FLOAT fDeltaTime);					// スキンメッシュアニメーション更新
	VOID UpdateSkinMesh();									// スキンメッシュ更新
	VOID RenderSkinMesh();									// スキンメッシュ描画
private:
	// [ボーンマトリクス]設置
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame);							// ボーンマトリクスの計算(階層)
	HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);	// ボーンマトリクスの計算(単体)
	VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);	// ボーンマトリクスの更新
	VOID DrawFrame(LPD3DXFRAME pFrame);												// フレームの描画(階層)
	VOID DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);	// フレームの描画(単体)
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif