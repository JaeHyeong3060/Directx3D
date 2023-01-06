#include "stdafx.h"
#include "UITimerBack.h"

#include "Export_Function.h"

CUITimerBack::CUITimerBack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CUITimerBack::CUITimerBack(const CUITimerBack& rhs)
	: CGameObject(rhs)
{

}

CUITimerBack::~CUITimerBack(void)
{
}

HRESULT CUITimerBack::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"UITimerBack";
	
	m_fX = 700.f;
	m_fY = 500.f;

	m_fDestfX = 800.f;

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;

	return S_OK;
}

Engine::_int CUITimerBack::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return -1;

	m_fTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	if (m_dwRenderCnt <= 0 && m_bEnd == false)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"TimesUpSting.wav", CSoundMgr::MONSTER);
		CGameManager::GetInstance()->End_Game();
		m_bEnd = true;
	}
	if (m_bEnd)
	{
		m_fEndtime -= fTimeDelta;
	}
	if (m_fEndtime < 0)
	{
		CGameManager::GetInstance()->ChangeResultScene();
		return -1;
		// ExGameObject에서 실행하면 문제생김 당연히
	}


	if (m_dwRenderCnt<185.f && !m_bOnce1)
	{
		CUIManager::GetInstance()->Add_Menu(L"SoupOnion");	
		m_bOnce1 = true;
	}
	if (m_dwRenderCnt < 160.f && !m_bOnce2)
	{
		CUIManager::GetInstance()->Add_Menu(L"Salad");
		m_bOnce2 = true;
	}
	if (m_dwRenderCnt < 120.f && !m_bOnce3)
	{
		CUIManager::GetInstance()->Add_Menu(L"SoupTomato");
		m_bOnce3 = true;
	}
	if (m_dwRenderCnt < 100.f && !m_bOnce4)
	{
		CUIManager::GetInstance()->Add_Menu(L"Salad");
		m_bOnce4 = true;
	}
	if (m_dwRenderCnt < 70.f && !m_bOnce5)
	{
		CUIManager::GetInstance()->Add_Menu(L"SoupOnion");
		m_bOnce5 = true;
	}
	if (m_dwRenderCnt < 60.f && !m_bOnce6)
	{
		CUIManager::GetInstance()->Add_Menu(L"SoupTomato");
		m_bOnce6 = true;
	}
	if (m_dwRenderCnt < 30.f && !m_bOnce7)
	{
		CUIManager::GetInstance()->Add_Menu(L"Salad");
		m_bOnce7 = true;
	}
	if (m_dwRenderCnt < 10.f && !m_bOnce8)
	{
		CUIManager::GetInstance()->Add_Menu(L"SoupOnion");
		m_bOnce8 = true;
	}

	return 0;
}

void CUITimerBack::Render_Object(void)
{
	

	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX * 0.5f;
	matWorld._22 = m_fSizeY * 0.5f;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szTimer, L"%d", m_dwRenderCnt);

		if(m_dwRenderCnt>0)
			m_dwRenderCnt -= 1;

		m_fTime = 0.f;
	}
	
	Render_Font(L"Font_Jinji", m_szTimer, &_vec2(660.f, 505.f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.9f));	
}

HRESULT CUITimerBack::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UITimerBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;

}

CUITimerBack* CUITimerBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUITimerBack*	pInstance = new CUITimerBack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUITimerBack::Free(void)
{
	CGameObject::Free();
}

