#include "stdafx.h"
#include "CrateC.h"

#include "Export_Function.h"

// 0 Idle
// 1 Open

CCrateC::CCrateC(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCrateC::CCrateC(const CCrateC& rhs)
	: CGameObject(rhs)
{

}

CCrateC::~CCrateC(void)
{

}

HRESULT CCrateC::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(5.f, 0.f, 5.f);
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-90.f));

	m_pMeshCom->Set_AnimationIndex(1);

	return S_OK;
}

Engine::_int CCrateC::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	//SetUp_OnTerrain();

	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");



	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	if (m_dwAnimFlag & Anim_Crate_Open)
	{
		m_pMeshCom->Set_AnimationIndex(1);
		m_dwAnimFlag &= ~Anim_Crate_Open;
	}


	if (true == m_pMeshCom->Is_AnimationsetFinish())
		m_pMeshCom->Set_AnimationIndex(0);


	m_pMeshCom->Play_Animation(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CCrateC::Render_Object(void)
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

	m_pSphereColliderCom->Render_SphereCollider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT CCrateC::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_CrateC"));
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
	//pComponent = m_pColliderCom = CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	// SphereCollider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, &m_pTransformCom->m_vInfo[INFO_POS], 0.15f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCollider", pComponent);

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

void CCrateC::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3* ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool CCrateC::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	CSphereCollider* pPlayerColliderCom = dynamic_cast<CSphereCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_SphereCollider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	_matrix test;
	D3DXMatrixIdentity(&test);

	return m_pCalculatorCom->Collision_Sphere(m_pSphereColliderCom->Get_Center(), m_pSphereColliderCom->Get_Radius(), m_pSphereColliderCom->Get_CollWorldMatrix(),
		pPlayerColliderCom->Get_Center(), pPlayerColliderCom->Get_Radius(), pPlayerColliderCom->Get_CollWorldMatrix());

}

HRESULT CCrateC::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

CCrateC* CCrateC::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrateC* pInstance = new CCrateC(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CCrateC::Free(void)
{
	CGameObject::Free();
}

