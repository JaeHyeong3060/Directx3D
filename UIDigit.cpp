#include "stdafx.h"
#include "UIDigit.h"

#include "Export_Function.h"


CUIDigit::CUIDigit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CUIDigit::~CUIDigit(void)
{
}

HRESULT CUIDigit::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"UI_Digit";

	m_iAlpha = 255;
	m_RGB = { 255,255,255 };

	m_fX = 500.f;
	m_fY = 500.f;

	m_fDestfX = 800.f;

	m_fSizeX = 20.f;
	m_fSizeY = 20.f;

	return S_OK;
}

_int CUIDigit::Update_Object(const _float& fTimeDelta)
{
	//m_iAlpha -= fTimeDelta;
	//m_vPos.y -= fTimeDelta;

	if (m_bIsDead)
		return -1;

	CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CUIDigit::Render_Object(void)
{
	float fCenterX = 0.f;
	float fCenterY = 0.f;

	//if (iSize != 0)
	//{
	//	if (m_iAlpha <= 0)
	//	{
	//		m_bIsDead = true;
	//	}
	//	for (int i = 0; i < iSize; i++)
	//	{
	//		m_pTextureCom[m_vDigit[i]];
	//		//m_vDigitTexture.push_back(m_pTextureCom[m_vDigit[i]]);

	//		//fCenterX = curTextInfo->tImageInfo.Width >> 1;
	//		//fCenterY = curTextInfo->tImageInfo.Height >> 1;
	//		//D3DXMatrixTranslation(&matTrans, -(fCenterX * i * 2.1f) + m_vPos.x, fCenterY + m_vPos.y, 0.f);

	//		//matWorld = matTrans;

	//		//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	//		//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(curTextInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, m_RGB.rgbtRed, m_RGB.rgbtGreen, m_RGB.rgbtBlue));
	//	}
	//}

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

	m_pTextureCom->Render_Texture(m_iNumber);
	m_pBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}

HRESULT CUIDigit::Add_Component(void)
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


CUIDigit* CUIDigit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIDigit* pInstance = new CUIDigit(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUIDigit::Free(void)
{
}