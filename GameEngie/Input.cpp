//=============================================================================//
// 
// Name : input.cpp
// Describe : 入力処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Input.h"				// 入力処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	NUM_KEY_MAX			(256)

// パッド用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値
//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//*****************************************************************************//
// (4) グローバル変数:
//*****************************************************************************//

// [キーボード]
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pKeyboardDevice = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

// [マウス]
LPDIRECTINPUTDEVICE8	g_pMouseDevice = NULL; // マウスデバイス

DIMOUSESTATE2   		g_mouseState;			// マウスのダイレクトな状態
DIMOUSESTATE2   		g_mouseTrigger;			// 押された瞬間だけON

// [パッド]
DIJOYSTATE2				g_dijs[GAMEPADMAX];		// DIJOY構造体
static LPDIRECTINPUTDEVICE8	g_pGamePadDevice[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// パッドデバイス

static DWORD			g_padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD			g_padTrigger[GAMEPADMAX];
static DWORD			g_padRelease[GAMEPADMAX];
static int				g_padCount = 0;			// 検出したパッドの数

//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//
// [キーボード]
HRESULT InitKeyboard(HWND hWnd);											// キーボードの初期化
VOID UninitKeyboard(void);													// キーボードの終了処理
HRESULT UpdateKeyboard(void);												// キーボードの更新処理

// [マウス]
HRESULT InitializeMouse(HWND hWnd); 										// マウスの初期化
VOID UninitMouse();															// マウスの終了処理
HRESULT UpdateMouse();														// マウスの更新処理

// [パッド]
HRESULT InitializeXInputPad(void);											// パッド初期化 (XInput)
HRESULT InitializeDInputPad(void);											// パッド初期化 (DInput)
BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);			// パッド検査コールバック
VOID UpdateXInputPad(void);													// パッド更新 (XInput)
VOID UpdateDInputPad(void);													// パッド更新 (DInput)
VOID UninitPad(void);														// パッド解放
//*****************************************************************************//
// (6) 関数
//*****************************************************************************//

//==========================================================================================================//
// NAME: HRESULT InitInput(HWND hWnd, bool DInput )
// DESC: 入力処理の初期化
//==========================================================================================================//
HRESULT InitInput(HWND hWnd, bool DInput)
{

	// 「DirectInput」オブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDInput, NULL)))
	{
		return E_FAIL;
	}

	// キーボードの初期化
	InitKeyboard(hWnd);

	// マウスの初期化
	InitializeMouse(hWnd);

	// パッドの初期化
	if (DInput)
	{
		// DIモード
		InitializeDInputPad();
	}
	else
	{
		// XIモード
		InitializeXInputPad();
	}

	return S_OK;
}

//==========================================================================================================//
// NAME: VOID UpdateInput( bool DInput )
// DESC: 入力処理の更新処理
//==========================================================================================================//
VOID UpdateInput(bool DInput )
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	// パッドの更新
	if (DInput)
	{
		// DIモード
		UpdateDInputPad();
	}
	else
	{
		// XIモード
		UpdateXInputPad();
	}

}

//==========================================================================================================//
// NAME: VOID UninitInput(void)
// DESC: 入力処理の終了処理
//==========================================================================================================//
VOID UninitInput()
{
	// キーボード解放
	SAFE_RELEASE(g_pKeyboardDevice)
	// マウス解放
	SAFE_RELEASE(g_pMouseDevice)
	// パット解放
	for (int i = 0; i < GAMEPADMAX; i++)
	{
		SAFE_RELEASE(g_pGamePadDevice[i])
	}
	// 入力デバイス解放
	SAFE_RELEASE(g_pDInput)
}

//==========================================================================================================//
// NAME: HRESULT InitKeyboard(HWND hWnd)
// DESC: キーボードの初期化
//==========================================================================================================//
HRESULT InitKeyboard(HWND hWnd)
{

	// [DirectInput]< 1. 「DirectInputデバイス」オブジェクトの作成 >
	if (FAILED(g_pDInput->CreateDevice(GUID_SysKeyboard,
		&g_pKeyboardDevice, NULL)))
	{
		MessageBox(hWnd, ("キーボードがねぇ！"), ("警告！"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 2. デバイスをキーボードに設定 >
	if (FAILED(g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, ("キーボードのデータフォーマットを設定できませんでした。"), ("警告！"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 3. 協調レベルの設定 > 
	if (FAILED(g_pKeyboardDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		MessageBox(hWnd, ("キーボードの協調モードを設定できませんでした。"), ("警告！"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 4. キーボードへのアクセス権を取得 >
	g_pKeyboardDevice->Acquire();
	return S_OK;
}

//==========================================================================================================//
// NAME: HRESULT UpdateKeyboard()
// DESC: キーボードの更新
//==========================================================================================================//
HRESULT UpdateKeyboard()
{
	HRESULT hr;
	BYTE keyStateOld[NUM_KEY_MAX];

	// 前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = g_pKeyboardDevice->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if (g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if (g_keyStateRepeatCnt[cnt] >= 20)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pKeyboardDevice->Acquire();
	}

	return S_OK;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardPress(int key)
// DESC: キーボードのプレス状態を取得
//==========================================================================================================//
BOOL GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardTrigger(int key)
// DESC: キーボードのトリガー状態を取得
//==========================================================================================================//
BOOL GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardRepeat(int key)
// DESC: キーボードのリピート状態を取得
//==========================================================================================================//
BOOL GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardRelease(int key)
// DESC: キーボードのリリ－ス状態を取得
//==========================================================================================================//
BOOL GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//==========================================================================================================//
// NAME: HRESULT InitializeMouse(HWND hWnd)
// DESC: マウスの初期化
//==========================================================================================================//
HRESULT InitializeMouse(HWND hWnd)
{

	// [DirectInput]< 1. 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(g_pDInput->CreateDevice(GUID_SysMouse,
		&g_pMouseDevice, NULL)))
	{
		MessageBox(hWnd, ("マウスがねぇ！"), ("警告！"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 2. デバイスをマウスに設定 >
	if (FAILED(g_pMouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, ("マウスのデータフォーマットを設定できませんでした。"), ("警告！"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 3. 協調レベルの設定 >
	if (FAILED(g_pMouseDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		MessageBox(hWnd, ("マウスの協調モードを設定できませんでした。"), ("警告！"), MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値


	if (FAILED(g_pMouseDevice->SetProperty(DIPROP_AXISMODE, &prop.diph)))
	{
		MessageBox(hWnd, ("マウスのプロパティを設定できませんでした"), ("警告！"), MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 4. マウスへのアクセス権を取得 >
	g_pMouseDevice->Acquire();

	return S_OK;
}

//==========================================================================================================//
// NAME: HRESULT UpdateMouse(void)
// DESC: マウスの更新
//==========================================================================================================//
HRESULT UpdateMouse(void)
{
	HRESULT result;

	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = g_mouseState;
	// データ取得
	result = g_pMouseDevice->GetDeviceState(sizeof(g_mouseState), &g_mouseState);
	if (SUCCEEDED(result))
	{
		g_mouseTrigger.lX = g_mouseTrigger.lX;
		g_mouseTrigger.lY = g_mouseTrigger.lY;
		g_mouseTrigger.lZ = g_mouseTrigger.lZ;
		// マウスのボタン状態
		for (int i = 0; i<8; i++)
		{
			g_mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				g_mouseState.rgbButtons[i]) & g_mouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = g_pMouseDevice->Acquire();
	}

	return result;
}


//==========================================================================================================//
// NAME: BOOL IsMouseLeftPressed(void)
// DESC: マウス左クリックのプレス状態を取得
//==========================================================================================================//
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(g_mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
}

//==========================================================================================================//
// NAME: BOOL IsMouseLeftTriggered(void)
// DESC: マウス左クリックのプレス状態を取得
//==========================================================================================================//
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(g_mouseTrigger.rgbButtons[0] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardPress(int key)
// DESC: マウス右クリックのプレス状態を取得
//==========================================================================================================//
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(g_mouseState.rgbButtons[1] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL IsMouseRightTriggered(void)
// DESC: マウス右クリックのトリガー状態を取得
//==========================================================================================================//
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(g_mouseTrigger.rgbButtons[1] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL IsMouseCenterPressed(void)
// DESC: マウス中クリックのプレス状態を取得
//==========================================================================================================//
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(g_mouseState.rgbButtons[2] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL IsMouseCenterTriggered(void)
// DESC: マウス中クリックのトリガー状態を取得
//==========================================================================================================//
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(g_mouseTrigger.rgbButtons[2] & 0x80);
}


//==========================================================================================================//
// NAME: LONG GetMouseX(void)
// DESC: マウスX軸移動量取得
//==========================================================================================================//
LONG GetMouseX(void)
{
	return g_mouseState.lX;
}

//==========================================================================================================//
// NAME: LONG GetMouseY(void)
// DESC: マウスY軸移動量取得
//==========================================================================================================//
LONG GetMouseY(void)
{
	return g_mouseState.lY;
}

//==========================================================================================================//
// NAME: LONG GetMouseZ(void)
// DESC: マウスZ軸移動量取得
//==========================================================================================================//
LONG GetMouseZ(void)
{
	return g_mouseState.lZ;
}

//==========================================================================================================//
// NAME: LONG GetMouseZ(void)
// DESC: コールバック関数 パットデバイスオブジェクトの作成
//==========================================================================================================//
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &g_pGamePadDevice[g_padCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}

//==========================================================================================================//
// NAME: HRESULT InitializeXInputPad(void)
// DESC: パッドの初期化(XInput)
//==========================================================================================================//
HRESULT InitializeXInputPad(void)
{
	HRESULT		result;
	int			i;

	g_padCount = 0;
	// ジョイパッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (i = 0; i<g_padCount; i++) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = g_pGamePadDevice[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // データフォーマットの設定に失敗

						  // モードを設定（フォアグラウンド＆非排他モード）
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // モードの設定に失敗

						  // 軸の値の範囲を設定
						  // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
						  // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
						  // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の範囲を設定(LT/RT)
		diprg.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RX軸の範囲を設定(Rスティック 左右)
		diprg.diph.dwObj = DIJOFS_RX;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RY軸の範囲を設定(Rスティック 上下)
		diprg.diph.dwObj = DIJOFS_RY;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		// X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z軸の無効ゾーンを設定(LT/RT)
		dipdw.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// RX軸の範囲を設定(Rスティック 左右)
		diprg.diph.dwObj = DIJOFS_RX;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &diprg.diph);
		// RY軸の範囲を設定(Rスティック 上下)
		diprg.diph.dwObj = DIJOFS_RY;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &diprg.diph);

		//ジョイスティック入力制御開始
		g_pGamePadDevice[i]->Acquire();
	}

	return true;

}

//==========================================================================================================//
// NAME: HRESULT InitializeDInputPad(void)
// DESC: パッドの初期化(DInput)
//==========================================================================================================//
HRESULT InitializeDInputPad(void)
{
	HRESULT		result;
	int			i;

	g_padCount = 0;
	// ジョイパッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (i = 0; i<g_padCount; i++) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = g_pGamePadDevice[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // データフォーマットの設定に失敗

						  // モードを設定（フォアグラウンド＆非排他モード）
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // モードの設定に失敗

						  // 軸の値の範囲を設定
						  // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
						  // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
						  // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の範囲を設定 (Rスティック 左右)
		diprg.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RZ軸の範囲を設定(Rスティック 上下)
		diprg.diph.dwObj = DIJOFS_RZ;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		// X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z軸の無効ゾーンを設定 (Rスティック 左右)
		dipdw.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// RZ軸の範囲を設定 (Rスティック 上下)
		diprg.diph.dwObj = DIJOFS_RZ;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &diprg.diph);

		//ジョイスティック入力制御開始
		g_pGamePadDevice[i]->Acquire();
	}

	return true;

}

//==========================================================================================================//
// NAME: VOID UpdatePad(void)
// DESC: パッドの更新
//==========================================================================================================//
VOID UpdateXInputPad(void)
{
	HRESULT			result;
	//DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i<g_padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = g_padState[i];
		g_padState[i] = 0x00000000l;	// 初期化

		result = g_pGamePadDevice[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}

		result = g_pGamePadDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &g_dijs[i]);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}
		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする(XInput用)
		//* 上
		if ( (g_dijs[i].lY < 0 ) && (g_dijs[i].lX >= -20 && g_dijs[i].lX <= 20 ) )	g_padState[i] |= BUTTON_UP;
		//* 下
		if ( (g_dijs[i].lY > 0 ) && (g_dijs[i].lX >= -20 && g_dijs[i].lX <= 20 ) )	g_padState[i] |= BUTTON_DOWN;
		//* 左
		if ( (g_dijs[i].lX < 0 ) && (g_dijs[i].lY >= -20 && g_dijs[i].lY <= 20) )	g_padState[i] |= BUTTON_LEFT;
		//* 右
		if ( (g_dijs[i].lX > 0 ) && (g_dijs[i].lY >= -20 && g_dijs[i].lY <= 20) )	g_padState[i] |= BUTTON_RIGHT;
		//* 左上
		if ( (g_dijs[i].lY < 0 ) && (g_dijs[i].lX <= -20 ) )						g_padState[i] |= BUTTON_UPLEFT;
		//* 右上
		if ( (g_dijs[i].lY < 0 ) && (g_dijs[i].lX >= 20 ) )							g_padState[i] |= BUTTON_UPRIGHT;
		//* 左下
		if ( (g_dijs[i].lY > 0 ) && (g_dijs[i].lX <= -20 ) )						g_padState[i] |= BUTTON_DOWNLEFT;
		//* 右下
		if ( (g_dijs[i].lY > 0 ) && (g_dijs[i].lX >= 20 ) )							g_padState[i] |= BUTTON_DOWNRIGHT;
		//* f310 Ａボタン
		if (g_dijs[i].rgbButtons[0] & 0x80)		g_padState[i] |= BUTTON_A;
		//* f310 Ｂボタン
		if (g_dijs[i].rgbButtons[1] & 0x80)		g_padState[i] |= BUTTON_B;
		//* f310 Ｘボタン
		if (g_dijs[i].rgbButtons[2] & 0x80)		g_padState[i] |= BUTTON_X;
		//* f310 Ｙボタン
		if (g_dijs[i].rgbButtons[3] & 0x80)		g_padState[i] |= BUTTON_Y;
		//* f310 LBボタン
		if (g_dijs[i].rgbButtons[4] & 0x80)		g_padState[i] |= BUTTON_LB;
		//* f310 RBボタン
		if (g_dijs[i].rgbButtons[5] & 0x80)		g_padState[i] |= BUTTON_RB;
		//* f310 LTボタン
		if (g_dijs[i].lZ < 0)					g_padState[i] |= BUTTON_LT;
		//* f310 RTボタン
		if (g_dijs[i].lZ > 0)					g_padState[i] |= BUTTON_RT;
		//* f310 BACKボタン
		if (g_dijs[i].rgbButtons[6] & 0x80)		g_padState[i] |= BUTTON_BACK;
		//* f310 STARTボタン
		if (g_dijs[i].rgbButtons[7] & 0x80)		g_padState[i] |= BUTTON_START;
		//* L3ボタン
		if (g_dijs[i].rgbButtons[8] & 0x80)		g_padState[i] |= BUTTON_L3;
		//* R3ボタン
		if (g_dijs[i].rgbButtons[9] & 0x80)		g_padState[i] |= BUTTON_R3;

		// Trigger設定
		g_padTrigger[i] = ((lastPadState ^ g_padState[i])	// 前回と違っていて
			& g_padState[i]);					// しかも今ONのやつ
												// Release設定
		g_padRelease[i] = ((lastPadState ^ g_padState[i])	// 前回と違っていて
			& ~g_padState[i]);					// しかも今ONのやつ

	}
}

//==========================================================================================================//
// NAME: VOID UpdatePad(void)
// DESC: パッドの更新
//==========================================================================================================//
VOID UpdateDInputPad(void)
{
	HRESULT			result;
	//DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i<g_padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = g_padState[i];
		g_padState[i] = 0x00000000l;	// 初期化

		result = g_pGamePadDevice[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}

		result = g_pGamePadDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &g_dijs[i]);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする(Direct Input用)
		//* 上
		if (g_dijs[i].rgdwPOV[0] == 0)						g_padState[i] |= BUTTON_UP;
		//* 下
		if (g_dijs[i].rgdwPOV[0] == 18000)					g_padState[i] |= BUTTON_DOWN;
		//* 左
		if (g_dijs[i].rgdwPOV[0] == 27000)					g_padState[i] |= BUTTON_LEFT;
		//* 右
		if (g_dijs[i].rgdwPOV[0] == 9000)					g_padState[i] |= BUTTON_RIGHT;
		//* 左上
		if (g_dijs[i].rgdwPOV[0] == 31500)					g_padState[i] |= BUTTON_UPLEFT;
		//* 右上
		if (g_dijs[i].rgdwPOV[0] == 4500)					g_padState[i] |= BUTTON_UPRIGHT;
		//* 左下
		if (g_dijs[i].rgdwPOV[0] == 22500)					g_padState[i] |= BUTTON_DOWNLEFT;
		//* 右下
		if (g_dijs[i].rgdwPOV[0] == 13500)					g_padState[i] |= BUTTON_DOWNRIGHT;
		//* f310 Ｘボタン
		if (g_dijs[i].rgbButtons[0] & 0x80)					g_padState[i] |= BUTTON_X;
		//* f310 Ａボタン
		if (g_dijs[i].rgbButtons[1] & 0x80)					g_padState[i] |= BUTTON_A;
		//* f310 Ｂボタン
		if (g_dijs[i].rgbButtons[2] & 0x80)					g_padState[i] |= BUTTON_B;
		//* f310 Ｙボタン
		if (g_dijs[i].rgbButtons[3] & 0x80)					g_padState[i] |= BUTTON_Y;
		//* f310 LBボタン
		if (g_dijs[i].rgbButtons[4] & 0x80)					g_padState[i] |= BUTTON_LB;
		//* f310 RBボタン
		if (g_dijs[i].rgbButtons[5] & 0x80)					g_padState[i] |= BUTTON_RB;
		//* f310 LTボタン
		if (g_dijs[i].rgbButtons[6] & 0x80)					g_padState[i] |= BUTTON_LT;
		//* f310 RTボタン
		if (g_dijs[i].rgbButtons[7] & 0x80)					g_padState[i] |= BUTTON_RT;
		//* f310 BACKボタン
		if (g_dijs[i].rgbButtons[8] & 0x80)					g_padState[i] |= BUTTON_BACK;
		//* f310 STARTボタン
		if (g_dijs[i].rgbButtons[9] & 0x80)					g_padState[i] |= BUTTON_START;
		//* f310 L3ボタン
		if (g_dijs[i].rgbButtons[10] & 0x80)				g_padState[i] |= BUTTON_L3;
		//* f310 R3ボタン
		if (g_dijs[i].rgbButtons[11] & 0x80)				g_padState[i] |= BUTTON_R3;
		// Trigger設定
		g_padTrigger[i] = ((lastPadState ^ g_padState[i])	// 前回と違っていて
			& g_padState[i]);					// しかも今ONのやつ
												// Release設定
		g_padRelease[i] = ((lastPadState ^ g_padState[i])	// 前回と違っていて
			& ~g_padState[i]);					// しかも今ONのやつ

	}
}

//==========================================================================================================//
// NAME: BOOL IsButtonPressed(int padNo, DWORD button)
// DESC: パッドのプレス状態を取得
//==========================================================================================================//
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & g_padState[padNo]);
}

//==========================================================================================================//
// NAME: BOOL IsButtonTriggered(int padNo, DWORD button)
// DESC: パッドのトリガー状態を取得
//==========================================================================================================//
BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & g_padTrigger[padNo]);
}

//==========================================================================================================//
// NAME: BOOL IsButtonRelease(int padNo, DWORD button)
// DESC: パッドのリリ－ス状態を取得
//==========================================================================================================//
BOOL IsButtonRelease(int padNo, DWORD button)
{
	return (button & g_padRelease[padNo]);
}

//==========================================================================================================//
// NAME: LONG GetXInput_RStickX(int padNo)
// DESC: XIモード パッドのRスティックX軸移動量取得
//==========================================================================================================//
LONG GetXInput_RStickX(int padNo)
{
	if ((g_dijs[padNo].lRx <= 50) && (g_dijs[padNo].lRx >= -50))
	{
		return 0;
	}
	else
	{
		return g_dijs[padNo].lRx;
	}
}

//==========================================================================================================//
// NAME: LONG GetXInput_RStickY(int padNo)
// DESC: XIモード パッドのRスティックY軸移動量取得
//==========================================================================================================//
LONG GetXInput_RStickY(int padNo)
{
	if ((g_dijs[padNo].lRy <= 50) && (g_dijs[padNo].lRy >= -50))
	{
		return 0;
	}
	else
	{
		return g_dijs[padNo].lRy;
	}
}

//==========================================================================================================//
// NAME: LONG GetXInput_LStick(int padNo)
// DESC: XIモード パッドのLスティック軸移動量取得
//==========================================================================================================//
LONG GetXInput_LStick(int padNo)
{
	if ((g_dijs[padNo].rgdwPOV[0] <= 10) && (g_dijs[padNo].rgdwPOV[0] >= -10))
	{
		return 0;
	}
	else
	{
		return g_dijs[padNo].rgdwPOV[0];
	}
}

//==========================================================================================================//
// NAME: LONG GetDInput_RStickY(int padNo)
// DESC: DIモード パッドのRスティックX軸移動量取得
//==========================================================================================================//
LONG GetDInput_RStickX(int padNo)
{
	if ((g_dijs[padNo].lZ <= 50) && (g_dijs[padNo].lZ >= -50))
	{
		return 0;
	}
	else
	{
		return g_dijs[padNo].lZ;
	}
}

//==========================================================================================================//
// NAME: LONG GetDInput_RStickY(int padNo)
// DESC: DIモード パッドのRスティックY軸移動量取得
//==========================================================================================================//
LONG GetDInput_RStickY(int padNo)
{
	if ((g_dijs[padNo].lRz <= 50) && (g_dijs[padNo].lRz >= -50))
	{
		return 0;
	}
	else
	{
		return g_dijs[padNo].lRz;
	}
}

//==========================================================================================================//
// NAME: LONG GetDInput_LStickX(int padNo)
// DESC: DIモード パッドのRスティックX軸移動量取得
//==========================================================================================================//
LONG GetDInput_LStickX(int padNo)
{
	if ((g_dijs[padNo].lX <= 50) && (g_dijs[padNo].lX >= -50))
	{
		return 0;
	}
	else
	{
		return g_dijs[padNo].lX;
	}
}

//==========================================================================================================//
// NAME: LONG GetDInput_LStickY(int padNo)
// DESC: DIモード パッドのRスティックX軸移動量取得
//==========================================================================================================//
LONG GetDInput_LStickY(int padNo)
{
	if ((g_dijs[padNo].lY <= 50) && (g_dijs[padNo].lY >= -50))
	{
		return 0;
	}
	else
	{
		return g_dijs[padNo].lY;
	}
}