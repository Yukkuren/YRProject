#include "Knight.h"
#include "PlayerBase.h"
#include "framework.h"
#include <fstream>


#if USE_IMGUI

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
};

std::array<std::string, scastI(ActState::ACT_END)> act_name_list =
{
	u8"何もない",
	u8"待機",
	u8"ガード",
	u8"しゃがみ",
	u8"右移動",
	u8"左移動",
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

std::array<std::string, scastI(KNIGHTHIT::END)> hit_name_list =
{
	u8"ボディ",
	u8"足",
};

std::array<std::string, scastI(HitBoxState::END)> hitstate_name_list =
{
	u8"ガードしない",
	u8"中段ガード",
	u8"下段ガード",
	u8"無敵",
	u8"空中ガード",
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
};
#endif // USE_IMGUI

bool Knight::DEBUGAttackLoad()
{
	//AttackStateの順に生成する
	attack_list.resize(scastI(AttackState::ATTACK_END));

	attack_list[scastI(AttackState::NONE)].attack_name = AttackState::NONE;
	attack_list[scastI(AttackState::JAKU)].attack_name = AttackState::JAKU;
	attack_list[scastI(AttackState::JAKU)].attack_max = 1;
	attack_list[scastI(AttackState::JAKU)].later = 0.2f;
	attack_list[scastI(AttackState::JAKU)].attack_single.resize(attack_list[scastI(AttackState::JAKU)].attack_max);
	ac_attack[scastI(AttackState::JAKU)].fream = 10.0f;
	ac_attack[scastI(AttackState::JAKU)].timer = 15.0f;
	ac_attack[scastI(AttackState::JAKU)].later = 1.5f;
	attack_list[scastI(AttackState::JAKU)].linkage_button = PAD::X;
	attack_list[scastI(AttackState::JAKU)].linkage_command = Command::NOCOMMAND;
	attack_list[scastI(AttackState::JAKU)].ground_on = Ground_C::GROUND;
	attack_list[scastI(AttackState::JAKU)].squat_on = false;
	attack_list[scastI(AttackState::JAKU)].linkage_stick = PAD::BUTTOM_END;
	attack_list[scastI(AttackState::JAKU)].need_gauge = 0.0f;
	attack_list[scastI(AttackState::JAKU)].aid_attack_name = AttackState::NONE;
	attack_list[scastI(AttackState::JAKU)].real_attack = attack_list[scastI(AttackState::JAKU)].attack_name;
	for (int i = 0; i < attack_list[scastI(AttackState::JAKU)].attack_max; i++)
	{
		attack_list[scastI(AttackState::JAKU)].attack_single[i].fream = 0.1f;
		attack_list[scastI(AttackState::JAKU)].attack_single[i].quantity = 1;
		attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter.resize(attack_list[scastI(AttackState::JAKU)].attack_single[i].quantity);
		for (int v = 0; v < attack_list[scastI(AttackState::JAKU)].attack_single[i].quantity; v++)
		{
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].damege = 5;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].distance.x = 6.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].distance.y = 1.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].gaugeout = true;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].HB_timer = 0.1f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].hitback.x = Getapply(1.0f);
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].hitback.y = 0.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].knockback = 1.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].size.x = 3.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].size.y = 1.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].stealtimer = 0.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].timer = 0.15f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].type = AttackKind::MIDDLE;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].HS_timer = HitStopTime::SHORT;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].gauge_get = 1.0f;
		}
	}
	//attack_list.push_back(AttackList());
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_name = AttackState::SPECIAL_ATTACK;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_max = 1;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].later = 1.5f;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single.resize(attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_max);
	ac_attack[scastI(AttackState::SPECIAL_ATTACK)].fream = 1.0f;
	ac_attack[scastI(AttackState::SPECIAL_ATTACK)].timer = 3.5f;
	ac_attack[scastI(AttackState::SPECIAL_ATTACK)].later = 0.8f;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].linkage_button = PAD::B;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].linkage_command = Command::NOCOMMAND;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].ground_on = Ground_C::GROUND;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].squat_on = false;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].linkage_stick = PAD::BUTTOM_END;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].need_gauge = 0.0f;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].aid_attack_name = AttackState::NONE;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].real_attack = attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_name;
	
	for (int i = 0; i < attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_max; i++)
	{
		attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].fream = 2.0f;
		attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].quantity = 1;
		attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter.resize(attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].quantity);
		for (int v = 0; v < attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].quantity; v++)
		{
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].damege = 30;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].distance.x = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].distance.y = 2.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].gaugeout = false;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].HB_timer = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].hitback.x = Getapply(1.0f);
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].hitback.y = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].knockback = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].size.x = 5.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].size.y = 5.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].stealtimer = 0.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].timer = 0.3f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].type = AttackKind::MIDDLE;
		}
	}

	AttackState a = AttackState::NONE;
	for (int i = 0; i < attack_list.size(); i++)
	{
		a = static_cast<AttackState>(i);
		attack_list[i].attack_name = a;
	}

	hitparam_list.resize(scastI(KNIGHTHIT::END));
	for (int list = 0; list < hitparam_list[scastI(KNIGHTHIT::BODY)].act_parameter.size(); list++)
	{
		hitparam_list[scastI(KNIGHTHIT::BODY)].act_parameter[list].distance = YR_Vector3(0.0f, 0.0f);
		hitparam_list[scastI(KNIGHTHIT::BODY)].act_parameter[list].size = YR_Vector3(2.0f, 2.9f);
		//hitparam_list[scastI(KNIGHTHIT::BODY)].act_parameter[list].state = HitBoxState::NOGUARD;
	}
	for (int list = 0; list < hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter.size(); list++)
	{
		hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter[list].distance = YR_Vector3(0.0f, 0.0f);
		hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter[list].size = YR_Vector3(1.4f, 0.8f);
		//hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter[list].state = HitBoxState::NOGUARD;
	}

	return true;
}


