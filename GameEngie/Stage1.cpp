
//=============================================================================//
// 
// Name : Stage1.cpp
// Describe : �X�e�[�W1����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Stage1.h"				// �X�e�[�W1����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: Stage1::Stage1(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �X�e�[�W1���� �R���X�g���N�^
//-----------------------------------------------------------------------------
Stage1::Stage1(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iScene = GameStage1A;
	// {�t�F�[�h�e�N�X�`��}
	pFade = new GFadeTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: Stage1::~Stage1()
// DESC: �X�e�[�W1���� �f�X�g���N�^
//-----------------------------------------------------------------------------
Stage1::~Stage1()
{
	// �X�e�[�W�������
	UninitStage();

	// {�t�F�[�h�e�N�X�`��}
	SAFE_DELETE(pFade)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Stage1::LoadStage()
// DESC: �X�e�[�W1���� �X�e�[�W�̓Ǎ���
//-----------------------------------------------------------------------------
HRESULT	Stage1::LoadStage()
{
	if (iCurrentState == SS_Loading)
	{
		if (pSourceManager->GetLoadingProcess() == 3)
		{
			// x�t�@�C���̓Ǎ���
			pSourceManager->LoadXfile(2, PATH_X_ROOM, "Room1.x");
			pSourceManager->LoadXfile(3, PATH_X_SLOPE, "Slope.x");
			pSourceManager->LoadXfile(4, PATH_X_ROOM, "Stoneroom1.x");
		}
		else if (pSourceManager->GetLoadingProcess() == 6)
		{
			// �A�j���[�V����x�t�@�C���̓Ǎ���
			pSourceManager->LoadAnimXfile(0, PATH_ANIM_YUKARI, "yukari_adv2.x", D3DINDEXED);
		}
		else if (pSourceManager->GetLoadingProcess() == 9)
		{
			// �e�N�X�`���̓Ǎ���
			pSourceManager->LoadTexturefile(7, PATH_STAGETEXTURE, "Stage1.png");
			pSourceManager->LoadTexturefile(8, PATH_STAGETEXTURE, "yukarinormal.png");
		}
		else if (pSourceManager->GetLoadingProcess() == 12)
		{
			// Wav�t�@�C���̓Ǎ���
			pSourceManager->LoadWavfile(0, PATH_BGM_WAV, "bgm_01.wav");
		}
		else if (pSourceManager->GetLoadingProcess() == 14)
		{
			// {�Q�[���e�N�X�`���̏�����}
			pGTexture[0] = new GTexture(pDevice);
			pGTexture[0]->InitGameTexture(D3DXVECTOR2(124.0f, 154.0f), D3DXVECTOR2(90.0f, 90.0f));

			// {�t�F�[�h�e�N�X�`���̏�����}
			pFade->InitFadeTexture();
			pFade->SetAlpha(0);

		}
		else if (pSourceManager->GetLoadingProcess() == 16)
		{
			// {�X�e�[�W1�V�[���̏�����}
			pStageScene = new Scene1A(pDevice);
			pStageScene->InitScene();
		}
		else if (pSourceManager->GetLoadingProcess() == 18)
		{
			// {XAudio2�\�[�X�{�C�X�̏�����}
			pSourceVoice[0] = new BXA2SourceVoice();
			pSourceVoice[0]->InitSourceVoice(SetWavfile(0), true);
		}
		else if (pSourceManager->GetLoadingProcess() == 20)
		{
			iTargetState = SS_Ready;
			iCurrentState = iTargetState;
			// {XAudio2�\�[�X�{�C�X�̍Đ�}
			pSourceVoice[0]->PlaySourceVoice();
		}

		pSourceManager->AddLoadingProcess(1);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::UninitStage()
// DESC: �X�e�[�W1���� �X�e�[�W�̉��
//-----------------------------------------------------------------------------
VOID Stage1::UninitStage()
{
	// {�Q�[���e�N�X�`���̉��}
	SAFE_DELETE(pGTexture[0])
	// {�t�F�[�h�e�N�X�`���̉��} 
	SAFE_DELETE(pFade)
	// {XAudio2�\�[�X�{�C�X�̉��}
	for (int i = 0; pSourceVoice[i] != NULL; i++)
	{
		SAFE_DELETE(pSourceVoice[i])
	}
	// �V�[���̉��
	SAFE_DELETE(pStageScene)

	// �e�N�X�`���̉��
	pSourceManager->FreeTextureFile(7);
	pSourceManager->FreeTextureFile(8);
	// x�t�@�C���̉��
	pSourceManager->FreeXfile(2);
	pSourceManager->FreeXfile(3);
	pSourceManager->FreeXfile(4);
	// �A�j���[�V����x�t�@�C���̉��
	pSourceManager->FreeAnimXfile(0);
	// Wav�t�@�C���̉��
	pSourceManager->FreeWavfile(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::UpdateController()
// DESC: �X�e�[�W1���� �X�e�[�W�R���g���[���[�̑��X�V����
//-----------------------------------------------------------------------------
VOID Stage1::UpdateController()
{
	if (!pStageScene->GetChangeSceneFlag())
	{
		if (pFade->iFadeState == FS_Ready)
		{
			// {�V�[���̃R���g���[���[�X�V}
			pStageScene->UpdateController();
		}

		// BGM�R���g���[��
		if (GetKeyboardTrigger(DIK_F7))
		{
			pSourceVoice[0]->RestartSourceVoice();
		}
		else if (GetKeyboardTrigger(DIK_F8))
		{
			pSourceVoice[0]->PauseSourceVoice();
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::UpdateStage()
// DESC: �X�e�[�W1���� �X�e�[�W�̑��X�V����
//-----------------------------------------------------------------------------
VOID Stage1::UpdateStage()
{
	// �X�e�[�W�̃t�F�[�h����
	UpdateFade();

	// ���݂̃V�[������
	// �V�[���`�F���W����
	if (pStageScene->GetChangeSceneFlag())
	{
		// �^�C�g���߂�
		if (pStageScene->GetNextScene() == -1)
		{
			ReturnTitle();
		}
		// �V�[���ړ�
		else if ((pStageScene->GetNextScene() <= 4) && (pStageScene->GetNextScene() > 0))
		{
			ChangeScene();
		}
		// �X�e�[�W�ړ�
		else if (pStageScene->GetNextScene() > 4) 
		{
			ChangeStage();
		}
	}

	if ((iCurrentState == SS_ChangeScene))
	{
		// {�Q�[���V�[���ؑւ�}
		switch (iScene)
		{
		case GameStage1A:
			SAFE_DELETE(pStageScene)
			pStageScene = new Scene1A(pDevice);
			ReloadScene();
			break;
		case GameStage1B:
			SAFE_DELETE(pStageScene)
			pStageScene = new Scene1B(pDevice);
			ReloadScene();
			break;
		}
	}

	// ���݂̃V�[������
	else
	{
		// {�V�[���̍X�V}
		pStageScene->UpdateScene();

		// {�Q�[���e�N�X�`���̍X�V}
		pGTexture[0]->UpdateGameTexture();
	}


	// �f�o�b�O�pIMGUI
	DebugImgui();
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::RenderStage()
// DESC: �X�e�[�W1���� �X�e�[�W�̑��`��
//-----------------------------------------------------------------------------
VOID Stage1::RenderStage()
{
	// {�V�[���̕`��}
	if (iCurrentState == SS_Ready)
	{
		pStageScene->RenderScene();

		// {�Q�[���e�N�X�`���̍X�V}
		pGTexture[0]->RenderGameTexture(SetTexture(8));
	}

	// {�t�F�[�h�e�N�X�`���̕`��}
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(false);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::UpdateFade()
// DESC: �X�e�[�W1���� �X�e�[�W�̃t�F�[�h����
//-----------------------------------------------------------------------------
VOID Stage1::UpdateFade()
{
	if (iCurrentState != iTargetState)
	{
		// �t�F�[�h�C���I������
		if ((pFade->iFadeState == FS_FadeIn) && (pFade->GetAlphaValue() == 255))
		{
			// �X�e�[�W��ԍX�V
			iCurrentState = iTargetState;
		}
	}

	// �t�F�[�h�A�E�g�I������
	if ((pFade->iFadeState == FS_FadeOut) && (pFade->GetAlphaValue() == 0))
	{
		// �t�F�[�h��ԉ�
		pFade->iFadeState = FS_Ready;
	}

	// �t�F�[�h�e�N�X�`���̍X�V
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->UpdateFadeTexture();
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::ChangeScene()
// DESC: �X�e�[�W1���� �V�[���`�F���W����
//-----------------------------------------------------------------------------
VOID Stage1::ChangeScene()
{
	if (pFade->iFadeState == FS_Ready)
	{
		// �V�[���ύX
		iScene = pStageScene->GetNextScene();
		// �V�[���`�F���W�J�n
		iTargetState = SS_ChangeScene;
		// �t�F�[�h�C���J�n����
		pFade->iFadeState = FS_FadeIn;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Stage1::DebugImgui(UINT scene)
// DESC: �X�e�[�W1����  �V�[�������[�h����
//-----------------------------------------------------------------------------
HRESULT Stage1::ReloadScene()
{
	if (pStageScene != NULL)
	{
		// �V�[��������
		if (SUCCEEDED(pStageScene->InitScene()))
		{
			iTargetState = SS_Ready;
			// �V�[����߂�
			pStageScene->SetNextScene(GameDefaultStage);
			// �V�[���ؑւ��t���O�I��
			pStageScene->SetChangeSceneFlag(false);
			// �t�F�[�h�A�E�g�J�n����
			pFade->iFadeState = FS_FadeOut;
			// �X�e�[�W��ԍX�V
			iCurrentState = iTargetState;

			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::ChangeScene()
// DESC: �X�e�[�W1���� �X�e�[�W�`�F���W����
//-----------------------------------------------------------------------------
VOID Stage1::ChangeStage()
{
	if (pFade->iFadeState == FS_Ready)
	{
		// �V�[���ύX
		iScene = pStageScene->GetNextScene();
		// �V�[���`�F���W�J�n
		iTargetState = SS_ChangeStage;
		// �V�[����߂�
		pStageScene->SetNextScene(GameDefaultStage);
		// �V�[���ؑւ��t���O�I��
		pStageScene->SetChangeSceneFlag(false);
		// �t�F�[�h�C���J�n����
		pFade->iFadeState = FS_FadeIn;
	}
}


//-----------------------------------------------------------------------------
// NAME: VOID Stage1::ReturnTitle()
// DESC: �X�e�[�W1���� �^�C�g���߂�����
//-----------------------------------------------------------------------------
VOID Stage1::ReturnTitle()
{
	if (pFade->iFadeState == FS_Ready)
	{
		// �V�[���ύX
		iScene = 0;
		// �V�[���`�F���W�J�n
		iTargetState = SS_ReturnStart;
		// �V�[����߂�
		pStageScene->SetNextScene(GameDefaultStage);
		// �V�[���ؑւ��t���O�I��
		pStageScene->SetChangeSceneFlag(false);
		// �t�F�[�h�C���J�n����
		pFade->iFadeState = FS_FadeIn;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::DebugImgui()
// DESC: �X�e�[�W1���� �X�e�[�W�f�o�b�O�pIMGUI
//-----------------------------------------------------------------------------
VOID Stage1::DebugImgui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 380));
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin(u8"�X�e�[�W���");
	ImGui::Text(u8"���݂̃X�e�[�W��ԁF%d", iCurrentState);
	ImGui::Text(u8"���̃X�e�[�W��ԁF%d", iTargetState);
	ImGui::Text(u8"�t�F�[�h���: %d", pFade->iFadeState);
	ImGui::Text(u8"�t�F�[�h�A���t�@�l: %d", pFade->GetAlphaValue());
	ImGui::Text(u8"���݂̃V�[���F%d", iScene);
	ImGui::Text(u8"���݂̃V�[���ؑւ��t���O�F%d", pStageScene->GetChangeSceneFlag());
	ImGui::Text(u8"���̃V�[���F%d", pStageScene->GetNextScene());
	ImGui::Text(u8"Mousex: %d", GetMouseX());
	ImGui::Text(u8"Mousey�F%d", GetMouseX());
	ImGui::Text(u8"L�X�e�B�b�NX: %d", GetDInput_LStickX(0));
	ImGui::Text(u8"L�X�e�B�b�NX�F%d", GetDInput_LStickY(0));
	ImGui::Text(u8"R�X�e�B�b�NX�F%d", GetDInput_RStickX(0));
	ImGui::Text(u8"R�X�e�B�b�NX�F%d", GetDInput_RStickY(0));
	ImGui::End();
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