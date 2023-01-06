#include "stdafx.h"
#include "ChefB.h"

#include "Export_Function.h"



/*animation*/
//0.wash
//1.chop
//2.idle
//3.walk

CChefB::CChefB(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CChefB::CChefB(const CChefB& rhs)
	: CGameObject(rhs)
{

}

CChefB::~CChefB(void)
{

}

HRESULT CChefB::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.0f, 1.f);
	m_pTransformCom->Set_Pos(15.f, 0.f, 11.f);

	m_vOffset = { 0.3f,0.3f,0.3f };

	//m_pNaviCom->Set_CellIndex(1);
	m_pMeshCom->Set_AnimationIndex(0);

	LPSTR meshStr = "Cleaver";

	m_pMeshCom->Add_RemoveStr(&meshStr);

	meshStr = "Knife";

	m_pMeshCom->Add_RemoveStr(&meshStr);

	meshStr = "Hand_Open_L";

	m_pMeshCom->Add_RemoveStr(&meshStr);

	meshStr = "Hand_Open_R";

	m_pMeshCom->Add_RemoveStr(&meshStr);

	return S_OK;
}

Engine::_int CChefB::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	SetUp_OnTerrain();

	Key_Input(fTimeDelta);


	m_bColl = Collision_ToObject(L"GameLogic", L"Crate");
	if (m_bColl)
		m_dwColState |= COL_CRATE_ONION;
	else
		m_dwColState &= ~COL_CRATE_ONION;

	m_bColl = Collision_ToObject(L"GameLogic", L"CrateB");
	if (m_bColl)
		m_dwColState |= COL_CRATE_TOMATO;
	else
		m_dwColState &= ~COL_CRATE_TOMATO;

	m_bColl = Collision_ToObject(L"GameLogic", L"CrateC");
	if (m_bColl)
		m_dwColState |= COL_CRATE_LETTUCE;
	else
		m_dwColState &= ~COL_CRATE_LETTUCE;

	m_bColl = Collision_ToObject(L"GameLogic", L"ChoppingBoard");
	if (m_bColl)
		m_dwColState |= COL_CHOPPINGBOARD;
	else
		m_dwColState &= ~COL_CHOPPINGBOARD;

	m_bColl = Collision_ToObject(L"GameLogic", L"Empty");
	if (m_bColl)
		m_dwColState |= COL_EMPTY;
	else
		m_dwColState &= ~COL_EMPTY;

	m_bColl = Collision_ToObject(L"GameLogic", L"ThePass");
	if (m_bColl)
		m_dwColState |= COL_THEPASS;
	else
		m_dwColState &= ~COL_THEPASS;

	m_bColl = Collision_ToObject(L"GameLogic", L"CookingStation");
	if (m_bColl)
		m_dwColState |= COL_COOKINGSTATION;
	else
		m_dwColState &= ~COL_COOKINGSTATION;

	m_bColl = Collision_ToObject(L"GameLogic", L"WashingStation");
	if (m_bColl)
		m_dwColState |= COL_WASHINGSTATION;
	else
		m_dwColState &= ~COL_WASHINGSTATION;

	m_bColl = Collision_ToObject(L"GameLogic", L"ThePassExit");
	if (m_bColl)
		m_dwColState |= COL_THEPASSEXIT;
	else
		m_dwColState &= ~COL_THEPASSEXIT;

	m_bBodyColl = Body_Collision_ToObject(L"GameLogic", L"Empty");

	if (!m_listInven.empty())
	{
		auto pGameObject = m_listInven.front();
		auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
		auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();
		_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42 + 0.4f,matPlayerCol->_43 };
		transform->Set_Pos(&vPos);
	}

	if (m_bCookState)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"KnifeChopCut.wav", CSoundMgr::PLAYER_B);
	}
	if (m_bWashState)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Washing_01.wav", CSoundMgr::PLAYER_B);
	}


	m_pMeshCom->Play_Animation(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CChefB::Render_Object(void)
{


	//m_pNaviCom->Render_NaviMesh();


	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iMaxPass = 0;

	pEffect->Begin(&iMaxPass, 0);	// 1. 현재 쉐이더 파일이 가진 최대 pass의 개수 반환 2. 시작하는 방식에 대한 flag 값(default 값)
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes_Ex(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	auto matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix matTemp;
	memcpy(&matTemp, matWorld, sizeof(_matrix));

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	vLook.x *= m_vOffset.x;
	vLook.y *= m_vOffset.y;
	vLook.z *= m_vOffset.z;

	matTemp._41 += vLook.x;
	matTemp._42 += vLook.y;
	matTemp._43 += vLook.z;

	m_pSphereColliderCom->Render_SphereCollider(COLLTYPE(m_bColl), &matTemp);

	m_pSphereColliderBodyCom->Render_SphereCollider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pSphereColliderCom->Render_SphereCollider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pSphereColliderCom->Render_SphereCollider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());


}

HRESULT CChefB::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_ChefB"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// NaviMesh
	pComponent = m_pNaviCom = dynamic_cast<CNaviMesh*>(Clone_Proto(L"Proto_Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);

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

	// SphereCollider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, &m_pTransformCom->m_vInfo[INFO_POS], 0.15f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCollider", pComponent);

	// SphereCollider
	pComponent = m_pSphereColliderBodyCom = CSphereCollider::Create(m_pGraphicDev, &m_pTransformCom->m_vInfo[INFO_POS], 0.2f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereColliderBody", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;

}

void CChefB::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (Get_DIKeyState(DIK_I) & 0x80)
	{
		_vec3	vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3{ 0.f,0.f,m_fSpeed } *fTimeDelta)));

		m_pTransformCom->m_vAngle.y = D3DXToRadian(0);

		if (m_bCookState == true)
			RemoveKnife();
		m_bCookState = false;
		m_bWashState = false;

		m_pMeshCom->Set_AnimationIndex(3);
		//printf("%f,%f,%f\n", vPos.x, vPos.y, vPos.z);
		CreateEffectStar();

		CSoundMgr::Get_Instance()->PlaySound(L"Step_01.wav", CSoundMgr::PLAYER_B_ENV);
	}
	if (Get_DIKeyState(DIK_J) & 0x80)
	{
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3{ -m_fSpeed,0.f,0.f } *fTimeDelta)));

		m_pTransformCom->m_vAngle.y = D3DXToRadian(-90);

		if (m_bCookState == true)
			RemoveKnife();
		m_bCookState = false;
		m_bWashState = false;

		m_pMeshCom->Set_AnimationIndex(3);
		CreateEffectStar();

		CSoundMgr::Get_Instance()->PlaySound(L"Step_01.wav", CSoundMgr::PLAYER_B_ENV);
	}
	if (Get_DIKeyState(DIK_K) & 0x80)
	{
		_vec3	vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3{ 0.f,0.f,-m_fSpeed } *fTimeDelta)));

		m_pTransformCom->m_vAngle.y = D3DXToRadian(180);

		if (m_bCookState == true)
			RemoveKnife();
		m_bCookState = false;
		m_bWashState = false;

		m_pMeshCom->Set_AnimationIndex(3);
		CreateEffectStar();

		CSoundMgr::Get_Instance()->PlaySound(L"Step_01.wav", CSoundMgr::PLAYER_B_ENV);
	}
	if (Get_DIKeyState(DIK_L) & 0x80)
	{
		_vec3	vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(_vec3{ m_fSpeed,0.f,0.f } *fTimeDelta)));

		m_pTransformCom->m_vAngle.y = D3DXToRadian(90);

		if (m_bCookState == true)
			RemoveKnife();
		m_bCookState = false;
		m_bWashState = false;

		m_pMeshCom->Set_AnimationIndex(3);
		CreateEffectStar();

		CSoundMgr::Get_Instance()->PlaySound(L"Step_01.wav", CSoundMgr::PLAYER_B_ENV);
	}

	//if (Get_DIKeyState(DIK_SPACE) & 0x80)
