#include "PlayerBase.h"
#include "../../framework.h"
#include <fstream>
#include "Player_name_list.h"
//#include "../../camera.h"


void CameraDirecting::Init(int now_player)
{
	//初期化
	now_event = 0;
	timer = 0.0f;
	this->now_player = now_player;
	YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
}


void CameraDirecting::Load(PLSELECT chara_name)
{
	//ファイルから読み込み
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name_s = GetName().chara_name_list[scastI(chara_name)];

	std::string AP_level = f_level + chara_name_s + std::string("/CameraDirecting.txt");
	std::ifstream ifs(AP_level);
	std::string str;
	int size = 0;

	std::getline(ifs, str);
	ifs >> size;
	camera_event.resize(size);

	std::getline(ifs, str);
	std::getline(ifs, str);
	ifs >> max_fream;

	if (!camera_event.empty())
	{
		//中身の読み込み
		for (int i = 0; i < camera_event.size(); i++)
		{
			std::getline(ifs, str);

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].camera_eye.x;
			ifs >> camera_event[i].camera_eye.y;
			ifs >> camera_event[i].camera_eye.z;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].camera_focus.x;
			ifs >> camera_event[i].camera_focus.y;
			ifs >> camera_event[i].camera_focus.z;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].camera_up.x;
			ifs >> camera_event[i].camera_up.y;
			ifs >> camera_event[i].camera_up.z;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].fov;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].event_point;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].wait_camera;


			int ek, fk, sk, cr;
			//エフェクト調整値読み込み
			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> ek;
			camera_event[i].effect_param.effect_kind = static_cast<EffectKind>(ek);

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].effect_param.distance.x;
			ifs >> camera_event[i].effect_param.distance.y;
			ifs >> camera_event[i].effect_param.distance.z;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].effect_param.scale.x;
			ifs >> camera_event[i].effect_param.scale.y;
			ifs >> camera_event[i].effect_param.scale.z;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].effect_param.axis.x;
			ifs >> camera_event[i].effect_param.axis.y;
			ifs >> camera_event[i].effect_param.axis.z;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].effect_param.angle;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].effect_param.rightORleft;

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> fk;
			camera_event[i].face_kind = static_cast<FaceAnim>(fk);

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> sk;
			camera_event[i].se_kind = static_cast<SEKind>(sk);

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> cr;
			camera_event[i].camera_req = static_cast<Camera::Request>(cr);

			std::getline(ifs, str);
			std::getline(ifs, str);
			ifs >> camera_event[i].camera_move;
		}
	}

	//もし落ちたらエラーを出す
	if (ifs.fail())
	{
		assert("ifs failled");
	}
	//ファイルを閉じる
	ifs.close();
}


void CameraDirecting::Write(PLSELECT chara_name)
{
	//ファイルに書き出し
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name_s = GetName().chara_name_list[scastI(chara_name)];

	std::string AP_level = f_level + chara_name_s + std::string("/CameraDirecting.txt");
	std::ofstream outputfile(AP_level);

	outputfile << "イベント合計" << std::endl;
	outputfile << camera_event.size() << std::endl;

	outputfile << "最大フレーム" << std::endl;
	outputfile << max_fream << std::endl;

	if (!camera_event.empty())
	{
		//中身の書き出し
		for (int i = 0; i < camera_event.size(); i++)
		{
			outputfile << "イベント" << i << std::endl;

			outputfile << "カメラ視点" << std::endl;
			outputfile << camera_event[i].camera_eye.x << std::endl;
			outputfile << camera_event[i].camera_eye.y << std::endl;
			outputfile << camera_event[i].camera_eye.z << std::endl;

			outputfile << "カメラ注視点" << std::endl;
			outputfile << camera_event[i].camera_focus.x << std::endl;
			outputfile << camera_event[i].camera_focus.y << std::endl;
			outputfile << camera_event[i].camera_focus.z << std::endl;

			outputfile << "カメラ上ベクトル" << std::endl;
			outputfile << camera_event[i].camera_up.x << std::endl;
			outputfile << camera_event[i].camera_up.y << std::endl;
			outputfile << camera_event[i].camera_up.z << std::endl;

			outputfile << "画角" << std::endl;
			outputfile << camera_event[i].fov << std::endl;

			outputfile << "待機時間" << std::endl;
			outputfile << camera_event[i].event_point << std::endl;

			outputfile << "カメラを動かさないか" << std::endl;
			outputfile << camera_event[i].wait_camera << std::endl;

			//エフェクト調整値書き出し
			outputfile << "エフェクトの種類" << std::endl;
			outputfile << scastI(camera_event[i].effect_param.effect_kind) << std::endl;

			outputfile << "座標の位置の差" << std::endl;
			outputfile << camera_event[i].effect_param.distance.x << std::endl;
			outputfile << camera_event[i].effect_param.distance.y << std::endl;
			outputfile << camera_event[i].effect_param.distance.z << std::endl;

			outputfile << "エフェクトの大きさ" << std::endl;
			outputfile << camera_event[i].effect_param.scale.x << std::endl;
			outputfile << camera_event[i].effect_param.scale.y << std::endl;
			outputfile << camera_event[i].effect_param.scale.z << std::endl;

			outputfile << "角度の方向" << std::endl;
			outputfile << camera_event[i].effect_param.axis.x << std::endl;
			outputfile << camera_event[i].effect_param.axis.y << std::endl;
			outputfile << camera_event[i].effect_param.axis.z << std::endl;

			outputfile << "角度" << std::endl;
			outputfile << camera_event[i].effect_param.angle << std::endl;

			outputfile << "角度をプレイヤーに依存させるかどうか" << std::endl;
			outputfile << camera_event[i].effect_param.rightORleft << std::endl;

			outputfile << "表情" << std::endl;
			outputfile << scastI(camera_event[i].face_kind) << std::endl;

			outputfile << "サウンドの種類" << std::endl;
			outputfile << scastI(camera_event[i].se_kind) << std::endl;

			outputfile << "カメラのリクエスト" << std::endl;
			outputfile << scastI(camera_event[i].camera_req) << std::endl;

			outputfile << "カメラをステートで動かすか" << std::endl;
			outputfile << camera_event[i].camera_move << std::endl;
		}
	}


	outputfile.close();
}


bool CameraDirecting::CameraEventUpdate(YR_Vector3 pos, float decision, float elapsed_time, Player* player)	//引数：プレイヤーの位置、プレイヤーの向いている方向,フレーム,
{
	//-------------------------------------------------------------
	//	イベント処理
	//-------------------------------------------------------------
	//
	//	・タイマーを増やしていき、executedがfalseかつタイマーがeventpointを超えたものを実行していく
	//	・camera_moveがfalse以外のものは、次のイベントのカメラの位置を参照し、徐々に近づける(XMVECTORの関数を使用)
	//	・最後のイベントになったらカメラはReleaseし、trueを返すようにする
	//	・イベントはfor文で全て確認していく
	//----------------------------------------------------------

	this->decision = decision;

	//カメライベントを更新していく
	if (camera_event.empty())
	{
		//中身が空なら処理をしない
		return true;
	}
	else
	{
		//イベントを回していく

		if (timer < max_fream)
		{
			//タイマーを増やしていく
			timer += elapsed_time;
		}

		//現在のイベント番号を算出する
		NowEventCheck();

		for (int e = 0; e < camera_event.size(); e++)
		{
			//タイマーがイベントポイント以上になったら
			if (camera_event[e].event_point <= timer && !test)
			{
				//まだ実行していないなら
				if (!camera_event[e].executed)
				{
					//処理を行う

					//現在のイベント番号を保存(あとで変更)
					now_event = e;

					//カメラのリクエストを指示
					YRCamera.RequestCamera(camera_event[e].camera_req, now_player);

					//カメラを動かす指示がある場合
					if (camera_event[e].camera_move)
					{
						//イベントステートからカメラを動かす
						CameraStateUpdate(pos);
					}

					//SEを再生する
					GetSound().SESinglePlay(camera_event[e].se_kind);

					//エフェクトを生成
					SetEffect(pos);

					//表情を変更する
					player->ChangeFace(camera_event[e].face_kind);
				}
			}
		}

		//イベントの実行確認を判別する
		ExecuteEvent();
	}
	return false;
}




float CameraDirecting::FovCalculation(float fov)
{
	//画角を返す
	return fov * 0.01745f;
}



float CameraDirecting::Getapply(float n)
{
	//プレイヤーの現在の向きからベクトルを返す
	return n * decision;
}




void CameraDirecting::NowEventCheck()
{
	//現在のイベント番号を割り出す
	for (int i = 0; i < camera_event.size(); i++)
	{
		if (camera_event[i].event_point <= timer)
		{
			//タイマーがイベントポイント以上なら
			now_event = i;
		}
	}
}





void CameraDirecting::ExecuteEvent()
{
	//タイマーが過ぎたイベントを実行済みにする
	for (int i = 0; i < camera_event.size(); i++)
	{
		if (camera_event[i].event_point <= timer)
		{
			//タイマーがイベントポイント以上なら
			camera_event[i].executed = true;
		}
		else
		{
			//タイマーがイベントポイント以下なら
			camera_event[i].executed = false;
		}
	}
}




void CameraDirecting::CameraStateUpdate(YR_Vector3 pos)
{
	//カメラをステートを使用して動かす
	YR_Vector3 eye,focus,up;

	//視点
	eye.x = pos.x + Getapply(camera_event[now_event].camera_eye.x);
	eye.y = pos.y + camera_event[now_event].camera_eye.y;
	eye.z = pos.z + camera_event[now_event].camera_eye.z;

	//注視点
	focus.x = pos.x + Getapply(camera_event[now_event].camera_focus.x);
	focus.y = pos.y + camera_event[now_event].camera_focus.y;
	focus.z = pos.z + camera_event[now_event].camera_focus.z;

	//上ベクトル
	up.x = camera_event[now_event].camera_focus.x;
	up.y = camera_event[now_event].camera_focus.y;
	up.z = camera_event[now_event].camera_focus.z;

	//カメラを設定する
	YRCamera.SetEye(eye.GetDXFLOAT3());
	YRCamera.SetFocus(focus.GetDXFLOAT3());
	YRCamera.SetFocus(up.GetDXFLOAT3());
	YRCamera.SetFov(FovCalculation(camera_event[now_event].fov));
}




void CameraDirecting::SetEffect(YR_Vector3 pos)
{
	//エフェクトを生成する
	if (camera_event[now_event].effect_param.rightORleft)
	{
		//プレイヤーの角度を依存させる場合
		YRGetEffect().PlayEffect(
			camera_event[now_event].effect_param.effect_kind, camera_event[now_event].handle,
			DirectX::XMFLOAT3(pos.x + camera_event[now_event].effect_param.distance.x, pos.y + camera_event[now_event].effect_param.distance.y, pos.z + camera_event[now_event].effect_param.distance.z),
			camera_event[now_event].effect_param.scale.GetDXFLOAT3(), camera_event[now_event].effect_param.axis.GetDXFLOAT3(), camera_event[now_event].effect_param.angle * decision);
	}
	else
	{
		//依存させない場合
		YRGetEffect().PlayEffect(
			camera_event[now_event].effect_param.effect_kind, camera_event[now_event].handle,
			DirectX::XMFLOAT3(pos.x + camera_event[now_event].effect_param.distance.x, pos.y + camera_event[now_event].effect_param.distance.y, pos.z + camera_event[now_event].effect_param.distance.z),
			camera_event[now_event].effect_param.scale.GetDXFLOAT3(), camera_event[now_event].effect_param.axis.GetDXFLOAT3(), camera_event[now_event].effect_param.angle);
	}
}




