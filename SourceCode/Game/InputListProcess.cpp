#include "../Scene/Scene.h"
#include "../framework.h"

void InputGamePadDraw::Init()
{
	//初期化
	p1_input_list.clear();
	p2_input_list.clear();
}


void InputGamePadDraw::Load()
{
	//スプライト読み込み
	if (button_img == nullptr)
	{
		button_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/button.png", 1280.0f, 2560.0f, 2, 4, 640.0f, 640.0f);
	}
	if (stick_img == nullptr)
	{
		stick_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/stick.png", 1280.0f, 3200.0f, 2, 5, 640.0f, 640.0f);
	}
}


void InputGamePadDraw::Update(Player* player1p, Player* player2p)
{
	//それぞれのプレイヤーの入力を取得し、入力リストに入れていく
	//入手した入力データをボタン、スティックにそれぞれ割り当てる

	//1P入力取得
	bool Generated = false;
	for (int i = 0; i < player1p->pad->x_input.size(); i++)
	{
		if (player1p->pad->x_input[i] == 1)
		{
			//このフレームに入力されていた
			int button = GetButtonNum(i);
			int stick = GetStickNum(i);

			if (button != -1 || stick != -1)
			{
				if (!Generated)
				{
					//1フレームに一度だけ生成する
					p1_input_list.push_back(Input_list_pad());
					Generated = true;
				}
			}
			if (stick != -1)
			{
				//スティックの入力があった場合、入力を保存する
				p1_input_list.back().stick_input = stick;
			}
			if (button != -1)
			{
				//ボタンの入力があった場合、リストを生成し入力を保存する
				p1_input_list.back().button_input.push_back(int());
				p1_input_list.back().button_input.back() = button;

				//コマンド判定を行い、成立している場合はそれぞれ保存する
				Command command = player1p->pad->com_list.CommandCheack(i, player1p->pad->input_history.data(), player1p->rightOrleft);
				if (command == Command::LHURF)
				{
					p1_input_list.back().stick_input = 9;
				}
				if (command == Command::RHURF)
				{
					p1_input_list.back().stick_input = 8;
				}
			}
		}

		if (Generated)
		{
			//このフレームで生成を行っていた場合
			if (p1_input_list.back().stick_input == -1)
			{
				//何もスティックの入力がなかった場合
				if (player1p->pad->x_input[i] > 0)
				{
					int stick = GetStickNum(i);
					if (stick != -1)
					{
						p1_input_list.back().stick_input = stick;
						p1_input_list.back().pushed = true;
					}
				}
			}
		}
		//10個以上になったら先頭から削除
		if (p1_input_list.size() > 10)
		{
			p1_input_list.pop_front();
		}
	}


	//2P入力取得
	Generated = false;
	for (int i = 0; i < player2p->pad->x_input.size(); i++)
	{
		if (player2p->pad->x_input[i] == 1)
		{
			//このフレームに入力されていた
			int button = GetButtonNum(i);
			int stick = GetStickNum(i);

			if (button != -1 || stick != -1)
			{
				if (!Generated)
				{
					//1フレームに一度だけ生成する
					p2_input_list.push_back(Input_list_pad());
					Generated = true;
				}
			}
			if (stick != -1)
			{
				//スティックの入力があった場合、入力を保存する
				p2_input_list.back().stick_input = stick;
			}
			if (button != -1)
			{
				//ボタンの入力があった場合、リストを生成し入力を保存する
				p2_input_list.back().button_input.push_back(int());
				p2_input_list.back().button_input.back() = button;

				//コマンド判定を行い、成立している場合はそれぞれ保存する
				Command command = player2p->pad->com_list.CommandCheack(i, player2p->pad->input_history.data(), player2p->rightOrleft);
				if (command == Command::LHURF)
				{
					p2_input_list.back().stick_input = 9;
				}
				if (command == Command::RHURF)
				{
					p2_input_list.back().stick_input = 8;
				}
			}
		}

		if (Generated)
		{
			//このフレームで生成を行っていた場合
			if (p2_input_list.back().stick_input == -1)
			{
				//何もスティックの入力がなかった場合
				if (player2p->pad->x_input[i] > 0)
				{
					int stick = GetStickNum(i);
					if (stick != -1)
					{
						p2_input_list.back().stick_input = stick;
						p2_input_list.back().pushed = true;
					}
				}
			}
		}
		//10個以上になったら先頭から削除
		if (p2_input_list.size() > 10)
		{
			p2_input_list.pop_front();
		}
	}

}

void InputGamePadDraw::Draw(YRShader* shader)
{
	float screen_weight = static_cast<float>(FRAMEWORK.SCREEN_WIDTH);
	float screen_height = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT);
	float size = 0.15f;
	float adjust_height = 100.0f;
	float adjust_weight_stick_p1 = 0.03f;
	float adjust_weight_stick_p2 = 0.97f;
	float adjust_weight_button_p1 = 0.09f;
	float adjust_weight_button_p2 = 0.92f;
	float adjust_weight_button = 100.0f;

	//1P入力表示
	int i = 0;
	for (auto& il : p1_input_list)
	{
		if (il.stick_input != -1)
		{
			//値が-1の場合は表示しない
			if (il.pushed)
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p1,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
			}
			else
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p1,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input);
			}
		}
		if (!il.button_input.empty())
		{
			for (int j = 0; j < il.button_input.size(); j++)
			{
				//値がない場合は表示しない
				button_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_button_p1 + (j * adjust_weight_button),
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.button_input[j]);
			}
		}
		i++;
	}

	//2P入力表示
	i = 0;
	for (auto& il : p2_input_list)
	{
		if (il.stick_input != -1)
		{
			//値が-1の場合は表示しない
			if (il.pushed)
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p2,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
			}
			else
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p2,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input);
			}
		}
		if (!il.button_input.empty())
		{
			for (int j = 0; j < il.button_input.size(); j++)
			{
				//値がない場合は表示しない
				button_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_button_p2 - (j * adjust_weight_button),
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.button_input[j]);
			}
		}
		i++;
	}
}

int InputGamePadDraw::GetButtonNum(int button)
{
	//引数で渡されたボタンが配列のどの場所にあるかを返す
	int returnNum = -1;
	for (int i = 0; i < button_img_list.size(); i++)
	{
		if (button_img_list[i] == button)
		{
			returnNum = i;
		}
	}
	return returnNum;
}


int InputGamePadDraw::GetStickNum(int stick)
{
	//引数で渡されたスティック番号が配列のどの場所にあるかを返す
	int returnNum = -1;
	for (int i = 0; i < stick_img_list.size(); i++)
	{
		if (stick_img_list[i] == stick)
		{
			returnNum = i;
		}
	}
	return returnNum;
}

void InputGamePadDraw::UnInit()
{
	button_img.reset();
	stick_img.reset();
	button_img = nullptr;
	stick_img = nullptr;
}