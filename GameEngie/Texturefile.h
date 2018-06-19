
//=============================================================================//
// 
// Name : Texturefile.h
// Describe : �e�N�X�`���t�@�C������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _TEXTUREFILE_H_
#define _TEXTUREFILE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_TEXTUREFILE	(100)					// �ő�e�N�X�`���t�@�C����

#define	PATH_TITLETEXTURE	("Data/Texture/Title/")		// �^�C�g���p�e�N�X�`���t�@�C���p�X
#define	PATH_STAGETEXTURE	("Data/Texture/Stage/")		// �X�e�[�W�p�e�N�X�`���t�@�C���p�X
#define	PATH_OTHERTEXTURE	("Data/Texture/Other/")		// ���̑��p�e�N�X�`���t�@�C���p�X
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }

// �e�N�X�`���N���X
class STexturefile
{
private:
	LPDIRECT3DDEVICE9		pDevice;						// �f�o�C�X�I�u�W�F�N�g
	LPDIRECT3DTEXTURE9		pTexture;  						// �e�N�X�`���ւ̃|�C���^�[
	D3DXVECTOR2				vSize;							// �e�N�X�`���̕��ƍ���
public:
	STexturefile(LPDIRECT3DDEVICE9 D3dDevice);				// �R���X�g���N�^
	~STexturefile();										// �f�X�g���N�^
public:
	HRESULT LoadTexturefile(char* path, char* name);		// �e�N�X�`����Ǎ���

	// [�擾�֐�]
	LPDIRECT3DTEXTURE9 GetTexture() { return pTexture; };	// �e�N�X�`�����擾����
	D3DXVECTOR2 GetSize() { return vSize; };				// �T�C�Y���擾����
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif