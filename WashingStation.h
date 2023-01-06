#ifndef WashingStation_h__
#define WashingStation_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CSphereCollider;
class COptimization;
class CShader;

END
class CWashingStation : public CGameObject
{
private:
	explicit CWashingStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWashingStation(const CWashingStation& rhs);
	virtual ~CWashingStation(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void					Set_Trigger(_bool trigger) { m_bTrigger = trigger; }

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	CStaticMesh*			m_pMeshCom			= nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	COptimization*			m_pOptimizationCom = nullptr;
	CSphereCollider*		m_pSphereColliderCom = nullptr;

	CShader*				m_pShaderCom = nullptr;
	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;

	_bool					m_bTrigger = false;

	_float					m_fPassTimer = 3.f;
	_float					m_fAccPassTimer = 3.f;

public:
	static CWashingStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // WashingStation_h__