bool Knight::AttackLoad()
{
	//書き出されたテキストデータから攻撃リストを生成する
	attack_list.resize(scastI(AttackState::ATTACK_END));
	std::ifstream ifs("./Data/CharaParameter/Knight/AttackParam.txt");

	ifs >> jump_later;
	ifs >> dash_later;

	for (int list = 0; list < attack_list.size(); list++)
	{
		ifs >> attack_list[list].later;
		ifs >> attack_list[list].attack_max;
		int pad,com,stick,aid,real,next,result,ground;
		ifs >> pad;
		ifs >> com;
		attack_list[list].linkage_button = static_cast<PAD>(pad);
		attack_list[list].linkage_command = static_cast<Command>(com);
		ifs >> ground;
		attack_list[list].ground_on = static_cast<Ground_C>(ground);
		ifs >> attack_list[list].squat_on;
		ifs >> attack_list[list].need_gauge;
		ifs >> stick;
		ifs >> aid;
		ifs >> real;
		attack_list[list].linkage_stick = static_cast<PAD>(stick);
		attack_list[list].aid_attack_name = static_cast<AttackState>(aid);
		attack_list[list].real_attack = static_cast<AttackState>(real);
		attack_list[list].attack_single.resize(attack_list[list].attack_max);
		ifs >> attack_list[list].speed_on;
		ifs >> attack_list[list].speed.x;
		ifs >> attack_list[list].speed.y;
		ifs >> attack_list[list].speed.z;
		ifs >> attack_list[list].advance_speed;
		ifs >> next;
		ifs >> result;
		attack_list[list].combo = static_cast<AttackState>(next);
		attack_list[list].conditions_hit = static_cast<HitResult>(result);
		ifs >> attack_list[list].timer;



		//攻撃回数ごとのパラメータ書き出し
		if (!attack_list[list].attack_single.empty())
		{
			for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
			{
				ifs >> attack_list[list].attack_single[sin].fream;
				ifs >> attack_list[list].attack_single[sin].quantity;
				attack_list[list].attack_single[sin].parameter.resize(attack_list[list].attack_single[sin].quantity);
				if (!attack_list[list].attack_single[sin].parameter.empty())
				{
					for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
					{
						//攻撃判定内部パラメーター書き出し
						ifs >> attack_list[list].attack_single[sin].parameter[para].distance.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].distance.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].size.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].size.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].damege;
						ifs >> attack_list[list].attack_single[sin].parameter[para].HB_timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].hitback.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].hitback.y;
						int type = 0;
						ifs >> type;
						attack_list[list].attack_single[sin].parameter[para].type = static_cast<AttackKind>(type);
						ifs >> attack_list[list].attack_single[sin].parameter[para].knockback;
						ifs >> attack_list[list].attack_single[sin].parameter[para].gaugeout;
						ifs >> attack_list[list].attack_single[sin].parameter[para].stealtimer;
						int hs;
						ifs >> hs;
						attack_list[list].attack_single[sin].parameter[para].HS_timer = static_cast<HitStopTime>(hs);
						ifs >> attack_list[list].attack_single[sin].parameter[para].gauge_get;
					}
				}
			}
		}
	}
	//もし落ちたらエラーを出す
	if (ifs.fail())
	{
		bool set = false;
		ImGui::SetNextWindowSize(ImVec2(250, 500), 2);
		ImGui::Begin("Error", &set);
		//std::cout << ”読み込みに失敗しました” << std::endl;
		ImGui::Text("読み込みに失敗しました");
		ImGui::End();
	}
	//ファイルを閉じる
	ifs.close();

	//アニメーション調整値を読み込む
	std::ifstream anim_ifs("./Data/CharaParameter/Knight/AnimParam.txt");
	for (int list = 0; list < ac_attack.size(); list++)
	{
		anim_ifs >> ac_attack[list].fream;
		anim_ifs >> ac_attack[list].timer;
		anim_ifs >> ac_attack[list].later;
	}
	//もし落ちたらエラーを出す
	if (anim_ifs.fail())
	{
		bool set = false;
		ImGui::SetNextWindowSize(ImVec2(250, 500), 2);
		ImGui::Begin("Error", &set);
		//std::cout << ”読み込みに失敗しました” << std::endl;
		ImGui::Text("アニメーション調整値の読み込みに失敗しました");
		ImGui::End();
	}
	//ファイルを閉じる
	anim_ifs.close();



	//アニメーション調整値を読み込む
	std::ifstream anim_act_ifs("./Data/CharaParameter/Knight/AnimActParam.txt");
	for (int list = 0; list < ac_act.size(); list++)
	{
		anim_act_ifs >> ac_act[list].fream;
		anim_act_ifs >> ac_act[list].timer;
		anim_act_ifs >> ac_act[list].later;
	}
	//もし落ちたらエラーを出す
	if (anim_ifs.fail())
	{
		bool set = false;
		ImGui::SetNextWindowSize(ImVec2(250, 500), 2);
		ImGui::Begin("Error", &set);
		//std::cout << ”読み込みに失敗しました” << std::endl;
		ImGui::Text("アニメーション調整値の読み込みに失敗しました");
		ImGui::End();
	}
	//ファイルを閉じる
	anim_act_ifs.close();


	//Xボタンコンボリストを読み込む
	std::ifstream combo_X_ifs("./Data/CharaParameter/Knight/Combo_X.txt");
	int size = -1;
	int  com = 0;
	combo_X_ifs >> size;
	if (size > 0)
	{
		combolist_X.combolist.resize(size);

		if (!combolist_X.combolist.empty())
		{
			for (int list = 0; list < combolist_X.combolist.size(); list++)
			{
				combo_X_ifs >> com;
				combolist_X.combolist[list] = static_cast<AttackState>(com);
			}
		}
	}
	//もし落ちたらエラーを出す
	if (combo_X_ifs.fail())
	{
		bool set = false;
		ImGui::SetNextWindowSize(ImVec2(250, 500), 2);
		ImGui::Begin("Error", &set);
		//std::cout << ”読み込みに失敗しました” << std::endl;
		ImGui::Text("アニメーション調整値の読み込みに失敗しました");
		ImGui::End();
	}
	//ファイルを閉じる
	combo_X_ifs.close();



	//当たり判定調整値を読み込む
	std::ifstream hit_ifs("./Data/CharaParameter/Knight/HitParam.txt");
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			//行動中当たり判定読み込み
			hit_ifs >> hitparam_list[list].act_parameter[act].distance.x;
			hit_ifs >> hitparam_list[list].act_parameter[act].distance.y;
			hit_ifs >> hitparam_list[list].act_parameter[act].size.x;
			hit_ifs >> hitparam_list[list].act_parameter[act].size.y;
			/*int state;
			hit_ifs >> state;*/
			//hitparam_list[list].act_parameter[act].state = static_cast<HitBoxState>(state);
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			//攻撃中当たり判定読み込み
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.y;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.y;
			/*int state;
			hit_ifs >> state;*/
			//hitparam_list[list].attack_parameter[atk].state = static_cast<HitBoxState>(state);
		}
	}
	//もし落ちたらエラーを出す
	if (hit_ifs.fail())
	{
		bool set = false;
		ImGui::SetNextWindowSize(ImVec2(250, 500), 2);
		ImGui::Begin("Error", &set);
		//std::cout << ”読み込みに失敗しました” << std::endl;
		ImGui::Text("アニメーション調整値の読み込みに失敗しました");
		ImGui::End();
	}
	//ファイルを閉じる
	hit_ifs.close();


	return true;
}

