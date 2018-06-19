
//=============================================================================//
// 
// Name : SkinMesh.h
// Describe : �X�L�����b�V������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "SkinMesh.h"				// �X�L�����b�V������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BSkinMesh::BSkinMesh(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �X�L�����b�V������ �R���X�g���N�^
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
// DESC: �X�L�����b�V������ �f�X�g���N�^
//-----------------------------------------------------------------------------
BSkinMesh::~BSkinMesh()
{
	SAFE_DELETE(pAnimController)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT BSkinMesh:InitSkinMesh(SAnimXfile* pAnimxfile)
// DESC: �X�L�����b�V������ �X�L�����b�V��������
//-----------------------------------------------------------------------------
HRESULT BSkinMesh::InitSkinMesh(SAnimXfile* animxfile)
{
	if (FAILED(pAnimXfile = animxfile))
	{
		// ���������s
		MessageBox(0, "x�t�@�C���擾�����s���܂���", NULL, MB_OK);
		return E_FAIL;
	}

	// �A�j���[�V�����R���g���[���[������
	pAnimController->InitAnimController(animxfile);

	// �{�[���}�g���N�X�̌v�Z
	SetupBoneMatrixPointers(pAnimXfile->pFrameRoot);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::UpdateAnimation(FLOAT fDeltaTime)
// DESC: �X�L�����b�V������ �X�L�����b�V���A�j���[�V�����X�V
//-----------------------------------------------------------------------------
VOID BSkinMesh::UpdateAnimation(FLOAT fDeltaTime)
{
	if (bUpdate)
	{
		// �A�j���[�V�����X�V
		pAnimController->UpdateAnimController(fDeltaTime);

		// �A�j���[�V�����ɂ���ăt���[���̃}�g���N�X�X�V
		UpdateFrameMatrices(pAnimXfile->pFrameRoot, &mtxWorld);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::UpdateSkinMesh()
// DESC: �X�L�����b�V������ �X�L�����b�V���X�V
//-----------------------------------------------------------------------------
VOID BSkinMesh::UpdateSkinMesh()
{
	if (bUpdate)
	{
		// ���t���[�����[���h�}�g���b�N�X�̏�����(�x�N�g���ړ��\)
		D3DXMatrixIdentity(&mtxWorld);

		// ���[���h�g�����X�t�H�[��
		UpdateWorldTransform();

	}
}


//--------------------------------------------------------------------------------------
// NAME: VOID BSkinMesh::RenderSkinMesh()
// DESC: �X�L�����b�V������ �X�L�����b�V���̕`��
//--------------------------------------------------------------------------------------
VOID BSkinMesh::RenderSkinMesh()
{
	if (bRender)
	{
		// ���[���h�}�g���N�X�ϊ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		// ���C�g�����Ď��̂̓����x��������悤�ɂȂ���
		//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �t���[���̕`��
		DrawFrame(pAnimXfile->pFrameRoot);

		// ���C�g��
		//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT BSkinMesh::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
// DESC: �X�L�����b�V������ �{�[���}�g���N�X�̌v�Z(�K�w)
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

	// �ċA�֐��Ŏq�t���[���}�g���N�X��g�ݗ���
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
// DESC: �X�L�����b�V������ �{�[���}�g���N�X�̌v�Z(�P��)
//-----------------------------------------------------------------------------
HRESULT BSkinMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iBone, cBones;

	// �N���X�����ϊ�
	SkinMesh_XFRAME *pFrame;
	SkinMesh_MESHCONTAINER *pMeshContainer = (SkinMesh_MESHCONTAINER*)pMeshContainerBase;

	// ���Y���b�V�����X�L�����������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			// �t���[�������擾����
			pFrame = (SkinMesh_XFRAME*)D3DXFrameFind(pAnimXfile->pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
			if (pFrame == NULL)
				return E_FAIL;
			// �t���[���̃}�g���N�X���v�Z����
			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
// DESC: �X�L�����b�V������ �{�[���}�g���N�X�̍X�V
//-----------------------------------------------------------------------------
VOID BSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	SkinMesh_XFRAME *pFrame = (SkinMesh_XFRAME*)pFrameBase;

	// ���{�[���̃}�g���N�X�Ɛe�{�[���̃}�g���N�X���|���Z
	if (pParentMatrix != NULL)
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	// �ċA�֐��Ŏq�{�[�����|���Z
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
// DESC: �X�L�����b�V������ �t���[���̕`��(�K�w)
//-----------------------------------------------------------------------------
VOID BSkinMesh::DrawFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;	// ���b�V���R���e�i�[���擾����

												// ���݂��Ă��郁�b�V���R���e�i�[���擾����(��ʓI�ɍ��t���[���������b�V���R���e�i�[�����݂��Ă���)
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pMeshContainer, pFrame); // �t���[���̕`��
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// �ċA�֐��Ŏq�t���[���̕`��
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
// DESC: �X�L�����b�V������ �t���[���̕`��(�P��)
//-----------------------------------------------------------------------------
VOID BSkinMesh::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	// �N���X�����ϊ�
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

	// �X�L�����b�V���̑���
	if (pMeshContainer->pSkinInfo != NULL)
	{
		if (pAnimXfile->pAllocateHier->iSkinMeshType == D3DNONINDEXED)		// �C���f�b�N�X��񖳂�
		{
			AttribIdPrev = UNUSED32;
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

			// �f�t�H���g�f�o�C�X�Œ��_���� (����HW)
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
					// ���[���h�}�g���N�X�v�Z�ƃ}�g���N�X�u�����f�B���O���v�Z
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

					// �T�u�Z�b�g�̃}�e���A����ݒu
					if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
					{
						pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
						pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
						AttribIdPrev = pBoneComb[iAttrib].AttribId;
					}
					// �T�u�Z�b�g���b�V����`�悷��
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);

				}
			}

			// �����K�v������΁AHW�����ł��Ȃ�������SW�ŏ���
			if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
			{
				AttribIdPrev = UNUSED32;
				// �\�t�g�E�F�A����
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
						// ���[���h�}�g���N�X�v�Z�ƃ}�g���N�X�u�����f�B���O���v�Z
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

						// �T�u�Z�b�g�̃}�e���A����ݒu
						if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
						{
							pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
							pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
							AttribIdPrev = pBoneComb[iAttrib].AttribId;
						}

						// �T�u�Z�b�g���b�V����`�悷��
						pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
					}
				}
				// �\�t�g�E�F�A�����I��
				pDevice->SetSoftwareVertexProcessing(FALSE);
			}

			// ���Z�b�g�u�����f�B���O�X�e�[�^�X
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		}
		else if (pAnimXfile->pAllocateHier->iSkinMeshType == D3DINDEXED)	// �C���f�b�N�X����
		{
			if (pMeshContainer->UseSoftwareVP)
			{
				// �\�t�g�E�F�A����
				pDevice->SetSoftwareVertexProcessing(TRUE);
			}

			// ���_�u�����f�B���O�C���f�b�N�X���̐ݒu
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

			// ���b�V���������O���[�v, �q�}�g���N�X�v�Z�ƃT�u�Z�b�g���b�V����`�悷��
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				// ���[���h�}�g���N�X�v�Z
				for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);

						pDevice->SetTransform(D3DTS_WORLDMATRIX(iPaletteEntry), &matTemp);
					}
				}

				// ID�Ń}�e���A���ƃe�N�X�`����ݒu
				pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
				pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);

				// �T�u�Z�b�g���b�V����`�悷��
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}

			// ���Z�b�g�u�����f�B���O�X�e�[�^�X
			pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

			if (pMeshContainer->UseSoftwareVP)
			{
				// �\�t�g�E�F�A�����I��
				pDevice->SetSoftwareVertexProcessing(FALSE);
			}
		}
	}
	//�ʏ탁�b�V���̏ꍇ
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
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//

//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//