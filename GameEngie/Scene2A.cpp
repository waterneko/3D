
//=============================================================================//
// 
// Name : Scene2A.cpp
// Describe : �V�[��2A����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Scene2A.h"				// �V�[��2A����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: Scene2A::Scene2A(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �X�e�[�W�V�[������ �R���X�g���N�^
//-----------------------------------------------------------------------------
Scene2A::Scene2A(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	// {�Q�[���R���g���[���[}
	pGController = new GController(0);
	// {�J����}
	pTPCamera = new BThirdPersonCamera(pDevice);
	// {���C�g}
	pLight = new BLight(pDevice, 0);
	// {���b�V��}
	pMesh[0] = new BMesh(pDevice);
	// {�X�L�����b�V��}
	pSkinMesh[0] = new BSkinMesh(pDevice);
	// {���C�����N���X}
	pCameraRay = new CRay();
	pCharacterRayX = new CRay();
	pCharacterRayY = new CRay();
	pCharacterRayZ = new CRay();
}

//-----------------------------------------------------------------------------
// NAME: Scene2A::~Scene2A()
// DESC: �X�e�[�W�V�[������ �f�X�g���N�^
//-----------------------------------------------------------------------------
Scene2A::~Scene2A()
{
	// {���b�V���̉��}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pMesh[i])
	}
	// {�X�L�����b�V���̉��}
	for (int i = 0; pSkinMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pSkinMesh[i])
	}
	// {�X�L�����b�V���̉��}
	for (int i = 0; pSkinMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pSkinMesh[i])
	}

	// {���C�����N���X�̉��}
	SAFE_DELETE(pCameraRay)
	SAFE_DELETE(pCharacterRayZ)
	SAFE_DELETE(pCharacterRayY)
	SAFE_DELETE(pCharacterRayX)
	// {���C�g�̉��}
	SAFE_DELETE(pLight)
	// {�J�����̉��}
	SAFE_DELETE(pTPCamera)
	// {�Q�[���R���g���[���[�̉��}
	SAFE_DELETE(pGController)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Scene2A::InitScene()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���Ǎ���
