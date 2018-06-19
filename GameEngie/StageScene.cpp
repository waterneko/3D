
//=============================================================================//
// 
// Name : StageScene.cpp
// Describe : �X�e�[�W�V�[�������N���X
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "StageScene.h"				// �X�e�[�W�V�[������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SceneInstance::SceneInstance()
// DESC: �X�e�[�W�V�[������ �R���X�g���N�^
//-----------------------------------------------------------------------------
SceneInstance::SceneInstance()
{
	bChangeFlag = false;
	iNextScene = 0;

	// �\�[�X�Ǘ����擾����
	pSourceManager = SourceManager::GetInstance();
}

//-----------------------------------------------------------------------------
// NAME: SceneInstance::~SceneInstance()
// DESC: �X�e�[�W�V�[������ �f�X�g���N�^
//-----------------------------------------------------------------------------
SceneInstance::~SceneInstance()
{

}

//-----------------------------------------------------------------------------
// NAME: HRESULT SceneInstance::InitScene()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���Ǎ���
//-----------------------------------------------------------------------------
HRESULT	SceneInstance::InitScene()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::UpdateController()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���R���g���[���[�̑��X�V����
//-----------------------------------------------------------------------------
VOID SceneInstance::UpdateController()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::UpdateScene()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���̑��X�V����
//-----------------------------------------------------------------------------
VOID SceneInstance::UpdateScene()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::RenderStage()
// DESC: �X�e�[�W�V�[������ �X�e�[�W�V�[���̑��`��
//-----------------------------------------------------------------------------
VOID SceneInstance::RenderScene()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::SetChangeSceneFlag(bool change)
// DESC: �X�e�[�W�V�[������ �V�[���؂�ւ��t���O�ݒu
//-----------------------------------------------------------------------------
VOID SceneInstance::SetChangeSceneFlag(bool change)
{
	bChangeFlag = change;
}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::SetNextScene(UINT nextscene)
// DESC: �X�e�[�W�V�[������ �V�[���؂�ւ�(0�͖߂�)
//-----------------------------------------------------------------------------
VOID SceneInstance::SetNextScene(UINT nextscene)
{
	iNextScene = nextscene;
}

//-----------------------------------------------------------------------------
// NAME: LPDIRECT3DTEXTURE9 SceneInstance::SetTexture(UINT n)
// DESC: �X�e�[�W�V�[������ �e�N�X�`���ݒu
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 SceneInstance::SetTexture(UINT n)
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
// NAME: LPDIRECT3DTEXTURE9 SceneInstance::SetTexture(UINT n)
// DESC: �X�e�[�W�V�[������ x�t�@�C���ݒu
//-----------------------------------------------------------------------------
SXfile* SceneInstance::SetXfile(UINT n)
{
	if (n <= pSourceManager->GetTextureNum())
	{
		return pSourceManager->GetXfile(n);
	}
	else
	{
		return pSourceManager->GetXfile(pSourceManager->GetTextureNum());
	}
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile* SceneInstance::SetAnimXfile(UINT n)
// DESC: �X�e�[�W�V�[������ �A�j���[�V����x�t�@�C���ݒu
//-----------------------------------------------------------------------------
SAnimXfile* SceneInstance::SetAnimXfile(UINT n)
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
// NAME: SWavfile* SceneInstance::SetWavfile(UINT n)
// DESC: �Q�[���X�e�[�W���� Wav�t�@�C���ݒu
//-----------------------------------------------------------------------------
SWavfile* SceneInstance::SetWavfile(UINT n)
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
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//

//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//