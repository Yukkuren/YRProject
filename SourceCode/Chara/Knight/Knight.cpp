#include "Knight.h"
#include "../../Key.h"
#include "../../Game/YRGamePad.h"
#include "../../camera.h"
#include "../../framework.h"
#include <algorithm>
#include "../../Game//World.h"
#include "../../Effect.h"
#include "../../YRSound.h"
#include "../PlayerBase/Player_name_list.h"

//-----------------------------------------------------------------
//			Knightクラス
//-----------------------------------------------------------------
//
//	・親クラスにほとんどの処理を任せて必要な部分のみこのクラスで行うようにする
//-----------------------------------------------------------------

#ifdef EXIST_IMGUI

std::array<std::string, scastI(KNIGHTHIT::END)> hit_name_list =
{
	u8"頭",
	u8"ボディ",
	u8"足",
};

#endif // USE_IMGUI


Knight::~Knight() = default;

void Knight::CharaInit()
{
	//初期化

	scale = YR_Vector3( 0.1f,0.1f,0.1f );
	angle = YR_Vector3(0.0f, 0.0f, 0.0f);

	intro_state = INTRO_KNIGHT::SET;
	win_state = WIN_PERFORMANCE_KNIGHT::SET;


	production_time = 0.0f;
	anim_ccodinate = 2.0f;

	eye_offset = { 0.0f,0.0f };
	mouth_offset = { 0.0f,0.0f };

	face_anim = FaceAnim::NORMAL;
	face_wink_time = 0.0f;
	face_wink_interval_timer = 0.0f;
	wink_state = Wink_State::FIRST;
	lip_sync_time = 0.0f;

	camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
	intro_timer = 0.0f;
	win_timer = 0.0f;

	eye_plus = YR_Vector3(0.0f, 0.0f, -10.0f);
	focus_plus = YR_Vector3(0.0f, 0.0f, 0.0f);
	face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
	ChangeFace(FaceAnim::NORMAL_LIP_SYNC);

	traject.Init(max_traject_count);

	//剣の原点座標を保存する
	YR_Vector3 master = YR_Vector3(0.0f, 0.0f, 0.0f);
	for (auto& m : main->GetNodes())
	{
		if (m.name == std::string("Master"))
		{
			master = m.translate;
		}
		if (m.name == std::string("Sword"))
		{
			sword_tail = m.translate;
			sword_tail.x *= master.x;
			sword_tail.y *= master.y;
			sword_tail.z *= master.z;
		}
	}

	sword_head = sword_tail;
	sword_head.y += 0.88f;
	sword_tail.y += 0.2f;
}

void Knight::Uninit()
{
	//モデル系を全て解放する
	main.reset();
	main = nullptr;
	anim.reset();
	anim = nullptr;
	model_motion.air_jump_R.reset();
	model_motion.air_jump_R = nullptr;
	model_motion.intro_R.reset();
	model_motion.intro_R = nullptr;
	model_motion.win_R.reset();
	model_motion.win_R = nullptr;

	model_motion.air_jump_L.reset();
	model_motion.air_jump_L = nullptr;
	model_motion.intro_L.reset();
	model_motion.intro_L = nullptr;
	model_motion.win_L.reset();
	model_motion.win_L = nullptr;


	for (int i = 0; i < model_motion.model_R_Attack.size(); i++)
	{
		model_motion.model_R_Attack[i].reset();
		model_motion.model_R_Attack[i] = nullptr;
	}
	for (int i = 0; i < model_motion.model_L_Attack.size(); i++)
	{
		model_motion.model_L_Attack[i].reset();
		model_motion.model_L_Attack[i] = nullptr;
	}
	for (int i = 0; i < model_motion.model_R_Act.size(); i++)
	{
		model_motion.model_R_Act[i].reset();
		model_motion.model_R_Act[i] = nullptr;
	}
	for (int i = 0; i < model_motion.model_L_Act.size(); i++)
	{
		model_motion.model_L_Act[i].reset();
		model_motion.model_L_Act[i] = nullptr;
	}

	atk.clear();
	projectile_atk.clear();
	hit.clear();
	attack_list.clear();
	hitparam_list.clear();
}

void Knight::Update(float decision, float elapsed_time)
{
	//更新処理
	Player::Update(decision, elapsed_time);

	FaceAnimation(elapsed_time);
}




