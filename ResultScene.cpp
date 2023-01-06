#include "stdafx.h"
#include "ResultScene.h"


#include "Export_Function.h"

CResultScene::CResultScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CResultScene::~CResultScene(void)
{

}

HRESULT CResultScene::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"OnionHouseMusicCut.wav");

	return S_OK;
}

Engine::_int CResultScene::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	CKeyManager::GetInstance()->Update_Key_Manager();

	if (m_fTotalDelay > 0)
		m_fTotalDelay -= fTimeDelta;

	if (m_fTotalDelayB > 0)
		m_fTotalDelayB -= fTimeDelta;

	return CScene::Update_Scene(fTimeDelta);
}

void CResultScene::Render_Scene(void)
{
	// debug ¿ë
		//++m_dwRenderCnt;

	//if (m_fTime >= 1.f)
	//{
	//	wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
	//	m_fTime = 0.f;
	//	m_dwRenderCnt = 0;
	//}
	if (m_fTotalDelay <= 0)
	{
		auto iCount = CGameManager::GetInstance()->Get_Count();
		wsprintf(m_szFPS, L"%d  X  10  = ", iCount);

		Render_Font(L"Font_Result", m_szFPS, &_vec2(300.f, 310.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

		if (m_bOnceEnv)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Shop0.wav", CSoundMgr::ENV);
			m_bOnceEnv = false;
		}
	}
	if (m_fTotalDelayB <= 0)
	{
		auto iCount = CGameManager::GetInstance()->Get_Count();
		wsprintf(m_szFPS, L"%d", iCount * 10);

		Render_Font(L"Font_Result", m_szFPS, &_vec2(470.f, 310.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

		if (m_bOnceEnvB)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Shop0.wav", CSoundMgr::PLAYER_A);
			m_bOnceEnvB = false;
		}
	}
}

void CResultScene::Add_EffectObj(CGameObject* pObj)
{
	m_mapLayer[L"UI"]->Add_ExGameObject(L"Effect",pObj);
}

HRESULT CResultScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*			pGameObject = nullptr;


	return S_OK;
}

HRESULT CResultScene::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CResultScene::Ready_UI_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	pGameObject = CUIResult::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"UI", pGameObject), E_FAIL);

	pGameObject = CUIScoreStar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CUIScoreStar*>(pGameObject)->Set_UI_Pos(320.f, 370.f);
	static_cast<CUIScoreStar*>(pGameObject)->Set_Delay(7.f);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"UIStarA", pGameObject), E_FAIL);

	pGameObject = CUIScoreStar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CUIScoreStar*>(pGameObject)->Set_Delay(9.f);
	static_cast<CUIScoreStar*>(pGameObject)->Set_UI_Pos(400.f, 370.f);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"UIStarB", pGameObject), E_FAIL);

	pGameObject = CUIScoreStar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	static_cast<CUIScoreStar*>(pGameObject)->Set_Delay(11.f);
	static_cast<CUIScoreStar*>(pGameObject)->Set_UI_Pos(480.f, 370.f);
	FAILED_CHECK_RETURN(pLayer->Add_ExGameObject(L"UIStarC", pGameObject), E_FAIL);





	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CResultScene::Ready_LightInfo(void)
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


CResultScene* CResultScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CResultScene* pInstance = new CResultScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CResultScene::Free(void)
{
	CScene::Free();
}
