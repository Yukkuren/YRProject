#pragma once

#include <xaudio2.h>
#include <mmsystem.h>
#include <cstdint>
#include <vector>

// �`�����N�f�[�^�̊�{�\�� 
struct Chunk
{
	char	id[4]; // �`�����N����ID
	int32_t	size;  // �`�����N�T�C�Y
};

// RIFF�w�b�_�[
struct RiffHeader
{
	Chunk	chunk;   // "RIFF"
	char	type[4]; // "WAVE"
};

// FMT�`�����N
struct FormatChunk
{
	Chunk		chunk; // "fmt "
	WAVEFORMAT	fmt;   // �g�`�t�H�[�}�b�g
};

enum class BGMKind : int
{
	TITLE = 0,			//�^�C�g��
	END,
};

enum class SEKind : int
{
	SELECT_ENTER = 0,	//�I�����艹
	END,
};

//�T�E���h�\�[�X�\����
struct AudioSource
{
	IXAudio2SourceVoice*	pSourceVoice = nullptr;	//�\�[�X�{�C�X�̃|�C���^
	XAUDIO2_BUFFER			buf;					//�o�b�t�@
	Chunk					data;					//�`�����N�f�[�^
	float					volume = 1.0f;			//�{�����[���l
};


class YRSound
{
private:
	IXAudio2*							pXAudio = nullptr;			//XAudio2�̃|�C���^
	IXAudio2MasteringVoice*				pMasteringVoice = nullptr;	//�}�X�^�[�{�C�X�̃|�C���^
	std::vector<AudioSource>			pBGM;						//BGM�\�[�X�{�C�X�̃|�C���^
	std::vector<AudioSource>			pSE;						//SE�\�[�X�{�C�X�̃|�C���^
public:
	float								bgm_all_volume = 1.0f;		//BGM�S�̂ɗ^����{�����[���l�̂������l
	float								se_all_volume = 1.0f;		//SE�S�̂ɗ^����{�����[���l�̂������l

	float								fado_volume = 1.0f;			//�����̃t�F�[�h�Ɏg�p����{�����[��
private:
	//�R���X�g���N�^
	YRSound() {};

	//�f�X�g���N�^
	~YRSound();

	//�����f�[�^�̓ǂݍ���(Wave)
	bool BGMLoad(const char* filename, const BGMKind& kind, UINT32 loop_count = XAUDIO2_LOOP_INFINITE,float volume = 1.0f);
	bool SELoad(const char* filename, const SEKind& kind, UINT32 loop_count = 0, float volume = 1.0f);

public:
	//������
	void Init();

	//�X�V
	void Update();

	//�����f�[�^�̍Đ�(wave)
	void BGMPlay(const BGMKind& kind);
	void SEPlay(const SEKind& kind);

	//�����f�[�^�̒P���Đ�(wave)[�Đ����Ȃ�ŏ�����Đ����Ȃ���]
	void SESinglePlay(const SEKind& kind);

	//�����f�[�^�̏I���m�F(wave)[�I�����Ă����ꍇ�ŏ��ɖ߂�]
	void SEFinCheack();

	//�����f�[�^�̉��ʐݒ�(wave)
	void BGMSetVolume(const BGMKind& kind);
	void SESetVolume(const SEKind& kind);

	//�����f�[�^�̒�~(wave)
	void BGMStop(const BGMKind& kind);
	void SEStop(const SEKind& kind);

	//�����f�[�^�̒�~(wave)
	void BGMPause(const BGMKind& kind);
	void SEPause(const SEKind& kind);

	//�T�E���h�f�o�b�O�\��
	void SoundDebugDrow();

	//�t�F�[�h�C��(elapsed_time, ���x)
	bool FadoIn(float elapsed_time, float adjust_time = 1.0f);
	//�t�F�[�h�A�E�g(elapsed_time, ���x)
	bool FadoOut(float elapsed_time, float adjust_time = 1.0f);

	static YRSound& getInstance()
	{
		static YRSound instance;
		return instance;
	}
};

inline YRSound& GetSound()
{
	return YRSound::getInstance();
}
