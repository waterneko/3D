
//=============================================================================//
// 
// Name : Stage1.h
// Describe : ステージ1処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _STAGE1_H_
#define _STAGE1_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理
#include "Input.h"					// 入力処理
#include "XA2SourceVoice.h"			// XAudio2ソースボイス処理

#include "GameStage.h"				// ゲームステージ処理

#include "Scene1A.h"				// シーン1A処理
#include "Scene1B.h"				// シーン1B処理

#include "FadeTexture.h"			// フェードテクスチャ処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }

// ステージ1処理クラス
class Stage1 : public StageInstance
{
private:
	LPDIRECT3DDEVICE9	pDevice; 							// デバイスオブジェクト

	SceneInstance*		pStageScene;						// ステージ1シーンクラス

	BXA2SourceVoice*	pSourceVoice[MAXNUM_SOURCEVOICE];	// XA2ソースボイスへのポインター

	GTexture*			pGTexture[MAXNUM_GAMETEXTURE];		// ゲームテクスチャクラスへのポインター
	GFadeTexture*		pFade;								// フェードテクスチャ処理(黒)
public:
	Stage1(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~Stage1();												// デストラクタ
public:
	HRESULT	LoadStage();									// ステージの読込み
	VOID UninitStage();										// ステージの解放

	VOID UpdateController();								// ステージコントローラーの総更新
	VOID UpdateStage();										// ステージの総更新
	VOID RenderStage();										// ステージの総描画
private:
	VOID UpdateFade();										// ステージのフェード処理処理 

	// 状態移行処理
	VOID ChangeScene();										// シーンチェンジ処理
	VOID ChangeStage();										// ステージチェンジ処理
	HRESULT ReloadScene();									// シーンリロード処理

	VOID ReturnTitle();										// タイトルを戻す処理(スタート/ メニュー)処理

	VOID DebugImgui();										// ステージデバッグ用IMGUI
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif