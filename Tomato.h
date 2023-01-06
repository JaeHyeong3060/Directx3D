#ifndef Tomato_h__
#define Tomato_h__

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
class CTomato : public CGameObject
{
private:
	explicit CTomato(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTomato(const CTomato& rhs);
	virtual ~CTomato(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	_bool					Get_IsChopFinished() { return m_bChopFinished; }

	HRESULT					Set_TomatoIndex(int iIndex);

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);



private:
	CDynamicMesh* m_pMeshCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCalculator* m_pCalculatorCom = nullptr;
	COptimization* m_pOptimizationCom = nullptr;
	//CCollider*				m_pColliderCom		= nullptr;
	//CSphereCollider*		m_pSphereColliderCom = nullptr;

	CShader* m_pShaderCom = nullptr;
	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;

	_bool					m_bChopFinished = false;

	_bool					m_bAnimChop = false;
	_bool					m_bAnimGather = false;



public:
	static CTomato* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Tomato_h__
