#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END
class CUIDigit :
    public CGameObject
{
private:
    explicit CUIDigit(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUIDigit(void);
public:
    void Set_Number(int number) { m_iNumber = number; };
    void Set_RGB(RGBTRIPLE& RGB) { m_RGB = RGB; }
public:
    // CGameObject을(를) 통해 상속됨
    virtual		HRESULT		Ready_Object(void) override;
    virtual		_int		Update_Object(const _float& fTimeDelta) override;
    virtual		void		Render_Object(void) override;
private:
    HRESULT					Add_Component(void);

public:
    static CUIDigit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    virtual void		Free(void);
private:
    CRcTex*		            m_pBufferCom;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

    _matrix					m_matProj;

    _int    m_iNumber;

    _vec3 m_vPos;
    _int m_iAlpha;

    RGBTRIPLE m_RGB;
};

