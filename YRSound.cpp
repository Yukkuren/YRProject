#include "YRSound.h"
#include <stdio.h>
#include <assert.h>
#include "CoList.h"

//コンストラクタ
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

	//ソースボイスの作成
	pBGM.resize(scastI(BGMKind::END));
	pSE.resize(scastI(SEKind::END));

	BGMLoad("./Data/Sound/BGM/megalovania.wav", BGMKind::TITLE);
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
bool YRSound::BGMLoad(const char* filename, const BGMKind& kind)
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

	//波形データをバッファに設定
	pBGM[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pBGM[scastI(kind)].buf);

	return pBGM[scastI(kind)].pSourceVoice;
}

//音声BGMデータの読み込み(Wave)
bool YRSound::SELoad(const char* filename, const SEKind& kind)
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

	//波形データをバッファに設定
	pSE[scastI(kind)].pSourceVoice->SubmitSourceBuffer(&pSE[scastI(kind)].buf);

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

//音声BGMデータの停止(wave)
void YRSound::BGMStop(const BGMKind& kind)
{
	pBGM[scastI(kind)].pSourceVoice->Stop();
}

//音声SEデータの停止(wave)
void YRSound::SEStop(const SEKind& kind)
{
	pSE[scastI(kind)].pSourceVoice->Stop();
}