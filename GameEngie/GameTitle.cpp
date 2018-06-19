
//=============================================================================//
// 
// Name : GameTitle.cpp
// Describe : �Q�[���^�C�g������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "GameTitle.h"				// �Q�[���^�C�g������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: TitleInstance::TitleInstance(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �Q�[���^�C�g������ �R���X�g���N�^
//-----------------------------------------------------------------------------
TitleInstance::TitleInstance(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iCurrentState = TS_Logo;
	iTargetState = TS_Logo;

	iSelectStage = 0;

	// �\�[�X�Ǘ����擾����
	pSourceManager = SourceManager::GetInstance();
}

//-----------------------------------------------------------------------------
// NAME: TitleInstance::~TitleInstance()
// DESC: �Q�[���^�C�g������ �f�X�g���N�^
//-----------------------------------------------------------------------------
TitleInstance::~TitleInstance()
{
	UninitTitle();
}

//-----------------------------------------------------------------------------
// NAME: HRESULT	TitleInstance::LoadTitle()
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���Ǎ���
//-----------------------------------------------------------------------------
HRESULT	TitleInstance::LoadTitle()
{
	if (pSourceManager->GetLoadingState() == LS_Loading)
	{
		// �\�[�X�̓Ǎ���
		if (FAILED(LoadSource()))
		{
			MessageBox(0, "�^�C�g�����[�h�����s���܂���", NULL, MB_OK);
			return E_FAIL;
		}

		// �Q�[���^�C�g���̑�������
		if (FAILED(InitObject()))
		{
			MessageBox(0, "�^�C�g�������������s���܂���", NULL, MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}
//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateGameTitle()
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���\�[�X�̓Ǎ���
//-----------------------------------------------------------------------------
HRESULT TitleInstance::LoadSource()
{

	// �e�N�X�`���̓Ǎ���
	if (pSourceManager->GetLoadingProcess() == 0)
	{
		// �e�N�X�`���̓Ǎ���
		pSourceManager->LoadTexturefile(4, PATH_TITLETEXTURE, "sign.png");
		pSourceManager->LoadTexturefile(5, PATH_TITLETEXTURE, "title.jpg");
		pSourceManager->LoadTexturefile(6, PATH_TITLETEXTURE, "menu.png");
	}
	else if (pSourceManager->GetLoadingProcess() == 3)
	{
		// x�t�@�C���̓Ǎ���
		pSourceManager->LoadXfile(0, PATH_X_CASTLE, "Castle.x");
		pSourceManager->LoadXfile(1, PATH_X_CASTLE, "Cilff.x");
	}
	else if (pSourceManager->GetLoadingProcess() == 6)
	{
		// {�X�J�C�h�[���̏�����}
		pGSkyDome = new GSkyDome(pDevice);
		pGSkyDome->LoadSkyTexture(TEXTURE_SKYDOMENIGHT);
	}
	else if (pSourceManager->GetLoadingProcess() == 9)
	{
		// {�t�F�[�h�e�N�X�`���̏�����}
		pFade = new GFadeTexture(pDevice);
		pFade->InitFadeTexture();
		pFade->SetAlpha(0);
	}

	pSourceManager->AddLoadingProcess(1);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT TitleInstance::InitObject()
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���I�u�W�F�N�g�̏���������
//-----------------------------------------------------------------------------
HRESULT TitleInstance::InitObject()
{
	if (pSourceManager->GetLoadingProcess() == 10)
	{
		// �A���r�G���g���C�g�i�����j��ݒ肷��
		pDevice->SetRenderState(D3DRS_AMBIENT, TITLE_START_AMBIENT);

		// {�Q�[���R���g���[���[������}
		pGController = new GController(0);

		// {�J����������}
		pTPCamera = new BThirdPersonCamera(pDevice);
		pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 200.0f);
		pTPCamera->SetRadius(900.0f);
		pTPCamera->SetRotation(TITLE_START_CAMERAROT);

		// {���C�g������}
		pLight = new BLight(pDevice, 0);
		pLight->InitDirectionalLight(D3DXVECTOR3(-0.5f, -1.0f, 0.5f), TITLE_START_LIGHTAMBIENT, TITLE_START_LIGHTDIFFUSE, TITLE_START_LIGHTSPECULAR);
		pLight->EnableLight();

		// {���b�V���̏�����}
		pMesh[0] = new BMesh(pDevice);
		pMesh[0]->vScaling = D3DXVECTOR3(3.0f, 3.0, 3.0f);
		pMesh[0]->vFixPosition = D3DXVECTOR3(0.0f, -100.0, 0.0f);
		pMesh[1] = new BMesh(pDevice);
		pMesh[1]->vScaling = D3DXVECTOR3(2.0f, 2.0, 2.0f);
		pMesh[1]->vFixPosition = D3DXVECTOR3(-25.0f, -100.0, -90.0f);

		// {�Q�[���e�N�X�`���̏�����}
		pGTexture[0] = new GTexture(pDevice);
		pGTexture[0]->InitGameTexture(D3DXVECTOR2(284.0f, 26.0f), D3DXVECTOR2(512.0f, 740.0f));
		pGTexture[1] = new GTexture(pDevice);
		pGTexture[1]->InitGameTexture(D3DXVECTOR2(524.0f, 312.0f), D3DXVECTOR2(512.0f, 180.0f));
		pGTexture[2] = new GTexture(pDevice);
		pGTexture[2]->InitGameTexture(D3DXVECTOR2(500.0f, 500.0f), D3DXVECTOR2(512.0f, 360.0f));

		// {�X�J�C�h�[���̏�����}
		pGSkyDome->InitSkyDome(5, 5, 6400.0f);
		pGSkyDome->vFixPosition = D3DXVECTOR3(0.0f, 800.0f, 0.0f);
		pGSkyDome->vFixRotation = D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f);
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateController()
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���R���g���[���[�̑��X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateController()
{
	if (pFade->iFadeState == FS_Ready)
	{
		switch (iCurrentState)
		{
		case TS_Logo:
			// �Q�[�����S�R���g���[���[�̍X�V
			UpdateLogoController();
			break;
		case TS_Start:
			// �Q�[���X�^�[�g�R���g���[���[�̍X�V
			UpdateStartController();
			break;
		case TS_Menu:
			// �Q�[�����j���[�R���g���[���[�̍X�V
			UpdateMenuController();
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateTitle()
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���̍X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateTitle()
{	
	// �^�C�g����Ԉڍs�̃t�F�[�h����
	UpdateFade();

	switch (iCurrentState)
	{
	case TS_Logo:
		// �Q�[�����S�̍X�V
		UpdateLogoObject();
		break;
	case TS_Start:
		// �Q�[���X�^�[�g�̍X�V
		UpdateStartObject();
		break;
	case TS_Menu:
		// �Q�[�����j���[�̍X�V
		UpdateMenuObject();
		break;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderTitle()
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���̑��`�揈��
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderTitle()
{
	switch (iCurrentState)
	{
	case TS_Logo:
		// �Q�[�����S�̕`��
		RenderLogo();
		break;
	case TS_Start:
		// �Q�[���X�^�[�g�̕`��
		RenderStart();
		break;
	case TS_Menu:
		// �Q�[�����j���[�̕`��
		RenderMenu();
		break;
	}

	// {�t�F�[�h�e�N�X�`���̕`��}
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(false);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateTitle()
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���̉������
//-----------------------------------------------------------------------------
VOID TitleInstance::UninitTitle()
{
	// ��ԏ�����
	iCurrentState = TS_Start;
	iTargetState = TS_Start;
	iSelectStage = 0;

	// {�X�J�C�h�[���̉��}
	SAFE_DELETE(pGSkyDome)

	// {���b�V���̉��}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pMesh[i])
	}

	// {�Q�[���e�N�X�`���̉��}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		SAFE_DELETE(pGTexture[i])
	}

	// {�t�F�[�h�e�N�X�`���̉��}
	SAFE_DELETE(pFade)
	// {���C�g���}
	SAFE_DELETE(pLight)
	// {�J�������}
	SAFE_DELETE(pTPCamera)
	// {�Q�[���R���g���[���[���}
	SAFE_DELETE(pGController)
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateFade()
// DESC: �Q�[���^�C�g������ �^�C�g����Ԉڍs�̃t�F�[�h����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateFade()
{
	if (iCurrentState != iTargetState)
	{
		// �t�F�[�h�C���I������
		if ((pFade->iFadeState == FS_FadeIn) && (pFade->GetAlphaValue() == 255))
		{
			// �t�F�[�h�A�E�g�J�n����
			pFade->iFadeState = FS_FadeOut;
			// �^�C�g����ԍX�V
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
// NAME: VOID TitleInstance::UpdateLogoController()
// DESC: �Q�[���^�C�g������ �Q�[�����S�R���g���[���[�̍X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateLogoController()
{
	if (pGController->PadBButton(false) || pGController->PadStartButton(false) || GetKeyboardTrigger(DIK_RETURN))
	{
		ChangeTitleState(TS_Start);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateLogoObject()
// DESC: �Q�[���^�C�g������ �Q�[�����S�I�u�W�F�N�g�̍X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateLogoObject()
{
	// {�Q�[���e�N�X�`���̍X�V}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		pGTexture[i]->UpdateGameTexture();
	}

}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateStartController()
// DESC: �Q�[���^�C�g������ �Q�[���X�^�[�g�R���g���[���[�̍X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateStartController()
{
	if (pGController->PadAButton(false))
	{
		ChangeTitleState(TS_Logo);
	}
	else if (pGController->PadBButton(false) || pGController->PadStartButton(false) || GetKeyboardTrigger(DIK_RETURN))
	{
		if (pTPCamera->GetRotation().y <= D3DXToRadian(10))
		{
			pTPCamera->SetRotation(D3DXVECTOR3(D3DXToRadian(-20), D3DXToRadian(10), 0.0f));
		}
		else
		{
			ChangeTitleState(TS_Menu);
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateStartObject()
// DESC: �Q�[���^�C�g������ �Q�[���X�^�[�g�I�u�W�F�N�g�̍X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateStartObject()
{
	// {���C�g�̍X�V}
	pLight->SetDirection(D3DXVECTOR3(pGSkyDome->vRotation.y,-1.0f,-0.5));

	// {�X�J�C�h�[���̍X�V}
	pGSkyDome->UpdateSkyDome();

	// {���b�V���̍X�V}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		pMesh[i]->UpdateMesh();
	}

	// {�Q�[���e�N�X�`���̍X�V}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		pGTexture[i]->UpdateGameTexture();
	}

	// {�J�����X�V}
	if (pTPCamera->GetRotation().y <= D3DXToRadian(10))
	{
		pTPCamera->RotationByUpVector(0.2f);
		//pTPCamera->RotationByRightVector(-0.3f);
	}
	pTPCamera->UpdateCamera();
	pTPCamera->SetViewMatrix();
	pTPCamera->SetProjMatrix();
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateMenuController()
// DESC: �Q�[���^�C�g������ �Q�[�����j���[�R���g���[���[�̍X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateMenuController()
{
	if (pGController->PadAButton(false))
	{
		ChangeTitleState(TS_Start);
		pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 200.0f);
		pTPCamera->SetRadius(900.0f);
		pTPCamera->SetRotation(TITLE_START_CAMERAROT);
	}
	else if (pGController->PadBButton(false) || pGController->PadStartButton(false) || GetKeyboardTrigger(DIK_RETURN))
	{
		iSelectStage = FIRST_GAMESTAGE;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateMenuObject()
// DESC: �Q�[���^�C�g������ �Q�[�����j���[�I�u�W�F�N�g�̍X�V����
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateMenuObject()
{
	// {�Q�[���e�N�X�`���̍X�V}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		pGTexture[i]->UpdateGameTexture();
	}

}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderLogo()
// DESC: �Q�[���^�C�g������ �Q�[�����S�̕`�揈��
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderLogo()
{
	// {�Q�[���e�N�X�`���̕`��}
	pGTexture[0]->RenderGameTexture(SetTexture(4));
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderLogo()
// DESC: �Q�[���^�C�g������ �Q�[���X�^�[�g�̕`�揈��
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderStart()
{
	// < 1.�w�i(�Ő�) >
	pGSkyDome->RenderSkyDome(false);

	// < 2.�A���t�@�e�X�g(��������) >
	// �A���t�@�e�X�g�̗L����(��Ȃ�)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// �s�����ɂ���l�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 40);

	// {���b�V���̉��}
	pMesh[0]->RenderMesh(SetXfile(0));
	pMesh[1]->RenderMesh(SetXfile(1));

	// < 3.�X�N���[���e�N�X�`�� >
	// {�Q�[���e�N�X�`���̕`��}
	//pGTexture[1]->RenderGameTexture(SetTexture(5));
}
//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderLogo()
// DESC: �Q�[���^�C�g������ �Q�[�����j���[�̕`�揈��
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderMenu()
{
	// {�Q�[���e�N�X�`���̕`��}
	pGTexture[2]->RenderGameTexture(SetTexture(6));
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::ChangeTitleState(int state)
// DESC: �Q�[���^�C�g������ �Q�[���^�C�g���̕ύX
//-----------------------------------------------------------------------------
VOID TitleInstance::ChangeTitleState(int state)
{
	if (pFade->iFadeState == FS_Ready)
	{
		// �^�C�g���ύX
		iTargetState = state;

		// �t�F�[�h�C���J�n����
		pFade->iFadeState = FS_FadeIn;
	}
}

//-----------------------------------------------------------------------------
// NAME: LPDIRECT3DTEXTURE9 SetTexture(UINT n)
// DESC: �Q�[���^�C�g������ �e�N�X�`���ݒu
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 TitleInstance::SetTexture(UINT n)
{
	if (n <= pSourceManager->GetTextureNum())
	{
		return pSourceManager->GetTextureFile(n)->GetTexture();
	}
	else
	{
		return pSourceManager->GetTextureFile(pSourceManager->GetTextureNum())->GetTexture();
	}
}

//-----------------------------------------------------------------------------
// NAME: SXfile* TitleInstance::SetXfile(UINT n)
// DESC: �Q�[���^�C�g������ x�t�@�C���ݒu
//-----------------------------------------------------------------------------
SXfile* TitleInstance::SetXfile(UINT n)
{
	if (n <= pSourceManager->GetXfileNum())
	{
		return pSourceManager->GetXfile(n);
	}
	else
	{
		return pSourceManager->GetXfile(pSourceManager->GetXfileNum());
	}
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile* TitleInstance::SetAnimXfile(UINT n)
// DESC: �Q�[���^�C�g������ �A�j���[�V����x�t�@�C���ݒu
//-----------------------------------------------------------------------------
SAnimXfile* TitleInstance::SetAnimXfile(UINT n)
{
	if (n <= pSourceManager->GetAnimXfileNum())
	{
		return pSourceManager->GetAnimXfile(n);
	}
	else
	{
		return pSourceManager->GetAnimXfile(pSourceManager->GetAnimXfileNum());
	}
}

//-----------------------------------------------------------------------------
// NAME: SWavfile* TitleInstance::SetWavfile(UINT n)
// DESC: �Q�[���X�e�[�W���� Wav�t�@�C���ݒu
//-----------------------------------------------------------------------------
SWavfile* TitleInstance::SetWavfile(UINT n)
{
	if (n <= pSourceManager->GetWavfileNum())
	{
		return pSourceManager->GetWavfile(n);
	}
	else
	{
		return pSourceManager->GetWavfile(pSourceManager->GetWavfileNum());
	}
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