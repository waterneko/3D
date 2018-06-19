//=============================================================================//
// 
// Name : input.cpp
// Describe : ���͏���
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Input.h"				// ���͏���

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	NUM_KEY_MAX			(256)

// �p�b�h�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//*****************************************************************************//
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

// [�L�[�{�[�h]
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pKeyboardDevice = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

// [�}�E�X]
LPDIRECTINPUTDEVICE8	g_pMouseDevice = NULL; // �}�E�X�f�o�C�X

DIMOUSESTATE2   		g_mouseState;			// �}�E�X�̃_�C���N�g�ȏ��
DIMOUSESTATE2   		g_mouseTrigger;			// �����ꂽ�u�Ԃ���ON

// [�p�b�h]
DIJOYSTATE2				g_dijs[GAMEPADMAX];		// DIJOY�\����
static LPDIRECTINPUTDEVICE8	g_pGamePadDevice[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// �p�b�h�f�o�C�X

static DWORD			g_padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD			g_padTrigger[GAMEPADMAX];
static DWORD			g_padRelease[GAMEPADMAX];
static int				g_padCount = 0;			// ���o�����p�b�h�̐�

//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//
// [�L�[�{�[�h]
HRESULT InitKeyboard(HWND hWnd);											// �L�[�{�[�h�̏�����
VOID UninitKeyboard(void);													// �L�[�{�[�h�̏I������
HRESULT UpdateKeyboard(void);												// �L�[�{�[�h�̍X�V����

// [�}�E�X]
HRESULT InitializeMouse(HWND hWnd); 										// �}�E�X�̏�����
VOID UninitMouse();															// �}�E�X�̏I������
HRESULT UpdateMouse();														// �}�E�X�̍X�V����

// [�p�b�h]
HRESULT InitializeXInputPad(void);											// �p�b�h������ (XInput)
HRESULT InitializeDInputPad(void);											// �p�b�h������ (DInput)
BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);			// �p�b�h�����R�[���o�b�N
VOID UpdateXInputPad(void);													// �p�b�h�X�V (XInput)
VOID UpdateDInputPad(void);													// �p�b�h�X�V (DInput)
VOID UninitPad(void);														// �p�b�h���
//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//

//==========================================================================================================//
// NAME: HRESULT InitInput(HWND hWnd, bool DInput )
// DESC: ���͏����̏�����
//==========================================================================================================//
HRESULT InitInput(HWND hWnd, bool DInput)
{

	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDInput, NULL)))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hWnd);

	// �}�E�X�̏�����
	InitializeMouse(hWnd);

	// �p�b�h�̏�����
	if (DInput)
	{
		// DI���[�h
		InitializeDInputPad();
	}
	else
	{
		// XI���[�h
		InitializeXInputPad();
	}

	return S_OK;
}

//==========================================================================================================//
// NAME: VOID UpdateInput( bool DInput )
// DESC: ���͏����̍X�V����
//==========================================================================================================//
VOID UpdateInput(bool DInput )
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();

	// �p�b�h�̍X�V
	if (DInput)
	{
		// DI���[�h
		UpdateDInputPad();
	}
	else
	{
		// XI���[�h
		UpdateXInputPad();
	}

}

//==========================================================================================================//
// NAME: VOID UninitInput(void)
// DESC: ���͏����̏I������
//==========================================================================================================//
VOID UninitInput()
{
	// �L�[�{�[�h���
	SAFE_RELEASE(g_pKeyboardDevice)
	// �}�E�X���
	SAFE_RELEASE(g_pMouseDevice)
	// �p�b�g���
	for (int i = 0; i < GAMEPADMAX; i++)
	{
		SAFE_RELEASE(g_pGamePadDevice[i])
	}
	// ���̓f�o�C�X���
	SAFE_RELEASE(g_pDInput)
}

