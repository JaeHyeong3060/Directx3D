#include "stdafx.h"
#include "Pot.h"

#include "Export_Function.h"

CPot::CPot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPot::CPot(const CPot& rhs)
	: CGameObject(rhs)
{

}

CPot::~CPot(void)
{
	pSoup->Free();
}

HRESULT CPot::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrObjTag = L"Pot";

	//m_pTransformCom->Set_Pos(15.f, 0.f, 12.f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));

	pSoup = CSoup::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSoup, E_FAIL);
	CTransform* Transform = dynamic_cast<CTransform*>(pSoup->Get_Component(L"Com_Transform", ID_DYNAMIC));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Transform->Set_Pos(&vPos);

	pUITimerObj = CUITimerObj::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUITimerObj, E_FAIL);
	CTransform* TransformTimer = dynamic_cast<CTransform*>(pUITimerObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	TransformTimer->Set_Pos(&vPos);

	return S_OK;
}

Engine::_int CPot::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	//m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	CTransform* TransformTimer = dynamic_cast<CTransform*>(pUITimerObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	TransformTimer->Set_Pos(&vPos);

	if (m_iInCount != 0)
	{
		pSoup->Update_Object(fTimeDelta);
		pUITimerObj->Update_Object(fTimeDelta);

		if (m_bIsCooking)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"FireLoop.wav", CSoundMgr::ENV);
		}
	}
	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CPot::Render_Object(void)
{
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
	
	if (m_iInCount != 0)
	{
		if (m_wstrPotContent == L"SoupOnion")
			dynamic_cast<CSoup*>(pSoup)->Set_SoupContentTag(L"SoupOnion");
		if (m_wstrPotContent == L"SoupTomato")
			dynamic_cast<CSoup*>(pSoup)->Set_SoupContentTag(L"SoupTomato");
		auto Transform = dynamic_cast<CTransform*>(pSoup->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		if (m_iInCount == 1)
			vPos += {0.f, 0.04f, 0.f };
		if (m_iInCount == 2)
			vPos += {0.f, 0.08f, 0.f };
		if (m_iInCount >= 3)
			vPos += {0.f, 0.12f, 0.f };
		Transform->Set_Pos(&vPos);
		pSoup->Render_Object();
	}
	if (m_iInCount >= 3 && static_cast<CUITimerObj*>(pUITimerObj)->Get_IsEndGauge())
	{
		m_bCookEnd = true;
		static_cast<CUITimerObj*>(pUITimerObj)->Set_ResetTime();
		static_cast<CUITimerObj*>(pUITimerObj)->Set_TimeStart(false);
		static_cast<CUITimerObj*>(pUITimerObj)->Set_IsEndGauge(false);
	}

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

void CPot::Add_InCount(int number)
{
	if(m_iInCount!=0)
		static_cast<CUITimerObj*>(pUITimerObj)->Set_RightX(0.1f);
	m_iInCount += number; 
}

void CPot::Set_Cooking(_bool bCook)
{
	 m_bIsCooking = bCook;
	 static_cast<CSoup*>(pSoup)->Set_Cooking(bCook);
	 static_cast<CUITimerObj*>(pUITimerObj)->Set_TimeStart(bCook);
	 
}

void CPot::Add_Food()
{
	
}

HRESULT CPot::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(L"Proto_Mesh_Pot"));
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

void CPot::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool CPot::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	CCollider*		pPlayerColliderCom = dynamic_cast<CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	return false;
}

HRESULT CPot::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

CPot* CPot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPot*	pInstance = new CPot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPot::Free(void)
{
	CGameObject::Free();
}

