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
	u8"タイトル",
	u8"ゲームメイン",
};

std::array<std::string, scastI(SEKind::END)> se_name_list =
{
	u8"決定音",
};
#endif // 

//初期化
void YRSound::Init()
{
	//Comコンポーネントの初期化
	if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED)))
	{
		assert("ComCompornent error");
	}

	//XAudio2の初期化
	if (FAILED(XAudio2Create(&pXAudio)))
	{
		assert("XAudio2Create error");
	}

	//MasteringVoiceの作成
	if (FAILED(pXAudio->CreateMasteringVoice(&pMasteringVoice)))
	{
		assert("MasterVoiceCreate error");
	}

	fado_volume = 1.0f;

	//ソースボイスの作成
	pBGM.resize(scastI(BGMKind::END));
	pSE.resize(scastI(SEKind::END));

	//BGM読み込み
	BGMLoad("./Data/Sound/BGM/Title.wav", BGMKind::TITLE, XAUDIO2_LOOP_INFINITE, 0.5f);
	BGMLoad("./Data/Sound/BGM/Stage_castle.wav", BGMKind::GAME, XAUDIO2_LOOP_INFINITE, 0.5f);

	//SE読み込み
	SELoad("./Data/Sound/SE/enter.wav", SEKind::SELECT_ENTER);
}

//デストラクタ
YRSound::~YRSound()
{
	//ソースボイスの解放
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

	//マスターボイスの解放
	if (pMasteringVoice)
	{
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}

	//XAudio2の解放
	if (pXAudio)
	{
		pXAudio->Release();
		pXAudio = nullptr;
	}

	//COMコンポーネントの解放
	CoUninitialize();
}

//音声BGMデータの読み込み(Wave)
bool YRSound::BGMLoad(const char* filename, const BGMKind& kind, UINT32 loop_count, float volume)
{
	FILE* file = NULL;
	if (fopen_s(&file, filename, "rb") != 0) {
		return nullptr;
	}
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, file);

	// Formatチャンクの読み込み
	FormatChunk format;
	fread(&format, sizeof(format), 1, file);

	// Dataチャンクの読み込み
	fread(&pBGM[scastI(kind)].data, sizeof(pBGM[scastI(kind)].data), 1, file);

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = (char*)malloc(pBGM[scastI(kind)].data.size);
	fread(pBuffer, pBGM[scastI(kind)].data.size, 1, file);

	fclose(file);

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// 波形フォーマットを元にSourceVoiceの生成
	if (FAILED(pXAudio->CreateSourceVoice(&pBGM[scastI(kind)].pSourceVoice, &wfex))) {
		free(pBuffer);
		return nullptr;
	}
	// 再生する波形データの設定
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

	//波形データをバッファに設定
	pBGM[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pBGM[scastI(kind)].buf);
	BGMSetVolume(kind);

	return pBGM[scastI(kind)].pSourceVoice;
}

//音声BGMデータの読み込み(Wave)
bool YRSound::SELoad(const char* filename, const SEKind& kind, UINT32 loop_count, float volume)
{
	FILE* file = NULL;
	if (fopen_s(&file, filename, "rb") != 0) {
		return nullptr;
	}
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	fread(&riff, sizeof(riff), 1, file);

	// Formatチャンクの読み込み
	FormatChunk format;
	fread(&format, sizeof(format), 1, file);

	// Dataチャンクの読み込み
	fread(&pSE[scastI(kind)].data, sizeof(pSE[scastI(kind)].data), 1, file);

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = (char*)malloc(pSE[scastI(kind)].data.size);
	fread(pBuffer, pSE[scastI(kind)].data.size, 1, file);

	fclose(file);

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// 波形フォーマットを元にSourceVoiceの生成
	if (FAILED(pXAudio->CreateSourceVoice(&pSE[scastI(kind)].pSourceVoice, &wfex))) {
		free(pBuffer);
		return nullptr;
	}
	// 再生する波形データの設定
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

	//波形データをバッファに設定
	pSE[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pSE[scastI(kind)].buf);
	SESetVolume(kind);

	return pSE[scastI(kind)].pSourceVoice;
}

//音声BGMデータの再生(wave)
void YRSound::BGMPlay(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->Start();
}

//音声SEデータの再生(wave)
void YRSound::SEPlay(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->Start();
}

//音声SEデータの単発再生(wave)[再生中なら最初から再生しなおす]
void YRSound::SESinglePlay(const SEKind& kind)
{
	//現在再生中か確認する
	XAUDIO2_VOICE_STATE xa2state;
	pSE[scastI(kind)].pSourceVoice->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{
		//再生中だった
		//再生を停止
		pSE[scastI(kind)].pSourceVoice->Stop();
		//バッファを一度削除する
		pSE[scastI(kind)].pSourceVoice->FlushSourceBuffers();
		//波形データをバッファに設定
		pSE[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pSE[scastI(kind)].buf);
	}
	//SEの再生
	pSE[scastI(kind)].pSourceVoice->Start();
}


//音声データの再生を確認し、
void YRSound::SEFinCheack()
{
	if (!pSE.empty())
	{
		for (int i = 0; i < pSE.size(); i++)
		{
			//現在再生中か確認する
			XAUDIO2_VOICE_STATE xa2state;
			pSE[i].pSourceVoice->GetState(&xa2state);

			if (xa2state.BuffersQueued == 0)
			{
				//再生が終わっていた
				//再生を停止
				pSE[i].pSourceVoice->Stop();
				//バッファを一度削除する
				pSE[i].pSourceVoice->FlushSourceBuffers();
				//波形データをバッファに設定
				pSE[i].pSourceVoice->SubmitSourceBuffer(&pSE[i].buf);
			}
		}
	}
}

//音声BGMデータの音量設定(wave)
void YRSound::BGMSetVolume(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->SetVolume(pBGM[scastI(kind)].volume * bgm_all_volume * fado_volume);
}

//音声SEデータの音量設定(wave)
void YRSound::SESetVolume(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->SetVolume(pSE[scastI(kind)].volume * se_all_volume * fado_volume);
}

//音声BGMデータの停止(wave)
void YRSound::BGMStop(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->Stop();
	//バッファを一度削除する
	pBGM[scastI(kind)].pSourceVoice->FlushSourceBuffers();
	//波形データをバッファに設定
	pBGM[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pBGM[scastI(kind)].buf);
}

//音声SEデータの停止(wave)
void YRSound::SEStop(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->Stop();
	//バッファを一度削除する
	pSE[scastI(kind)].pSourceVoice->FlushSourceBuffers();
	//波形データをバッファに設定
	pSE[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pSE[scastI(kind)].buf);
}

//音声BGMデータの一時停止(wave)
void YRSound::BGMPause(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->Stop();
}

//音声SEデータの一時停止(wave)
void YRSound::SEPause(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->Stop();
}

//デバッグ描画
void YRSound::SoundDebugDrow()
{
#ifdef  _DEBUG
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
#endif // 
}

//更新処理
void YRSound::Update()
{
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


//フェードイン(elapsed_time, 速度)
bool YRSound::FadoIn(float elapsed_time, float adjust_time)
{
	fado_volume += (elapsed_time * adjust_time);

	if (fado_volume > 1.0f)
	{
		fado_volume = 1.0f;
		return true;
	}

	return false;
}

//フェードアウト(elapsed_time, 速度)
bool YRSound::FadoOut(float elapsed_time, float adjust_time)
{
	fado_volume -= (elapsed_time * adjust_time);

	if (fado_volume < 0.0f)
	{
		fado_volume = 0.0f;
		return true;
	}

	return false;
}