//=========================================================
//
// ターゲットUI処理 [uitarget.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _UITARGET_H_     // このマクロ定義がされてなかったら
#define _UITARGET_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"

//===============================================
// 前方宣言
//===============================================
class CObjectBillboard;

//===============================================
// マクロ定義
//===============================================
//#define NUM_TEX		(3)				// テクスチャの数

//===============================================
// ターゲットUIクラス
//===============================================
class CUiTarget
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CUiTarget();						// デフォルトコンストラクタ
	CUiTarget(int nPriority = 5);		// オーバーロードされたコンストラクタ
	~CUiTarget();						// デストラクタ

	enum TYPE
	{
		TYPE_BURN,			// 燃えるゴミ
		TYPE_NONFLAMMABLE,	// 燃えないゴミ
		TYPE_RECYCLABLE,		// 資源ゴミ
		TYPE_MAX
	};

	static CUiTarget *Create(int nIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CObjectBillboard *m_apObjectBillboard[TYPE_MAX];		// オブジェクトビルボードクラスのポインタ
	static const char *m_apTextureData[TYPE_MAX];				// テクスチャファイル名
	static int m_aIdxTexture[TYPE_MAX];							// 使用するテクスチャの番号
	D3DXVECTOR3 m_aPos[TYPE_MAX];								// 合計位置
};

#endif