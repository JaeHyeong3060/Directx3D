#ifndef Onion_h__
#define Onion_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class COptimization;
class CShader;
class CSphereCollider;

END
class COnion : public CGameObject
{
private:
	explicit COnion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COnion(const COnion& rhs);
	virtual ~COnion(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	_bool					Get_IsChopFinished() { return m_bChopFinished; }

	HRESULT					Set_OnionIndex(int iIndex);

	void					Set_ChopFinished(_bool chop) { m_bChopFinished = chop; }

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);


	

private:	
	CDynamicMesh*			m_pMeshCom			= nullptr;
	CTransform*				m_pTransformCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CCalculator*			m_pCalculatorCom	= nullptr;
	COptimization*			m_pOptimizationCom	= nullptr;
	//CCollider*				m_pColliderCom		= nullptr;
	//CSphereCollider*		m_pSphereColliderCom = nullptr;

	CShader*				m_pShaderCom = nullptr;
	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;

	_bool					m_bChopFinished = false;

	_bool					m_bAnimChop = false;
	_bool					m_bAnimGather = false;

	_float					m_fDelay = 1.f;



public:
	static COnion*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Onion_h__
