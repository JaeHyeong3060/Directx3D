#pragma once
//0000	0001
#define KEY_LBUTTON 0x0001
//0000	0010
#define KEY_RBUTTON 0x0002
//0000	0100
#define KEY_LEFT	0x0004
//0000	1000
#define KEY_RIGHT	0x0008
//0001	0000
#define KEY_UP		0x0010
//0010	0000
#define KEY_DOWN	0x0020
#define KEY_Z		0x0040
#define KEY_X		0x0080
#define KEY_RETURN	0x0100
#define KEY_P		0x0200
#define KEY_T		0x0400
#define KEY_F		0x0800
#define KEY_G		0x1000
#define KEY_Y		0x2000

#include "Define.h"

class CKeyManager
{
	DECLARE_SINGLETON(CKeyManager)
private:
	explicit CKeyManager();
	~CKeyManager();
public:
	void Set_KeyLock(bool lock) { m_bIsKeyLock = lock; }
	bool Get_KeyLock() { return m_bIsKeyLock; }
	DWORD Get_Key() { return m_dwKey; }
public:
	void Update_Key_Manager();
	bool Key_Up(DWORD dwKey);
	bool Key_Down(DWORD dwKey);
	bool Key_Pressing(DWORD dwKey);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyUP;

	bool m_bIsKeyLock;
};
