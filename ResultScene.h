#ifndef ResultScene_h__
#define ResultScene_h__

#include "Scene.h"
#include "Define.h"
#include "BackGround.h"
#include "Player.h"
#include "Chef.h"
#include "Monster.h"

#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"

#include "Stone.h"
#include "Sword.h"
#include "Tree.h"
#include "Effect.h"
#include "UI.h"

class CResultScene : public CScene
{
private:
	explicit CResultScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CResultScene(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

	void			Add_EffectObj(CGameObject* pObj);

private:
	HRESULT			Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo(void);

private:
	_ulong			m_dwRenderCnt = 0;
	_tchar			m_szFPS[256];
	_float			m_fTime = 0.f;

	_float			m_fTotalDelay = 3.f;
	_float			m_fTotalDelayB = 5.f;

	_bool			m_bOnceEnv = true;
	_bool			m_bOnceEnvB = true;

	list<CGameObject*> m_EffectObj;

public:
	static CResultScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);


};

#endif // ResultScene_h__
