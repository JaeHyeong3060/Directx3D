#include "stdafx.h"
#include "ChoppingBoard.h"

#include "Export_Function.h"

CChoppingBoard::CChoppingBoard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CChoppingBoard::CChoppingBoard(const CChoppingBoard& rhs)
	: CGameObject(rhs)
{

}

CChoppingBoard::~CChoppingBoard(void)
{

}

HRESULT CChoppingBoard::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(5.f, 0.f, 5.f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));

	pUITimerObj = CUITimerObj::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUITimerObj, E_FAIL);
	CTransform* TransformTimer = dynamic_cast<CTransform*>(pUITimerObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	TransformTimer->Set_Pos(&vPos);
	static_cast<CUITimerObj*>(pUITimerObj)->Set_TimeSpeed(0.040f);

	
	//effect
	pEffectObj = CEffect::Create(m_pGraphicDev);
	static_cast<CEffect*>(pEffectObj)->Set_TextureWstr(L"Effect_Chop");
	auto transform = dynamic_cast<CTransform*>(pEffectObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	transform->Set_Pos(vPos.x, vPos.y + 0.5f, vPos.z);
	NULL_CHECK_RETURN(pEffectObj, E_FAIL);
	//FAILED_CHECK_RETURN(CurLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);

	return S_OK;
}

Engine::_int CChoppingBoard::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	//SetUp_OnTerrain();

	if(m_bStartChop)
		m_bColl = Collision_ToObject(L"GameLogic", L"Player");
	if (!m_bColl)
		m_bStartChop = false;

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTransform* TransformTimer = dynamic_cast<CTransform*>(pUITimerObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	TransformTimer->Set_Pos(&vPos);

	CTransform* TransformEffect = dynamic_cast<CTransform*>(pEffectObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
	TransformEffect->Set_Pos(vPos.x,vPos.y+0.5f,vPos.z);

	if (static_cast<CUITimerObj*>(pUITimerObj)->Get_IsEndGauge())
	{
		static_cast<CUITimerObj*>(pUITimerObj)->Set_ResetTime();
		static_cast<CUITimerObj*>(pUITimerObj)->Set_TimeStart(false);
		static_cast<CUITimerObj*>(pUITimerObj)->Set_IsEndGauge(false);
	}

	if (!m_listInven.empty())
	{
		if(m_bStartChop)
		{
			pUITimerObj->Update_Object(fTimeDelta);
			m_listInven.front()->Set_AnimStart(true);
		}
		else
			m_listInven.front()->Set_AnimStart(false);

	}

	if (m_bStartChop)
	{
		pEffectObj->Update_Object(fTimeDelta);
	}


	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CChoppingBoard::Render_Object(void)
{
	if (!m_listInven.empty())
		pUITimerObj->Render_Object();



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

	if (m_bStartChop)
	{
		pEffectObj->Render_Object();
	}
}

void CChoppingBoard::Set_StartChop(_bool chop)
{
	m_bStartChop = chop;
	static_cast<CUITimerObj*>(pUITimerObj)->Set_TimeStart(chop);
}

void CChoppingBoard::Reset_Time()
{
	if(m_listInven.empty())
		static_cast<CUITimerObj*>(pUITimerObj)->Set_ResetTime();
}

HRESULT CChoppingBoard::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(L"Proto_Mesh_ChoppingBoard"));
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

	//// Collider
	//pComponent = m_pColliderCom = CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	// SphereCollider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, &m_pTransformCom->m_vInfo[INFO_POS], 0.25f);
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

void CChoppingBoard::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool CChoppingBoard::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	//CCollider*		pPlayerColliderCom = dynamic_cast<CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	//NULL_CHECK_RETURN(pPlayerColliderCom, false);

	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(pLayerTag);

	list<CGameObject*>* pExMapObject = CurLayer->Get_ExMapObject(pObjTag);

	for (auto& iter : *pExMapObject)
	{
		CSphereCollider* pObjColliderCom = dynamic_cast<CSphereCollider*>(iter->Get_Component(L"Com_SphereCollider", ID_STATIC));

		if (m_pCalculatorCom->Collision_Sphere(m_pSphereColliderCom->Get_Center(), m_pSphereColliderCom->Get_Radius(), m_pSphereColliderCom->Get_CollWorldMatrix(),
			pObjColliderCom->Get_Center(), pObjColliderCom->Get_Radius(), pObjColliderCom->Get_CollWorldMatrix()))
		{
			return true;
		}
	}

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	/*return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
		m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	return false;

}

HRESULT CChoppingBoard::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

CChoppingBoard* CChoppingBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChoppingBoard*	pInstance = new CChoppingBoard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChoppingBoard::Free(void)
{
	CGameObject::Free();
}