#pragma region EVENT_KEY_Z
	if (CKeyManager::GetInstance()->Key_Up(KEY_F))
	{
		if (m_bCookState == true)
			RemoveKnife();
		m_bCookState = false;
		m_bWashState = false;

#pragma region ChefB_Inventory_EMPTY
		if (m_listInven.empty())
		{
			// 인벤X Crate 양파 충돌
			if (m_dwColState & COL_CRATE_ONION)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				m_DestObj->Add_AnimFlag(Anim_Crate_Open);

				CGameObject* pGameObject = nullptr;

				pGameObject = COnion::Create(m_pGraphicDev);
				static_cast<COnion*>(pGameObject)->Set_OnionIndex((++m_OnionIndex) % 3 +3);

				auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));

				auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

				_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

				transform->Set_Pos(&vPos);

				NULL_CHECK(pGameObject, E_FAIL);
				CurLayer->Add_ExGameObject(L"Onion", pGameObject);

				m_listInven.push_back(pGameObject);

				CSoundMgr::Get_Instance()->PlaySound(L"AddIngredient_01.wav", CSoundMgr::PLAYER_B);
			}
			// 인벤X Crate 토마토 충돌
			if (m_dwColState & COL_CRATE_TOMATO)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				m_DestObj->Add_AnimFlag(Anim_Crate_Open);

				CGameObject* pGameObject = nullptr;

				pGameObject = CTomato::Create(m_pGraphicDev);
				static_cast<CTomato*>(pGameObject)->Set_TomatoIndex((++m_TomatoIndex) % 3 + 3);

				auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));

				auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

				_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

				transform->Set_Pos(&vPos);

				NULL_CHECK(pGameObject, E_FAIL);
				CurLayer->Add_ExGameObject(L"Tomato", pGameObject);

				m_listInven.push_back(pGameObject);

				CSoundMgr::Get_Instance()->PlaySound(L"AddIngredient_01.wav", CSoundMgr::PLAYER_B);
			}
			// 인벤X Crate 양배추 충돌
			if (m_dwColState & COL_CRATE_LETTUCE)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				m_DestObj->Add_AnimFlag(Anim_Crate_Open);

				CGameObject* pGameObject = nullptr;

				pGameObject = CLettuce::Create(m_pGraphicDev);
				static_cast<CLettuce*>(pGameObject)->Set_LettuceIndex((++m_TomatoIndex) % 3 + 3);

				auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));

				auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

				_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

				transform->Set_Pos(&vPos);

				NULL_CHECK(pGameObject, E_FAIL);
				CurLayer->Add_ExGameObject(L"Lettuce", pGameObject);

				m_listInven.push_back(pGameObject);

				CSoundMgr::Get_Instance()->PlaySound(L"AddIngredient_01.wav", CSoundMgr::PLAYER_B);
			}
			// 인벤X 도마 충돌
			if (m_dwColState & COL_CHOPPINGBOARD)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				dynamic_cast<CChoppingBoard*>(m_DestObj)->Reset_Time();

				if (m_DestObj->Get_Inven()->size() != 0)
				{
					auto pfrontItemInven = m_DestObj->Get_Inven()->front();

					if (pfrontItemInven->Get_ObjTag() == L"Onion")
					{
						_bool bIsFinished = dynamic_cast<COnion*>(pfrontItemInven)->Get_IsChopFinished();
						if (bIsFinished)
						{
							auto pfrontItemTranfrom = dynamic_cast<CTransform*>(pfrontItemInven->Get_Component(L"Com_Transform", ID_DYNAMIC));

							pfrontItemInven->Add_AnimFlag(Anim_Gather);
							pfrontItemInven->Delete_AnimFlag(Anim_Chop);

							auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

							_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

							pfrontItemTranfrom->Set_Pos(&vPos);

							m_listInven.push_back(pfrontItemInven);

							m_DestObj->Delete_InvenFront();

							CSoundMgr::Get_Instance()->PlaySound(L"Item_PickUp_01.wav", CSoundMgr::PLAYER_B);
						}
					}
					if (pfrontItemInven->Get_ObjTag() == L"Tomato")
					{
						_bool bIsFinished = dynamic_cast<CTomato*>(pfrontItemInven)->Get_IsChopFinished();
						if (bIsFinished)
						{
							auto pfrontItemTranfrom = dynamic_cast<CTransform*>(pfrontItemInven->Get_Component(L"Com_Transform", ID_DYNAMIC));

							pfrontItemInven->Add_AnimFlag(Anim_Gather);
							pfrontItemInven->Delete_AnimFlag(Anim_Chop);

							auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

							_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

							pfrontItemTranfrom->Set_Pos(&vPos);

							m_listInven.push_back(pfrontItemInven);

							m_DestObj->Delete_InvenFront();

							CSoundMgr::Get_Instance()->PlaySound(L"Item_PickUp_01.wav", CSoundMgr::PLAYER_B);
						}
					}
					if (pfrontItemInven->Get_ObjTag() == L"Lettuce")
					{
						_bool bIsFinished = dynamic_cast<CLettuce*>(pfrontItemInven)->Get_IsChopFinished();
						if (bIsFinished)
						{
							auto pfrontItemTranfrom = dynamic_cast<CTransform*>(pfrontItemInven->Get_Component(L"Com_Transform", ID_DYNAMIC));

							pfrontItemInven->Add_AnimFlag(Anim_Gather);
							pfrontItemInven->Delete_AnimFlag(Anim_Chop);

							auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

							_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

							pfrontItemTranfrom->Set_Pos(&vPos);

							m_listInven.push_back(pfrontItemInven);

							m_DestObj->Delete_InvenFront();

							CSoundMgr::Get_Instance()->PlaySound(L"Item_PickUp_01.wav", CSoundMgr::PLAYER_B);
						}
					}
				}
			}
			// 인벤X 장소 충돌
			if (m_dwColState & COL_EMPTY)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				// 빈 장소에 아이템이 있으면
				if (m_DestObj->Get_Inven()->size() != 0)
				{
					auto pfrontItemInven = m_DestObj->Get_Inven()->front();

					auto pfrontItemTranfrom = dynamic_cast<CTransform*>(pfrontItemInven->Get_Component(L"Com_Transform", ID_DYNAMIC));

					auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

					_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

					pfrontItemTranfrom->Set_Pos(&vPos);

					m_listInven.push_back(pfrontItemInven);

					m_DestObj->Delete_InvenFront();

					CSoundMgr::Get_Instance()->PlaySound(L"Item_PickUp_01.wav", CSoundMgr::PLAYER_B);
				}
			}
			// 인벤X 쿠킹스테이션
			if (m_dwColState & COL_COOKINGSTATION)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				// 빈 장소에 아이템이 있으면
				if (m_DestObj->Get_Inven()->size() != 0)
				{
					auto pfrontItemInven = m_DestObj->Get_Inven()->front();

					dynamic_cast<CPot*>(pfrontItemInven)->Set_Cooking(false);

					auto pfrontItemTranfrom = dynamic_cast<CTransform*>(pfrontItemInven->Get_Component(L"Com_Transform", ID_DYNAMIC));

					auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

					_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

					pfrontItemTranfrom->Set_Pos(&vPos);

					m_listInven.push_back(pfrontItemInven);

					m_DestObj->Delete_InvenFront();

					CSoundMgr::Get_Instance()->PlaySound(L"Item_PickUp_01.wav", CSoundMgr::PLAYER_B);

				}
			}
			// 인벤x 패스출구
			if (m_dwColState & COL_THEPASSEXIT)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				// 빈 장소에 아이템이 있으면
				if (m_DestObj->Get_Inven()->size() != 0)
				{
					auto pfrontItemInven = m_DestObj->Get_Inven()->front();

					auto pfrontItemTranfrom = dynamic_cast<CTransform*>(pfrontItemInven->Get_Component(L"Com_Transform", ID_DYNAMIC));

					auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

					_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

					pfrontItemTranfrom->Set_Pos(&vPos);

					m_listInven.push_back(pfrontItemInven);

					m_DestObj->Delete_InvenFront();

					CSoundMgr::Get_Instance()->PlaySound(L"Item_PickUp_01.wav", CSoundMgr::PLAYER_B);
				}
			}
			// 설거지
			if (m_dwColState & COL_WASHINGSTATION)
			{
				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				// 빈 장소에 아이템이 있으면
				if (m_DestObj->Get_Inven()->size() != 0)
				{
					auto pfrontItemInven = m_DestObj->Get_Inven()->front();

					if (pfrontItemInven->Get_ObjTag() == L"Plate")
					{
						auto pfrontItemTranfrom = dynamic_cast<CTransform*>(pfrontItemInven->Get_Component(L"Com_Transform", ID_DYNAMIC));

						auto matPlayerCol = m_pSphereColliderCom->Get_CollWorldMatrix();

						_vec3 vPos = { matPlayerCol->_41,matPlayerCol->_42,matPlayerCol->_43 };

						pfrontItemTranfrom->Set_Pos(&vPos);

						m_listInven.push_back(pfrontItemInven);

						m_DestObj->Delete_InvenFront();

						CSoundMgr::Get_Instance()->PlaySound(L"Item_PickUp_01.wav", CSoundMgr::PLAYER_B);
					}
				}
			}
		}
#pragma endregion
#pragma region 아이템 들고 있는 상태
		// 아이템 있음
		else if (!m_listInven.empty())
		{
			// pass랑 충돌
			if (m_dwColState & COL_THEPASS)
			{
				if (m_listInven.front()->Get_ObjTag() == L"Plate")
				{
					/*auto pGameObject = m_listInven.front();
					auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
					auto vDestPos = m_DestCol->Get_CollWorldMatrix();

					m_DestObj->Add_Inven(m_listInven.back());
					_vec3 vPos = { vDestPos->_41,vDestPos->_42 + 0.2f,vDestPos->_43 };
					transform->Set_Pos(&vPos);

					m_listInven.pop_front();*/
					CGameManager::GetInstance()->Serving_Food(m_listInven.front());
					//m_listInven.front()->Set_Dead();
					m_listInven.pop_front();

					CSoundMgr::Get_Instance()->PlaySound(L"SuccessfulDelivery.wav", CSoundMgr::PLAYER_B);
				}

			}
			// 아이템을 들고 있고 도마에 있는 경우
			if (m_dwColState & COL_CHOPPINGBOARD)
			{
				auto pGameObject = m_listInven.front();
				auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
				auto vDestPos = m_DestCol->Get_CollWorldMatrix();

				m_DestObj->Add_Inven(m_listInven.back());
				_vec3 vPos = { vDestPos->_41,vDestPos->_42 + 0.2f,vDestPos->_43 };
				transform->Set_Pos(&vPos);

				m_listInven.pop_front();

				CSoundMgr::Get_Instance()->PlaySound(L"Item_PutDown_01.wav", CSoundMgr::PLAYER_B);
			}
			// 아이템 들고 빈 공간에 
			if (m_dwColState & COL_EMPTY)
			{
				PUSH_KEY_Z_COL_EMPTY();

				CSoundMgr::Get_Instance()->PlaySound(L"Item_PutDown_01.wav", CSoundMgr::PLAYER_B);
			}
			// 아이템 들고 쿠킹
			if (m_dwColState & COL_COOKINGSTATION)
			{
				if (m_listInven.front()->Get_ObjTag() == L"Pot")
				{
					dynamic_cast<CPot*>(m_listInven.front())->Set_Cooking(true);
				}
				PUSH_KEY_Z_COL_EMPTY();

				CSoundMgr::Get_Instance()->PlaySound(L"Item_PutDown_01.wav", CSoundMgr::PLAYER_B);
			}
			// 아이템 들고 설거지
			if (m_dwColState & COL_WASHINGSTATION)
			{
				// 빈 장소에 아이템이 있으면
				if (m_DestObj->Get_Inven()->empty())
				{
					if (m_listInven.front()->Get_ObjTag() == L"DirtyPlate")
					{
						auto pGameObject = m_listInven.front();
						auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
						auto vDestPos = m_DestCol->Get_CollWorldMatrix();

						m_DestObj->Add_Inven(m_listInven.back());
						_vec3 vPos = { vDestPos->_41,vDestPos->_42 + 0.2f,vDestPos->_43 + 0.2f };
						transform->Set_Pos(&vPos);
						transform->Rotation(ROT_X, 1.f);
						m_listInven.pop_front();

						CSoundMgr::Get_Instance()->PlaySound(L"Item_PutDown_01.wav", CSoundMgr::PLAYER_B);
					}
				}
			}

		}
	}
#pragma endregion

#pragma region EVENT_KEY_X
	if (CKeyManager::GetInstance()->Key_Up(KEY_G))
	{
		// 인벤 X
		if (m_listInven.empty())
		{
			if (m_dwColState & COL_CHOPPINGBOARD)
			{
				LPSTR meshStr = "Knife";

				m_pMeshCom->Delete_RemoveStr(&meshStr);

				auto CurScene = CManagement::GetInstance()->Get_Scene();
				auto CurLayer = CurScene->Get_Layer(L"GameLogic");

				static_cast<CChoppingBoard*>(m_DestObj)->Set_StartChop(true);

				auto pfrontItemInven = m_DestObj->Get_Inven()->front();

				pfrontItemInven->Add_AnimFlag(Anim_Chop);

				m_pMeshCom->Set_AnimationIndex(1);

				m_bCookState = true;

			}
			if (m_dwColState & COL_WASHINGSTATION)
			{
				if (!m_DestObj->Get_Inven()->empty())
				{
					if (m_DestObj->Get_Inven()->front()->Get_ObjTag() == L"DirtyPlate")
					{
						auto CurScene = CManagement::GetInstance()->Get_Scene();
						auto CurLayer = CurScene->Get_Layer(L"GameLogic");

						static_cast<CWashingStation*>(m_DestObj)->Set_Trigger(true);

						m_pMeshCom->Set_AnimationIndex(0);

						m_bWashState = true;


					}
				}
			}
		}

	}
