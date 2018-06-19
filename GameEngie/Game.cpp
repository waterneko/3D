
//=============================================================================//
// 
// Name : Game.cpp
// Describe : �Q�[��������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Game.h"				// �Q�[��������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//


//-----------------------------------------------------------------------------
// NAME: GameInstance::GameInstance(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �Q�[�������� �R���X�g���N�^
//-----------------------------------------------------------------------------
GameInstance::GameInstance(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	// �\�[�X�Ǘ����擾����
	pSourceManager = SourceManager::GetInstance();
	pSourceManager->InitSourceManager(pDevice);

	iCurrentState = 0;
	iTargetState = FIRST_GAMESTATE;
	iLoadingTexture = 0;

	// {�Q�[���^�C�g��}
	pTitleInstance = new TitleInstance(pDevice);

	// {���[�h�e�N�X�`��}
	pGLoadingTexture[0] = new GTexture(pDevice);
	pGLoadingTexture[1] = new GTexture(pDevice);

	// {�X�N���[���Q�[�W}
	pLoadingGauge = new GScreenGauge(pDevice);

	// {�t�F�[�h�e�N�X�`��}
	pFade = new GFadeTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: GameInstance::~GameInstance()
// DESC: �Q�[�������� �f�X�g���N�^
//-----------------------------------------------------------------------------
GameInstance::~GameInstance()
{
	// {�Q�[���^�C�g���̉��}
	if (pTitleInstance)
	{
		pTitleInstance->UninitTitle();
		SAFE_DELETE(pTitleInstance)
	}

	// {�Q�[���X�e�[�W�̉��}
	if (pStageInstance)
	{
		pStageInstance->UninitStage();
		SAFE_DELETE(pStageInstance)
	}
	// {���[�h�e�N�X�`���̉��}
	SAFE_DELETE(pGLoadingTexture[0])
	// {�X�N���[���Q�[�W�̉��}
	SAFE_DELETE(pLoadingGauge)
	// {�t�F�[�h�e�N�X�`��}
	SAFE_DELETE(pFade)

	// �\�[�X�Ǘ����
	pSourceManager->UninitSourceManager();
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GameInstance::InitGame()
// DESC: �Q�[�������� �Q�[���̏�����
//-----------------------------------------------------------------------------
HRESULT	GameInstance::InitGame()
{
	// {�Q�[���^�C�g�����[�h�J�n}
	if (pSourceManager->GetLoadingState() == LS_Ready)
	{
		pSourceManager->ResetLoadingProcess();
		pSourceManager->SetLoadingState(LS_BeginLoad);
	}

	if (pSourceManager->GetLoadingState() == LS_BeginLoad)
	{
		pSourceManager->LoadTexturefile(0, PATH_LOADINGTEXTURE, "loading.png");
		pSourceManager->LoadTexturefile(1, PATH_LOADINGTEXTURE, "loading01.png");
		pSourceManager->LoadTexturefile(2, PATH_LOADINGTEXTURE, "loading02.png");
		pSourceManager->LoadTexturefile(3, PATH_LOADINGTEXTURE, "loading03.png");

		// {���[�h�e�N�X�`��}
		pGLoadingTexture[0] = new GTexture(pDevice);
		pGLoadingTexture[0]->InitGameTexture(D3DXVECTOR2(420.0f, 160.0f), D3DXVECTOR2(850.0f, 720.0f));

		pGLoadingTexture[1] = new GTexture(pDevice);
		pGLoadingTexture[1]->InitGameTexture(D3DXVECTOR2(1024.0f, 768.0f), D3DXVECTOR2(512.0f, 384.0f));

		// {�X�N���[���Q�[�W�̏�����}
		pLoadingGauge->InitGaugeTexture(D3DXVECTOR2(282.0f, 15.0f), D3DXVECTOR2(512.0f, 660.0f), 0);
		pLoadingGauge->InitGauge(false, MAX_TITLE_PROCESS);

		// {�t�F�[�h�e�N�X�`���̏�����}
		pFade->InitFadeTexture();
		pFade->SetAlpha(255);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::UpdateGame()
// DESC: �Q�[�������� �Q�[���̍X�V
//-----------------------------------------------------------------------------
VOID GameInstance::UpdateGame()
{
	// �Q�[���̏�Ԉڍs�̃t�F�[�h����
	UpdateFade();
	
	// �R���g���[���[�X�V
	if (pSourceManager->GetLoadingState() == LS_Ready)
	{
		if (iCurrentState == GS_Title)
		{
			// {�Q�[���^�C�g���R���g���[���[�̍X�V}
			pTitleInstance->UpdateController();
		}
		else if (iCurrentState == GS_Game)
		{
			// {�Q�[���X�e�[�W�R���g���[���[�̍X�V}
			pStageInstance->UpdateController();
		}
	}

	// �\�[�X�Ǎ��ݏ�Ԃ̍X�V
	if (pSourceManager->GetLoadingState() == LS_Loading)
	{
		if (iTargetState == GS_Title)
		{
			// {�Q�[���^�C�g���̓Ǎ���} 
			if (SUCCEEDED(pTitleInstance->LoadTitle()))
			{
				if (pSourceManager->GetLoadingProcess() >= MAX_TITLE_PROCESS)
				{
					// ���݂̃Q�[����Ԃ��X�V
					iCurrentState = GS_Title;
					// �Q�[���X�e�[�W�̉������
					if (pStageInstance)
					{
						pStageInstance->UninitStage();
					}
					// �\�[�X�ҋ@��Ԃֈڍs
					pSourceManager->SetLoadingState(LS_EndLoad);
				}
			}
		}
		else if (iTargetState == GS_Game)
		{
			// {�Q�[���X�e�[�W�̓Ǎ���} 
			if (SUCCEEDED(pStageInstance->LoadStage()))
			{
				// �X�V����
				if (pSourceManager->GetLoadingProcess() >= MAX_STAGE_PROCESS)
				{
					// ���݂̃Q�[����Ԃ��X�V
					iCurrentState = GS_Game;
					// �^�C�g���̉������
					if (pTitleInstance)
					{
						pTitleInstance->UninitTitle();
					}
					// �\�[�X�ҋ@��Ԃֈڍs
					pSourceManager->SetLoadingState(LS_EndLoad);
				}
			}
		}
	}
	// �Q�[���̑��X�V
	else
	{
		if (iCurrentState == GS_Title)
		{
			// {�Q�[���^�C�g���̍X�V}
			pTitleInstance->UpdateTitle();
			
			// �Q�[���X�e�[�W�ֈڍs 
			if (StartCheck())
			{
				switch (pTitleInstance->GetSelectStage())
				{
				case GameStage1:
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage1(pDevice);
					break;
				case GameStage2:
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage2(pDevice);
					break;
				}
				iTargetState = GS_Game;
				pSourceManager->SetLoadingState(LS_BeginLoad);
			}
		}
		else if (iCurrentState == GS_Game)
		{
			// {�Q�[���X�e�[�W�̍X�V} 
			if (pStageInstance->GetCurrentState() == SS_Loading)
			{
				// �X�e�[�W�Ǎ���
				pStageInstance->LoadStage();
			}
			else
			{
				pStageInstance->UpdateStage();
			}

			// �Q�[���^�C�g���ֈڍs 
			if (ReturnCheck())
			{
				iTargetState = GS_Title;
				pSourceManager->SetLoadingState(LS_BeginLoad);
			}
			// �X�e�[�W�ؑւ�
			else if (ChangeStageCheck())
			{
				if ((pStageInstance->GetScene() > 0) && (pStageInstance->GetScene() <= 4))
				{
					pStageInstance->UninitStage();
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage1(pDevice);
				}
				else if (pStageInstance->GetScene() > 4)
				{
					pStageInstance->UninitStage();
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage2(pDevice);
				}

				iCurrentState = GS_GameReload;
				pSourceManager->SetLoadingState(LS_BeginLoad);
			}
		}
	}

	// �f�o�b�O�pIMGUI
	DebugImgui();
}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::RenderGame()
// DESC: �Q�[�������� �Q�[���̕`��
//-----------------------------------------------------------------------------
VOID GameInstance::RenderGame()
{

	// �Q�[���̑��`��
	if (iCurrentState == GS_Title)
	{
		// {�Q�[���^�C�g���̕`��}
		pTitleInstance->RenderTitle();
	}
	else if (iCurrentState == GS_Game)
	{
		// {�Q�[���X�e�[�W�̕`��} 
		pStageInstance->RenderStage();
	}


	// �t�F�[�h�e�N�X�`���̕`��
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(true);
	}

	// �\�[�X�Ǎ��ݏ�Ԃ̕`��
	if ((iTargetState != iCurrentState))
	{
		if (pFade->GetAlphaValue() > 128 )
		{
			// {���[�h�e�N�X�`���̕`��}
			if (iLoadingTexture != 0)
			{
				pGLoadingTexture[1]->RenderGameTexture(pSourceManager->GetTextureFile(iLoadingTexture)->GetTexture());
			}

			pGLoadingTexture[0]->RenderGameTexture(pSourceManager->GetTextureFile(0)->GetTexture());

			// {�X�N���[���Q�[�W�̕`��}
			if (iLoadingTexture != 0)
			{
				pLoadingGauge->RenderGaugeTexture();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GameInstance::StartCheck()
// DESC: �Q�[�������� �Q�[���J�n�ڍs����
//-----------------------------------------------------------------------------
BOOL GameInstance::StartCheck()
{
	// �ڍs�����`�F�b�N
	if ( (iCurrentState == GS_Title) && (pFade->iFadeState == FS_Ready) )
	{
		// �^�C�g���`�F�b�N
		if ((pTitleInstance->GetSelectStage() != 0) && (pTitleInstance->GetCurrentState() == TS_Menu))
		{
			// ���[�h��ʂ̃e�N�X�`�������߂�
			iLoadingTexture = rand() % 3 + 1;

			// ���[�h��ʂ̃Q�[�W������
			pLoadingGauge->InitGauge(false, MAX_STAGE_PROCESS);
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// NAME: BOOL GameInstance::ChangeStageCheck()
// DESC: �Q�[�������� �X�e�[�W�ؑւ������`�F�b�N
//-----------------------------------------------------------------------------
BOOL GameInstance::ChangeStageCheck()
{
	// �ڍs�����`�F�b�N
	if ((iCurrentState == GS_Game) && (pFade->iFadeState == FS_Ready))
	{
		// �X�e�[�W�`�F�b�N
		if (pStageInstance->GetCurrentState() == SS_ChangeStage)
		{
			// ���[�h��ʂ̃e�N�X�`�������߂�
			iLoadingTexture = rand() % 3 + 1;

			// ���[�h��ʂ̃Q�[�W������
			pLoadingGauge->InitGauge(false, MAX_STAGE_PROCESS);
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// NAME: BOOL GameInstance::ReturnCheck()
// DESC: �Q�[�������� �^�C�g���߂������`�F�b�N
//-----------------------------------------------------------------------------
BOOL GameInstance::ReturnCheck()
{
	// �ڍs�����`�F�b�N
	if ((iCurrentState == GS_Game) && (pFade->iFadeState == FS_Ready))
	{
		// �X�e�[�W�`�F�b�N
		if (pStageInstance->GetCurrentState() == SS_ReturnStart)
		{
			// ���[�h��ʂ̃e�N�X�`�������߂�
			iLoadingTexture = rand() % 3 + 1;

			// ���[�h��ʂ̃Q�[�W������
			pLoadingGauge->InitGauge(false, MAX_TITLE_PROCESS);
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::UpdateFade()
// DESC: �Q�[�������� �Q�[���̏�Ԉڍs�̈Ó]����
//-----------------------------------------------------------------------------
VOID GameInstance::UpdateFade()
{
	if (iCurrentState != iTargetState)
	{
		// �Ó]�J�n
		if (pSourceManager->GetLoadingState() == LS_BeginLoad)
		{
			// �t�F�[�h�C���J�n����
			if (pFade->iFadeState == FS_Ready)
			{
				pFade->iFadeState = FS_FadeIn;
			}

			// �t�F�[�h�C���I������
			if ((pFade->iFadeState == FS_FadeIn) && (pFade->GetAlphaValue() == 255))
			{
				// �\�[�X���[�h����Ԃֈڍs
				pSourceManager->SetLoadingState(LS_Loading);
			}
		}

		// {�X�N���[���Q�[�W�̍X�V}
		pLoadingGauge->fTargetValue = (float)pSourceManager->GetLoadingProcess();
		pLoadingGauge->UpdateGaugeTexture(5);
	}

	// �Ó]��
	if (pSourceManager->GetLoadingState() == LS_EndLoad)
	{
		// �t�F�[�h�A�E�g�J�n����
		if (pFade->iFadeState == FS_FadeIn)
		{
			pFade->iFadeState = FS_FadeOut;
		}

		// �t�F�[�h�A�E�g�I������
		if ((pFade->iFadeState == FS_FadeOut) && (pFade->GetAlphaValue() == 0))
		{
			// �\�[�X�ҋ@��Ԃֈڍs
			pSourceManager->SetLoadingState(LS_Ready);
			pSourceManager->ResetLoadingProcess();
			// �t�F�[�h��ԉ�
			pFade->iFadeState = FS_Ready;
		}
	}

	// �t�F�[�h�e�N�X�`���̍X�V
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->UpdateFadeTexture();
	}

}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::DebugImgui()
// DESC: �Q�[�������� �f�o�b�O�pIMGUI
//-----------------------------------------------------------------------------
VOID GameInstance::DebugImgui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 120));
	ImGui::SetNextWindowSize(ImVec2(200, 240));
	ImGui::Begin(u8"�Q�[�����");
	ImGui::Text(u8"���݂̃Q�[����ԁF%d", iCurrentState);
	ImGui::Text(u8"���̃Q�[����ԁF%d", iTargetState);
	ImGui::Text(u8"�\�[�X�Ǘ����: %d", pSourceManager->GetLoadingState());
	ImGui::Text(u8"�\�[�X�v���Z�X���: %d", pSourceManager->GetLoadingProcess());
	ImGui::Text(u8"�t�F�[�h���: %d", pFade->iFadeState);
	ImGui::Text(u8"�t�F�[�h�A���t�@�l: %d", pFade->GetAlphaValue());
	ImGui::Text(u8"���݂̃^�C�g�����: %d", pTitleInstance->GetCurrentState());
	ImGui::Text(u8"���̃^�C�g�����: %d", pTitleInstance->GetTargetState());
	if (pTitleInstance->GetFade())
	{
		ImGui::Text(u8"�^�C�g���t�F�[�h���: %d", pTitleInstance->GetFade()->iFadeState);
		ImGui::Text(u8"�^�C�g���t�F�[�h�A���t�@�l: %d", pTitleInstance->GetFade()->GetAlphaValue());
	}
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