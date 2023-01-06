#include "stdafx.h"
#include "DigitMgr.h"

#include "Export_Function.h"


CDigitMgr::CDigitMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CDigitMgr::~CDigitMgr(void)
{
}

HRESULT CDigitMgr::Set_Number(int number)
{
	m_iNumber = number;
	while (!m_vDigitIndex.empty())
	{
		m_vDigitIndex.pop_back();
	}
	while (!m_vDigitObject.empty())
	{
		m_vDigitObject.pop_back();
	}
	m_vDigitIndex.clear();
	m_vDigitObject.clear();

    while (true)
    {
        int lastNum = number % 10;
 
		m_vDigitIndex.push_back(lastNum);
       
        number /= 10;
        if (number == 0)
            break;
    }

	int iSize = m_vDigitIndex.size();
	for (int i = 0; i < iSize; i++)
	{
		CGameObject* pObject = nullptr;
		pObject = CUIDigit::Create(m_pGraphicDev);
		static_cast<CUIDigit*>(pObject)->Set_Number(m_vDigitIndex[i]);
		NULL_CHECK_RETURN(pObject, E_FAIL);
		m_vDigitObject.push_back(pObject);
	}
}

void CDigitMgr::Change_Number(int number)
{
}

HRESULT CDigitMgr::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"UI_Digit";

	m_iAlpha = 255;
	m_RGB = { 255,255,255 };

	m_fX = 550.f;
	m_fY = 500.f;

	m_fDestfX = 800.f;

	m_fSizeX = 20.f;
	m_fSizeY = 20.f;

	
	/*
	CTransform* Transform = dynamic_cast<CTransform*>(m_vecDigit->Get_Component(L"Com_Transform", ID_DYNAMIC));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Transform->Set_Pos(&vPos);*/

	return S_OK;
}

_int CDigitMgr::Update_Object(const _float& fTimeDelta)
{
	//m_iAlpha -= fTimeDelta;
	//m_vPos.y -= fTimeDelta;

	if (m_bIsDead)
		return -1;

	CGameObject::Update_Object(fTimeDelta);

	int iSize = m_vDigitObject.size();
	for (int i = 0; i < iSize; i++)
		m_vDigitObject[i]->Update_Object(fTimeDelta);

	m_fTime += fTimeDelta;

	if (m_fTime >= 1.f)
	{
		m_iNumber--;
		Set_Number(m_iNumber);
		m_fTime = 0.f;
	}


	Add_RenderGroup(RENDER_UI, this);

	//D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CDigitMgr::Render_Object(void)
{
	float fCenterX = 0.f;
	float fCenterY = 0.f;

	int iSize = m_vDigitObject.size();

	_float fOffset = 20.f;
	for (int i = 0; i < iSize; i++)
	{
		_float newfX = m_fX - fOffset*i;
		m_vDigitObject[i]->Set_UI_Pos(newfX, m_fY);
		m_vDigitObject[i]->Render_Object();
	}

	//_matrix		matWorld, matView, matOldView, matOldProj;

	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixIdentity(&matView);

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	//matWorld._11 = m_fSizeX * 0.5f;
	//matWorld._22 = m_fSizeY * 0.5f;
	//matWorld._33 = 1.f;
	//matWorld._41 = m_fX - WINCX * 0.5f;
	//matWorld._42 = -m_fY + WINCY * 0.5f;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//m_pTextureCom->Render_Texture();
	//m_pBufferCom->Render_Buffer();
	//
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}

HRESULT CDigitMgr::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Digit"));
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

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}


CDigitMgr* CDigitMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDigitMgr* pInstance = new CDigitMgr(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CDigitMgr::Free(void)
{
}