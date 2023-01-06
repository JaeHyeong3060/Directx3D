#include "stdafx.h"
#include "UIScoreStar.h"

#include "Export_Function.h"

CUIScoreStar::CUIScoreStar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CUIScoreStar::CUIScoreStar(const CUIScoreStar& rhs)
	: CGameObject(rhs)
{

}

CUIScoreStar::~CUIScoreStar(void)
{
}

HRESULT CUIScoreStar::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"UIScoreStar";
	
	m_fX = 700.f;
	m_fY = 500.f;

	m_fDestfX = 800.f;

	m_fSizeX = 80.f;
	m_fSizeY = 80.f;

	return S_OK;
}

Engine::_int CUIScoreStar::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return -1;

	if (m_fDelay < 0)
	{
		CGameObject::Update_Object(fTimeDelta);

		Add_RenderGroup(RENDER_UI, this);

		D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

		m_pTransformCom->Rotation(ROT_Y, 0.001f);

		if (m_bOnceEnv)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SYSTME);
			CSoundMgr::Get_Instance()->PlaySound(L"Shop0.wav",CSoundMgr::SYSTME);
			m_bOnceEnv = false;
		}
	}
	else
		m_fDelay -= fTimeDelta;


	return 0;
}

void CUIScoreStar::Render_Object(void)
{
	if (m_fDelay > 0)
		return;
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
}

HRESULT CUIScoreStar::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UIScoreStar"));
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

CUIScoreStar* CUIScoreStar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIScoreStar*	pInstance = new CUIScoreStar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUIScoreStar::Free(void)
{
	CGameObject::Free();
}

