#ifndef UITimerObj_h__
#define UITimerObj_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class COptimization;
class CShader;
class CSphereCollider;

END
class CUITimerObj : public CGameObject
{
private:
	explicit CUITimerObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUITimerObj(const CUITimerObj& rhs);
	virtual ~CUITimerObj(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void					Set_TimeStart(_bool set) { m_bTimeStart = set; }
	void					Set_RightX(_float fX = 0.3f);
	void					Set_TimeSpeed(_float fSpeed) { m_fTimeSpeed = fSpeed; }

	void					Set_ResetTime() { m_fRightX = m_fLeftX; }

	void					Set_IsEndGauge(_bool IsEnd) { m_bIsEndGauge = IsEnd; }
	_bool					Get_IsEndGauge() { return m_bIsEndGauge; }


private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);



private:
	CTransform* m_pTransformCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCalculator* m_pCalculatorCom = nullptr;
	COptimization* m_pOptimizationCom = nullptr;

	CRcTex* m_pBufferComGauge = nullptr;
	CTexture* m_pTextureGauge = nullptr;

	CShader* m_pShaderCom = nullptr;

	_bool					m_bDraw = false;


	_float					m_fLeftX = -0.2f;
	_float					m_fTopY = 0.6f;
	_float					m_fDownY = 0.5f;
	_float					m_fRightX = 0.2f;

	_float					m_fTimeSpeed = 0.1f;

	_bool					m_bIsEndGauge = false;
	_bool					m_bTimeStart = false;



public:
	static CUITimerObj* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // UITimerObj_h__
