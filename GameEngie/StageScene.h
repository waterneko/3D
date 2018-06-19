
//=============================================================================//
// 
// Name : StageScene.h
// Describe : �X�e�[�W�V�[������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _STAGESCENE_H_
#define _STAGESCENE_H_

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
// �V�[�����
enum GameStageSceneState
{
	SSS_Normal = 0,	// �V�[�����s
	SSS_Event,		// �V�[���C�x���g
	SSS_Battle,		// �V�[���퓬
	SSS_Pause,		// �V�[���x�~
};

// �X�e�[�W�V�[��
enum StageSceneIndex
{
	GameDefaultStage = 0,
	GameStage1A,
	GameStage1B,
	GameStage2A = 5,
	GameStage2B,
};
// { �\���� }

// { �N���X }

// �X�e�[�W�V�[�������N���X
class SceneInstance
{
protected:
	SourceManager*			pSourceManager;						// �\�[�X�Ǘ�

	BOOL					bChangeFlag;						// �V�[���؂�ւ��t���O

	UINT					iNextScene;							// ���̃V�[���ԍ�
public:
	SceneInstance();											// �R���X�g���N�^
	~SceneInstance();											// �f�X�g���N�^
public:
	virtual HRESULT	InitScene();								// �V�[���̏�����
	virtual VOID UpdateController();							// �V�[���X�e�[�W�R���g���[���[�̑��X�V
	virtual VOID UpdateScene();									// �V�[���X�e�[�W�̑��X�V
	virtual VOID RenderScene();									// �V�[���X�e�[�W�̑��`��

	// [�V�[���ؑւ��֐�]
	VOID SetChangeSceneFlag(bool change);						// �V�[���؂�ւ��t���O�ݒu
	VOID SetNextScene(UINT nextscene);							// �V�[���؂�ւ�

	// [�ݒu�֐�]
	LPDIRECT3DTEXTURE9 SetTexture(UINT n);						// �e�N�X�`���ݒu
	SXfile* SetXfile(UINT n);									// x�t�@�C���ݒu
	SAnimXfile* SetAnimXfile(UINT n);							// �A�j���[�V����x�t�@�C���ݒu
	SWavfile* SetWavfile(UINT n);								// Wav�t�@�C���ݒu

	// [�擾�֐�]
	BOOL GetChangeSceneFlag() { return bChangeFlag; };			// �V�[���؂�ւ��t���O
	UINT GetNextScene() { return iNextScene; };					// ���̃V�[���ԍ�
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif