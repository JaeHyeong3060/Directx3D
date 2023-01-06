#include "stdafx.h"
#include "NPC_Buck.h"

#include "Export_Function.h"

/* animation
0 Idle
*/

CNPC_Buck::CNPC_Buck(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CNPC_Buck::CNPC_Buck(const CNPC_Buck& rhs)
	: CGameObject(rhs)
{

}

CNPC_Buck::~CNPC_Buck(void)
{

}

HRESULT CNPC_Buck::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(15.f, 0.f, 15.f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));
	m_pMeshCom->Set_AnimationIndex(0);

	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90.f));

	m_wstrObjTag = L"NPC_Buck";

	return S_OK;
}

Engine::_int CNPC_Buck::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	if (m_bIsDead)
		return -1;

	Add_RenderGroup(RENDER_NONALPHA, this);

	m_pMeshCom->Play_Animation(fTimeDelta);

	return 0;
}

void CNPC_Buck::Render_Object(void)
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

HRESULT CNPC_Buck::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	//DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_NPC_Buck"));
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

HRESULT CNPC_Buck::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

CNPC_Buck* CNPC_Buck::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNPC_Buck*	pInstance = new CNPC_Buck(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CNPC_Buck::Free(void)
{
	CGameObject::Free();
}

