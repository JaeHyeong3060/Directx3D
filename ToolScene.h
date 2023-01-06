#ifndef ToolScene_h__
#define ToolScene_h__

#include "Scene.h"
#include "Define.h"
#include "BackGround.h"
#include "Player.h"
#include "Monster.h"

#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"

#include "Stone.h"
#include "Sword.h"

class CToolScene : public CScene
{
private:
	explicit CToolScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolScene(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo(void);

private:
	_ulong			m_dwRenderCnt = 0;
	_tchar			m_szFPS[256];
	_float			m_fTime = 0.f;

public:
	static CToolScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);


};

#endif // ToolScene_h__
