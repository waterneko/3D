
//=============================================================================//
// 
// Name : GameStage.h
// Describe : �Q�[���X�e�[�W����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMESTAGE_H_
#define _GAMESTAGE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������
#include "Input.h"					// ���͏���

#include "SourceManager.h"			// �\�[�X�Ǘ�
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �X�e�[�W���
enum GameStageState
{
	SS_Ready = 0,	// �Q�[���X�e�[�W�ҋ@
	SS_Loading,		// �Q�[���X�e�[�W���[�h
	SS_ChangeScene,	// �Q�[���V�[���ؑւ�
	SS_ChangeStage,	// �Q�[���X�e�[�W�ؑւ�
	SS_ReturnStart,	// �Q�[���^�C�g���ɖ߂�
};
// { �\���� }

// { �N���X }

// �Q�[���X�e�[�W�����N���X
class StageInstance
{
protected:
	UINT					iCurrentState;						// ���݂̃X�e�[�W���
	UINT					iTargetState;						// ���̃X�e�[�W���

	UINT					iScene;								// ���݂̃X�e�[�W�V�[��

	SourceManager*			pSourceManager;						// �\�[�X�Ǘ�
public:
	StageInstance();											// �R���X�g���N�^
	~StageInstance();											// �f�X�g���N�^
public:
	virtual HRESULT	LoadStage();								// �Q�[���X�e�[�W�̓Ǎ���
	virtual VOID UninitStage();									// �Q�[���X�e�[�W�̉��

	virtual VOID UpdateController();							// �Q�[���X�e�[�W�R���g���[���[�̑��X�V
	virtual VOID UpdateStage();									// �Q�[���X�e�[�W�̑��X�V
	virtual VOID RenderStage();									// �Q�[���X�e�[�W�̑��`��

	// [�ݒu�֐�]
	LPDIRECT3DTEXTURE9 SetTexture(UINT n);						// �e�N�X�`���ݒu
	SXfile* SetXfile(UINT n);									// x�t�@�C���ݒu
	SAnimXfile* SetAnimXfile(UINT n);							// �A�j���[�V����x�t�@�C���ݒu
	SWavfile* SetWavfile(UINT n);								// Wav�t�@�C���ݒu

	// [�擾�֐�]
	UINT GetCurrentState() { return iCurrentState; };			// ���݂̃X�e�[�W��Ԃ��擾����
	UINT GetTargetState() { return iTargetState; };				// ���̃X�e�[�W��Ԃ��擾����
	UINT GetScene() { return iScene; };							// ���̃V�[�����擾����

	//BOOL GetChangeSceneFlag() { return bChangeScene; };			// �V�[���؂�ւ��t���O
	//UINT GetNextSceneNum() { return iNextScene; };				// ���̃V�[���ԍ�
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif