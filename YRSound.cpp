#include "YRSound.h"
#include <stdio.h>
#include <assert.h>
#include "CoList.h"
#include <string>
#include <array>

#include"./imgui/imgui.h"
#include"./imgui/imgui_impl_win32.h"
#include"./imgui/imgui_impl_dx11.h"

#ifdef  _DEBUG
std::array<std::string, scastI(BGMKind::END)> bgm_name_list =
{
	u8"�^�C�g��",
};

std::array<std::string, scastI(SEKind::END)> se_name_list =
{
	u8"���艹",
};
#endif // 

//�R���X�g���N�^
void YRSound::Init()
{
	//Com�R���|�[�l���g�̏�����
	if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
	{
		assert("ComCompornent error");
	}

	//XAudio2�̏�����
	if (FAILED(XAudio2Create(&pXAudio)))
	{
		assert("XAudio2Create error");
	}

	//MasteringVoice�̍쐬
	if (FAILED(pXAudio->CreateMasteringVoice(&pMasteringVoice)))
	{
		assert("MasterVoiceCreate error");
	}

	//�\�[�X�{�C�X�̍쐬
	pBGM.resize(scastI(BGMKind::END));
	pSE.resize(scastI(SEKind::END));

	BGMLoad("./Data/Sound/BGM/Title.wav", BGMKind::TITLE, XAUDIO2_LOOP_INFINITE, 0.5f);
	SELoad("./Data/Sound/SE/enter.wav", SEKind::SELECT_ENTER);
}

//�f�X�g���N�^
YRSound::~YRSound()
{
	//�\�[�X�{�C�X�̉��
	if (!pBGM.empty())
	{
		for (int i = 0; i < pBGM.size(); i++)
		{
			pBGM[i].pSourceVoice->Stop();
			pBGM[i].pSourceVoice->DestroyVoice();
			pBGM[i].pSourceVoice = nullptr;
			if (pBGM[i].buf.pAudioData != NULL) { delete[] pBGM[i].buf.pAudioData; }
		}
		pBGM.clear();
	}

	if (!pSE.empty())
	{
		for (int i = 0; i < pSE.size(); i++)
		{
			pSE[i].pSourceVoice->Stop();
			pSE[i].pSourceVoice->DestroyVoice();
			pSE[i].pSourceVoice = nullptr;
			if (pSE[i].buf.pAudioData != NULL) { delete[] pSE[i].buf.pAudioData; }
		}
		pSE.clear();
	}

	//�}�X�^�[�{�C�X�̉��
	if (pMasteringVoice)
	{
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}

	//XAudio2�̉��
	if (pXAudio)
	{
		pXAudio->Release();
		pXAudio = nullptr;
	}

	//COM�R���|�[�l���g�̉��
	CoUninitialize();
}

//����BGM�f�[�^�̓ǂݍ���(Wave)
bool YRSound::BGMLoad(const char* filename, const BGMKind& kind, UINT32 loop_count, float volume)
{
	FILE* file = NULL;
	if (fopen_s(&file, filename, "rb") != 0) {
		return nullptr;
	}
	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, file);

	// Format�`�����N�̓ǂݍ���
	FormatChunk format;
	fread(&format, sizeof(format), 1, file);

	// Data�`�����N�̓ǂݍ���
	fread(&pBGM[scastI(kind)].data, sizeof(pBGM[scastI(kind)].data), 1, file);

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = (char*)malloc(pBGM[scastI(kind)].data.size);
	fread(pBuffer, pBGM[scastI(kind)].data.size, 1, file);

	fclose(file);

	WAVEFORMATEX wfex{};
	// �g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	if (FAILED(pXAudio->CreateSourceVoice(&pBGM[scastI(kind)].pSourceVoice, &wfex))) {
		free(pBuffer);
		return nullptr;
	}
	// �Đ�����g�`�f�[�^�̐ݒ�
	pBGM[scastI(kind)].buf.pAudioData = (BYTE*)pBuffer;
	pBGM[scastI(kind)].buf.Flags = XAUDIO2_END_OF_STREAM;
	pBGM[scastI(kind)].buf.AudioBytes = pBGM[scastI(kind)].data.size;
	pBGM[scastI(kind)].buf.LoopCount = loop_count;
	if (loop_count == 0)
	{
		pBGM[scastI(kind)].buf.LoopBegin = 0;
		pBGM[scastI(kind)].buf.LoopLength = 0;
	}
	pBGM[scastI(kind)].volume = volume;

	//�g�`�f�[�^���o�b�t�@�ɐݒ�
	pBGM[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pBGM[scastI(kind)].buf);
	BGMSetVolume(kind);

	return pBGM[scastI(kind)].pSourceVoice;
}