void Knight::Draw(
	YRShader				*parallel_shader,
	YRShader				*shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	//描画処理

	float game_speed = elapsed_time;		//基本はゲームのスピードを入力

	if (steal_escape > 0.0f && steal_escape < target_max)
	{
		game_speed = 0.0f;			//もし掴まれている状態ならアニメーションをさせたくないので0にする
	}

	//光の当たる角度は向いている向きによって固定化されるので向きを参照している
	light_direction.x = -rightOrleft;

	bool invincible = false;	//無敵かどうかを判別するフラグ

	//キャラの当たり判定に無敵がついていた場合無敵フラグをtrueに
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].state == HitBoxState::INVINCIBLE)
		{
			invincible = true;
		}
	}


	//Blenderのモデルのサブセットには0.8のmaterial_colorが入っているため逆数を入れている
	DirectX::XMFLOAT4 material_color = { 1.1f,1.1f,1.1f,1.0f };


	//体力が2割以下になった場合
	if (hp <= chara_state.max_hp * HP_Danger_point && hp != 0.0f)
	{
		//描画用のタイマーを増やす
		if (elapsed_time != 0.0f)
		{
			draw_count++;
		}

		//特定のタイミングで
		if (draw_count % 80 < 8)
		{
			//赤色にする
			material_color = { 1.1f,0.3f,0.3f,1.0f };
		}
	}

	//左向き
	if (rightOrleft < 0)
	{
		if (invincible)
		{
			//無敵なので全て発光させる
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			//無敵じゃない
			lumi_material = Model::Material_Attribute::NONE;
		}

	}
	//右向き
	else
	{
		if (invincible)
		{
			//無敵なので全て発光させる
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			lumi_material = Model::Material_Attribute::NONE;
		}
	}

	//モデル描画

	//カメラの状態によって描画が変更されるため個別で分けている

	const float shadow_y = -5.2f;
	const float shadow_z = 3.0f;

	//剣の軌跡を描画する際に使用
	DirectX::XMFLOAT4X4 return_inverse =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};



	//カメラの状態によってモデルの描画方法が違うため、分けている
	switch (YRCamera.camera_state)
	{
	case Camera::CAMERA_STATE::MAIN:
	{
		//カメラがゲーム画面全体を見ている場合
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], lumi_material, material_color);
		//影の表示
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
		break;
	case Camera::CAMERA_STATE::ZOOM_CAMERA:
	{
		//カメラがズームしている時
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], lumi_material, material_color);
		//影の表示
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
		break;
	case Camera::CAMERA_STATE::PLAYER1P:
	case Camera::CAMERA_STATE::PLAYER2P:
	{
		if (scastI(YRCamera.camera_state) == now_player)
		{
			//カメラがキャラを見ている場合
			anim->UpdateAnimation(game_speed * anim_ccodinate);
			anim->CalculateLocalTransform();
			anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
			return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], lumi_material, material_color);
			//影の表示
			anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
			anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}
		break;
	case Camera::CAMERA_STATE::END_GAME:
	{
		//対戦終了時
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], lumi_material, material_color);
		//影の表示
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
		break;
	default:
		break;
	}

	if (YRCamera.GetRequest() == Camera::Request::WEAKEN)
	{
		//カメラがキャラから画面全体を見るように移行している時
		anim->UpdateAnimation(game_speed* anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
		//影の表示
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}


	//剣の軌跡描画

	DirectX::XMFLOAT3 head = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 tail = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT4X4 coodinate_conversion = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
	};
	//剣のワールド変換行列を取得する
	DirectX::XMMATRIX sword_world_transform;
	for (auto& a : anim->GetNodes())
	{
		if (a.name == std::string("Sword"))
		{
			/*coodinate_conversion._41 = a.translate.x;
			coodinate_conversion._42 = a.translate.y;
			coodinate_conversion._43 = a.translate.z;*/
			sword_world_transform = DirectX::XMLoadFloat4x4(&a.world_transform);
			break;
		}
	}

	//取得した剣のワールド変換行列を使って剣の先端と根本の座標を割り出す
	DirectX::XMVECTOR head_vec = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&sword_head.GetDXFLOAT3()), sword_world_transform);
	DirectX::XMVECTOR tail_vec = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&sword_tail.GetDXFLOAT3()), sword_world_transform);


	DirectX::XMStoreFloat3(&head, head_vec);
	DirectX::XMStoreFloat3(&tail, tail_vec);

	if (timer < non_target && timer > 0.0f)
	{
		//攻撃が出ている状態なら剣の履歴を保存する
		if (traject_on)
		{
			//攻撃によっては剣の軌跡を表示しない
			traject.SetTrajectoryPos(head, tail);
		}
	}

	traject.Update(elapsed_time);

	traject.render(
			pos.GetDXFLOAT3(),
			scale.GetDXFLOAT3(), angle.GetDXFLOAT3(), view, projection, material_color);

	//テキスト描画
	TextDraw();
}




void Knight::WinAnimSet()
{
	//勝利演出用のセット処理

	pos = YR_Vector3( 0.0f,0.0f,0.0f );
	angle = YR_Vector3( 0.0f,0.0f,0.0f );
	hp = chara_state.max_hp;

	YR_Vector3 eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 80.0f);
	YR_Vector3 focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
	YRCamera.SetEye(eye.GetDXFLOAT3());
	YRCamera.SetFocus(focus.GetDXFLOAT3());
	if (now_player == 1)
	{
		anim->NodeChange(model_motion.win_R);
	}
	else
	{
		anim->NodeChange(model_motion.win_L);
	}
	anim->PlayAnimation(0, false);
	ChangeFace(FaceAnim::TOZI);
	FaceAnimation(0.0f);
	win_state = WIN_PERFORMANCE_KNIGHT::SET;
	anim_ccodinate = 1.1f;
}

bool Knight::WinPerformance(float elapsed_time)
{
	//勝利演出処理

	YR_Vector3 focus;
	YR_Vector3 eye;

	float apply = 1.0f;
	if (now_player == 1)
	{
		apply = 1.0f;
	}
	else
	{
		apply = -1.0f;
	}
	//勝利演出の行動をする
	//カメラもこちらで動かす
	switch (win_state)
	{
	case Knight::WIN_PERFORMANCE_KNIGHT::SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 80.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		win_state = WIN_PERFORMANCE_KNIGHT::ZOOM;
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::ZOOM:
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.1f * elapsed_time);
		if (win_timer > 1.5f)
		{
			ChangeFace(FaceAnim::YEAH);
			FaceAnimation(0.0f);
			win_state = WIN_PERFORMANCE_KNIGHT::PULL;
		}
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::PULL:
		eye = YR_Vector3(pos.x+(apply*-18.0f), pos.y + 5.0f, pos.z - 43.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 8.0f * elapsed_time);
		if (win_timer > 1.8f)
		{
			win_state = WIN_PERFORMANCE_KNIGHT::STOP;
		}
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::STOP:
		if (win_timer > 3.0f)
		{
			win_state = WIN_PERFORMANCE_KNIGHT::FINISH;
		}
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}
	win_timer += elapsed_time;

	return false;
}

bool Knight::Intro(float elapsed_time)
{
	//イントロの行動をする
	//カメラもこちらで動かす
	YR_Vector3	focus;
	YR_Vector3	eye;

	switch (intro_state)
	{
	case Knight::INTRO_KNIGHT::SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y + 1.8f, pos.z + 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::WAIT;
		lip_text = RandTextSelect();
		GetSound().SESinglePlay(SEKind::INTRO_WIND);
		break;
	case Knight::INTRO_KNIGHT::WAIT:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y +1.8f, pos.z + 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f*elapsed_time);
		if (intro_timer > 1.0f)
		{
			intro_state = INTRO_KNIGHT::ZOOM_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::ZOOM_SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(25.0f), pos.y + 5.0f, pos.z - 45.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::ZOOM;
		ChangeFace(FaceAnim::NORMAL_LIP_SYNC);
		break;
	case Knight::INTRO_KNIGHT::ZOOM:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.5f*elapsed_time);
		text_on = true;

		if (intro_timer > 3.0f)
		{
			intro_state = INTRO_KNIGHT::PULL;
			ChangeFace(FaceAnim::YARUKI);
			text_on = false;
		}
		break;
	case Knight::INTRO_KNIGHT::PULL:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.0f)
		{
			intro_state = INTRO_KNIGHT::PUSH;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 41.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.5f)
		{
			intro_state = INTRO_KNIGHT::PULL_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PULL_2:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 30.0f, pos.z - 85.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.9f)
		{
			intro_state = INTRO_KNIGHT::PUSH_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH_2:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x+Getapply(27.0f), pos.y + 5.4f, pos.z - 38.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 5.3f)
		{
			intro_state = INTRO_KNIGHT::FIN_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::FIN_SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(30.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x , pos.y, pos.z - 5.4f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::FIN;
		break;
	case Knight::INTRO_KNIGHT::FIN:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x - Getapply(48.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x - Getapply(5.0f), pos.y, pos.z - 5.4f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 8.0f * elapsed_time);
		if (intro_timer > 6.0f)
		{
			intro_state = INTRO_KNIGHT::FINISH;
		}
		break;
	case Knight::INTRO_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}
	intro_timer += elapsed_time;
	FaceAnimation(elapsed_time);
	/*if (intro_timer > 6.0f)
	{
		return true;
	}*/

	return false;
}


