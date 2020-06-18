#pragma once
#include <windows.h>

class KeyState {
public:
	//キー関連
	int leftflg = 0;
	int rightflg = 0;
	int upflg = 0;
	int downflg = 0;
	int aflg = 0;
	int bflg = 0;
	int cflg = 0;
	int dflg = 0;
	int eflg = 0;
	int fflg = 0;
	int gflg = 0;
	int hflg = 0;
	int iflg = 0;
	int jflg = 0;
	int kflg = 0;
	int lflg = 0;
	int mflg = 0;
	int nflg = 0;
	int oflg = 0;
	int pflg = 0;
	int qflg = 0;
	int rflg = 0;
	int sflg = 0;
	int tflg = 0;
	int uflg = 0;
	int vflg = 0;
	int wflg = 0;
	int xflg = 0;
	int yflg = 0;
	int zflg = 0;
	int k0flg = 0;
	int k1flg = 0;
	int k2flg = 0;
	int k3flg = 0;
	int k4flg = 0;
	int k5flg = 0;
	int k6flg = 0;
	int k7flg = 0;
	int k8flg = 0;
	int k9flg = 0;
	int spaceflg = 0;
	int backflg = 0;
	int lshiftflg = 0;
	void CheckKeyState() { //キーを押しているか
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) { leftflg++; } //1の時は押した瞬間
		else { leftflg = 0; } //離している時は0
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { rightflg++; }
		else { rightflg = 0; }
		if (GetAsyncKeyState(VK_UP) & 0x8000) { upflg++; }
		else { upflg = 0; }
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) { downflg++; }
		else { downflg = 0; }
		if (GetAsyncKeyState('A') & 0x8000) { aflg++; }
		else { aflg = 0; }
		if (GetAsyncKeyState('B') & 0x8000) { bflg++; }
		else { bflg = 0; }
		if (GetAsyncKeyState('C') & 0x8000) { cflg++; }
		else { cflg = 0; }
		if (GetAsyncKeyState('D') & 0x8000) { dflg++; }
		else { dflg = 0; }
		if (GetAsyncKeyState('E') & 0x8000) { eflg++; }
		else { eflg = 0; }
		if (GetAsyncKeyState('F') & 0x8000) { fflg++; }
		else { fflg = 0; }
		if (GetAsyncKeyState('G') & 0x8000) { gflg++; }
		else { gflg = 0; }
		if (GetAsyncKeyState('H') & 0x8000) { hflg++; }
		else { hflg = 0; }
		if (GetAsyncKeyState('I') & 0x8000) { iflg++; }
		else { iflg = 0; }
		if (GetAsyncKeyState('J') & 0x8000) { jflg++; }
		else { jflg = 0; }
		if (GetAsyncKeyState('K') & 0x8000) { kflg++; }
		else { kflg = 0; }
		if (GetAsyncKeyState('L') & 0x8000) { lflg++; }
		else { lflg = 0; }
		if (GetAsyncKeyState('M') & 0x8000) { mflg++; }
		else { mflg = 0; }
		if (GetAsyncKeyState('N') & 0x8000) { nflg++; }
		else { nflg = 0; }
		if (GetAsyncKeyState('O') & 0x8000) { oflg++; }
		else { oflg = 0; }
		if (GetAsyncKeyState('P') & 0x8000) { pflg++; }
		else { pflg = 0; }
		if (GetAsyncKeyState('Q') & 0x8000) { qflg++; }
		else { qflg = 0; }
		if (GetAsyncKeyState('R') & 0x8000) { rflg++; }
		else { rflg = 0; }
		if (GetAsyncKeyState('S') & 0x8000) { sflg++; }
		else { sflg = 0; }
		if (GetAsyncKeyState('T') & 0x8000) { tflg++; }
		else { tflg = 0; }
		if (GetAsyncKeyState('U') & 0x8000) { uflg++; }
		else { uflg = 0; }
		if (GetAsyncKeyState('V') & 0x8000) { vflg++; }
		else { vflg = 0; }
		if (GetAsyncKeyState('W') & 0x8000) { wflg++; }
		else { wflg = 0; }
		if (GetAsyncKeyState('X') & 0x8000) { xflg++; }
		else { xflg = 0; }
		if (GetAsyncKeyState('Y') & 0x8000) { yflg++; }
		else { yflg = 0; }
		if (GetAsyncKeyState('Z') & 0x8000) { zflg++; }
		else { zflg = 0; }
		if (GetAsyncKeyState('0') & 0x8000) { k0flg++; }
		else { k0flg = 0; }
		if (GetAsyncKeyState('1') & 0x8000) { k1flg++; }
		else { k1flg = 0; }
		if (GetAsyncKeyState('2') & 0x8000) { k2flg++; }
		else { k2flg = 0; }
		if (GetAsyncKeyState('3') & 0x8000) { k3flg++; }
		else { k3flg = 0; }
		if (GetAsyncKeyState('4') & 0x8000) { k4flg++; }
		else { k4flg = 0; }
		if (GetAsyncKeyState('5') & 0x8000) { k5flg++; }
		else { k5flg = 0; }
		if (GetAsyncKeyState('6') & 0x8000) { k6flg++; }
		else { k6flg = 0; }
		if (GetAsyncKeyState('7') & 0x8000) { k7flg++; }
		else { k7flg = 0; }
		if (GetAsyncKeyState('8') & 0x8000) { k8flg++; } 
		else { k8flg = 0; }
		if (GetAsyncKeyState('9') & 0x8000) { k9flg++; } 
		else { k9flg = 0; }
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) { spaceflg++; } 
		else { spaceflg = 0; }
		if (GetAsyncKeyState(VK_BACK) & 0x8000) { backflg++; }
		else { backflg = 0; }
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) { lshiftflg++; }
		else { lshiftflg = 0; }
	}

	static KeyState& GetInstance() {
		static KeyState instance;
		return instance;
	}

};
#define pKeyState (KeyState::GetInstance())