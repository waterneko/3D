
//=============================================================================//
// 
// Name : GameTitle.cpp
// Describe : ゲームタイトル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "GameTitle.h"				// ゲームタイトル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: TitleInstance::TitleInstance(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ゲームタイトル処理 コンストラクタ
//-----------------------------------------------------------------------------
TitleInstance::TitleInstance(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iCurrentState = TS_Logo;
	iTargetState = TS_Logo;

	iSelectStage = 0;

	// ソース管理を取得する
	pSourceManager = SourceManager::GetInstance();
}

//-----------------------------------------------------------------------------
// NAME: TitleInstance::~TitleInstance()
// DESC: ゲームタイトル処理 デストラクタ
//-----------------------------------------------------------------------------
TitleInstance::~TitleInstance()
{
	UninitTitle();
}

//-----------------------------------------------------------------------------
// NAME: HRESULT	TitleInstance::LoadTitle()
// DESC: ゲームタイトル処理 ゲームタイトル読込み
//-----------------------------------------------------------------------------
HRESULT	TitleInstance::LoadTitle()
{
	if (pSourceManager->GetLoadingState() == LS_Loading)
	{
		// ソースの読込み
		if (FAILED(LoadSource()))
		{
			MessageBox(0, "タイトルロードを失敗しました", NULL, MB_OK);
			return E_FAIL;
		}

		// ゲームタイトルの総初期化
		if (FAILED(InitObject()))
		{
			MessageBox(0, "タイトル初期化を失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}
//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateGameTitle()
// DESC: ゲームタイトル処理 ゲームタイトルソースの読込み
//-----------------------------------------------------------------------------
HRESULT TitleInstance::LoadSource()
{

	// テクスチャの読込み
	if (pSourceManager->GetLoadingProcess() == 0)
	{
		// テクスチャの読込み
		pSourceManager->LoadTexturefile(4, PATH_TITLETEXTURE, "sign.png");
		pSourceManager->LoadTexturefile(5, PATH_TITLETEXTURE, "title.jpg");
		pSourceManager->LoadTexturefile(6, PATH_TITLETEXTURE, "menu.png");
	}
	else if (pSourceManager->GetLoadingProcess() == 3)
	{
		// xファイルの読込み
		pSourceManager->LoadXfile(0, PATH_X_CASTLE, "Castle.x");
		pSourceManager->LoadXfile(1, PATH_X_CASTLE, "Cilff.x");
	}
	else if (pSourceManager->GetLoadingProcess() == 6)
	{
		// {スカイドームの初期化}
		pGSkyDome = new GSkyDome(pDevice);
		pGSkyDome->LoadSkyTexture(TEXTURE_SKYDOMENIGHT);
	}
	else if (pSourceManager->GetLoadingProcess() == 9)
	{
		// {フェードテクスチャの初期化}
		pFade = new GFadeTexture(pDevice);
		pFade->InitFadeTexture();
		pFade->SetAlpha(0);
	}

	pSourceManager->AddLoadingProcess(1);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT TitleInstance::InitObject()
// DESC: ゲームタイトル処理 ゲームタイトルオブジェクトの初期化処理
//-----------------------------------------------------------------------------
HRESULT TitleInstance::InitObject()
{
	if (pSourceManager->GetLoadingProcess() == 10)
	{
		// アンビエントライト（環境光）を設定する
		pDevice->SetRenderState(D3DRS_AMBIENT, TITLE_START_AMBIENT);

		// {ゲームコントローラー初期化}
		pGController = new GController(0);

		// {カメラ初期化}
		pTPCamera = new BThirdPersonCamera(pDevice);
		pTPCamera->SetTargetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 200.0f);
		pTPCamera->SetRadius(900.0f);
		pTPCamera->SetRotation(TITLE_START_CAMERAROT);

		// {ライト初期化}
		pLight = new BLight(pDevice, 0);
		pLight->InitDirectionalLight(D3DXVECTOR3(-0.5f, -1.0f, 0.5f), TITLE_START_LIGHTAMBIENT, TITLE_START_LIGHTDIFFUSE, TITLE_START_LIGHTSPECULAR);
		pLight->EnableLight();

		// {メッシュの初期化}
		pMesh[0] = new BMesh(pDevice);
		pMesh[0]->vScaling = D3DXVECTOR3(3.0f, 3.0, 3.0f);
		pMesh[0]->vFixPosition = D3DXVECTOR3(0.0f, -100.0, 0.0f);
		pMesh[1] = new BMesh(pDevice);
		pMesh[1]->vScaling = D3DXVECTOR3(2.0f, 2.0, 2.0f);
		pMesh[1]->vFixPosition = D3DXVECTOR3(-25.0f, -100.0, -90.0f);

		// {ゲームテクスチャの初期化}
		pGTexture[0] = new GTexture(pDevice);
		pGTexture[0]->InitGameTexture(D3DXVECTOR2(284.0f, 26.0f), D3DXVECTOR2(512.0f, 740.0f));
		pGTexture[1] = new GTexture(pDevice);
		pGTexture[1]->InitGameTexture(D3DXVECTOR2(524.0f, 312.0f), D3DXVECTOR2(512.0f, 180.0f));
		pGTexture[2] = new GTexture(pDevice);
		pGTexture[2]->InitGameTexture(D3DXVECTOR2(500.0f, 500.0f), D3DXVECTOR2(512.0f, 360.0f));

		// {スカイドームの初期化}
		pGSkyDome->InitSkyDome(5, 5, 6400.0f);
		pGSkyDome->vFixPosition = D3DXVECTOR3(0.0f, 800.0f, 0.0f);
		pGSkyDome->vFixRotation = D3DXVECTOR3(0.0f, D3DX_PI / 4, 0.0f);
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateController()
// DESC: ゲームタイトル処理 ゲームタイトルコントローラーの総更新処理
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateController()
{
	if (pFade->iFadeState == FS_Ready)
	{
		switch (iCurrentState)
		{
		case TS_Logo:
			// ゲームロゴコントローラーの更新
			UpdateLogoController();
			break;
		case TS_Start:
			// ゲームスタートコントローラーの更新
			UpdateStartController();
			break;
		case TS_Menu:
			// ゲームメニューコントローラーの更新
			UpdateMenuController();
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateTitle()
// DESC: ゲームタイトル処理 ゲームタイトルの更新処理
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateTitle()
{	
	// タイトル状態移行のフェード処理
	UpdateFade();

	switch (iCurrentState)
	{
	case TS_Logo:
		// ゲームロゴの更新
		UpdateLogoObject();
		break;
	case TS_Start:
		// ゲームスタートの更新
		UpdateStartObject();
		break;
	case TS_Menu:
		// ゲームメニューの更新
		UpdateMenuObject();
		break;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderTitle()
// DESC: ゲームタイトル処理 ゲームタイトルの総描画処理
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderTitle()
{
	switch (iCurrentState)
	{
	case TS_Logo:
		// ゲームロゴの描画
		RenderLogo();
		break;
	case TS_Start:
		// ゲームスタートの描画
		RenderStart();
		break;
	case TS_Menu:
		// ゲームメニューの描画
		RenderMenu();
		break;
	}

	// {フェードテクスチャの描画}
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(false);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateTitle()
// DESC: ゲームタイトル処理 ゲームタイトルの解放処理
//-----------------------------------------------------------------------------
VOID TitleInstance::UninitTitle()
{
	// 状態初期化
	iCurrentState = TS_Start;
	iTargetState = TS_Start;
	iSelectStage = 0;

	// {スカイドームの解放}
	SAFE_DELETE(pGSkyDome)

	// {メッシュの解放}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		SAFE_DELETE(pMesh[i])
	}

	// {ゲームテクスチャの解放}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		SAFE_DELETE(pGTexture[i])
	}

	// {フェードテクスチャの解放}
	SAFE_DELETE(pFade)
	// {ライト解放}
	SAFE_DELETE(pLight)
	// {カメラ解放}
	SAFE_DELETE(pTPCamera)
	// {ゲームコントローラー解放}
	SAFE_DELETE(pGController)
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateFade()
// DESC: ゲームタイトル処理 タイトル状態移行のフェード処理
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateFade()
{
	if (iCurrentState != iTargetState)
	{
		// フェードイン終了処理
		if ((pFade->iFadeState == FS_FadeIn) && (pFade->GetAlphaValue() == 255))
		{
			// フェードアウト開始処理
			pFade->iFadeState = FS_FadeOut;
			// タイトル状態更新
			iCurrentState = iTargetState;
		}
	}

	// フェードアウト終了処理
	if ((pFade->iFadeState == FS_FadeOut) && (pFade->GetAlphaValue() == 0))
	{
		// フェード状態回復
		pFade->iFadeState = FS_Ready;
	}

	// フェードテクスチャの更新
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->UpdateFadeTexture();
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateLogoController()
// DESC: ゲームタイトル処理 ゲームロゴコントローラーの更新処理
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
// DESC: ゲームタイトル処理 ゲームロゴオブジェクトの更新処理
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateLogoObject()
{
	// {ゲームテクスチャの更新}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		pGTexture[i]->UpdateGameTexture();
	}

}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::UpdateStartController()
// DESC: ゲームタイトル処理 ゲームスタートコントローラーの更新処理
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
// DESC: ゲームタイトル処理 ゲームスタートオブジェクトの更新処理
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateStartObject()
{
	// {ライトの更新}
	pLight->SetDirection(D3DXVECTOR3(pGSkyDome->vRotation.y,-1.0f,-0.5));

	// {スカイドームの更新}
	pGSkyDome->UpdateSkyDome();

	// {メッシュの更新}
	for (int i = 0; pMesh[i] != NULL; i++)
	{
		pMesh[i]->UpdateMesh();
	}

	// {ゲームテクスチャの更新}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		pGTexture[i]->UpdateGameTexture();
	}

	// {カメラ更新}
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
// DESC: ゲームタイトル処理 ゲームメニューコントローラーの更新処理
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
// DESC: ゲームタイトル処理 ゲームメニューオブジェクトの更新処理
//-----------------------------------------------------------------------------
VOID TitleInstance::UpdateMenuObject()
{
	// {ゲームテクスチャの更新}
	for (int i = 0; pGTexture[i] != NULL; i++)
	{
		pGTexture[i]->UpdateGameTexture();
	}

}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderLogo()
// DESC: ゲームタイトル処理 ゲームロゴの描画処理
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderLogo()
{
	// {ゲームテクスチャの描画}
	pGTexture[0]->RenderGameTexture(SetTexture(4));
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderLogo()
// DESC: ゲームタイトル処理 ゲームスタートの描画処理
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderStart()
{
	// < 1.背景(最先) >
	pGSkyDome->RenderSkyDome(false);

	// < 2.アルファテスト(透明物体) >
	// アルファテストの有効化(大なり)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// 不透明にする値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 40);

	// {メッシュの解放}
	pMesh[0]->RenderMesh(SetXfile(0));
	pMesh[1]->RenderMesh(SetXfile(1));

	// < 3.スクリーンテクスチャ >
	// {ゲームテクスチャの描画}
	//pGTexture[1]->RenderGameTexture(SetTexture(5));
}
//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::RenderLogo()
// DESC: ゲームタイトル処理 ゲームメニューの描画処理
//-----------------------------------------------------------------------------
VOID TitleInstance::RenderMenu()
{
	// {ゲームテクスチャの描画}
	pGTexture[2]->RenderGameTexture(SetTexture(6));
}

//-----------------------------------------------------------------------------
// NAME: VOID TitleInstance::ChangeTitleState(int state)
// DESC: ゲームタイトル処理 ゲームタイトルの変更
//-----------------------------------------------------------------------------
VOID TitleInstance::ChangeTitleState(int state)
{
	if (pFade->iFadeState == FS_Ready)
	{
		// タイトル変更
		iTargetState = state;

		// フェードイン開始処理
		pFade->iFadeState = FS_FadeIn;
	}
}

//-----------------------------------------------------------------------------
// NAME: LPDIRECT3DTEXTURE9 SetTexture(UINT n)
// DESC: ゲームタイトル処理 テクスチャ設置
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
// DESC: ゲームタイトル処理 xファイル設置
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
// DESC: ゲームタイトル処理 アニメーションxファイル設置
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
// DESC: ゲームステージ処理 Wavファイル設置
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
// (4) グローバル変数:
//*****************************************************************************//

//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//

//*****************************************************************************//
// (6) 関数
//*****************************************************************************//