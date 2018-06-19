
//=============================================================================//
// 
// Name : Xfile.h
// Describe : x�t�@�C������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _XFILE_H_
#define _XFILE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_XFILE		(100)					// �ő�x�t�@�C����

#define	PATH_X_YUKARI		("Data/Xfile/yukari/")	// x�t�@�C���p�Xyukari
#define	PATH_X_CASTLE		("Data/Xfile/Castle/")	// x�t�@�C���p�XCastle
#define	PATH_X_ROOM			("Data/Xfile/Room/")	// x�t�@�C���p�XRoom
#define	PATH_X_SLOPE		("Data/Xfile/Slope/")	// x�t�@�C���p�XSlope
#define	PATH_X_CORRIDOR		("Data/Xfile/Corridor/")// x�t�@�C���p�XCorridor
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �V�F�[�f�B���O�^�C�v

// { �\���� }

// { �N���X }

// x�t�@�C���N���X
class SXfile
{
private:
	LPDIRECT3DDEVICE9		pDevice;						// �f�o�C�X�I�u�W�F�N�g
	LPD3DXMESH				pMesh;  						// ���b�V���ւ̃|�C���^�[
public:
	D3DMATERIAL9*			pMeshMaterials; 				// �}�e���A���ւ̃|�C���^�[
	LPDIRECT3DTEXTURE9*		pMeshTextures;  				// �e�N�X�`���ւ̃|�C���^�[
	DWORD					dwNumMaterials;					// �}�e���A���̐�
public:
	SXfile(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~SXfile();												// �f�X�g���N�^
public:
	HRESULT LoadXfile(char* path, char* name);				// x�t�@�C���ǂݍ���

	// [�擾�֐�]
	LPD3DXMESH GetMesh() { return pMesh; };					// ���b�V�����擾����
	DWORD GetMaterialsNum() { return dwNumMaterials; };		// �}�e���A�����擾����

};
//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif