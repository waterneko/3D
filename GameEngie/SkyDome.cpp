
//=============================================================================//
// 
// Name : SkyDome.cpp
// Describe : �X�J�C�h�[������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "SkyDome.h"				// �X�J�C�h�[������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: GSkyDome::GSkyDome(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �X�J�C�h�[������ �R���X�g���N�^
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
// DESC: �X�J�C�h�[������ �f�X�g���N�^  
//-----------------------------------------------------------------------------
GSkyDome::~GSkyDome()
{
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pVertexBuffer);
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GSkyDome::LoadSkyTexture(char* Path)
// DESC: �X�J�C�h�[������ �e�N�X�`����Ǎ���
//-----------------------------------------------------------------------------
HRESULT GSkyDome::LoadSkyTexture(char* Path)
{
	// �e�N�X�`����Ǎ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, Path, &pTexture)))
	{
		MessageBox(NULL, "�X�J�C�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GSkyDome::InitSkyDome(int nAlpha, int nBeta, float fRadius)
// DESC: �X�J�C�h�[������ �X�J�C�h�[��������
//-----------------------------------------------------------------------------
HRESULT GSkyDome::InitSkyDome(int nAlpha, int nBeta, float fRadius)
{
	fSkySphereRadius = fRadius;                    // �X�t�B�A�̔��a
	iNumLatitudes = 360 / nAlpha;                  // �ܐ��̐�
	iNumLongitudes = 90 / nBeta;                   // �o���̐�
	iVertsPerLati = iNumLongitudes + 1;            // �ܐ��̒��_��
	iVertsPerLongi = iNumLatitudes + 1;            // �o���̒��_��
	iNumVertices = iVertsPerLati * iVertsPerLongi; // ���_����


	// [���_�o�b�t�@�[]
	// ���_�o�b�t�@�[������
	if (FAILED(pDevice->CreateVertexBuffer(iNumVertices * sizeof(VertexSkyDome),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEXSKY,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pVertexBuffer,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�[�����b�N����
	pVertexBuffer->Lock(0, 0, (void**)&VtxSkyDome, 0);

	// ���_�o�b�t�@�[���v�Z����
	int nIndex = 0;
	float fAlpha = 2.0f * D3DX_PI * nAlpha / 360.0f;       // �o�x���p�x�ɕϊ�����
	float fBeta = 2.0f * D3DX_PI * nBeta / 360.0f;         // �ܓx���p�x�ɕϊ�����
	for (int row = 0; row < iNumLongitudes + 1; row++)
	{
		for (int col = 0; col < iNumLatitudes + 1; col++)
		{
			// ���_���W���v�Z����
			VtxSkyDome[nIndex].vtx.x = fRadius * cosf(row * fBeta) * cosf(col * fAlpha);
			VtxSkyDome[nIndex].vtx.y = fRadius * sinf(row * fBeta);
			VtxSkyDome[nIndex].vtx.z = fRadius * cosf(row * fBeta) * sinf(col * fAlpha);
			// ���_�̃e�N�X�`�����W���v�Z����
			VtxSkyDome[nIndex].tex.x = col * fAlpha / (2.0f * D3DX_PI);
			VtxSkyDome[nIndex].tex.y = row * fBeta / (D3DX_PI / 2.0f);

			nIndex++;
		}
	}
	// ���_�o�b�t�@�[���A�����b�N����
	pVertexBuffer->Unlock();

	// [�C���f�b�N�X�o�b�t�@�[]
	// �C���f�b�N�X�o�b�t�@�[������
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
	// �C���f�b�N�X�o�b�t�@�[�����b�N����
	pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
	// �C���f�b�N�X�o�b�t�@�[���v�Z����
	nIndex = 0;
	for (int row = 0; row < iNumLongitudes; row++)
	{
		for (int col = 0; col < iNumLatitudes; col++)
		{
			// �O�p�`ABC�̒��_
			pIndices[nIndex + 0] = row   * iVertsPerLongi + col;
			pIndices[nIndex + 1] = (row + 1) * iVertsPerLongi + col;
			pIndices[nIndex + 2] = (row + 1) * iVertsPerLongi + col + 1;
			// �O�p�`CBD�̒��_
			pIndices[nIndex + 3] = row   * iVertsPerLongi + col;
			pIndices[nIndex + 4] = (row + 1) * iVertsPerLongi + col + 1;
			pIndices[nIndex + 5] = row   * iVertsPerLongi + col + 1;
			nIndex += 6;
		}
	}
	// �C���f�b�N�X�o�b�t�@�[���A�����b�N����
	pIndexBuffer->Unlock();

	// �������W�ݒu
	vPosition = D3DXVECTOR3(0.0f, INIT_POSY_SKYDOME, 0.0f);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GSkyDome::UpdateSkyDome()
// DESC: �X�J�C�h�[������ �X�J�C�h�[���X�V
//-----------------------------------------------------------------------------
VOID GSkyDome::UpdateSkyDome()
{
	if (bUpdate)
	{
		// ������]����
		if (bAutoRotation)
		{
			RotationByYAxis(0.002f);
		}
		// ���t���[�����[���h�}�g���b�N�X�̏�����(�x�N�g���ړ��\)
		D3DXMatrixIdentity(&mtxWorld);

		// ���[���h�g�����X�t�H�[���X�V
		UpdateWorldTransform();
	}
}


//--------------------------------------------------------------------------------------
// NAME: VOID GSkyDome::RenderSkyDome(bool bRenderFrame)
// DESC: �X�J�C�h�[������ �X�J�C�h�[���`��
//--------------------------------------------------------------------------------------
VOID GSkyDome::RenderSkyDome(bool bRenderFrame)
{
	// �Z�b�g���[���h�}�g���N�X
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (bRender)
	{
		pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VertexSkyDome)); 		// ���_�o�b�t�@�[�ݒu
		pDevice->SetFVF(FVF_VERTEXSKY);												// �X�K�C�X�t�B�A�pFVF�\����
		pDevice->SetIndices(pIndexBuffer); 											// �C���f�b�N�X�o�b�t�@�[�ݒu
		pDevice->SetTexture(0, pTexture); 											// ���_�o�b�t�@�[�ŕ`�悷��

		// �X�J�C�{�b�N�X�̃��^�����O����(�J�����O�ƌ��v�Z���Ȃ�)
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, iNumVertices, 0, iNumVertices * 2);		// ���_�o�b�t�@�[(�C���f�b�N�X����)��`�悷��

		pDevice->SetTexture(0, 0);													// �e�N�X�`���̖߂�

		// �X�J�C�X�t�B�A�̃��^�����O��߂�
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// ���̕`�攻��
		if (bRenderFrame)
		{
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);			// ���^�����O����(���̂�)
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, iNumVertices, 0, iNumVertices * 2); // ���_�o�b�t�@�[�ŕ`�悷��(���̂�)
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);				// ���^�����O����(�߂�)
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