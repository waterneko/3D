
//=============================================================================//
// 
// Name : SkinMesh.h
// Describe : スキンメッシュ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "SkinMesh.h"				// スキンメッシュ処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BSkinMesh::BSkinMesh(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: スキンメッシュ処理 コンストラクタ
//-----------------------------------------------------------------------------
BSkinMesh::BSkinMesh(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	bUpdate = true;
	bRender = true;

	pAnimController = new BAnimController(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: BSkinMesh::~BSkinMesh()
// DESC: スキンメッシュ処理 デストラクタ
//-----------------------------------------------------------------------------
BSkinMesh::~BSkinMesh()
{
	SAFE_DELETE(pAnimController)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT BSkinMesh:InitSkinMesh(SAnimXfile* pAnimxfile)
// DESC: スキンメッシュ処理 スキンメッシュ初期化
//-----------------------------------------------------------------------------
HRESULT BSkinMesh::InitSkinMesh(SAnimXfile* animxfile)
{
	if (FAILED(pAnimXfile = animxfile))
	{
		// 初期化失敗
		MessageBox(0, "xファイル取得を失敗しました", NULL, MB_OK);
		return E_FAIL;
	}

	// アニメーションコントローラー初期化
	pAnimController->InitAnimController(animxfile);

	// ボーンマトリクスの計算
	SetupBoneMatrixPointers(pAnimXfile->pFrameRoot);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::UpdateAnimation(FLOAT fDeltaTime)
// DESC: スキンメッシュ処理 スキンメッシュアニメーション更新
//-----------------------------------------------------------------------------
VOID BSkinMesh::UpdateAnimation(FLOAT fDeltaTime)
{
	if (bUpdate)
	{
		// アニメーション更新
		pAnimController->UpdateAnimController(fDeltaTime);

		// アニメーションによってフレームのマトリクス更新
		UpdateFrameMatrices(pAnimXfile->pFrameRoot, &mtxWorld);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::UpdateSkinMesh()
// DESC: スキンメッシュ処理 スキンメッシュ更新
//-----------------------------------------------------------------------------
VOID BSkinMesh::UpdateSkinMesh()
{
	if (bUpdate)
	{
		// 毎フレームワールドマトリックスの初期化(ベクトル移動可能)
		D3DXMatrixIdentity(&mtxWorld);

		// ワールドトランスフォーム
		UpdateWorldTransform();

	}
}


//--------------------------------------------------------------------------------------
// NAME: VOID BSkinMesh::RenderSkinMesh()
// DESC: スキンメッシュ処理 スキンメッシュの描画
//--------------------------------------------------------------------------------------
VOID BSkinMesh::RenderSkinMesh()
{
	if (bRender)
	{
		// ワールドマトリクス変換
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// ライト消して自体の透明度を見えるようになった
		//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// フレームの描画
		DrawFrame(pAnimXfile->pFrameRoot);

		// ライト回復
		//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT BSkinMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
// DESC: スキンメッシュ処理 ボーンマトリクスの計算(階層)
//-----------------------------------------------------------------------------
HRESULT BSkinMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	HRESULT hr;

	if (pFrame->pMeshContainer != NULL)
	{
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}

	// 再帰関数で子フレームマトリクスを組み立て
	if (pFrame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT BSkinMesh::SetupBoneMatrixPointers(LPD3DXMESHCONTAINER pMeshContainerBase)
// DESC: スキンメッシュ処理 ボーンマトリクスの計算(単体)
//-----------------------------------------------------------------------------
HRESULT BSkinMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iBone, cBones;

	// クラス強制変換
	SkinMesh_XFRAME *pFrame;
	SkinMesh_MESHCONTAINER *pMeshContainer = (SkinMesh_MESHCONTAINER*)pMeshContainerBase;

	// 当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			// フレーム名を取得する
			pFrame = (SkinMesh_XFRAME*)D3DXFrameFind(pAnimXfile->pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
			if (pFrame == NULL)
				return E_FAIL;
			// フレームのマトリクスを計算する
			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
// DESC: スキンメッシュ処理 ボーンマトリクスの更新
//-----------------------------------------------------------------------------
VOID BSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	SkinMesh_XFRAME *pFrame = (SkinMesh_XFRAME*)pFrameBase;

	// 現ボーンのマトリクスと親ボーンのマトリクスを掛け算
	if (pParentMatrix != NULL)
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	// 再帰関数で子ボーンを掛け算
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::DrawFrame(LPD3DXFRAME pFrame)
// DESC: スキンメッシュ処理 フレームの描画(階層)
//-----------------------------------------------------------------------------
VOID BSkinMesh::DrawFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;	// メッシュコンテナーを取得する

												// 存在しているメッシュコンテナーを取得する(一般的に根フレームだけメッシュコンテナーが存在している)
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pMeshContainer, pFrame); // フレームの描画
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// 再帰関数で子フレームの描画
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
// DESC: スキンメッシュ処理 フレームの描画(単体)
//-----------------------------------------------------------------------------
VOID BSkinMesh::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	// クラス強制変換
	SkinMesh_MESHCONTAINER* pMeshContainer = (SkinMesh_MESHCONTAINER*)pMeshContainerBase;
	SkinMesh_XFRAME* pFrame = (SkinMesh_XFRAME*)pFrameBase;
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	UINT iPaletteEntry;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pDevice->GetDeviceCaps(&d3dCaps);

	// スキンメッシュの存在
	if (pMeshContainer->pSkinInfo != NULL)
	{
		if (pAnimXfile->pAllocateHier->iSkinMeshType == D3DNONINDEXED)		// インデックス情報無し
		{
			AttribIdPrev = UNUSED32;
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

			// デフォルトデバイスで頂点処理 (多分HW)
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				NumBlend = 0;
				for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
				{
					if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
					{
						NumBlend = i;
					}
				}

				if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
				{
					// ワールドマトリクス計算とマトリクスブレンディング数計算
					for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
					{
						iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
						if (iMatrixIndex != UINT_MAX)
						{
							D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
								pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
							pDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
						}
					}
					pDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

					// サブセットのマテリアルを設置
					if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
					{
						pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
						pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
						AttribIdPrev = pBoneComb[iAttrib].AttribId;
					}
					// サブセットメッシュを描画する
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);

				}
			}

			// もし必要があれば、HW処理できない部分はSWで処理
			if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
			{
				AttribIdPrev = UNUSED32;
				// ソフトウェア処理
				pDevice->SetSoftwareVertexProcessing(TRUE);
				for (iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
				{
					NumBlend = 0;
					for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
					{
						if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
						{
							NumBlend = i;
						}
					}

					if (d3dCaps.MaxVertexBlendMatrices < NumBlend + 1)
					{
						// ワールドマトリクス計算とマトリクスブレンディング数計算
						for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
						{
							iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
							if (iMatrixIndex != UINT_MAX)
							{
								D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
									pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
								pDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
							}
						}

						pDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

						// サブセットのマテリアルを設置
						if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
						{
							pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
							pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
							AttribIdPrev = pBoneComb[iAttrib].AttribId;
						}

						// サブセットメッシュを描画する
						pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
					}
				}
				// ソフトウェア処理終了
				pDevice->SetSoftwareVertexProcessing(FALSE);
			}

			// リセットブレンディングステータス
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		}
		else if (pAnimXfile->pAllocateHier->iSkinMeshType == D3DINDEXED)	// インデックスあり
		{
			if (pMeshContainer->UseSoftwareVP)
			{
				// ソフトウェア処理
				pDevice->SetSoftwareVertexProcessing(TRUE);
			}

			// 頂点ブレンディングインデックス数の設置
			if (pMeshContainer->NumInfl == 1)
			{
				pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
			}
			else
			{
				pDevice->SetRenderState(D3DRS_VERTEXBLEND, pMeshContainer->NumInfl - 1);
			}

			if (pMeshContainer->NumInfl)
			{
				pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
			}

			// メッシュ中属性グループ, 子マトリクス計算とサブセットメッシュを描画する
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				// ワールドマトリクス計算
				for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);

						pDevice->SetTransform(D3DTS_WORLDMATRIX(iPaletteEntry), &matTemp);
					}
				}

				// IDでマテリアルとテクスチャを設置
				pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
				pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);

				// サブセットメッシュを描画する
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}

			// リセットブレンディングステータス
			pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

			if (pMeshContainer->UseSoftwareVP)
			{
				// ソフトウェア処理終了
				pDevice->SetSoftwareVertexProcessing(FALSE);
			}
		}
	}
	//通常メッシュの場合
	else
	{
		//pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			pDevice->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
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