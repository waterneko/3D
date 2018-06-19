
//=============================================================================//
// 
// Name : GameController.h
// Describe : �Q�[���R���g���[���[����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "Input.h"					// �X�N���[���e�N�X�`������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �Q�[���p�b�g���[�h
enum PadMode
{
	PM_DInput = 0,
	PM_XInput
};
// { �\���� }

// { �N���X }
// �Q�[���R���g���[���[�N���X
class GController
{
private:
	D3DXVECTOR3 vLstick;			// L�X�e�B�b�N���W
	D3DXVECTOR3 vRstick;			// R�X�e�B�b�N���W
	D3DXVECTOR3 vFixLstick;			// �J�����C��L�X�e�B�b�N���W

	D3DXMATRIX mtxCameraY;			// �J����Y����]�}�g���N�X
public:
	UINT						iPadState;				// �Q�[���p�b�g���[�h
	UINT						iPadNum;				// �p�b�g�ԍ�
public:
	GController(UINT num);								// �R���X�g���N�^
	~GController();										// �f�X�g���N�^
public:
	VOID SetPadMode(UINT state);							// �Q�[���p�b�g���[�h�ݒu
	VOID UpdateFixLstick(D3DXVECTOR3 rot);					// �J����Y����]�C���̍X�V

	BOOL PadAButton(bool press);							// �Q�[���p�b�gA�{�^�� ( = �L�����Z���{�^�� )
	BOOL PadBButton(bool press);							// �Q�[���p�b�gB�{�^�� ( = �m��{�^�� )
	BOOL PadXButton(bool press);							// �Q�[���p�b�gX�{�^��
	BOOL PadYButton(bool press);							// �Q�[���p�b�gY�{�^��
	BOOL PadUpButton();										// �Q�[���p�b�g��ړ��{�^�� ( = ���{�^�� )
	BOOL PadDownButton();									// �Q�[���p�b�g���ړ��{�^�� ( = ���{�^�� )
	BOOL PadLeftButton();									// �Q�[���p�b�g���ړ��{�^�� ( = ���{�^�� )
	BOOL PadRightButton();									// �Q�[���p�b�g�E�ړ��{�^�� ( = ���{�^�� )
	BOOL PadUpLeftButton();									// �Q�[���p�b�g����ړ��{�^�� ( = �����{�^�� )
	BOOL PadUpRightButton();								// �Q�[���p�b�g�E��ړ��{�^�� ( = �����{�^�� )
	BOOL PadDownLeftButton();								// �Q�[���p�b�g�����ړ��{�^�� ( = �����{�^�� )
	BOOL PadDownRightButton();								// �Q�[���p�b�g�E���ړ��{�^�� ( = �����{�^�� )

	BOOL PadLBButton(bool press);							// �Q�[���p�b�gLB�{�^�� ( = LB�{�^�� )
	BOOL PadRBButton(bool press);							// �Q�[���p�b�gRB�{�^�� ( = RB�{�^�� )
	BOOL PadLTButton(bool press);							// �Q�[���p�b�gLB�{�^�� ( = LB�{�^�� )
	BOOL PadRTButton(bool press);							// �Q�[���p�b�gRB�{�^�� ( = RB�{�^�� )
	BOOL PadStartButton(bool press);						// �Q�[���p�b�gSTART�{�^��
	BOOL PadBackButton(bool press);							// �Q�[���p�b�gBACK�{�^��

	BOOL ReleasePadMoveButton();							// �ړ��{�^�������
	D3DXVECTOR3 GetLstick() { return vLstick; };			// L�X�e�B�b�N���W���擾����
	D3DXVECTOR3 GetRstick() { return vRstick; };			// R�X�e�B�b�N���W���擾����
	D3DXVECTOR3 GetFixLstick() { return vFixLstick; };		// �J�����C��L�X�e�B�b�N���W���擾����
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif