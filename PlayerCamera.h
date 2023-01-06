#ifndef PlayerCamera_h__
#define PlayerCamera_h__

#include "Camera.h"
#include "Define.h"

class CPlayerCamera : public CCamera
{
private:
	explicit CPlayerCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerCamera(const CPlayerCamera& rhs);
	virtual ~CPlayerCamera(void);

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& fTimeDelta) override;

private:
	void			Key_Input(const _float& fTimeDelta, const _float& fSpeed);
	void			Mouse_Move(const _float& fTimeDelta);
	void			Mouse_Fix(void);
	
private:
	_bool			m_bClick	= false;
	_int			m_bFixCnt		= 1;

public:
	static CPlayerCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, 
										const _float& fFov, const _float& fAspect, 
										const _float& fNear, const _float& fFar);

private:
	virtual void		Free(void);
};


#endif // PlayerCamera_h__
