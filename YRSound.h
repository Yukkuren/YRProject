#pragma once

#include <xaudio2.h>
#include <mmsystem.h>
#include <cstdint>
#include <vector>

// チャンクデータの基本構造 
struct Chunk
{
	char	id[4]; // チャンク毎のID
	int32_t	size;  // チャンクサイズ
};

// RIFFヘッダー
struct RiffHeader
{
	Chunk	chunk;   // "RIFF"
	char	type[4]; // "WAVE"
};

// FMTチャンク
struct FormatChunk
{
	Chunk		chunk; // "fmt "
	WAVEFORMAT	fmt;   // 波形フォーマット
};

enum class BGMKind : int
{
	TITLE = 0,			//タイトル
	END,
};

enum class SEKind : int
{
	SELECT_ENTER = 0,	//選択決定音
	END,
};

//サウンドソース構造体
struct AudioSource
{
	IXAudio2SourceVoice*	pSourceVoice = nullptr;	//ソースボイスのポインタ
	XAUDIO2_BUFFER			buf;					//バッファ
	Chunk					data;					//チャンクデータ
	float					volume = 1.0f;			//ボリューム値
};


class YRSound
{
private:
	IXAudio2*							pXAudio = nullptr;			//XAudio2のポインタ
	IXAudio2MasteringVoice*				pMasteringVoice = nullptr;	//マスターボイスのポインタ
	std::vector<AudioSource>			pBGM;						//BGMソースボイスのポインタ
	std::vector<AudioSource>			pSE;						//SEソースボイスのポインタ
public:
	float								bgm_all_volume = 1.0f;		//BGM全体に与えるボリューム値のしきい値
	float								se_all_volume = 1.0f;		//SE全体に与えるボリューム値のしきい値

	float								fado_volume = 1.0f;			//音声のフェードに使用するボリューム
private:
	//コンストラクタ
	YRSound() {};

	//デストラクタ
	~YRSound();

	//音声データの読み込み(Wave)
	bool BGMLoad(const char* filename, const BGMKind& kind, UINT32 loop_count = XAUDIO2_LOOP_INFINITE,float volume = 1.0f);
	bool SELoad(const char* filename, const SEKind& kind, UINT32 loop_count = 0, float volume = 1.0f);

public:
	//初期化
	void Init();

	//更新
	void Update();

	//音声データの再生(wave)
	void BGMPlay(const BGMKind& kind);
	void SEPlay(const SEKind& kind);

	//音声データの単発再生(wave)[再生中なら最初から再生しなおす]
	void SESinglePlay(const SEKind& kind);

	//音声データの終了確認(wave)[終了していた場合最初に戻す]
	void SEFinCheack();

	//音声データの音量設定(wave)
	void BGMSetVolume(const BGMKind& kind);
	void SESetVolume(const SEKind& kind);

	//音声データの停止(wave)
	void BGMStop(const BGMKind& kind);
	void SEStop(const SEKind& kind);

	//音声データの停止(wave)
	void BGMPause(const BGMKind& kind);
	void SEPause(const SEKind& kind);

	//サウンドデバッグ表示
	void SoundDebugDrow();

	//フェードイン(elapsed_time, 速度)
	bool FadoIn(float elapsed_time, float adjust_time = 1.0f);
	//フェードアウト(elapsed_time, 速度)
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