//イントロ後、ゲーム開始までの設定を行う
void Knight::ReadySet()
{
	text_on = false;
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.model_R_Act[scastI(ActState::WAIT)]);
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Act[scastI(ActState::WAIT)]);
	}
	anim_ccodinate = ac_act[scastI(ActState::WAIT)].fream;
	ChangeFace(FaceAnim::NORMAL);
	FaceAnimation(0.0f);
	//GetSound().SEPlay(SEKind::INTRO_WIND);
}


//カメラ調整用デバッグ関数
void Knight::IntroDEBUG()
{
#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		ImGui::Begin(u8"イントロカメラ");
		int state_in = scastI(intro_state);
		ImGui::InputInt(u8"ステート", &state_in, 1, 10);
		if (ImGui::TreeNode("Input"))
		{
			ImGui::InputFloat("eye.X", &eye_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Y", &eye_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Z", &eye_plus.z, 0.1f, 1.0f);
			ImGui::InputFloat("focus.X", &focus_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Y", &focus_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Z", &focus_plus.z, 0.1f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Slider"))
		{
			ImGui::SliderFloat("eye_X", &eye_plus.x, -350.1f, 350.1f);
			ImGui::SliderFloat("eye_Y", &eye_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("eye_Z", &eye_plus.z, -350.1f, 350.1);
			ImGui::SliderFloat("focus_X", &focus_plus.x, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Y", &focus_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Z", &focus_plus.z, -350.1f, 350.1);
			ImGui::TreePop();
		}
		ImGui::Text("intro_timer = %.3f", intro_timer);
		ImGui::End();
	}
#endif // USE_IMGUI

}

void Knight::WinDEBUG()
{
#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		ImGui::Begin(u8"勝利カメラ");
		int state_in = scastI(win_state);
		ImGui::InputInt(u8"ステート", &state_in, 1, 10);
		if (ImGui::TreeNode("Input"))
		{
			ImGui::InputFloat("eye.X", &eye_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Y", &eye_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Z", &eye_plus.z, 0.1f, 1.0f);
			ImGui::InputFloat("focus.X", &focus_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Y", &focus_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Z", &focus_plus.z, 0.1f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Slider"))
		{
			ImGui::SliderFloat("eye_X", &eye_plus.x, -350.1f, 350.1f);
			ImGui::SliderFloat("eye_Y", &eye_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("eye_Z", &eye_plus.z, -350.1f, 350.1);
			ImGui::SliderFloat("focus_X", &focus_plus.x, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Y", &focus_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Z", &focus_plus.z, -350.1f, 350.1);
			ImGui::TreePop();
		}
		ImGui::Text("intro_timer = %.3f", win_timer);
		ImGui::End();
	}
#endif // USE_IMGUI

}



//顔のテクスチャアニメーション
void Knight::FaceAnimation(float elapsed_time)
{
	switch (face_anim)
	{
	case Knight::FaceAnim::NORMAL:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
		//mouth_offset = face_mouth_offset[scastI(FaceMouth_Num::NORMAL_MOUSE)];
		face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
		face_wink_interval_timer += elapsed_time;
		if (face_wink_interval_timer > wink_interval)
		{
			face_anim = FaceAnim::WINK;
			face_wink_interval_timer = 0.0f;
		}
		break;
	case Knight::FaceAnim::NORMAL_LIP_SYNC:
		FaceLipSync(elapsed_time);
		face_wink_interval_timer += elapsed_time;
		if (face_wink_interval_timer > wink_interval)
		{
			if (FaceWink_bool(elapsed_time))
			{
				face_wink_interval_timer = 0.0f;
			}
		}
		break;
	case Knight::FaceAnim::WINK:
		FaceWink(elapsed_time);
		break;
	case Knight::FaceAnim::Damage:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::KAOMOJI)];
		face_mouth_num = FaceMouth_Num::POKAN;
		break;
	case Knight::FaceAnim::YARUKI:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::TURI)];
		face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
		break;
	case Knight::FaceAnim::KOUHUN:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::KIRAME)];
		face_mouth_num = FaceMouth_Num::OOGUTI;
		break;
	case Knight::FaceAnim::TOZI:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
		face_mouth_num = FaceMouth_Num::TOZI;
		break;
	case Knight::FaceAnim::YEAH:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::TURI)];
		face_mouth_num = FaceMouth_Num::OOGUTI;
		break;
	default:
		break;
	}
}


//瞬き処理
void Knight::FaceWink(float elapsed_time)
{
	face_wink_time += elapsed_time;
	if (face_wink_time > 0.05f)
	{
		switch (wink_state)
		{
		case Wink_State::FIRST:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SECOND;
			break;
		case Wink_State::SECOND:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::THIRD;
			break;
		case Wink_State::THIRD:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
			wink_state = Wink_State::FOURTH;
			break;
		case Wink_State::FOURTH:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::FIVE;
			break;
		case Wink_State::FIVE:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SIX;
			break;
		case Wink_State::SIX:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
			wink_state = Wink_State::FIRST;
			face_anim = FaceAnim::NORMAL;
			break;
		default:
			break;
		}
		face_wink_time = 0.0f;
	}
}


bool Knight::FaceWink_bool(float elapsed_time)
{
	//ウインクが終わったらtrueを返す
	face_wink_time += elapsed_time;
	if (face_wink_time > 0.05f)
	{
		switch (wink_state)
		{
		case Wink_State::FIRST:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SECOND;
			break;
		case Wink_State::SECOND:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::THIRD;
			break;
		case Wink_State::THIRD:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
			wink_state = Wink_State::FOURTH;
			break;
		case Wink_State::FOURTH:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::FIVE;
			break;
		case Wink_State::FIVE:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SIX;
			break;
		case Wink_State::SIX:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
			wink_state = Wink_State::FIRST;
			return true;
			break;
		default:
			break;
		}
		face_wink_time = 0.0f;
	}

	return false;
}


//口パクの処理
void Knight::FaceLipSync(float elapsed_time)
{
	lip_sync_time += elapsed_time;
	if (lip_sync_time > 0.1f)
	{
		for (int i = 0; i < 300; i++)
		{
			int mouth = rand() % lip_sync_can.size();
			if (mouth != scastI(face_mouth_num))
			{
				face_mouth_num = lip_sync_can[mouth];
				break;
			}
		}

		lip_sync_time = 0.0f;
	}
}



//表情変更
void Knight::ChangeFace(FaceAnim anim)
{
	face_anim = anim;
	face_wink_time = 0.0f;
	wink_state = Wink_State::FIRST;
	face_wink_interval_timer = 0.0f;
	face_wink_time = 0.0f;
	//表情がまばたき込みの場合のみ
	if (face_anim == FaceAnim::NORMAL||
		face_anim ==FaceAnim::NORMAL_LIP_SYNC)
	{
		//ランダムでまばたきの間隔を決定
		wink_interval =static_cast<float>((rand() % 5) + 1);
	}
}



