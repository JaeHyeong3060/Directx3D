#ifndef DirtyPlate_h__
#define DirtyPlate_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class COptimization;
class CShader;

END
class CDirtyPlate : public CGameObject
{
private:
	explicit CDirtyPlate(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDirtyPlate(const CDirtyPlate& rhs);
	virtual ~CDirtyPlate(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void					Add_InCount(int number) { m_iInCount += number; };
	void					Delete_InCount() { m_iInCount = 0; }

	void					Set_DirtyPlateContentTag(wstring wstrTag);
	wstring					Get_DirtyPlateContentTag() { return m_wstrDirtyPlateContent; }

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
	CCollider*				m_pColliderCom		= nullptr;
	COptimization*			m_pOptimizationCom	= nullptr;
	CShader*				m_pShaderCom = nullptr;
	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;

	CGameObject*			pSoup = nullptr;
	_int					m_iInCount = 0;

	wstring					m_wstrDirtyPlateContent = L"";

public:
	static CDirtyPlate*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // DirtyPlate_h__
