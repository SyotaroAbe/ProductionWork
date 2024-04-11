//=========================================================
//
// ゲージフル描画処理 [gagefull.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _GAGEFULL_H_     // このマクロ定義がされてなかったら
#define _GAGEFULL_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "objectBillboard.h"

//===============================================
// ゲージフルクラス
//===============================================
class CGageFull : public CObjectBillboard
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CGageFull();						// デフォルトコンストラクタ
	CGageFull(int nPriority = 2);		// オーバーロードされたコンストラクタ
	~CGageFull();						// デストラクタ

	static CGageFull *Create(int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const bool bMax);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;			// 使用するテクスチャの番号
	bool m_bMaxScaling;					// 最大のサイズになったか
	bool m_bMaxGage;					// ゲージが最大か
};

#endif