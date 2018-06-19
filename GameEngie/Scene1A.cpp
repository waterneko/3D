
//=============================================================================//
// 
// Name : Scene1A.cpp
// Describe : シーン1A処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Scene1A.h"				// シーン1A処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: Scene1A::Scene1A(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ステージシーン処理 コンストラクタ
//-----------------------------------------------------------------------------
Scene1A::Scene1A(LPDIRECT3DDEVICE9 D3dDevice)
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
	pMesh[1] = new BMesh(pDevice);
	pMesh[2] = new BMesh(pDevice);
	// {スキンメッシュ}
	pSkinMesh[0] = new BSkinMesh(pDevice);
	// {ゲームテクスチャ}
	pGTexture[0] = new GTexture(pDevice);
	// {レイ交差クラス}
	pCameraRay = new CRay();
	pCharacterRayX = new CRay();
	pCharacterRayY = new CRay();
	pCharacterRayZ = new CRay();
}

//-----------------------------------------------------------------------------
// NAME: Scene1A::~Scene1A()
// DESC: ステージシーン処理 デストラクタ
//-----------------------------------------------------------------------------
Scene1A::~Scene1A()
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

	// {ゲームテクスチャの解放}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		SAFE_DELETE(pGTexture[i])
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
// NAME: HRESULT Scene1A::InitScene()
// DESC: ステージシーン処理 ステージシーン読込み
//-----------------------------------------------------------------------------
HRESULT	Scene1A::InitScene()
{ 
	// {カメラの初期化}
	pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pTPCamera->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// {ライトの初期化}
	pLight->InitDirectionalLight(D3DXVECTOR3(-0.5f, -1.0f, 0.5f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	pLight->EnableLight();

	// {メッシュの初期化}
	pMesh[0]->vFixPosition = D3DXVECTOR3(400.0f, 0.0f, 200.0f);
	pMesh[0]->vScaling = D3DXVECTOR3(50.0f, 50.0f, 50.0f);

	pMesh[1]->vFixPosition = D3DXVECTOR3(-100.0f, 0.0f, 200.0f);
	pMesh[1]->vScaling = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	pMesh[2]->vFixPosition = D3DXVECTOR3(-1200.0f, 0.0f, 200.0f);
	pMesh[2]->vFixRotation = D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, 0.0f);
	pMesh[2]->vScaling = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	// {スキンメッシュの初期化}
	pSkinMesh[0]->InitSkinMesh(SetAnimXfile(0));
	pSkinMesh[0]->pAnimController->ChangeAnimation(true, 11);
	pSkinMesh[0]->vFixRotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	pSkinMesh[0]->vPosition = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	pSkinMesh[0]->vScaling = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

	// {ゲームテクスチャの初期化}
	pGTexture[0]->InitGameTexture(D3DXVECTOR2(500.0f, 500.0f), D3DXVECTOR2(512.0f, 400.0f));
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene1A::UpdateController()
// DESC: ステージシーン処理 ステージシーンコントローラーの総更新処理
//-----------------------------------------------------------------------------
VOID Scene1A::UpdateController()
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
		SetNextScene(GameStage2A);
		SetChangeSceneFlag(true);
	}
	else if (pGController->PadBButton(false))
	{
		pSkinMesh[0]->pAnimController->ChangeAnimation(true, 11);
		SetNextScene(GameStage1B);
		SetChangeSceneFlag(true);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene1A::UpdateStage()
// DESC: ステージシーン処理 ステージシーンの総更新処理
//-----------------------------------------------------------------------------
VOID Scene1A::UpdateScene()
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
	// {ゲームテクスチャの更新}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		pGTexture[i]->UpdateGameTexture();
	}
	// {カメラの更新}
	pTPCamera->SetTargetPosition(pSkinMesh[0]->vPosition);
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

	if (pCameraRay->RayCollision(pTPCamera->GetTargetPosition(), pTPCamera->GetCameraDirection(true), SetXfile(3)->GetMesh(), pMesh[1]))
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

	if (pCameraRay->RayCollision(pTPCamera->GetTargetPosition(), pTPCamera->GetCameraDirection(true), SetXfile(3)->GetMesh(), pMesh[2]))
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

	if (pCharacterRayZ->RayCollision(pSkinMesh[0]->vPosition, pSkinMesh[0]->vAxisZ, SetXfile(3)->GetMesh(), pMesh[1]))
	{
		if (pCharacterRayZ->fHitDistance < 80.0f)
		{
			move += 6 * pCharacterRayZ->vHitNormal;
		}
	}

	if (pCharacterRayZ->RayCollision(pSkinMesh[0]->vPosition, pSkinMesh[0]->vAxisZ, SetXfile(3)->GetMesh(), pMesh[2]))
	{
		if (pCharacterRayZ->fHitDistance < 80.0f)
		{
			move += 6 * pCharacterRayZ->vHitNormal;
		}
	}

	if (pCharacterRayY->RayCollision(pSkinMesh[0]->vPosition, -pSkinMesh[0]->vAxisY, SetXfile(3)->GetMesh(), pMesh[1]))
	{
		if (pCharacterRayY->fHitDistance < 10.0f)
		{
			move += 2 * pCharacterRayY->vHitNormal;
		}
	}

	if (pCharacterRayY->RayCollision(pSkinMesh[0]->vPosition, -pSkinMesh[0]->vAxisY, SetXfile(3)->GetMesh(), pMesh[2]))
	{
		if (pCharacterRayY->fHitDistance < 10.0f)
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
// NAME: VOID Scene1A::RenderStage()
// DESC: ステージシーン処理 ステージシーンの総描画
//-----------------------------------------------------------------------------
VOID Scene1A::RenderScene()
{
	// カリングをしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// {メッシュの描画}
	pMesh[0]->RenderMesh(SetXfile(2));
	pMesh[1]->RenderMesh(SetXfile(3));
	pMesh[2]->RenderMesh(SetXfile(3));
	// {スキンメッシュの描画}
	pSkinMesh[0]->UpdateAnimation(AMIMATION_FRAMETIME);
	pSkinMesh[0]->RenderSkinMesh();

	// {ゲームテクスチャの描画}
	//pGTexture[0]->RenderGameTexture(SetTexture(4));

	// 裏面カリングをする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-----------------------------------------------------------------------------
// NAME: VOID Scene1A::DebugImgui()
// DESC: ステージ1処理 ステージデバッグ用IMGUI
//-----------------------------------------------------------------------------
VOID Scene1A::DebugImgui()
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