//-----------------------------------------------------------------------------
HRESULT	Scene2A::InitScene()
{
	// {�J�����̏�����}
	pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// {���C�g�̏�����}
	pLight->InitDirectionalLight(D3DXVECTOR3(-0.5f, -1.0f, 0.5f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	pLight->EnableLight();

	// {���b�V���̏�����}
	pMesh[0]->vFixPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pMesh[0]->vScaling = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	// {�X�L�����b�V���̏�����}
	pSkinMesh[0]->InitSkinMesh(SetAnimXfile(0));
	pSkinMesh[0]->pAnimController->ChangeAnimation(true, 11);
	pSkinMesh[0]->vFixRotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	pSkinMesh[0]->vPosition = D3DXVECTOR3(0.0f, 5.0f, 100.0f);
	pSkinMesh[0]->vScaling = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene2A::UpdateController()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���R���g���[���[�̑��X�V����
//-----------------------------------------------------------------------------
VOID Scene2A::UpdateController()
{
	// {�Q�[���R���g���[���[�J�����C���X�V}
	pGController->UpdateFixLstick(pTPCamera->GetRotation());

	// {�J�����R���g���[��}
	// �J��������]
	pTPCamera->Zoom(GetMouseZ() * -0.2f);

	if (IsMouseCenterPressed())
	{
		// �ړ�
		//pTPCamera->MoveByRightVector(GetMouseX() * 5.0f);
		pTPCamera->MoveByLookVector(GetMouseY() * -2.5f);
		pTPCamera->MoveByRightVector(GetMouseX() * -2.5f);
	}
	else if (IsMouseRightPressed())
	{
		// ��]
		pTPCamera->RotationByRightVector(GetMouseY() * 0.25f);
		pTPCamera->RotationByUpVector(GetMouseX() * 0.25f);
	}

	// �J�������Z�b�g
	if (GetKeyboardTrigger(DIK_F4))
	{
		pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pTPCamera->SetRadius(DEFAULT_CAMERA_RADIUS);
	}

	// �Q�[���p�b�g�R���g���[��
	pTPCamera->RotationByRightVector(-2 * pGController->GetRstick().z);
	pTPCamera->RotationByUpVector(-2 * pGController->GetRstick().x);

	if (pGController->PadXButton(true))
	{
		pSkinMesh[0]->MoveByYAxis(2.0f);
	}
	else if (pGController->PadYButton(true))
	{
		pSkinMesh[0]->MoveByYAxis(-2.0f);
	}

	if (pGController->PadLTButton(false))
	{
		pSkinMesh[0]->pAnimController->ChangeAnimation(true, pSkinMesh[0]->pAnimController->GetAnimIndex() - 1);
	}
	else if (pGController->PadRTButton(false))
	{
		pSkinMesh[0]->pAnimController->ChangeAnimation(true, pSkinMesh[0]->pAnimController->GetAnimIndex() + 1);
	}

	if (pGController->PadLBButton(true))
	{
		pSkinMesh[0]->RotationByYAxis(-5.0f);
	}
	else if (pGController->PadRBButton(true))
	{
		pSkinMesh[0]->RotationByYAxis(5.0f);
	}

	if (pGController->GetLstick() != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		if (pSkinMesh[0]->vRotation.y != GetVectorRotation(pGController->GetFixLstick()))
		{
			if (fabs(pSkinMesh[0]->vRotation.y - GetVectorRotation(pGController->GetFixLstick())) <= D3DX_PI / 9.0f)
			{
				pSkinMesh[0]->vRotation.y = GetVectorRotation(pGController->GetFixLstick());
			}
			else
			{
				if ((pSkinMesh[0]->vAxisZ.z * pGController->GetFixLstick().x) - (pSkinMesh[0]->vAxisZ.x * pGController->GetFixLstick().z) > 0)
				{
					pSkinMesh[0]->RotationByYAxis(10);
				}
				else
				{
					pSkinMesh[0]->RotationByYAxis(-10);
				}
			}
		}

		pSkinMesh[0]->pAnimController->ChangeAnimation(true, 0);
		pSkinMesh[0]->MoveByVector(5 * pGController->GetFixLstick());
	}
	else
	{
		pSkinMesh[0]->pAnimController->ChangeAnimation(true, 11);
	}

	if (pGController->PadBackButton(false))
	{
		SetNextScene(-1);
		SetChangeSceneFlag(true);
	}
	else if (pGController->PadStartButton(false))
	{
		pSkinMesh[0]->pAnimController->ChangeAnimation(true, 11);
		SetNextScene(GameStage1A);
		SetChangeSceneFlag(true);
	}
	else if (pGController->PadBButton(false))
	{
		pSkinMesh[0]->pAnimController->ChangeAnimation(true, 11);
		SetNextScene(GameStage2A);
		SetChangeSceneFlag(true);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene2A::UpdateStage()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���̑��X�V����
//-----------------------------------------------------------------------------
VOID Scene2A::UpdateScene()
{
	// {���b�V���̍X�V}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		pMesh[i]->UpdateMesh();
	}
	// {�X�L�����b�V���̍X�V}
	for (int i = 0; pSkinMesh[i] != NULL; i++)
	{
		pSkinMesh[i]->UpdateSkinMesh();
	}
	// {�J�����̍X�V}
	pTPCamera->SetTargetPosition(pSkinMesh[0]->vPosition, 100.f);
	pTPCamera->UpdateCamera();
	pTPCamera->SetViewMatrix();
	pTPCamera->SetProjMatrix();

	// {���C�����X�V}
	if (pCameraRay->RayCollision(pTPCamera->GetTargetPosition(), pTPCamera->GetCameraDirection(true), SetXfile(2)->GetMesh(), pMesh[0]))
	{
		if (pCameraRay->fHitDistance < DEFAULT_CAMERA_RADIUS)
		{
			pTPCamera->SetRadius(pCameraRay->fHitDistance * 0.85f);
		}
		else
		{
			pTPCamera->SetRadius(DEFAULT_CAMERA_RADIUS);
		}
	}

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pCharacterRayZ->RayCollision(pSkinMesh[0]->vPosition, pSkinMesh[0]->vAxisZ, SetXfile(2)->GetMesh(), pMesh[0]))
	{
		if (pCharacterRayZ->fHitDistance < 80.0f)
		{
			move += 6 * pCharacterRayZ->vHitNormal;

		}
	}

	if (pCharacterRayY->RayCollision(pSkinMesh[0]->vPosition, -pSkinMesh[0]->vAxisY, SetXfile(2)->GetMesh(), pMesh[0]))
	{
		if (pCharacterRayY->fHitDistance < 20.0f)
		{
			move += 2 * pCharacterRayY->vHitNormal;
		}
	}

	if (pCharacterRayX->RayCollision(pSkinMesh[0]->vPosition, pSkinMesh[0]->vAxisX, SetXfile(2)->GetMesh(), pMesh[0]))
	{
		if (pCharacterRayX->fHitDistance < 40.0f)
		{
			move += 6 * pCharacterRayX->vHitNormal;
		}
	}

	if (move != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		pSkinMesh[0]->MoveByVector(move);
	}

	DebugImgui();
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene2A::RenderStage()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���̑��`��
//-----------------------------------------------------------------------------
VOID Scene2A::RenderScene()
{
	// �J�����O�����Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// {���b�V���̕`��}
	pMesh[0]->RenderMesh(SetXfile(2));
	// {�X�L�����b�V���̕`��}
	pSkinMesh[0]->UpdateAnimation(AMIMATION_FRAMETIME);
	pSkinMesh[0]->RenderSkinMesh();

	// ���ʃJ�����O������
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene2A::DebugImgui()
// DESC: �X�e�[�W1���� �X�e�[�W�f�o�b�O�pIMGUI
//-----------------------------------------------------------------------------
VOID Scene2A::DebugImgui()
{
	ImGui::SetNextWindowPos(ImVec2(660, 140));
	ImGui::SetNextWindowSize(ImVec2(380, 160));
	ImGui::Begin(u8"�J�������");
	ImGui::InputFloat3(u8"�J������]�p�x", pTPCamera->GetRotationAngel());
	ImGui::InputFloat3(u8"�J�������_", pTPCamera->GetCameraPosition());
	ImGui::InputFloat3(u8"�J���������_", pTPCamera->GetTargetPosition());
	ImGui::InputFloat3(u8"�J��������", pTPCamera->GetCameraDirection(true));
	ImGui::InputFloat3(u8"�J������������", pTPCamera->GetLookVector());
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(660, 400));
	ImGui::SetNextWindowSize(ImVec2(380, 160));
	ImGui::Begin(u8"�L�������");
	ImGui::InputFloat3(u8"�L�������W", pSkinMesh[0]->vPosition);
	ImGui::InputFloat3(u8"�L������]�p�x", pSkinMesh[0]->vRotation);
	ImGui::InputFloat3(u8"�L����X��", pSkinMesh[0]->vAxisX);
	ImGui::InputFloat3(u8"�L����Y��", pSkinMesh[0]->vAxisY);
	ImGui::InputFloat3(u8"�L����Z��", pSkinMesh[0]->vAxisZ);
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