#pragma once

#include "Define.h"

BEGIN(Engine)
END
class CGameManager
{
public:
	DECLARE_SINGLETON(CGameManager)
private:
	CGameManager();
	~CGameManager();

public:
	void		Ready_Game_Manager(LPDIRECT3DDEVICE9 pGraphicDev);

	void		Serving_Food(CGameObject* pObject);
	_int		Get_Score() { return m_iScore; }
	_int		Get_Count() { return m_iSuccesCount; }

	void		End_Game();

	HRESULT		ChangeResultScene();

	HRESULT		Create_Dirty_Plate();

public:
	_int		m_iScore = 0;
	_int		m_iSuccesCount = 0;
private:
	LPDIRECT3DDEVICE9					m_pGraphicDev;

};

