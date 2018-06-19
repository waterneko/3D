//=============================================================================//
// 
// Name :  AnimationController.cpp
// Describe : �A�j���[�V�����R���g���[���[����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "AnimationController.h"				// �A�j���[�V�����R���g���[���[����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BAnimController::DAnimController(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �A�j���[�V�����R���g���[���[���� �R���X�g���N�^
//-----------------------------------------------------------------------------
BAnimController::BAnimController(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	bAnimation = true;

	iCurrentTrack = 0;
	iAnimIndex = 0;
	iAnimMaxNum = 0;
	iAnimFrame = 0;

	bAnimBlend = false;
	fShiftTime = SMOOTH_FRAMETIME;
	fCurWeightTime = 0.0f;

	pAnimController = NULL;
	pAnimSet = NULL;
	ZeroMemory(&TrackDesc[0], sizeof(TrackDesc[0]));
	ZeroMemory(&TrackDesc[1], sizeof(TrackDesc[1]));
}

//-----------------------------------------------------------------------------
// NAME: BAnimController::~BAnimController()
// DESC: �A�j���[�V�����R���g���[���[���� �f�X�g���N�^
//-----------------------------------------------------------------------------
BAnimController::~BAnimController()
{
	SAFE_RELEASE(pAnimController)
		//SAFE_RELEASE(pAnimSet)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT BAnimController::InitAnimController(DAnimXfile* pAnimxfile)	
// DESC: �A�j���[�V�����R���g���[���[���� �A�j���[�V�����R���g���[���[������
//-----------------------------------------------------------------------------
HRESULT BAnimController::InitAnimController(SAnimXfile* animxfile)
{
	// �A�j���[�V�����R���g���[�����R�s�[����
	animxfile->pOrigAnimController->CloneAnimationController(
		animxfile->pOrigAnimController->GetMaxNumAnimationOutputs(),
		animxfile->pOrigAnimController->GetNumAnimationSets(),
		animxfile->pOrigAnimController->GetMaxNumTracks(),
		animxfile->pOrigAnimController->GetMaxNumEvents(),
		&pAnimController);

	iAnimMaxNum = animxfile->pOrigAnimController->GetNumAnimationSets();

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BAnimController::UpdateAnimController(FLOAT time)
// DESC: �A�j���[�V�����R���g���[���[���� �A�j���[�V�����X�V
//-----------------------------------------------------------------------------
VOID BAnimController::UpdateAnimController(FLOAT time)
{
	// ���������ۂ��𔻒�
	if (bAnimBlend)
	{
		fCurWeightTime += time;
		if (fCurWeightTime <= fShiftTime)
		{
			// �������B�E�F�C�g���Z�o
			FLOAT Weight = fCurWeightTime / fShiftTime;
			// �E�F�C�g��o�^
			pAnimController->SetTrackWeight(0, Weight);       // ���݂̃A�j���[�V����
			pAnimController->SetTrackWeight(1, 1 - Weight);   // �O�̃A�j���[�V����
		}
		else
		{
			// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
			pAnimController->SetTrackWeight(0, 1.0f);       // ���݂̃A�j���[�V����
			pAnimController->SetTrackEnable(1, false);      // �O�̃A�j���[�V�����𖳌��ɂ���
			fCurWeightTime = 0.0f;
			bAnimBlend = false;
		}
	}

	// ���Ԃ��X�V
	if (bAnimation)
	{
		pAnimController->AdvanceTime(time, NULL);
	}
	else
	{
		pAnimController->AdvanceTime(0, NULL);
	}

	// �g���b�N���X�V
	pAnimController->GetTrackDesc(0, &TrackDesc[0]);
	pAnimController->GetTrackDesc(1, &TrackDesc[1]);

	// ���ԍX�V(4800�t���[����1�b�C60/4800 = 0.0125)
	iAnimFrame++;
}

//-----------------------------------------------------------------------------
// NAME: VOID BAnimController::ChangeAnimation(bool Smooth, int aniIndex)	
// DESC: �A�j���[�V�����R���g���[���[���� �A�j���[�V�����؂�ւ�
//-----------------------------------------------------------------------------
VOID BAnimController::ChangeAnimation(bool Smooth, int aniIndex)
{
	if (iAnimIndex != aniIndex)
	{
		int iNewTrack = 1;

		// �A�j���[�V�����C���f�b�N�X�ԍ�����
		if (aniIndex > iAnimMaxNum - 1)
		{
			aniIndex = 0;
		}
		else if (aniIndex < 0)
		{
			aniIndex = iAnimMaxNum - 1;
		}

		if (Smooth)
		{
			// <1.���݂̃g���b�N����>
			// ���݂̃g���b�N�����擾����
			D3DXTRACK_DESC TD;
			pAnimController->GetTrackDesc(iCurrentTrack, &TD);

			// ���݂̃A�j���[�V�������擾���A�����Ď擾�����A�j���[�V�������g���b�N1�Ɉڍs
			if (SetAnimationToTrack(iAnimIndex, iNewTrack))
			{
				// ���݂̃g���b�N�̐ݒ�l���g���b�N1�Ɉڍs
				pAnimController->SetTrackDesc(iNewTrack, &TD);
			}

			// <2.�V�����g���b�N����>
			// �V�����A�j���[�V�������擾���A�����Ď擾�����A�j���[�V�������g���b�N0�Ɉڍs
			if (SetAnimationToTrack(aniIndex, iCurrentTrack))
			{
				// �g���b�N0�̃|�W�V�����̐ݒ�
				pAnimController->SetTrackPosition(iCurrentTrack, 0.0f);
				// �g���b�N0�̃X�s�[�h�̐ݒ�
				pAnimController->SetTrackSpeed(iCurrentTrack, 1.0f);
			}

			// �g���b�N�̍���������(�L���̃g���b�N�������I�ɍ���)
			pAnimController->SetTrackEnable(iCurrentTrack, true);
			pAnimController->SetTrackEnable(iNewTrack, true);

			bAnimBlend = true;
		}
		else
		{
			// <1.�V�����g���b�N����>
			// �V�����A�j���[�V�������擾���A�����Ď擾�����A�j���[�V�������g���b�N0�Ɉڍs
			if (SetAnimationToTrack(aniIndex, iCurrentTrack))
			{
				// �g���b�N0�̃|�W�V�����̐ݒ�
				pAnimController->SetTrackPosition(iCurrentTrack, 0.0f);
				// �g���b�N0�̃E�F�C�g�̐ݒ�
				pAnimController->SetTrackWeight(iCurrentTrack, 1.0f);
				// �g���b�N0�̃X�s�[�h�̐ݒ�
				pAnimController->SetTrackSpeed(iCurrentTrack, 1.0f);
			}

			// �g���b�N�̍������֎~
			pAnimController->SetTrackEnable(iCurrentTrack, true);
			pAnimController->SetTrackEnable(iNewTrack, false);

			bAnimBlend = false;

		}
		// �E�F�C�g���Ԃ�������
		fCurWeightTime = 0.0f;

		// ���݂̃A�j���[�V�����ԍ���؂�ւ�
		iAnimIndex = aniIndex;

		// ���ԃ��Z�b�g
		pAnimController->ResetTime();
		iAnimFrame = 0;
	}
}

//-----------------------------------------------------------------------------
// NAME: DWORD BAnimController::GetAnimIndexByName(char sString[])
// DESC: �X�L�����b�V������ ���ŃA�j���[�V�����Z�b�g���擾����
//-----------------------------------------------------------------------------
DWORD BAnimController::GetAnimIndexByName(char sString[])
{
	HRESULT hr;
	LPD3DXANIMATIONSET pAS;
	DWORD dwRet = -1;

	for (DWORD i = 0; i < pAnimController->GetNumAnimationSets(); ++i)
	{
		hr = pAnimController->GetAnimationSet(i, &pAS);
		if (FAILED(hr))
			continue;

		if (pAS->GetName() &&
			!strncmp(pAS->GetName(), sString, min(strlen(pAS->GetName()), strlen(sString))))
		{
			dwRet = i;
			pAS->Release();
			break;
		}
		pAS->Release();
	}
	return dwRet;
}

//-----------------------------------------------------------------------------
// NAME: BOOL BAnimController::SetAnimationToTrack(int aniInx, int track)
// DESC: �X�L�����b�V������ �A�j���[�V�����g���b�N�Z�b�g����
//-----------------------------------------------------------------------------
BOOL BAnimController::SetAnimationToTrack(int aniInx, int track)
{
	HRESULT hr;

	// �A�j���[�V�����R���g���[���[����w��̃A�j���[�V�������擾����
	hr = pAnimController->GetAnimationSet(aniInx, &pAnimSet);
	if (FAILED(hr))
		return false;
	// �擾�����A�j���[�V�������w��̃g���b�N�Ɉڍs����
	hr = pAnimController->SetTrackAnimationSet(track, pAnimSet);
	if (FAILED(hr))
		return false;
	// �A�j���[�V�����Z�b�g���
	pAnimSet->Release();

	return true;
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