//==========================================================================================================//
// NAME: HRESULT InitKeyboard(HWND hWnd)
// DESC: �L�[�{�[�h�̏�����
//==========================================================================================================//
HRESULT InitKeyboard(HWND hWnd)
{

	// [DirectInput]< 1. �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬 >
	if (FAILED(g_pDInput->CreateDevice(GUID_SysKeyboard,
		&g_pKeyboardDevice, NULL)))
	{
		MessageBox(hWnd, ("�L�[�{�[�h���˂��I"), ("�x���I"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 2. �f�o�C�X���L�[�{�[�h�ɐݒ� >
	if (FAILED(g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, ("�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B"), ("�x���I"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 3. �������x���̐ݒ� > 
	if (FAILED(g_pKeyboardDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		MessageBox(hWnd, ("�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B"), ("�x���I"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 4. �L�[�{�[�h�ւ̃A�N�Z�X�����擾 >
	g_pKeyboardDevice->Acquire();
	return S_OK;
}

//==========================================================================================================//
// NAME: HRESULT UpdateKeyboard()
// DESC: �L�[�{�[�h�̍X�V
//==========================================================================================================//
HRESULT UpdateKeyboard()
{
	HRESULT hr;
	BYTE keyStateOld[NUM_KEY_MAX];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pKeyboardDevice->Acquire();
	}

	return S_OK;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardPress(int key)
// DESC: �L�[�{�[�h�̃v���X��Ԃ��擾
//==========================================================================================================//
BOOL GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardTrigger(int key)
// DESC: �L�[�{�[�h�̃g���K�[��Ԃ��擾
//==========================================================================================================//
BOOL GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardRepeat(int key)
// DESC: �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//==========================================================================================================//
BOOL GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardRelease(int key)
// DESC: �L�[�{�[�h�̃����|�X��Ԃ��擾
//==========================================================================================================//
BOOL GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//==========================================================================================================//
// NAME: HRESULT InitializeMouse(HWND hWnd)
// DESC: �}�E�X�̏�����
//==========================================================================================================//
HRESULT InitializeMouse(HWND hWnd)
{

	// [DirectInput]< 1. �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(g_pDInput->CreateDevice(GUID_SysMouse,
		&g_pMouseDevice, NULL)))
	{
		MessageBox(hWnd, ("�}�E�X���˂��I"), ("�x���I"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 2. �f�o�C�X���}�E�X�ɐݒ� >
	if (FAILED(g_pMouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, ("�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B"), ("�x���I"), MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 3. �������x���̐ݒ� >
	if (FAILED(g_pMouseDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		MessageBox(hWnd, ("�}�E�X�̋������[�h��ݒ�ł��܂���ł����B"), ("�x���I"), MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl


	if (FAILED(g_pMouseDevice->SetProperty(DIPROP_AXISMODE, &prop.diph)))
	{
		MessageBox(hWnd, ("�}�E�X�̃v���p�e�B��ݒ�ł��܂���ł���"), ("�x���I"), MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	// [DirectInput]< 4. �}�E�X�ւ̃A�N�Z�X�����擾 >
	g_pMouseDevice->Acquire();

	return S_OK;
}

//==========================================================================================================//
// NAME: HRESULT UpdateMouse(void)
// DESC: �}�E�X�̍X�V
//==========================================================================================================//
HRESULT UpdateMouse(void)
{
	HRESULT result;

	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = g_mouseState;
	// �f�[�^�擾
	result = g_pMouseDevice->GetDeviceState(sizeof(g_mouseState), &g_mouseState);
	if (SUCCEEDED(result))
	{
		g_mouseTrigger.lX = g_mouseTrigger.lX;
		g_mouseTrigger.lY = g_mouseTrigger.lY;
		g_mouseTrigger.lZ = g_mouseTrigger.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i<8; i++)
		{
			g_mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				g_mouseState.rgbButtons[i]) & g_mouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = g_pMouseDevice->Acquire();
	}

	return result;
}


//==========================================================================================================//
// NAME: BOOL IsMouseLeftPressed(void)
// DESC: �}�E�X���N���b�N�̃v���X��Ԃ��擾
//==========================================================================================================//
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(g_mouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
}

//==========================================================================================================//
// NAME: BOOL IsMouseLeftTriggered(void)
// DESC: �}�E�X���N���b�N�̃v���X��Ԃ��擾
//==========================================================================================================//
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(g_mouseTrigger.rgbButtons[0] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL GetKeyboardPress(int key)
// DESC: �}�E�X�E�N���b�N�̃v���X��Ԃ��擾
//==========================================================================================================//
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(g_mouseState.rgbButtons[1] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL IsMouseRightTriggered(void)
// DESC: �}�E�X�E�N���b�N�̃g���K�[��Ԃ��擾
//==========================================================================================================//
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(g_mouseTrigger.rgbButtons[1] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL IsMouseCenterPressed(void)
// DESC: �}�E�X���N���b�N�̃v���X��Ԃ��擾
//==========================================================================================================//
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(g_mouseState.rgbButtons[2] & 0x80);
}

//==========================================================================================================//
// NAME: BOOL IsMouseCenterTriggered(void)
// DESC: �}�E�X���N���b�N�̃g���K�[��Ԃ��擾
//==========================================================================================================//
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(g_mouseTrigger.rgbButtons[2] & 0x80);
}


//==========================================================================================================//
// NAME: LONG GetMouseX(void)
// DESC: �}�E�XX���ړ��ʎ擾
//==========================================================================================================//
LONG GetMouseX(void)
{
	return g_mouseState.lX;
}

//==========================================================================================================//
// NAME: LONG GetMouseY(void)
// DESC: �}�E�XY���ړ��ʎ擾
//==========================================================================================================//
LONG GetMouseY(void)
{
	return g_mouseState.lY;
}

//==========================================================================================================//
// NAME: LONG GetMouseZ(void)
// DESC: �}�E�XZ���ړ��ʎ擾
//==========================================================================================================//
LONG GetMouseZ(void)
{
	return g_mouseState.lZ;
}

//==========================================================================================================//
// NAME: LONG GetMouseZ(void)
// DESC: �R�[���o�b�N�֐� �p�b�g�f�o�C�X�I�u�W�F�N�g�̍쐬
//==========================================================================================================//
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &g_pGamePadDevice[g_padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}

//==========================================================================================================//
// NAME: HRESULT InitializeXInputPad(void)
// DESC: �p�b�h�̏�����(XInput)
//==========================================================================================================//
HRESULT InitializeXInputPad(void)
{
	HRESULT		result;
	int			i;

	g_padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i<g_padCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = g_pGamePadDevice[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

						  // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // ���[�h�̐ݒ�Ɏ��s

						  // ���̒l�͈̔͂�ݒ�
						  // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
						  // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
						  // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z���͈̔͂�ݒ�(LT/RT)
		diprg.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RX���͈̔͂�ݒ�(R�X�e�B�b�N ���E)
		diprg.diph.dwObj = DIJOFS_RX;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RY���͈̔͂�ݒ�(R�X�e�B�b�N �㉺)
		diprg.diph.dwObj = DIJOFS_RY;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		// X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z���̖����]�[����ݒ�(LT/RT)
		dipdw.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// RX���͈̔͂�ݒ�(R�X�e�B�b�N ���E)
		diprg.diph.dwObj = DIJOFS_RX;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &diprg.diph);
		// RY���͈̔͂�ݒ�(R�X�e�B�b�N �㉺)
		diprg.diph.dwObj = DIJOFS_RY;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &diprg.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		g_pGamePadDevice[i]->Acquire();
	}

	return true;

}

//==========================================================================================================//
// NAME: HRESULT InitializeDInputPad(void)
// DESC: �p�b�h�̏�����(DInput)
//==========================================================================================================//
HRESULT InitializeDInputPad(void)
{
	HRESULT		result;
	int			i;

	g_padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i<g_padCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = g_pGamePadDevice[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

						  // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // ���[�h�̐ݒ�Ɏ��s

						  // ���̒l�͈̔͂�ݒ�
						  // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
						  // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
						  // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z���͈̔͂�ݒ� (R�X�e�B�b�N ���E)
		diprg.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RZ���͈̔͂�ݒ�(R�X�e�B�b�N �㉺)
		diprg.diph.dwObj = DIJOFS_RZ;
		g_pGamePadDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		// X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z���̖����]�[����ݒ� (R�X�e�B�b�N ���E)
		dipdw.diph.dwObj = DIJOFS_Z;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// RZ���͈̔͂�ݒ� (R�X�e�B�b�N �㉺)
		diprg.diph.dwObj = DIJOFS_RZ;
		g_pGamePadDevice[i]->SetProperty(DIPROP_DEADZONE, &diprg.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		g_pGamePadDevice[i]->Acquire();
	}

	return true;

}

//==========================================================================================================//
// NAME: VOID UpdatePad(void)
// DESC: �p�b�h�̍X�V
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
		g_padState[i] = 0x00000000l;	// ������

		result = g_pGamePadDevice[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}

		result = g_pGamePadDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &g_dijs[i]);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}
		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���(XInput�p)
		//* ��
		if ( (g_dijs[i].lY < 0 ) && (g_dijs[i].lX >= -20 && g_dijs[i].lX <= 20 ) )	g_padState[i] |= BUTTON_UP;
		//* ��
		if ( (g_dijs[i].lY > 0 ) && (g_dijs[i].lX >= -20 && g_dijs[i].lX <= 20 ) )	g_padState[i] |= BUTTON_DOWN;
		//* ��
		if ( (g_dijs[i].lX < 0 ) && (g_dijs[i].lY >= -20 && g_dijs[i].lY <= 20) )	g_padState[i] |= BUTTON_LEFT;
		//* �E
		if ( (g_dijs[i].lX > 0 ) && (g_dijs[i].lY >= -20 && g_dijs[i].lY <= 20) )	g_padState[i] |= BUTTON_RIGHT;
		//* ����
		if ( (g_dijs[i].lY < 0 ) && (g_dijs[i].lX <= -20 ) )						g_padState[i] |= BUTTON_UPLEFT;
		//* �E��
		if ( (g_dijs[i].lY < 0 ) && (g_dijs[i].lX >= 20 ) )							g_padState[i] |= BUTTON_UPRIGHT;
		//* ����
		if ( (g_dijs[i].lY > 0 ) && (g_dijs[i].lX <= -20 ) )						g_padState[i] |= BUTTON_DOWNLEFT;
		//* �E��
		if ( (g_dijs[i].lY > 0 ) && (g_dijs[i].lX >= 20 ) )							g_padState[i] |= BUTTON_DOWNRIGHT;
		//* f310 �`�{�^��
		if (g_dijs[i].rgbButtons[0] & 0x80)		g_padState[i] |= BUTTON_A;
		//* f310 �a�{�^��
		if (g_dijs[i].rgbButtons[1] & 0x80)		g_padState[i] |= BUTTON_B;
		//* f310 �w�{�^��
		if (g_dijs[i].rgbButtons[2] & 0x80)		g_padState[i] |= BUTTON_X;
		//* f310 �x�{�^��
		if (g_dijs[i].rgbButtons[3] & 0x80)		g_padState[i] |= BUTTON_Y;
		//* f310 LB�{�^��
		if (g_dijs[i].rgbButtons[4] & 0x80)		g_padState[i] |= BUTTON_LB;
		//* f310 RB�{�^��
		if (g_dijs[i].rgbButtons[5] & 0x80)		g_padState[i] |= BUTTON_RB;
		//* f310 LT�{�^��
		if (g_dijs[i].lZ < 0)					g_padState[i] |= BUTTON_LT;
		//* f310 RT�{�^��
		if (g_dijs[i].lZ > 0)					g_padState[i] |= BUTTON_RT;
		//* f310 BACK�{�^��
		if (g_dijs[i].rgbButtons[6] & 0x80)		g_padState[i] |= BUTTON_BACK;
		//* f310 START�{�^��
		if (g_dijs[i].rgbButtons[7] & 0x80)		g_padState[i] |= BUTTON_START;
		//* L3�{�^��
		if (g_dijs[i].rgbButtons[8] & 0x80)		g_padState[i] |= BUTTON_L3;
		//* R3�{�^��
		if (g_dijs[i].rgbButtons[9] & 0x80)		g_padState[i] |= BUTTON_R3;

		// Trigger�ݒ�
		g_padTrigger[i] = ((lastPadState ^ g_padState[i])	// �O��ƈ���Ă���
			& g_padState[i]);					// ��������ON�̂��
												// Release�ݒ�
		g_padRelease[i] = ((lastPadState ^ g_padState[i])	// �O��ƈ���Ă���
			& ~g_padState[i]);					// ��������ON�̂��

	}
}

//==========================================================================================================//
// NAME: VOID UpdatePad(void)
// DESC: �p�b�h�̍X�V
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
		g_padState[i] = 0x00000000l;	// ������

		result = g_pGamePadDevice[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}

		result = g_pGamePadDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &g_dijs[i]);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = g_pGamePadDevice[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = g_pGamePadDevice[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���(Direct Input�p)
		//* ��
		if (g_dijs[i].rgdwPOV[0] == 0)						g_padState[i] |= BUTTON_UP;
		//* ��
		if (g_dijs[i].rgdwPOV[0] == 18000)					g_padState[i] |= BUTTON_DOWN;
		//* ��
		if (g_dijs[i].rgdwPOV[0] == 27000)					g_padState[i] |= BUTTON_LEFT;
		//* �E
		if (g_dijs[i].rgdwPOV[0] == 9000)					g_padState[i] |= BUTTON_RIGHT;
		//* ����
		if (g_dijs[i].rgdwPOV[0] == 31500)					g_padState[i] |= BUTTON_UPLEFT;
		//* �E��
		if (g_dijs[i].rgdwPOV[0] == 4500)					g_padState[i] |= BUTTON_UPRIGHT;
		//* ����
		if (g_dijs[i].rgdwPOV[0] == 22500)					g_padState[i] |= BUTTON_DOWNLEFT;
		//* �E��
		if (g_dijs[i].rgdwPOV[0] == 13500)					g_padState[i] |= BUTTON_DOWNRIGHT;
		//* f310 �w�{�^��
		if (g_dijs[i].rgbButtons[0] & 0x80)					g_padState[i] |= BUTTON_X;
		//* f310 �`�{�^��
		if (g_dijs[i].rgbButtons[1] & 0x80)					g_padState[i] |= BUTTON_A;
		//* f310 �a�{�^��
		if (g_dijs[i].rgbButtons[2] & 0x80)					g_padState[i] |= BUTTON_B;
		//* f310 �x�{�^��
		if (g_dijs[i].rgbButtons[3] & 0x80)					g_padState[i] |= BUTTON_Y;
		//* f310 LB�{�^��
		if (g_dijs[i].rgbButtons[4] & 0x80)					g_padState[i] |= BUTTON_LB;
		//* f310 RB�{�^��
		if (g_dijs[i].rgbButtons[5] & 0x80)					g_padState[i] |= BUTTON_RB;
		//* f310 LT�{�^��
		if (g_dijs[i].rgbButtons[6] & 0x80)					g_padState[i] |= BUTTON_LT;
		//* f310 RT�{�^��
		if (g_dijs[i].rgbButtons[7] & 0x80)					g_padState[i] |= BUTTON_RT;
		//* f310 BACK�{�^��
		if (g_dijs[i].rgbButtons[8] & 0x80)					g_padState[i] |= BUTTON_BACK;
		//* f310 START�{�^��
		if (g_dijs[i].rgbButtons[9] & 0x80)					g_padState[i] |= BUTTON_START;
		//* f310 L3�{�^��
		if (g_dijs[i].rgbButtons[10] & 0x80)				g_padState[i] |= BUTTON_L3;
		//* f310 R3�{�^��
		if (g_dijs[i].rgbButtons[11] & 0x80)				g_padState[i] |= BUTTON_R3;
		// Trigger�ݒ�
		g_padTrigger[i] = ((lastPadState ^ g_padState[i])	// �O��ƈ���Ă���
			& g_padState[i]);					// ��������ON�̂��
												// Release�ݒ�
		g_padRelease[i] = ((lastPadState ^ g_padState[i])	// �O��ƈ���Ă���
			& ~g_padState[i]);					// ��������ON�̂��

	}
}

//==========================================================================================================//
// NAME: BOOL IsButtonPressed(int padNo, DWORD button)
// DESC: �p�b�h�̃v���X��Ԃ��擾
//==========================================================================================================//
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & g_padState[padNo]);
}

//==========================================================================================================//
// NAME: BOOL IsButtonTriggered(int padNo, DWORD button)
// DESC: �p�b�h�̃g���K�[��Ԃ��擾
//==========================================================================================================//
BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & g_padTrigger[padNo]);
}

//==========================================================================================================//
// NAME: BOOL IsButtonRelease(int padNo, DWORD button)
// DESC: �p�b�h�̃����|�X��Ԃ��擾
//==========================================================================================================//
BOOL IsButtonRelease(int padNo, DWORD button)
{
	return (button & g_padRelease[padNo]);
}

//==========================================================================================================//
// NAME: LONG GetXInput_RStickX(int padNo)
// DESC: XI���[�h �p�b�h��R�X�e�B�b�NX���ړ��ʎ擾
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
// DESC: XI���[�h �p�b�h��R�X�e�B�b�NY���ړ��ʎ擾
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
// DESC: XI���[�h �p�b�h��L�X�e�B�b�N���ړ��ʎ擾
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
// DESC: DI���[�h �p�b�h��R�X�e�B�b�NX���ړ��ʎ擾
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
// DESC: DI���[�h �p�b�h��R�X�e�B�b�NY���ړ��ʎ擾
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
// DESC: DI���[�h �p�b�h��R�X�e�B�b�NX���ړ��ʎ擾
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
// DESC: DI���[�h �p�b�h��R�X�e�B�b�NX���ړ��ʎ擾
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