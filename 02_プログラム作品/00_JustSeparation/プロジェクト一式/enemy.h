//=========================================================
//
// 敵処理 [enemy.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _ENEMY_H_     // このマクロ定義がされてなかったら
#define _ENEMY_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// マクロ定義
//===============================================
#define ENEMY_SIZEX		(30)								// 幅（半分）
#define ENEMY_SIZEY		(30)								// 高さ（半分）
#define ENEMY_SCORE		(500)								// 敵のスコア

//===============================================
// 敵クラス
//===============================================
class CEnemy : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CEnemy();						// デフォルトコンストラクタ
	CEnemy(int nPriority = 2);		// オーバーロードされたコンストラクタ
	~CEnemy();						// デストラクタ

	static CEnemy *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;	// 使用するテクスチャの番号
};

#endif