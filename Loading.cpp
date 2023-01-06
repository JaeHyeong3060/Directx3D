#include "stdafx.h"
#include "Loading.h"
#include "Export_Function.h"




CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eID = eID;

	return S_OK;
}

Engine::_uint CLoading::LoadingForStage(void)
{
	lstrcpy(m_szLoading, L"Texture Loading.................");
	// 버퍼
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	
	// 텍스쳐
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", TYPE_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Terrain2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TYPE_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Effect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TYPE_NORMAL, 90)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UI", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/hpbar.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Filter", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Filter.bmp", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Aura", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Aura0.tga", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SoupOnion", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SoupOnion.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SoupTomato", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SoupTomato.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Salad", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Salad.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Digit", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Digit/Number%d.png", TYPE_NORMAL, 10)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_TimeGauge", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/TimeGauge.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UITimerBack", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UITimerBack.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UITimerObj", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UITimerObj.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UIScoreBack", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UIScoreBack.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UIResult", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UIResult.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UIScoreStar", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UIScoreStar.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_TimeUp", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/TimeUp.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Black", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Black.png", TYPE_NORMAL, 1)), E_FAIL);


	
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Effect_Chop", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/Testhit_%d.png", TYPE_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Effect_Star", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/Star_%d.png", TYPE_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Effect_StarB", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/STARB_%d.png", TYPE_NORMAL, 5)), E_FAIL);

	// 스태틱 메쉬
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Tree", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Tree/", L"Tree01.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Stone", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/TombStone/", L"TombStone.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Van", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Van/", L"Van.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_WorkTop", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/WorkTop/", L"WorkTop.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Pot", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Pot/", L"Pot.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_WashingStation", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/WashingStation/", L"WashingStation.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_CookingStation", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/CookingStation/", L"CookingStation.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_ChoppingBoard", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/ChoppingBoard/", L"ChoppingBoard.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_ThePass", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/ThePass/", L"ThePass.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Pot", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Pot/", L"Pot.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Plate", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Plate/", L"Plate.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_DirtyPlate", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Plate/", L"DirtyPlate.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_MyMap", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/MyMap/", L"MyMap.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Salad_Tomato", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Salad/", L"Salad_Tomato.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Salad_Lettuce", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Salad/", L"Salad_Lettuce.X")), E_FAIL);

	//다이나믹 메쉬
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_NPC_Buck", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC_Buck/", L"NPC_Buck.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_NPC_Business", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC_Business/", L"NPC_Business.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_NPC_Mel", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC_Mel/", L"NPC_Mel.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_NPC_Penguin", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC_Penguin/", L"NPC_Penguin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_NPC_Waiter", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC_Waiter/", L"NPC_Eskimo.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Crate", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Crate/", L"Crate.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_CrateB", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/CrateB/", L"CrateB.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_CrateC", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/CrateC/", L"CrateC.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Tomato", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Tomato/", L"Tomato.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_TomatoB", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Tomato/", L"Tomato.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_TomatoC", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Tomato/", L"Tomato.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_TomatoD", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Tomato/", L"Tomato.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_TomatoE", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Tomato/", L"Tomato.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_TomatoF", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Tomato/", L"Tomato.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Onion", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Onion/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_OnionB", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Onion/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_OnionC", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Onion/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_OnionD", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Onion/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_OnionE", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Onion/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_OnionF", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Onion/", L"Onion.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Lettuce", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Lettuce/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_LettuceB", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Lettuce/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_LettuceC", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Lettuce/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_LettuceD", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Lettuce/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_LettuceE", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Lettuce/", L"Onion.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_LettuceF", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Lettuce/", L"Onion.X")), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Soup", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Soup/", L"Soup.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_SoupT", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Soup/", L"SoupT.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Chef", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Chef/", L"Chef.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_ChefB", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/ChefB/", L"ChefB.X")), E_FAIL);

	//Ready_Proto(L"Proto_Mesh_Player", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/PlayerXfile/", L"Player.X"));


	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Tree", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Tree/", L"Tree01.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Stone",CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/TombStone/", L"TombStone.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Navi", CNaviMesh::Create(m_pGraphicDev)), E_FAIL);

#pragma region PLAYER
//	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Sword", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Sword/", L"Sword.X")), E_FAIL);
//	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Player", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/PlayerXfile/", L"Player.X")), E_FAIL);

#pragma endregion PLAYER
		
	// 기타 등등
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Optimization", COptimization::Create(m_pGraphicDev, true, VTXCNTX, VTXCNTZ)), E_FAIL);



	m_bFinish = true;



	lstrcpy(m_szLoading, L"Loading Complete!!!!!!!!!!!!!!!!!!!!!!");


	return 0;
}

Engine::_uint CLoading::LoadingForBoss(void)
{
	// 수업상 정의한 코드

	return 0;
}

HRESULT CLoading::LoadDataPath(const wstring& wstrFilePath)
{
	
	return S_OK;
}


unsigned int CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->LoadingForStage();
		break;

	case LOADING_BOSS:
		iFlag = pLoading->LoadingForBoss();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	// _endthreadex(0);		리턴값이 0인 경우엔 _endthreadex함수가 자동 호출되어 문제가 없지만 그렇지 않은 경우에는 예외처리로 이 함수를 호출할 수 있도록 설계해야 한다.
	
	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);

}

