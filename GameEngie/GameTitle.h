
//=============================================================================//
// 
// Name : GameTitle.h
// Describe : �Q�[���^�C�g������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMETITLE_H_
#define _GAMETITLE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������
#include "Input.h"					// ���͏���

#include "SourceManager.h"			// �\�[�X�Ǘ�

#include "Camera.h"					// �J��������
#include "Light.h"					// ���C�g����

#include "Mesh.h"					// ���b�V������
#include "SkyDome.h"				// �X�J�C�h�[������

#include "GameController.h"			// �Q�[���R���g���[���[����
#include "GameTexture.h"			// �Q�[���e�N�X�`������
#include "ScreenGauge.h"			// �X�N���[���Q�[�W����
#include "FadeTexture.h"			// �t�F�[�h�e�N�X�`������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define FIRST_GAMESTAGE		(1)		// �ŏ��̃X�e�[�W

#define	TITLE_START_CAMERAROT			(D3DXVECTOR3(D3DXToRadian(-20),D3DXToRadian(-25) ,0.0f))	// �Q�[���X�^�[�g��ʃJ�����p�x
#define	TITLE_START_AMBIENT				(D3DCOLOR_RGBA(12,10,40,255))			// �Q�[���X�^�[�g��ʊ���
#define	TITLE_START_LIGHTAMBIENT		(D3DCOLOR_RGBA(68,68,68,255))			// �Q�[���X�^�[�g��ʃ��C�g�A���r�G���g
#define	TITLE_START_LIGHTDIFFUSE		(D3DCOLOR_RGBA(210,95,150,255))			// �Q�[���X�^�[�g��ʃ��C�g�f�B�t�F�[�Y
#define	TITLE_START_LIGHTSPECULAR		(D3DCOLOR_RGBA(12,8,60,255))			// �Q�[���X�^�[�g��ʃ��C�g�X�y�L����
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �^�C�g�����
enum TitleState
{
	TS_Logo = 0,	// �Q�[�����S
	TS_Start,		// �Q�[���X�^�[�g
	TS_Menu,		// �Q�[�����j���[
};
// { �\���� }

// { �N���X }

// �Q�[���^�C�g�������N���X
class TitleInstance
{
private:
	LPDIRECT3DDEVICE9		pDevice;							// �f�o�C�X�I�u�W�F�N�g

	UINT					iCurrentState;						// ���݂̃^�C�g�����
	UINT					iTargetState;						// ���̃^�C�g�����

	UINT					iSelectStage;						// �I�������X�e�[�W

	SourceManager*			pSourceManager;						// �\�[�X�Ǘ�

	BThirdPersonCamera*		pTPCamera;							// �J�����N���X�ւ̃|�C���^�[
	BLight*					pLight;								// ���C�g�N���X�ւ̃|�C���^�[

	BMesh*					pMesh[MAXNUM_MESH];					// ���b�V���N���X�ւ̃|�C���^�[
	GSkyDome*				pGSkyDome;							// �X�J�C�h�[���N���X�ւ̃|�C���^�[
	GTexture*				pGTexture[MAXNUM_GAMETEXTURE];		// �Q�[���e�N�X�`���N���X�ւ̃|�C���^�[
	GController*			pGController;						// �Q�[���R���g���[���[�N���X�ւ̃|�C���^�[
	GFadeTexture*			pFade;								// �t�F�[�h�e�N�X�`������(��)
public:
	TitleInstance(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~TitleInstance();											// �f�X�g���N�^
public:
	HRESULT	LoadTitle();										// �Q�[���^�C�g���̓Ǎ���
	VOID UninitTitle();								// �Q�[���^�C�g���̉��

	VOID UpdateController();									// �Q�[���^�C�g���R���g���[���[�̑��X�V����
	VOID UpdateTitle();											// �Q�[���^�C�g���̑��X�V����
	VOID RenderTitle();											// �Q�[���^�C�g���̑��`�揈��

	// [�擾�֐�]
	UINT GetCurrentState() { return iCurrentState; };			// ���݂̃^�C�g����Ԃ��擾����
	UINT GetTargetState() { return iTargetState; };				// ���̃^�C�g����Ԃ��擾����
	UINT GetSelectStage() { return iSelectStage; };				// �I�������X�e�[�W���擾����
	GFadeTexture* GetFade() { return pFade; };					// �t�F�[�h�|�C���^�[���擾����
private:
	HRESULT LoadSource();										// �Q�[���^�C�g���\�[�X�̓Ǎ���
	HRESULT	InitObject();										// �Q�[���^�C�g���Ώۂ̑�����������

	VOID UpdateLogoController();								// �Q�[�����S�R���g���[���[�̍X�V����
	VOID UpdateStartController();								// �Q�[���X�^�[�g�R���g���[���[�̍X�V����
	VOID UpdateMenuController();								// �Q�[�����j���[�R���g���[���[�̍X�V����

	VOID UpdateFade();											// �^�C�g����Ԉڍs�̃t�F�[�h��������
	VOID UpdateLogoObject();									// �Q�[�����S�Ώۂ̍X�V����
	VOID UpdateStartObject();									// �Q�[���X�^�[�g�Ώۂ̍X�V����
	VOID UpdateMenuObject();									// �Q�[�����j���[�Ώۂ̍X�V����

	VOID RenderLogo();											// �Q�[�����S�̕`�揈��
	VOID RenderStart();											// �Q�[���X�^�[�g�̕`�揈��
	VOID RenderMenu();											// �Q�[�����j���[�̕`�揈��

	// ��Ԉڍs����
	VOID ChangeTitleState(int state);							// �Q�[���^�C�g����Ԃ̕ύX

	// [�ݒu�֐�]
	LPDIRECT3DTEXTURE9 SetTexture(UINT n);						// �e�N�X�`���ݒu
	SXfile* SetXfile(UINT n);									// x�t�@�C���ݒu
	SAnimXfile* SetAnimXfile(UINT n);							// �A�j���[�V����x�t�@�C���ݒu
	SWavfile* SetWavfile(UINT n);								// Wav�t�@�C���ݒu
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif