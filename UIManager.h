#pragma once

#include "Define.h"

BEGIN(Engine)
END
class CUIManager
{
public:
	DECLARE_SINGLETON(CUIManager)
private:
	CUIManager();
	~CUIManager();

public:
	void Ready_UI_Manager(LPDIRECT3DDEVICE9 pGraphicDev);

	HRESULT Add_Menu(wstring wstrKey);
	HRESULT Delete_Menu(wstring wstrKey);
	HRESULT Sort_Menu();

public:

private:

	_float m_fMenuOffsetfX = 150.f;
	_float m_fMenuOffsetfY = 0.f;

	_float m_fMenuSizeX = 130.f;
	_float m_fMenuSizeY = 130.f;

	LPDIRECT3DDEVICE9					m_pGraphicDev;
};

