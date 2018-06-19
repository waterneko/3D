
//=============================================================================//
// 
// Name : Light.cpp
// Describe : ���C�g����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Light.h"				// ���C�g����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BLight::BLight(LPDIRECT3DDEVICE9 D3dDevice, int id)
// DESC: ���C�g���� �R���X�g���N�^
//-----------------------------------------------------------------------------
BLight::BLight(LPDIRECT3DDEVICE9 D3dDevice, int id)
{
	pDevice = D3dDevice;

	Id = id;
	ZeroMemory(&Light, sizeof(Light));
}

//-----------------------------------------------------------------------------
// NAME: BLight::~BLight()
// DESC: ���C�g���� �f�X�g���N�^
//-----------------------------------------------------------------------------
BLight::~BLight()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::InitDirectionalLight(GameLight* Dlight, D3DXVECTOR3 vDir, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular)
// DESC: ���C�g���� �f�B���N�V���i���E���C�g������
//-----------------------------------------------------------------------------
VOID BLight::InitDirectionalLight(D3DXVECTOR3 vDir, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular)
{
	// �x�N�g���̐��K��
	// D3DXVec3Normalize(&vDir, &vDir);
	Light.Type = D3DLIGHT_DIRECTIONAL; 					// ���C�g�^�C�v
	Light.Ambient = ambient;							// �A���r�G���g
	Light.Diffuse = diffuse;							// �f�B�t�F�[�Y
	Light.Specular = specular;							// �X�y�L����
	Light.Direction = vDir; 							// ���C�g�����x�N�g��
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::InitPointLight(D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
// DESC: ���C�g���� �|�C���g�E���C�g������
//-----------------------------------------------------------------------------
VOID BLight::InitPointLight(D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
{

	Light.Type = D3DLIGHT_POINT;						// ���C�g�^�C�v
	Light.Ambient = ambient;							// �A���r�G���g
	Light.Diffuse = diffuse;							// �f�B�t�F�[�Y
	Light.Specular = specular;							// �X�y�L����
	Light.Attenuation0 = 1.0f;							// ���C�g�̌���
	Light.Attenuation1 = 0.0f;							// ���C�g�̌���
	Light.Attenuation2 = 0.0f;							// ���C�g�̌���
	Light.Range = fRange;								// ���C�g�̍ő�˒�
	Light.Position = vPos;								// ���C�g��ԍ��W
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::InitSpotLight(D3DXVECTOR3 vDir, D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
// DESC: ���C�g���� �X�|�b�g�E���C�g������
//-----------------------------------------------------------------------------
VOID BLight::InitSpotLight(D3DXVECTOR3 vDir, D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
{
	// �x�N�g���̐��K��
	// D3DXVec3Normalize(&vDir, &vDir);
	Light.Type = D3DLIGHT_SPOT;							// ���C�g�^�C�v
	Light.Ambient = ambient;							// �A���r�G���g
	Light.Diffuse = diffuse;							// �f�B�t�F�[�Y
	Light.Specular = specular;							// �X�y�L����
	Light.Attenuation0 = 1.0f;							// ���C�g�̌���
	Light.Attenuation1 = 0.0f;							// ���C�g�̌���
	Light.Attenuation2 = 0.0f;							// ���C�g�̌���
	Light.Range = fRange;								// ���C�g�̍ő�˒�
	Light.Falloff = 1.0f;
	Light.Phi = D3DX_PI / 3.0f;
	Light.Theta = D3DX_PI / 6.0f;
	Light.Direction = vDir; 							// ���C�g�����x�N�g��
	Light.Position = vPos;								// ���C�g��ԍ��W

}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::EnableLight()
// DESC: ���C�g���� ���C�g�g�p�֐�
//-----------------------------------------------------------------------------
VOID BLight::EnableLight()
{
	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(Id, &Light);
	// ���C�g�̂��g�p�g�p��ԂɎg�p
	pDevice->LightEnable(Id, true);
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::DisableLight()
// DESC: ���C�g���� ���C�g�֎~�֐�
//-----------------------------------------------------------------------------
VOID BLight::DisableLight()
{
	// ���C�g�̂��g�p�g�p��Ԃɋ֎~
	pDevice->LightEnable(Id, false);
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::SetDiffuse(D3DXCOLOR a)
// DESC: ���C�g���� �A���r�G���g�ݒ�֐�
//-----------------------------------------------------------------------------
VOID BLight::SetAmbient(D3DXCOLOR a)
{
	Light.Ambient = a;
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::SetDiffuse(D3DXCOLOR d)
// DESC: ���C�g����  �f�B�t�F�[�Y�ݒ�֐�
//-----------------------------------------------------------------------------
VOID BLight::SetDiffuse(D3DXCOLOR d)
{
	Light.Diffuse = d;
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::SetSpecular(D3DXCOLOR s)
// DESC: ���C�g���� �X�y�L�����ݒ�֐�
//-----------------------------------------------------------------------------
VOID BLight::SetSpecular(D3DXCOLOR s)
{
	Light.Specular = s;
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::EnableLight()
// DESC: ���C�g���� ���C�g��ԍ��W�ݒ�֐�
//-----------------------------------------------------------------------------
VOID BLight::SetPosition(D3DXVECTOR3 pos)
{
	if ((Light.Type == D3DLIGHT_POINT) || (Light.Type == D3DLIGHT_SPOT))
	{
		Light.Position = pos;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::EnableLight()
// DESC: ���C�g���� ���C�g�����x�N�g���ݒ�֐�
//-----------------------------------------------------------------------------
VOID BLight::SetDirection(D3DXVECTOR3 dir)
{
	if ((Light.Type == D3DLIGHT_DIRECTIONAL) || (Light.Type == D3DLIGHT_SPOT))
	{
		Light.Direction = dir;
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
