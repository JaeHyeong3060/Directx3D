#ifndef Pot_h__
#define Pot_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class COptimization;
class CShader;

END
class CPot : public CGameObject
{
private:
	explicit CPot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPot(const CPot& rhs);
	virtual ~CPot(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void					Add_InCount(int number);
	void					Delete_InCount() { m_iInCount = 0; }

	void					Set_IsCookEnd(_bool bCook) { m_bCookEnd = bCook; }
	_bool					Get_IsCookEnd() { return m_bCookEnd; }

	void					Set_Cooking(_bool bCook);
	_bool					Get_Cooking() { return m_bIsCooking; }

	void					Set_PotContentTag(wstring wstrTag) { m_wstrPotContent = wstrTag; };
	wstring					Get_PotContentTag() { return m_wstrPotContent; }

	void					Add_Food();

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

	CGameObject*			pSoup = nullptr;

	CGameObject*			pUITimerObj = nullptr;


	_bool					m_bIsEndGauge = false;

	

	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;

	_bool					m_bCookEnd = false;

	_int					m_iInCount;

	_bool					m_bIsCooking = false;

	wstring					m_wstrPotContent = L"";

public:
	static CPot*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Pot_h__
