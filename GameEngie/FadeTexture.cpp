
//=============================================================================//
// 
// Name : FadeTexture.cpp
// Describe : �t�F�[�h�e�N�X�`������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "FadeTexture.h"				// �t�F�[�h�e�N�X�`������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �t�F�[�h�e�N�X�`������ �R���X�g���N�^
//-----------------------------------------------------------------------------
GFadeTexture::GFadeTexture(LPDIRECT3DDEVICE9 D3dDevice) : BScreenTexture(D3dDevice)
{
	pDevice = D3dDevice;

	bUpdate = true;
	bRender = true;

	iFadeState = FS_Ready;

	vSize = D3DXVECTOR2(0.0f, 0.0f);
	vScreenPosition = D3DXVECTOR2(0.0f, 0.0f);

	pBlackTexture = NULL;
	pWhiteTexture = NULL;

	ScreenVtx[NUM_VERTEX_SCREENTEXTURE];
}

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::~BScreenTexture()
// DESC: �t�F�[�h�e�N�X�`������ �f�X�g���N�^
//-----------------------------------------------------------------------------
GFadeTexture::~GFadeTexture()
{
	SAFE_RELEASE(pBlackTexture)
	SAFE_RELEASE(pWhiteTexture)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GFadeTexture::InitFadeTexture()
// DESC: �t�F�[�h�e�N�X�`������ �t�F�[�h�e�N�X�`��������
//-----------------------------------------------------------------------------
HRESULT GFadeTexture::InitFadeTexture()
{
	D3DXCreateTextureFromFile(pDevice, PATH_BLACKFADE, &pBlackTexture);
	D3DXCreateTextureFromFile(pDevice, PATH_WHITEFADE, &pWhiteTexture);

	InitScreenTexture(D3DXVECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT), D3DXVECTOR2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GFadeTexture::UpdateFadeTexture()
// DESC: �t�F�[�h�e�N�X�`������ �t�F�[�h�e�N�X�`���X�V
//-----------------------------------------------------------------------------
VOID GFadeTexture::UpdateFadeTexture()
{
	if (bUpdate)
	{
		// �t�F�[�h����
		if (iFadeState == FS_FadeIn)
		{
			if (iAlpha < 255)
			{
				iAlpha += SPEED_FADE;
			}
		}
		else if (iFadeState == FS_FadeOut)
		{
			if (iAlpha > 0)
			{
				iAlpha -= SPEED_FADE;
			}
		}

		// �A���t�@�l�X�V
		UpdateAlpha();
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GFadeTexture::RenderFadeTexture(bool black)
// DESC: �t�F�[�h�e�N�X�`������ �t�F�[�h�e�N�X�`���`��
//-----------------------------------------------------------------------------
VOID GFadeTexture::RenderFadeTexture(bool black)
{
	if (bRender)
	{
		if (black)
		{
			RenderScreenTexture(pBlackTexture);
		}
		else
		{
			RenderScreenTexture(pWhiteTexture);
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