
//=============================================================================//
// 
// Name : StageScene.h
// Describe : ステージシーン処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _STAGESCENE_H_
#define _STAGESCENE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理
#include "Input.h"					// 入力処理

#include "SourceManager.h"			// ソース管理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// シーン状態
enum GameStageSceneState
{
	SSS_Normal = 0,	// シーン実行
	SSS_Event,		// シーンイベント
	SSS_Battle,		// シーン戦闘
	SSS_Pause,		// シーン休止
};

// ステージシーン
enum StageSceneIndex
{
	GameDefaultStage = 0,
	GameStage1A,
	GameStage1B,
	GameStage2A = 5,
	GameStage2B,
};
// { 構造体 }

// { クラス }

// ステージシーン処理クラス
class SceneInstance
{
protected:
	SourceManager*			pSourceManager;						// ソース管理

	BOOL					bChangeFlag;						// シーン切り替えフラグ

	UINT					iNextScene;							// 次のシーン番号
public:
	SceneInstance();											// コンストラクタ
	~SceneInstance();											// デストラクタ
public:
	virtual HRESULT	InitScene();								// シーンの初期化
	virtual VOID UpdateController();							// シーンステージコントローラーの総更新
	virtual VOID UpdateScene();									// シーンステージの総更新
	virtual VOID RenderScene();									// シーンステージの総描画

	// [シーン切替え関数]
	VOID SetChangeSceneFlag(bool change);						// シーン切り替えフラグ設置
	VOID SetNextScene(UINT nextscene);							// シーン切り替え

	// [設置関数]
	LPDIRECT3DTEXTURE9 SetTexture(UINT n);						// テクスチャ設置
	SXfile* SetXfile(UINT n);									// xファイル設置
	SAnimXfile* SetAnimXfile(UINT n);							// アニメーションxファイル設置
	SWavfile* SetWavfile(UINT n);								// Wavファイル設置

	// [取得関数]
	BOOL GetChangeSceneFlag() { return bChangeFlag; };			// シーン切り替えフラグ
	UINT GetNextScene() { return iNextScene; };					// 次のシーン番号
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif