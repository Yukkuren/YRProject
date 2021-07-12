#include "PlayerBase.h"
#include "../../framework.h"
#include <fstream>
#include "Player_name_list.h"

//-------------------------------------------------------------------------------------
//							プレイヤーステータス調整用cpp
//-------------------------------------------------------------------------------------
//・キャラごとのステータス(ダッシュ速度など)の調整を行い、データのロード、書き出しを行っている
//-------------------------------------------------------------------------------------




//テキストファイルから読み込み
void CharaStateParameter::Load(PLSELECT chara_name)
{
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara = GetName().chara_name_list[scastI(chara_name)];

	std::string Chara_level = f_level + chara + std::string("/CharaState.txt");
	std::ifstream ifs(Chara_level);

	ifs >> walkspeed;
	ifs >> dashspeed;
	ifs >> backstepS;
	ifs >> backstepD;
	ifs >> stepspeed;
	ifs >> stepD;
	ifs >> jump_max;
	ifs >> high_jump_max;
	ifs >> jump_speed;
	ifs >> high_jump_speed;
	ifs >> brake_speed;
	ifs >> track_speed;
	ifs >> passive_speed.x;
	ifs >> passive_speed.y;
	ifs >> passive_brake;
	ifs >> max_hp;

	//もし落ちたらエラーを出す
	if (ifs.fail())
	{
		assert("ifs failled");
	}

	ifs.close();
}


//テキストファイルに書き出し
void CharaStateParameter::Save(PLSELECT chara_name)
{
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara = GetName().chara_name_list[scastI(chara_name)];

	std::string Chara_level = f_level + chara + std::string("/CharaState.txt");
	std::ofstream outputfile(Chara_level);

	outputfile << walkspeed << std::endl;
	outputfile << dashspeed << std::endl;
	outputfile << backstepS << std::endl;
	outputfile << backstepD << std::endl;
	outputfile << stepspeed << std::endl;
	outputfile << stepD << std::endl;
	outputfile << jump_max << std::endl;
	outputfile << high_jump_max << std::endl;
	outputfile << jump_speed << std::endl;
	outputfile << high_jump_speed << std::endl;
	outputfile << brake_speed << std::endl;
	outputfile << track_speed << std::endl;
	outputfile << passive_speed.x << std::endl;
	outputfile << passive_speed.y << std::endl;
	outputfile << passive_brake << std::endl;
	outputfile << max_hp << std::endl;

	outputfile.close();
}


//デバッグ用ツール描画
void CharaStateParameter::Draw()
{
#ifdef EXIST_IMGUI

	if (Get_Use_ImGui())
	{
		if (ImGui::TreeNode(u8"ステータス調整"))
		{
			ImGui::SliderFloat(u8"後退速度", &walkspeed, 0.0f, 100.0f);
			ImGui::SliderFloat(u8"ダッシュスピード", &dashspeed, 0.0f, 100.0f);
			ImGui::SliderFloat(u8"バックステップ速度", &backstepS, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"バックステップブレーキ", &backstepD, 0.0f, 800.0f);
			ImGui::SliderFloat(u8"ステップ速度", &stepspeed, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"ステップブレーキ", &stepD, 0.0f, 800.0f);
			ImGui::SliderFloat(u8"ジャンプ初速度", &jump_speed, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"ジャンプ最大速度", &jump_max, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"ハイジャンプ最大速度", &high_jump_speed, 0.0f, 200.0f);
			ImGui::SliderFloat(u8"ハイジャンプ最大速度", &high_jump_max, 0.0f, 200.0f);
			ImGui::SliderFloat(u8"ダッシュブレーキ", &brake_speed, 0.0f, 20000.0f);
			ImGui::SliderFloat(u8"ホーミングダッシュ速度", &track_speed, 0.0f, 200.0f);
			ImGui::SliderFloat(u8"受け身初速度X", &passive_speed.x, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"受け身初速度Y", &passive_speed.y, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"受け身ブレーキ", &passive_brake, 0.0f, 500.0f);
			ImGui::SliderFloat(u8"最大HP", &max_hp, 0.0f, 3000.0f);
			ImGui::TreePop();
		}
	}

#endif // EXIST_IMGUI

}








//-------------------------------------------------------------------------------------
//							キャラステータス調整
//-------------------------------------------------------------------------------------
//・キャラごとの当たり判定などの調整を行い、データのロード、書き出しを行っている
//-------------------------------------------------------------------------------------


//書き出されたテキストデータから攻撃リストを生成する
bool Player::AttackLoad()
{
	attack_list.resize(scastI(AttackState::ATTACK_END));

	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name = GetName().chara_name_list[scastI(this->chara_name)];

	std::string AP_level = f_level + chara_name + std::string("/AttackParam.txt");
	std::ifstream ifs(AP_level);

	ifs >> jump_later;
	ifs >> dash_later;

	for (int list = 0; list < attack_list.size(); list++)
	{
		/*if (list == scastI(AttackState::BURST)|| list == scastI(AttackState::COMBOBURST))
		{
			continue;
		}*/

		attack_list[list].attack_name = static_cast<AttackState>(list);
		ifs >> attack_list[list].later;
		ifs >> attack_list[list].attack_max;
		int pad, com, stick, aid, real, next, result, ground, effect_k, anim_k, func_n;
		EffectParameter e_param;
		ifs >> pad;
		ifs >> com;
		attack_list[list].linkage_button = static_cast<PAD>(pad);
		attack_list[list].linkage_command = static_cast<Command>(com);
		ifs >> ground;
		attack_list[list].ground_on = static_cast<Ground_C>(ground);
		ifs >> attack_list[list].squat_on;
		ifs >> attack_list[list].need_power;
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
		ifs >> attack_list[list].traject_on;
		ifs >> anim_k;
		ifs >> func_n;
		attack_list[list].anim_kind = static_cast<AttackState>(anim_k);
		attack_list[list].function_num = static_cast<AT_Function_List>(func_n);


		ifs >> effect_k;
		attack_list[list].effect_param.effect_kind = static_cast<EffectKind>(effect_k);
		ifs >> attack_list[list].effect_param.distance.x;
		ifs >> attack_list[list].effect_param.distance.y;
		ifs >> attack_list[list].effect_param.distance.z;
		ifs >> attack_list[list].effect_param.scale.x;
		ifs >> attack_list[list].effect_param.scale.y;
		ifs >> attack_list[list].effect_param.scale.z;
		ifs >> attack_list[list].effect_param.axis.x;
		ifs >> attack_list[list].effect_param.axis.y;
		ifs >> attack_list[list].effect_param.axis.z;
		ifs >> attack_list[list].effect_param.angle;
		ifs >> attack_list[list].effect_param.rightORleft;

		/*ifs >> effect_k;
		e_param.effect_kind = static_cast<EffectKind>(effect_k);
		ifs >> e_param.distance.x;
		ifs >> e_param.distance.y;
		ifs >> e_param.distance.z;
		ifs >> e_param.scale.x;
		ifs >> e_param.scale.y;
		ifs >> e_param.scale.z;
		ifs >> e_param.axis.x;
		ifs >> e_param.axis.y;
		ifs >> e_param.axis.z;
		ifs >> e_param.angle;
		ifs >> e_param.rightORleft;*/


		//攻撃回数ごとのパラメータ読み込み
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
						//攻撃判定内部パラメーター読み込み
						ifs >> attack_list[list].attack_single[sin].parameter[para].distance.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].distance.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].size.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].size.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.damage;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.HB_timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.hitback.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.hitback.y;
						int type = 0;
						ifs >> type;
						attack_list[list].attack_single[sin].parameter[para].type = static_cast<AttackKind>(type);
						ifs >> attack_list[list].attack_single[sin].parameter[para].knockback;
						ifs >> attack_list[list].attack_single[sin].parameter[para].gaugeout;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.steal_timer;
						int hs;
						ifs >> hs;
						attack_list[list].attack_single[sin].parameter[para].HS_timer = static_cast<HitStopTime>(hs);
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_back.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_back.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_shaving;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.combo_correction;
						ifs >> attack_list[list].attack_single[sin].parameter[para].gauge_get;

						ifs >> effect_k;
						attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind = static_cast<EffectKind>(effect_k);
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.distance.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.distance.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.distance.z;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.axis.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.axis.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.axis.z;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.angle;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.rightORleft;

						//attack_list[list].attack_single[sin].parameter[para].effect_param = e_param;

					}
				}
			}
		}
	}
	//もし落ちたらエラーを出す
	if (ifs.fail())
	{
		assert("ifs failled");
	}
	//ファイルを閉じる
	ifs.close();

	//アニメーション調整値を読み込む
	std::string Anim_level = f_level + chara_name + std::string("/AnimParam.txt");
	std::ifstream anim_ifs(Anim_level);
	for (int list = 0; list < ac_attack.size(); list++)
	{
		/*if (list == scastI(AttackState::BURST) || list == scastI(AttackState::COMBOBURST))
		{
			continue;
		}*/
		anim_ifs >> ac_attack[list].fream;
		anim_ifs >> ac_attack[list].timer;
		anim_ifs >> ac_attack[list].later;
	}
	//もし落ちたらエラーを出す
	if (anim_ifs.fail())
	{
		assert("anim_ifs failled");
	}
	//ファイルを閉じる
	anim_ifs.close();



	//アニメーション調整値を読み込む
	std::string Anim_Act_level = f_level + chara_name + std::string("/AnimActParam.txt");
	std::ifstream anim_act_ifs(Anim_Act_level);
	for (int list = 0; list < ac_act.size(); list++)
	{
		anim_act_ifs >> ac_act[list].fream;
		anim_act_ifs >> ac_act[list].timer;
		anim_act_ifs >> ac_act[list].later;
	}
	//もし落ちたらエラーを出す
	if (anim_act_ifs.fail())
	{
		assert("anim_act_ifs failled");
	}
	//ファイルを閉じる
	anim_act_ifs.close();


	//Xボタンコンボリストを読み込む
	std::string ComboX_level = f_level + chara_name + std::string("/Combo_X.txt");
	std::ifstream combo_X_ifs(ComboX_level);
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
		assert("combo_X_ifs failled");
	}
	//ファイルを閉じる
	combo_X_ifs.close();


	//Yボタンコンボリストを読み込む
	std::string ComboY_level = f_level + chara_name + std::string("/Combo_Y.txt");
	std::ifstream combo_Y_ifs(ComboY_level);
	int size_y = -1;
	int  com_y = 0;
	combo_Y_ifs >> size_y;
	if (size_y > 0)
	{
		combolist_Y.combolist.resize(size_y);

		if (!combolist_Y.combolist.empty())
		{
			for (int list = 0; list < combolist_Y.combolist.size(); list++)
			{
				combo_Y_ifs >> com_y;
				combolist_Y.combolist[list] = static_cast<AttackState>(com_y);
			}
		}
	}
	//もし落ちたらエラーを出す
	if (combo_Y_ifs.fail())
	{
		assert("combo_Y_ifs failled");
	}
	//ファイルを閉じる
	combo_Y_ifs.close();


	//Bボタンコンボリストを読み込む
	std::string ComboB_level = f_level + chara_name + std::string("/Combo_B.txt");
	std::ifstream combo_B_ifs(ComboB_level);
	int size_b = -1;
	int  com_b = 0;
	combo_B_ifs >> size_b;
	if (size_b > 0)
	{
		combolist_B.combolist.resize(size_b);

		if (!combolist_B.combolist.empty())
		{
			for (int list = 0; list < combolist_B.combolist.size(); list++)
			{
				combo_B_ifs >> com_b;
				combolist_B.combolist[list] = static_cast<AttackState>(com_b);
			}
		}
	}
	//もし落ちたらエラーを出す
	if (combo_B_ifs.fail())
	{
		assert("combo_B_ifs failled");
	}
	//ファイルを閉じる
	combo_B_ifs.close();


	//当たり判定調整値を読み込む
	std::string Hit_level = f_level + chara_name + std::string("/HitParam.txt");
	std::ifstream hit_ifs(Hit_level);
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			//行動中当たり判定読み込み
			hit_ifs >> hitparam_list[list].act_parameter[act].distance.x;
			hit_ifs >> hitparam_list[list].act_parameter[act].distance.y;
			hit_ifs >> hitparam_list[list].act_parameter[act].size.x;
			hit_ifs >> hitparam_list[list].act_parameter[act].size.y;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			/*if (atk == scastI(AttackState::BURST) || atk == scastI(AttackState::COMBOBURST))
			{
				continue;
			}*/
			//攻撃中当たり判定読み込み
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.y;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.y;
		}
	}
	//もし落ちたらエラーを出す
	if (hit_ifs.fail())
	{
		assert("hit_ifs failled");
	}
	//ファイルを閉じる
	hit_ifs.close();

	//キャラステータスを読み込む
	chara_state.Load(this->chara_name);

	return true;
}




//テキストデータを書き出す
bool Player::AttackWrite()
{
	//AttackStateの順に保存する
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name = GetName().chara_name_list[scastI(this->chara_name)];

	std::string AP_level = f_level + chara_name + std::string("/AttackParam.txt");
	std::ofstream outputfile(AP_level);

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
		outputfile << attack_list[list].need_power << std::endl;
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
		outputfile << attack_list[list].traject_on << std::endl;

		outputfile << scastI(attack_list[list].anim_kind) << std::endl;
		outputfile << scastI(attack_list[list].function_num) << std::endl;

		//攻撃リストのエフェクト調整値
		outputfile << scastI(attack_list[list].effect_param.effect_kind) << std::endl;
		outputfile << attack_list[list].effect_param.distance.x << std::endl;
		outputfile << attack_list[list].effect_param.distance.y << std::endl;
		outputfile << attack_list[list].effect_param.distance.z << std::endl;
		outputfile << attack_list[list].effect_param.scale.x << std::endl;
		outputfile << attack_list[list].effect_param.scale.y << std::endl;
		outputfile << attack_list[list].effect_param.scale.z << std::endl;
		outputfile << attack_list[list].effect_param.axis.x << std::endl;
		outputfile << attack_list[list].effect_param.axis.y << std::endl;
		outputfile << attack_list[list].effect_param.axis.z << std::endl;
		outputfile << attack_list[list].effect_param.angle << std::endl;
		outputfile << attack_list[list].effect_param.rightORleft << std::endl;

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
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.damage << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.HB_timer << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.hitback.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.hitback.y << std::endl;
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].type) << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].knockback << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].gaugeout << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.steal_timer << std::endl;
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].HS_timer) << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_back.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_back.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_shaving << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_timer << std::endl;

						outputfile << attack_list[list].attack_single[sin].parameter[para].param.combo_correction << std::endl;
						if (!attack_list[list].attack_single[sin].parameter[para].gaugeout)
						{
							outputfile << attack_list[list].attack_single[sin].parameter[para].gauge_get << std::endl;
						}
						else
						{
							outputfile << float(0.0) << std::endl;
						}


						//エフェクト調整値書き出し
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind) << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.distance.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.distance.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.distance.z << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.axis.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.axis.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.axis.z << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.angle << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.rightORleft << std::endl;

					}
				}
			}
		}
	}

	outputfile.close();

	//アニメーション調整値も書き出す
	std::string Anim_level = f_level + chara_name + std::string("/AnimParam.txt");
	std::ofstream animout(Anim_level);
	for (int list = 0; list < ac_attack.size(); list++)
	{
		animout << ac_attack[list].fream << std::endl;
		animout << ac_attack[list].timer << std::endl;
		animout << ac_attack[list].later << std::endl;
	}

	animout.close();

	std::string Anim_Act_level = f_level + chara_name + std::string("/AnimActParam.txt");
	std::ofstream animactout(Anim_Act_level);
	for (int list = 0; list < ac_act.size(); list++)
	{
		animactout << ac_act[list].fream << std::endl;
		animactout << ac_act[list].timer << std::endl;
		animactout << ac_act[list].later << std::endl;
	}

	animactout.close();

	//当たり判定調整値も書き出す
	std::string Hit_level = f_level + chara_name + std::string("/HitParam.txt");
	std::ofstream hitout(Hit_level);
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			//行動中当たり判定書き込み
			hitout << hitparam_list[list].act_parameter[act].distance.x << std::endl;
			hitout << hitparam_list[list].act_parameter[act].distance.y << std::endl;
			hitout << hitparam_list[list].act_parameter[act].size.x << std::endl;
			hitout << hitparam_list[list].act_parameter[act].size.y << std::endl;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			//攻撃中当たり判定書き込み
			hitout << hitparam_list[list].attack_parameter[atk].distance.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].distance.y << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.y << std::endl;
		}
	}
	hitout.close();

	std::string ComboX_level = f_level + chara_name + std::string("/Combo_X.txt");
	std::ofstream combo_X_out(ComboX_level);
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


	std::string ComboY_level = f_level + chara_name + std::string("/Combo_Y.txt");
	std::ofstream combo_Y_out(ComboY_level);
	if (!combolist_Y.combolist.empty())
	{
		combo_Y_out << combolist_Y.combolist.size() << std::endl;
		for (int list = 0; list < combolist_Y.combolist.size(); list++)
		{
			//Yボタンコンボリスト書き込み
			combo_Y_out << scastI(combolist_Y.combolist[list]) << std::endl;
		}
	}
	combo_Y_out.close();


	std::string ComboB_level = f_level + chara_name + std::string("/Combo_B.txt");
	std::ofstream combo_B_out(ComboB_level);
	if (!combolist_B.combolist.empty())
	{
		combo_B_out << combolist_B.combolist.size() << std::endl;
		for (int list = 0; list < combolist_B.combolist.size(); list++)
		{
			//Bボタンコンボリスト書き込み
			combo_B_out << scastI(combolist_B.combolist[list]) << std::endl;
		}
	}
	combo_B_out.close();

	//キャラステータスを書き出す
	chara_state.Save(this->chara_name);

	return true;
}




