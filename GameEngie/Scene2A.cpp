
//=============================================================================//
// 
// Name : Scene2A.cpp
// Describe : シーン2A処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Scene2A.h"				// シーン2A処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: Scene2A::Scene2A(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ステージシーン処理 コンストラクタ
//-----------------------------------------------------------------------------
Scene2A::Scene2A(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	// {ゲームコントローラー}
	pGController = new GController(0);
	// {カメラ}
	pTPCamera = new BThirdPersonCamera(pDevice);
	// {ライト}
	pLight = new BLight(pDevice, 0);
	// {メッシュ}
	pMesh[0] = new BMesh(pDevice);
	// {スキンメッシュ}
	pSkinMesh[0] = new BSkinMesh(pDevice);
	// {レイ交差クラス}
	pCameraRay = new CRay();
	pCharacterRayX = new CRay();
	pCharacterRayY = new CRay();
	pCharacterRayZ = new CRay();
}

//-----------------------------------------------------------------------------
// NAME: Scene2A::~Scene2A()
// DESC: ステージシーン処理 デストラクタ
//-----------------------------------------------------------------------------
Scene2A::~Scene2A()
{
	// {メッシュの解放}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pMesh[i])
	}
	// {スキンメッシュの解放}
	for (int i = 0; pSkinMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pSkinMesh[i])
	}
	// {スキンメッシュの解放}
	for (int i = 0; pSkinMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pSkinMesh[i])
	}

	// {レイ交差クラスの解放}
	SAFE_DELETE(pCameraRay)
	SAFE_DELETE(pCharacterRayZ)
	SAFE_DELETE(pCharacterRayY)
	SAFE_DELETE(pCharacterRayX)
	// {ライトの解放}
	SAFE_DELETE(pLight)
	// {カメラの解放}
	SAFE_DELETE(pTPCamera)
	// {ゲームコントローラーの解放}
	SAFE_DELETE(pGController)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Scene2A::InitScene()
// DESC: ステージシーン処理 ステージシーン読込み
//-----------------------------------------------------------------------------
HRESULT	Scene2A::InitScene()
{
	// {カメラの初期化}
	pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// {ライトの初期化}
	pLight->InitDirectionalLight(D3DXVECTOR3(-0.5f, -1.0f, 0.5f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	pLight->EnableLight();

	// {メッシュの初期化}
	pMesh[0]->vFixPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pMesh[0]->vScaling = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	// {スキンメッシュの初期化}
	pSkinMesh[0]->InitSkinMesh(SetAnimXfile(0));
	pSkinMesh[0]->pAnimController->ChangeAnimation(true, 11);
	pSkinMesh[0]->vFixRotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	pSkinMesh[0]->vPosition = D3DXVECTOR3(0.0f, 5.0f, 100.0f);
	pSkinMesh[0]->vScaling = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene2A::UpdateController()
// DESC: ステージシーン処理 ステージシーンコントローラーの総更新処理
//-----------------------------------------------------------------------------
VOID Scene2A::UpdateController()
{
	// {ゲームコントローラーカメラ修正更新}
	pGController->UpdateFixLstick(pTPCamera->GetRotation());

	// {カメラコントロール}
	// カメラ自回転
	pTPCamera->Zoom(GetMouseZ() * -0.2f);

	if (IsMouseCenterPressed())
	{
		// 移動
		//pTPCamera->MoveByRightVector(GetMouseX() * 5.0f);
		pTPCamera->MoveByLookVector(GetMouseY() * -2.5f);
		pTPCamera->MoveByRightVector(GetMouseX() * -2.5f);
	}
	else if (IsMouseRightPressed())
	{
		// 回転
		pTPCamera->RotationByRightVector(GetMouseY() * 0.25f);
		pTPCamera->RotationByUpVector(GetMouseX() * 0.25f);
	}

	// カメラリセット
	if (GetKeyboardTrigger(DIK_F4))
	{
		pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pTPCamera->SetRadius(DEFAULT_CAMERA_RADIUS);
	}

	// ゲームパットコントロール
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
// DESC: ステージシーン処理 ステージシーンの総更新処理
//-----------------------------------------------------------------------------
VOID Scene2A::UpdateScene()
{
	// {メッシュの更新}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		pMesh[i]->UpdateMesh();
	}
	// {スキンメッシュの更新}
	for (int i = 0; pSkinMesh[i] != NULL; i++)
	{
		pSkinMesh[i]->UpdateSkinMesh();
	}
	// {カメラの更新}
	pTPCamera->SetTargetPosition(pSkinMesh[0]->vPosition, 100.f);
	pTPCamera->UpdateCamera();
	pTPCamera->SetViewMatrix();
	pTPCamera->SetProjMatrix();

	// {レイ交差更新}
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
// DESC: ステージシーン処理 ステージシーンの総描画
//-----------------------------------------------------------------------------
VOID Scene2A::RenderScene()
{
	// カリングをしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// {メッシュの描画}
	pMesh[0]->RenderMesh(SetXfile(2));
	// {スキンメッシュの描画}
	pSkinMesh[0]->UpdateAnimation(AMIMATION_FRAMETIME);
	pSkinMesh[0]->RenderSkinMesh();

	// 裏面カリングをする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene2A::DebugImgui()
// DESC: ステージ1処理 ステージデバッグ用IMGUI
//-----------------------------------------------------------------------------
VOID Scene2A::DebugImgui()
{
	ImGui::SetNextWindowPos(ImVec2(660, 140));
	ImGui::SetNextWindowSize(ImVec2(380, 160));
	ImGui::Begin(u8"カメラ状態");
	ImGui::InputFloat3(u8"カメラ回転角度", pTPCamera->GetRotationAngel());
	ImGui::InputFloat3(u8"カメラ視点", pTPCamera->GetCameraPosition());
	ImGui::InputFloat3(u8"カメラ注視点", pTPCamera->GetTargetPosition());
	ImGui::InputFloat3(u8"カメラ方向", pTPCamera->GetCameraDirection(true));
	ImGui::InputFloat3(u8"カメラ視線方向", pTPCamera->GetLookVector());
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(660, 400));
	ImGui::SetNextWindowSize(ImVec2(380, 160));
	ImGui::Begin(u8"キャラ状態");
	ImGui::InputFloat3(u8"キャラ座標", pSkinMesh[0]->vPosition);
	ImGui::InputFloat3(u8"キャラ回転角度", pSkinMesh[0]->vRotation);
	ImGui::InputFloat3(u8"キャラX軸", pSkinMesh[0]->vAxisX);
	ImGui::InputFloat3(u8"キャラY軸", pSkinMesh[0]->vAxisY);
	ImGui::InputFloat3(u8"キャラZ軸", pSkinMesh[0]->vAxisZ);
	ImGui::End();
}

//*****************************************************************************//
// (4) グローバル変数:
//*****************************************************************************//

//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//

//*****************************************************************************//
// (6) 関数
//*****************************************************************************//