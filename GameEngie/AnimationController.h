
//=============================================================================//
// 
// Name : AnimationController.h
// Describe : �A�j���[�V�����R���g���[���[����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _ANIMATIONCONTROLLER_H_
#define _ANIMATIONCONTROLLER_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "AnimXfile.h"				// �A�j���[�V����x�t�@�C������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define AMIMATION_FRAMETIME 	( 0.0125f )		// ���t���[���A�j���[�V��������( 60�t���[�� / AnimTicksPerSecond )
#define SMOOTH_FRAMETIME		( 0.1f )		// ���炩�ȃA�j���[�V��������
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }
// �A�j���[�V�����R���g���[���[�N���X
class BAnimController
{
private:
	LPDIRECT3DDEVICE9			pDevice;					// �f�o�C�X�I�u�W�F�N�g

	LPD3DXANIMATIONCONTROLLER	pAnimController;			// �A�j���[�V�����R���g���[���[
	LPD3DXANIMATIONSET	 		pAnimSet;					// �A�j���[�V�����Z�b�g
	D3DXTRACK_DESC  			TrackDesc[2];				// �A�j���[�V�����g���b�N���

	int							iCurrentTrack;				// ���̃g���b�N�ԍ�
	int							iAnimIndex;					// �A�j���[�V�����Z�b�g�ԍ�
	int							iAnimMaxNum;				// �A�j���[�V�����Z�b�g����
	int							iAnimFrame;					// �A�j���[�V�����t���[������

	float						fShiftTime;					// �V�t�g����̂ɂ����鎞��
	float						fCurWeightTime;				// �A�j���[�V�����������g�p��������
public:
	bool						bAnimBlend;					// �����t���O
	bool						bAnimation;					// �A�j���[�V�����t���O
public:
	BAnimController(LPDIRECT3DDEVICE9 D3dDevice);			// �R���X�g���N�^
	~BAnimController();										// �f�X�g���N�^
public:
	// [�A�j���[�V����]�ݒu
	HRESULT InitAnimController(SAnimXfile* pAnimxfile);		// �A�j���[�V�����R���g���[���[������
	VOID UpdateAnimController(FLOAT time);					// �A�j���[�V�����X�V
	VOID ChangeAnimation(bool Smooth, int aniIndex);		// �A�j���[�V�����؂�ւ�

	// [�擾�֐�]
	UINT GetAnimIndex() { return iAnimIndex; };									// ���̃A�j���[�V�����ԍ����擾����
	DWORD GetAnimIndexByName(char sString[]);									// ���ŃA�j���[�V�����ԍ����擾����

	int GetAnimFrame() { return iAnimFrame; }; 									// �A�j���[�V�����t���[�������擾����
	int GetCurrentTrack() { return iCurrentTrack; };							// ���̃g���b�N�ԍ����擾����
	DOUBLE GetAnimCurrentTime() { return pAnimController->GetTime(); };	// ���̃A�j���[�V�������Ԃ��擾����

	LPD3DXANIMATIONCONTROLLER GetAnimController() { return pAnimController; };	// �A�j���[�V�������擾����
	D3DXTRACK_DESC GetTrackInfo(int n) { return TrackDesc[n]; };				// �A�j���[�V�����g���b�N���擾����
private:
	BOOL SetAnimationToTrack(int aniInx, int track);							// �A�j���[�V�����g���b�N�Z�b�g����
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif