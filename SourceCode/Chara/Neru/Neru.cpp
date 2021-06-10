#include "Neru.h"
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
//			Knight�N���X
//-----------------------------------------------------------------
//
//	�E�e�N���X�ɂقƂ�ǂ̏�����C���ĕK�v�ȕ����݂̂��̃N���X�ōs���悤�ɂ���
//-----------------------------------------------------------------

#ifdef EXIST_IMGUI

std::array<std::string, scastI(NERUHIT::END)> hit_name_list_Neru =
{
	u8"��",
	u8"�{�f�B",
	u8"��",
	u8"������",
};

#endif // USE_IMGUI


Neru::~Neru() = default;

void Neru::CharaInit()
{
	//������

	scale = YR_Vector3( 0.08f,0.08f,0.08f );
	angle = YR_Vector3(0.0f, 0.0f, 0.0f);

	intro_state = INTRO_NERU::SET;
	win_state = WIN_PERFORMANCE_NERU::SET;


	production_time = 0.0f;
	anim_ccodinate = 2.0f;

	camera_state_neru = CAMERA_STATE_NERU::FIRST;
	intro_timer = 0.0f;
	win_timer = 0.0f;

	eye_plus = YR_Vector3(0.0f, 0.0f, -10.0f);
	focus_plus = YR_Vector3(0.0f, 0.0f, 0.0f);

	//traject.Init(max_traject_count);

	//���̌��_���W��ۑ�����
	/*YR_Vector3 master = YR_Vector3(0.0f, 0.0f, 0.0f);
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
	sword_tail.y += 0.2f;*/
}

void Neru::Uninit()
{
	//���f���n��S�ĉ������
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

void Neru::Update(float decision, float elapsed_time)
{
	//�X�V����
	Player::Update(decision, elapsed_time);
}




void Neru::Draw(
	YRShader				*parallel_shader,
	YRShader				*shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	//�`�揈��

	float game_speed = elapsed_time;		//��{�̓Q�[���̃X�s�[�h�����

	if (steal_escape > 0.0f && steal_escape < target_max)
	{
		game_speed = 0.0f;			//�����͂܂�Ă����ԂȂ�A�j���[�V���������������Ȃ��̂�0�ɂ���
	}

	//���̓�����p�x�͌����Ă�������ɂ���ČŒ艻�����̂Ō������Q�Ƃ��Ă���
	light_direction.x = -rightOrleft;

	bool invincible = false;	//���G���ǂ����𔻕ʂ���t���O

	//�L�����̓����蔻��ɖ��G�����Ă����ꍇ���G�t���O��true��
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].state == HitBoxState::INVINCIBLE)
		{
			invincible = true;
		}
	}


	//Blender�̃��f���̃T�u�Z�b�g�ɂ�0.8��material_color�������Ă��邽�ߋt�������Ă���
	DirectX::XMFLOAT4 material_color = { 1.1f,1.1f,1.1f,1.0f };


	//�̗͂�2���ȉ��ɂȂ����ꍇ
	if (hp <= chara_state.max_hp * HP_Danger_point && hp != 0.0f)
	{
		//�`��p�̃^�C�}�[�𑝂₷
		if (elapsed_time != 0.0f)
		{
			draw_count++;
		}

		//����̃^�C�~���O��
		if (draw_count % 80 < 8)
		{
			//�ԐF�ɂ���
			material_color = { 1.1f,0.3f,0.3f,1.0f };
		}
	}

	//������
	if (rightOrleft < 0)
	{
		if (invincible)
		{
			//���G�Ȃ̂őS�Ĕ���������
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			//���G����Ȃ�
			lumi_material = Model::Material_Attribute::NONE;
		}

	}
	//�E����
	else
	{
		if (invincible)
		{
			//���G�Ȃ̂őS�Ĕ���������
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			lumi_material = Model::Material_Attribute::NONE;
		}
	}

	//���f���`��

	//�J�����̏�Ԃɂ���ĕ`�悪�ύX����邽�ߌʂŕ����Ă���

	const float shadow_y = -5.2f;
	const float shadow_z = 3.0f;

	//���̋O�Ղ�`�悷��ۂɎg�p
	DirectX::XMFLOAT4X4 return_inverse =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	DirectX::XMFLOAT2 eye_offset = { 0.0f,0.0f };
	DirectX::XMFLOAT2 face_mouth_offset = { 0.0f,0.0f };


	DirectX::XMFLOAT3 draw_pos = { pos.x, pos.y + adjust_pos_Y, pos.z };

	//�J�����̏�Ԃɂ���ă��f���̕`����@���Ⴄ���߁A�����Ă���
	switch (YRCamera.camera_state)
	{
	case Camera::CAMERA_STATE::MAIN:
	{
		//�J�������Q�[����ʑS�̂����Ă���ꍇ
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(draw_pos, scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, lumi_material, material_color);
		//�e�̕\��
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
		break;
	case Camera::CAMERA_STATE::ZOOM_CAMERA:
	{
		//�J�������Y�[�����Ă��鎞
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(draw_pos, scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, lumi_material, material_color);
		//�e�̕\��
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
		break;
	case Camera::CAMERA_STATE::PLAYER1P:
	case Camera::CAMERA_STATE::PLAYER2P:
	{
		if (scastI(YRCamera.camera_state) == now_player)
		{
			//�J�������L���������Ă���ꍇ
			anim->UpdateAnimation(game_speed * anim_ccodinate);
			anim->CalculateLocalTransform();
			anim->CalculateWorldTransform(draw_pos, scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
			return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, lumi_material, material_color);
			//�e�̕\��
			anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
			anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}
		break;
	case Camera::CAMERA_STATE::END_GAME:
	{
		//�ΐ�I����
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(draw_pos, scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, lumi_material, material_color);
		//�e�̕\��
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
		break;
	default:
		break;
	}

	if (YRCamera.GetRequest() == Camera::Request::WEAKEN)
	{
		//�J�������L���������ʑS�̂�����悤�Ɉڍs���Ă��鎞
		anim->UpdateAnimation(game_speed* anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		return_inverse = anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset,lumi_material, material_color);
		//�e�̕\��
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, shadow_y, pos.z + shadow_z), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset, Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}


	//if (Get_Use_ImGui())
	//{
	//	ImGui::Begin(u8"Neru");
	//	ImGui::Text("dot_angle : %f", dot_angle);
	//	ImGui::Text("up : %f", upper);
	//	ImGui::Text("down : %f", dowwnn);
	//	//ImGui::Text("now_at_num : %d", now_at_num);
	//	ImGui::End();
	//}

	//���̋O�Օ`��

	//DirectX::XMFLOAT3 head = { 0.0f,0.0f,0.0f };
	//DirectX::XMFLOAT3 tail = { 0.0f,0.0f,0.0f };
	//DirectX::XMFLOAT4X4 coodinate_conversion = {
	//1,0,0,0,
	//0,1,0,0,
	//0,0,1,0,
	//0,0,0,1
	//};
	////���̃��[���h�ϊ��s����擾����
	//DirectX::XMMATRIX sword_world_transform;
	//for (auto& a : anim->GetNodes())
	//{
	//	if (a.name == std::string("Sword"))
	//	{
	//		/*coodinate_conversion._41 = a.translate.x;
	//		coodinate_conversion._42 = a.translate.y;
	//		coodinate_conversion._43 = a.translate.z;*/
	//		sword_world_transform = DirectX::XMLoadFloat4x4(&a.world_transform);
	//		break;
	//	}
	//}

	////�擾�������̃��[���h�ϊ��s����g���Č��̐�[�ƍ��{�̍��W������o��
	//DirectX::XMVECTOR head_vec = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&sword_head.GetDXFLOAT3()), sword_world_transform);
	//DirectX::XMVECTOR tail_vec = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&sword_tail.GetDXFLOAT3()), sword_world_transform);


	//DirectX::XMStoreFloat3(&head, head_vec);
	//DirectX::XMStoreFloat3(&tail, tail_vec);

	//if (timer < non_target && timer > 0.0f)
	//{
	//	//�U�����o�Ă����ԂȂ猕�̗�����ۑ�����
	//	if (traject_on)
	//	{
	//		//�U���ɂ���Ă͌��̋O�Ղ�\�����Ȃ�
	//		traject.SetTrajectoryPos(head, tail);
	//	}
	//}

	//traject.Update(elapsed_time);

	//traject.render(
	//		pos.GetDXFLOAT3(),
	//		scale.GetDXFLOAT3(), angle.GetDXFLOAT3(), view, projection, material_color);

	//�e�L�X�g�`��
	TextDraw();
}




void Neru::WinAnimSet()
{
	//�������o�p�̃Z�b�g����

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
	win_state = WIN_PERFORMANCE_NERU::SET;
	anim_ccodinate = 1.1f;
}

bool Neru::WinPerformance(float elapsed_time)
{
	//�������o����

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
	//�������o�̍s��������
	//�J������������œ�����
	switch (win_state)
	{
	case Neru::WIN_PERFORMANCE_NERU::SET:
		//���_��ݒ�(������)
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 80.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		win_state = WIN_PERFORMANCE_NERU::ZOOM;
		break;
	case Neru::WIN_PERFORMANCE_NERU::ZOOM:
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.1f * elapsed_time);
		if (win_timer > 1.5f)
		{
			ChangeFace(FaceAnim::YEAH);
			win_state = WIN_PERFORMANCE_NERU::PULL;
		}
		break;
	case Neru::WIN_PERFORMANCE_NERU::PULL:
		eye = YR_Vector3(pos.x+(apply*-18.0f), pos.y + 5.0f, pos.z - 43.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 8.0f * elapsed_time);
		if (win_timer > 1.8f)
		{
			win_state = WIN_PERFORMANCE_NERU::STOP;
		}
		break;
	case Neru::WIN_PERFORMANCE_NERU::STOP:
		if (win_timer > 3.0f)
		{
			win_state = WIN_PERFORMANCE_NERU::FINISH;
		}
		break;
	case Neru::WIN_PERFORMANCE_NERU::FINISH:
		return true;
		break;
	default:
		break;
	}
	win_timer += elapsed_time;

	return false;
}

bool Neru::Intro(float elapsed_time)
{
	//�C���g���̍s��������
	//�J������������œ�����
	YR_Vector3	focus;
	YR_Vector3	eye;

	switch (intro_state)
	{
	case Neru::INTRO_NERU::SET:
		//���_��ݒ�(������)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y + 1.8f, pos.z + 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_NERU::WAIT;
		lip_text = RandTextSelect();
		GetSound().SESinglePlay(SEKind::INTRO_WIND);
		break;
	case Neru::INTRO_NERU::WAIT:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y +1.8f, pos.z + 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f*elapsed_time);
		if (intro_timer > 1.0f)
		{
			intro_state = INTRO_NERU::ZOOM_SET;
		}
		break;
	case Neru::INTRO_NERU::ZOOM_SET:
		//���_��ݒ�(������)
		eye = YR_Vector3(pos.x - Getapply(25.0f), pos.y + 5.0f, pos.z - 45.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_NERU::ZOOM;
		ChangeFace(FaceAnim::NORMAL_LIP_SYNC);
		break;
	case Neru::INTRO_NERU::ZOOM:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.5f*elapsed_time);
		text_on = true;

		if (intro_timer > 3.0f)
		{
			intro_state = INTRO_NERU::PULL;
			ChangeFace(FaceAnim::YARUKI);
			text_on = false;
		}
		break;
	case Neru::INTRO_NERU::PULL:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.0f)
		{
			intro_state = INTRO_NERU::PUSH;
		}
		break;
	case Neru::INTRO_NERU::PUSH:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 41.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.5f)
		{
			intro_state = INTRO_NERU::PULL_2;
		}
		break;
	case Neru::INTRO_NERU::PULL_2:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x, pos.y + 30.0f, pos.z - 85.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.9f)
		{
			intro_state = INTRO_NERU::PUSH_2;
		}
		break;
	case Neru::INTRO_NERU::PUSH_2:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x+Getapply(27.0f), pos.y + 5.4f, pos.z - 38.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 5.3f)
		{
			intro_state = INTRO_NERU::FIN_SET;
		}
		break;
	case Neru::INTRO_NERU::FIN_SET:
		//���_��ݒ�(������)
		eye = YR_Vector3(pos.x - Getapply(30.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x , pos.y, pos.z - 5.4f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_NERU::FIN;
		break;
	case Neru::INTRO_NERU::FIN:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x - Getapply(48.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x - Getapply(5.0f), pos.y, pos.z - 5.4f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 8.0f * elapsed_time);
		if (intro_timer > 6.0f)
		{
			intro_state = INTRO_NERU::FINISH;
		}
		break;
	case Neru::INTRO_NERU::FINISH:
		return true;
		break;
	default:
		break;
	}
	intro_timer += elapsed_time;
	/*if (intro_timer > 6.0f)
	{
		return true;
	}*/

	return false;
}


