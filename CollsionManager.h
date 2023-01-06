#pragma once

//#define COL_CRATE				0x0001
//#define COL_WALL				0x0002
//#define COL_WASHING				0x0004
//#define COL_FLOOR				0x0008


//enum COL_KEY { CRATE, WALL, WASHING, COL_KEY_END };
#include "Define.h"

BEGIN(Engine)
END
class CCollsionManager
{
public:
	DECLARE_SINGLETON(CCollsionManager)
private:
	CCollsionManager();
	~CCollsionManager();

public:

	//_dw		 Get_ColKey()		  { return m_dwColState; }
	//void	 Add_ColKey(_dw eKey) { m_dwColState |= eKey; }
	//void	 Del_ColKey(_dw eKey) 
	//{
	//	if (m_dwColState & eKey)
	//	{
	//		m_dwColState ^= eKey;
	//	}
	//}

private:
	//_dw		m_dwColState=0;
};