void Knight::AttackDetailsSet(const AttackState& attack_state)
{
	//攻撃ごとに初回だけ通す処理を記述する
	switch (attack_state)
	{
	case AttackState::NONE:
		break;
	case AttackState::JAKU:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::THU:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::D_KYO:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::D_JAKU:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::D_THU:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::U_KYO:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::A_JAKU:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::A_THU:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::A_KYO:
		ChangeFace(FaceAnim::YARUKI);
		break;
	case AttackState::A_UKYO:
		//speed.y = 0.0f;
		//speed_Y.Set(attack_list[scastI(attack_state)].advance_speed);
		ChangeFace(FaceAnim::YARUKI);
		break;
	case AttackState::STEAL:
		ChangeFace(FaceAnim::YARUKI);
		break;
	case AttackState::SLOW:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::TRACK_DASH:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::JAKU_RHURF:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::THU_RHURF:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::KYO_RHURF:
		ChangeFace(FaceAnim::YEAH);
		GetSound().SESinglePlay(SEKind::SPECIAL_ATTACK);
		break;
	case AttackState::JAKU_LHURF:
		ChangeFace(FaceAnim::KOUHUN);
		break;
	case AttackState::THU_LHURF:
		ChangeFace(FaceAnim::KOUHUN);
		break;
	case AttackState::KYO_LHURF:
		ChangeFace(FaceAnim::KOUHUN);
		break;
	case AttackState::SPECIAL_ATTACK:
		YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
		camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
		ChangeFace(FaceAnim::KOUHUN);
		lumi_material= Model::Material_Attribute::SWORD;
		GetSound().SESinglePlay(SEKind::SPECIAL_ATTACK);
		YRGetEffect().PlayEffect(EffectKind::WIND, DirectX::XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);
		YRGetEffect().PlayEffect(EffectKind::WIND, DirectX::XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);
		cut_in_timer = 0.0f;
		break;
	case AttackState::DESIRE_SPECIAL:
		break;
	case AttackState::DESIRE_METEOR:
		break;
	case AttackState::EXTENDATK:
		break;
	case AttackState::JAKU_THU:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::JAKU_KYO:
		ChangeFace(FaceAnim::YEAH);
		break;
	case AttackState::ATTACK_END:
		break;
	default:
		break;
	}
}

std::wstring Knight::RandTextSelect()
{
	//イントロ中に表示するテキストをランダムに設定する

	TextList rnd = static_cast<TextList>(rand() % scastI(TextList::TEXT_END));

	switch (rnd)
	{
	case Knight::TextList::NORMAL://通常
		return std::wstring(L"さぁ、始めようか！");
		break;
	case Knight::TextList::WARLIKE://好戦的
		return std::wstring(L"一気に行くよ！");
		break;
	case Knight::TextList::CRIOSITY://好奇心
		return std::wstring(L"私と戦いたいの？");
		break;
	case Knight::TextList::TEXT_END:
		break;
	default:
		break;
	}
}


//デバッグ時当たり判定調整ツール描画
void Knight::DrawDEBUGHitParam()
{
	if (ImGui::TreeNode(u8"プレイヤー当たり判定調整"))
	{
		if (ImGui::TreeNode(u8"行動当たり判定"))
		{
			for (int act = 0; act < scastI(ActState::ACT_END); act++)
			{
				if (ImGui::TreeNode(GetName().act_name_list[act].c_str()))
				{
					for (int list = 0; list < hitparam_list.size(); list++)
					{
						//プレイヤーの当たり判定をそれぞれ出す
						if (ImGui::TreeNode(hit_name_list[list].c_str()))
						{
							ImGui::SliderFloat(u8"プレイヤーとの距離X", &hitparam_list[list].act_parameter[act].distance.x, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"プレイヤーとの距離Y", &hitparam_list[list].act_parameter[act].distance.y, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"大きさX", &hitparam_list[list].act_parameter[act].size.x, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"大きさY", &hitparam_list[list].act_parameter[act].size.y, 0.0f, 50.0f);
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
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"攻撃当たり判定"))
		{
			for (int atk = 0; atk < scastI(AttackState::ATTACK_END); atk++)
			{
				if (ImGui::TreeNode(GetName().attack_name_list[atk].c_str()))
				{
					for (int list = 0; list < hitparam_list.size(); list++)
					{
						if (ImGui::TreeNode(hit_name_list[list].c_str()))
						{
							ImGui::SliderFloat(u8"プレイヤーとの距離X", &hitparam_list[list].attack_parameter[atk].distance.x, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"プレイヤーとの距離Y", &hitparam_list[list].attack_parameter[atk].distance.y, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"大きさX", &hitparam_list[list].attack_parameter[atk].size.x, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"大きさY", &hitparam_list[list].attack_parameter[atk].size.y, 0.0f, 50.0f);
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}