
//=============================================================================//
// 
// Name : ScreenTexture.h
// Describe : �X�N���[���e�N�X�`��(���_�o�b�t�@)����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SCREENTEXTURE_H_
#define _SCREENTEXTURE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_SCREENTEXTURE		(100)					// �ő啽�ʐ�

#define	FVF_VERTEX_SCREENTEXTURE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)  // UIFVF

// UI�p
#define	NUM_VERTEX_SCREENTEXTURE		(4)		// ���_��
#define	NUM_POLYGON_SCREENTEXTURE		(2)		// �|���S����
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }
// �X�N���[�����_FVF�\����
struct ScreenVertex
{
	D3DXVECTOR3 vtx;	// ���_���W
	float		rhw;	// �X�N���[�����W�t���O
	D3DCOLOR	diffuse;// �f�B�t�F�[�Y
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
};

// { �N���X }
// �X�N���[�����_�N���X
class BScreenTexture
{
protected:
	LPDIRECT3DDEVICE9			pDevice; 								// �f�o�C�X�I�u�W�F�N�g

	int							iAlpha;									// �A���t�@�l

	D3DXVECTOR2					vSize;									// �T�C�Y
	D3DXVECTOR2					vScreenPosition;						// �X�N���[�����W

	ScreenVertex				ScreenVtx[NUM_VERTEX_SCREENTEXTURE]; 	// �X�N���[�����_FVF�\����
public:
	BScreenTexture(LPDIRECT3DDEVICE9 D3dDevice);			// �R���X�g���N�^
	~BScreenTexture();										// �f�X�g���N�^
public:
	HRESULT InitScreenTexture(D3DXVECTOR2 size, D3DXVECTOR2 position);	// �X�N���[�����_������
	VOID UpdateScreenTexture();											// �X�N���[�����_�X�V
	VOID RenderScreenTexture(LPDIRECT3DTEXTURE9 texture);				// �X�N���[�����_�`��

	VOID UpdateAlpha();									// �����x�X�V

	// [�ݒu�֐�]
	VOID SetCenterPosition(D3DXVECTOR2 pos);			// ���S���W�ݒu
	VOID SetDiffuse(D3DCOLOR d);						// ���_�f�B�t�F�[�Y�ݒu
	VOID SetAlpha(int alpha);							// ���_�����x�ݒu

	VOID SetVertexPosition(int n, D3DXVECTOR2 pos);		// ���蒸�_���W�ݒu
	VOID SetVertexTextureUV(int n, D3DXVECTOR2 pos);	// ���蒸�_�e�N�X�`��UV���W�ݒu
	VOID SetVertexDiffuse(int n, D3DCOLOR d);			// ���蒸�_���_�f�B�t�F�[�Y�ݒu
	VOID SetVertexAlpha(int n,int alpha);				// ���蒸�_�����x�ݒu

	VOID MoveByVector(D3DXVECTOR2 move);				// ���_�ړ�
	VOID MoveAlpha(int movealpha);						// ���_�����x���₷/����

	VOID MoveVertexByVector(int n, D3DXVECTOR2 pos);	// ���蒸�_���W�ݒu
	VOID MoveVertexTextureUV(int n, D3DXVECTOR2 pos);	// ���蒸�_�e�N�X�`��UV���W�ݒu

	// [�擾�֐�]
	D3DXVECTOR2 GetPosition() { return vScreenPosition; };			// �X�N���[�����W���擾����
	D3DXVECTOR2 GetSize() { return vSize; };						// �T�C�Y���擾����
	int GetAlphaValue() { return iAlpha; };							// �S�̓����x���擾����

	D3DXVECTOR2 GetVertexPosition(int n) { return D3DXVECTOR2(ScreenVtx[n].vtx.x, ScreenVtx[n].vtx.y); };// ���蒸�_�e�N�X�`�����W���擾����
	D3DXVECTOR2 GetVertexTextureUV(int n) { return ScreenVtx[n].tex; };				// ���蒸�_�e�N�X�`��UV���W���擾����
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif