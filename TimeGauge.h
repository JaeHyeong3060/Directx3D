#ifndef TimeGauge_h__
#define TimeGauge_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CTimeGauge : public CGameObject
{
private:
	explicit CTimeGauge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTimeGauge(const CTimeGauge& rhs);
	virtual ~CTimeGauge(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	virtual void Render_Object_Ex(void);

	_bool		Get_IsEndGauge() { return m_bIsEndGauge; }

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	CShader* m_pShaderCom = nullptr;

	_matrix					m_matProj;

	_float					m_fTimeSpeed;

	_bool					m_bIsEndGauge = false;
	_bool					m_bIsEndLoad = false;

public:
	static CTimeGauge*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // TimeGauge_h__
