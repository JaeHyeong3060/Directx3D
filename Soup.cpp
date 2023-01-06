#include "stdafx.h"
#include "Soup.h"

#include "Export_Function.h"

/* animation
0 Drop
1 Gather
2 Chop
3 Idle
*/

/* mesh
	LPSTR test = "Soup_Whole";
	LPSTR test = "Soup_Sliced";
*/

CSoup::CSoup(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSoup::CSoup(const CSoup& rhs)
	: CGameObject(rhs)
{

}

CSoup::~CSoup(void)
{

}

HRESULT CSoup::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshOnionCom->Set_AnimationIndex(1);

	return S_OK;
}

Engine::_int CSoup::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	//m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	if (m_bIsCooking)
	{
		if(m_wstrSoupTag==L"SoupOnion")
			m_pMeshOnionCom->Play_Animation(fTimeDelta);

		if (m_wstrSoupTag == L"SoupTomato")
			m_pMeshTomatoCom->Play_Animation(fTimeDelta);

	}

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CSoup::Render_Object(void)
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

	if (m_wstrSoupTag == L"SoupOnion")
		m_pMeshOnionCom->Render_Meshes_Ex(pEffect);

	if (m_wstrSoupTag == L"SoupTomato")
		m_pMeshTomatoCom->Render_Meshes_Ex(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	
	//m_pSphereColliderCom->Render_SphereCollider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT CSoup::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshOnionCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_Soup"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// DynamicMesh
	pComponent = m_pMeshTomatoCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_SoupT"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_MeshT", pComponent);

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

void CSoup::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();

	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool CSoup::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return false;

}

HRESULT CSoup::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

CSoup* CSoup::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSoup*	pInstance = new CSoup(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSoup::Free(void)
{
	CGameObject::Free();
}

