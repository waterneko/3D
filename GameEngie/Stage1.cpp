
//=============================================================================//
// 
// Name : Stage1.cpp
// Describe : ステージ1処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Stage1.h"				// ステージ1処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: Stage1::Stage1(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ステージ1処理 コンストラクタ
//-----------------------------------------------------------------------------
Stage1::Stage1(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iScene = GameStage1A;
	// {フェードテクスチャ}
	pFade = new GFadeTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: Stage1::~Stage1()
// DESC: ステージ1処理 デストラクタ
//-----------------------------------------------------------------------------
Stage1::~Stage1()
{
	// ステージ解放処理
	UninitStage();

	// {フェードテクスチャ}
	SAFE_DELETE(pFade)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT Stage1::LoadStage()
// DESC: ステージ1処理 ステージの読込み
//-----------------------------------------------------------------------------
HRESULT	Stage1::LoadStage()
{
	if (iCurrentState == SS_Loading)
	{
		if (pSourceManager->GetLoadingProcess() == 3)
		{
			// xファイルの読込み
			pSourceManager->LoadXfile(2, PATH_X_ROOM, "Room1.x");
			pSourceManager->LoadXfile(3, PATH_X_SLOPE, "Slope.x");
			pSourceManager->LoadXfile(4, PATH_X_ROOM, "Stoneroom1.x");
		}
		else if (pSourceManager->GetLoadingProcess() == 6)
		{
			// アニメーションxファイルの読込み
			pSourceManager->LoadAnimXfile(0, PATH_ANIM_YUKARI, "yukari_adv2.x", D3DINDEXED);
		}
		else if (pSourceManager->GetLoadingProcess() == 9)
		{
			// テクスチャの読込み
			pSourceManager->LoadTexturefile(7, PATH_STAGETEXTURE, "Stage1.png");
			pSourceManager->LoadTexturefile(8, PATH_STAGETEXTURE, "yukarinormal.png");
		}
		else if (pSourceManager->GetLoadingProcess() == 12)
		{
			// Wavファイルの読込み
			pSourceManager->LoadWavfile(0, PATH_BGM_WAV, "bgm_01.wav");
		}
		else if (pSourceManager->GetLoadingProcess() == 14)
		{
			// {ゲームテクスチャの初期化}
			pGTexture[0] = new GTexture(pDevice);
			pGTexture[0]->InitGameTexture(D3DXVECTOR2(124.0f, 154.0f), D3DXVECTOR2(90.0f, 90.0f));

			// {フェードテクスチャの初期化}
			pFade->InitFadeTexture();
			pFade->SetAlpha(0);

		}
		else if (pSourceManager->GetLoadingProcess() == 16)
		{
			// {ステージ1シーンの初期化}
			pStageScene = new Scene1A(pDevice);
			pStageScene->InitScene();
		}
		else if (pSourceManager->GetLoadingProcess() == 18)
		{
			// {XAudio2ソースボイスの初期化}
			pSourceVoice[0] = new BXA2SourceVoice();
			pSourceVoice[0]->InitSourceVoice(SetWavfile(0), true);
		}
		else if (pSourceManager->GetLoadingProcess() == 20)
		{
			iTargetState = SS_Ready;
			iCurrentState = iTargetState;
			// {XAudio2ソースボイスの再生}
			pSourceVoice[0]->PlaySourceVoice();
		}

		pSourceManager->AddLoadingProcess(1);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::UninitStage()
// DESC: ステージ1処理 ステージの解放
//-----------------------------------------------------------------------------
VOID Stage1::UninitStage()
{
	// {ゲームテクスチャの解放}
	SAFE_DELETE(pGTexture[0])
	// {フェードテクスチャの解放} 
	SAFE_DELETE(pFade)
	// {XAudio2ソースボイスの解放}
	for (int i = 0; pSourceVoice[i] != NULL; i++)
	{
		SAFE_DELETE(pSourceVoice[i])
	}
	// シーンの解放
	SAFE_DELETE(pStageScene)

	// テクスチャの解放
	pSourceManager->FreeTextureFile(7);
	pSourceManager->FreeTextureFile(8);
	// xファイルの解放
	pSourceManager->FreeXfile(2);
	pSourceManager->FreeXfile(3);
	pSourceManager->FreeXfile(4);
	// アニメーションxファイルの解放
	pSourceManager->FreeAnimXfile(0);
	// Wavファイルの解放
	pSourceManager->FreeWavfile(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::UpdateController()
// DESC: ステージ1処理 ステージコントローラーの総更新処理
//-----------------------------------------------------------------------------
VOID Stage1::UpdateController()
{
	if (!pStageScene->GetChangeSceneFlag())
	{
		if (pFade->iFadeState == FS_Ready)
		{
			// {シーンのコントローラー更新}
			pStageScene->UpdateController();
		}

		// BGMコントロール
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
// DESC: ステージ1処理 ステージの総更新処理
//-----------------------------------------------------------------------------
VOID Stage1::UpdateStage()
{
	// ステージのフェード処理
	UpdateFade();

	// 現在のシーン処理
	// シーンチェンジ処理
	if (pStageScene->GetChangeSceneFlag())
	{
		// タイトル戻す
		if (pStageScene->GetNextScene() == -1)
		{
			ReturnTitle();
		}
		// シーン移動
		else if ((pStageScene->GetNextScene() <= 4) && (pStageScene->GetNextScene() > 0))
		{
			ChangeScene();
		}
		// ステージ移動
		else if (pStageScene->GetNextScene() > 4) 
		{
			ChangeStage();
		}
	}

	if ((iCurrentState == SS_ChangeScene))
	{
		// {ゲームシーン切替え}
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
// NAME: VOID Stage1::RenderStage()
// DESC: ステージ1処理 ステージの総描画
//-----------------------------------------------------------------------------
VOID Stage1::RenderStage()
{
	// {シーンの描画}
	if (iCurrentState == SS_Ready)
	{
		pStageScene->RenderScene();

		// {ゲームテクスチャの更新}
		pGTexture[0]->RenderGameTexture(SetTexture(8));
	}

	// {フェードテクスチャの描画}
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(false);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID Stage1::UpdateFade()
// DESC: ステージ1処理 ステージのフェード処理
//-----------------------------------------------------------------------------
VOID Stage1::UpdateFade()
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
// NAME: VOID Stage1::ChangeScene()
// DESC: ステージ1処理 シーンチェンジ処理
//-----------------------------------------------------------------------------
VOID Stage1::ChangeScene()
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
// NAME: HRESULT Stage1::DebugImgui(UINT scene)
// DESC: ステージ1処理  シーンリロード処理
//-----------------------------------------------------------------------------
HRESULT Stage1::ReloadScene()
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
// NAME: VOID Stage1::ChangeScene()
// DESC: ステージ1処理 ステージチェンジ処理
//-----------------------------------------------------------------------------
VOID Stage1::ChangeStage()
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
// NAME: VOID Stage1::ReturnTitle()
// DESC: ステージ1処理 タイトル戻す処理
//-----------------------------------------------------------------------------
VOID Stage1::ReturnTitle()
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
VOID Stage1::DebugImgui()
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