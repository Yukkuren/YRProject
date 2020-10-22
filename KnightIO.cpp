#include "Knight.h"
#include "PlayerBase.h"
#include "framework.h"
#include <fstream>

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
	attack_list[scastI(AttackState::JAKU)].ground_on = true;
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
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].type = AttackBox::MIDDLE;
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
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].ground_on = true;
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
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].type = AttackBox::MIDDLE;
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
		hitparam_list[scastI(KNIGHTHIT::BODY)].act_parameter[list].state = HitBoxState::NOGUARD;
	}
	for (int list = 0; list < hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter.size(); list++)
	{
		hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter[list].distance = YR_Vector3(0.0f, 0.0f);
		hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter[list].size = YR_Vector3(1.4f, 0.8f);
		hitparam_list[scastI(KNIGHTHIT::LEG)].act_parameter[list].state = HitBoxState::NOGUARD;
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
		int pad,com,stick,aid,real;
		ifs >> pad;
		ifs >> com;
		attack_list[list].linkage_button = static_cast<PAD>(pad);
		attack_list[list].linkage_command = static_cast<Command>(com);
		ifs >> attack_list[list].ground_on;
		ifs >> attack_list[list].squat_on;
		ifs >> attack_list[list].need_gauge;
		ifs >> stick;
		ifs >> aid;
		ifs >> real;
		attack_list[list].linkage_stick = static_cast<PAD>(stick);
		attack_list[list].aid_attack_name = static_cast<AttackState>(aid);
		attack_list[list].real_attack = static_cast<AttackState>(real);
		attack_list[list].attack_single.resize(attack_list[list].attack_max);


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
						ifs >> attack_list[list].attack_single[sin].parameter[para].type;
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
			int state;
			hit_ifs >> state;
			hitparam_list[list].act_parameter[act].state = static_cast<HitBoxState>(state);
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			//攻撃中当たり判定読み込み
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.y;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.y;
			int state;
			hit_ifs >> state;
			hitparam_list[list].attack_parameter[atk].state = static_cast<HitBoxState>(state);
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
		attack_list[list].ground_on = true;
		attack_list[list].squat_on = false;
		attack_list[list].need_gauge = 0.0f;
		attack_list[list].linkage_stick = PAD::BUTTOM_END;
		attack_list[list].aid_attack_name = AttackState::NONE;
		attack_list[list].attack_name = static_cast<AttackState>(list);
		attack_list[list].real_attack = attack_list[list].attack_name;
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
				attack_list[list].attack_single[sin].parameter[para].type = 1;
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
			hitparam_list[list].act_parameter[act].state = HitBoxState::NOGUARD;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			hitparam_list[list].attack_parameter[atk].distance = YR_Vector3(0.0f, 0.0f);
			hitparam_list[list].attack_parameter[atk].size = YR_Vector3(1.0f, 1.0f);
			hitparam_list[list].attack_parameter[atk].state = HitBoxState::NOGUARD;
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
		outputfile << attack_list[list].ground_on << std::endl;
		outputfile << attack_list[list].squat_on << std::endl;
		outputfile << attack_list[list].need_gauge << std::endl;
		outputfile << scastI(attack_list[list].linkage_stick) << std::endl;
		outputfile << scastI(attack_list[list].aid_attack_name) << std::endl;
		outputfile << scastI(attack_list[list].real_attack) << std::endl;

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
						outputfile << attack_list[list].attack_single[sin].parameter[para].type << std::endl;
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
			hitout << scastI(hitparam_list[list].act_parameter[act].state) << std::endl;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			//攻撃中当たり判定書き込み
			hitout << hitparam_list[list].attack_parameter[atk].distance.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].distance.y << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.y << std::endl;
			hitout << scastI(hitparam_list[list].attack_parameter[atk].state) << std::endl;
		}
	}
	hitout.close();

	return true;
}