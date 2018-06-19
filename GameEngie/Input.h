//=============================================================================//
// 
// Name : Input.h
// Describe : 入力処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる


// パットキー(16進数使って４キー１つつで入力判断する)
#define BUTTON_UP			0x00000001l	// 方向キー上(XI IY<0 && IX ==0) or (DI Pov 0)
#define BUTTON_DOWN			0x00000002l	// 方向キー下(XI IY>0 && IX ==0) or (DI Pov 18000)
#define BUTTON_LEFT			0x00000004l	// 方向キー左(XI IX<0 && IY ==0) or (DI Pov 27000)
#define BUTTON_RIGHT		0x00000008l	// 方向キー右(XI IX>0 && IY ==0) or (DI Pov 9000)
#define BUTTON_UPLEFT		0x00000010l	// 方向キー左上(XI IY<0 && IX<0) or (DI Pov 4500)
#define BUTTON_UPRIGHT		0x00000020l	// 方向キー右上(XI IY<0 && IX>0) or (DI Pov 31500)
#define BUTTON_DOWNLEFT		0x00000040l	// 方向キー左下(XI IY>0 && IX<0) or (DI Pov 22500)
#define BUTTON_DOWNRIGHT	0x00000080l	// 方向キー左下(XI IY>0 && IX<0) or (DI Pov 13500)

#define BUTTON_RS_UP		0x00000100l	// スティック上(XI IRy < 0) or (DI IZ < 0)
#define BUTTON_RS_DOWN		0x00000200l	// スティック下(XI IRy > 0) or (DI IZ > 0)
#define BUTTON_RS_LEFT		0x00000400l	// スティック左(XI lRx < 0) or (DI lRz < 0)
#define BUTTON_RS_RIGHT		0x00000800l	// スティック右(XI lRx > 0) or (DI lRz > 0)
#define BUTTON_A			0x00001000l	// Ａボタン(XI rgbButtons[0]&0x80) or (DI rgbButtons[0]&0x80)
#define BUTTON_B			0x00002000l	// Ｂボタン(XI rgbButtons[1]&0x80) or (DI rgbButtons[1]&0x80)
#define BUTTON_Y			0x00004000l	// Ｙボタン(XI rgbButtons[2]&0x80) or (DI rgbButtons[2]&0x80)
#define BUTTON_X			0x00008000l	// Ｘボタン(XI rgbButtons[3]&0x80) or (DI rgbButtons[3]&0x80)
#define BUTTON_LB			0x00010000l	// LBボタン(XI rgbButtons[4]&0x80) or (DI rgbButtons[4]&0x80)
#define BUTTON_RB			0x00020000l	// RBボタン(XI rgbButtons[5]&0x80) or (DI rgbButtons[5]&0x80)
#define BUTTON_LT			0x00040000l	// LTボタン(XI IZ<0) or (DI rgbButtons[6]&0x80)
#define BUTTON_RT			0x00080000l	// RTボタン(XI IZ>0) or (DI rgbButtons[7]&0x80)
#define BUTTON_BACK			0x00100000l	// BACKボタン(XI rgbButtons[6]&0x80) or (DI rgbButtons[8]&0x80)
#define BUTTON_START		0x00200000l	// STARTボタン(XI rgbButtons[7]&0x80) or (DI rgbButtons[9]&0x80)
#define BUTTON_L3			0x00400000l	// L3ボタン(XI rgbButtons[8]&0x80) or (DI rgbButtons[10]&0x80)
#define BUTTON_R3			0x00800000l	// R3ボタン(XI rgbButtons[9]&0x80) or (DI rgbButtons[11]&0x80)
#define BUTTON_FIX_IRX		(-4)	// スティックX修正
#define BUTTON_FIX_IRY		(4)		// スティックY修正
#define GAMEPADMAX			4			// 同時に接続するジョイパッドの最大数をセット


//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//
HRESULT InitInput(HWND hWnd, bool DInput);      // 入力処理総初期化処理
VOID UpdateInput(bool DInput);					// 入力処理総更新処理
VOID UninitInput(void);							// 入力処理総終了処理

// [キーボード]
BOOL GetKeyboardPress(int nKey);				// キープレス状態(押した状態)
BOOL GetKeyboardTrigger(int nKey);				// キートリガー状態(押した瞬間)
BOOL GetKeyboardRepeat(int nKey);				// キーリピート状態(連打)
BOOL GetKeyboardRelease(int nKey);				// キーリリ－ス状態(離した瞬間)

// [マウス]
BOOL IsMouseLeftPressed(void);      // 左クリックした状態
BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
BOOL IsMouseRightPressed(void);     // 右クリックした状態
BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
BOOL IsMouseCenterPressed(void);    // 中クリックした状態
BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
LONG  GetMouseX(void);              // マウスがX方向に動いた相対値
LONG  GetMouseY(void);              // マウスがY方向に動いた相対値
LONG  GetMouseZ(void);              // マウスホイールが動いた相対値

// [パット]
BOOL IsButtonPressed(int padNo, DWORD button);		// ボタンプレス状態(押した状態)
BOOL IsButtonTriggered(int padNo, DWORD button);	// ボタントリガー状態(押した瞬間)
BOOL IsButtonRelease(int padNo, DWORD button);		// ボタンリリ－ス状態(離した瞬間)

LONG GetXInput_RStickX(int padNo);					// パッドのRスティックX軸移動量取得
LONG GetXInput_RStickY(int padNo);					// パッドのRスティックY軸移動量取得
LONG GetXInput_LStick(int padNo);					// パッドのLスティック軸移動量取得

LONG GetDInput_RStickX(int padNo);					// パッドのRスティックX軸移動量取得
LONG GetDInput_RStickY(int padNo);					// パッドのRスティックY軸移動量取得
LONG GetDInput_LStickX(int padNo);					// パッドのLスティックX軸移動量取得
LONG GetDInput_LStickY(int padNo);					// パッドのLスティックY軸移動量取得
#endif