
//=============================================================================//
// 
// Name : Stage2.cpp
// Describe : ステージ2処理
// Author :
// Create Date : 2018/05/01
// Modify Date : 2018/05/25
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Stage2.h"				// ステージ1処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: Stage2::Stage2(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ステージ2処理 コンストラクタ
//-----------------------------------------------------------------------------
Stage2::Stage2(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iScene = GameStage2A;
	// {フェードテクスチャ}
	pFade = new GFadeTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: Stage2::~Stage2()
// DESC: ステージ2処理 デストラクタ
//-----------------------------------------------------------------------------
Stage2::~Stage2()
{
	// ステージ解放処理
	UninitStage();

	// {フェードテクスチャ}
	SAFE_DELETE(pFade)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Stage2::LoadStage()
// DESC: ステージ2処理 ステージの読込み
//-----------------------------------------------------------------------------
HRESULT	Stage2::LoadStage()
{
	if (iCurrentState == SS_Loading)
	{
		if (pSourceManager->GetLoadingProcess() == 3)
		{
			// xファイルの読込み
			pSourceManager->LoadXfile(2, PATH_X_CORRIDOR, "Corridor.x");
		}
		else if (pSourceManager->GetLoadingProcess() == 6)
		{
			// アニメーションxファイルの読込み
			pSourceManager->LoadAnimXfile(0, PATH_ANIM_YUKARI, "yukari_adv2.x", D3DINDEXED);
		}
		else if (pSourceManager->GetLoadingProcess() == 9)
		{
			// テクスチャの読込み
			pSourceManager->LoadTexturefile(7, PATH_STAGETEXTURE, "yukarinormal.png");
		}
		else if (pSourceManager->GetLoadingProcess() == 12)
		{
			// {ゲームテクスチャの初期化}
			pGTexture[0] = new GTexture(pDevice);
			pGTexture[0]->InitGameTexture(D3DXVECTOR2(124.0f, 154.0f), D3DXVECTOR2(90.0f, 90.0f));

			// {フェードテクスチャの初期化}
			pFade->InitFadeTexture();
			pFade->SetAlpha(0);
		}
		else if (pSourceManager->GetLoadingProcess() == 15)
		{
			// {ステージ2シーンの初期化}
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
// DESC: ステージ2処理 ステージの解放
//-----------------------------------------------------------------------------
VOID Stage2::UninitStage()
{
	// {ゲームテクスチャの解放}
	SAFE_DELETE(pGTexture[0])
	// {フェードテクスチャの解放} 
	SAFE_DELETE(pFade)
	// シーンの解放
	SAFE_DELETE(pStageScene)

	// テクスチャの解放
	pSourceManager->FreeTextureFile(7);
	// xファイルの解放
	pSourceManager->FreeXfile(2);
	//  アニメーションxファイルの解放
	pSourceManager->FreeAnimXfile(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::UpdateController()
// DESC: ステージ2処理 ステージコントローラーの総更新処理
//-----------------------------------------------------------------------------
VOID Stage2::UpdateController()
{
	if (!pStageScene->GetChangeSceneFlag())
	{
		if (pFade->iFadeState == FS_Ready)
		{
			// {シーンのコントローラー更新}
			pStageScene->UpdateController();
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::::UpdateStage()
// DESC: ステージ2処理 ステージの総更新処理
//-----------------------------------------------------------------------------
VOID Stage2::UpdateStage()
{
	// ステージのフェード処理
	UpdateFade();

	// シーンチェンジチェック
	if (pStageScene->GetChangeSceneFlag())
	{
		// タイトル戻す
		if (pStageScene->GetNextScene() == -1)
		{
			ReturnTitle();
		}
		// シーン移動
		else if (pStageScene->GetNextScene() > 4)
		{
			ChangeScene();
		}
		// ステージ移動
		else if ( (pStageScene->GetNextScene() <= 4) && (pStageScene->GetNextScene() > 0) )
		{
			ChangeStage();
		}
	}
		
	// シーンチェンジ処理
	if (iCurrentState == SS_ChangeScene)
	{
		// {ゲームシーン切替え}
		switch (iScene)
		{
		case GameStage2A:
			SAFE_DELETE(pStageScene)
			pStageScene = new Scene2A(pDevice);
			ReloadScene();
			break;
		}
	}
	// 現在のシーン処理
	else
	{
		// {シーンの更新}
		pStageScene->UpdateScene();

		// {ゲームテクスチャの更新}
		pGTexture[0]->UpdateGameTexture();
	}

	// デバッグ用IMGUI
	DebugImgui();
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::RenderStage()
// DESC: ステージ2処理 ステージの総描画
//-----------------------------------------------------------------------------
VOID Stage2::RenderStage()
{
	// {シーンの描画}
	if (iCurrentState == SS_Ready)
	{
		pStageScene->RenderScene();

		// {ゲームテクスチャの更新}
		pGTexture[0]->RenderGameTexture(SetTexture(7));
	}

	// {フェードテクスチャの描画}
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(false);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::UpdateFade()
// DESC: ステージ2処理 ステージのフェード処理
//-----------------------------------------------------------------------------
VOID Stage2::UpdateFade()
{
	if (iCurrentState != iTargetState)
	{
		// フェードイン終了処理
		if ((pFade->iFadeState == FS_FadeIn) && (pFade->GetAlphaValue() == 255))
		{
			// ステージ状態更新
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
// NAME: VOID Stage2::ChangeScene()
// DESC: ステージ2処理 シーンチェンジ処理
//-----------------------------------------------------------------------------
VOID Stage2::ChangeScene()
{
	if (pFade->iFadeState == FS_Ready)
	{
		// シーン変更
		iScene = pStageScene->GetNextScene();
		// シーンチェンジ開始
		iTargetState = SS_ChangeScene;
		// フェードイン開始処理
		pFade->iFadeState = FS_FadeIn;

	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Stage2::DebugImgui(UINT scene)
// DESC: ステージ2処理  シーンリロード処理
//-----------------------------------------------------------------------------
HRESULT Stage2::ReloadScene()
{
	if (pStageScene != NULL)
	{
		// シーン初期化
		if (SUCCEEDED(pStageScene->InitScene()))
		{
			iTargetState = SS_Ready;
			// シーンを戻す
			pStageScene->SetNextScene(GameDefaultStage);
			// シーン切替えフラグ終了
			pStageScene->SetChangeSceneFlag(false);
			// フェードアウト開始処理
			pFade->iFadeState = FS_FadeOut;
			// ステージ状態更新
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
// DESC: ステージ2処理 ステージチェンジ処理
//-----------------------------------------------------------------------------
VOID Stage2::ChangeStage()
{
	if (pFade->iFadeState == FS_Ready)
	{
		// シーン変更
		iScene = pStageScene->GetNextScene();
		// シーンチェンジ開始
		iTargetState = SS_ChangeStage;
		// シーンを戻す
		pStageScene->SetNextScene(GameDefaultStage);
		// シーン切替えフラグ終了
		pStageScene->SetChangeSceneFlag(false);
		// フェードイン開始処理
		pFade->iFadeState = FS_FadeIn;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage2::ReturnTitle()
// DESC: ステージ2処理 タイトル戻す処理
//-----------------------------------------------------------------------------
VOID Stage2::ReturnTitle()
{
	if (pFade->iFadeState == FS_Ready)
	{
		// シーン変更
		iScene = 0;
		// シーンチェンジ開始
		iTargetState = SS_ReturnStart;
		// シーンを戻す
		pStageScene->SetNextScene(GameDefaultStage);
		// シーン切替えフラグ終了
		pStageScene->SetChangeSceneFlag(false);
		// フェードイン開始処理
		pFade->iFadeState = FS_FadeIn;
	}
}


//-----------------------------------------------------------------------------
// NAME: VOID Stage1::DebugImgui()
// DESC: ステージ1処理 ステージデバッグ用IMGUI
//-----------------------------------------------------------------------------
VOID Stage2::DebugImgui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 380));
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin(u8"ステージ状態");
	ImGui::Text(u8"現在のステージ状態：%d", iCurrentState);
	ImGui::Text(u8"次のステージ状態：%d", iTargetState);
	ImGui::Text(u8"フェード状態: %d", pFade->iFadeState);
	ImGui::Text(u8"フェードアルファ値: %d", pFade->GetAlphaValue());
	ImGui::Text(u8"現在のシーン：%d", iScene);
	ImGui::Text(u8"現在のシーン切替えフラグ：%d", pStageScene->GetChangeSceneFlag());
	ImGui::Text(u8"次のシーン：%d", pStageScene->GetNextScene());
	ImGui::Text(u8"Mousex: %d", GetMouseX());
	ImGui::Text(u8"Mousey：%d", GetMouseX());
	ImGui::Text(u8"LスティックX: %d", GetDInput_LStickX(0));
	ImGui::Text(u8"LスティックX：%d", GetDInput_LStickY(0));
	ImGui::Text(u8"RスティックX：%d", GetDInput_RStickX(0));
	ImGui::Text(u8"RスティックX：%d", GetDInput_RStickY(0));
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