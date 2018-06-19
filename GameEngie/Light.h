
//=============================================================================//
// 
// Name : Light.h
// Describe : ���C�g����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_LIGHT		(10)					// �ő僉�C�g��

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }

// ���C�g�N���X
class BLight
{
private:
	LPDIRECT3DDEVICE9		pDevice;			// �f�o�C�X�I�u�W�F�N�g

	int 					Id;					// ���C�g�̃C���f�b�N�X
	D3DLIGHT9				Light;				// D3D���C�g
public:
	BLight(LPDIRECT3DDEVICE9 D3dDevice, int id);		// �R���X�g���N�^
	~BLight();											// �f�X�g���N�^

	VOID EnableLight();																			// ���C�g�g�p��Ԑݒu
	VOID DisableLight();																		// ���C�g�֎~��Ԑݒu

	// [�������֐�]
	VOID InitDirectionalLight(D3DXVECTOR3, D3DXCOLOR, D3DXCOLOR, D3DXCOLOR); 					// �f�B���N�V���i���E���C�g������
	VOID InitPointLight(D3DXVECTOR3, D3DXCOLOR, D3DXCOLOR, D3DXCOLOR, float); 					// �|�C���g�E���C�g������
	VOID InitSpotLight(D3DXVECTOR3, D3DXVECTOR3, D3DXCOLOR, D3DXCOLOR, D3DXCOLOR, float); 		// �X�|�b�g�E���C�g������

	// [�ݒ�֐�]
	VOID SetAmbient(D3DXCOLOR a); 										// �A���r�G���g�ݒ�֐�
	VOID SetDiffuse(D3DXCOLOR d); 										// �f�B�t�F�[�Y�ݒ�֐�
	VOID SetSpecular(D3DXCOLOR s); 										// �X�y�L�����ݒ�֐�
	VOID SetPosition(D3DXVECTOR3 pos); 									// ���C�g��ԍ��W�ݒ�֐�
	VOID SetDirection(D3DXVECTOR3 dir); 								// ���C�g�����x�N�g���ݒ�֐�

	// [�擾�֐�]
	D3DXCOLOR GetAmbient() { return D3DXCOLOR(Light.Ambient); };		// �A���r�G���g�擾�֐�
	D3DXCOLOR GetDiffuse() { return D3DXCOLOR(Light.Diffuse); };		// �f�B�t�F�[�Y�擾�֐�
	D3DXCOLOR GetSpecular() { return D3DXCOLOR(Light.Specular); };		// �X�y�L�����擾�֐�
	D3DXVECTOR3 GetPosition() { return Light.Position; };				// ���C�g��ԍ��W�擾�֐�
	D3DXVECTOR3 GetDirection() { return Light.Direction; };				// ���C�g�����x�N�g���擾�֐�
};
//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//
#endif