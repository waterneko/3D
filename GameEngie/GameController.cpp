
//=============================================================================//
// 
// Name : GameController.cpp
// Describe : ゲームコントローラー処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "GameController.h"				// ゲームコントローラー処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: GController::GController(UINT num)
// DESC: ゲームコントローラークラス コンストラクタ
//-----------------------------------------------------------------------------
GController::GController(UINT num)
{
	iPadState = PM_DInput;
	iPadNum = num;
	vLstick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vRstick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vFixLstick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixIdentity(&mtxCameraY);
}

//-----------------------------------------------------------------------------
// NAME: GController::~GController()
// DESC: ゲームコントローラークラス デストラクタ
//-----------------------------------------------------------------------------
GController::~GController()
{

}


//-----------------------------------------------------------------------------
// NAME: VOID GController::SetPadMode(UINT state)
// DESC: ゲームコントローラークラス ゲームパットモード設置
//-----------------------------------------------------------------------------
VOID GController::SetPadMode(UINT state)
{
	iPadState = state;
}

//-----------------------------------------------------------------------------
// NAME: VOID GController::UpdateFixLstick(D3DXVECTOR3 rot)
// DESC: ゲームコントローラークラス カメラY軸回転修正の更新
//-----------------------------------------------------------------------------
VOID GController::UpdateFixLstick(D3DXVECTOR3 rot)
{
	D3DXMatrixIdentity(&mtxCameraY);
	if (iPadState == PM_DInput)
	{
		vLstick = D3DXVECTOR3((float)GetDInput_LStickX(iPadNum), 0.0f, (float)-GetDInput_LStickY(iPadNum));
		vRstick = D3DXVECTOR3((float)GetDInput_RStickX(iPadNum), 0.0f, (float)GetDInput_RStickY(iPadNum));
	}
	else if (iPadState == PM_XInput)
	{
		vRstick = D3DXVECTOR3((float)GetXInput_RStickX(iPadNum), 0.0f, (float)GetXInput_RStickY(iPadNum));
	}
	// スティック正規化
	D3DXVec3Normalize(&vLstick, &vLstick);
	D3DXVec3Normalize(&vRstick, &vRstick);
	// カメラ回転マトリクス更新
	D3DXMatrixRotationY(&mtxCameraY, rot.y);
	D3DXVec3TransformCoord(&vFixLstick, &vLstick, &mtxCameraY);
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadAButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットAボタン ( = キャンセルボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadAButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_A) || GetKeyboardPress(DIK_X))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_A) || GetKeyboardTrigger(DIK_X))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadBButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットBボタン ( = 確定ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadBButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_B) || GetKeyboardPress(DIK_Z))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_B) || GetKeyboardTrigger(DIK_Z))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadXButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットXボタン
//-----------------------------------------------------------------------------
BOOL GController::PadXButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_X) || GetKeyboardPress(DIK_C))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_X) || GetKeyboardTrigger(DIK_C))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadXButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットYボタン
//-----------------------------------------------------------------------------
BOOL GController::PadYButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_Y) || GetKeyboardPress(DIK_V))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_Y) || GetKeyboardTrigger(DIK_V))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadUpButton()
// DESC: ゲームコントローラークラス ゲームパット上移動ボタン ( = ↑ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadUpButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_UP) || GetKeyboardTrigger(DIK_W))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadDownButton()
// DESC: ゲームコントローラークラス ゲームパット下移動ボタン ( = ↓ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadDownButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_DOWN) || GetKeyboardTrigger(DIK_S))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadLeftButton()
// DESC: ゲームコントローラークラス ゲームパット左移動ボタン ( = ←ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadLeftButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_LEFT) || GetKeyboardTrigger(DIK_A))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadRightButton()
// DESC: ゲームコントローラークラス ゲームパット右移動ボタン ( = →ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadRightButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_RIGHT) || GetKeyboardTrigger(DIK_D))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadUpLeftButton()
// DESC: ゲームコントローラークラス ゲームパット左上移動ボタン ( = ←↑ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadUpLeftButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_UPLEFT))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadUpRightButton()
// DESC: ゲームコントローラークラス ゲームパット右上移動ボタン ( = →↑ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadUpRightButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_UPRIGHT))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadDownLeftButton()
// DESC: ゲームコントローラークラス ゲームパット左下移動ボタン ( = ←↓ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadDownLeftButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_DOWNLEFT))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadDownRightButton()
// DESC: ゲームコントローラークラス ゲームパット右下移動ボタン ( = →↓ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadDownRightButton()
{
	if (IsButtonPressed(iPadNum, BUTTON_DOWNRIGHT))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadLBButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットLBボタン ( = L1ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadLBButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_LB))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_LB))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadRBButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットRBボタン ( = R1ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadRBButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_RB))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_RB))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadLTButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットLTボタン ( = L2ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadLTButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_LT))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_LT))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadRTButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットRTボタン ( = R2ボタン )
//-----------------------------------------------------------------------------
BOOL GController::PadRTButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_RT))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_RT))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadStartButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットSTARTボタン
//-----------------------------------------------------------------------------
BOOL GController::PadStartButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_START))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_START))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadBackButton(bool press)
// DESC: ゲームコントローラークラス ゲームパットBACKボタン
//-----------------------------------------------------------------------------
BOOL GController::PadBackButton(bool press)
{
	if (press)
	{
		if (IsButtonPressed(iPadNum, BUTTON_BACK))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (IsButtonTriggered(iPadNum, BUTTON_BACK))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::ReleasePadMoveButton()
// DESC: ゲームコントローラークラス 移動ボタン離れる
//-----------------------------------------------------------------------------
BOOL GController::ReleasePadMoveButton()
{
	if (IsButtonRelease(0, BUTTON_UP) || IsButtonRelease(0, BUTTON_DOWN) || IsButtonRelease(0, BUTTON_LEFT) || IsButtonRelease(0, BUTTON_RIGHT)
		|| GetKeyboardRelease(DIK_W) || GetKeyboardRelease(DIK_S) || GetKeyboardRelease(DIK_A) || GetKeyboardRelease(DIK_D))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//*****************************************************************************//
// (4) グローバル変数:
//*****************************************************************************//

//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//

//*****************************************************************************//
// (6) 関数
//*****************************************************************************//