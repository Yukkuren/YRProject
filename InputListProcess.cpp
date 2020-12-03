#include "Scene.h"

void InputGamePadDraw::Init()
{
	//初期化
	for (int i = 0; i < p1_input_list.size(); i++)
	{
		p1_input_list[i].button_input = -1;
		p1_input_list[i].stick_input = -1;
	}

	for (int i = 0; i < p2_input_list.size(); i++)
	{
		p2_input_list[i].button_input = -1;
		p2_input_list[i].stick_input = -1;
	}
}


void InputGamePadDraw::Update(std::list<InputListor>que1, std::list<InputListor>que2)
{
	//それぞれのプレイヤーの入力を取得し、入力リストに入れていく
	if (!que1.empty())
	{
		int i = 0;
		for (auto& it : que1)
		{
			p1_input_list[i].kind = it.kind;
			i++;
		}
	}
	if (!que2.empty())
	{
		int i = 0;
		for (auto& it : que2)
		{
			p2_input_list[i].kind = it.kind;
			i++;
		}
	}

	//入手した入力データをボタン、スティックにそれぞれ割り当てる
}

void InputGamePadDraw::Draw(YRShader* shader)
{

}