void CameraDirecting::DrawTimeLine(std::string timeline_name, PLSELECT chara_name)
{
	//タイムラインの描画
	ImGui::Begin(timeline_name.c_str());

	ImGui::SliderFloat(u8"最大フレーム", &max_fream, 0.0f, 5.0f);
	ImGui::SliderFloat(u8"現在のタイマー", &timer, 0.0f, max_fream);
	ImGui::Checkbox(u8"テストフラグ", &test);

	if (ImGui::Button(u8"イベント追加"))
	{
		if (camera_event.empty())
		{
			//イベントが一つもない場合
			camera_event.push_back(CameraEvent());
			camera_event.back().camera_eye = YRCamera.GetEye();
			camera_event.back().camera_focus = YRCamera.GetFocus();
			camera_event.back().camera_up = YRCamera.GetUp();
			camera_event.back().fov = YRCamera.GetFov();
		}
		else
		{
			//既にイベントがある場合
			CameraEvent f_event = camera_event.back();
			camera_event.push_back(f_event);
		}
	}

	if (ImGui::TreeNode(u8"イベント削除"))
	{
		if (!camera_event.empty())
		{
			if (ImGui::Button(u8"最後尾を削除"))
			{
				int size = camera_event.size() - 1;
				camera_event.resize(size);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::Button(u8"ロード"))
	{
		ImGui::OpenPopup("Load");
	}
	if (ImGui::BeginPopup("Load"))
	{
		ImGui::Text(u8"ロードしますか？");
		if (ImGui::Button(u8"はい"))
		{
			Load(chara_name);
			load_timer = 6.0f;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button(u8"いいえ"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (load_timer > 0.0f)
	{
		ImGui::Text(u8"ロードしました");
		load_timer -= 0.1f;
	}


	if (!camera_event.empty())
	{

		ImGui::Text("------------------------------------------------------------------------------");

		if (ImGui::Button(u8"セーブ"))
		{
			ImGui::OpenPopup("Save");
		}//ImGui::SameLine();
		if (ImGui::BeginPopup("Save"))
		{
			ImGui::Text(u8"セーブしますか？");
			if (ImGui::Button(u8"はい"))
			{
				Write(chara_name);
				save_timer = 6.0f;
				ImGui::CloseCurrentPopup();
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
			save_timer -= 0.1f;
		}

		if (ImGui::BeginTabBar(u8"イベント", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
		{
			for (int i = 0; i < camera_event.size(); i++)
			{
				std::string tab_name = std::to_string(camera_event[i].event_point);
				if (ImGui::BeginTabItem(tab_name.c_str()))
				{
					ImGui::SliderFloat(u8"位置X", &camera_event[i].camera_eye.x, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"位置Y", &camera_event[i].camera_eye.y, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"位置Z", &camera_event[i].camera_eye.z, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"注視点X", &camera_event[i].camera_focus.x, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"注視点Y", &camera_event[i].camera_focus.y, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"注視点Z", &camera_event[i].camera_focus.z, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"上X", &camera_event[i].camera_up.x, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"上Y", &camera_event[i].camera_up.y, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"上Z", &camera_event[i].camera_up.z, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"画角", &camera_event[i].fov, 0.0f, 100.0f);
					ImGui::SliderFloat(u8"イベント発生フレーム", &camera_event[i].event_point, 0.0f, 5.0f);
					ImGui::Checkbox(u8"待機するかどうか", &camera_event[i].wait_camera);

					int effect_k_h = scastI(camera_event[i].effect_param.effect_kind);
					ImGui::SliderInt(u8"エフェクトの種類", &effect_k_h, 0, scastI(EffectKind::END) - 1);
					ImGui::Text(GetName().effect_kind_name_list[scastI(camera_event[i].effect_param.effect_kind)].c_str());
					camera_event[i].effect_param.effect_kind = static_cast<EffectKind>(effect_k_h);

					if (camera_event[i].effect_param.effect_kind != EffectKind::NONE)
					{
						if (ImGui::TreeNode(u8"エフェクト詳細設定"))
						{
							ImGui::SliderFloat(u8"座標X", &camera_event[i].effect_param.distance.x, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"座標Y", &camera_event[i].effect_param.distance.y, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"座標Z", &camera_event[i].effect_param.distance.z, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"大きさX", &camera_event[i].effect_param.scale.x, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"大きさY", &camera_event[i].effect_param.scale.y, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"大きさZ", &camera_event[i].effect_param.scale.z, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"方向X", &camera_event[i].effect_param.axis.x, 0.0, 1.0f);
							ImGui::SliderFloat(u8"方向Y", &camera_event[i].effect_param.axis.y, 0.0, 1.0f);
							ImGui::SliderFloat(u8"方向Z", &camera_event[i].effect_param.axis.z, 0.0, 1.0f);
							ImGui::SliderFloat(u8"角度", &camera_event[i].effect_param.angle, -360.0f, 360.0f);
							ImGui::Checkbox(u8"角度をプレイヤー依存にするか", &camera_event[i].effect_param.rightORleft);

							ImGui::TreePop();
						}
					}

					int face = scastI(camera_event[i].face_kind);
					ImGui::SliderInt(u8"表情", &face, 0, scastI(FaceAnim::END) - 1);
					camera_event[i].face_kind = static_cast<FaceAnim>(face);

					int se = scastI(camera_event[i].se_kind);
					ImGui::SliderInt(u8"鳴らすSE", &se, 0, scastI(SEKind::END) - 1);
					camera_event[i].se_kind = static_cast<SEKind>(se);

					int req = scastI(camera_event[i].camera_req);
					ImGui::SliderInt(u8"カメラリクエスト", &req, 0, scastI(Camera::Request::Request_END) - 1);
					camera_event[i].camera_req = static_cast<Camera::Request>(req);

					ImGui::Checkbox(u8"カメラをステータスで動かすか", &camera_event[i].camera_move);

					if (camera_event[i].camera_eye.x == camera_event[i].camera_focus.x)
					{
						camera_event[i].camera_focus.x += 0.1f;
					}
					if (camera_event[i].camera_eye.y == camera_event[i].camera_focus.y)
					{
						camera_event[i].camera_focus.y += 0.1f;
					}
					if (camera_event[i].camera_eye.z == camera_event[i].camera_focus.z)
					{
						camera_event[i].camera_focus.z += 0.1f;
					}

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}
