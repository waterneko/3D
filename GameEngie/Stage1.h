
//=============================================================================//
// 
// Name : Stage1.h
// Describe : �X�e�[�W1����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _STAGE1_H_
#define _STAGE1_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������
#include "Input.h"					// ���͏���
#include "XA2SourceVoice.h"			// XAudio2�\�[�X�{�C�X����

#include "GameStage.h"				// �Q�[���X�e�[�W����

#include "Scene1A.h"				// �V�[��1A����
#include "Scene1B.h"				// �V�[��1B����

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

// �X�e�[�W1�����N���X
class Stage1 : public StageInstance
{
private:
	LPDIRECT3DDEVICE9	pDevice; 							// �f�o�C�X�I�u�W�F�N�g

	SceneInstance*		pStageScene;						// �X�e�[�W1�V�[���N���X

	BXA2SourceVoice*	pSourceVoice[MAXNUM_SOURCEVOICE];	// XA2�\�[�X�{�C�X�ւ̃|�C���^�[

	GTexture*			pGTexture[MAXNUM_GAMETEXTURE];		// �Q�[���e�N�X�`���N���X�ւ̃|�C���^�[
	GFadeTexture*		pFade;								// �t�F�[�h�e�N�X�`������(��)
public:
	Stage1(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~Stage1();												// �f�X�g���N�^
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

	VOID ReturnTitle();										// �^�C�g����߂�����(�X�^�[�g/ ���j���[)����

	VOID DebugImgui();										// �X�e�[�W�f�o�b�O�pIMGUI
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif