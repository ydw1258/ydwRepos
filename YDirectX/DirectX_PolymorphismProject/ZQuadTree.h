#pragma once
#include "ZFrustum.h"
//#include "CUSTOM_DATATYPES.h"

//for문으로 전체 충돌을 확인하는 것보다 depth가 깊더라도 전체를 확인하는 것보다는 연산이 적다
//데이터가 많아지면 (Depth가 깊더라도 for문으로 전체를 확인하는 것보다 효율적이다.)
//xz만 처리한 예제 (y축처리 (날아다니는)까지한 OctaTree 컬링도 존재)
class ZQuadTree
{
	/// 쿼드트리에 보관되는 4개의 코너값에 대한 상수값
	enum			CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };

	/// 쿼드트리와 프러스텀간의 관계
	enum			QuadLocation {
		FRUSTUM_OUT = 0,				/// 프러스텀에서 완전벗어남
		FRUSTUM_PARTIALLY_IN = 1,	/// 프러스텀에 부분포함
		FRUSTUM_COMPLETELY_IN = 2,	/// 프러스텀에 완전포함
		FRUSTUM_UNKNOWN = -1
	};		/// 모르겠음(^^;)

private:
	ZQuadTree*	m_pChild[4];		/// QuadTree의 4개의 자식노드

	int			m_nCenter;			/// QuadTree에 보관할 첫번째 값
	int			m_nCorner[4];		/// QuadTree에 보관할 두번째 값
									///    TopLeft(TL)      TopRight(TR)
									///              0------1
									///              |      |
									///              |      |
									///              2------3
									/// BottomLeft(BL)      BottomRight(BR)
	BOOL		m_bCulled;			/// 프러스텀에서 컬링된 노드인가?
	float		m_fRadius;			/// 노드를 감싸는 경계구(bounding sphere)의 반지름

private:
	/// 자식 노드를 추가한다.
	ZQuadTree*	_AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	/// 4개의 코너값을 셋팅한다.
	BOOL		_SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	/// Quadtree를 4개의 하위 트리로 부분분할(subdivide)한다.
	BOOL		_SubDivide();		// Quadtree를 subdivide한다.

	/// 현재 노드가 출력이 가능한 노드인가?
	BOOL		_IsVisible() { return (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1); }

	/// 출력할 폴리곤의 인덱스를 생성한다.
	int			_GenTriIndex(int nTris, LPVOID pIndex);

	/// 메모리에서 쿼드트리를 삭제한다.
	void		_Destroy();

	/// 현재노드가 프러스텀에 포함되는가?
	int			_IsInFrustum(TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum);

	/// _IsInFrustum()함수의 결과에 따라 프러스텀 컬링 수행
	void		_FrustumCull(TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum);
public:
	/// 최초 루트노드 생성자
	ZQuadTree(int cx, int cy);

	/// 하위 자식노드 생성자
	ZQuadTree(ZQuadTree* pParent);

	/// 소멸자
	~ZQuadTree();

	/// QuadTree를 구축한다.
	BOOL		Build(TERRAINVERTEX* pHeightMap);

	///	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다.
	int			GenerateIndex(LPVOID pIndex, TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum);
};