bool Knight::AttackClean()
{
	jump_later = 1.0f;
	dash_later = 1.0f;

	//攻撃パラメーターを全て初期化する
	for (int list = 0; list < attack_list.size(); list++)
	{
		attack_list[list].later = 0.1f;
		attack_list[list].attack_max = 1;
		attack_list[list].attack_single.resize(attack_list[list].attack_max);
		ac_attack[list].fream = 1.0f;
		ac_attack[list].timer = 1.0f;
		ac_attack[list].later = 1.0f;
		attack_list[list].now_attack_num = 0;
		attack_list[list].linkage_button = PAD::BUTTOM_END;
		attack_list[list].linkage_command = Command::NOCOMMAND;
		attack_list[list].ground_on = attack_list[list].ground_on;
		attack_list[list].squat_on = false;
		attack_list[list].need_gauge = 0.0f;
		attack_list[list].linkage_stick = PAD::BUTTOM_END;
		attack_list[list].aid_attack_name = AttackState::NONE;
		attack_list[list].attack_name = static_cast<AttackState>(list);
		attack_list[list].real_attack = attack_list[list].attack_name;
		attack_list[list].speed_on = false;
		attack_list[list].speed = YR_Vector3(0.0f, 0.0f);
		attack_list[list].advance_speed = 0.0f;
		attack_list[list].combo = attack_list[list].attack_name;
		attack_list[list].conditions_hit = HitResult::HIT;
		attack_list[list].timer = 0.0f;
		//攻撃回数ごとのパラメータ初期化
		for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
		{
			attack_list[list].attack_single[sin].fream = 0.1f;
			attack_list[list].attack_single[sin].quantity = 1;
			attack_list[list].attack_single[sin].parameter.resize(attack_list[list].attack_single[sin].quantity);
			for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
			{
				//攻撃判定内部パラメーター初期化
				attack_list[list].attack_single[sin].parameter[para].distance.x = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].distance.y = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].size.x = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].size.y = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].timer = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].damege = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].HB_timer = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].hitback.x = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].hitback.y = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].type = AttackKind::MIDDLE;
				attack_list[list].attack_single[sin].parameter[para].knockback = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].gaugeout = false;
				attack_list[list].attack_single[sin].parameter[para].stealtimer = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].HS_timer = HitStopTime::NORMAL;
				attack_list[list].attack_single[sin].parameter[para].gauge_get = 0.1f;
			}
		}
	}

	//アニメーション調整値を全て初期化する
	for (int list = 0; list < ac_attack.size(); list++)
	{
		ac_attack[list].fream = 1.0f;
		ac_attack[list].timer = 1.0f;
		ac_attack[list].later = 1.0f;
	}
	for (int list = 0; list < ac_act.size(); list++)
	{
		ac_act[list].fream = 1.0f;
		ac_act[list].timer = 1.0f;
		ac_act[list].later = 1.0f;
	}

	//当たり判定調整値も全て初期化する
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			hitparam_list[list].act_parameter[act].distance = YR_Vector3(0.0f, 0.0f);
			hitparam_list[list].act_parameter[act].size = YR_Vector3(1.0f, 1.0f);
			//hitparam_list[list].act_parameter[act].state = HitBoxState::NOGUARD;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			hitparam_list[list].attack_parameter[atk].distance = YR_Vector3(0.0f, 0.0f);
			hitparam_list[list].attack_parameter[atk].size = YR_Vector3(1.0f, 1.0f);
			//hitparam_list[list].attack_parameter[atk].state = HitBoxState::NOGUARD;
		}
	}
	return true;
}

