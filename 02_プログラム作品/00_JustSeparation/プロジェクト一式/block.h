//=========================================================
//
// ブロック処理 [bullet.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _BLOCK_H_     // このマクロ定義がされてなかったら
#define _BLOCK_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// マクロ定義
//===============================================
#define BLOCK_SIZEX		(60)				// 幅（半分）
#define BLOCK_SIZEY		(60)				// 高さ（半分）

//===============================================
// ブロッククラス
//===============================================
class CBlock : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CBlock();						// デフォルトコンストラクタ
	CBlock(int nPriority = 3);		// オーバーロードされたコンストラクタ
	~CBlock();						// デストラクタ

	static CBlock *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	static bool CollisionPlayer(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY);
	static bool Collision(const TYPE Objtype, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY);

	static int GetNumAll(void) { return m_nNumAll; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;	// 使用するテクスチャの番号
	static int m_nNumAll;		// ブロックの総数
};

#endif