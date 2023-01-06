#include "stdafx.h"
#include "NPC_Mel.h"

#include "Export_Function.h"

/* animation
0 Idle
*/

CNPC_Mel::CNPC_Mel(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CNPC_Mel::CNPC_Mel(const CNPC_Mel& rhs)
	: CGameObject(rhs)
{

}

CNPC_Mel::~CNPC_Mel(void)
{

}

HRESULT CNPC_Mel::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(15.f, 0.f, 15.f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));
	m_pMeshCom->Set_AnimationIndex(0);

	m_wstrObjTag = L"NPC_Mel";

	return S_OK;
}

Engine::_int CNPC_Mel::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	if (m_bIsDead)
		return -1;

	m_fAcctime += fTimeDelta;

	if (m_fAcctime > m_fTurnTime)
	{
		m_fAcctime = 0.f;
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90.f));
	}

	_vec3 vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	vRight *= -1.f;
	m_pTransformCom->Move_Pos(&vRight, 0.5f, fTimeDelta);


	Add_RenderGroup(RENDER_NONALPHA, this);

	m_pMeshCom->Play_Animation(fTimeDelta);

	return 0;
}

void CNPC_Mel::Render_Object(void)
{
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
}

HRESULT CNPC_Mel::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	//DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_NPC_Mel"));
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

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;

}

HRESULT CNPC_Mel::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

CNPC_Mel* CNPC_Mel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNPC_Mel*	pInstance = new CNPC_Mel(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CNPC_Mel::Free(void)
{
	CGameObject::Free();
}

