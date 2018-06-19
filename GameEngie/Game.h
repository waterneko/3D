
//=============================================================================//
// 
// Name : Game.h
// Describe : ゲーム総処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "SourceManager.h"			// ソース管理

#include "GameTitle.h"				// ゲームタイトル処理

#include "Stage1.h"					// ゲームステージ処理
#include "Stage2.h"					// ゲームステージ処理

#include "ScreenGauge.h"			// スクリーンゲージ処理
#include "FadeTexture.h"			// フェードテクスチャ処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define FIRST_GAMESTATE			(GS_Title)				// 最初のゲーム状態

#define MAX_TITLE_PROCESS		(10)					// タイトル最大ロードプロセス値
#define MAX_STAGE_PROCESS		(20)					// ステージ最大ロードプロセス値

#define	PATH_LOADINGTEXTURE		("Data/Texture/Other/Loading/")		// ロード用テクスチャファイルパス
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// ゲーム状態
enum GameState
{
	// ゲームタイトル
	GS_Title = 1,
	// ゲーム本番
	GS_Game,
	GS_GameReload
};

// ステージ番号
enum StageIndex
{
	GameStage1 = 1,
	GameStage2,
};

// { 構造体 }

// { クラス }

// ゲーム総処理クラス
class GameInstance
{
private:
	LPDIRECT3DDEVICE9		pDevice;							// デバイスオブジェクト

	UINT					iCurrentState;						// 現在のゲーム状態
	UINT					iTargetState;						// 次のゲーム状態

	UINT					iLoadingTexture;					// ロード画面テクスチャ番号

	SourceManager*			pSourceManager;						// ソース管理

	TitleInstance*			pTitleInstance;						// タイトル処理
	StageInstance*			pStageInstance;						// ステージ処理

	GTexture*				pGLoadingTexture[MAXNUM_GAMETEXTURE];// ゲームテクスチャクラスへのポインター
	GScreenGauge*			pLoadingGauge;						// ロード画面用ゲージ
	GFadeTexture*			pFade;								// フェードテクスチャ処理(黒)
public:
	GameInstance(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~GameInstance();											// デストラクタ
public:
	HRESULT	InitGame();											// ゲームの初期化処理

	VOID UpdateGame();											// ゲームの更新処理
	VOID RenderGame();											// ゲームの描画処理

	VOID MoveGameState(int nextstate);							// ゲームの状態切り替え
private:
	// 状態移行処理
	BOOL StartCheck();											// ゲーム開始移行処理チェック
	BOOL ChangeStageCheck();									// ステージ切替え処理チェック
	BOOL ReturnCheck();											// タイトル戻す処理チェック

	VOID DebugImgui();											// デバッグ用IMGUI処理
	VOID UpdateFade();											// ゲームの状態移行のフェード処理
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif