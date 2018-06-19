
//=============================================================================//
// 
// Name : Xfile.cpp
// Describe : x�t�@�C������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Xfile.h"				// x�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SXfile::SXfile(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: x�t�@�C������ �R���X�g���N�^
//-----------------------------------------------------------------------------
SXfile::SXfile(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	pMesh = NULL;
	pMeshMaterials = NULL;
	pMeshTextures = NULL;
	dwNumMaterials = 0;
}

//-----------------------------------------------------------------------------
// NAME: SXfile::~SXfile()
// DESC: x�t�@�C������ �f�X�g���N�^
//-----------------------------------------------------------------------------
SXfile::~SXfile()
{
	// �}�e���A���ւ̃|�C���^�[�̉��
	SAFE_DELETE_ARRAY(pMeshMaterials)

	// �e�N�X�`���ւ̃|�C���^�[�̉��
	for (UINT i = 0; i < dwNumMaterials; i++)
	{
		SAFE_RELEASE(pMeshTextures[i])
	}
	SAFE_DELETE_ARRAY(pMeshTextures);

	//���b�V���ւ̃|�C���^�[�̉��
	SAFE_RELEASE(pMesh);
}

//==========================================================================================================//
// NAME: HRESULT SXfile::LoadXfile(char* path, char* name)
// DESC: x�t�@�C������ x�t�@�C���ǂݍ���
//==========================================================================================================//
HRESULT SXfile::LoadXfile(char* path, char* name)
{
	LPD3DXBUFFER pAdjacencyBuffer = NULL;  //�אڐ��f�[�^�o�b�t�@�ւ̃|�C���^�[
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;   //�}�e���A���o�b�t�@�ւ̃|�C���^�[

	 // �t�@�C���̃p�X���󏈗�
	char FilePath[255];
	strcpy_s(FilePath, path);
	strcat_s(FilePath, name);

	// X�t�@�C�����烁�b�V�������[�h����	
	if (FAILED(D3DXLoadMeshFromX(FilePath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		&pAdjacencyBuffer,
		&pD3DXMtrlBuffer,
		NULL,
		&dwNumMaterials,
		&pMesh)))
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return E_FAIL;
	}

	// �}�e���A���ƃe�N�X�`���̏�����
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		//�}�e���A���ƃ��C�g�̐ݒ�
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;
		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// �e�N�X�`���̃p�X���󏈗�
			char textureFname[200];
			strcpy_s(textureFname, path);
			strcat_s(textureFname, d3dxMaterials[i].pTextureFilename);

			if (FAILED(D3DXCreateTextureFromFile(pDevice, textureFname, &pMeshTextures[i])))
			{
				MessageBox(NULL, "X�t�@�C���̃e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
			}
		}
	}

	//���b�V�����œK��
	pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_STRIPREORDER,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL);

	pD3DXMtrlBuffer->Release();
	pAdjacencyBuffer->Release();

	return S_OK;
}

//==========================================================================================================//
// NAME: VOID DXfile::SetShadingList(int charter)
// DESC: x�t�@�C������ �V�F�[�f�B���O���f���X�g
//==========================================================================================================//
//VOID DXfile::SetShadingList(int charter)
//{
//
//	pShadingList->OutLineList = new BOOL[dwNumMaterials];
//	pShadingList->ToonRenderList = new BOOL[dwNumMaterials];
//
//	for (int i = 0; i < dwNumMaterials; i++)
//	{
//		pShadingList->OutLineList[dwNumMaterials] = true;
//		pShadingList->ToonRenderList[dwNumMaterials] = true;
//	}
//
//	switch (charter)
//	{
//	case Mesh_Angela:
//		pShadingList->OutLineList[1] = false;
//		pShadingList->OutLineList[2] = false;
//		pShadingList->OutLineList[12] = false;
//		pShadingList->OutLineList[13] = false;
//		pShadingList->OutLineList[14] = false;
//		pShadingList->OutLineList[15] = false;
//		pShadingList->OutLineList[16] = false;
//		pShadingList->OutLineList[17] = false;
//		pShadingList->OutLineList[18] = false;
//		pShadingList->OutLineList[19] = false;
//
//		pShadingList->ToonRenderList[1] = false;
//		pShadingList->ToonRenderList[2] = false;
//		pShadingList->ToonRenderList[12] = false;
//
//		break;
//	case Mesh_Yukari:
//		// 0:�畆 1�F�� 2�F��� 3.�� 4.�� 5.�����N 6. �X�J�[�g 7. �� 8. �� 9.�� 10. �� 11.��(T) 12.? 13. �� 14.
//
//		pShadingList->OutLineList[3] = false;
//		pShadingList->OutLineList[10] = false;
//		pShadingList->OutLineList[11] = false;
//		pShadingList->OutLineList[13] = false;
//
//		pShadingList->ToonRenderList[0] = false;
//		pShadingList->ToonRenderList[10] = false;
//		pShadingList->ToonRenderList[13] = false;
//		break;
//	}
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