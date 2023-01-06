#ifndef UIScoreBack_h__
#define UIScoreBack_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUIScoreBack : public CGameObject
{
private:
	explicit CUIScoreBack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUIScoreBack(const CUIScoreBack& rhs);
	virtual ~CUIScoreBack(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;

	_float					m_fTimeSpeed;

	_ulong					m_dwRenderCnt = 180;
	_tchar					m_szTimer[256];
	_float					m_fTime = 0.f;


	_bool					m_bIsEndLoad = false;
	_bool					m_bIsEndGauge = false;

public:
	static CUIScoreBack*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // UIScoreBack_h__
