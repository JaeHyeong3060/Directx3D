#include "stdafx.h"
#include "Stage.h"


#include "Export_Function.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"FeelinGoodCut.wav");

	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	CKeyManager::GetInstance()->Update_Key_Manager();

	return CScene::Update_Scene(fTimeDelta);
}

void CStage::Render_Scene(void)
{
	// debug ¿ë
	//++m_dwRenderCnt;

	//if (m_fTime >= 1.f)
	//{
	//	wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
	//	m_fTime = 0.f;
	//	m_dwRenderCnt = 0;
	//}

	//Render_Font(L"Font_Jinji", m_szFPS, &_vec2(400.f, 20.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

}

void CStage::Add_EffectObj(CGameObject* pObj)
{
	m_mapLayer[L"UI"]->Add_ExGameObject(L"Effect",pObj);
}

HRESULT CStage::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*			pGameObject = nullptr;

	// DynamicCamera
	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, 
	//	&_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), 
	//	D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	pGameObject = CPlayerCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 25.f, -15.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(30.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"PlayerCamera", pGameObject), E_FAIL);

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);
	
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	// Player
	pGameObject = CChef::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Player", pGameObject), E_FAIL);

	// PlayerB
	pGameObject = CChefB::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Player", pGameObject), E_FAIL);

	pGameObject = CPlate::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Plate", pGameObject), E_FAIL);

	

	Load_GameData(pLayer);
	Load_TilePos(pLayer);


	pGameObject = CThePassExit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	auto Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	Transform->Set_Pos(&_vec3{ 14.097f, 0.1f, 12.41f });
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"ThePassExit", pGameObject), E_FAIL);



	//pGameObject = CThePassExit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//auto Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	//Transform->Set_Pos(&_vec3{ 17.5f, 0.1f, 12.5f });
	//FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Empty", pGameObject), E_FAIL);
	
//
//	
//
//#pragma region PLAYER

//
//	// Sword
//	//pGameObject = CSword::Create(m_pGraphicDev);
//	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
//	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Sword", pGameObject), E_FAIL);
//#pragma endregion PLAYER
//
//
//	// Stone
//	pGameObject = CStone::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone", pGameObject), E_FAIL);
//
//	// Tree
//	pGameObject = CTree::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Tree", pGameObject), E_FAIL);
//
//	for (_ulong i = 0; i < 150; ++i)
//	{
//		// effect
//		pGameObject = CEffect::Create(m_pGraphicDev);
//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);
//	}
//	
	////effect
	//pGameObject = CEffect::Create(m_pGraphicDev);
	//static_cast<CEffect*>(pGameObject)->Set_TextureWstr(L"Effect_Chop");
	//static_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Pos(25.f, 1.f,10.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;


	// UI
	/*pGameObject = CTimeGauge::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);*/

	pGameObject = CUITimerBack::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	pGameObject = CUIScoreBack::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"UI", pGameObject), E_FAIL);

	//pGameObject = CDigitMgr::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CDigitMgr*>(pGameObject)->Set_Number(2503);
	//FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"UI", pGameObject), E_FAIL);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.f, 0.f, 1.f);
	tLightInfo.Position = _vec3(5.f, 5.f, 5.f);
	tLightInfo.Range = 10.f;

	FAILED_CHECK_RETURN(Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);
	
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.2f, 1.f);
	tLightInfo.Position = _vec3(10.f, 5.f, 10.f);
	tLightInfo.Range = 10.f;

	FAILED_CHECK_RETURN(Ready_Light(m_pGraphicDev, &tLightInfo, 2), E_FAIL);

	return S_OK;
}

