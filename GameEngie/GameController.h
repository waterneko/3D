
//=============================================================================//
// 
// Name : GameController.h
// Describe : ゲームコントローラー処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "Input.h"					// スクリーンテクスチャ処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// ゲームパットモード
enum PadMode
{
	PM_DInput = 0,
	PM_XInput
};
// { 構造体 }

// { クラス }
// ゲームコントローラークラス
class GController
{
private:
	D3DXVECTOR3 vLstick;			// Lスティック座標
	D3DXVECTOR3 vRstick;			// Rスティック座標
	D3DXVECTOR3 vFixLstick;			// カメラ修正Lスティック座標

	D3DXMATRIX mtxCameraY;			// カメラY軸回転マトリクス
public:
	UINT						iPadState;				// ゲームパットモード
	UINT						iPadNum;				// パット番号
public:
	GController(UINT num);								// コンストラクタ
	~GController();										// デストラクタ
public:
	VOID SetPadMode(UINT state);							// ゲームパットモード設置
	VOID UpdateFixLstick(D3DXVECTOR3 rot);					// カメラY軸回転修正の更新

	BOOL PadAButton(bool press);							// ゲームパットAボタン ( = キャンセルボタン )
	BOOL PadBButton(bool press);							// ゲームパットBボタン ( = 確定ボタン )
	BOOL PadXButton(bool press);							// ゲームパットXボタン
	BOOL PadYButton(bool press);							// ゲームパットYボタン
	BOOL PadUpButton();										// ゲームパット上移動ボタン ( = ↑ボタン )
	BOOL PadDownButton();									// ゲームパット下移動ボタン ( = ↓ボタン )
	BOOL PadLeftButton();									// ゲームパット左移動ボタン ( = ←ボタン )
	BOOL PadRightButton();									// ゲームパット右移動ボタン ( = →ボタン )
	BOOL PadUpLeftButton();									// ゲームパット左上移動ボタン ( = ←↑ボタン )
	BOOL PadUpRightButton();								// ゲームパット右上移動ボタン ( = →↑ボタン )
	BOOL PadDownLeftButton();								// ゲームパット左下移動ボタン ( = ←↓ボタン )
	BOOL PadDownRightButton();								// ゲームパット右下移動ボタン ( = →↓ボタン )

	BOOL PadLBButton(bool press);							// ゲームパットLBボタン ( = LBボタン )
	BOOL PadRBButton(bool press);							// ゲームパットRBボタン ( = RBボタン )
	BOOL PadLTButton(bool press);							// ゲームパットLBボタン ( = LBボタン )
	BOOL PadRTButton(bool press);							// ゲームパットRBボタン ( = RBボタン )
	BOOL PadStartButton(bool press);						// ゲームパットSTARTボタン
	BOOL PadBackButton(bool press);							// ゲームパットBACKボタン

	BOOL ReleasePadMoveButton();							// 移動ボタン離れる
	D3DXVECTOR3 GetLstick() { return vLstick; };			// Lスティック座標を取得する
	D3DXVECTOR3 GetRstick() { return vRstick; };			// Rスティック座標を取得する
	D3DXVECTOR3 GetFixLstick() { return vFixLstick; };		// カメラ修正Lスティック座標を取得する
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif