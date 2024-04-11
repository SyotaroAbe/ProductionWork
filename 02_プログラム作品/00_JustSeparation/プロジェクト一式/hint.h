//=========================================================
//
// ヒント描画処理 [hint.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _HINT_H_     // このマクロ定義がされてなかったら
#define _HINT_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// ヒント描画クラス
//===============================================
class CHint : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CHint();						// デフォルトコンストラクタ
	CHint(int nPriority = 0);		// オーバーロードされたコンストラクタ
	~CHint();						// デストラクタ

	static CHint *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_IdxTexture;			// 使用するテクスチャの番号
	int m_nTime;						// 経過時間
	bool m_bDisp;						// 表示するか
};

#endif