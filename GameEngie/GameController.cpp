
//=============================================================================//
// 
// Name : GameController.cpp
// Describe : �Q�[���R���g���[���[����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "GameController.h"				// �Q�[���R���g���[���[����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: GController::GController(UINT num)
// DESC: �Q�[���R���g���[���[�N���X �R���X�g���N�^
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
// DESC: �Q�[���R���g���[���[�N���X �f�X�g���N�^
//-----------------------------------------------------------------------------
GController::~GController()
{

}


//-----------------------------------------------------------------------------
// NAME: VOID GController::SetPadMode(UINT state)
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g���[�h�ݒu
//-----------------------------------------------------------------------------
VOID GController::SetPadMode(UINT state)
{
	iPadState = state;
}

//-----------------------------------------------------------------------------
// NAME: VOID GController::UpdateFixLstick(D3DXVECTOR3 rot)
// DESC: �Q�[���R���g���[���[�N���X �J����Y����]�C���̍X�V
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
	// �X�e�B�b�N���K��
	D3DXVec3Normalize(&vLstick, &vLstick);
	D3DXVec3Normalize(&vRstick, &vRstick);
	// �J������]�}�g���N�X�X�V
	D3DXMatrixRotationY(&mtxCameraY, rot.y);
	D3DXVec3TransformCoord(&vFixLstick, &vLstick, &mtxCameraY);
}

//-----------------------------------------------------------------------------
// NAME: BOOL GController::PadAButton(bool press)
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gA�{�^�� ( = �L�����Z���{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gB�{�^�� ( = �m��{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gX�{�^��
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gY�{�^��
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g��ړ��{�^�� ( = ���{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g���ړ��{�^�� ( = ���{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g���ړ��{�^�� ( = ���{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g�E�ړ��{�^�� ( = ���{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g����ړ��{�^�� ( = �����{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g�E��ړ��{�^�� ( = �����{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g�����ړ��{�^�� ( = �����{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�g�E���ړ��{�^�� ( = �����{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gLB�{�^�� ( = L1�{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gRB�{�^�� ( = R1�{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gLT�{�^�� ( = L2�{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gRT�{�^�� ( = R2�{�^�� )
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gSTART�{�^��
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
// DESC: �Q�[���R���g���[���[�N���X �Q�[���p�b�gBACK�{�^��
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
// DESC: �Q�[���R���g���[���[�N���X �ړ��{�^�������
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
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//

//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//