HRESULT CStage::Load_GameData(CLayer* pLayer)
{
	//LoadDataPath(L"../../Data/map1.dat");

	HANDLE hFile = CreateFile(L"../../Data/mapTest.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStringCount = 0;
	TCHAR* szBuf = nullptr;
	ClientObjINFO* pObjInfo = nullptr;
	while (true)
	{
		ReadFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		szBuf = new TCHAR[dwStringCount];
		pObjInfo = new ClientObjINFO;
		ReadFile(hFile, szBuf, dwStringCount, &dwByte, nullptr);
		pObjInfo->wstrKey = szBuf;
		Safe_Delete_Array(szBuf);

		ReadFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
		szBuf = new TCHAR[dwStringCount];
		ReadFile(hFile, szBuf, dwStringCount, &dwByte, nullptr);
		pObjInfo->wstrObjName = szBuf;
		Safe_Delete_Array(szBuf);

		ReadFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
		szBuf = new TCHAR[dwStringCount];
		ReadFile(hFile, szBuf, dwStringCount, &dwByte, nullptr);
		pObjInfo->wstrMeshType = szBuf;
		Safe_Delete_Array(szBuf);

		_vec3 vScale;
		ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);
		_vec3 vAngle;
		ReadFile(hFile, &vAngle, sizeof(_vec3), &dwByte, nullptr);
		_vec3 vPos;
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);

		CGameObject* pGameObject = nullptr;

		if (pObjInfo->wstrKey == L"Stone")
		{
			pGameObject = CStone::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"Van")
		{
			pGameObject = CVan::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"MyMap")
		{
			pGameObject = CMyMap::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"Crate")
		{
			pGameObject = CCrate::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"CrateB")
		{
			pGameObject = CCrateB::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"Onion")
		{
			pGameObject = CCrateB::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"Pot")
		{
			pGameObject = CPot::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"WashingStation")
		{
			pGameObject = CWashingStation::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"CookingStation")
		{
			pGameObject = CCookingStation::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"ThePass")
		{
			pGameObject = CThePass::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"WorkTop")
		{
			pGameObject = CWorkTop::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"ChoppingBoard")
		{
			pGameObject = CChoppingBoard::Create(m_pGraphicDev);
		}
		if (pObjInfo->wstrKey == L"Chef")
		{
			pGameObject = CChef::Create(m_pGraphicDev);
		}

		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		auto transform = pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC);

		dynamic_cast<CTransform*>(transform)->m_vAngle = vAngle;
		dynamic_cast<CTransform*>(transform)->Set_Pos(&vPos);
		dynamic_cast<CTransform*>(transform)->Set_Scale(vScale.x, vScale.y, vScale.z);

		//FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(pObjInfo->wstrKey.c_str(), pGameObject), E_FAIL);
		
		FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(pObjInfo->wstrKey, pGameObject), E_FAIL);

	}
	CloseHandle(hFile);

}

#pragma region TILE_POS
HRESULT CStage::Load_TilePos(CLayer* pLayer)
{
	CGameObject* pGameObject = nullptr;

	_vec3 vOffset = { 15.f,0.f,25.f };
	vector<_vec3> vPosGroup;
	vPosGroup.push_back(_vec3{ -0.431f, 0.1f, -11.646f });
	
	vPosGroup.push_back(_vec3{ 0.513f, 0.1f, -11.646f });
	vPosGroup.push_back(_vec3{ 0.985f, 0.1f, -11.646f });
	vPosGroup.push_back(_vec3{ 1.929f, 0.1f, -11.646f });
	vPosGroup.push_back(_vec3{ 2.873f, 0.1f, -11.646f });

	vPosGroup.push_back(_vec3{ 3.817f, 0.1f, -12.121f });
	//vPosGroup.push_back(_vec3{ 3.817f, 0.1f, -12.596f });
	vPosGroup.push_back(_vec3{ 3.817f, 0.1f, -14.021f });
	vPosGroup.push_back(_vec3{ 3.817f, 0.1f, -14.496f });

	vPosGroup.push_back(_vec3{ 3.345f, 0.1f, -14.971f });
	vPosGroup.push_back(_vec3{ 3.345f, 0.1f, -14.971f });
	vPosGroup.push_back(_vec3{ 2.873f, 0.1f, -14.971f });
	vPosGroup.push_back(_vec3{ 2.401f, 0.1f, -14.971f });
	vPosGroup.push_back(_vec3{ 1.929f, 0.1f, -14.971f });
	vPosGroup.push_back(_vec3{ 1.457f, 0.1f, -14.971f });
	vPosGroup.push_back(_vec3{ 0.985f, 0.1f, -14.971f });
	vPosGroup.push_back(_vec3{ 0.041f, 0.1f, -14.971f });

	vPosGroup.push_back(_vec3{ -0.903f, 0.1f, -14.495f });
	vPosGroup.push_back(_vec3{ -0.903f, 0.1f, -14.009f });
	vPosGroup.push_back(_vec3{ -0.903f, 0.1f, -12.12f });


	vPosGroup.push_back(_vec3{ 1.457f, 0.1f, -12.121f });
	vPosGroup.push_back(_vec3{ 1.457f, 0.1f, -12.593f });
	vPosGroup.push_back(_vec3{ 1.457f, 0.1f, -13.065f });
	vPosGroup.push_back(_vec3{ 1.457f, 0.1f, -13.519f });

	_int iSize = vPosGroup.size();
	for (int i = 0; i < iSize; i++)
	{
		pGameObject = CEmpty::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		auto Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
		vPosGroup[i] += vOffset;
		Transform->Set_Pos(&vPosGroup[i]);
		FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Empty", pGameObject), E_FAIL);

		if (i == 11)
		{
			CGameObject* pStageGameObject = nullptr;
			pStageGameObject = CPot::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			auto Transform = dynamic_cast<CTransform*>(pStageGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
			Transform->Set_Pos(&(vPosGroup[i] + _vec3{ 0.f,0.1f,0.f }));
			FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Pot", pStageGameObject), E_FAIL);

			pGameObject->Add_Inven(pStageGameObject);
		}
		if (i == 12)
		{
			CGameObject* pStageGameObject = nullptr;
			pStageGameObject = CPlate::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			auto Transform = dynamic_cast<CTransform*>(pStageGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
			Transform->Set_Pos(&(vPosGroup[i] + _vec3{ 0.f,0.1f,0.f }));
			FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Plate", pStageGameObject), E_FAIL);

			pGameObject->Add_Inven(pStageGameObject);
		}
		if (i == 8)
		{
			CGameObject* pStageGameObject = nullptr;
			pStageGameObject = CPlate::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			auto Transform = dynamic_cast<CTransform*>(pStageGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
			Transform->Set_Pos(&(vPosGroup[i] + _vec3{ 0.f,0.1f,0.f }));
			FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"Plate", pStageGameObject), E_FAIL);

			pGameObject->Add_Inven(pStageGameObject);
		}
	}

	pGameObject = CThePass::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	auto Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	_vec3 vPos = { -0.9f ,0.012f, -13.299f};
	vPos += vOffset;
	Transform->Set_Pos(&vPos);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"ThePass", pGameObject), E_FAIL);

	pGameObject = CCrateC::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos = { 18.817f ,0.012f, 12.404f };
	Transform->Set_Pos(&vPos);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"CrateC", pGameObject), E_FAIL);

	pGameObject = CNPC_Buck::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos = { -2.924f ,0.012f, -13.081f };
	vPos += vOffset;
	Transform->Set_Pos(&vPos);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"NPC_Buck", pGameObject), E_FAIL);

	pGameObject = CNPC_Business::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos = { -2.924f ,0.025f, -14.114f };
	vPos += vOffset;
	Transform->Set_Pos(&vPos);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"NPC_Buck", pGameObject), E_FAIL);

	pGameObject = CNPC_Mel::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos = { 3.642f ,0.025f, -10.294f };
	vPos += vOffset;
	Transform->Set_Pos(&vPos);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"NPC_Buck", pGameObject), E_FAIL);

	pGameObject = CNPC_Penguin::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos = { -2.557f ,0.025f, -10.937f };
	vPos += vOffset;
	Transform->Set_Pos(&vPos);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"NPC_Buck", pGameObject), E_FAIL);

	pGameObject = CNPC_Waiter::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos = { 5.472f ,0.025f, -12.087f };
	vPos += vOffset;
	Transform->Set_Pos(&vPos);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"NPC_Buck", pGameObject), E_FAIL);


	return S_OK;
}
#pragma endregion

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	CScene::Free();
}
