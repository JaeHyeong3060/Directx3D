#ifndef Stage_h__
#define Stage_h__

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

class CStage : public CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage(void);

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

	HRESULT			Load_GameData(CLayer* pLayer);
	HRESULT			Load_TilePos(CLayer* pLayer);


private:
	_ulong			m_dwRenderCnt = 0;
	_tchar			m_szFPS[256];
	_float			m_fTime = 0.f;

	list<CGameObject*> m_EffectObj;

public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);


};

#endif // Stage_h__