//�C���g����A�Q�[���J�n�܂ł̐ݒ���s��
void Neru::ReadySet()
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
	//GetSound().SEPlay(SEKind::INTRO_WIND);
}


//�J���������p�f�o�b�O�֐�
void Neru::IntroDEBUG()
{
#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		ImGui::Begin(u8"�C���g���J����");
		int state_in = scastI(intro_state);
		ImGui::InputInt(u8"�X�e�[�g", &state_in, 1, 10);
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

void Neru::WinDEBUG()
{
#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		ImGui::Begin(u8"�����J����");
		int state_in = scastI(win_state);
		ImGui::InputInt(u8"�X�e�[�g", &state_in, 1, 10);
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




void Neru::AttackDetailsSet(const AttackState& attack_state)
{
	//�U�����Ƃɏ��񂾂��ʂ��������L�q����
	switch (attack_state)
	{
	case AttackState::NONE:
		break;
	case AttackState::JAKU:
		break;
	case AttackState::THU:
		break;
	case AttackState::D_KYO:
		break;
	case AttackState::D_JAKU:
		break;
	case AttackState::D_THU:
		break;
	case AttackState::U_KYO:
		break;
	case AttackState::A_JAKU:
		break;
	case AttackState::A_THU:
		break;
	case AttackState::A_KYO:
		break;
	case AttackState::A_UKYO:
		//speed.y = 0.0f;
		//speed_Y.Set(attack_list[scastI(attack_state)].advance_speed);
		break;
	case AttackState::STEAL:
		break;
	case AttackState::SLOW:
		break;
	case AttackState::TRACK_DASH:
		break;
	case AttackState::JAKU_RHURF:
		break;
	case AttackState::THU_RHURF:
		break;
	case AttackState::KYO_RHURF:
		GetSound().SESinglePlay(SEKind::SPECIAL_ATTACK);
		break;
	case AttackState::JAKU_LHURF:
		break;
	case AttackState::THU_LHURF:
		break;
	case AttackState::KYO_LHURF:
		break;
	case AttackState::SPECIAL_ATTACK:
		YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
		camera_state_neru = CAMERA_STATE_NERU::FIRST;
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
		break;
	case AttackState::JAKU_KYO:
		break;
	case AttackState::ATTACK_END:
		break;
	default:
		break;
	}
}

std::wstring Neru::RandTextSelect()
{
	//�C���g�����ɕ\������e�L�X�g�������_���ɐݒ肷��

	TextList rnd = static_cast<TextList>(rand() % scastI(TextList::TEXT_END));

	switch (rnd)
	{
	case Neru::TextList::NORMAL://�ʏ�
		return std::wstring(L"�����A�n�߂悤���I");
		break;
	case Neru::TextList::WARLIKE://�D��I
		return std::wstring(L"��C�ɍs����I");
		break;
	case Neru::TextList::CRIOSITY://�D��S
		return std::wstring(L"���Ɛ킢�����́H");
		break;
	case Neru::TextList::TEXT_END:
		break;
	default:
		break;
	}
}


//�f�o�b�O�������蔻�蒲���c�[���`��
void Neru::DrawDEBUGHitParam()
{
	if (ImGui::TreeNode(u8"�v���C���[�����蔻�蒲��"))
	{
		if (ImGui::TreeNode(u8"�s�������蔻��"))
		{
			for (int act = 0; act < scastI(ActState::ACT_END); act++)
			{
				if (ImGui::TreeNode(GetName().act_name_list[act].c_str()))
				{
					for (int list = 0; list < hitparam_list.size(); list++)
					{
						//�v���C���[�̓����蔻������ꂼ��o��
						if (ImGui::TreeNode(hit_name_list_Neru[list].c_str()))
						{
							ImGui::SliderFloat(u8"�v���C���[�Ƃ̋���X", &hitparam_list[list].act_parameter[act].distance.x, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"�v���C���[�Ƃ̋���Y", &hitparam_list[list].act_parameter[act].distance.y, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"�傫��X", &hitparam_list[list].act_parameter[act].size.x, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"�傫��Y", &hitparam_list[list].act_parameter[act].size.y, 0.0f, 50.0f);
							//int state = scastI(hitparam_list[list].act_parameter[act].state);
							//ImGui::SliderInt(u8"���", &state, 0, scastI(HitBoxState::END)-1);
							//hitparam_list[list].act_parameter[act].state = static_cast<HitBoxState>(state);
							//ImGui::Text(hitstate_name_list[state].c_str());
							ImGui::InputFloat(u8"���[�V�������x : ����", &ac_act[act].fream, 0.01f, 0.1f);
							ImGui::InputFloat(u8"���[�V�������x : ����", &ac_act[act].timer, 0.01f, 0.1f);
							ImGui::InputFloat(u8"���[�V�������x : ��X�L", &ac_act[act].later, 0.01f, 0.1f);
							ImGui::Text(u8"���̂ɂ���Ă�fream�����g�p���Ȃ����̂�����̂Œ���");
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"�U�������蔻��"))
		{
			for (int atk = 0; atk < scastI(AttackState::ATTACK_END); atk++)
			{
				if (ImGui::TreeNode(GetName().attack_name_list[atk].c_str()))
				{
					for (int list = 0; list < hitparam_list.size(); list++)
					{
						if (ImGui::TreeNode(hit_name_list_Neru[list].c_str()))
						{
							ImGui::SliderFloat(u8"�v���C���[�Ƃ̋���X", &hitparam_list[list].attack_parameter[atk].distance.x, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"�v���C���[�Ƃ̋���Y", &hitparam_list[list].attack_parameter[atk].distance.y, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"�傫��X", &hitparam_list[list].attack_parameter[atk].size.x, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"�傫��Y", &hitparam_list[list].attack_parameter[atk].size.y, 0.0f, 50.0f);
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