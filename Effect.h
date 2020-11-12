#pragma once

#include "EXlib/Effekseer.h"
#include "EXlib/EffekseerRendererDX11.h"

#include <vector>
#include <DirectXMath.h>
//----------------------------------
//		�G�t�F�N�g�N���X
//----------------------------------

enum class EffectKind : int
{
	GUARD = 0,
	END,
};


class YR_Effect
{
private:
	const int square_max_count = 8000;				//�G�t�F�N�g�������ɏo�����b�V���̍ő吔

	std::vector<Effekseer::Handle> handles;			//�G�t�F�N�g�Đ��p�n���h��
	std::vector<Effekseer::Effect*> effects;		//�G�t�F�N�g�{��

	Effekseer::Manager *manager;					//�G�t�F�N�g�}�l�[�W���[
	EffekseerRendererDX11::Renderer* renderer;		//�G�t�F�N�g�����_���[
public:
	//�R���X�g���N�^
	YR_Effect() {};

	//�f�X�g���N�^
	~YR_Effect();

	//������
	void Init();

	//�J�����������_���[�ɃZ�b�g����
	void CameraSet();

	//�A�j���[�V�����̍Đ��J�n
	void PlayEffect(
		EffectKind kind,
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& axis,
		const float& angle);

	//�A�j���[�V�����Đ��̒�~
	void StopEffect(EffectKind kind);

	//�G�t�F�N�g�̕`��
	void Draw();

	static YR_Effect& getInstance()
	{
		static YR_Effect instance;
		return instance;
	}
};

inline YR_Effect& YRGetEffect()
{
	return YR_Effect::getInstance();
}
