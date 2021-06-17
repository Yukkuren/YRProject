#ifndef _COLIST_H_
#define _COLIST_H_

#include<array>
//#include"Common.h"

constexpr float TIMER_LIMIT = 0.5;				//�R�}���h�^�C�}�[�����̐��ȉ��Ȃ�R�}���h������J�n����
constexpr float timer_max_command = 1.0f;		//���̐��ȉ��Ȃ�R�}���h�^�C�}�[�𑝂₷
#define		scastI				static_cast<int>

//�R�}���h����p��
enum class Command : int
{
	NOCOMMAND = 0,	//�R�}���h�͖���
	RHURF,			//�O�����R�}���h236(�E�����̏ꍇ)
	LHURF,			//������R�}���h214(�E�����̏ꍇ)
	END,
};

class CommandList
{
private:
	int						lastinput;		//�����ł��炤�ŏI����
	int						command_start;	//�Ȃɂ���
	std::array<int, 10>		histry = { 0 };	//list�̃f�[�^�������Ŏ󂯎��
	Command					state;			//���ݔ��肵�Ă���R�}���h
	int						numI;			//�R�}���h�̓��X�g�̉������画�肷�邩��ۑ�����ϐ�
public:
	bool trigger = false;					//�R�}���h�𔻒肷�邩�ǂ����̃g���K�[
	float command_timer = 0.0f;				//�R�}���h�P�\����

	Command CommandCheack(int last, int* hist, float distans);
	void Reset();
};



#endif // !_COLIST_H_


