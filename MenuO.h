#ifndef MenuO_h__
#define MenuO_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CMenuO : public CGameObject
{
private:
	explicit CMenuO(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMenuO(const CMenuO& rhs);
	virtual ~CMenuO(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	HRESULT					Set_MenuTex(wstring wstrKey);

	wstring					Get_wstrMenuTag() { return m_wstrMenuTag; }


private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	CGameObject*			m_pObjectTimeGauge = nullptr;

	_matrix					m_matProj;

	_float					m_fTimeSpeed;

	wstring					m_wstrMenuTag = L"";



	_bool					m_bIsEndLoad = false;
	_bool					m_bIsEndGauge = false;

public:
	static CMenuO*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // MenuO_h__
