//=========================================================
//
// UI処理 [ui.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _UI_H_     // このマクロ定義がされてなかったら
#define _UI_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// UIクラス
//===============================================
class CUi : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CUi();						// デフォルトコンストラクタ
	CUi(int nPriority = 0);		// オーバーロードされたコンストラクタ
	~CUi();						// デストラクタ

	static CUi *Create(int nIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_IdxTexture;			// 使用するテクスチャの番号
	bool m_bMove;								// テクスチャ座標を動かすか
	float m_fTexU;								// テクスチャ座標の開始位置（U値）
	float m_fTexV;								// テクスチャ座標の開始位置（V値）
};

#endif