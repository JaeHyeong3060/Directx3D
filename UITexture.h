#ifndef UITexture_h__
#define UITexture_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUITexture : public CGameObject
{
private:
	explicit CUITexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUITexture(const CUITexture& rhs);
	virtual ~CUITexture(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	HRESULT					Set_MenuTex(wstring wstrKey);

	wstring					Get_wstrMenuTag() { return m_wstrMenuTag; }

	void					Set_ExistTime(_float fTime) { m_fExistTime = fTime; }

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;

	_float					m_fTimeSpeed;

	wstring					m_wstrMenuTag = L"";

	_bool					m_bIsEndLoad = false;
	_bool					m_bIsEndGauge = false;

	_float					m_fExistTime = 9900.f;

	_int					m_iAlpha = 255.f;

public:
	static CUITexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);


	
};


#endif // UITexture_h__
