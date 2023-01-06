#include "stdafx.h"
#include "UIScoreBack.h"

#include "Export_Function.h"

CUIScoreBack::CUIScoreBack(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CUIScoreBack::CUIScoreBack(const CUIScoreBack& rhs)
	: CGameObject(rhs)
{

}

CUIScoreBack::~CUIScoreBack(void)
{
}

HRESULT CUIScoreBack::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"UIScoreBack";
	
	m_fX = 50.f;
	m_fY = 550.f;

	m_fDestfX = 800.f;

	m_fSizeX = 100.f;
	m_fSizeY = 100.f;

	return S_OK;
}

Engine::_int CUIScoreBack::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return -1;

	m_fTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CUIScoreBack::Render_Object(void)
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

	m_dwRenderCnt = CGameManager::GetInstance()->Get_Score();
	wsprintf(m_szTimer, L"%d", m_dwRenderCnt);

	Render_Font(L"Font_Jinji", m_szTimer, &_vec2(35.f, 535.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	
}

HRESULT CUIScoreBack::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UIScoreBack"));
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

CUIScoreBack* CUIScoreBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIScoreBack*	pInstance = new CUIScoreBack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUIScoreBack::Free(void)
{
	CGameObject::Free();
}

