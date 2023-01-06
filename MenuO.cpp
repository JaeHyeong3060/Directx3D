#include "stdafx.h"
#include "MenuO.h"

#include "Export_Function.h"

CMenuO::CMenuO(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CMenuO::CMenuO(const CMenuO& rhs)
	: CGameObject(rhs)
{

}

CMenuO::~CMenuO(void)
{
	m_pObjectTimeGauge->Free();
}

HRESULT CMenuO::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pObjectTimeGauge = CTimeGauge::Create(m_pGraphicDev);

	m_wstrObjTag = L"Menu";
	
	m_fX = 70.f;
	m_fDestfX = 800.f;
	m_fY = 0.f;

	m_fSizeX = 130.f;
	m_fSizeY = 130.f;

	return S_OK;
}

Engine::_int CMenuO::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return -1;

	CGameObject::Update_Object(fTimeDelta);

	m_pObjectTimeGauge->Update_Object(fTimeDelta);

	if (m_fDestfX < m_fX)
	{
		m_fX -= 4.f;
		m_pObjectTimeGauge->Set_UI_Pos(m_pObjectTimeGauge->Get_UI_PosX() - 4.f, m_pObjectTimeGauge->Get_UI_PosY());
	}

	if (m_fY <= m_fDestfY)
	{
		if (!m_bIsEndLoad)
		{
			m_pObjectTimeGauge->Set_UI_Pos(m_fX, m_fY - 50.f);
			m_fY += 3.f;
		}
	}
	else
		m_bIsEndLoad = true;

	if (dynamic_cast<CTimeGauge*>(m_pObjectTimeGauge)->Get_IsEndGauge() == true)
	{
		m_bIsEndGauge = true;
		m_fY -= 5.f;
	}
	if (m_bIsEndGauge && m_fY < -m_fDestfY*0.5f)
		m_bIsDead = true;

	Add_RenderGroup(RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CMenuO::Render_Object(void)
{
	m_pObjectTimeGauge->Render_Object();

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

HRESULT CMenuO::Set_MenuTex(wstring wstrKey)
{
	CComponent* pComponent = nullptr;

	// texture
	if (wstrKey == L"SoupOnion")
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SoupOnion"));
		m_wstrMenuTag = L"SoupOnion";
	}
	if (wstrKey == L"SoupTomato")
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SoupTomato"));
		m_wstrMenuTag = L"SoupTomato";
	}
	if (wstrKey == L"Salad")
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Salad"));
		m_wstrMenuTag = L"Salad";
	}

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

}

HRESULT CMenuO::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);



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

CMenuO* CMenuO::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMenuO*	pInstance = new CMenuO(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMenuO::Free(void)
{
	CGameObject::Free();
}

