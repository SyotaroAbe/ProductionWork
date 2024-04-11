//=========================================================
//
// 操作方法表示処理 [uioperation.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _UIOPERATION_H_     // このマクロ定義がされてなかったら
#define _UIOPERATION_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// マクロ定義
//===============================================

//===============================================
// 操作方法表示クラス
//===============================================
class CUiOperation : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CUiOperation();						// デフォルトコンストラクタ
	CUiOperation(int nPriority = 5);	// オーバーロードされたコンストラクタ
	~CUiOperation();					// デストラクタ

	// 手順
	enum TEX
	{
		TEX_NOT_OPERATION = 0,	// 移動操作できない
		TEX_COLLECT,			// ターゲットを重ねる
		TEX_TARGET,				// ターゲット移動
		TEX_MATCHCOL,			// 色を揃えよう
		TEX_BURN,				// 燃えるゴミ
		TEX_NONFLAMMABLE,		// 燃えないゴミ
		TEX_RECYCLABLE,			// 資源ゴミ
		TEX_GAGE,				// ゲージ
		TEX_DUMPSTER,			// ゴミステーション
		TEX_END,				// 終了確認
		TEX_MAX
	};

	// ボタン
	enum BUTTON
	{
		BUTTON_W = 0,	// W
		BUTTON_A,		// A
		BUTTON_S,		// S
		BUTTON_D,		// D
		BUTTON_MAX
	};

	static CUiOperation *Create(int nTex, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, int nTex, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nType);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static const char *m_apTextureData[TEX_MAX];	// テクスチャファイル名
	static int m_aIdxTexture[TEX_MAX];				// 使用するテクスチャの番号
	static int m_nProgress;							// 進捗
	static int m_nTime;								// 経過時間

	int m_nTypeTex;									// テクスチャの種類
	bool m_bPressInput[BUTTON_MAX];					// ボタンを押したか
	int m_nGarbage;								// ゴミの量
};

#endif