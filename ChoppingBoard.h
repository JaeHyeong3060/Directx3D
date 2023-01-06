#ifndef ChoppingBoard_h__
#define ChoppingBoard_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CSphereCollider;

class COptimization;
class CShader;

END
class CChoppingBoard : public CGameObject
{
private:
	explicit CChoppingBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChoppingBoard(const CChoppingBoard& rhs);
	virtual ~CChoppingBoard(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void	Set_StartChop(_bool chop);

	void	Reset_Time();

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:	
	CStaticMesh*			m_pMeshCom			= nullptr;
	CTransform*				m_pTransformCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CCalculator*			m_pCalculatorCom	= nullptr;
	//CCollider*				m_pColliderCom		= nullptr;
	CSphereCollider*		m_pSphereColliderCom = nullptr;

	COptimization*			m_pOptimizationCom	= nullptr;
	CShader*				m_pShaderCom = nullptr;
	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;

	_bool					m_bStartChop = false;

	CGameObject*			pUITimerObj = nullptr;

	CGameObject*			pEffectObj = nullptr;

	_bool					m_bCookEffect = 1;
	

public:
	static CChoppingBoard*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // ChoppingBoard_h__
