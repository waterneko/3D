
//=============================================================================//
// 
// Name : AnimXfile.cpp
// Describe : �A�j���[�V����x�t�@�C������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "AnimXfile.h"				// �A�j���[�V����x�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//--------------------------------------------------------------------------------------
// NAME: AllocateHierarchy::AllocateHierarchy(char* path, int type)
// DESC: �{�[�����b�V���N���X �R���X�g���N�^
//--------------------------------------------------------------------------------------
AllocateHierarchy::AllocateHierarchy(char* path, int type)
{
	strPath = path;
	iSkinMeshType = type;
	pBoneMatrices = NULL;
	iNumBoneMatricesMax = 0;
}

//--------------------------------------------------------------------------------------
// NAME: AllocateHierarchy::~AllocateHierarchy()
// DESC: �{�[�����b�V���N���X �f�X�g���N�^
//--------------------------------------------------------------------------------------
AllocateHierarchy::~AllocateHierarchy()
{

}

//--------------------------------------------------------------------------------------
// NAME: HRESULT AllocateHierarchy:: AllocateName(LPCSTR Name, LPSTR* pNewName)
// DESC: �{�[�����b�V���N���X �{�[�����b�V���̖��Ɠ������z
//--------------------------------------------------------------------------------------
HRESULT AllocateHierarchy::AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT AllocateHierarchy::GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, SkinMesh_MESHCONTAINER* pMeshContainer)
// DESC: �{�[�����b�V���N���X �X�L�����b�V���̐���
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, SkinMesh_MESHCONTAINER *pMeshContainer)
{
	HRESULT hr;

	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);

	// �����X�L����񑶍݂��Ȃ�������A�����I��
	if (pMeshContainer->pSkinInfo == NULL)
		return S_OK;

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

	if (iSkinMeshType == D3DNONINDEXED)	// �C���f�b�N�X��񖳂�
	{
		// ConvertToBlendedMesh�ŃX�L�����b�V�����쐬
		if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
			pMeshContainer->pOrigMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh)))

			return E_FAIL;

		// �f�o�C�X�}�g���N�X�Q�}�g���N�X�u�����f�B���O�����Ȃ�AConvertToBlendedMesh�őS�����b�V���ɏ����ł��Ȃ�
		// ���b�V���ɂQ�����ɕ�����āA�n�[�h�E�F�A(HW)�łQ�}�g���N�X�A���̕����̓\�t�g�E�F�A(SW)�ŏ�������
		LPD3DXBONECOMBINATION rgBoneCombinations = reinterpret_cast<LPD3DXBONECOMBINATION>(
			pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// �����ł��Ȃ��}�g���N�X��T���B
		for (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups;
		pMeshContainer->iAttributeSW++)
		{
			DWORD cInfl = 0;

			for (DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl++)
			{
				if (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX)
				{
					++cInfl;
				}
			}

			if (cInfl > d3dCaps.MaxVertexBlendMatrices)
			{
				break;
			}
		}

		// SW����
		if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
		{
			LPD3DXMESH pMeshTmp;

			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING |
				pMeshContainer->MeshData.pMesh->GetOptions(),
				pMeshContainer->MeshData.pMesh->GetFVF(),
				pd3dDevice, &pMeshTmp);
			if (FAILED(hr))
			{
				return E_FAIL;
			}

			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMeshTmp;
			pMeshTmp = NULL;
		}
		return S_OK;
	}
	else if (iSkinMeshType == D3DINDEXED)	// �C���f�b�N�X����
	{
		DWORD NumMaxFaceInfl;
		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

		LPDIRECT3DINDEXBUFFER9 pIB;
		hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
		if (FAILED(hr))
			return E_FAIL;

		hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB,
			pMeshContainer->pOrigMesh->GetNumFaces(),
			&NumMaxFaceInfl);

		pIB->Release();
		if (FAILED(hr))
			return E_FAIL;

		// �{�[������̐ݒu
		// 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
		// can be handled
		NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

		// HW SW���f����
		if (d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl)
		{
			// HW�̓C���f�b�N�X���_�u�����f�B���O�����ł��Ȃ��ASW�ŏ�������
			pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
			pMeshContainer->UseSoftwareVP = true;
			Flags |= D3DXMESH_SYSTEMMEM;
		}
		else
		{
			// HW����(�ő�}�g���N�X�̔���)
			//pMeshContainer->NumPaletteEntries = min((d3dCaps.MaxVertexBlendMatrixIndex + 1) / 2, pMeshContainer->pSkinInfo->GetNumBones());

			// Note :�v�Z���ԈႢ�ɂȂ�����,���s�ɂȂ�܂�,�����Ńf�o�C�X�Ɠ�������
			pMeshContainer->NumPaletteEntries = min((d3dCaps.MaxVertexBlendMatrixIndex), pMeshContainer->pSkinInfo->GetNumBones());
			pMeshContainer->UseSoftwareVP = false;
			Flags |= D3DXMESH_MANAGED;
		}

		// ConvertToIndexedBlendedMesh�ŃX�L�����b�V�����쐬
		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
			(
				pMeshContainer->pOrigMesh,
				Flags,
				pMeshContainer->NumPaletteEntries,
				pMeshContainer->pAdjacency,
				NULL, NULL, NULL,
				&pMeshContainer->NumInfl,
				&pMeshContainer->NumAttributeGroups,
				&pMeshContainer->pBoneCombinationBuf,
				&pMeshContainer->MeshData.pMesh);

		if (FAILED(hr))
			return E_FAIL;
	}
	else if (iSkinMeshType == D3DINDEXEDHLSLVS)	// �C���f�b�N�X���� + �o�[�e�b�N�X�E�V�F�[�_�[�g��
	{
		// Get palette size
		// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
		// (96 - 9) /3 i.e. Maximum constant count - used constants 
		// �{�[������̐ݒu
		UINT MaxMatrices = 26;
		pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

		// �o�[�e�b�N�X�E�V�F�[�_�[�`�F�b�N
		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
		if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
			Flags |= D3DXMESH_MANAGED;
		}
		else
		{
			MessageBox(NULL, "�V�F�[�_�[���g���Ȃ��I", "", MB_OK);
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		// ConvertToIndexedBlendedMesh�ŃX�L�����b�V�����쐬
		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
			(
				pMeshContainer->pOrigMesh,
				Flags,
				pMeshContainer->NumPaletteEntries,
				pMeshContainer->pAdjacency,
				NULL, NULL, NULL,
				&pMeshContainer->NumInfl,
				&pMeshContainer->NumAttributeGroups,
				&pMeshContainer->pBoneCombinationBuf,
				&pMeshContainer->MeshData.pMesh);

		if (FAILED(hr))
			return hr;


		// FVF �t�H�[�}�b�g�œK��. �V�F�[�_�[�g�������߂̂̃o�[�e�b�N�X�E�V�F�[�_�[�ɍ��킹�ݒ�
		DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
		{
			LPD3DXMESH pMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pd3dDevice, &pMesh);
			if (!FAILED(hr))
			{
				pMeshContainer->MeshData.pMesh->Release();
				pMeshContainer->MeshData.pMesh = pMesh;
				pMesh = NULL;
			}
		}

		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		LPD3DVERTEXELEMENT9 pDeclCur;
		hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
		if (FAILED(hr))
			return hr;

		// �o�[�e�b�N�X�E�V�F�[�_�[��UBYTE4�^����D3DCOLOR�����߂���, ������X�V 
		//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
		//          this is more of a "cast" operation
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
		if (FAILED(hr))
			return hr;

		// �{�[���s��o�b�t�@, �T�C�Y�̐ݒ�
		if (iNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
		{
			iNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

			// �{�[�������s��̃�������ԍĔz�u
			delete[] pBoneMatrices;
			pBoneMatrices = new D3DXMATRIX[iNumBoneMatricesMax];
			if (pBoneMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				return hr;
			}
		}
	}
	else
	{
		MessageBox(NULL, "�X�L�����b�V���������@�͖��I��", "", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
// DESC: �{�[�����b�V���N���X �t���[���̏�����
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	// �h���t���[���\����
	SkinMesh_XFRAME *pFrame;

	*ppNewFrame = NULL;
	// �h���t���[���̐���
	pFrame = new SkinMesh_XFRAME;
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		delete pFrame;
		return hr;
	}
	// �t���[�����̐ݒ�
	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
	{
		delete pFrame;
		return hr;
	}

	// �}�g���N�X�̏�����
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	delete pFrame;
	return hr;
}


//-----------------------------------------------------------------------------
// HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
// �{�[�����b�V������ ���b�V���R���e�i�[�̐���
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::CreateMeshContainer(LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT hr;
	// �h�����b�V���R���e�i�[
	SkinMesh_MESHCONTAINER *pMeshContainer = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPD3DXMESH pMesh = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;

	*ppNewMeshContainer = NULL;

	// ����D3DX�pMESH�𔭌�����Ȃ�������A���s�ɂȂ�
	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		hr = E_FAIL;
		return hr;
	}

	// ���b�V���f�[�^���擾���A���̃|�C���^�[��pMesh�ɓn��
	pMesh = pMeshData->pMesh;

	// �������b�V���f�[�^��FVF����Ȃ��A���s�ɂȂ�
	if (pMesh->GetFVF() == 0)
	{
		hr = E_FAIL;
		return hr;
	}

	// �h�����b�V���R���e�i�[�̏�����
	pMeshContainer = new SkinMesh_MESHCONTAINER;
	if (pMeshContainer == NULL)
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}
	memset(pMeshContainer, 0, sizeof(SkinMesh_MESHCONTAINER));

	// ���b�V���R���e�i�[���ƃ�������ݒu����
	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
	{
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return hr;
	}

	pMesh->GetDevice(&pDevice);
	NumFaces = pMesh->GetNumFaces();

	// �����@�����݂��Ă��Ȃ��A�@�����Đ�������(FVF�t�H�[�}�b�g)
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// �R�s�[���b�V��
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
		{
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// ���b�V���Ɋ܂܂��e���_�̖@�����v�Z����B
		D3DXComputeNormals(pMesh, NULL);
	}
	else  //  �����@�����݂��Ă���A�������b�V���R���e�i�[�̃f�[�^��n��
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// ���b�V���̃}�e���A���ƃe�N�X�`���̃�������Ԃ𕪔z����
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		hr = E_OUTOFMEMORY;
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return hr;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	// ���b�V���̃}�e���A���ƃe�N�X�`����ݒu
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				// �e�N�X�`�������݂��Ă���
				if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
				{
					// �e�N�X�`���p�X��ݒ肷��
					char textureFname[200];
					strcpy_s(textureFname, strPath);
					strcat_s(textureFname, pMeshContainer->pMaterials[iMaterial].pTextureFilename);

					if (FAILED(D3DXCreateTextureFromFileA(pDevice, textureFname,
						&pMeshContainer->ppTextures[iMaterial])))
						pMeshContainer->ppTextures[iMaterial] = NULL;

					// don't remember a pointer into the dynamic memory, just forget the name after loading
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
				}
			}
		}
	}
	else // ���b�V���̃f�t�H���g�ݒu
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// ���Y���b�V�����X�L�j���O���������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
	if (pSkinInfo != NULL)
	{
		// �X�L�����ƃI���W�i�����b�V����ۑ�����
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		// �{�[���̐����擾����
		cBones = pSkinInfo->GetNumBones();

		// �{�[���̏����}�g���N�X�̓����𕪔z����
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}

		// �{�[���̏����}�g���N�X���擾����
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}

		// �X�L�����b�V����������
		hr = GenerateSkinnedMesh(pDevice, pMeshContainer);
		if (FAILED(hr))
		{
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}
	}

	// ���[�J���ɐ����������b�V���R���e�i�[�|�C���^�[�̃|�C���^�[��ݒu
	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

	// �������������
	if (pMeshContainer != NULL)
	{
		DestroyMeshContainer(pMeshContainer);
	}

	return hr;
}

//-----------------------------------------------------------------------------
// HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
// �{�[�����b�V������ �t���[����j������
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

//-----------------------------------------------------------------------------
// HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
// �{�[�����b�V������ ���b�V���R���e�i�[��j������
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iMaterial;
	SkinMesh_MESHCONTAINER* pMeshContainer = (SkinMesh_MESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	// �S�e�N�X�`���������
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_DELETE(pMeshContainer);
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile::SAnimXfile(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �A�j���[�V����x�t�@�C������ �R���X�g���N�^
//-----------------------------------------------------------------------------
SAnimXfile::SAnimXfile(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	pFrameRoot = NULL;
	pOrigAnimController = NULL;
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile::~SAnimXfile()
// DESC: �A�j���[�V����x�t�@�C������ �f�X�g���N�^
//-----------------------------------------------------------------------------
SAnimXfile::~SAnimXfile()
{
	// ���b�V���ƃt���[����j������
	D3DXFrameDestroy(pFrameRoot, pAllocateHier);

	// �A�j���[�V�����R���g���[����j������
	SAFE_RELEASE(pOrigAnimController);
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SAnimXfile::LoadAnimXfile(char* path, char* name, int type)
// DESC: �A�j���[�V����x�t�@�C�� �t�@�C���ǂݍ���
//-----------------------------------------------------------------------------
HRESULT SAnimXfile::LoadAnimXfile(char* path, char* name, int type)
{
	// �h���{�[�����b�V���N���X������
	pAllocateHier = new AllocateHierarchy(path, type);

	// �t�@�C���̃p�X���󏈗�
	char FilePath[255];
	strcpy_s(FilePath, path);
	strcat_s(FilePath, name);

	// X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
	if (FAILED(
		D3DXLoadMeshHierarchyFromX(FilePath,
			D3DXMESH_MANAGED,
			pDevice,             // �f�o�C�X�I�u�W�F�N�g
			pAllocateHier,       // �{�[���N���X
			NULL,
			&pFrameRoot,         // �t���[���\����
			&pOrigAnimController)))  // �A�j���[�V�����R���g���[��
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", FilePath, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID DAnimXfile::SetShadingList(int charter)
// DESC: �A�j���[�V����x�t�@�C�� �V�F�[�f�B���O���f���X�g
//-----------------------------------------------------------------------------
//VOID DAnimXfile::SetShadingList(int charter)
//{
//		switch (charter)
//		{
//		case SkinMesh_Angela:
//			break;
//		case SkinMesh_Yukari:
//			pShadingList->OutLineList = new BOOL[14];
//			pShadingList->ToonRenderList = new BOOL[14];
//
//			// 0:�畆 1�F�畆 2�F��(�O) 3.��+�� 4.�� 5.��� 6. ��(��) 7. �Ђ� 8. ��2 9.�����N 10. �X�J�[�g 11.�O�� 12.�㔯 13. ���� 
//
//			for (int i = 0; i < 14; i++)
//			{
//				pShadingList->OutLineList[i] = true;
//				pShadingList->ToonRenderList[i] = true;
//			}
//
//			pShadingList->OutLineList[0] = false;
//			pShadingList->OutLineList[6] = false;
//			pShadingList->OutLineList[9] = false;
//			//pShadingList->OutLineList[11] = false;
//			//pShadingList->OutLineList[13] = false;
//
//			pShadingList->ToonRenderList[0] = false;
//			pShadingList->ToonRenderList[1] = false;
//			break;
//		}
//}


//*****************************************************************************//
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//

//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//