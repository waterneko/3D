
//=============================================================================//
// 
// Name : ScreenGauge.cpp
// Describe : �X�N���[���Q�[�W����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "ScreenGauge.h"				// �X�N���[���Q�[�W����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: GScreenGauge::GScreenGauge(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �X�N���[���Q�[�W���� �R���X�g���N�^
//-----------------------------------------------------------------------------
GScreenGauge::GScreenGauge(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	bUpdate = true;
	bRender = true;
	bGaugeMove = true;

	fMaxValue = 0.0f;
	fCurrentValue = 0.0f;
	fTargetValue = 0.0f;

	iAlpha = 255;
	vSize = D3DXVECTOR2(0.0f, 0.0f);
	vScreenPosition = D3DXVECTOR2(0.0f, 0.0f);

	pGaugeTexture = NULL;
	pMaskTexture = NULL;
	// {�X�N���[���e�N�X�`��}
	pMask = new BScreenTexture(pDevice);
	pGauge = new BScreenTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: GScreenGauge::~GScreenGauge()
// DESC: �X�N���[���Q�[�W���� �f�X�g���N�^
//-----------------------------------------------------------------------------
GScreenGauge::~GScreenGauge()
{
	// {�X�N���[���e�N�X�`���̉��}
	SAFE_DELETE(pMask)
	SAFE_DELETE(pGauge)

	SAFE_RELEASE(pGaugeTexture)
	SAFE_RELEASE(pMaskTexture)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GScreenGauge::InitGaugeTexture(D3DXVECTOR2 size, D3DXVECTOR2 position, UINT type)
// DESC: �X�N���[���Q�[�W���� �X�N���[���Q�[�W������
//-----------------------------------------------------------------------------
HRESULT GScreenGauge::InitGaugeTexture(D3DXVECTOR2 size, D3DXVECTOR2 position, UINT type)
{
	switch (type)
	{
	case 0:
		D3DXCreateTextureFromFile(pDevice, PATH_GAUGE, &pGaugeTexture);
		D3DXCreateTextureFromFile(pDevice, PATH_GAUGEMASK, &pMaskTexture);
		break;
	case 1:
		break;
	}

	vSize = size;
	vScreenPosition = position;

	// {�X�N���[���e�N�X�`��������}
	pMask->InitScreenTexture(vSize, vScreenPosition);
	pGauge->InitScreenTexture(vSize, vScreenPosition);

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::UpdateGaugeTexture(float percent)
// DESC: �X�N���[���Q�[�W���� �X�N���[���Q�[�W�X�V
//-----------------------------------------------------------------------------
VOID GScreenGauge::UpdateGaugeTexture(float percent)
{
	if (bUpdate)
	{
		// �A���t�@�l�X�V
		pMask->SetAlpha(iAlpha);
		pGauge->SetAlpha(iAlpha);

		pMask->UpdateAlpha();
		pGauge->UpdateAlpha();

		// �ڕW�l�`�F�b�N
		if (fTargetValue < 0.0f)
		{
			fTargetValue = 0.0f;
		}
		else if (fTargetValue > fMaxValue)
		{
			fTargetValue = fMaxValue;
		}

		// �Q�[�W�ړ�
		if (bGaugeMove)
		{
			// �Q�[�W����
			if (fCurrentValue < fTargetValue)
			{
				if (fCurrentValue + fMaxValue * (percent / 100.0f) <= fTargetValue)
				{
					GaugeMove(percent);
				}
			}
			// �Q�[�W����
			else if (fCurrentValue > fTargetValue)
			{
				if (fCurrentValue - fMaxValue * (percent / 100.0f) >= fTargetValue)
				{
					GaugeMove(-percent);
				}
			}

		}
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::RenderGaugeTexture()
// DESC: �X�N���[���Q�[�W���� �X�N���[���Q�[�W�`��
//-----------------------------------------------------------------------------
VOID GScreenGauge::RenderGaugeTexture()
{
	if (bRender)
	{
		pMask->RenderScreenTexture(pMaskTexture);
		pGauge->RenderScreenTexture(pGaugeTexture);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::InitGauge(bool max, float maxvalue)
// DESC: �X�N���[���Q�[�W���� �Q�[�W������
//-----------------------------------------------------------------------------
VOID GScreenGauge::InitGauge(bool max, float maxvalue)
{
	// �Q�[�W�ő�l�ݒ�
	SetMaxValue(maxvalue);

	if (!max)
	{
		// �Q�[�W�l�[������
		fCurrentValue = 0.0f;
		fTargetValue = 0.0f;

		// ���_�ړ�(�Q�[�W�[��)
		pGauge->SetVertexPosition(1, D3DXVECTOR2(vScreenPosition.x - vSize.x / 2.0f, vScreenPosition.y - vSize.y / 2.0f));
		pGauge->SetVertexPosition(3, D3DXVECTOR2(vScreenPosition.x - vSize.x / 2.0f, vScreenPosition.y + vSize.y / 2.0f));

		// UV���W�ړ�(�Q�[�W�[��)
		pGauge->SetVertexTextureUV(1, D3DXVECTOR2(0.0f, 0.0f));
		pGauge->SetVertexTextureUV(3, D3DXVECTOR2(0.0f, 1.0f));
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::SetMaxValue(float max)
// DESC: �X�N���[���Q�[�W���� �Q�[�W�ő�l�ݒ�
//-----------------------------------------------------------------------------
VOID GScreenGauge::SetMaxValue(float max)
{
	if (max > 0)
	{
		fMaxValue = max;
		fCurrentValue = fMaxValue;
		fTargetValue = fMaxValue;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::SetMaxValue(float max)
// DESC: �X�N���[���Q�[�W���� �Q�[�W���O�l�ݒ�
//-----------------------------------------------------------------------------
VOID GScreenGauge::SetCurrentValue(float value)
{
	if ( (value <= fMaxValue) && (value >= 0.0f) )
	{
		fCurrentValue = value;
	}
	else if (value < 0.0f)
	{
		fCurrentValue = 0.0f;
	}
	else
	{
		fCurrentValue = fMaxValue;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID GScreenGauge::SetMaxValue(float percent)
// DESC: �X�N���[���Q�[�W���� �Q�[�W�ړ�
//-----------------------------------------------------------------------------
VOID GScreenGauge::GaugeMove(float percent)
{
	float fPercent;
	fPercent = (fCurrentValue / fMaxValue);

	if ((fPercent >= 0.0f) && (fPercent <= 1.0f))
	{
		// ���_�ړ�
		pGauge->MoveVertexByVector(1, D3DXVECTOR2(vSize.x / 100.0f * percent, 0.0f));
		pGauge->MoveVertexByVector(3, D3DXVECTOR2(vSize.x / 100.0f * percent, 0.0f));

		// UV���W�ړ�
		pGauge->MoveVertexTextureUV(1, D3DXVECTOR2(1.0f / 100.0f * percent, 0.0f));
		pGauge->MoveVertexTextureUV(3, D3DXVECTOR2(1.0f / 100.0f * percent, 0.0f));

		// ���O�l�Čv�Z
		fCurrentValue += fMaxValue * (percent / 100.0f);
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