//����BGM�f�[�^�̓ǂݍ���(Wave)
bool YRSound::SELoad(const char* filename, const SEKind& kind, UINT32 loop_count, float volume)
{
	FILE* file = NULL;
	if (fopen_s(&file, filename, "rb") != 0) {
		return nullptr;
	}
	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, file);

	// Format�`�����N�̓ǂݍ���
	FormatChunk format;
	fread(&format, sizeof(format), 1, file);

	// Data�`�����N�̓ǂݍ���
	fread(&pSE[scastI(kind)].data, sizeof(pSE[scastI(kind)].data), 1, file);

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = (char*)malloc(pSE[scastI(kind)].data.size);
	fread(pBuffer, pSE[scastI(kind)].data.size, 1, file);

	fclose(file);

	WAVEFORMATEX wfex{};
	// �g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	if (FAILED(pXAudio->CreateSourceVoice(&pSE[scastI(kind)].pSourceVoice, &wfex))) {
		free(pBuffer);
		return nullptr;
	}
	// �Đ�����g�`�f�[�^�̐ݒ�
	pSE[scastI(kind)].buf.pAudioData = (BYTE*)pBuffer;
	pSE[scastI(kind)].buf.Flags = XAUDIO2_END_OF_STREAM;
	pSE[scastI(kind)].buf.AudioBytes = pSE[scastI(kind)].data.size;
	pSE[scastI(kind)].buf.LoopCount = loop_count;
	if (loop_count == 0)
	{
		pSE[scastI(kind)].buf.LoopBegin = 0;
		pSE[scastI(kind)].buf.LoopLength = 0;
	}
	pSE[scastI(kind)].volume = volume;

	//�g�`�f�[�^���o�b�t�@�ɐݒ�
	pSE[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pSE[scastI(kind)].buf);
	SESetVolume(kind);

	return pSE[scastI(kind)].pSourceVoice;
}

//����BGM�f�[�^�̍Đ�(wave)
void YRSound::BGMPlay(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->Start();
}

//����SE�f�[�^�̍Đ�(wave)
void YRSound::SEPlay(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->Start();
}

//����BGM�f�[�^�̉��ʐݒ�(wave)
void YRSound::BGMSetVolume(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->SetVolume(pBGM[scastI(kind)].volume);
}

//����SE�f�[�^�̉��ʐݒ�(wave)
void YRSound::SESetVolume(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->SetVolume(pSE[scastI(kind)].volume);
}

//����BGM�f�[�^�̒�~(wave)
void YRSound::BGMStop(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->Stop();
}

//����SE�f�[�^�̒�~(wave)
void YRSound::SEStop(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->Stop();
}


void YRSound::SoundDebugDrow()
{
#ifdef  _DEBUG
	ImGui::Begin("SoundVolume");

	if (!pBGM.empty())
	{
		for (int b = 0; b < pBGM.size(); b++)
		{
			ImGui::SliderFloat(bgm_name_list[b].c_str(), &pBGM[b].volume, 0.0f, 1.0f);
			BGMSetVolume(static_cast<BGMKind>(b));
		}
	}

	if (!pSE.empty())
	{
		for (int s = 0; s < pSE.size(); s++)
		{
			ImGui::SliderFloat(se_name_list[s].c_str(), &pSE[s].volume, 0.0f, 1.0f);
			SESetVolume(static_cast<SEKind>(s));
		}
	}

	ImGui::End();
#endif // 
}