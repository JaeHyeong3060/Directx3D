#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CTerrain::CTerrain(const CTerrain& rhs)
	: CGameObject(rhs)
{

}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	//m_pIndex = new INDEX32[m_pBufferCom->Get_TriCnt()];

	return S_OK;
}

Engine::_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	/*m_pOptimizationCom->Isin_FrustumForTerrain(m_pBufferCom->Get_VtxPos(),
												m_pBufferCom->Get_VtxCntX(),
												m_pBufferCom->Get_VtxCntZ(), 
												m_pIndex, 
												&m_dwTriCnt);*/

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::Render_Object(void)
{
	//FAILED_CHECK_RETURN(SetUp_Material(), );

	//m_pBufferCom->Copy_Index(m_pIndex, m_dwTriCnt);

	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//NULL_CHECK(pEffect);
	//pEffect->AddRef();

	//FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	//_uint	iMaxPass = 0;

	//pEffect->Begin(&iMaxPass, 0);	// 1. 현재 쉐이더 파일이 가진 최대 pass의 개수 반환 2. 시작하는 방식에 대한 flag 값(default 값)
	//pEffect->BeginPass(0);

	//m_pBufferCom->Render_Buffer();

	//pEffect->EndPass();
	//pEffect->End();

	//Safe_Release(pEffect);

}

HRESULT CTerrain::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_Buffer_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);
	
	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Filter
	pComponent = m_pFilterCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Filter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Filter", pComponent);

	// Aura
	pComponent = m_pAuraCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Aura"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Aura", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Clone_Proto(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}


HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);
	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture1", 1);
	m_pFilterCom->Set_Texture(pEffect, "g_FilterTexture", 0);
	m_pAuraCom->Set_Texture(pEffect, "g_AuraTexture", 0);
	
	//CTransform*		pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", ID_DYNAMIC));
	//_vec3	vPos;
	//pPlayerTransform->Get_Info(INFO_POS, &vPos);

	//pEffect->SetVector("g_vPlayerPos", &_vec4(vPos, 1.f));
	pEffect->SetFloat("g_fRange", 3.f);

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{

	//Safe_Delete_Array(m_pIndex);


	CGameObject::Free();
}