//パラメーターを全て初期化する
bool Player::AttackClean()
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
		attack_list[list].need_power = 0;
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
		attack_list[list].traject_on = true;
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
				attack_list[list].attack_single[sin].parameter[para].param.damage = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].param.HB_timer = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].param.hitback.x = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].param.hitback.y = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].type = AttackKind::MIDDLE;
				attack_list[list].attack_single[sin].parameter[para].knockback = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].gaugeout = false;
				attack_list[list].attack_single[sin].parameter[para].param.steal_timer = 0.1f;
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




//デバッグ用ツール表示
void Player::DrawDEBUG(
	YRShader* geoshader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	//Debug表示関数

#ifdef EXIST_IMGUI
	if (Get_Debug_Draw())
	{

		for (int i = 0; i < hit.size(); i++)
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
	}

	//攻撃リスト作成用処理
	if (Get_Use_ImGui())
	{

		std::string p1_hp = std::to_string(hp);

		FRAMEWORK.font->Begin(FRAMEWORK.context.Get());
		if (now_player > 1)
		{
			FRAMEWORK.font->Draw(
				static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f) + 200.0f,
				100.0f, p1_hp.c_str());
		}
		else
		{
			FRAMEWORK.font->Draw(
				static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f) - 500.0f,
				100.0f, p1_hp.c_str());
		}

		FRAMEWORK.font->End(FRAMEWORK.context.Get());

		std::string now_play = std::to_string(now_player);
		now_play += std::string("P : ") + GetName().chara_name_list[scastI(this->chara_name)];
		ImGui::Begin(now_play.c_str());


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
		/*if (pad->x_input[scastI(PAD::SELECT)] == 1)
		{
			ImGui::OpenPopup("Load");
		}*/

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

		if (ImGui::BeginTabBar(u8"設定用ImGui",ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
		{
			if (ImGui::BeginTabItem(u8"キャラステータス"))
			{
				if (ImGui::BeginTabBar(u8"キャラステータスTabBar", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
				{
					if (ImGui::BeginTabItem(u8"ステータス基本情報"))
					{
						ImGui::Text(u8"行動ステート");
						ImGui::Text(GetName().act_name_list[scastI(act_state)].c_str());
						ImGui::Text(u8"攻撃ステート");
						ImGui::Text(GetName().attack_name_list[scastI(attack_state)].c_str());

						ImGui::SliderFloat(u8"ジャンプの着地スキ", &jump_later, 0.0f, 2.0f);
						ImGui::SliderFloat(u8"ダッシュの着地スキ", &dash_later, 0.0f, 2.0f);
						ImGui::Text("fream : "); ImGui::SameLine(); ImGui::Text("%f", fream);
						ImGui::Text("timer : "); ImGui::SameLine(); ImGui::Text("%f", timer);
						ImGui::Text("later : "); ImGui::SameLine(); ImGui::Text("%f", later);
						ImGui::Text("knocktimer : "); ImGui::SameLine(); ImGui::Text("%f", knocktimer);


						//ImGui::Text("later : "); ImGui::SameLine(); ImGui::Text("%f", later);



						chara_state.Draw();

						//ImGui::InputFloat("eye_offset.x", &eye_offset.x, 0.01f, 0.01f);
						//ImGui::InputFloat("eye_offset.y", &eye_offset.y, 0.01f, 0.01f);
						//ImGui::InputFloat("mouse_offset.x", &mouth_offset.x, 0.01f, 0.01f);
						//ImGui::InputFloat("mouse_offset.y", &mouth_offset.y, 0.01f, 0.01f);
						ImGui::Text("player.x:%f", pos.x);
						ImGui::Text("player.y:%f", pos.y);
						ImGui::Text("command_timer:%f", pad->com_list.command_timer);
						ImGui::Text("track:%d", trackgauge);
						ImGui::Text("hitState : "); ImGui::SameLine();
						ImGui::Text(GetName().hitstate_name_list[scastI(hit[0].state)].c_str());
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

						ImGui::Text(u8"向いている方向："); ImGui::SameLine();
						if (rightOrleft > 0)
						{
							ImGui::Text(u8"右");
						}
						else
						{
							ImGui::Text(u8"左");
						}

						/*if (ImGui::BeginTabBar("testBar"))
						{
							for (int i = 0; i < 100; i++)
							{
								if (ImGui::BeginTabItem(std::to_string(i).data()))
								{
									{
										ImGui::Text(std::to_string(i).data());
										ImGui::EndTabItem();
									}
								}
							}
							ImGui::EndTabBar();
						}*/

						ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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
						ImGui::EndTabItem();
					}
					//当たり判定調整関数
					DrawDEBUGHitParam();
					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}



			//攻撃判定を全て出す
			int at_num_tab = -1;
			for (int list = 0; list < attack_list.size(); list++)
			{
				std::string tab_num = std::string("attack_");

				if (list % 5 == 0)
				{
					//5個づつ分けてタブ化する
					at_num_tab++;
				}

				tab_num += std::to_string(at_num_tab);

				if (ImGui::BeginTabItem(tab_num.data()))
				{
					if (ImGui::BeginTabBar(u8"判定ごと", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
					{
						if (ImGui::BeginTabItem(GetName().attack_name_list[list].c_str()))
						{
							if (ImGui::BeginTabBar(u8"攻撃内容", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
							{
								bool real_attack_not_now = false;
								if (ImGui::BeginTabItem(u8"基本情報"))
								{
									int real = scastI(attack_list[list].real_attack);
									int pad = scastI(attack_list[list].linkage_button);
									ImGui::SliderInt(u8"ボタン", &pad, 0, scastI(PAD::PAD_END)-1);
									attack_list[list].linkage_button = static_cast<PAD>(pad);
									ImGui::Text(GetName().pad_name_list[pad].c_str());
									int com = scastI(attack_list[list].linkage_command);
									ImGui::SliderInt(u8"コマンド", &com, 0, scastI(Command::END)-1);
									attack_list[list].linkage_command = static_cast<Command>(com);
									ImGui::Text(GetName().command_name_list[com].c_str());
									int ground = scastI(attack_list[list].ground_on);
									ImGui::SliderInt(u8"攻撃はどこで出せるか", &ground, 0, scastI(Ground_C::END) - 1);
									attack_list[list].ground_on = static_cast<Ground_C>(ground);
									ImGui::Text(GetName().ground_name_list[ground].c_str());
									int next = scastI(attack_list[list].combo);
									ImGui::SliderInt(u8"次の攻撃", &next, 0, scastI(AttackState::ATTACK_END) - 1);
									attack_list[list].combo = static_cast<AttackState>(next);
									ImGui::Text(GetName().attack_name_list[scastI(attack_list[list].combo)].c_str());
									ImGui::Checkbox(u8"しゃがみ攻撃", &attack_list[list].squat_on);
									ImGui::Checkbox(u8"剣の軌跡を表示する", &attack_list[list].traject_on);

									int anim_k = scastI(attack_list[list].anim_kind);
									ImGui::SliderInt(u8"使用アニメーション", &anim_k, 0, scastI(AttackState::NORMAL_ATTACK_END) - 1);
									attack_list[list].anim_kind = static_cast<AttackState>(anim_k);
									ImGui::Text(GetName().attack_name_list[anim_k].c_str());

									int function_n = scastI(attack_list[list].function_num);
									ImGui::SliderInt(u8"使用する関数", &function_n, 0, scastI(AT_Function_List::AT_END) - 1);
									attack_list[list].function_num = static_cast<AT_Function_List>(function_n);
									ImGui::Text(GetName().function_name_list[function_n].c_str());

									if (attack_list[list].linkage_stick != PAD::BUTTOM_END)
									{
										GetName().linkage_stick_on[list] = true;
									}

									ImGui::Checkbox(u8"スティックの入力を必要とする", &GetName().linkage_stick_on[list]);
									if (GetName().linkage_stick_on[list])
									{
										int stick = scastI(attack_list[list].linkage_stick);
										ImGui::SliderInt(u8"スティック", &stick, scastI(PAD::STICK_R), scastI(PAD::STICK_D));
										ImGui::Text(GetName().pad_name_list[stick].c_str());
										attack_list[list].linkage_stick = static_cast<PAD>(stick);
									}
									else
									{
										attack_list[list].linkage_stick = PAD::BUTTOM_END;
									}

									int effect_k_h = scastI(attack_list[list].effect_param.effect_kind);
									ImGui::SliderInt(u8"エフェクトの種類", &effect_k_h, 0, scastI(EffectKind::END) - 1);
									ImGui::Text(GetName().effect_kind_name_list[scastI(attack_list[list].effect_param.effect_kind)].c_str());
									attack_list[list].effect_param.effect_kind = static_cast<EffectKind>(effect_k_h);

									if (attack_list[list].effect_param.effect_kind != EffectKind::NONE)
									{
										if (ImGui::TreeNode(u8"エフェクト詳細設定"))
										{
											ImGui::SliderFloat(u8"座標X", &attack_list[list].effect_param.distance.x, -50.0f, 50.0f);
											ImGui::SliderFloat(u8"座標Y", &attack_list[list].effect_param.distance.y, -50.0f, 50.0f);
											ImGui::SliderFloat(u8"座標Z", &attack_list[list].effect_param.distance.z, -50.0f, 50.0f);
											//float scale_all = attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x;
											//ImGui::SliderFloat(u8"大きさ全体", &scale_all, 0.0f, 50.0f);
											ImGui::SliderFloat(u8"大きさX", &attack_list[list].effect_param.scale.x, 0.0f, 50.0f);
											ImGui::SliderFloat(u8"大きさY", &attack_list[list].effect_param.scale.y, 0.0f, 50.0f);
											ImGui::SliderFloat(u8"大きさZ", &attack_list[list].effect_param.scale.z, 0.0f, 50.0f);
											/*if (attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z)
											{
												attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x = scale_all;
												attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y = scale_all;
												attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z = scale_all;
											}*/
											ImGui::SliderFloat(u8"方向X", &attack_list[list].effect_param.axis.x, 0.0, 1.0f);
											ImGui::SliderFloat(u8"方向Y", &attack_list[list].effect_param.axis.y, 0.0, 1.0f);
											ImGui::SliderFloat(u8"方向Z", &attack_list[list].effect_param.axis.z, 0.0, 1.0f);
											ImGui::SliderFloat(u8"角度", &attack_list[list].effect_param.angle, -360.0f, 360.0f);
											ImGui::Checkbox(u8"角度をプレイヤー依存にするか", &attack_list[list].effect_param.rightORleft);

											ImGui::TreePop();
										}
									}

									ImGui::SliderInt(u8"攻撃内容", &real, 0, scastI(AttackState::ATTACK_END) - 1);
									attack_list[list].real_attack = static_cast<AttackState>(real);


									if (attack_list[list].attack_name != attack_list[list].real_attack)
									{
										ImGui::Text(GetName().attack_name_list[scastI(attack_list[list].real_attack)].c_str());
										real_attack_not_now = true;
									}
									else
									{
										int result = scastI(attack_list[list].conditions_hit);
										ImGui::InputFloat(u8"後スキ", &attack_list[list].later, 0.01f, 0.1f);
										ImGui::InputInt(u8"攻撃回数", &attack_list[list].attack_max, 1, 10);
										ImGui::InputFloat(u8"モーション速度:発生", &ac_attack[list].fream, 0.01f, 0.1f);
										ImGui::InputFloat(u8"モーション速度:持続", &ac_attack[list].timer, 0.01f, 0.1f);
										ImGui::InputFloat(u8"モーション速度:後スキ", &ac_attack[list].later, 0.01f, 0.1f);
										ImGui::InputFloat(u8"前進する距離", &attack_list[list].advance_speed, 0.01f, 0.1f);
										ImGui::SliderInt(u8"攻撃遷移条件", &result, 0, scastI(HitResult::END) - 1);
										attack_list[list].conditions_hit = static_cast<HitResult>(result);
										ImGui::Text(GetName().result_name_list[scastI(attack_list[list].conditions_hit)].c_str());
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
										ImGui::InputInt(u8"必要なゲージ量", &attack_list[list].need_power, 1.0f, 10.0f);
										if (attack_list[list].need_power >= 1)
										{
											int aid_attack = scastI(attack_list[list].aid_attack_name);
											ImGui::SliderInt(u8"ゲージが足りない場合の攻撃", &aid_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
											ImGui::Text(GetName().attack_name_list[aid_attack].c_str());
											attack_list[list].aid_attack_name = static_cast<AttackState>(aid_attack);
										}
										else
										{
											attack_list[list].aid_attack_name = AttackState::NONE;
										}


										attack_list[list].attack_single.resize(attack_list[list].attack_max);

										real_attack_not_now = false;
									}
									ImGui::EndTabItem();
								}

								if(!real_attack_not_now)
								{
									//攻撃回数ごとのパラメータ表示
									if (!attack_list[list].attack_single.empty())
									{
										if (ImGui::Button(u8"攻撃内容複製"))
										{
											AttackSingle back = attack_list[list].attack_single.back();
											attack_list[list].attack_single.push_back(back);
											attack_list[list].attack_max++;
										}ImGui::SameLine();
										if (ImGui::Button(u8"攻撃内容最後尾削除"))
										{
											int size = attack_list[list].attack_single.size() - 1;
											attack_list[list].attack_single.resize(size);
											attack_list[list].attack_max--;
										}
										for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
										{
											std::string now_sin = std::string("ATK_Single:") + std::to_string(sin);
											if (ImGui::BeginTabItem(now_sin.c_str()))
											{
												ImGui::Text(u8"攻撃回数ごとのパラメータ");
												ImGui::InputFloat(u8"発生フレーム", &attack_list[list].attack_single[sin].fream, 0.01f, 0.1f);
												ImGui::InputInt(u8"当たり判定の数", &attack_list[list].attack_single[sin].quantity, 1, 10);
												attack_list[list].attack_single[sin].parameter.resize(attack_list[list].attack_single[sin].quantity);
												if (!attack_list[list].attack_single[sin].parameter.empty())
												{
													if (ImGui::Button(u8"判定複製"))
													{
														AttackParameter back = attack_list[list].attack_single[sin].parameter.back();
														attack_list[list].attack_single[sin].parameter.push_back(back);
														attack_list[list].attack_single[sin].quantity++;
													}ImGui::SameLine();
													if (ImGui::Button(u8"判定最後尾削除"))
													{
														int size = attack_list[list].attack_single[sin].parameter.size() - 1;
														attack_list[list].attack_single[sin].parameter.resize(size);
														attack_list[list].attack_single[sin].quantity--;
													}
													if (ImGui::BeginTabBar(u8"判定", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
													{
														for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
														{
															//攻撃判定内部パラメーター表示
															std::string now_para = u8"判定";
															now_para += std::to_string(para);
															if (ImGui::BeginTabItem(now_para.c_str()))
															{
																int effect_k = scastI(attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind);
																ImGui::SliderInt(u8"エフェクトの種類", &effect_k, 0, scastI(EffectKind::END) - 1);
																ImGui::Text(GetName().effect_kind_name_list[scastI(attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind)].c_str());
																attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind = static_cast<EffectKind>(effect_k);

																if (attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind != EffectKind::NONE)
																{
																	if (ImGui::TreeNode(u8"エフェクト詳細設定"))
																	{
																		ImGui::SliderFloat(u8"座標X", &attack_list[list].attack_single[sin].parameter[para].effect_param.distance.x, -50.0f, 50.0f);
																		ImGui::SliderFloat(u8"座標Y", &attack_list[list].attack_single[sin].parameter[para].effect_param.distance.y, -50.0f, 50.0f);
																		ImGui::SliderFloat(u8"座標Z", &attack_list[list].attack_single[sin].parameter[para].effect_param.distance.z, -50.0f, 50.0f);
																		//float scale_all = attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x;
																		//ImGui::SliderFloat(u8"大きさ全体", &scale_all, 0.0f, 50.0f);
																		ImGui::SliderFloat(u8"大きさX", &attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x, 0.0f, 50.0f);
																		ImGui::SliderFloat(u8"大きさY", &attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y, 0.0f, 50.0f);
																		ImGui::SliderFloat(u8"大きさZ", &attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z, 0.0f, 50.0f);
																		/*if (attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z)
																		{
																			attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x = scale_all;
																			attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y = scale_all;
																			attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z = scale_all;
																		}*/
																		ImGui::SliderFloat(u8"方向X", &attack_list[list].attack_single[sin].parameter[para].effect_param.axis.x, 0.0, 1.0f);
																		ImGui::SliderFloat(u8"方向Y", &attack_list[list].attack_single[sin].parameter[para].effect_param.axis.y, 0.0, 1.0f);
																		ImGui::SliderFloat(u8"方向Z", &attack_list[list].attack_single[sin].parameter[para].effect_param.axis.z, 0.0, 1.0f);
																		ImGui::SliderFloat(u8"角度", &attack_list[list].attack_single[sin].parameter[para].effect_param.angle, -360.0f, 360.0f);
																		ImGui::Checkbox(u8"角度をプレイヤー依存にするか", &attack_list[list].attack_single[sin].parameter[para].effect_param.rightORleft);

																		ImGui::TreePop();
																	}
																}

																ImGui::SliderFloat(u8"プレイヤーとの距離X", &attack_list[list].attack_single[sin].parameter[para].distance.x, -10.0f, 10.0f);
																ImGui::SliderFloat(u8"プレイヤーとの距離Y", &attack_list[list].attack_single[sin].parameter[para].distance.y, -10.0f, 10.0f);
																ImGui::SliderFloat(u8"サイズX", &attack_list[list].attack_single[sin].parameter[para].size.x, 0.0f, 20.0f);
																ImGui::SliderFloat(u8"サイズY", &attack_list[list].attack_single[sin].parameter[para].size.y, 0.0f, 20.0f);
																ImGui::SliderFloat(u8"持続フレーム", &attack_list[list].attack_single[sin].parameter[para].timer, 0.0f, 2.0f);
																ImGui::SliderFloat(u8"ダメージ", &attack_list[list].attack_single[sin].parameter[para].param.damage, 0.0f, 300.0f);
																ImGui::SliderFloat(u8"のけぞり時間", &attack_list[list].attack_single[sin].parameter[para].param.HB_timer, 0.0f, 10.0f);
																ImGui::SliderFloat(u8"吹っ飛びX", &attack_list[list].attack_single[sin].parameter[para].param.hitback.x, -300.0f, 300.0f);
																ImGui::SliderFloat(u8"吹っ飛びY", &attack_list[list].attack_single[sin].parameter[para].param.hitback.y, -300.0f, 300.0f);
																int type = scastI(attack_list[list].attack_single[sin].parameter[para].type);
																ImGui::SliderInt(u8"攻撃タイプ", &type, 0, scastI(AttackKind::END) - 1);
																attack_list[list].attack_single[sin].parameter[para].type = static_cast<AttackKind>(type);
																ImGui::Text(GetName().attack_kind_name_list[type].c_str());
																ImGui::SliderFloat(u8"ノックバック(Xのみ)", &attack_list[list].attack_single[sin].parameter[para].knockback, -100.0f, 100.0f);
																ImGui::Checkbox(u8"ゲージを獲得しない", &attack_list[list].attack_single[sin].parameter[para].gaugeout);
																ImGui::SliderFloat(u8"つかみ抜けされる時間", &attack_list[list].attack_single[sin].parameter[para].param.steal_timer, 0.0f, 2.0f);
																int hit_stop_time = scastI(attack_list[list].attack_single[sin].parameter[para].HS_timer);
																ImGui::SliderInt(u8"ヒットストップ時間", &hit_stop_time, 0, scastI(HitStopTime::END) - 1);
																ImGui::Text(u8"0:短い。1:普通。2:長い。3:ズームストップ。4:ほぼ無い。5:少しだけ長い");
																attack_list[list].attack_single[sin].parameter[para].HS_timer = static_cast<HitStopTime>(hit_stop_time);
																ImGui::SliderFloat(u8"ガードバックX", &attack_list[list].attack_single[sin].parameter[para].param.guard_back.x, -100.0f, 100.0f);
																ImGui::SliderFloat(u8"ガードバックY", &attack_list[list].attack_single[sin].parameter[para].param.guard_back.y, -100.0f, 100.0f);
																ImGui::SliderFloat(u8"ガード削り", &attack_list[list].attack_single[sin].parameter[para].param.guard_shaving, 0.0f, 100.0f);
																ImGui::SliderFloat(u8"ガード硬直", &attack_list[list].attack_single[sin].parameter[para].param.guard_timer, 0.0f, 10.0f);
																ImGui::SliderFloat(u8"コンボ補正値", &attack_list[list].attack_single[sin].parameter[para].param.combo_correction, 0.0f, 10.0f);

																if (!attack_list[list].attack_single[sin].parameter[para].gaugeout)
																{
																	ImGui::InputFloat(u8"ゲージ獲得量", &attack_list[list].attack_single[sin].parameter[para].gauge_get, 0.01f, 0.1f);
																}
																ImGui::EndTabItem();
															}
														}
														ImGui::EndTabBar();
													}
												}

												ImGui::EndTabItem();
											}
										}
									}
								}
								ImGui::EndTabBar();
							}
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::End();


		static int combo_flg = 0;


		std::string now_com = std::to_string(now_player);
		now_com += std::string(":X_Combolist");
		ImGui::Begin(now_com.c_str());

		ImGui::RadioButton(u8"Xコンボ", &combo_flg, 0); ImGui::SameLine();
		ImGui::RadioButton(u8"Yコンボ", &combo_flg, 1); ImGui::SameLine();
		ImGui::RadioButton(u8"Bコンボ", &combo_flg, 2);

		switch (combo_flg)
		{
		case 0:
		{
			//Xコンボリスト作成
			static AttackState at_state = AttackState::NONE;
			int com_attack = scastI(at_state);
			ImGui::SliderInt(u8"追加するコンボ攻撃", &com_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
			ImGui::Text(GetName().attack_name_list[com_attack].c_str());
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
					ImGui::Text(GetName().attack_name_list[scastI(combolist_X.combolist[i])].c_str());
				}
				ImGui::EndChild();
			}
		}
		break;
		case 1:
		{
			//Yコンボリスト作成
			static AttackState at_state = AttackState::NONE;
			int com_attack = scastI(at_state);
			ImGui::SliderInt(u8"追加するコンボ攻撃", &com_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
			ImGui::Text(GetName().attack_name_list[com_attack].c_str());
			at_state = static_cast<AttackState>(com_attack);

			if (ImGui::Button(u8"コンボ追加"))
			{
				combolist_Y.combolist.push_back(AttackState());
				combolist_Y.combolist.back() = at_state;
			}ImGui::SameLine();
			if (ImGui::Button(u8"コンボ削除"))
			{
				if (!combolist_Y.combolist.empty())
				{
					int size = combolist_Y.combolist.size() - 1;
					combolist_Y.combolist.resize(size);
				}
			}

			if (!combolist_Y.combolist.empty())
			{
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
				for (int i = 0; i < combolist_Y.combolist.size(); ++i) {
					ImGui::Text(GetName().attack_name_list[scastI(combolist_Y.combolist[i])].c_str());
				}
				ImGui::EndChild();
			}
		}
		break;
		case 2:
		{
			//Bコンボリスト作成
			static AttackState at_state = AttackState::NONE;
			int com_attack = scastI(at_state);
			ImGui::SliderInt(u8"追加するコンボ攻撃", &com_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
			ImGui::Text(GetName().attack_name_list[com_attack].c_str());
			at_state = static_cast<AttackState>(com_attack);

			if (ImGui::Button(u8"コンボ追加"))
			{
				combolist_B.combolist.push_back(AttackState());
				combolist_B.combolist.back() = at_state;
			}ImGui::SameLine();
			if (ImGui::Button(u8"コンボ削除"))
			{
				if (!combolist_B.combolist.empty())
				{
					int size = combolist_B.combolist.size() - 1;
					combolist_B.combolist.resize(size);
				}
			}

			if (!combolist_B.combolist.empty())
			{
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
				for (int i = 0; i < combolist_B.combolist.size(); ++i) {
					ImGui::Text(GetName().attack_name_list[scastI(combolist_B.combolist[i])].c_str());
				}
				ImGui::EndChild();
			}
		}
		break;
		default:
			break;
		}


		ImGui::End();

		std::string now_pro = std::to_string(now_player);
		now_pro += std::string(u8"：飛び道具判定");
		ImGui::Begin(now_pro.c_str());

		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 500), ImGuiWindowFlags_NoTitleBar);

		if (!projectile_atk.empty())
		{
			for (int i = 0; i < projectile_atk.size(); i++)
			{
				ImGui::Text("pro[%d]:", i); ImGui::SameLine();
				ImGui::Text("pos_X : %f", projectile_atk[i].pos.x);
			}
		}

		ImGui::EndChild();
		ImGui::End();

		std::string sp_st = std::to_string(now_player) + std::string(u8" P : ") + GetName().chara_name_list[scastI(this->chara_name)] + std::string(u8" : 超必殺イベント");
		special_event.DrawTimeLine(sp_st,chara_name);
	}


#endif // USE_IMGUI
}


//デバッグ時当たり判定調整ツール描画
void Player::DrawDEBUGHitParam()
{
	//オーバーライドして使用する
}