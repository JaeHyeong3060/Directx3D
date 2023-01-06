#include "stdafx.h"
#include "Plate.h"

#include "Export_Function.h"

CPlate::CPlate(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlate::CPlate(const CPlate& rhs)
	: CGameObject(rhs)
{

}

CPlate::~CPlate(void)
{
	pSaladTomato->Free();
	pSaladLettuce->Free();
}

HRESULT CPlate::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"Plate";

	//m_pTransformCom->Set_Pos(15.f, 0.f, 10.f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));

	pSoup = CSoup::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSoup, E_FAIL);
	auto Transform = dynamic_cast<CTransform*>(pSoup->Get_Component(L"Com_Transform", ID_DYNAMIC));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Transform->Set_Pos(&vPos);

	pSaladTomato = CSaladTomato::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSaladTomato, E_FAIL);
	auto TomatoTransform = dynamic_cast<CTransform*>(pSaladTomato->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	TomatoTransform->Set_Pos(&vPos);

	pSaladLettuce = CSaladLettuce::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSaladLettuce, E_FAIL);
	auto LettuceTransform = dynamic_cast<CTransform*>(pSaladLettuce->Get_Component(L"Com_Transform", ID_DYNAMIC));
	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	LettuceTransform->Set_Pos(&vPos);

	//m_iTomatoCount = 1;

	return S_OK;
}

Engine::_int CPlate::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	if (m_bIsDead)
	{
		pSoup->Set_Dead();
		pSoup->Free();
		return -1;
	}

	pSoup->Update_Object(fTimeDelta);

	if (m_iTomatoCount>0)
		pSaladTomato->Update_Object(fTimeDelta);

	if (m_iLettuceCount > 0)
		pSaladLettuce->Update_Object(fTimeDelta);

	//SetUp_OnTerrain();

	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CPlate::Render_Object(void)
{
	//if (false == m_bDraw)
	//	return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iMaxPass = 0;

	pEffect->Begin(&iMaxPass, 0);	
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	if (m_iInCount != 0)
	{
		auto Transform = dynamic_cast<CTransform*>(pSoup->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		if (m_iInCount >= 1)
			vPos += {0.f, 0.015f, 0.f };
		Transform->Set_Pos(&vPos);
		pSoup->Render_Object();
	}

	if (m_iTomatoCount > 0)
	{
		auto Transform = dynamic_cast<CTransform*>(pSaladTomato->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos += {0.f, 0.025f, 0.f };
		Transform->Set_Pos(&vPos);
		pSaladTomato->Render_Object();
	}

	if (m_iLettuceCount > 0)
	{
		auto Transform = dynamic_cast<CTransform*>(pSaladLettuce->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos += {0.f, 0.025f, 0.f };
		Transform->Set_Pos(&vPos);
		pSaladLettuce->Render_Object();
	}
	

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

void CPlate::Set_PlateContentTag(wstring wstrTag)
{ 
	m_wstrPlateContent = wstrTag;
	if(wstrTag == L"SoupTomato" || wstrTag == L"SoupOnion")
		static_cast<CSoup*>(pSoup)->Set_SoupContentTag(m_wstrPlateContent);
}

HRESULT CPlate::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(L"Proto_Mesh_Plate"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

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

	// Collider
	pComponent = m_pColliderCom = CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

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

void CPlate::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool CPlate::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	CCollider*		pPlayerColliderCom = dynamic_cast<CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
		m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());

}

HRESULT CPlate::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

CPlate* CPlate::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlate*	pInstance = new CPlate(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlate::Free(void)
{
	CGameObject::Free();
}

