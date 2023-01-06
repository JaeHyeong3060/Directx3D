#include "stdafx.h"
#include "UIManager.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
{
	
}

CUIManager::~CUIManager()
{
}

void CUIManager::Ready_UI_Manager(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
}

HRESULT CUIManager::Add_Menu(wstring wstrKey)
{
	Sort_Menu();

	CGameObject* pGameObject = nullptr;

	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(L"UI");

	pGameObject = CMenuO::Create(m_pGraphicDev);

	if (wstrKey == L"SoupOnion")
	{
		dynamic_cast<CMenuO*>(pGameObject)->Set_MenuTex(L"SoupOnion");
	}

	if (wstrKey == L"SoupTomato")
	{
		dynamic_cast<CMenuO*>(pGameObject)->Set_MenuTex(L"SoupTomato");
	}
	if (wstrKey == L"Salad")
	{
		dynamic_cast<CMenuO*>(pGameObject)->Set_MenuTex(L"Salad");
	}

	auto ExMapList = CurLayer->Get_ExMapObject(L"UI");
	int iSize = 0;
	for (auto& iter : *ExMapList)
	{
		if (iter->Get_ObjTag() == L"Menu")
			iSize++;
	}
	pGameObject->Set_UI_Pos(iSize * m_fMenuOffsetfX + 70.f, iSize * m_fMenuOffsetfY);
	pGameObject->Set_UI_DestPos(iSize * m_fMenuOffsetfX + 70.f, 70.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	CurLayer->Add_ExGameObject(L"UI", pGameObject);

	return S_OK;
}

HRESULT CUIManager::Delete_Menu(wstring wstrKey)
{
	return S_OK;
}

HRESULT CUIManager::Sort_Menu()
{
	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(L"UI");

	auto ExMapList = CurLayer->Get_ExMapObject(L"UI");
	int iSize = 0;
	for (auto& iter : *ExMapList)
	{
		if (iter->Get_ObjTag() == L"Menu")
		{
			iter->Set_UI_DestPos(iSize * m_fMenuOffsetfX + 70.f, 70.f);
			iSize++;
		}
	}

	return S_OK;
}
