#ifndef _ZFRUSTUM_H_
#define _ZFRUSTUM_H_

#include <d3d9.h>
#include <d3dx9.h>

/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	5.0f

/**
 * �������� �ø��� �ϱ����� Ŭ����
 *
 */
class ZFrustum
{
private:
	static ZFrustum* m_pThis;

	D3DXVECTOR3	m_vtx[8];	/// ���������� ������ ���� 8��
	D3DXVECTOR3	m_vPos;		/// ���� ī�޶��� ������ǥ
	D3DXPLANE	m_plane[6];	/// ���������� �����ϴ� 6���� ���

	ZFrustum();

	
public:
	static ZFrustum* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new ZFrustum();
		}

		return m_pThis;
	}

	static void Release()
	{
		if (m_pThis)
		{
			delete m_pThis;
			m_pThis = NULL;
		}
	}
	
	/// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
	BOOL	Make( D3DXMATRIXA16* pmatViewProj );

	/// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	BOOL	IsIn( D3DXVECTOR3* pv );

	/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
	 *  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	 */
	BOOL	IsInSphere( D3DXVECTOR3* pv, float radius );

	/// ���������� ȭ�鿡 �׷��ش�.
	BOOL	Draw( LPDIRECT3DDEVICE9 pDev );

	/// ���� ī�޶��� ������ǥ�� ����ش�.
	D3DXVECTOR3*	GetPos() { return &m_vPos; }
};

/// ����� �������� �������ϴ� �Լ�, ����� ���ǰ� ���� �ʴ�.
void ZMakePlane( D3DXPLANE* pPlane, D3DXVECTOR3* pv0, D3DXVECTOR3* pv1, D3DXVECTOR3* pv2 );

#endif // _ZFRUSTUM_H_