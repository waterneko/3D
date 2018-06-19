//=============================================================================//
// 
// Name : Input.h
// Describe : ���͏���
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD										// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE											// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD												// �錾����ƃp�b�h�ő���\�ɂȂ�


// �p�b�g�L�[(16�i���g���ĂS�L�[�P�œ��͔��f����)
#define BUTTON_UP			0x00000001l	// �����L�[��(XI IY<0 && IX ==0) or (DI Pov 0)
#define BUTTON_DOWN			0x00000002l	// �����L�[��(XI IY>0 && IX ==0) or (DI Pov 18000)
#define BUTTON_LEFT			0x00000004l	// �����L�[��(XI IX<0 && IY ==0) or (DI Pov 27000)
#define BUTTON_RIGHT		0x00000008l	// �����L�[�E(XI IX>0 && IY ==0) or (DI Pov 9000)
#define BUTTON_UPLEFT		0x00000010l	// �����L�[����(XI IY<0 && IX<0) or (DI Pov 4500)
#define BUTTON_UPRIGHT		0x00000020l	// �����L�[�E��(XI IY<0 && IX>0) or (DI Pov 31500)
#define BUTTON_DOWNLEFT		0x00000040l	// �����L�[����(XI IY>0 && IX<0) or (DI Pov 22500)
#define BUTTON_DOWNRIGHT	0x00000080l	// �����L�[����(XI IY>0 && IX<0) or (DI Pov 13500)

#define BUTTON_RS_UP		0x00000100l	// �X�e�B�b�N��(XI IRy < 0) or (DI IZ < 0)
#define BUTTON_RS_DOWN		0x00000200l	// �X�e�B�b�N��(XI IRy > 0) or (DI IZ > 0)
#define BUTTON_RS_LEFT		0x00000400l	// �X�e�B�b�N��(XI lRx < 0) or (DI lRz < 0)
#define BUTTON_RS_RIGHT		0x00000800l	// �X�e�B�b�N�E(XI lRx > 0) or (DI lRz > 0)
#define BUTTON_A			0x00001000l	// �`�{�^��(XI rgbButtons[0]&0x80) or (DI rgbButtons[0]&0x80)
#define BUTTON_B			0x00002000l	// �a�{�^��(XI rgbButtons[1]&0x80) or (DI rgbButtons[1]&0x80)
#define BUTTON_Y			0x00004000l	// �x�{�^��(XI rgbButtons[2]&0x80) or (DI rgbButtons[2]&0x80)
#define BUTTON_X			0x00008000l	// �w�{�^��(XI rgbButtons[3]&0x80) or (DI rgbButtons[3]&0x80)
#define BUTTON_LB			0x00010000l	// LB�{�^��(XI rgbButtons[4]&0x80) or (DI rgbButtons[4]&0x80)
#define BUTTON_RB			0x00020000l	// RB�{�^��(XI rgbButtons[5]&0x80) or (DI rgbButtons[5]&0x80)
#define BUTTON_LT			0x00040000l	// LT�{�^��(XI IZ<0) or (DI rgbButtons[6]&0x80)
#define BUTTON_RT			0x00080000l	// RT�{�^��(XI IZ>0) or (DI rgbButtons[7]&0x80)
#define BUTTON_BACK			0x00100000l	// BACK�{�^��(XI rgbButtons[6]&0x80) or (DI rgbButtons[8]&0x80)
#define BUTTON_START		0x00200000l	// START�{�^��(XI rgbButtons[7]&0x80) or (DI rgbButtons[9]&0x80)
#define BUTTON_L3			0x00400000l	// L3�{�^��(XI rgbButtons[8]&0x80) or (DI rgbButtons[10]&0x80)
#define BUTTON_R3			0x00800000l	// R3�{�^��(XI rgbButtons[9]&0x80) or (DI rgbButtons[11]&0x80)
#define BUTTON_FIX_IRX		(-4)	// �X�e�B�b�NX�C��
#define BUTTON_FIX_IRY		(4)		// �X�e�B�b�NY�C��
#define GAMEPADMAX			4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g


//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//
HRESULT InitInput(HWND hWnd, bool DInput);      // ���͏���������������
VOID UpdateInput(bool DInput);					// ���͏������X�V����
VOID UninitInput(void);							// ���͏������I������

// [�L�[�{�[�h]
BOOL GetKeyboardPress(int nKey);				// �L�[�v���X���(���������)
BOOL GetKeyboardTrigger(int nKey);				// �L�[�g���K�[���(�������u��)
BOOL GetKeyboardRepeat(int nKey);				// �L�[���s�[�g���(�A��)
BOOL GetKeyboardRelease(int nKey);				// �L�[�����|�X���(�������u��)

// [�}�E�X]
BOOL IsMouseLeftPressed(void);      // ���N���b�N�������
BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
LONG  GetMouseX(void);              // �}�E�X��X�����ɓ��������Βl
LONG  GetMouseY(void);              // �}�E�X��Y�����ɓ��������Βl
LONG  GetMouseZ(void);              // �}�E�X�z�C�[�������������Βl

// [�p�b�g]
BOOL IsButtonPressed(int padNo, DWORD button);		// �{�^���v���X���(���������)
BOOL IsButtonTriggered(int padNo, DWORD button);	// �{�^���g���K�[���(�������u��)
BOOL IsButtonRelease(int padNo, DWORD button);		// �{�^�������|�X���(�������u��)

LONG GetXInput_RStickX(int padNo);					// �p�b�h��R�X�e�B�b�NX���ړ��ʎ擾
LONG GetXInput_RStickY(int padNo);					// �p�b�h��R�X�e�B�b�NY���ړ��ʎ擾
LONG GetXInput_LStick(int padNo);					// �p�b�h��L�X�e�B�b�N���ړ��ʎ擾

LONG GetDInput_RStickX(int padNo);					// �p�b�h��R�X�e�B�b�NX���ړ��ʎ擾
LONG GetDInput_RStickY(int padNo);					// �p�b�h��R�X�e�B�b�NY���ړ��ʎ擾
LONG GetDInput_LStickX(int padNo);					// �p�b�h��L�X�e�B�b�NX���ړ��ʎ擾
LONG GetDInput_LStickY(int padNo);					// �p�b�h��L�X�e�B�b�NY���ړ��ʎ擾
#endif