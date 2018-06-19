
//=============================================================================//
// 
// Name : SourceManager.cpp
// Describe : �\�[�X�Ǘ�
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "SourceManager.h"				//  �\�[�X�Ǘ�

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SourceManager::SourceManager()
// DESC: �\�[�X�Ǘ� �R���X�g���N�^
//-----------------------------------------------------------------------------
SourceManager::SourceManager()
{

}

//-----------------------------------------------------------------------------
// NAME: SourceManager::SourceManager(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �\�[�X�Ǘ� �f�X�g���N�^
//-----------------------------------------------------------------------------
SourceManager::~SourceManager()
{

}

//-----------------------------------------------------------------------------
// NAME: SourceManager* SourceManager::GetInstance()
// DESC: �\�[�X�Ǘ� �C���X�^���X���擾����
//-----------------------------------------------------------------------------
SourceManager* SourceManager::GetInstance()
{
	return s_pInstance;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �\�[�X�Ǘ� �\�[�X�Ǘ�������
//-----------------------------------------------------------------------------
HRESULT SourceManager::InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice)
{
	// �C���X�^���X���ݔ��f
	if (s_pInstance != NULL)
	{
		pDevice = D3dDevice;

		iTextureNum = 0;
		iXfileNum = 0;
		iAnimXfileNum = 0;
		iWavfileNum = 0;

		iLoadingState = LS_Ready;
		iLoadingProcess = 0;
	}
	else
	{
		MessageBox(0, "�\�[�X�Ǘ������������s���܂���", "�x���I", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �\�[�X�Ǘ� �\�[�X�Ǘ����
//-----------------------------------------------------------------------------
VOID SourceManager::UninitSourceManager()
{
	for (UINT i = 0; i < iTextureNum + 1; i++)
	{
		SAFE_DELETE(pTexturefile[i])
	}

	for (UINT i = 0; i < iXfileNum + 1; i++)
	{
		SAFE_DELETE(pXfile[i])
	}

	for (UINT i = 0; i < iAnimXfileNum + 1; i++)
	{
		SAFE_DELETE(pAnimXfile[i])
	}

	for (UINT i = 0; i < iWavfileNum + 1; i++)
	{
		SAFE_DELETE(pWavfile[i])
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SourceManager::LoadTexturefile(int num, char* path, char* name)
// DESC: �\�[�X�Ǘ� �e�N�X�`����ǂݍ���
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadTexturefile(int num, char* path, char* name)
{
	// �t�@�C���`�F�b�N
	if (pTexturefile[num] == NULL)
	{ 
		pTexturefile[num] = new STexturefile(pDevice);

		if (SUCCEEDED(pTexturefile[num]->LoadTexturefile(path, name)))
		{
			iTextureNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SourceManager::LoadXfile(int num, char* path, char* name)
// DESC: �\�[�X�Ǘ� x�t�@�C����ǂݍ��݂�ǂݍ���
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadXfile(int num, char* path, char* name)
{
	// �t�@�C���`�F�b�N
	if (pXfile[num] == NULL)
	{
		pXfile[num] = new SXfile(pDevice);

		if (SUCCEEDED(pXfile[num]->LoadXfile(path, name)))
		{
			iXfileNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT LoadAnimXfile(int num, char* path, char* name, int type)
// DESC: �\�[�X�Ǘ� �A�j���[�V����x�t�@�C����ǂݍ���
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadAnimXfile(int num, char* path, char* name, int type)
{
	if (pAnimXfile[num] == NULL)
	{
		pAnimXfile[num] = new SAnimXfile(pDevice);

		if (SUCCEEDED(pAnimXfile[num]->LoadAnimXfile(path, name, type)))
		{
			iAnimXfileNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SourceManager::LoadWavfile(int num, char* path, char* name)
// DESC: �\�[�X�Ǘ� Wav�t�@�C����ǂݍ���
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadWavfile(int num, char* path, char* name)
{
	if (pWavfile[num] == NULL)
	{
		pWavfile[num] = new SWavfile();

		if (SUCCEEDED(pWavfile[num]->LoadWavFile(path, name)))
		{
			iWavfileNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: STexturefile* SourceManager::GetTextureFile(UINT n)
// DESC: �\�[�X�Ǘ� �e�N�X�`�����擾����
//-----------------------------------------------------------------------------
STexturefile* SourceManager::GetTextureFile(UINT n)
{
	if (n > iTextureNum)
	{
		return pTexturefile[iTextureNum];
	}
	else
	{
		return pTexturefile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: SXfile* SourceManager::GetXfile(UINT n)
// DESC: �\�[�X�Ǘ� x�t�@�C�����擾����
//-----------------------------------------------------------------------------
SXfile* SourceManager::GetXfile(UINT n)
{
	if (n > iXfileNum)
	{
		return pXfile[iXfileNum];
	}
	else
	{
		return pXfile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile* SourceManager::GetAnimXfile(UINT n)
// DESC: �\�[�X�Ǘ� �A�j���[�V����x�t�@�C�����擾����
//-----------------------------------------------------------------------------
SAnimXfile* SourceManager::GetAnimXfile(UINT n)
{
	if (n > iAnimXfileNum)
	{
		return pAnimXfile[iAnimXfileNum];
	}
	else
	{
		return pAnimXfile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: SWavfile* SourceManager::GetWavfile(UINT n)
// DESC: �\�[�X�Ǘ� Wav�t�@�C�����擾����
//-----------------------------------------------------------------------------
SWavfile* SourceManager::GetWavfile(UINT n)
{
	if (n > iWavfileNum)
	{
		return pWavfile[iWavfileNum];
	}
	else
	{
		return pWavfile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeTextureFile(UINT n)
// DESC: �\�[�X�Ǘ� �e�N�X�`�����������
//-----------------------------------------------------------------------------
VOID SourceManager::FreeTextureFile(UINT n)
{
	if (n <= iTextureNum)
	{
		SAFE_DELETE(pTexturefile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeTextureFile(UINT n)
// DESC: �\�[�X�Ǘ� x�t�@�C�����������
//-----------------------------------------------------------------------------
VOID SourceManager::FreeXfile(UINT n)
{
	if (n <= iXfileNum)
	{
		SAFE_DELETE(pXfile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeTextureFile(UINT n)
// DESC: �\�[�X�Ǘ� �A�j���[�V����x�t�@�C�����������
//-----------------------------------------------------------------------------
VOID SourceManager::FreeAnimXfile(UINT n)
{
	if (n <= iAnimXfileNum)
	{
		SAFE_DELETE(pAnimXfile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeWavfile(UINT n)
// DESC: �\�[�X�Ǘ� Wav�t�@�C�����������
//-----------------------------------------------------------------------------
VOID SourceManager::FreeWavfile(UINT n)
{
	if (n <= iWavfileNum)
	{
		SAFE_DELETE(pWavfile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::SetLoadingState(UINT state)
// DESC: �\�[�X�Ǘ� �Ǎ��ݏ�Ԑݒu
//-----------------------------------------------------------------------------
VOID SourceManager::SetLoadingState(UINT state)
{
	iLoadingState = state;
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::AddLoadingProcess(int n)
// DESC: �\�[�X�Ǘ� �Ǎ��݃v���Z�X�J�n
//-----------------------------------------------------------------------------
VOID SourceManager::AddLoadingProcess(int n)
{
	if (iLoadingState == LS_Loading)
	{
		iLoadingProcess += n;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::ResetLoadingProcess()
// DESC: �\�[�X�Ǘ� �Ǎ��݃v���Z�X���[�b�g
//-----------------------------------------------------------------------------
VOID SourceManager::ResetLoadingProcess()
{
	if (iLoadingState != LS_Loading)
	{
		iLoadingProcess = 0;
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