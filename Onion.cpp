#include "stdafx.h"
#include "Onion.h"

#include "Export_Function.h"

/* animation
0 Drop
1 Gather
2 Chop
3 Idle
*/

/* mesh
	LPSTR test = "Onion_Whole";
	LPSTR test = "Onion_Sliced";
*/

COnion::COnion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

COnion::COnion(const COnion& rhs)
	: CGameObject(rhs)
{

}

COnion::~COnion(void)
{

}

HRESULT COnion::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(15.f, 0.f, 15.f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));

	m_wstrObjTag = L"Onion";

	return S_OK;
}

Engine::_int COnion::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	if (m_bIsDead)
		return -1;

	//SetUp_OnTerrain();

	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);
	

	if (m_dwAnimFlag & Anim_Chop && m_bAnimChop==false)
	{
		LPSTR meshStr = "Onion_Whole";
		m_pMeshCom->Add_RemoveStr(&meshStr);
		meshStr = "Onion_Sliced";
		m_pMeshCom->Delete_RemoveStr(&meshStr);
		
		m_bAnimChop = true;
	}
	if (m_dwAnimFlag & Anim_Gather && m_bAnimGather == false)
	{
		m_bAnimGather = true;
	}
	if (m_dwAnimFlag & Anim_Chop)
	{
		m_pMeshCom->Set_AnimationIndex(2);
		if (false == m_pMeshCom->Is_AnimationsetFinish())
		{
			m_fDelay += fTimeDelta;
			if (m_bAnimStart && m_fDelay>0.05)
			{
				m_pMeshCom->Play_Animation(fTimeDelta);
				m_fDelay = 0.f;
			}
		}
		else
		{
			m_pMeshCom->Set_AnimationIndex(2);
			m_bChopFinished = true;
		}
	}
	if (m_dwAnimFlag & Anim_Gather && m_bChopFinished)
	{
		m_pMeshCom->Set_AnimationIndex(1);
		if (false == m_pMeshCom->Is_AnimationsetFinish())
			m_pMeshCom->Play_Animation(fTimeDelta);
	}


	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void COnion::Render_Object(void)
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

	m_pMeshCom->Render_Meshes_Ex(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	
	//m_pSphereColliderCom->Render_SphereCollider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT COnion::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// DynamicMesh
	//pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_Onion"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

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
	/*pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, &m_pTransformCom->m_vInfo[INFO_POS], 0.15f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCollider", pComponent);*/

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

void COnion::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool COnion::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	/*CSphereCollider* pPlayerColliderCom = dynamic_cast<CSphereCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_SphereCollider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	_matrix test;
	D3DXMatrixIdentity(&test);

	return m_pCalculatorCom->Collision_Sphere(m_pSphereColliderCom->Get_Center(), m_pSphereColliderCom->Get_Radius(), m_pSphereColliderCom->Get_CollWorldMatrix(),
		pPlayerColliderCom->Get_Center(), pPlayerColliderCom->Get_Radius(), pPlayerColliderCom->Get_CollWorldMatrix());*/
	return false;
}

HRESULT COnion::Set_OnionIndex(int iIndex)
{
	CComponent* pComponent = nullptr;

	// texture
	if (iIndex == 0)
	{
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_Onion"));
	}
	if (iIndex == 1)
	{
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_OnionB"));	
	}
	if (iIndex == 2)
	{
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_OnionC"));
	}
	if (iIndex == 3)
	{
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_OnionD"));
	}
	if (iIndex == 4)
	{
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_OnionE"));
	}
	if (iIndex == 5)
	{
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_OnionF"));
	}
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	m_pMeshCom->Set_AnimationIndex(0);

	LPSTR meshStr = "Onion_Sliced";

	m_pMeshCom->Add_RemoveStr(&meshStr);

}

HRESULT COnion::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

COnion* COnion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COnion*	pInstance = new COnion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void COnion::Free(void)
{
	CGameObject::Free();
}