#pragma endregion

	if (true == m_pMeshCom->Is_AnimationsetFinish() && m_bCookState == false && m_bWashState == false)
		m_pMeshCom->Set_AnimationIndex(2);
}

void CChefB::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3* ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_vec3 CChefB::PickUp_OnTerrain(void)
{
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

HRESULT CChefB::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

Engine::_bool CChefB::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(pLayerTag);

	list<CGameObject*>* pExMapObject = CurLayer->Get_ExMapObject(pObjTag);

	_bool bCol = false;

	for (auto& iter : *pExMapObject)
	{
		CSphereCollider* pObjColliderCom = dynamic_cast<CSphereCollider*>(iter->Get_Component(L"Com_SphereCollider", ID_STATIC));

		if (m_pCalculatorCom->Collision_Sphere(m_pSphereColliderCom->Get_Center(), m_pSphereColliderCom->Get_Radius(), m_pSphereColliderCom->Get_CollWorldMatrix(),
			pObjColliderCom->Get_Center(), pObjColliderCom->Get_Radius(), pObjColliderCom->Get_CollWorldMatrix()))
		{
			m_DestObj = iter;
			m_DestCol = pObjColliderCom;
			return true;
		}
	}

	return false;
}

_bool CChefB::Body_Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(pLayerTag);

	list<CGameObject*>* pExMapObject = CurLayer->Get_ExMapObject(pObjTag);

	_bool bCol = false;

	for (auto& iter : *pExMapObject)
	{
		CSphereCollider* pObjColliderCom = dynamic_cast<CSphereCollider*>(iter->Get_Component(L"Com_SphereCollider", ID_STATIC));

		/*_vec2 fOut = m_pCalculatorCom->Collision_SphereEX(m_pSphereColliderBodyCom->Get_Center(), m_pSphereColliderBodyCom->Get_Radius(), m_pSphereColliderBodyCom->Get_CollWorldMatrix(),
			pObjColliderCom->Get_Center(), pObjColliderCom->Get_Radius(), pObjColliderCom->Get_CollWorldMatrix(),(CComponent*)m_pTransformCom);*/

		_vec2 fOut = m_pCalculatorCom->Collision_SphereEX(pObjColliderCom->Get_Center(), pObjColliderCom->Get_Radius(), pObjColliderCom->Get_CollWorldMatrix(),
			m_pSphereColliderBodyCom->Get_Center(), m_pSphereColliderBodyCom->Get_Radius(), m_pSphereColliderBodyCom->Get_CollWorldMatrix(),
			(CComponent*)m_pTransformCom);



	}

	return false;
}

