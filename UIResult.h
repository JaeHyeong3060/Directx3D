#ifndef UIResult_h__
#define UIResult_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUIResult : public CGameObject
{
private:
	explicit CUIResult(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUIResult(const CUIResult& rhs);
	virtual ~CUIResult(void);

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

public:
	static CUIResult*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // UIResult_h__
