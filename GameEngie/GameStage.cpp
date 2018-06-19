
//=============================================================================//
// 
// Name : GameStage.cpp
// Describe : �Q�[���X�e�[�W����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "GameStage.h"				// �Q�[���X�e�[�W����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: StageInstance::StageInstance()
// DESC: �Q�[���X�e�[�W���� �R���X�g���N�^
//-----------------------------------------------------------------------------
StageInstance::StageInstance()
{
	iCurrentState = SS_Loading;
	iTargetState = SS_Loading;

	// �\�[�X�Ǘ����擾����
	pSourceManager = SourceManager::GetInstance();
}

//-----------------------------------------------------------------------------
// NAME: StageInstance::~StageInstance()
// DESC: �Q�[���X�e�[�W���� �f�X�g���N�^
//-----------------------------------------------------------------------------
StageInstance::~StageInstance()
{
	UninitStage();
}

//-----------------------------------------------------------------------------
// NAME: HRESULT StageInstance::LoadStage()
// DESC: �Q�[���X�e�[�W���� �Q�[���^�C�g���Ǎ���
//-----------------------------------------------------------------------------
HRESULT	StageInstance::LoadStage()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID StageInstance::UpdatePlayerController()
// DESC: �Q�[���X�e�[�W���� �Q�[���X�e�[�W�̉��
//-----------------------------------------------------------------------------
VOID StageInstance::UninitStage()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID StageInstance::UpdatePlayerController()
// DESC: �Q�[���X�e�[�W���� �Q�[���X�e�[�W�R���g���[���[�̑��X�V����
//-----------------------------------------------------------------------------
VOID StageInstance::UpdateController()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID StageInstance::UpdateStage()
// DESC: �Q�[���X�e�[�W���� �Q�[���X�e�[�W�̑��X�V����
//-----------------------------------------------------------------------------
VOID StageInstance::UpdateStage()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID StageInstance::RenderStage()
// DESC: �Q�[���X�e�[�W���� �Q�[���X�e�[�W�̑��`��
//-----------------------------------------------------------------------------
VOID StageInstance::RenderStage()
{

}

//-----------------------------------------------------------------------------
// NAME: LPDIRECT3DTEXTURE9 StageInstance::SetTexture(UINT n)
// DESC: �Q�[���X�e�[�W���� �e�N�X�`���ݒu
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 StageInstance::SetTexture(UINT n)
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
// NAME: SXfile* StageInstance::SetXfile(UINT n)
// DESC: �Q�[���X�e�[�W���� x�t�@�C���ݒu
//-----------------------------------------------------------------------------
SXfile* StageInstance::SetXfile(UINT n)
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
// DESC: �Q�[���X�e�[�W���� �A�j���[�V����x�t�@�C���ݒu
//-----------------------------------------------------------------------------
SAnimXfile* StageInstance::SetAnimXfile(UINT n)
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
// NAME: SWavfile* StageInstance::SetWavfile(UINT n)
// DESC: �Q�[���X�e�[�W���� Wav�t�@�C���ݒu
//-----------------------------------------------------------------------------
SWavfile* StageInstance::SetWavfile(UINT n)
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