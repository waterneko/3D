
//=============================================================================//
// 
// Name : FadeTexture.h
// Describe : �t�F�[�h�e�N�X�`������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _FADETEXTURE_H_
#define _FADETEXTURE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "ScreenTexture.h"			// �X�N���[���e�N�X�`������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_FADETEXTURE		(100)						// �ő�t�F�[�h�e�N�X�`����

#define	PATH_BLACKFADE	("Data/Texture/Other/Fade/black.png")	// ���e�N�X�`���t�@�C���p�X
#define	PATH_WHITEFADE	("Data/Texture/Other/Fade/white.png")	// ���e�N�X�`���t�@�C���p�X

#define	SPEED_FADE				(4)							// �t�F�[�h���x
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �t�F�[�h���
enum FadeState
{
	FS_Ready = 0,
	FS_FadeIn,
	FS_FadeOut
};
// { �\���� }

// { �N���X }
// �t�F�[�h�e�N�X�`���N���X
class GFadeTexture : public BScreenTexture
{
private:
	LPDIRECT3DDEVICE9			pDevice; 				// �f�o�C�X�I�u�W�F�N�g

	LPDIRECT3DTEXTURE9			pBlackTexture;			// ���e�N�X�`��
	LPDIRECT3DTEXTURE9			pWhiteTexture;			// ���e�N�X�`��
public:
	bool						bUpdate;				// �X�V�t���O
	bool						bRender;				// �`��t���O
	UINT						iFadeState;				// �t�F�[�h���
public:
	GFadeTexture(LPDIRECT3DDEVICE9 D3dDevice);			// �R���X�g���N�^
	~GFadeTexture();									// �f�X�g���N�^
public:
	HRESULT InitFadeTexture();							// �t�F�[�h�e�N�X�`��������
	VOID UpdateFadeTexture();							// �t�F�[�h�e�N�X�`���X�V
	VOID RenderFadeTexture(bool black);					// �t�F�[�h�e�N�X�`���`��
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif