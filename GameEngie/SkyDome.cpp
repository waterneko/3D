
//=============================================================================//
// 
// Name : SkyDome.cpp
// Describe : スカイドーム処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "SkyDome.h"				// スカイドーム処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: GSkyDome::GSkyDome(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: スカイドーム処理 コンストラクタ
//-----------------------------------------------------------------------------
GSkyDome::GSkyDome(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	pTexture = NULL;
	pIndexBuffer = NULL;
	pVertexBuffer = NULL;

	iNumVertices = 0;
	iNumLatitudes = 0;
	iNumLongitudes = 0;
	iVertsPerLongi = 0;
	iVertsPerLati = 0;
	fSkySphereRadius = 0;

	bUpdate = true;
	bRender = true;
	bAutoRotation = true;

	VtxSkyDome = NULL;
}


//-----------------------------------------------------------------------------
// NAME: GSkyDome::~GSkyDome()
// DESC: スカイドーム処理 デストラクタ  
//-----------------------------------------------------------------------------
GSkyDome::~GSkyDome()
{
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pVertexBuffer);
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GSkyDome::LoadSkyTexture(char* Path)
// DESC: スカイドーム処理 テクスチャを読込み
//-----------------------------------------------------------------------------
HRESULT GSkyDome::LoadSkyTexture(char* Path)
{
	// テクスチャを読込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, Path, &pTexture)))
	{
		MessageBox(NULL, "スカイテクスチャの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GSkyDome::InitSkyDome(int nAlpha, int nBeta, float fRadius)
// DESC: スカイドーム処理 スカイドーム初期化
//-----------------------------------------------------------------------------
HRESULT GSkyDome::InitSkyDome(int nAlpha, int nBeta, float fRadius)
{
	fSkySphereRadius = fRadius;                    // スフィアの半径
	iNumLatitudes = 360 / nAlpha;                  // 緯線の数
	iNumLongitudes = 90 / nBeta;                   // 経線の数
	iVertsPerLati = iNumLongitudes + 1;            // 緯線の頂点数
	iVertsPerLongi = iNumLatitudes + 1;            // 経線の頂点数
	iNumVertices = iVertsPerLati * iVertsPerLongi; // 頂点総数


	// [頂点バッファー]
	// 頂点バッファー初期化
	if (FAILED(pDevice->CreateVertexBuffer(iNumVertices * sizeof(VertexSkyDome),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEXSKY,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&pVertexBuffer,				// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	// 頂点バッファーをロックする
	pVertexBuffer->Lock(0, 0, (void**)&VtxSkyDome, 0);

	// 頂点バッファーを計算する
	int nIndex = 0;
	float fAlpha = 2.0f * D3DX_PI * nAlpha / 360.0f;       // 経度を角度に変換する
	float fBeta = 2.0f * D3DX_PI * nBeta / 360.0f;         // 緯度を角度に変換する
	for (int row = 0; row < iNumLongitudes + 1; row++)
	{
		for (int col = 0; col < iNumLatitudes + 1; col++)
		{
			// 頂点座標を計算する
			VtxSkyDome[nIndex].vtx.x = fRadius * cosf(row * fBeta) * cosf(col * fAlpha);
			VtxSkyDome[nIndex].vtx.y = fRadius * sinf(row * fBeta);
			VtxSkyDome[nIndex].vtx.z = fRadius * cosf(row * fBeta) * sinf(col * fAlpha);
			// 頂点のテクスチャ座標を計算する
			VtxSkyDome[nIndex].tex.x = col * fAlpha / (2.0f * D3DX_PI);
			VtxSkyDome[nIndex].tex.y = row * fBeta / (D3DX_PI / 2.0f);

			nIndex++;
		}
	}
	// 頂点バッファーをアンロックする
	pVertexBuffer->Unlock();

	// [インデックスバッファー]
	// インデックスバッファー初期化
	if (FAILED(pDevice->CreateIndexBuffer(iNumVertices * 6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pIndexBuffer,
		0)))
	{
		return E_FAIL;
	}

	WORD* pIndices = NULL;
	// インデックスバッファーをロックする
	pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
	// インデックスバッファーを計算する
	nIndex = 0;
	for (int row = 0; row < iNumLongitudes; row++)
	{
		for (int col = 0; col < iNumLatitudes; col++)
		{
			// 三角形ABCの頂点
			pIndices[nIndex + 0] = row   * iVertsPerLongi + col;
			pIndices[nIndex + 1] = (row + 1) * iVertsPerLongi + col;
			pIndices[nIndex + 2] = (row + 1) * iVertsPerLongi + col + 1;
			// 三角形CBDの頂点
			pIndices[nIndex + 3] = row   * iVertsPerLongi + col;
			pIndices[nIndex + 4] = (row + 1) * iVertsPerLongi + col + 1;
			pIndices[nIndex + 5] = row   * iVertsPerLongi + col + 1;
			nIndex += 6;
		}
	}
	// インデックスバッファーをアンロックする
	pIndexBuffer->Unlock();

	// 初期座標設置
	vPosition = D3DXVECTOR3(0.0f, INIT_POSY_SKYDOME, 0.0f);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GSkyDome::UpdateSkyDome()
// DESC: スカイドーム処理 スカイドーム更新
//-----------------------------------------------------------------------------
VOID GSkyDome::UpdateSkyDome()
{
	if (bUpdate)
	{
		// 自動回転処理
		if (bAutoRotation)
		{
			RotationByYAxis(0.002f);
		}
		// 毎フレームワールドマトリックスの初期化(ベクトル移動可能)
		D3DXMatrixIdentity(&mtxWorld);

		// ワールドトランスフォーム更新
		UpdateWorldTransform();
	}
}


//--------------------------------------------------------------------------------------
// NAME: VOID GSkyDome::RenderSkyDome(bool bRenderFrame)
// DESC: スカイドーム処理 スカイドーム描画
//--------------------------------------------------------------------------------------
VOID GSkyDome::RenderSkyDome(bool bRenderFrame)
{
	// セットワールドマトリクス
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (bRender)
	{
		pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VertexSkyDome)); 		// 頂点バッファー設置
		pDevice->SetFVF(FVF_VERTEXSKY);												// スガイスフィア用FVF構造体
		pDevice->SetIndices(pIndexBuffer); 											// インデックスバッファー設置
		pDevice->SetTexture(0, pTexture); 											// 頂点バッファーで描画する

		// スカイボックスのレタリング処理(カリングと光計算しない)
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, iNumVertices, 0, iNumVertices * 2);		// 頂点バッファー(インデックスあり)を描画する

		pDevice->SetTexture(0, 0);													// テクスチャの戻す

		// スカイスフィアのレタリングを戻す
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// 線の描画判定
		if (bRenderFrame)
		{
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);			// レタリング処理(線のみ)
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, iNumVertices, 0, iNumVertices * 2); // 頂点バッファーで描画する(線のみ)
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);				// レタリング処理(戻る)
		}
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