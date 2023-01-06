#ifndef Plate_h__
#define Plate_h__

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
class CPlate : public CGameObject
{
private:
	explicit CPlate(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlate(const CPlate& rhs);
	virtual ~CPlate(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void					Add_InCount(int number) { m_iInCount += number; };
	void					Delete_InCount() { m_iInCount = 0; }

	void					Add_TomatoCount(int number) { m_iTomatoCount += number; };
	void					Add_LettuceCount(int number) { m_iLettuceCount += number; };

	_int					Get_TomatoCount() { return m_iTomatoCount; };
	_int					Get_LettuceCount() { return m_iLettuceCount; };

	void					Set_PlateContentTag(wstring wstrTag);
	wstring					Get_PlateContentTag() { return m_wstrPlateContent; }

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
	CGameObject*			pSaladTomato = nullptr;
	CGameObject*			pSaladLettuce = nullptr;


	_int					m_iInCount = 0;

	_int					m_iLettuceCount = 0;
	_int					m_iTomatoCount = 0;

	wstring					m_wstrPlateContent = L"";

public:
	static CPlate*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Plate_h__