void CChefB::PUSH_KEY_Z_COL_EMPTY()
{
	auto pGameObject = m_listInven.front();
	auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));
	auto vDestPos = m_DestCol->Get_CollWorldMatrix();

	// Onion을 들고 
	if (m_listInven.front()->Get_ObjTag() == L"Onion")
	{
		// 냄비가 있을 경우
		if (m_DestObj->Get_Inven()->size() > 0)
		{
			// 썰기 완료
			_bool bIsFinished = dynamic_cast<COnion*>(m_listInven.front())->Get_IsChopFinished();
			if (bIsFinished)
			{
				auto pFrontItem = m_DestObj->Get_Inven()->front();
				if (pFrontItem->Get_ObjTag() == L"Pot")
				{
					dynamic_cast<CPot*>(pFrontItem)->Add_InCount(1);
					dynamic_cast<CPot*>(pFrontItem)->Set_PotContentTag(L"SoupOnion");
					m_listInven.front()->Set_Dead();
					m_listInven.pop_front();
				}
			}
		}
		else
		{
			m_DestObj->Add_Inven(m_listInven.back());
			m_listInven.pop_front();
		}
	}
	// tomato 를 들고
	else if (m_listInven.front()->Get_ObjTag() == L"Tomato")
	{
		if (m_DestObj->Get_Inven()->size() > 0)
		{
			// 썰기 완료
			_bool bIsFinished = dynamic_cast<CTomato*>(m_listInven.front())->Get_IsChopFinished();
			if (bIsFinished)
			{
				auto pFrontItem = m_DestObj->Get_Inven()->front();
				// 냄비가 있을 경우
				if (pFrontItem->Get_ObjTag() == L"Pot")
				{
					dynamic_cast<CPot*>(pFrontItem)->Add_InCount(1);
					dynamic_cast<CPot*>(pFrontItem)->Set_PotContentTag(L"SoupTomato");
					m_listInven.front()->Set_Dead();
					m_listInven.pop_front();
				}
				// 접시일 경우
				if (pFrontItem->Get_ObjTag() == L"Plate")
				{
					dynamic_cast<CPlate*>(pFrontItem)->Add_TomatoCount(1);
					dynamic_cast<CPlate*>(pFrontItem)->Set_PlateContentTag(L"Salad");
					m_listInven.front()->Set_Dead();
					m_listInven.pop_front();
				}
			}
		}
		else
		{
			m_DestObj->Add_Inven(m_listInven.back());
			m_listInven.pop_front();
		}
	}
	// Lettuce 를 들고
	else if (m_listInven.front()->Get_ObjTag() == L"Lettuce")
	{
		// 빈 장소에 아이템이 있을경우
		if (m_DestObj->Get_Inven()->size() > 0)
		{
			// 썰기 완료
			_bool bIsFinished = dynamic_cast<CLettuce*>(m_listInven.front())->Get_IsChopFinished();
			if (bIsFinished)
			{
				auto pFrontItem = m_DestObj->Get_Inven()->front();
				// 접시일 경우
				if (pFrontItem->Get_ObjTag() == L"Plate")
				{
					dynamic_cast<CPlate*>(pFrontItem)->Add_LettuceCount(1);
					dynamic_cast<CPlate*>(pFrontItem)->Set_PlateContentTag(L"Salad");
					m_listInven.front()->Set_Dead();
					m_listInven.pop_front();
				}
			}
		}
		else
		{
			m_DestObj->Add_Inven(m_listInven.back());
			m_listInven.pop_front();
		}
	}
	// 접시 들고
	else if (m_listInven.front()->Get_ObjTag() == L"Plate")
	{
		// 냄비가 있을 경우
		if (m_DestObj->Get_Inven()->size() > 0)
		{
			// 요리 끝남
			_bool bIsCookFinished = dynamic_cast<CPot*>(m_DestObj->Get_Inven()->front())->Get_IsCookEnd();
			if (bIsCookFinished)
			{
				auto pFrontItem = m_DestObj->Get_Inven()->front();
				if (pFrontItem->Get_ObjTag() == L"Pot")
				{
					dynamic_cast<CPot*>(pFrontItem)->Delete_InCount();
					dynamic_cast<CPot*>(pFrontItem)->Set_IsCookEnd(false);
					dynamic_cast<CPlate*>(m_listInven.front())->Add_InCount(1);

					wstring Tag = dynamic_cast<CPot*>(pFrontItem)->Get_PotContentTag();
					dynamic_cast<CPlate*>(m_listInven.front())->Set_PlateContentTag(Tag);
				}
			}
		}
		else
		{
			m_DestObj->Add_Inven(m_listInven.back());

			m_listInven.pop_front();
		}
	}
	else
	{
		m_DestObj->Add_Inven(m_listInven.back());

		m_listInven.pop_front();
	}
	_vec3 vPos = { vDestPos->_41,vDestPos->_42 + 0.2f,vDestPos->_43 };
	transform->Set_Pos(&vPos);
}

void CChefB::RemoveKnife()
{
	LPSTR meshStr = "Knife";
	m_pMeshCom->Add_RemoveStr(&meshStr);
}

void CChefB::CreateEffectStar()
{
	m_fEffectStarInterval += 1.f;
	if (m_fEffectStarEnd > m_fEffectStarInterval)
		return;
	m_fEffectStarInterval = 0.f;

	auto CurScene = CManagement::GetInstance()->Get_Scene();
	auto CurLayer = CurScene->Get_Layer(L"UI");

	CGameObject* pGameObject = nullptr;

	pGameObject = CEffect::Create(m_pGraphicDev);
	static_cast<CEffect*>(pGameObject)->Set_TextureWstr(L"Effect_StarB");
	auto transform = dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC));

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	transform->Set_Pos(vPos.x, vPos.y + 0.4f, vPos.z);

	NULL_CHECK(pGameObject, E_FAIL);
	CurLayer->Add_ExGameObject(L"Effect", pGameObject);
}

CChefB* CChefB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChefB* pInstance = new CChefB(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChefB::Free(void)
{
	CGameObject::Free();
}

