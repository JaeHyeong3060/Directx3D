#ifndef SaladTomato_h__
#define SaladTomato_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class COptimization;
class CShader;

END
class CSaladTomato : public CGameObject
{
private:
	explicit CSaladTomato(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSaladTomato(const CSaladTomato& rhs);
	virtual ~CSaladTomato(void);

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
	CStaticMesh*			m_pMeshCom			= nullptr;
	CTransform*				m_pTransformCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CCalculator*			m_pCalculatorCom	= nullptr;
	COptimization*			m_pOptimizationCom	= nullptr;
	CShader*				m_pShaderCom = nullptr;

	_int					m_iInCount = 0;

public:
	static CSaladTomato*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // SaladTomato_h__
