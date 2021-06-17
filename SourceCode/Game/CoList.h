#ifndef _COLIST_H_
#define _COLIST_H_

#include<array>
//#include"Common.h"

constexpr float TIMER_LIMIT = 0.5;				//コマンドタイマーがこの数以下ならコマンド判定を開始する
constexpr float timer_max_command = 1.0f;		//この数以下ならコマンドタイマーを増やす
#define		scastI				static_cast<int>

//コマンド判定用列挙
enum class Command : int
{
	NOCOMMAND = 0,	//コマンドは無い
	RHURF,			//前方向コマンド236(右向きの場合)
	LHURF,			//後方向コマンド214(右向きの場合)
	END,
};

class CommandList
{
private:
	int						lastinput;		//引数でもらう最終入力
	int						command_start;	//なにこれ
	std::array<int, 10>		histry = { 0 };	//listのデータを引数で受け取る
	Command					state;			//現在判定しているコマンド
	int						numI;			//コマンドはリストの何処から判定するかを保存する変数
public:
	bool trigger = false;					//コマンドを判定するかどうかのトリガー
	float command_timer = 0.0f;				//コマンド猶予時間

	Command CommandCheack(int last, int* hist, float distans);
	void Reset();
};



#endif // !_COLIST_H_


