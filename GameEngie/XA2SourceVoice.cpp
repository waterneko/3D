
//=============================================================================//
// 
// Name : XA2SourceVoice.cpp
// Describe : XAudio2�\�[�X�{�C�X����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "XA2SourceVoice.h"				// XAudio2�\�[�X�{�C�X����

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BXA2SourceVoice::BXA2SourceVoice()
// DESC: XAudio2�\�[�X�{�C�X���� �R���X�g���N�^
//-----------------------------------------------------------------------------
BXA2SourceVoice::BXA2SourceVoice()
{
	bLoop = true;

	pSourceVoice = NULL;
}

//-----------------------------------------------------------------------------
// NAME: BXA2SourceVoice::~BXA2SourceVoice()
// DESC: XAudio2�\�[�X�{�C�X���� �f�X�g���N�^
//-----------------------------------------------------------------------------
BXA2SourceVoice::~BXA2SourceVoice()
{
	if (pSourceVoice)
	{
		// �ꎞ��~
		pSourceVoice->Stop(0);

		// �\�[�X�{�C�X�̔j��
		pSourceVoice->DestroyVoice();
		pSourceVoice = NULL;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT InitSourceVoice(SWavfile* wavfile, bool loop)
// DESC: XAudio2�\�[�X�{�C�X���� �\�[�X�{�C�X�̏�����
//-----------------------------------------------------------------------------
HRESULT BXA2SourceVoice::InitSourceVoice(SWavfile* wavfile, bool loop)
{
	HRESULT hr;

	if (GetMasterVoice() != NULL)
	{
		bLoop = loop;

		// �\�[�X�{�C�X�̐���
		hr = GetXaudio2()->CreateSourceVoice(&pSourceVoice, &(wavfile->wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(NULL, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		memset(&XA2buffer, 0, sizeof(XAUDIO2_BUFFER));
		XA2buffer.Flags = XAUDIO2_END_OF_STREAM;			// �Đ��I������玟�̃o�b�t�@�Đ����Ȃ�
		XA2buffer.AudioBytes = wavfile->dwSizeAudio;		// �I�[�f�B�I �f�[�^�̃T�C�Y
		XA2buffer.pAudioData = wavfile->byDataAudio;		// �I�[�f�B�I �f�[�^�ւ̃|�C���^�[
		XA2buffer.PlayBegin = 0;							// �Đ��n���ʒu
		XA2buffer.PlayLength = 0;							// �Đ������̈�̒����@0 = �S���Đ�
		XA2buffer.LoopBegin = 0;							// ���[�v�n���ʒu LoopBegin < PlayBegin + PlayLength
		XA2buffer.LoopLength = 0;							// ���[�v�̈�̒��� PlayBegin < LoopBegin + LoopLength < PlayBegin + PlayLength

		// ���[�v�� 0 = ���[�v���Ȃ� �CXAUDIO2_LOOP_INFINITE = �������[�v
		if (bLoop)
		{
			XA2buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else
		{
			XA2buffer.LoopCount = 0;
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		pSourceVoice->SubmitSourceBuffer(&XA2buffer);
	}
	else
	{
		MessageBox(NULL, "�}�X�^�[�{�C�X�������I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::PlaySourceVoice()
// DESC: XAudio2�\�[�X�{�C�X���� �Z�O�����g�Đ�
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::PlaySourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	pSourceVoice->GetState(&xa2state);

	// �Đ�
	pSourceVoice->Start(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::RestartSourceVoice()
// DESC: XAudio2�\�[�X�{�C�X���� �Z�O�����g�����Đ�
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::RestartSourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	pSourceVoice->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{
		// �ꎞ��~
		pSourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		pSourceVoice->FlushSourceBuffers();

		// �I�[�f�B�I�o�b�t�@�̍ēo�^
		pSourceVoice->SubmitSourceBuffer(&XA2buffer);
	}

	// �Đ�
	pSourceVoice->Start(0);
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::PauseSourceVoice()
// DESC: XAudio2�\�[�X�{�C�X���� �Z�O�����g�ꎞ��~
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::PauseSourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	pSourceVoice->GetState(&xa2state);

	// �Đ��� 
	if (xa2state.BuffersQueued != 0)
	{
		// �ꎞ��~
		pSourceVoice->Stop(0);
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BXA2SourceVoice::PauseSourceVoice()
// DESC: XAudio2�\�[�X�{�C�X���� �Z�O�����g��~
//-----------------------------------------------------------------------------
VOID BXA2SourceVoice::StopSourceVoice()
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	pSourceVoice->GetState(&xa2state);

	// �Đ��� 
	if (xa2state.BuffersQueued != 0)
	{
		// �ꎞ��~
		pSourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		pSourceVoice->FlushSourceBuffers();
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