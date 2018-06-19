
//=============================================================================//
// 
// Name : GameStage.h
// Describe : ゲームステージ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMESTAGE_H_
#define _GAMESTAGE_H_

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
// ステージ状態
enum GameStageState
{
	SS_Ready = 0,	// ゲームステージ待機
	SS_Loading,		// ゲームステージロード
	SS_ChangeScene,	// ゲームシーン切替え
	SS_ChangeStage,	// ゲームステージ切替え
	SS_ReturnStart,	// ゲームタイトルに戻す
};
// { 構造体 }

// { クラス }

// ゲームステージ処理クラス
class StageInstance
{
protected:
	UINT					iCurrentState;						// 現在のステージ状態
	UINT					iTargetState;						// 次のステージ状態

	UINT					iScene;								// 現在のステージシーン

	SourceManager*			pSourceManager;						// ソース管理
public:
	StageInstance();											// コンストラクタ
	~StageInstance();											// デストラクタ
public:
	virtual HRESULT	LoadStage();								// ゲームステージの読込み
	virtual VOID UninitStage();									// ゲームステージの解放

	virtual VOID UpdateController();							// ゲームステージコントローラーの総更新
	virtual VOID UpdateStage();									// ゲームステージの総更新
	virtual VOID RenderStage();									// ゲームステージの総描画

	// [設置関数]
	LPDIRECT3DTEXTURE9 SetTexture(UINT n);						// テクスチャ設置
	SXfile* SetXfile(UINT n);									// xファイル設置
	SAnimXfile* SetAnimXfile(UINT n);							// アニメーションxファイル設置
	SWavfile* SetWavfile(UINT n);								// Wavファイル設置

	// [取得関数]
	UINT GetCurrentState() { return iCurrentState; };			// 現在のステージ状態を取得する
	UINT GetTargetState() { return iTargetState; };				// 次のステージ状態を取得する
	UINT GetScene() { return iScene; };							// 今のシーンを取得する

	//BOOL GetChangeSceneFlag() { return bChangeScene; };			// シーン切り替えフラグ
	//UINT GetNextSceneNum() { return iNextScene; };				// 次のシーン番号
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif