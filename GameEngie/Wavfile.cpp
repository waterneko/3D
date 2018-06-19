
//=============================================================================//
// 
// Name : Wavfile.cpp
// Describe : Wav�t�@�C������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/06
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Wavfile.h"				// Wav�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SWavfile::SWavfile()
// DESC: Wav�t�@�C������ �R���X�g���N�^
//-----------------------------------------------------------------------------
SWavfile::SWavfile()
{
	byDataAudio = NULL;
}

//-----------------------------------------------------------------------------
// NAME: SWavfile::~SWavfile()
// DESC: Wav�t�@�C������ �f�X�g���N�^
//-----------------------------------------------------------------------------
SWavfile::~SWavfile()
{
	SAFE_DELETE(byDataAudio)
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SWavfile::LoadWavFile(char* path, char* name)
// DESC: Wav�t�@�C������ �f�X�g���N�^
//-----------------------------------------------------------------------------
HRESULT SWavfile::LoadWavFile(char* path, char* name)
{
	HRESULT hr;
	HANDLE	hFile;
	DWORD	dwChunkSize;
	DWORD	dwChunkPosition;
	DWORD	dwFiletype;

	// �t�@�C���̃p�X���󏈗�
	char FilePath[255];
	strcpy_s(FilePath, path);
	strcat_s(FilePath, name);

	// �o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));

	// �T�E���h�f�[�^�t�@�C���̐���
	hFile = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// �t�@�C���|�C���^��擪�Ɉړ�
		MessageBox(NULL, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// WAVE�t�@�C���̃`�F�b�N
	hr = FindChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}

	// �t�H�[�}�b�g�`�F�b�N
	hr = FindChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = FindChunk(hFile, 'atad', &dwSizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	byDataAudio = (BYTE*)malloc(dwSizeAudio);
	hr = ReadChunkData(hFile, byDataAudio, dwSizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT FindChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
// DESC: Wav�t�@�C������ �`�����N����T��
//-----------------------------------------------------------------------------
HRESULT SWavfile::FindChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SWavfile::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
// DESC: Wav�t�@�C������ �`�����N����T��
//-----------------------------------------------------------------------------
HRESULT SWavfile::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
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