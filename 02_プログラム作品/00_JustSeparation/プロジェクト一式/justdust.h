//=========================================================
//
// JustDust表示処理 [justdust.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _JUSTDUST_H_     // このマクロ定義がされてなかったら
#define _JUSTDUST_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "objectBillboard.h"

//===============================================
// マクロ定義
//===============================================

//===============================================
// ジャストタイミング表示クラス
//===============================================
class CJustDust : public CObjectBillboard
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CJustDust();					// デフォルトコンストラクタ
	CJustDust(int nPriority = 5);	// オーバーロードされたコンストラクタ
	~CJustDust();					// デストラクタ

	static CJustDust *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(bool bScall);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;				// 使用するテクスチャの番号
	int m_nTime;							// 経過時間
	bool m_bScall;							// サイズ変更フラグ
};

#endif