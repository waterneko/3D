
//=============================================================================//
// 
// Name : Game.h
// Describe : �Q�[��������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "SourceManager.h"			// �\�[�X�Ǘ�

#include "GameTitle.h"				// �Q�[���^�C�g������

#include "Stage1.h"					// �Q�[���X�e�[�W����
#include "Stage2.h"					// �Q�[���X�e�[�W����

#include "ScreenGauge.h"			// �X�N���[���Q�[�W����
#include "FadeTexture.h"			// �t�F�[�h�e�N�X�`������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define FIRST_GAMESTATE			(GS_Title)				// �ŏ��̃Q�[�����

#define MAX_TITLE_PROCESS		(10)					// �^�C�g���ő働�[�h�v���Z�X�l
#define MAX_STAGE_PROCESS		(20)					// �X�e�[�W�ő働�[�h�v���Z�X�l

#define	PATH_LOADINGTEXTURE		("Data/Texture/Other/Loading/")		// ���[�h�p�e�N�X�`���t�@�C���p�X
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �Q�[�����
enum GameState
{
	// �Q�[���^�C�g��
	GS_Title = 1,
	// �Q�[���{��
	GS_Game,
	GS_GameReload
};

// �X�e�[�W�ԍ�
enum StageIndex
{
	GameStage1 = 1,
	GameStage2,
};

// { �\���� }

// { �N���X }

// �Q�[���������N���X
class GameInstance
{
private:
	LPDIRECT3DDEVICE9		pDevice;							// �f�o�C�X�I�u�W�F�N�g

	UINT					iCurrentState;						// ���݂̃Q�[�����
	UINT					iTargetState;						// ���̃Q�[�����

	UINT					iLoadingTexture;					// ���[�h��ʃe�N�X�`���ԍ�

	SourceManager*			pSourceManager;						// �\�[�X�Ǘ�

	TitleInstance*			pTitleInstance;						// �^�C�g������
	StageInstance*			pStageInstance;						// �X�e�[�W����

	GTexture*				pGLoadingTexture[MAXNUM_GAMETEXTURE];// �Q�[���e�N�X�`���N���X�ւ̃|�C���^�[
	GScreenGauge*			pLoadingGauge;						// ���[�h��ʗp�Q�[�W
	GFadeTexture*			pFade;								// �t�F�[�h�e�N�X�`������(��)
public:
	GameInstance(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~GameInstance();											// �f�X�g���N�^
public:
	HRESULT	InitGame();											// �Q�[���̏���������

	VOID UpdateGame();											// �Q�[���̍X�V����
	VOID RenderGame();											// �Q�[���̕`�揈��

	VOID MoveGameState(int nextstate);							// �Q�[���̏�Ԑ؂�ւ�
private:
	// ��Ԉڍs����
	BOOL StartCheck();											// �Q�[���J�n�ڍs�����`�F�b�N
	BOOL ChangeStageCheck();									// �X�e�[�W�ؑւ������`�F�b�N
	BOOL ReturnCheck();											// �^�C�g���߂������`�F�b�N

	VOID DebugImgui();											// �f�o�b�O�pIMGUI����
	VOID UpdateFade();											// �Q�[���̏�Ԉڍs�̃t�F�[�h����
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif