#include "stdafx.h"
#include "UITimerObj.h"

#include "Export_Function.h"

/* animation
0 Drop
1 Gather
2 Chop
3 Idle
*/

/* mesh
	LPSTR test = "UITimerObj_Whole";
	LPSTR test = "UITimerObj_Sliced";
*/

CUITimerObj::CUITimerObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUITimerObj::CUITimerObj(const CUITimerObj& rhs)
	: CGameObject(rhs)
{

}

CUITimerObj::~CUITimerObj(void)
{

}

HRESULT CUITimerObj::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fRightX = m_fLeftX;

	m_fTimeSpeed = 0.01f;

	return S_OK;
}

Engine::_int CUITimerObj::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	Add_RenderGroup(RENDER_NONALPHA, this);

	dynamic_cast<CRcTex*>(m_pBufferComGauge)->Set_TexPos(m_fLeftX,m_fTopY,m_fDownY,m_fRightX);

	if ((-m_fLeftX) <= m_fRightX)
	{
		m_bIsEndGauge = true;
	}

	if(m_bIsEndGauge == false && m_bTimeStart == true)
		m_fRightX += fTimeDelta * m_fTimeSpeed;


	return 0;
}

void CUITimerObj::Render_Object(void)
{
	//if (false == m_bDraw)
	//	return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureGauge->Render_Texture();
	m_pBufferComGauge->Render_Buffer();

	//m_pTextureBack->Render_Texture();
	//m_pBufferComBack->Render_Buffer();

	//m_pSphereColliderCom->Render_SphereCollider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

void CUITimerObj::Set_RightX(_float fX)
{
	if (m_fRightX > 0)
		m_fRightX *= fX;
	else
		m_fRightX -= m_fRightX * fX;
}

HRESULT CUITimerObj::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	// buffer
	pComponent = m_pBufferComGauge = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// buffer
	//pComponent = m_pBufferComBack = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureGauge = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UITimerObj"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	//// texture
	//pComponent = m_pTextureBack = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UITimerObjBack"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Clone_Proto(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;

}

void CUITimerObj::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3* ptPos = pTerrainBufferCom->Get_VtxPos();

	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool CUITimerObj::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return false;

}

HRESULT CUITimerObj::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

CUITimerObj* CUITimerObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUITimerObj* pInstance = new CUITimerObj(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUITimerObj::Free(void)
{
	CGameObject::Free();
}

