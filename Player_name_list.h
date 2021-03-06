#pragma once

#include <string>
#include "framework.h"

#ifdef EXIST_IMGUI

//Imguiで表示するために文字列を保存している

struct Name_list
{
public:

	//キャラ名リスト(ファイル読み込みに使用する為、読み込むファイルの名前と同じにすること)
	std::array<std::string, scastI(PLSELECT::PLSELECT_END)> chara_name_list =
	{
		u8"Knight",
		u8"Neru",
	};


	std::array<std::string, scastI(AttackState::ATTACK_END)> attack_name_list =
	{
		u8"攻撃無し()の中身は攻撃内容",
		u8"弱",
		u8"中(下中攻撃)",
		u8"下強(中の次に出る強攻撃)",
		u8"下弱",
		u8"下中",
		u8"上強(飛び上がり)",
		u8"空弱攻撃",
		u8"空中攻撃",
		u8"空強攻撃",
		u8"空上強攻撃(打ち上げ攻撃)",
		u8"つかみ",
		u8"投げ",
		u8"ホーミングダッシュ",
		u8"前弱必殺",
		u8"前中必殺",
		u8"前強必殺",
		u8"後弱必殺",
		u8"後中必殺",
		u8"後強必殺",
		u8"前超必殺",
		u8"後超必殺",
		u8"即死技",
		u8"無敵攻撃",
		u8"弱の次に出る中攻撃",
		u8"中の次に出る強攻撃",
		u8"空中前弱必殺",
		u8"空中前中必殺",
		u8"空中前強必殺",
		u8"空中後弱必殺",
		u8"空中後中必殺",
		u8"空中後強必殺",
		u8"空中前超必殺",
		u8"空中後超必殺",
		u8"Xボタンコンボ",
		u8"Yボタンコンボ",
		u8"Bボタンコンボ",
		u8"Aボタンを押すと出る攻撃(プレイヤー選択)",
		u8"Aボタンを押すと出る空中攻撃(プレイヤー選択)",
	};

	std::array<std::string, scastI(ActState::ACT_END)> act_name_list =
	{
		u8"何もない",
		u8"待機",
		u8"ガード",
		u8"しゃがみ",
		u8"後退",
		u8"ダミー",
		u8"ダッシュ",
		u8"バックステップ",
		u8"ジャンプ",
		u8"空中前ダッシュ",
		u8"空中後ダッシュ",
		u8"ステートを奪われた状態",
		u8"起き上がり",
		u8"受け身",
		u8"ダウン",
		u8"空中ダウン",
		u8"のけぞり",
		u8"叩きつけられ中",
		u8"ダウン攻撃を受けた",
		u8"攻撃中",
	};

	std::array<std::string, scastI(PAD::PAD_END)> pad_name_list =
	{
		u8"UP",
		u8"DOWN",
		u8"LEFT",
		u8"RIGHT",
		u8"START",
		u8"SELECT",
		u8"L3",
		u8"R3",
		u8"LBボタン",
		u8"RBボタン",
		u8"Aボタン",
		u8"Bボタン",
		u8"Xボタン",
		u8"Yボタン",
		u8"BUTTOM_END",
		u8"L_TRIGGER",
		u8"R_TRIGGER",
		u8"STICK_R",
		u8"STICK_L",
		u8"STICK_U",
		u8"STICK_D",
		u8"STICK_RDown",
		u8"STICK_LDown",
		u8"STICK_RUp",
		u8"STICK_LUp",
		u8"HIGH_UP",
		u8"HIGH_UP_R",
		u8"HIGH_UP_L",
		u8"R_DASH",
		u8"L_DASH",
	};


	std::array < std::string, scastI(Command::END) > command_name_list =
	{
		u8"コマンド無し",
		u8"236コマンド",
		u8"214コマンド"
	};

	std::array<bool, scastI(AttackState::ATTACK_END)> linkage_stick_on = { false };


	std::array<std::string, scastI(HitBoxState::END)> hitstate_name_list =
	{
		u8"ガードしない",
		u8"中段ガード",
		u8"下段ガード",
		u8"無敵",
		u8"空中ガード",
		u8"上段攻撃に対して無敵",
	};

	std::array<std::string, scastI(HitResult::END)> result_name_list =
	{
		u8"まだ発生してない",
		u8"当たってない",
		u8"ガードされた",
		u8"当たった",
		u8"キャンセルできない",
	};

	std::array<std::string, scastI(Ground_C::END)> ground_name_list =
	{
		u8"空中",
		u8"地上",
		u8"どちらでも",
	};

	std::array<std::string, scastI(AttackKind::END)> attack_kind_name_list =
	{
		u8"上段",
		u8"中段",
		u8"下段",
		u8"つかみ",
		u8"たたきつけ(高さが一定なら滑り状態にする)",
		u8"ロック技",
		u8"飛び道具",
		u8"ホーミングダッシュ",
		u8"相殺しない攻撃",
		u8"上段攻撃と相殺しない攻撃",
		u8"ダウン攻撃",
	};


	std::array<std::string, scastI(EffectKind::END)> effect_kind_name_list =
	{
		u8"何もなし",
		u8"ガード",
		u8"竜巻",
		u8"剣ヒットエフェクト",
		u8"ドリル",
		u8"相殺",
		u8"赤いドリル",
		u8"緑のドリル",
		u8"白いドリル",
		u8"緑の竜巻",
		u8"衝撃波",
		u8"砂煙",
		u8"ダメージエフェクト",
		u8"ホーミングダッシュ時エフェクト",
		u8"壁激突",
	};


	static Name_list& getInstance()
	{
		static Name_list instance;
		return instance;
	}
};

inline Name_list& GetName()
{
	return Name_list::getInstance();
}
#endif // USE_IMGUI
