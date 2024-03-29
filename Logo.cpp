#include "stdafx.h"
#include "Logo.h"
#include "Stage.h"

#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);


	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	
	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			CScene*		pScene = nullptr;

			pScene = CStage::Create(m_pGraphicDev);
			//pScene = CResultScene::Create(m_pGraphicDev);

			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Set_Scene(pScene), E_FAIL);

			return iExit;
		}
	}

	return iExit;
}

void CLogo::Render_Scene(void)
{
	// debug 용
	//Render_Font(L"Font_Jinji", m_pLoading->Get_String(), &_vec2(10.f, 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

HRESULT CLogo::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	CGameObject*			pGameObject = nullptr;

	// background
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	/*pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);*/

	
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{

	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	FAR, FAG, FAB		 	(1 - FA)R, (1 - FA)G, (1 - FA)B

	PLAYER->Render();


	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/


	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/

	// 이 둘은 직접 지정해주지 않으면 기본값으로 TRUE 상태
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);		// 깊이버퍼에 깊이 값을 무조건 기록은 한다, 하지만 자동 정렬을 수행할 지 말 지 결정
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); // 깊이버퍼에 깊이 값을 저장할 지 말 지 결정


	return S_OK;
}

HRESULT CLogo::Ready_UI_Layer(const _tchar * pLayerTag)
{
	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);

	CScene::Free();
}
HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Player", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TYPE_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/Logo.png", TYPE_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);

	return S_OK;
}
