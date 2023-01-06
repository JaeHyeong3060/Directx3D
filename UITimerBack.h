#ifndef UITimerBack_h__
#define UITimerBack_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUITimerBack : public CGameObject
{
private:
	explicit CUITimerBack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUITimerBack(const CUITimerBack& rhs);
	virtual ~CUITimerBack(void);

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

	_ulong					m_dwRenderCnt = 200;
	_tchar					m_szTimer[256];
	_float					m_fTime = 0.f;


	_bool					m_bIsEndLoad = false;
	_bool					m_bIsEndGauge = false;

	_bool					m_bEnd = false;
	_float					m_fEndtime = 5.f;

	_bool					m_bOnce1 = false;
	_bool					m_bOnce2 = false;
	_bool					m_bOnce3 = false;
	_bool					m_bOnce4 = false;
	_bool					m_bOnce5 = false;
	_bool					m_bOnce6 = false;
	_bool					m_bOnce7 = false;
	_bool					m_bOnce8 = false;

public:
	static CUITimerBack*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // UITimerBack_h__
