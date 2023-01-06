#ifndef Empty_h__
#define Empty_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class COptimization;
class CSphereCollider;
class CShader;

END
class CEmpty : public CGameObject
{
private:
	explicit CEmpty(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEmpty(const CEmpty& rhs);
	virtual ~CEmpty(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:	
	CTransform*				m_pTransformCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CCalculator*			m_pCalculatorCom	= nullptr;
	COptimization*			m_pOptimizationCom	= nullptr;

	CSphereCollider*		m_pSphereColliderCom = nullptr;

	CShader*				m_pShaderCom = nullptr;
	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;


public:
	static CEmpty*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Empty_h__
