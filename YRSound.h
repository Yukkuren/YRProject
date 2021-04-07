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

//BGM�̎��
enum class BGMKind : int
{
	TITLE = 0,			//�^�C�g��
	CHARA_SELECT,		//�L�����I�����
	LOAD,				//���[�h���
	GAME,				//�Q�[���{��
	KNIGHT_WIN,			//�i�C�g����
	END,
};

//SE�̎��
enum class SEKind : int
{
	SELECT_ENTER = 0,	//�I�����艹
	SELECT_CANCEL,		//�I������
	HIT_SWORD,			//��������������
	INTRO_WIND,			//�C���g���̕�
	OFFSET,				//���E
	SPECIAL_ATTACK,		//���K
	SPECIAL_ATTACK2,	//���K2
	SPECIAL_ATTACK3,	//���K3
	TORNADO,			//����
	UP_ATTACK,			//�ł��グ�U��
	HIT,				//�U��������������
	PROJECTILE,			//��ѓ���
	SELECT,				//�I����
	TRACK,				//�z�[�~���O�_�b�V����
	BACKSTEP,			//�o�b�N�X�e�b�v
	SLAM,				//�@������
	SLIDE,				//������
	PASSIVE,			//�󂯐g
	READY,				//�J�n�O
	GUARD,				//�K�[�h
	JUMP,				//�W�����v
	HIGH_JUMP,			//�n�C�W�����v
	LANDING,			//���n
	DANGER,				//DANGER���A�i�E���X
	GAUGE_UP,			//�Q�[�W������
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


//-----------------------------------------------------------------
//			YRSound�N���X
//-----------------------------------------------------------------
//�E�����f�[�^��S�ĊǗ�����N���X
//�E�Q�[�����N���������_�őS�Ẳ����f�[�^��ǂݍ��ނ��߁A
//  �ǂ̃V�[���ł����p���邱�Ƃ��ł���
//-----------------------------------------------------------------

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

	float								fade_volume = 1.0f;			//�����̃t�F�[�h�Ɏg�p����{�����[��
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
	bool FadeIn(float elapsed_time, float adjust_time = 1.0f);
	//�t�F�[�h�A�E�g(elapsed_time, ���x)
	bool FadeOut(float elapsed_time, float adjust_time = 1.0f);

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
