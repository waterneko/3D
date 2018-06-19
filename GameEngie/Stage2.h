
//=============================================================================//
// 
// Name : Stage2.h
// Describe : �X�e�[�W2����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _STAGE2_H_
#define _STAGE2_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������
#include "Input.h"					// ���͏���

#include "GameStage.h"				// �Q�[���X�e�[�W����

#include "Scene2A.h"				// �V�[��2A����

#include "FadeTexture.h"			// �t�F�[�h�e�N�X�`������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }

// �X�e�[�W2�����N���X
class Stage2 : public StageInstance
{
private:
	LPDIRECT3DDEVICE9		pDevice; 						// �f�o�C�X�I�u�W�F�N�g

	SceneInstance*			pStageScene;					// �X�e�[�W2�V�[���N���X

	GTexture*				pGTexture[MAXNUM_GAMETEXTURE];	// �Q�[���e�N�X�`���N���X�ւ̃|�C���^�[
	GFadeTexture*			pFade;							// �t�F�[�h�e�N�X�`������(��)
public:
	Stage2(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~Stage2();												// �f�X�g���N�^
public:
	HRESULT	LoadStage();									// �X�e�[�W�̓Ǎ���
	VOID UninitStage();										// �X�e�[�W�̉��

	VOID UpdateController();								// �X�e�[�W�R���g���[���[�̑��X�V
	VOID UpdateStage();										// �X�e�[�W�̑��X�V
	VOID RenderStage();										// �X�e�[�W�̑��`��
private:
	VOID UpdateFade();										// �X�e�[�W�̃t�F�[�h�������� 

	// ��Ԉڍs����
	VOID ChangeScene();										// �V�[���`�F���W����
	VOID ChangeStage();										// �X�e�[�W�`�F���W����
	HRESULT ReloadScene();									// �V�[�������[�h����

	VOID ReturnTitle();										// �^�C�g����߂�����

	VOID DebugImgui();										// �X�e�[�W�f�o�b�O�pIMGUI
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif