#ifndef Soup_h__
#define Soup_h__

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
class CSoup : public CGameObject
{
private:
	explicit CSoup(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSoup(const CSoup& rhs);
	virtual ~CSoup(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void					Set_Cooking(_bool bCook) { m_bIsCooking = bCook; }
	_bool					Get_Cooking() { return m_bIsCooking; }

	void					Set_SoupContentTag(wstring wstrTag) { m_wstrSoupTag = wstrTag; };
	wstring					Get_SoupContentTag() { return m_wstrSoupTag; }

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

	

private:	
	CDynamicMesh*			m_pMeshOnionCom		= nullptr;
	CDynamicMesh*			m_pMeshTomatoCom	= nullptr;
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

	_bool					m_bIsCooking = false;

	wstring					m_wstrSoupTag = L"";

public:
	static CSoup*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Soup_h__
