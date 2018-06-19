
//=============================================================================//
// 
// Name : Stage2.cpp
// Describe : �X�e�[�W2����
// Author :
// Create Date : 2018/05/01
// Modify Date : 2018/05/25
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Stage2.h"				// �X�e�[�W1����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: Stage2::Stage2(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �X�e�[�W2���� �R���X�g���N�^
//-----------------------------------------------------------------------------
Stage2::Stage2(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iScene = GameStage2A;
	// {�t�F�[�h�e�N�X�`��}
	pFade = new GFadeTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: Stage2::~Stage2()
// DESC: �X�e�[�W2���� �f�X�g���N�^
//-----------------------------------------------------------------------------
Stage2::~Stage2()
{
	// �X�e�[�W�������
	UninitStage();

	// {�t�F�[�h�e�N�X�`��}
	SAFE_DELETE(pFade)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Stage2::LoadStage()
// DESC: �X�e�[�W2���� �X�e�[�W�̓Ǎ���
//-----------------------------------------------------------------------------
HRESULT	Stage2::LoadStage()
{
	if (iCurrentState == SS_Loading)
	{
		if (pSourceManager->GetLoadingProcess() == 3)
		{
			// x�t�@�C���̓Ǎ���
			pSourceManager->LoadXfile(2, PATH_X_CORRIDOR, "Corridor.x");
		}
		else if (pSourceManager->GetLoadingProcess() == 6)
		{
			// �A�j���[�V����x�t�@�C���̓Ǎ���
			pSourceManager->LoadAnimXfile(0, PATH_ANIM_YUKARI, "yukari_adv2.x", D3DINDEXED);
		}
		else if (pSourceManager->GetLoadingProcess() == 9)
		{
			// �e�N�X�`���̓Ǎ���
			pSourceManager->LoadTexturefile(7, PATH_STAGETEXTURE, "yukarinormal.png");
		}
		else if (pSourceManager->GetLoadingProcess() == 12)
		{
			// {�Q�[���e�N�X�`���̏�����}
			pGTexture[0] = new GTexture(pDevice);
			pGTexture[0]->InitGameTexture(D3DXVECTOR2(124.0f, 154.0f), D3DXVECTOR2(90.0f, 90.0f));

			// {�t�F�[�h�e�N�X�`���̏�����}
			pFade->InitFadeTexture();
			pFade->SetAlpha(0);
		}
		else if (pSourceManager->GetLoadingProcess() == 15)
		{
			// {�X�e�[�W2�V�[���̏�����}
			pStageScene = new Scene2A(pDevice);
			pStageScene->InitScene();
		}
		else if (pSourceManager->GetLoadingProcess() == 20)
		{
			iTargetState = SS_Ready;
			iCurrentState = iTargetState;
		}

		pSourceManager->AddLoadingProcess(1);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::UninitStage()
// DESC: �X�e�[�W2���� �X�e�[�W�̉��
//-----------------------------------------------------------------------------
VOID Stage2::UninitStage()
{
	// {�Q�[���e�N�X�`���̉��}
	SAFE_DELETE(pGTexture[0])
	// {�t�F�[�h�e�N�X�`���̉��} 
	SAFE_DELETE(pFade)
	// �V�[���̉��
	SAFE_DELETE(pStageScene)

	// �e�N�X�`���̉��
	pSourceManager->FreeTextureFile(7);
	// x�t�@�C���̉��
	pSourceManager->FreeXfile(2);
	//  �A�j���[�V����x�t�@�C���̉��
	pSourceManager->FreeAnimXfile(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::UpdateController()
// DESC: �X�e�[�W2���� �X�e�[�W�R���g���[���[�̑��X�V����
//-----------------------------------------------------------------------------
VOID Stage2::UpdateController()
{
	if (!pStageScene->GetChangeSceneFlag())
	{
		if (pFade->iFadeState == FS_Ready)
		{
			// {�V�[���̃R���g���[���[�X�V}
			pStageScene->UpdateController();
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::::UpdateStage()
// DESC: �X�e�[�W2���� �X�e�[�W�̑��X�V����
//-----------------------------------------------------------------------------
VOID Stage2::UpdateStage()
{
	// �X�e�[�W�̃t�F�[�h����
	UpdateFade();

	// �V�[���`�F���W�`�F�b�N
	if (pStageScene->GetChangeSceneFlag())
	{
		// �^�C�g���߂�
		if (pStageScene->GetNextScene() == -1)
		{
			ReturnTitle();
		}
		// �V�[���ړ�
		else if (pStageScene->GetNextScene() > 4)
		{
			ChangeScene();
		}
		// �X�e�[�W�ړ�
		else if ( (pStageScene->GetNextScene() <= 4) && (pStageScene->GetNextScene() > 0) )
		{
			ChangeStage();
		}
	}
		
	// �V�[���`�F���W����
	if (iCurrentState == SS_ChangeScene)
	{
		// {�Q�[���V�[���ؑւ�}
		switch (iScene)
		{
		case GameStage2A:
			SAFE_DELETE(pStageScene)
			pStageScene = new Scene2A(pDevice);
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
// NAME: VOID Stage2::RenderStage()
// DESC: �X�e�[�W2���� �X�e�[�W�̑��`��
//-----------------------------------------------------------------------------
VOID Stage2::RenderStage()
{
	// {�V�[���̕`��}
	if (iCurrentState == SS_Ready)
	{
		pStageScene->RenderScene();

		// {�Q�[���e�N�X�`���̍X�V}
		pGTexture[0]->RenderGameTexture(SetTexture(7));
	}

	// {�t�F�[�h�e�N�X�`���̕`��}
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(false);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::UpdateFade()
// DESC: �X�e�[�W2���� �X�e�[�W�̃t�F�[�h����
//-----------------------------------------------------------------------------
VOID Stage2::UpdateFade()
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
// NAME: VOID Stage2::ChangeScene()
// DESC: �X�e�[�W2���� �V�[���`�F���W����
//-----------------------------------------------------------------------------
VOID Stage2::ChangeScene()
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
// NAME: HRESULT Stage2::DebugImgui(UINT scene)
// DESC: �X�e�[�W2����  �V�[�������[�h����
//-----------------------------------------------------------------------------
HRESULT Stage2::ReloadScene()
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
// NAME: VOID Stage2::ChangeScene()
// DESC: �X�e�[�W2���� �X�e�[�W�`�F���W����
//-----------------------------------------------------------------------------
VOID Stage2::ChangeStage()
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
// NAME: VOID Stage2::ReturnTitle()
// DESC: �X�e�[�W2���� �^�C�g���߂�����
//-----------------------------------------------------------------------------
VOID Stage2::ReturnTitle()
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
VOID Stage2::DebugImgui()
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