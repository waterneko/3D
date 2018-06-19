
//=============================================================================//
// 
// Name : Scene2A.h
// Describe : シーン2A処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SCENE2A_H_
#define _SCENE2A_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理
#include "Input.h"					// 入力処理
#include "Math.h"					// 数学用関数

#include "Camera.h"					// カメラ処理
#include "Light.h"					// ライト処理

#include "Mesh.h"					// メッシュ処理
#include "SkinMesh.h"				// スキンメッシュ処理
#include "GameTexture.h"			// ゲームテクスチャ処理
#include "GameController.h"			// ゲームコントローラー処理

#include "RayIntersect.h"			// レイ交差処理

#include "StageScene.h"				// ステージシーン処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }

// シーン2A処理クラス
class Scene2A : public SceneInstance
{
private:
	LPDIRECT3DDEVICE9		pDevice; 						// デバイスオブジェクト

	BThirdPersonCamera*		pTPCamera;						// カメラクラスへのポインター
	BLight*					pLight;							// ライトクラスへのポインター
	BMesh*					pMesh[MAXNUM_MESH];				// メッシュクラスへのポインター
	BSkinMesh*				pSkinMesh[MAXNUM_SKINMESH];		// スキンメッシュクラスへのポインター
	GTexture*				pGTexture[MAXNUM_GAMETEXTURE];	// ゲームテクスチャクラスへのポインター
	GController*			pGController;					// ゲームコントローラークラスへのポインター
	CRay*					pCameraRay;						// カメラ用レイ交差クラスへのポインター
	CRay*					pCharacterRayZ;					// キャラ用レイ交差クラスへのポインター
	CRay*					pCharacterRayY;					// キャラ用レイ交差クラスへのポインター
	CRay*					pCharacterRayX;					// キャラ用レイ交差クラスへのポインター
public:
	Scene2A(LPDIRECT3DDEVICE9 D3dDevice);				// コンストラクタ
	~Scene2A();											// デストラクタ
public:
	HRESULT	InitScene();								// シーンの初期化
	VOID UpdateController();							// シーンコントローラーの総更新
	VOID UpdateScene();									// シーンの総更新
	VOID RenderScene();									// シーンの総描画

	VOID DebugImgui();									// シーンデバッグ用IMGUI
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif