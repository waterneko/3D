
//=============================================================================//
// 
// Name : ScreenGauge.h
// Describe : �X�N���[���Q�[�W����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SCREENGAUGE_H_
#define _SCREENGAUGE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "ScreenTexture.h"			// �X�N���[���e�N�X�`������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_GAUGETEXTURE		(100)						// �ő�X�N���[���Q�[�W��

#define	PATH_GAUGEMASK	("Data/Texture/Other/Gauge/GauguMask.png")
#define	PATH_GAUGE		("Data/Texture/Other/Gauge/GauguTexture.png")

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }
// �X�N���[���Q�[�W�N���X
class GScreenGauge
{
private:
	LPDIRECT3DDEVICE9			pDevice; 				// �f�o�C�X�I�u�W�F�N�g

	int							iAlpha;					// �A���t�@�l

	float						fMaxValue;				// �ő�l
	float						fCurrentValue;			// ���O�l

	D3DXVECTOR2					vSize;					// �T�C�Y
	D3DXVECTOR2					vScreenPosition;		// �X�N���[�����W

	LPDIRECT3DTEXTURE9			pGaugeTexture;			// �Q�[�W�e�N�X�`��
	LPDIRECT3DTEXTURE9			pMaskTexture;			// ���̃e�N�X�`��

	BScreenTexture*				pMask;					// �Q�[�W�̃e�N�X�`���N���X
	BScreenTexture*				pGauge;					// ���̃e�N�X�`���N���X
public:
	bool						bUpdate;				// �X�V�t���O
	bool						bRender;				// �`��t���O

	bool						bGaugeMove;				// �Q�[�W�ړ��t���O
	float						fTargetValue;			// �ڕW�l
public:
	GScreenGauge(LPDIRECT3DDEVICE9 D3dDevice);			// �R���X�g���N�^
	~GScreenGauge();									// �f�X�g���N�^
public:
	HRESULT InitGaugeTexture(D3DXVECTOR2 size, D3DXVECTOR2 position, UINT type);	// �X�N���[���Q�[�W������
	VOID UpdateGaugeTexture(float percent);												// �X�N���[���Q�[�W�X�V
	VOID RenderGaugeTexture();														// �X�N���[���Q�[�W�`��

	VOID InitGauge(bool max, float maxvalue);										// �Q�[�W������
	VOID GaugeMove(float percent);													// �Q�[�W�ړ�

	VOID SetMaxValue(float value);													// �Q�[�W�ő�l�ݒ�
	VOID SetCurrentValue(float value);												// �Q�[�W���O�l�ݒ�
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif