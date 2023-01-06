#include "stdafx.h"
#include "PlayerCamera.h"

#include "Export_Function.h"

CPlayerCamera::CPlayerCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CPlayerCamera::~CPlayerCamera(void)
{

}

HRESULT CPlayerCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);


	return S_OK;
}

Engine::_int CPlayerCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta, 10.f);

	if (0 == m_bFixCnt)
	{
		Mouse_Fix();
		Mouse_Move(fTimeDelta);
	}
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	_int	iExit = CCamera::Update_Object(fTimeDelta);

	if (1 == m_bFixCnt)
	{
		auto CurScene = CManagement::GetInstance()->Get_Scene();
		auto CurLayer = CurScene->Get_Layer(L"GameLogic");

		list<CGameObject*>* pExMapObject = CurLayer->Get_ExMapObject(L"Player");

		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pExMapObject->front()->Get_Component(L"Com_Transform", ID_DYNAMIC));
		if (pPlayerTransform == nullptr) return iExit;

		_vec3		vPlayerPos;
		pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

		m_vEye = _vec3{ vPlayerPos.x,vPlayerPos.y + 8.f,vPlayerPos.z - 4.f };
		m_vAt = _vec3{ vPlayerPos.x,vPlayerPos.y,vPlayerPos.z };
	}
	if (2 == m_bFixCnt)
	{
		auto CurScene = CManagement::GetInstance()->Get_Scene();
		auto CurLayer = CurScene->Get_Layer(L"GameLogic");

		list<CGameObject*>* pExMapObject = CurLayer->Get_ExMapObject(L"Player");

		auto& iter = pExMapObject->begin();
		++iter;

		CTransform* pPlayerTransform = dynamic_cast<CTransform*>((*iter)->Get_Component(L"Com_Transform", ID_DYNAMIC));
		if (pPlayerTransform == nullptr) return iExit;

		_vec3		vPlayerPos;
		pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

		m_vEye = _vec3{ vPlayerPos.x,vPlayerPos.y + 8.f,vPlayerPos.z - 4.f };
		m_vAt = _vec3{ vPlayerPos.x,vPlayerPos.y,vPlayerPos.z };
	}

	return iExit;
}

void CPlayerCamera::Key_Input(const _float& fTimeDelta, const _float& fSpeed)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;
		
		m_vEye += vLength;
		m_vAt	+= vLength;
	}

	if (Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}


	if (Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Get_DIKeyState(DIK_TAB) & 0x80)
	{
	/*	if (true == m_bClick)
			return;

		m_bClick = true;*/

		m_bFixCnt = 0;

	}
	if (Get_DIKeyState(DIK_1) & 0x80)
	{
		//if (true == m_bClick)
		//	return;

		//m_bClick = true;

		m_bFixCnt = 1;

	}
	if (Get_DIKeyState(DIK_2) & 0x80)
	{
		//if (true == m_bClick)
		//	return;

		//m_bClick = true;

		m_bFixCnt = 2;

	}
	/*else
		m_bClick = false;*/



	if (0 == m_bFixCnt)
		return;

}

void CPlayerCamera::Mouse_Move(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long	dwMouseMove = 0;

	if (dwMouseMove = Get_DIMouseMove(DIMS_Y))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3		vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Get_DIMouseMove(DIMS_X))
	{
		_vec3		vUp = _vec3(0.f, 1.f, 0.f);
	
		_vec3		vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

}

void CPlayerCamera::Mouse_Fix(void)
{
	POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


}

CPlayerCamera* CPlayerCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CPlayerCamera*	pInstance = new CPlayerCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayerCamera::Free(void)
{
	CCamera::Free();
}
