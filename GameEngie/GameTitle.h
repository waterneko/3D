
//=============================================================================//
// 
// Name : GameTitle.h
// Describe : ゲームタイトル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMETITLE_H_
#define _GAMETITLE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理
#include "Input.h"					// 入力処理

#include "SourceManager.h"			// ソース管理

#include "Camera.h"					// カメラ処理
#include "Light.h"					// ライト処理

#include "Mesh.h"					// メッシュ処理
#include "SkyDome.h"				// スカイドーム処理

#include "GameController.h"			// ゲームコントローラー処理
#include "GameTexture.h"			// ゲームテクスチャ処理
#include "ScreenGauge.h"			// スクリーンゲージ処理
#include "FadeTexture.h"			// フェードテクスチャ処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define FIRST_GAMESTAGE		(1)		// 最初のステージ

#define	TITLE_START_CAMERAROT			(D3DXVECTOR3(D3DXToRadian(-20),D3DXToRadian(-25) ,0.0f))	// ゲームスタート画面カメラ角度
#define	TITLE_START_AMBIENT				(D3DCOLOR_RGBA(12,10,40,255))			// ゲームスタート画面環境光
#define	TITLE_START_LIGHTAMBIENT		(D3DCOLOR_RGBA(68,68,68,255))			// ゲームスタート画面ライトアンビエント
#define	TITLE_START_LIGHTDIFFUSE		(D3DCOLOR_RGBA(210,95,150,255))			// ゲームスタート画面ライトディフェーズ
#define	TITLE_START_LIGHTSPECULAR		(D3DCOLOR_RGBA(12,8,60,255))			// ゲームスタート画面ライトスペキュラ
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// タイトル状態
enum TitleState
{
	TS_Logo = 0,	// ゲームロゴ
	TS_Start,		// ゲームスタート
	TS_Menu,		// ゲームメニュー
};
// { 構造体 }

// { クラス }

// ゲームタイトル処理クラス
class TitleInstance
{
private:
	LPDIRECT3DDEVICE9		pDevice;							// デバイスオブジェクト

	UINT					iCurrentState;						// 現在のタイトル状態
	UINT					iTargetState;						// 次のタイトル状態

	UINT					iSelectStage;						// 選択したステージ

	SourceManager*			pSourceManager;						// ソース管理

	BThirdPersonCamera*		pTPCamera;							// カメラクラスへのポインター
	BLight*					pLight;								// ライトクラスへのポインター

	BMesh*					pMesh[MAXNUM_MESH];					// メッシュクラスへのポインター
	GSkyDome*				pGSkyDome;							// スカイドームクラスへのポインター
	GTexture*				pGTexture[MAXNUM_GAMETEXTURE];		// ゲームテクスチャクラスへのポインター
	GController*			pGController;						// ゲームコントローラークラスへのポインター
	GFadeTexture*			pFade;								// フェードテクスチャ処理(白)
public:
	TitleInstance(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~TitleInstance();											// デストラクタ
public:
	HRESULT	LoadTitle();										// ゲームタイトルの読込み
	VOID UninitTitle();								// ゲームタイトルの解放

	VOID UpdateController();									// ゲームタイトルコントローラーの総更新処理
	VOID UpdateTitle();											// ゲームタイトルの総更新処理
	VOID RenderTitle();											// ゲームタイトルの総描画処理

	// [取得関数]
	UINT GetCurrentState() { return iCurrentState; };			// 現在のタイトル状態を取得する
	UINT GetTargetState() { return iTargetState; };				// 次のタイトル状態を取得する
	UINT GetSelectStage() { return iSelectStage; };				// 選択したステージを取得する
	GFadeTexture* GetFade() { return pFade; };					// フェードポインターを取得する
private:
	HRESULT LoadSource();										// ゲームタイトルソースの読込み
	HRESULT	InitObject();										// ゲームタイトル対象の総初期化処理

	VOID UpdateLogoController();								// ゲームロゴコントローラーの更新処理
	VOID UpdateStartController();								// ゲームスタートコントローラーの更新処理
	VOID UpdateMenuController();								// ゲームメニューコントローラーの更新処理

	VOID UpdateFade();											// タイトル状態移行のフェード処理処理
	VOID UpdateLogoObject();									// ゲームロゴ対象の更新処理
	VOID UpdateStartObject();									// ゲームスタート対象の更新処理
	VOID UpdateMenuObject();									// ゲームメニュー対象の更新処理

	VOID RenderLogo();											// ゲームロゴの描画処理
	VOID RenderStart();											// ゲームスタートの描画処理
	VOID RenderMenu();											// ゲームメニューの描画処理

	// 状態移行処理
	VOID ChangeTitleState(int state);							// ゲームタイトル状態の変更

	// [設置関数]
	LPDIRECT3DTEXTURE9 SetTexture(UINT n);						// テクスチャ設置
	SXfile* SetXfile(UINT n);									// xファイル設置
	SAnimXfile* SetAnimXfile(UINT n);							// アニメーションxファイル設置
	SWavfile* SetWavfile(UINT n);								// Wavファイル設置
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif