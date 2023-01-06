#ifndef NPC_Penguin_h__
#define NPC_Penguin_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CShader;

END
class CNPC_Penguin : public CGameObject
{
private:
	explicit CNPC_Penguin(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNPC_Penguin(const CNPC_Penguin& rhs);
	virtual ~CNPC_Penguin(void);

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
	static CNPC_Penguin*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // NPC_Penguin_h__
