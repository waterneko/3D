
//=============================================================================//
// 
// Name : ScreenTexture.cpp
// Describe : �X�N���[���e�N�X�`��(���_�o�b�t�@)����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "ScreenTexture.h"				// �X�N���[���e�N�X�`������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �X�N���[���e�N�X�`������ �R���X�g���N�^
//-----------------------------------------------------------------------------
BScreenTexture::BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iAlpha = 255;
	vSize = D3DXVECTOR2(0.0f, 0.0f);
	vScreenPosition = D3DXVECTOR2(0.0f, 0.0f);

	ScreenVtx[NUM_VERTEX_SCREENTEXTURE];
}

//-----------------------------------------------------------------------------
// NAME: BScreenTexture::~BScreenTexture()
// DESC: �X�N���[���e�N�X�`������ �f�X�g���N�^
//-----------------------------------------------------------------------------
BScreenTexture::~BScreenTexture()
{

}

//-----------------------------------------------------------------------------
// NAME: HRESULT BScreenTexture::InitScreenTexture(D3DXVECTOR2 size, D3DXVECTOR2 position)
// DESC: �X�N���[���e�N�X�`������ UI������
//-----------------------------------------------------------------------------
HRESULT BScreenTexture::InitScreenTexture(D3DXVECTOR2 size, D3DXVECTOR2 position)
{
	vSize = size;
	vScreenPosition = position;
	iAlpha = 255;

	// ���S���W�ݒu
	SetCenterPosition(vScreenPosition);

	// ���ˌ��̐ݒ�
	SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, iAlpha));

	ScreenVtx[0].rhw = 1.0f;
	ScreenVtx[1].rhw = 1.0f;
	ScreenVtx[2].rhw = 1.0f;
	ScreenVtx[3].rhw = 1.0f;
	// �e�N�X�`�����W�̐ݒ�
	ScreenVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ScreenVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ScreenVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ScreenVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::UpdateScreenTexture()
// DESC: �X�N���[���e�N�X�`������ �X�N���[���e�N�X�`���X�V
//-----------------------------------------------------------------------------
VOID BScreenTexture::UpdateScreenTexture()
{
	// �����x�X�V
	UpdateAlpha();
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::RenderScreenTexture(LPDIRECT3DTEXTURE9 texture)
// DESC: �X�N���[���e�N�X�`������ �X�N���[���e�N�X�`���`��
//-----------------------------------------------------------------------------
VOID BScreenTexture::RenderScreenTexture(LPDIRECT3DTEXTURE9 texture)
{
	// ���_FVF�ŕ`�悷��
	pDevice->SetFVF(FVF_VERTEX_SCREENTEXTURE);
	// �e�N�X�`����ݒ肷��
	pDevice->SetTexture(0, texture);
	// UI���^�����O
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON_SCREENTEXTURE, ScreenVtx, sizeof(ScreenVertex));
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::UpdateAlpha()
// DESC: �X�N���[���e�N�X�`������ �����x�X�V
//-----------------------------------------------------------------------------
VOID BScreenTexture::UpdateAlpha()
{
	static int iCurrentAlpha = 255;

	if (iCurrentAlpha != iAlpha)
	{
		if (iAlpha > 255)
		{
			SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, 255));
			iAlpha = 255;
		}
		else if (iAlpha < 0)
		{
			SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, 0));
			iAlpha = 0;
		}
		else
		{
			SetDiffuse(D3DCOLOR_RGBA(255, 255, 255, iAlpha));
		}

		// ���݂̃A���t�@�l�X�V
		iCurrentAlpha = iAlpha;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetCenterPosition(D3DXVECTOR2 pos)
// DESC: �X�N���[���e�N�X�`������ ���W�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetCenterPosition(D3DXVECTOR2 pos)
{
	vScreenPosition = pos;
	// ���_���W�̐ݒ�
	ScreenVtx[0].vtx = D3DXVECTOR3(-vSize.x / 2, -vSize.y / 2, 0.0f);
	ScreenVtx[1].vtx = D3DXVECTOR3(vSize.x / 2, -vSize.y / 2, 0.0f);
	ScreenVtx[2].vtx = D3DXVECTOR3(-vSize.x / 2, vSize.y / 2, 0.0f);
	ScreenVtx[3].vtx = D3DXVECTOR3(vSize.x / 2, vSize.y / 2, 0.0f);

	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].vtx += D3DXVECTOR3(vScreenPosition.x, vScreenPosition.y, 0.0f);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetDiffuse()
// DESC: �X�N���[���e�N�X�`������ ���_�f�B�t�F�[�Y�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetDiffuse(D3DCOLOR d)
{
	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].diffuse = d;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetAlpha(int alpha)
// DESC: �X�N���[���e�N�X�`������ ���_�����x�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetAlpha(int alpha)
{
	iAlpha = alpha;
	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, iAlpha);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetVertexPosition(int n, D3DXVECTOR2 pos)
// DESC: �X�N���[���e�N�X�`������ ���蒸�_���W�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetVertexPosition(int n, D3DXVECTOR2 pos)
{
	ScreenVtx[n].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetVertexTextureUV(int n, D3DXVECTOR2 pos)
// DESC: �X�N���[���e�N�X�`������ ���蒸�_�e�N�X�`��UV���W�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetVertexTextureUV(int n, D3DXVECTOR2 pos)
{
	ScreenVtx[n].tex = pos;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::SetVertexDiffuse(int n, D3DXCOLOR d)
// DESC: �X�N���[���e�N�X�`������ ���蒸�_���_�f�B�t�F�[�Y�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::SetVertexDiffuse(int n, D3DCOLOR d)
{
	ScreenVtx[n].diffuse = d;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveByVector(D3DXVECTOR2 move)
// DESC: �X�N���[���e�N�X�`������ ���_�ړ�
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveByVector(D3DXVECTOR2 move)
{
	for (int i = 0; i < 4; i++)
	{
		ScreenVtx[i].vtx += D3DXVECTOR3(move.x, move.y, 0.0f);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveAlpha(int movealpha)
// DESC: �X�N���[���e�N�X�`������ ���_�����x���₷/����
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveAlpha(int movealpha)
{
	iAlpha += movealpha;
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveVertexByVector(int n, D3DXVECTOR2 move)
// DESC: �X�N���[���e�N�X�`������ ���蒸�_���W�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveVertexByVector(int n, D3DXVECTOR2 move)
{
	ScreenVtx[n].vtx += D3DXVECTOR3(move.x, move.y, 0.0f);
}

//-----------------------------------------------------------------------------
// NAME: VOID BScreenTexture::MoveVertexTextureUV(int n, D3DXVECTOR2 move)
// DESC: �X�N���[���e�N�X�`������ ���蒸�_�e�N�X�`��UV���W�ݒu
//-----------------------------------------------------------------------------
VOID BScreenTexture::MoveVertexTextureUV(int n, D3DXVECTOR2 move)
{
	ScreenVtx[n].tex += move;
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