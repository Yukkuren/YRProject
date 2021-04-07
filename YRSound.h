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

//BGMの種類
enum class BGMKind : int
{
	TITLE = 0,			//タイトル
	CHARA_SELECT,		//キャラ選択画面
	LOAD,				//ロード画面
	GAME,				//ゲーム本編
	KNIGHT_WIN,			//ナイト勝利
	END,
};

//SEの種類
enum class SEKind : int
{
	SELECT_ENTER = 0,	//選択決定音
	SELECT_CANCEL,		//選択解除
	HIT_SWORD,			//剣が当たった時
	INTRO_WIND,			//イントロの風
	OFFSET,				//相殺
	SPECIAL_ATTACK,		//超必
	SPECIAL_ATTACK2,	//超必2
	SPECIAL_ATTACK3,	//超必3
	TORNADO,			//竜巻
	UP_ATTACK,			//打ち上げ攻撃
	HIT,				//攻撃が当たった時
	PROJECTILE,			//飛び道具
	SELECT,				//選択音
	TRACK,				//ホーミングダッシュ時
	BACKSTEP,			//バックステップ
	SLAM,				//叩き落し
	SLIDE,				//滑り状態
	PASSIVE,			//受け身
	READY,				//開始前
	GUARD,				//ガード
	JUMP,				//ジャンプ
	HIGH_JUMP,			//ハイジャンプ
	LANDING,			//着地
	DANGER,				//DANGER時アナウンス
	GAUGE_UP,			//ゲージ増加時
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


//-----------------------------------------------------------------
//			YRSoundクラス
//-----------------------------------------------------------------
//・音声データを全て管理するクラス
//・ゲームを起動した時点で全ての音声データを読み込むため、
//  どのシーンでも利用することができる
//-----------------------------------------------------------------

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

	float								fade_volume = 1.0f;			//音声のフェードに使用するボリューム
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
	bool FadeIn(float elapsed_time, float adjust_time = 1.0f);
	//フェードアウト(elapsed_time, 速度)
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
