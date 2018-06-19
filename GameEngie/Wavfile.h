
//=============================================================================//
// 
// Name : Wavfile.h
// Describe : Wav�t�@�C������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _WAVFILE_H_
#define _WAVFILE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_WAVFILE			(50)						// �ő�Wav�t�@�C����
#define	PATH_BGM_WAV			("Data/Sound/BGM/")			// WAV�t�@�C���p�XBGM
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }
// wav�t�@�C���N���X
class SWavfile
{
public:
	WAVEFORMATEXTENSIBLE	wfx;							// �t�H�[�}�b�g
	BYTE*					byDataAudio;					// �I�[�f�B�I�f�[�^
	DWORD					dwSizeAudio;					// �I�[�f�B�I�f�[�^�T�C�Y
public:
	SWavfile();												// �R���X�g���N�^
	~SWavfile();											// �f�X�g���N�^
public:
	HRESULT LoadWavFile(char* path, char* name);
private:
	HRESULT FindChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);		// �`�����N����T��
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);		// �`�����N�f�[�^�̓Ǎ���
};
//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif