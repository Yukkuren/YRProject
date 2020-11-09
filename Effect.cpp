#include "Effect.h"
#include "CoList.h"
#include "framework.h"

YR_Effect::YR_Effect()
{
	//�G�t�F�N�g�����_���[�̍쐬
	renderer = EffekseerRendererDX11::Renderer::Create(FRAMEWORK.device.Get(), FRAMEWORK.context.Get(), square_max_count);

	//�G�t�F�N�g�}�l�[�W���[�̐���
	manager = Effekseer::Manager::Create(square_max_count);

	//�`�惂�W���[���̍쐬
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	//�e�N�X�`���A���f���A�}�e���A�����[�_�[�̐ݒ������
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());

	//�n���h���ƃG�t�F�N�g�𐶐�
	handles.resize(scastI(EffectKind::END));
	effects.resize(scastI(EffectKind::END));

	//�G�t�F�N�g�̓ǂݍ���
	EFK_CHAR efk_char = EFK_CHAR("./Data/Effect/guard.efk");
	effects[scastI(EffectKind::GUARD)] = Effekseer::Effect::Create(manager, &efk_char);

	//�n���h���̏�����
	for (int i = 0; i < handles.size(); i++)
	{
		handles[i] = 0;
	}
}

YR_Effect::~YR_Effect()
{
	//�G�t�F�N�g�̉��
	for (int i = 0; i < effects.size(); i++)
	{
		ES_SAFE_RELEASE(effects[i]);
	}
	effects.clear();

	//�n���h���̉��
	for (int i = 0; i < handles.size(); i++)
	{
		handles[i] = 0;
	}
	handles.clear();

	// �}�l�[�W���[�̔j��
	manager->Destroy();

	// �����_���[�̔j��
	renderer->Destroy();
}

//�J�����̃Z�b�g
void YR_Effect::CameraSet()
{
	//�J�����̈ʒu���擾
	DirectX::XMFLOAT3 eye = YRCamera.GetEye();
	
	//�J�����̒����_���擾
	DirectX::XMFLOAT3 focus = YRCamera.GetFocus();

	//�J�����̏�������擾
	DirectX::XMFLOAT3 up = YRCamera.GetUp();

	//�J�����̃A�X�y�N�g����擾
	float aspect = YRCamera.GetAspect();
	float fov = YRCamera.GetFov();
	float Near = YRCamera.GetNear();
	float Far = YRCamera.GetFar();

	// ���_�ʒu���m��
	Effekseer::Vector3D g_position = ::Effekseer::Vector3D(eye.x, eye.y, eye.z);
	// ���_�������m��
	Effekseer::Vector3D g_at = ::Effekseer::Vector3D(focus.x, focus.y, focus.z);
	// ���_��������m��
	Effekseer::Vector3D g_up = ::Effekseer::Vector3D(up.x, up.y, up.z);

	// ���e�s���ݒ�
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		fov, aspect, Near, Far));

	// �J�����s���ݒ�
	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(g_position, g_at, g_up));
}

//�A�j���[�V�����̕`��
void YR_Effect::Draw()
{
	//�}�l�[�W���[�̍X�V
	manager->Update();
	
	// �G�t�F�N�g�̕`��J�n�������s���B
	renderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	renderer->EndRendering();
}

//�A�j���[�V�����̍Đ�
void YR_Effect::PlayEffect(
	EffectKind kind,
	const DirectX::XMFLOAT3& pos,
	const DirectX::XMFLOAT3& scale,
	const DirectX::XMFLOAT3& axis,
	const float& angle)
{
	// �G�t�F�N�g�̈ړ�
	//manager->AddLocation(handles[scastI(kind)], ::Effekseer::Vector3D(pos.x, pos.y, pos.z));

	// �G�t�F�N�g�̈ړ�
	manager->SetLocation(handles[scastI(kind)], ::Effekseer::Vector3D(pos.x, pos.y, pos.z));

	// �G�t�F�N�g�̉�]
	manager->SetRotation(handles[scastI(kind)], ::Effekseer::Vector3D(axis.x, axis.y, axis.z),angle);

	// �G�t�F�N�g�̊g��k��
	manager->SetScale(handles[scastI(kind)], scale.x,scale.y,scale.z);

	// �G�t�F�N�g�̍Đ�
	//handles[scastI(kind)] = manager->Play(effects[scastI(kind)], 0, 0, 0);
	handles[scastI(kind)] = manager->Play(effects[scastI(kind)], pos.x,pos.y,pos.z);
}

//�A�j���[�V�����Đ��̒�~
void YR_Effect::StopEffect(EffectKind kind)
{
	// �G�t�F�N�g�̒�~
	manager->StopEffect(handles[scastI(kind)]);
}