bool Knight::AttackWrite()
{
	//AttackStateの順に保存する
	std::ofstream outputfile("./Data/CharaParameter/Knight/AttackParam.txt");
	
	outputfile << jump_later << std::endl;
	outputfile << dash_later << std::endl;

	for (int list = 0; list < attack_list.size(); list++)
	{
		outputfile << attack_list[list].later << std::endl;
		outputfile << attack_list[list].attack_max << std::endl;
		outputfile << scastI(attack_list[list].linkage_button) << std::endl;
		outputfile << scastI(attack_list[list].linkage_command) << std::endl;
		outputfile << scastI(attack_list[list].ground_on) << std::endl;
		outputfile << attack_list[list].squat_on << std::endl;
		outputfile << attack_list[list].need_gauge << std::endl;
		outputfile << scastI(attack_list[list].linkage_stick) << std::endl;
		outputfile << scastI(attack_list[list].aid_attack_name) << std::endl;
		outputfile << scastI(attack_list[list].real_attack) << std::endl;
		outputfile << attack_list[list].speed_on << std::endl;
		outputfile << attack_list[list].speed.x << std::endl;
		outputfile << attack_list[list].speed.y << std::endl;
		outputfile << attack_list[list].speed.z << std::endl;
		outputfile << attack_list[list].advance_speed << std::endl;
		outputfile << scastI(attack_list[list].combo) << std::endl;
		outputfile << scastI(attack_list[list].conditions_hit) << std::endl;
		outputfile << attack_list[list].timer << std::endl;

		//攻撃回数ごとのパラメータ書き出し
		if (!attack_list[list].attack_single.empty())
		{
			for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
			{
				outputfile << attack_list[list].attack_single[sin].fream << std::endl;
				outputfile << attack_list[list].attack_single[sin].quantity << std::endl;
				if (!attack_list[list].attack_single[sin].parameter.empty())
				{
					for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
					{
						//攻撃判定内部パラメーター書き出し
						outputfile << attack_list[list].attack_single[sin].parameter[para].distance.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].distance.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].size.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].size.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].timer << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].damege << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].HB_timer << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].hitback.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].hitback.y << std::endl;
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].type) << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].knockback << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].gaugeout << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].stealtimer << std::endl;
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].HS_timer) << std::endl;
						if (!attack_list[list].attack_single[sin].parameter[para].gaugeout)
						{
							outputfile << attack_list[list].attack_single[sin].parameter[para].gauge_get << std::endl;
						}
						else
						{
							outputfile << float(0.0) << std::endl;
						}
					}
				}
			}
		}
	}

	outputfile.close();

	//アニメーション調整値も書き出す
	std::ofstream animout("./Data/CharaParameter/Knight/AnimParam.txt");
	for (int list = 0; list < ac_attack.size(); list++)
	{
		animout << ac_attack[list].fream << std::endl;
		animout << ac_attack[list].timer << std::endl;
		animout << ac_attack[list].later << std::endl;
	}

	animout.close();

	std::ofstream animactout("./Data/CharaParameter/Knight/AnimActParam.txt");
	for (int list = 0; list < ac_act.size(); list++)
	{
		animactout << ac_act[list].fream << std::endl;
		animactout << ac_act[list].timer << std::endl;
		animactout << ac_act[list].later << std::endl;
	}

	animactout.close();

	//当たり判定調整値も書き出す
	std::ofstream hitout("./Data/CharaParameter/Knight/HitParam.txt");
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			//行動中当たり判定書き込み
			hitout << hitparam_list[list].act_parameter[act].distance.x << std::endl;
			hitout << hitparam_list[list].act_parameter[act].distance.y << std::endl;
			hitout << hitparam_list[list].act_parameter[act].size.x << std::endl;
			hitout << hitparam_list[list].act_parameter[act].size.y << std::endl;
			//hitout << scastI(hitparam_list[list].act_parameter[act].state) << std::endl;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			//攻撃中当たり判定書き込み
			hitout << hitparam_list[list].attack_parameter[atk].distance.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].distance.y << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.y << std::endl;
			//hitout << scastI(hitparam_list[list].attack_parameter[atk].state) << std::endl;
		}
	}
	hitout.close();

	std::ofstream combo_X_out("./Data/CharaParameter/Knight/Combo_X.txt");
	if (!combolist_X.combolist.empty())
	{
		combo_X_out << combolist_X.combolist.size() << std::endl;
		for (int list = 0; list < combolist_X.combolist.size(); list++)
		{
			//Xボタンコンボリスト書き込み
			combo_X_out << scastI(combolist_X.combolist[list]) << std::endl;
		}
	}

	combo_X_out.close();

	return true;
}


