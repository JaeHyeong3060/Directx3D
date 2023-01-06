#include "stdafx.h"
#include "GameManager.h"
#include "Management.h"

#include "Engine_Fuction.h"

IMPLEMENT_SINGLETON(CGameManager)


CGameManager::CGameManager()
{
	
}

CGameManager::~CGameManager()
{
}

void CGameManager::Ready_Game_Manager(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
}

void CGameManager::Serving_Food(CGameObject* pObject)
{
	wstring wstrContentTag = static_cast<CPlate*>(pObject)->Get_PlateContentTag();

	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(L"UI");

	list<CGameObject*>* pExUIObject = CurLayer->Get_ExMapObject(L"UI");

	for (auto& iter : *pExUIObject)
	{
		if (iter->Get_ObjTag() == L"Menu")
		{
			wstring MenuTag = static_cast<CMenuO*>(iter)->Get_wstrMenuTag();
			if (MenuTag == wstrContentTag)
			{
				m_iScore += 10;
				m_iSuccesCount += 1;
				(iter)->Set_Dead();
			}
		}
	}
	CUIManager::GetInstance()->Sort_Menu();

	pObject->Set_Dead();
	/*for (auto iter = (*pExUIObject).begin(); iter != (*pExUIObject).end();)
	{
		if ((*iter)->Get_ObjTag() == L"Menu")
		{
			wstring MenuTag = static_cast<CMenuO*>(*iter)->Get_wstrMenuTag();
			if (MenuTag == wstrContentTag)
			{
				printf("메뉴가 일치했다!!!!!!\n");
				(*iter)->Set_Dead();
				iter++;
			}
		}
	}*/

	CurLayer = CurScene->Get_Layer(L"GameLogic");
	list<CGameObject*>* pThePassExit = CurLayer->Get_ExMapObject(L"ThePassExit");
	dynamic_cast<CThePassExit*>(pThePassExit->front())->Set_Trigger(true);
}

void CGameManager::End_Game()
{
	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(L"GameLogic");

	CGameObject* pGameObject = nullptr;

	pGameObject = CUITexture::Create(m_pGraphicDev);
	static_cast<CUITexture*>(pGameObject)->Set_MenuTex(L"TimeUp");

	NULL_CHECK(pGameObject, E_FAIL);
	CurLayer->Add_ExGameObject(L"TimeUp", pGameObject);


}

HRESULT CGameManager::ChangeResultScene()
{
	CScene* pScene = nullptr;

	//			pScene = CStage::Create(m_pGraphicDev);
	pScene = CResultScene::Create(m_pGraphicDev);

	NULL_CHECK_RETURN(pScene, E_FAIL);
	CManagement::GetInstance()->Set_Scene(pScene);
}

HRESULT CGameManager::Create_Dirty_Plate()
{

	/*auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(L"GameLogic");

	list<CGameObject*>* pThePassExit = CurLayer->Get_ExMapObject(L"ThePassExit");
	dynamic_cast<CThePassExit*>(pThePassExit->front())->Set_Trigger(true);*/

	/*CGameObject* pGameObject = nullptr;

	pGameObject = CThePassExit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	auto Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	Transform->Set_Pos(&_vec3{ 14.097f, 0.1f, 12.41f });
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"ThePassExit", pGameObject), E_FAIL);*/
	return true;
}
