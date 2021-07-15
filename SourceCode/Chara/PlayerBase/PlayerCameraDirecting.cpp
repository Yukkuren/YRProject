#include "PlayerBase.h"
#include "../../framework.h"
#include <fstream>
#include "Player_name_list.h"
//#include "../../camera.h"


void CameraDirecting::Init(int now_player)
{
	//������
	now_event = 0;
	timer = 0.0f;
	this->now_player = now_player;

	if (!camera_event.empty())
	{
		for (int i = 0; i < camera_event.size(); i++)
		{
			camera_event[i].executed = false;
		}
	}
	//YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
}


void CameraDirecting::Load(PLSELECT chara_name, std::string filename)
{
	//�t�@�C������ǂݍ���
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name_s = GetName().chara_name_list[scastI(chara_name)];

	std::string AP_level = f_level + chara_name_s + std::string("/") + filename + std::string("CameraDirecting.txt");
	std::ifstream ifs(AP_level);
	std::string str;
	int size = 0;

	std::getline(ifs, str);
	ifs >> size;
	camera_event.resize(size);

	std::getline(ifs, str);
	std::getline(ifs, str);
	ifs >> max_fream;

	std::getline(ifs, str);
	std::getline(ifs, str);
	ifs >> anim_ccodinate;

	std::getline(ifs, str);
	std::getline(ifs, str);
	ifs >> anim_have;

	if (!camera_event.empty())
	{
		//���g�̓ǂݍ���
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
			//�G�t�F�N�g�����l�ǂݍ���
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
			ifs >> camera_event[i].text_on;

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

	//������������G���[���o��
	if (ifs.fail())
	{
		assert("ifs failled");
	}
	//�t�@�C�������
	ifs.close();
}


void CameraDirecting::Write(PLSELECT chara_name, std::string filename)
{
	//�t�@�C���ɏ����o��
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name_s = GetName().chara_name_list[scastI(chara_name)];

	std::string AP_level = f_level + chara_name_s + std::string("/") + filename + std::string("CameraDirecting.txt");
	std::ofstream outputfile(AP_level);

	outputfile << "�C�x���g���v" << std::endl;
	outputfile << camera_event.size() << std::endl;

	outputfile << "�ő�t���[��" << std::endl;
	outputfile << max_fream << std::endl;

	outputfile << "�A�j���[�V���������l" << std::endl;
	outputfile << anim_ccodinate << std::endl;

	outputfile << "�A�j���������s��" << std::endl;
	outputfile << anim_have << std::endl;

	if (!camera_event.empty())
	{
		//���g�̏����o��
		for (int i = 0; i < camera_event.size(); i++)
		{
			outputfile << "�C�x���g" << i << std::endl;

			outputfile << "�J�������_" << std::endl;
			outputfile << camera_event[i].camera_eye.x << std::endl;
			outputfile << camera_event[i].camera_eye.y << std::endl;
			outputfile << camera_event[i].camera_eye.z << std::endl;

			outputfile << "�J���������_" << std::endl;
			outputfile << camera_event[i].camera_focus.x << std::endl;
			outputfile << camera_event[i].camera_focus.y << std::endl;
			outputfile << camera_event[i].camera_focus.z << std::endl;

			outputfile << "�J������x�N�g��" << std::endl;
			outputfile << camera_event[i].camera_up.x << std::endl;
			outputfile << camera_event[i].camera_up.y << std::endl;
			outputfile << camera_event[i].camera_up.z << std::endl;

			outputfile << "��p" << std::endl;
			outputfile << camera_event[i].fov << std::endl;

			outputfile << "�ҋ@����" << std::endl;
			outputfile << camera_event[i].event_point << std::endl;

			outputfile << "�J�����𓮂����Ȃ���" << std::endl;
			outputfile << camera_event[i].wait_camera << std::endl;

			//�G�t�F�N�g�����l�����o��
			outputfile << "�G�t�F�N�g�̎��" << std::endl;
			outputfile << scastI(camera_event[i].effect_param.effect_kind) << std::endl;

			outputfile << "���W�̈ʒu�̍�" << std::endl;
			outputfile << camera_event[i].effect_param.distance.x << std::endl;
			outputfile << camera_event[i].effect_param.distance.y << std::endl;
			outputfile << camera_event[i].effect_param.distance.z << std::endl;

			outputfile << "�G�t�F�N�g�̑傫��" << std::endl;
			outputfile << camera_event[i].effect_param.scale.x << std::endl;
			outputfile << camera_event[i].effect_param.scale.y << std::endl;
			outputfile << camera_event[i].effect_param.scale.z << std::endl;

			outputfile << "�p�x�̕���" << std::endl;
			outputfile << camera_event[i].effect_param.axis.x << std::endl;
			outputfile << camera_event[i].effect_param.axis.y << std::endl;
			outputfile << camera_event[i].effect_param.axis.z << std::endl;

			outputfile << "�p�x" << std::endl;
			outputfile << camera_event[i].effect_param.angle << std::endl;

			outputfile << "�p�x���v���C���[�Ɉˑ������邩�ǂ���" << std::endl;
			outputfile << camera_event[i].effect_param.rightORleft << std::endl;

			outputfile << "�\��" << std::endl;
			outputfile << scastI(camera_event[i].face_kind) << std::endl;

			outputfile << "�e�L�X�g" << std::endl;
			outputfile << camera_event[i].text_on << std::endl;

			outputfile << "�T�E���h�̎��" << std::endl;
			outputfile << scastI(camera_event[i].se_kind) << std::endl;

			outputfile << "�J�����̃��N�G�X�g" << std::endl;
			outputfile << scastI(camera_event[i].camera_req) << std::endl;

			outputfile << "�J�������X�e�[�g�œ�������" << std::endl;
			outputfile << camera_event[i].camera_move << std::endl;
		}
	}


	outputfile.close();
}


bool CameraDirecting::CameraEventUpdate(float elapsed_time, Player* player)	//�����F�t���[��,�v���C���[�̃A�h���X
{
	//-------------------------------------------------------------
	//	�C�x���g����
	//-------------------------------------------------------------
	//
	//	�E�^�C�}�[�𑝂₵�Ă����Aexecuted��false���^�C�}�[��eventpoint�𒴂������̂����s���Ă���
	//	�Ecamera_move��false�ȊO�̂��̂́A���̃C�x���g�̃J�����̈ʒu���Q�Ƃ��A���X�ɋ߂Â���(XMVECTOR�̊֐����g�p)
	//	�E�Ō�̃C�x���g�ɂȂ�����J������Release���Atrue��Ԃ��悤�ɂ���
	//	�E�C�x���g��for���őS�Ċm�F���Ă���
	//----------------------------------------------------------

	if (timer >= max_fream)
	{
		return true;
	}

	if (anim_have)
	{
		player->anim_ccodinate = anim_ccodinate;
	}

	this->decision = player->rightOrleft;

	//�J�����C�x���g���X�V���Ă���
	if (camera_event.empty())
	{
		//���g����Ȃ珈�������Ȃ�
		return true;
	}
	else
	{
		//�C�x���g���񂵂Ă���

		if (timer < max_fream && !test)
		{
			//�^�C�}�[�𑝂₵�Ă���
			timer += elapsed_time;
		}

		//���݂̃C�x���g�ԍ����Z�o����
		NowEventCheck();

		for (int e = 0; e < camera_event.size(); e++)
		{
			//�^�C�}�[���C�x���g�|�C���g�ȏ�ɂȂ�����
			if (camera_event[e].event_point <= timer)
			{
				//�܂����s���Ă��Ȃ��Ȃ�
				if (!camera_event[e].executed)
				{
					//�������s��

					//���݂̃C�x���g�ԍ���ۑ�(���ƂŕύX)
					//now_event = e;

					//�J�����̃��N�G�X�g���w��
					YRCamera.RequestCamera(camera_event[e].camera_req, now_player);

					//�J�����𓮂����w��������ꍇ
					if (camera_event[e].camera_move)
					{
						//�C�x���g�X�e�[�g����J�����𓮂���
						CameraStateUpdate(player->pos);
					}

					//SE���Đ�����
					GetSound().SESinglePlay(camera_event[e].se_kind);

					//�G�t�F�N�g�𐶐�
					SetEffect(player->pos);

					//�\���ύX����
					player->ChangeFace(camera_event[e].face_kind);

					//�e�L�X�g���o��
					player->text_on = camera_event[e].text_on;
				}
			}
		}

		//�C�x���g�̎��s�m�F�𔻕ʂ���
		ExecuteEvent();

		//�J�����X�V
		CameraUpdate(player->pos);

		if (timer >= max_fream)
		{
			//�^�C�}�[���ő�t���[���𒴂�����
			YRCamera.RequestCamera(Camera::Request::RELEASE, now_player);
			for (int i = 0; i < camera_event.size(); i++)
			{
				YRGetEffect().StopEffect(camera_event[i].effect_param.effect_kind, camera_event[i].handle);
			}
			return true;
		}
	}
	return false;
}




float CameraDirecting::FovCalculation(float fov)
{
	//��p��Ԃ�
	return fov * 0.01745f;
}



float CameraDirecting::Getapply(float n)
{
	//�v���C���[�̌��݂̌�������x�N�g����Ԃ�
	return n * decision;
}




void CameraDirecting::NowEventCheck()
{
	//���݂̃C�x���g�ԍ�������o��
	for (int i = 0; i < camera_event.size(); i++)
	{
		if (camera_event[i].event_point <= timer)
		{
			//�^�C�}�[���C�x���g�|�C���g�ȏ�Ȃ�
			now_event = i;
		}
	}
}





void CameraDirecting::ExecuteEvent()
{
	//�^�C�}�[���߂����C�x���g�����s�ς݂ɂ���
	for (int i = 0; i < camera_event.size(); i++)
	{
		if (camera_event[i].event_point <= timer)
		{
			//�^�C�}�[���C�x���g�|�C���g�ȏ�Ȃ�
			camera_event[i].executed = true;
		}
		else
		{
			//�^�C�}�[���C�x���g�|�C���g�ȉ��Ȃ�
			camera_event[i].executed = false;
		}
	}
}




void CameraDirecting::CameraStateUpdate(YR_Vector3 pos)
{
	//�J�������X�e�[�g���g�p���ē�����
	YR_Vector3 eye,focus,up;

	//���_
	eye.x = pos.x + Getapply(camera_event[now_event].camera_eye.x);
	eye.y = pos.y + camera_event[now_event].camera_eye.y;
	eye.z = pos.z + camera_event[now_event].camera_eye.z;

	//�����_
	focus.x = pos.x + Getapply(camera_event[now_event].camera_focus.x);
	focus.y = pos.y + camera_event[now_event].camera_focus.y;
	focus.z = pos.z + camera_event[now_event].camera_focus.z;

	//��x�N�g��
	up.x = camera_event[now_event].camera_focus.x;
	up.y = camera_event[now_event].camera_focus.y;
	up.z = camera_event[now_event].camera_focus.z;

	//�J������ݒ肷��
	YRCamera.SetEye(eye.GetDXFLOAT3());
	YRCamera.SetFocus(focus.GetDXFLOAT3());
	//YRCamera.SetUp(up.GetDXFLOAT3());
	YRCamera.SetFov(FovCalculation(camera_event[now_event].fov));
	YRCamera.Active();
}



void CameraDirecting::CameraUpdate(YR_Vector3 pos)
{
	//�J�����X�V

	//���݂̃C�x���g���玟�̃C�x���g�܂łɃJ�����𓮂����Ă���
	if (now_event == camera_event.size() - 1)
	{
		//���݂̃C�x���g���Ō�Ȃ牽�����Ȃ�
		CameraStateUpdate(pos);
		return;
	}

	if (camera_event[now_event].wait_camera)
	{
		//�J�����𓮂����Ȃ��ꍇ�͉������Ȃ�
		CameraStateUpdate(pos);
		return;
	}

	//Larp�֐����g�p���Č��݂̃J�����̈ʒu������o��

	//���݂̃J�����X�e�[�g
	DirectX::XMFLOAT3 now_eye;
	now_eye.x = pos.x + Getapply(camera_event[now_event].camera_eye.x);
	now_eye.y = pos.y + camera_event[now_event].camera_eye.y;
	now_eye.z = pos.z + camera_event[now_event].camera_eye.z;
	DirectX::XMFLOAT3 now_focus;
	now_focus.x = pos.x + Getapply(camera_event[now_event].camera_focus.x);
	now_focus.y = pos.y + camera_event[now_event].camera_focus.y;
	now_focus.z = pos.z + camera_event[now_event].camera_focus.z;
	DirectX::XMFLOAT3 now_up = camera_event[now_event].camera_up;
	float now_fov = FovCalculation(camera_event[now_event].fov);

	//���̃J�����X�e�[�g
	DirectX::XMFLOAT3 next_eye;
	next_eye.x = pos.x + Getapply(camera_event[now_event + 1].camera_eye.x);
	next_eye.y = pos.y + camera_event[now_event + 1].camera_eye.y;
	next_eye.z = pos.z + camera_event[now_event + 1].camera_eye.z;
	DirectX::XMFLOAT3 next_focus;
	next_focus.x = pos.x + Getapply(camera_event[now_event + 1].camera_focus.x);
	next_focus.y = pos.y + camera_event[now_event + 1].camera_focus.y;
	next_focus.z = pos.z + camera_event[now_event + 1].camera_focus.z;
	DirectX::XMFLOAT3 next_up = camera_event[now_event + 1].camera_up;
	float next_fov = FovCalculation(camera_event[now_event + 1].fov);

	//���ꂼ��x�N�^�[�ɕύX
	DirectX::XMVECTOR now_eye_vector = DirectX::XMLoadFloat3(&now_eye);
	DirectX::XMVECTOR next_eye_vector = DirectX::XMLoadFloat3(&next_eye);
	DirectX::XMVECTOR now_focus_vector = DirectX::XMLoadFloat3(&now_focus);
	DirectX::XMVECTOR next_focus_vector = DirectX::XMLoadFloat3(&next_focus);
	DirectX::XMVECTOR now_up_vector = DirectX::XMLoadFloat3(&now_up);
	DirectX::XMVECTOR next_up_vector = DirectX::XMLoadFloat3(&next_up);
	DirectX::XMVECTOR now_fov_vector = DirectX::XMLoadFloat(&now_fov);
	DirectX::XMVECTOR next_vector = DirectX::XMLoadFloat(&next_fov);


	//Larp�֐��ɓn����Ԑ���W�������߂�
	float len = camera_event[now_event + 1].event_point - camera_event[now_event].event_point;

	float t = (timer - camera_event[now_event].event_point) / len;

	//Larp�֐����g���Č��݂̈ʒu�����߂�
	DirectX::XMVECTOR eye_larp = DirectX::XMVectorLerp(now_eye_vector, next_eye_vector, t);
	DirectX::XMVECTOR focus_larp = DirectX::XMVectorLerp(now_focus_vector, next_focus_vector, t);
	DirectX::XMVECTOR up_larp = DirectX::XMVectorLerp(now_up_vector, next_up_vector, t);
	DirectX::XMVECTOR fov_larp = DirectX::XMVectorLerp(now_fov_vector, next_vector, t);

	DirectX::XMFLOAT3 eye, focus, up;
	float fov;

	DirectX::XMStoreFloat3(&eye, eye_larp);
	DirectX::XMStoreFloat3(&focus, focus_larp);
	DirectX::XMStoreFloat3(&up, up_larp);
	DirectX::XMStoreFloat(&fov, fov_larp);

	YRCamera.SetEye(eye);
	YRCamera.SetFocus(focus);
	//YRCamera.SetUp(up);
	YRCamera.SetFov(fov);
	YRCamera.Active();

}




void CameraDirecting::SetEffect(YR_Vector3 pos)
{
	//�G�t�F�N�g�𐶐�����
	if (camera_event[now_event].effect_param.rightORleft)
	{
		//�v���C���[�̊p�x���ˑ�������ꍇ
		YRGetEffect().PlayEffect(
			camera_event[now_event].effect_param.effect_kind, camera_event[now_event].handle,
			DirectX::XMFLOAT3(pos.x + Getapply(camera_event[now_event].effect_param.distance.x), pos.y + camera_event[now_event].effect_param.distance.y, pos.z + camera_event[now_event].effect_param.distance.z),
			camera_event[now_event].effect_param.scale.GetDXFLOAT3(), camera_event[now_event].effect_param.axis.GetDXFLOAT3(), camera_event[now_event].effect_param.angle * decision);
	}
	else
	{
		//�ˑ������Ȃ��ꍇ
		YRGetEffect().PlayEffect(
			camera_event[now_event].effect_param.effect_kind, camera_event[now_event].handle,
			DirectX::XMFLOAT3(pos.x + Getapply(camera_event[now_event].effect_param.distance.x), pos.y + camera_event[now_event].effect_param.distance.y, pos.z + camera_event[now_event].effect_param.distance.z),
			camera_event[now_event].effect_param.scale.GetDXFLOAT3(), camera_event[now_event].effect_param.axis.GetDXFLOAT3(), camera_event[now_event].effect_param.angle);
	}
}




void CameraDirecting::DrawTimeLine(std::string timeline_name, Player* player, std::string filename)
{
	//�^�C�����C���̕`��
	ImGui::Begin(timeline_name.c_str());

	ImGui::SliderFloat(u8"�ő�t���[��", &max_fream, 0.0f, 5.0f);
	ImGui::SliderFloat(u8"���݂̃^�C�}�[", &timer, 0.0f, max_fream);
	ImGui::Checkbox(u8"�e�X�g�t���O", &test);
	DrawPlayerSetting();

	if (ImGui::Button(u8"�C�x���g�ǉ�"))
	{
		if (camera_event.empty())
		{
			//�C�x���g������Ȃ��ꍇ
			camera_event.push_back(CameraEvent());
			camera_event.back().camera_eye = YRCamera.GetEye();
			camera_event.back().camera_focus = YRCamera.GetFocus();
			camera_event.back().camera_up = YRCamera.GetUp();
			camera_event.back().fov = YRCamera.GetFov() / 0.01745f;
		}
		else
		{
			//���ɃC�x���g������ꍇ
			CameraEvent f_event = camera_event.back();
			camera_event.push_back(f_event);
			camera_event.back().event_point = timer;
		}
	}

	if (ImGui::TreeNode(u8"�C�x���g�폜"))
	{
		if (!camera_event.empty())
		{
			if (ImGui::Button(u8"�Ō�����폜"))
			{
				int size = camera_event.size() - 1;
				camera_event.resize(size);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::Button(u8"���[�h"))
	{
		ImGui::OpenPopup("Load");
	}
	if (ImGui::BeginPopup("Load"))
	{
		ImGui::Text(u8"���[�h���܂����H");
		if (ImGui::Button(u8"�͂�"))
		{
			Load(player->chara_name, filename);
			load_timer = 6.0f;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button(u8"������"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (load_timer > 0.0f)
	{
		ImGui::Text(u8"���[�h���܂���");
		load_timer -= 0.1f;
	}


	if (!camera_event.empty())
	{

		ImGui::Text("------------------------------------------------------------------------------");

		if (ImGui::Button(u8"�Z�[�u"))
		{
			ImGui::OpenPopup("Save");
		}//ImGui::SameLine();
		if (ImGui::BeginPopup("Save"))
		{
			ImGui::Text(u8"�Z�[�u���܂����H");
			if (ImGui::Button(u8"�͂�"))
			{
				Write(player->chara_name, filename);
				save_timer = 6.0f;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button(u8"������"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		if (save_timer > 0.0f)
		{
			ImGui::Text(u8"�Z�[�u���܂���");
			save_timer -= 0.1f;
		}

		if (ImGui::BeginTabBar(u8"�C�x���g", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
		{
			for (int i = 0; i < camera_event.size(); i++)
			{
				std::string tab_name = std::string(u8"�C�x���g") + std::to_string(i);
				if (ImGui::BeginTabItem(tab_name.c_str()))
				{
					ImGui::SliderFloat(u8"�C�x���g�����t���[��", &camera_event[i].event_point, 0.0f, max_fream);
					ImGui::SliderFloat(u8"�ʒuX", &camera_event[i].camera_eye.x, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"�ʒuY", &camera_event[i].camera_eye.y, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"�ʒuZ", &camera_event[i].camera_eye.z, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"�����_X", &camera_event[i].camera_focus.x, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"�����_Y", &camera_event[i].camera_focus.y, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"�����_Z", &camera_event[i].camera_focus.z, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"��X", &camera_event[i].camera_up.x, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"��Y", &camera_event[i].camera_up.y, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"��Z", &camera_event[i].camera_up.z, -100.0f, 100.0f);
					ImGui::SliderFloat(u8"��p", &camera_event[i].fov, 0.0f, 100.0f);
					ImGui::Checkbox(u8"�ҋ@���邩�ǂ���", &camera_event[i].wait_camera);

					int effect_k_h = scastI(camera_event[i].effect_param.effect_kind);
					ImGui::SliderInt(u8"�G�t�F�N�g�̎��", &effect_k_h, 0, scastI(EffectKind::END) - 1);
					ImGui::Text(GetName().effect_kind_name_list[scastI(camera_event[i].effect_param.effect_kind)].c_str());
					camera_event[i].effect_param.effect_kind = static_cast<EffectKind>(effect_k_h);

					if (camera_event[i].effect_param.effect_kind != EffectKind::NONE)
					{
						if (ImGui::TreeNode(u8"�G�t�F�N�g�ڍאݒ�"))
						{
							ImGui::SliderFloat(u8"���WX", &camera_event[i].effect_param.distance.x, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"���WY", &camera_event[i].effect_param.distance.y, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"���WZ", &camera_event[i].effect_param.distance.z, -50.0f, 50.0f);
							ImGui::SliderFloat(u8"�傫��X", &camera_event[i].effect_param.scale.x, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"�傫��Y", &camera_event[i].effect_param.scale.y, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"�傫��Z", &camera_event[i].effect_param.scale.z, 0.0f, 50.0f);
							ImGui::SliderFloat(u8"����X", &camera_event[i].effect_param.axis.x, 0.0, 1.0f);
							ImGui::SliderFloat(u8"����Y", &camera_event[i].effect_param.axis.y, 0.0, 1.0f);
							ImGui::SliderFloat(u8"����Z", &camera_event[i].effect_param.axis.z, 0.0, 1.0f);
							ImGui::SliderFloat(u8"�p�x", &camera_event[i].effect_param.angle, -360.0f, 360.0f);
							ImGui::Checkbox(u8"�p�x���v���C���[�ˑ��ɂ��邩", &camera_event[i].effect_param.rightORleft);

							ImGui::TreePop();
						}
					}

					int face = scastI(camera_event[i].face_kind);
					ImGui::SliderInt(u8"�\��", &face, 0, scastI(FaceAnim::END) - 1);
					ImGui::Text(GetName().face_name_list[face].c_str());
					camera_event[i].face_kind = static_cast<FaceAnim>(face);

					int se = scastI(camera_event[i].se_kind);
					ImGui::SliderInt(u8"�炷SE", &se, 0, scastI(SEKind::END) - 1);
					ImGui::Text(GetName().se_name_list[se].c_str());
					camera_event[i].se_kind = static_cast<SEKind>(se);

					int req = scastI(camera_event[i].camera_req);
					ImGui::SliderInt(u8"�J�������N�G�X�g", &req, 0, scastI(Camera::Request::Request_END) - 1);
					ImGui::Text(GetName().request_name_list[req].c_str());
					camera_event[i].camera_req = static_cast<Camera::Request>(req);

					ImGui::Checkbox(u8"�J�������X�e�[�^�X�œ�������", &camera_event[i].camera_move);

					ImGui::Checkbox(u8"�e�L�X�g���o��", &camera_event[i].text_on);

					/*if (camera_event[i].camera_eye.x == camera_event[i].camera_focus.x)
					{
						camera_event[i].camera_focus.x += 0.1f;
					}
					if (camera_event[i].camera_eye.y == camera_event[i].camera_focus.y)
					{
						camera_event[i].camera_focus.y += 0.1f;
					}*/
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


void CameraDirecting::DrawPlayerSetting()
{
	//�v���C���[���̐ݒ��\������
	ImGui::SliderFloat(u8"�A�j���[�V�����X�V���x", &anim_ccodinate, 0.0f, 3.0f);
	ImGui::Checkbox(u8"�A�j���[�V���������l��ێ�����", &anim_have);
	//if (anim_have)player->anim_ccodinate = anim_ccodinate;
}