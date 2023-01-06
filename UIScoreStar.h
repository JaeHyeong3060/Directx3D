#ifndef UIScoreStar_h__
#define UIScoreStar_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUIScoreStar : public CGameObject
{
private:
	explicit CUIScoreStar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUIScoreStar(const CUIScoreStar& rhs);
	virtual ~CUIScoreStar(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void		Set_Delay(_float fdelya) { m_fDelay = fdelya; }

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;

	_float					m_fTimeSpeed;

	_float					m_fDelay = 0.f;

	_bool					m_bOnceEnv = true;

public:
	static CUIScoreStar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // UIScoreStar_h__
