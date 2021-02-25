#include "YRSound.h"
#include <stdio.h>
#include <assert.h>
#include "CoList.h"
#include <string>
#include <array>
#include "framework.h"
#include "Key.h"

#include"./imgui/imgui.h"
#include"./imgui/imgui_impl_win32.h"
#include"./imgui/imgui_impl_dx11.h"

#ifdef  EXIST_IMGUI
std::array<std::string, scastI(BGMKind::END)> bgm_name_list =
{
	u8"�^�C�g��",
	u8"�L�����I��",
	u8"���[�h���",
	u8"�Q�[�����C��",
};

std::array<std::string, scastI(SEKind::END)> se_name_list =
{
	u8"���艹",
};
#endif // 

//������
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

	fade_volume = 1.0f;

	//�\�[�X�{�C�X�̍쐬
	pBGM.resize(scastI(BGMKind::END));
	pSE.resize(scastI(SEKind::END));

	bgm_all_volume = 0.7f;
	se_all_volume = 1.0f;

#ifdef _DEBUG
	bgm_all_volume = 0.0f;
	se_all_volume = 0.0f;
#endif // _DEBUG


	//BGM�ǂݍ���
	BGMLoad("./Data/Sound/BGM/Title.wav", BGMKind::TITLE, XAUDIO2_LOOP_INFINITE, 0.5f);
	BGMLoad("./Data/Sound/BGM/Chara_Select.wav", BGMKind::CHARA_SELECT, XAUDIO2_LOOP_INFINITE, 0.5f);
	BGMLoad("./Data/Sound/BGM/Load_Intro.wav", BGMKind::LOAD, 0, 0.5f);
	BGMLoad("./Data/Sound/BGM/Stage_castle.wav", BGMKind::GAME, XAUDIO2_LOOP_INFINITE, 0.5f);
	BGMLoad("./Data/Sound/BGM/knight_win.wav", BGMKind::KNIGHT_WIN, 0.0f, 0.5f);

	//SE�ǂݍ���
	SELoad("./Data/Sound/SE/enter.wav", SEKind::SELECT_ENTER);
	SELoad("./Data/Sound/SE/cancel.wav", SEKind::SELECT_CANCEL);
	SELoad("./Data/Sound/SE/hit_sword.wav", SEKind::HIT_SWORD);
	SELoad("./Data/Sound/SE/intro_wind.wav", SEKind::INTRO_WIND);
	SELoad("./Data/Sound/SE/offset.wav", SEKind::OFFSET);
	SELoad("./Data/Sound/SE/special_attack.wav", SEKind::SPECIAL_ATTACK);
	SELoad("./Data/Sound/SE/tornado.wav", SEKind::TORNADO);
	SELoad("./Data/Sound/SE/hit.wav", SEKind::HIT);
	SELoad("./Data/Sound/SE/special_attack2.wav", SEKind::SPECIAL_ATTACK2);
	SELoad("./Data/Sound/SE/up_attack.wav", SEKind::UP_ATTACK);
	SELoad("./Data/Sound/SE/projectile.wav", SEKind::PROJECTILE);
	SELoad("./Data/Sound/SE/special_attack3.wav", SEKind::SPECIAL_ATTACK3);
	SELoad("./Data/Sound/SE/select.wav", SEKind::SELECT);
	SELoad("./Data/Sound/SE/track.wav", SEKind::TRACK);
	SELoad("./Data/Sound/SE/slam.wav", SEKind::SLAM);
	SELoad("./Data/Sound/SE/backstep.wav", SEKind::BACKSTEP);
	SELoad("./Data/Sound/SE/passive.wav", SEKind::PASSIVE);
	SELoad("./Data/Sound/SE/slide.wav", SEKind::SLIDE);
	SELoad("./Data/Sound/SE/guard.wav", SEKind::GUARD);
	SELoad("./Data/Sound/SE/ready.wav", SEKind::READY);
	SELoad("./Data/Sound/SE/jump.wav", SEKind::JUMP);
	SELoad("./Data/Sound/SE/high_jump.wav", SEKind::HIGH_JUMP);
	SELoad("./Data/Sound/SE/landing.wav", SEKind::LANDING);
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
	if (pBGM.size() <= scastI(kind))
	{
		return;
	}
	pBGM[scastI(kind)].pSourceVoice->Start();
}

//����SE�f�[�^�̍Đ�(wave)
void YRSound::SEPlay(const SEKind& kind)
{
	if (pSE.size() <= scastI(kind))
	{
		return;
	}
	pSE[scastI(kind)].pSourceVoice->Start();
}

//����SE�f�[�^�̒P���Đ�(wave)[�Đ����Ȃ�ŏ�����Đ����Ȃ���]
void YRSound::SESinglePlay(const SEKind& kind)
{
	if (pSE.size() <= scastI(kind))
	{
		return;
	}
	//���ݍĐ������m�F����
	XAUDIO2_VOICE_STATE xa2state;
	pSE[scastI(kind)].pSourceVoice->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{
		//�Đ���������
		//�Đ����~
		pSE[scastI(kind)].pSourceVoice->Stop();
		//�o�b�t�@����x�폜����
		pSE[scastI(kind)].pSourceVoice->FlushSourceBuffers();
		//�g�`�f�[�^���o�b�t�@�ɐݒ�
		pSE[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pSE[scastI(kind)].buf);
	}
	//SE�̍Đ�
	pSE[scastI(kind)].pSourceVoice->Start();
}


//�����f�[�^�̍Đ����m�F���A
void YRSound::SEFinCheack()
{
	if (!pSE.empty())
	{
		for (int i = 0; i < pSE.size(); i++)
		{
			//���ݍĐ������m�F����
			XAUDIO2_VOICE_STATE xa2state;
			pSE[i].pSourceVoice->GetState(&xa2state);

			if (xa2state.BuffersQueued == 0)
			{
				//�Đ����I����Ă���
				//�Đ����~
				pSE[i].pSourceVoice->Stop();
				//�o�b�t�@����x�폜����
				pSE[i].pSourceVoice->FlushSourceBuffers();
				//�g�`�f�[�^���o�b�t�@�ɐݒ�
				pSE[i].pSourceVoice->SubmitSourceBuffer(&pSE[i].buf);
			}
		}
	}
}

//����BGM�f�[�^�̉��ʐݒ�(wave)
void YRSound::BGMSetVolume(const BGMKind& kind)
{
	if (pBGM.size() <= scastI(kind))
	{
		return;
	}
	pBGM[scastI(kind)].pSourceVoice->SetVolume(pBGM[scastI(kind)].volume * bgm_all_volume * fade_volume);
}

//����SE�f�[�^�̉��ʐݒ�(wave)
void YRSound::SESetVolume(const SEKind& kind)
{
	if (pSE.size() <= scastI(kind))
	{
		return;
	}
	pSE[scastI(kind)].pSourceVoice->SetVolume(pSE[scastI(kind)].volume * se_all_volume * fade_volume);
}

//����BGM�f�[�^�̒�~(wave)
void YRSound::BGMStop(const BGMKind& kind)
{
	if (pBGM.size() <= scastI(kind))
	{
		return;
	}
	pBGM[scastI(kind)].pSourceVoice->Stop();
	//�o�b�t�@����x�폜����
	pBGM[scastI(kind)].pSourceVoice->FlushSourceBuffers();
	//�g�`�f�[�^���o�b�t�@�ɐݒ�
	pBGM[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pBGM[scastI(kind)].buf);
}

//����SE�f�[�^�̒�~(wave)
void YRSound::SEStop(const SEKind& kind)
{
	if (pSE.size() <= scastI(kind))
	{
		return;
	}
	pSE[scastI(kind)].pSourceVoice->Stop();
	//�o�b�t�@����x�폜����
	pSE[scastI(kind)].pSourceVoice->FlushSourceBuffers();
	//�g�`�f�[�^���o�b�t�@�ɐݒ�
	pSE[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pSE[scastI(kind)].buf);
}

//����BGM�f�[�^�̈ꎞ��~(wave)
void YRSound::BGMPause(const BGMKind& kind)
{
	if (pBGM.size() <= scastI(kind))
	{
		return;
	}
	pBGM[scastI(kind)].pSourceVoice->Stop();
}

//����SE�f�[�^�̈ꎞ��~(wave)
void YRSound::SEPause(const SEKind& kind)
{
	if (pSE.size() <= scastI(kind))
	{
		return;
	}
	pSE[scastI(kind)].pSourceVoice->Stop();
}

//�f�o�b�O�`��
void YRSound::SoundDebugDrow()
{

#ifdef  EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		ImGui::Begin("SoundVolume");

		ImGui::SliderFloat("BGM_All_volume", &bgm_all_volume, 0.0f, 1.0f);
		ImGui::SliderFloat("SE_All_volume", &se_all_volume, 0.0f, 1.0f);
		if (!pBGM.empty())
		{
			for (int b = 0; b < pBGM.size(); b++)
			{
				ImGui::SliderFloat(bgm_name_list[b].c_str(), &pBGM[b].volume, 0.0f, 1.0f);

			}
		}

		if (!pSE.empty())
		{
			for (int s = 0; s < pSE.size(); s++)
			{
				ImGui::SliderFloat(se_name_list[s].c_str(), &pSE[s].volume, 0.0f, 1.0f);
				//SESetVolume(static_cast<SEKind>(s));
			}
		}

		ImGui::End();
	}
#endif //
}

//�X�V����
void YRSound::Update()
{

#ifdef EXIST_IMGUI

	if (Get_Use_ImGui())
	{
		if (pKeyState.lshiftflg > 0)
		{
			if (pKeyState.upflg > 0)
			{
				bgm_all_volume += 0.01f;
				se_all_volume += 0.01f;
				if (bgm_all_volume > 1.0f)
				{
					bgm_all_volume = 1.0f;
				}
				if (se_all_volume > 1.0f)
				{
					se_all_volume = 1.0f;
				}
			}

			if (pKeyState.downflg > 0)
			{
				bgm_all_volume -= 0.01f;
				se_all_volume -= 0.01f;
				if (bgm_all_volume < 0.0f)
				{
					bgm_all_volume = 0.0f;
				}
				if (se_all_volume < 0.0f)
				{
					se_all_volume = 0.0f;
				}
			}
		}
	}

#endif // EXIST_IMGUI


	if (!pBGM.empty())
	{
		for (int b = 0; b < pBGM.size(); b++)
		{
			BGMSetVolume(static_cast<BGMKind>(b));
		}
	}

	if (!pSE.empty())
	{
		for (int s = 0; s < pSE.size(); s++)
		{
			SESetVolume(static_cast<SEKind>(s));
		}
	}
}


//�t�F�[�h�C��(elapsed_time, ���x)
bool YRSound::FadeIn(float elapsed_time, float adjust_time)
{
	fade_volume += (elapsed_time * adjust_time);

	if (fade_volume > 1.0f)
	{
		fade_volume = 1.0f;
		return true;
	}

	return false;
}

//�t�F�[�h�A�E�g(elapsed_time, ���x)
bool YRSound::FadeOut(float elapsed_time, float adjust_time)
{
	fade_volume -= (elapsed_time * adjust_time);

	if (fade_volume < 0.0f)
	{
		fade_volume = 0.0f;
		return true;
	}

	return false;
}