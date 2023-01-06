#include "stdafx.h"
#include "UIResult.h"

#include "Export_Function.h"

CUIResult::CUIResult(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CUIResult::CUIResult(const CUIResult& rhs)
	: CGameObject(rhs)
{

}

CUIResult::~CUIResult(void)
{
}

HRESULT CUIResult::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"UIResult";
	
	m_fX = 400.f;
	m_fY = 300.f;

	m_fDestfX = 800.f;

	m_fSizeX = 800.f;
	m_fSizeY = 600.f;



	return S_OK;
}

Engine::_int CUIResult::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return -1;

	m_fTime += fTimeDelta;

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CUIResult::Render_Object(void)
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

	//if (m_fTime >= 1.f)
	//{
	//	wsprintf(m_szTimer, L"%d", m_dwRenderCnt);
	//	m_dwRenderCnt -= 1;
	//	m_fTime = 0.f;
	//	//m_dwRenderCnt = 0;
	//}
	//Render_Font(L"Font_Jinji", m_szTimer, &_vec2(655.f, 505.f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.9f));	
}

HRESULT CUIResult::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UIResult"));
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

CUIResult* CUIResult::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIResult*	pInstance = new CUIResult(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUIResult::Free(void)
{
	CGameObject::Free();
}

