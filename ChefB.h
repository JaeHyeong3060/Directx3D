#ifndef ChefB_h__
#define ChefB_h__



#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CSphereCollider;
class CNaviMesh;
class CShader;

END
class CChefB : public CGameObject
{
private:
	explicit CChefB(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChefB(const CChefB& rhs);
	virtual ~CChefB(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);
	void					Key_Input(const _float& fTimeDelta);
	void					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	_bool					Body_Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);


	void					PUSH_KEY_Z_COL_EMPTY();

	void					RemoveKnife();

	void					CreateEffectStar();

private:
	CDynamicMesh* m_pMeshCom = nullptr;
	CNaviMesh* m_pNaviCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CCalculator* m_pCalculatorCom = nullptr;
	CShader* m_pShaderCom = nullptr;
	CSphereCollider* m_pSphereColliderCom = nullptr;
	CSphereCollider* m_pSphereColliderBodyCom = nullptr;
	_vec3					m_vDir;

	CSphereCollider* m_DestCol = nullptr;
	CGameObject* m_DestObj = nullptr;

	_bool					m_bColl = false;
	_bool					m_bBodyColl = false;

	_float					m_fSpeed = 2.f;
	_vec3					m_vOffset;
	DWORD					m_dwColState = 0;

	_bool					m_bCookState = false;
	_bool					m_bWashState = false;

	_int					m_OnionIndex = 0;
	_int					m_TomatoIndex = 0;

	_float					m_fEffectStarEnd = 15.f;
	_float					m_fEffectStarInterval = 1.f;


public:
	static CChefB* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // ChefB_h__
