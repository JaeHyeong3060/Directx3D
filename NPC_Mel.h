#ifndef NPC_Mel_h__
#define NPC_Mel_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CShader;

END
class CNPC_Mel : public CGameObject
{
private:
	explicit CNPC_Mel(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNPC_Mel(const CNPC_Mel& rhs);
	virtual ~CNPC_Mel(void);

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

	_float					m_fTurnTime = 6.f;
	_float					m_fAcctime = 0.f;

public:
	static CNPC_Mel*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // NPC_Mel_h__
