#ifndef NPC_Waiter_h__
#define NPC_Waiter_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CShader;

END
class CNPC_Waiter : public CGameObject
{
private:
	explicit CNPC_Waiter(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNPC_Waiter(const CNPC_Waiter& rhs);
	virtual ~CNPC_Waiter(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:	
	CDynamicMesh*			m_pMeshCom			= nullptr;
	CTransform*				m_pTransformCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CShader*				m_pShaderCom = nullptr;

	_uint					m_iFlag;

public:
	static CNPC_Waiter*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // NPC_Waiter_h__