void Knight::DrawDEBUG(
	YRShader* geoshader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		hit[i].Draw(geoshader, view, projection, light_direction, light_color, ambient_color);
	}

	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (a.attack)
			{
				if (a.hit_ok)
				{
					a.Draw(geoshader, view, projection, light_direction, light_color, ambient_color);
				}
			}
		}
	}

	if (!projectile_atk.empty())
	{
		for (int i = 0; i < projectile_atk.size(); i++)
		{
			if (projectile_atk[i].attack)
			{
				if (projectile_atk[i].hit_ok)
				{
					projectile_atk[i].Draw(geoshader, view, projection, light_direction, light_color, ambient_color);
				}
			}
		}
	}

	//攻撃リスト作成用処理
#if USE_IMGUI
	{

		std::string now_play = std::to_string(now_player);
		now_play += std::string(":RyuHitBox");
		ImGui::Begin(now_play.c_str());
		ImGui::Text(u8"行動ステート");
		ImGui::Text(act_name_list[scastI(act_state)].c_str());
		ImGui::Text(u8"攻撃ステート");
		ImGui::Text(attack_name_list[scastI(attack_state)].c_str());

		ImGui::SliderFloat(u8"ジャンプの着地スキ", &jump_later, 0.0f, 2.0f);
		ImGui::SliderFloat(u8"ダッシュの着地スキ", &dash_later, 0.0f, 2.0f);

		if (ImGui::TreeNode(u8"プレイヤー当たり判定調整"))
		{
			for (int list = 0; list < hitparam_list.size(); list++)
			{
				//プレイヤーの当たり判定をそれぞれ出す
				if (ImGui::TreeNode(hit_name_list[list].c_str()))
				{
					if (ImGui::TreeNode(u8"行動当たり判定"))
					{
						for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
						{
							if (ImGui::TreeNode(act_name_list[act].c_str()))
							{
								ImGui::SliderFloat(u8"プレイヤーとの距離X", &hitparam_list[list].act_parameter[act].distance.x, -200.0f, 200.0f);
								ImGui::SliderFloat(u8"プレイヤーとの距離Y", &hitparam_list[list].act_parameter[act].distance.y, -200.0f, 200.0f);
								ImGui::SliderFloat(u8"大きさX", &hitparam_list[list].act_parameter[act].size.x, 0.0f, 500.0f);
								ImGui::SliderFloat(u8"大きさY", &hitparam_list[list].act_parameter[act].size.y, 0.0f, 500.0f);
								//int state = scastI(hitparam_list[list].act_parameter[act].state);
								//ImGui::SliderInt(u8"状態", &state, 0, scastI(HitBoxState::END)-1);
								//hitparam_list[list].act_parameter[act].state = static_cast<HitBoxState>(state);
								//ImGui::Text(hitstate_name_list[state].c_str());
								ImGui::InputFloat(u8"モーション速度 : 発生", &ac_act[act].fream, 0.01f, 0.1f);
								ImGui::InputFloat(u8"モーション速度 : 持続", &ac_act[act].timer, 0.01f, 0.1f);
								ImGui::InputFloat(u8"モーション速度 : 後スキ", &ac_act[act].later, 0.01f, 0.1f);
								ImGui::Text(u8"ものによってはfreamしか使用しないものもあるので注意");
								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}

					if (ImGui::TreeNode(u8"攻撃当たり判定"))
					{
						for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
						{
							if (ImGui::TreeNode(attack_name_list[atk].c_str()))
							{
								ImGui::SliderFloat(u8"プレイヤーとの距離X", &hitparam_list[list].attack_parameter[atk].distance.x, -200.0f, 200.0f);
								ImGui::SliderFloat(u8"プレイヤーとの距離Y", &hitparam_list[list].attack_parameter[atk].distance.y, -200.0f, 200.0f);
								ImGui::SliderFloat(u8"大きさX", &hitparam_list[list].attack_parameter[atk].size.x, 0.0f, 500.0f);
								ImGui::SliderFloat(u8"大きさY", &hitparam_list[list].attack_parameter[atk].size.y, 0.0f, 500.0f);
								//int state = scastI(hitparam_list[list].attack_parameter[atk].state);
								//ImGui::SliderInt(u8"状態", &state, 0, scastI(HitBoxState::END));
								//hitparam_list[list].attack_parameter[atk].state = static_cast<HitBoxState>(state);
								//ImGui::Text(hitstate_name_list[state].c_str());
								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
		ImGui::InputFloat("eye_offset.x", &eye_offset.x, 0.01f, 0.01f);
		ImGui::InputFloat("eye_offset.y", &eye_offset.y, 0.01f, 0.01f);
		ImGui::InputFloat("mouse_offset.x", &mouth_offset.x, 0.01f, 0.01f);
		ImGui::InputFloat("mouse_offset.y", &mouth_offset.y, 0.01f, 0.01f);
		ImGui::Text("player.x:%f", pos.x);
		ImGui::Text("player.y:%f", pos.y);
		ImGui::Text("command_timer:%f", pad->com_list.command_timer);
		ImGui::Text("track:%d", trackgauge);
		ImGui::Text("hitState : "); ImGui::SameLine();
		ImGui::Text(hitstate_name_list[scastI(hit[0].state)].c_str());
		if (ImGui::TreeNode("ModelParameter"))
		{
			ImGui::SliderFloat("scale_x", &scale.x, -10.0f, 10.0f);
			ImGui::SliderFloat("scale_y", &scale.y, -10.0f, 10.0f);
			ImGui::SliderFloat("scale_z", &scale.z, -10.0f, 10.0f);
			ImGui::SliderFloat("angle_x", &angle.x, -10.0f, 10.0f);
			ImGui::SliderFloat("angle_y", &angle.y, -10.0f, 10.0f);
			ImGui::SliderFloat("angle_z", &angle.z, -10.0f, 10.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("StepParameter"))
		{
			ImGui::Text("speed.x:%f", speed.x);
			//ImGui::SliderFloat("speed", &backstepS, 0.0f, 500.0f);
			//ImGui::SliderFloat("down", &backstepD, 0.0f, 500.0f);
			//ImGui::SliderFloat("air_speed", &stepspeed, 0.0f, 500.0f);
			//ImGui::SliderFloat("air_down", &stepD, 0.0f, 500.0f);
			//ImGui::SliderFloat("passive_speed_X", &passive_speed.x, 0.0f, 500.0f);
			//ImGui::SliderFloat("passive_speed_Y", &passive_speed.y, 0.0f, 500.0f);
			//ImGui::SliderFloat("passive_break", &passive_brake, 0.0f, 500.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("JumpParameter"))
		{
			static float s = 0.0f;


			if (jumpflag)
			{
				if (!max_jump_flag)
				{
					s = 0.0f;
				}
				s = min(speed.y, s);
			}

			ImGui::Checkbox("jump_flug", &jumpflag);
			ImGui::Checkbox("max_jump_flug", &max_jump_flag);
			ImGui::Text("speed.y:%f", speed.y);
			ImGui::Text("speed_X:%f", speed_X.speed);
			ImGui::Text("speed_Y:%f", speed_Y.speed);
			ImGui::Text("down_force:%f", down_force);
			ImGui::Text("s:%f", s);
			//ImGui::SliderFloat("jump_max", &jump_max, 0.0f, 500.0f);
			ImGui::SliderFloat("down_force", &down_force, 0.0f, 1000.0f);
			//ImGui::SliderFloat("high_jump_max", &high_jump_max, 0.0f, 1000.0f);
			//ImGui::SliderFloat("jump_speed", &jump_speed, 0.0f, 5000.0f);
			//ImGui::SliderFloat("high_jump_speed", &high_jump_speed, 0.0f, 5000.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Light"))
		{
			ImGui::SliderFloat("light_direction.x", &this->light_direction.x, -1.0f, 1.0f);
			ImGui::SliderFloat("light_direction.y", &this->light_direction.y, -1.0f, 1.0f);
			ImGui::SliderFloat("light_direction.z", &this->light_direction.z, -1.0f, 1.0f);
			ImGui::SliderFloat("light_direction.w", &this->light_direction.w, -1.0f, 1.0f);
			ImGui::TreePop();
		}


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


		//ここから攻撃判定調整
		static float save_timer = 0.0f;
		static float init_timer = 0.0f;
		static float load_timer = 0.0f;
		if (ImGui::Button(u8"セーブ"))
		{
			ImGui::OpenPopup("Save");
		}ImGui::SameLine();
		if (ImGui::BeginPopup("Save"))
		{
			ImGui::Text(u8"セーブしますか？");
			if (ImGui::Button(u8"はい"))
			{
				if (AttackWrite())
				{
					save_timer = 2.0f;
					ImGui::CloseCurrentPopup();
				}
			}
			if (ImGui::Button(u8"いいえ"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (pad->x_input[scastI(PAD::SELECT)] == 1)
		{
			ImGui::OpenPopup("Load");
		}

		if (ImGui::Button(u8"ロード"))
		{
			ImGui::OpenPopup("Load");
		}ImGui::SameLine();
		if (ImGui::BeginPopup("Load"))
		{
			ImGui::Text(u8"ロードしますか？");
			if (ImGui::Button(u8"はい") || pad->x_input[scastI(PAD::X)] == 1)
			{
				if (AttackLoad())
				{
					load_timer = 2.0f;
					ImGui::CloseCurrentPopup();
				}
			}
			if (ImGui::Button(u8"いいえ"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button(u8"パラメーター初期化"))
		{
			ImGui::OpenPopup("New");
		}
		if (ImGui::BeginPopup("New"))
		{
			ImGui::Text(u8"本当に初期化しますか？");
			if (ImGui::Button(u8"はい"))
			{
				if (AttackClean())
				{
					init_timer = 2.0f;
					ImGui::CloseCurrentPopup();
				}
			}
			if (ImGui::Button(u8"いいえ"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (save_timer > 0.0f)
		{
			ImGui::Text(u8"セーブしました");
			save_timer -= elapsed_time;
		}
		if (load_timer > 0.0f)
		{
			ImGui::Text(u8"ロードしました");
			load_timer -= elapsed_time;
		}
		if (init_timer > 0.0f)
		{
			ImGui::Text(u8"初期化しました");
			init_timer -= elapsed_time;
		}


		//攻撃判定を全て出す
		for (int list = 0; list < attack_list.size(); list++)
		{
			if (ImGui::TreeNode(attack_name_list[scastI(attack_list[list].attack_name)].c_str()))
			{
				int real = scastI(attack_list[list].real_attack);
				int pad = scastI(attack_list[list].linkage_button);
				ImGui::SliderInt(u8"ボタン", &pad, 0, scastI(PAD::PAD_END));
				attack_list[list].linkage_button = static_cast<PAD>(pad);
				ImGui::Text(pad_name_list[pad].c_str());
				int com = scastI(attack_list[list].linkage_command);
				ImGui::SliderInt(u8"コマンド", &com, 0, scastI(Command::LHURF));
				attack_list[list].linkage_command = static_cast<Command>(com);
				ImGui::Text(command_name_list[com].c_str());
				int ground = scastI(attack_list[list].ground_on);
				ImGui::SliderInt(u8"攻撃はどこで出せるか", &ground, 0, scastI(Ground_C::END) - 1);
				attack_list[list].ground_on = static_cast<Ground_C>(ground);
				ImGui::Text(ground_name_list[ground].c_str());
				ImGui::Checkbox(u8"しゃがみ攻撃", &attack_list[list].squat_on);

				if (attack_list[list].linkage_stick != PAD::BUTTOM_END)
				{
					linkage_stick_on[list] = true;
				}

				ImGui::Checkbox(u8"スティックの入力を必要とする", &linkage_stick_on[list]);
				if (linkage_stick_on[list])
				{
					int stick = scastI(attack_list[list].linkage_stick);
					ImGui::SliderInt(u8"スティック", &stick, scastI(PAD::STICK_R), scastI(PAD::STICK_D));
					ImGui::Text(pad_name_list[stick].c_str());
					attack_list[list].linkage_stick = static_cast<PAD>(stick);
				}
				else
				{
					attack_list[list].linkage_stick = PAD::BUTTOM_END;
				}
				ImGui::SliderInt(u8"攻撃内容", &real, 0, scastI(AttackState::ATTACK_END) - 1);
				attack_list[list].real_attack = static_cast<AttackState>(real);
				if (attack_list[list].attack_name != attack_list[list].real_attack)
				{
					ImGui::Text(attack_name_list[scastI(attack_list[list].real_attack)].c_str());
				}
				else
				{
					int next = scastI(attack_list[list].combo);
					int result = scastI(attack_list[list].conditions_hit);
					ImGui::InputFloat(u8"後スキ", &attack_list[list].later, 0.01f, 0.1f);
					ImGui::InputInt(u8"攻撃回数", &attack_list[list].attack_max, 1, 10);
					ImGui::InputFloat(u8"モーション速度:発生", &ac_attack[list].fream, 0.01f, 0.1f);
					ImGui::InputFloat(u8"モーション速度:持続", &ac_attack[list].timer, 0.01f, 0.1f);
					ImGui::InputFloat(u8"モーション速度:後スキ", &ac_attack[list].later, 0.01f, 0.1f);
					ImGui::InputFloat(u8"前進する距離", &attack_list[list].advance_speed, 0.01f, 0.1f);
					ImGui::SliderInt(u8"次の攻撃", &next, 0, scastI(AttackState::ATTACK_END) - 1);
					attack_list[list].combo = static_cast<AttackState>(next);
					ImGui::Text(attack_name_list[scastI(attack_list[list].combo)].c_str());
					ImGui::SliderInt(u8"攻撃遷移条件", &result, 0, scastI(HitResult::END) - 1);
					attack_list[list].conditions_hit = static_cast<HitResult>(result);
					ImGui::Text(result_name_list[scastI(attack_list[list].conditions_hit)].c_str());
					ImGui::Checkbox(u8"当たり判定にスピードを付与する", &attack_list[list].speed_on);
					if (attack_list[list].speed_on)
					{
						ImGui::InputFloat(u8"加算スピードX", &attack_list[list].speed.x, 0.01f, 0.1f);
						ImGui::InputFloat(u8"加算スピードY", &attack_list[list].speed.y, 0.01f, 0.1f);
						ImGui::InputFloat(u8"プレイヤーの持続フレーム", &attack_list[list].timer, 0.01f, 0.1f);
					}
					else
					{
						attack_list[list].speed = YR_Vector3(0.0f, 0.0f);
						attack_list[list].timer = 0.0f;
					}
					ImGui::InputFloat(u8"必要なゲージ量", &attack_list[list].need_gauge, 1.0f, 10.0f);
					if (attack_list[list].need_gauge >= 1.0f)
					{
						int aid_attack = scastI(attack_list[list].aid_attack_name);
						ImGui::SliderInt(u8"ゲージが足りない場合の攻撃", &aid_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
						ImGui::Text(attack_name_list[aid_attack].c_str());
						attack_list[list].aid_attack_name = static_cast<AttackState>(aid_attack);
					}
					else
					{
						attack_list[list].aid_attack_name = AttackState::NONE;
					}


					attack_list[list].attack_single.resize(attack_list[list].attack_max);
					//攻撃回数ごとのパラメータ表示
					ImGui::Text(u8"攻撃回数ごとのパラメータ");
					if (!attack_list[list].attack_single.empty())
					{
						for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
						{
							std::string now_sin = std::string("ATK_Single:") + std::to_string(sin);
							if (ImGui::TreeNode(now_sin.c_str()))
							{
								ImGui::InputFloat(u8"発生フレーム", &attack_list[list].attack_single[sin].fream, 0.01f, 0.1f);
								ImGui::InputInt(u8"当たり判定の数", &attack_list[list].attack_single[sin].quantity, 1, 10);
								attack_list[list].attack_single[sin].parameter.resize(attack_list[list].attack_single[sin].quantity);
								if (!attack_list[list].attack_single[sin].parameter.empty())
								{
									for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
									{
										//攻撃判定内部パラメーター表示
										std::string now_para = u8"判定";
										now_para += std::to_string(para);
										if (ImGui::TreeNode(now_para.c_str()))
										{
											ImGui::InputFloat(u8"プレイヤーとの距離X", &attack_list[list].attack_single[sin].parameter[para].distance.x, 0.01f, 0.1f);
											ImGui::InputFloat(u8"プレイヤーとの距離Y", &attack_list[list].attack_single[sin].parameter[para].distance.y, 0.01f, 0.1f);
											ImGui::InputFloat(u8"サイズX", &attack_list[list].attack_single[sin].parameter[para].size.x, 0.01f, 0.1f);
											ImGui::InputFloat(u8"サイズY", &attack_list[list].attack_single[sin].parameter[para].size.y, 0.01f, 0.1f);
											ImGui::InputFloat(u8"持続フレーム", &attack_list[list].attack_single[sin].parameter[para].timer, 0.01f, 0.1f);
											ImGui::InputFloat(u8"ダメージ", &attack_list[list].attack_single[sin].parameter[para].damege, 0.01f, 0.1f);
											ImGui::InputFloat(u8"のけぞり時間", &attack_list[list].attack_single[sin].parameter[para].HB_timer, 0.01f, 0.1f);
											ImGui::InputFloat(u8"吹っ飛びX", &attack_list[list].attack_single[sin].parameter[para].hitback.x, 0.01f, 0.1f);
											ImGui::InputFloat(u8"吹っ飛びY", &attack_list[list].attack_single[sin].parameter[para].hitback.y, 0.01f, 0.1f);
											int type = scastI(attack_list[list].attack_single[sin].parameter[para].type);
											ImGui::SliderInt(u8"攻撃タイプ", &type, 0, scastI(AttackKind::END)-1);
											attack_list[list].attack_single[sin].parameter[para].type = static_cast<AttackKind>(type);
											ImGui::Text(attack_kind_name_list[type].c_str());
											ImGui::InputFloat(u8"ノックバック(Xのみ)", &attack_list[list].attack_single[sin].parameter[para].knockback, 0.01f, 0.1f);
											ImGui::Checkbox(u8"ゲージを獲得しない", &attack_list[list].attack_single[sin].parameter[para].gaugeout);
											ImGui::InputFloat(u8"つかみ抜けされる時間", &attack_list[list].attack_single[sin].parameter[para].stealtimer, 0.01f, 0.1f);
											int hit_stop_time = scastI(attack_list[list].attack_single[sin].parameter[para].HS_timer);
											ImGui::SliderInt(u8"ヒットストップ時間", &hit_stop_time, 0, scastI(HitStopTime::END)-1);
											ImGui::Text(u8"0:短い。1:普通。2:長い。3:ズームストップ");
											attack_list[list].attack_single[sin].parameter[para].HS_timer = static_cast<HitStopTime>(hit_stop_time);
											if (!attack_list[list].attack_single[sin].parameter[para].gaugeout)
											{
												ImGui::InputFloat(u8"ゲージ獲得量", &attack_list[list].attack_single[sin].parameter[para].gauge_get, 0.01f, 0.1f);
											}
											ImGui::TreePop();
										}
									}
								}

								ImGui::TreePop();
							}
						}
					}
				}
				ImGui::TreePop();
			}
		}

		if (ImGui::TreeNode(u8"当たり判定"))
		{
			if (!atk.empty())
			{
				for (int i = 0; i < atk.size(); i++)
				{
					ImGui::InputFloat(".x", &atk[i].pos.x, 0.01f, 0.01f);
					ImGui::InputFloat(".x", &atk[i].pos.y, 0.01f, 0.01f);
				}
			}
			ImGui::TreePop();
		}
		ImGui::End();

		std::string now_com = std::to_string(now_player);
		now_com += std::string(":X_Combolist");
		ImGui::Begin(now_com.c_str());

		static AttackState at_state = AttackState::NONE;
		int com_attack = scastI(at_state);
		ImGui::SliderInt(u8"追加するコンボ攻撃", &com_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
		ImGui::Text(attack_name_list[com_attack].c_str());
		at_state = static_cast<AttackState>(com_attack);

		if (ImGui::Button(u8"コンボ追加"))
		{
			combolist_X.combolist.push_back(AttackState());
			combolist_X.combolist.back() = at_state;
		}ImGui::SameLine();
		if (ImGui::Button(u8"コンボ削除"))
		{
			if (!combolist_X.combolist.empty())
			{
				int size = combolist_X.combolist.size() - 1;
				combolist_X.combolist.resize(size);
			}
		}

		if (!combolist_X.combolist.empty())
		{
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
			for (int i = 0; i < combolist_X.combolist.size(); ++i) {
				ImGui::Text(attack_name_list[scastI(combolist_X.combolist[i])].c_str());
			}
			ImGui::EndChild();
		}
		

		ImGui::End();
	}
	/*{
		std::string now = std::to_string(now_player);
		now += std::string("KnightAtkList");
		ImGui::Begin(now.c_str());

		ImGui::Text(u8"0:攻撃無し\n1:弱\n2:中\n3:強(対空&下強)\n4:下弱");
		ImGui::Text(u8"5:下中\n6:上強\n7:空弱攻撃\n8:空中攻撃");
		ImGui::Text(u8"9:空強攻撃\n10:空上強攻撃(打ち上げ攻撃)");
		ImGui::Text(u8"11:つかみ\n12:投げ\n13:ホーミングダッシュ");
		ImGui::Text(u8"14:前弱必殺\n15:前中必殺\n16:前強必殺\n17:後弱必殺\n18:後中必殺");
		ImGui::Text(u8"19:後強必殺\n20:前超必殺\n21:後超必殺\n22:即死技\n23:無敵攻撃");
		ImGui::End();
	}*/

#endif // USE_IMGUI
}