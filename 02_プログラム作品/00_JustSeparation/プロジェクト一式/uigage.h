//=========================================================
//
// ゴミゲージ処理 [uigage.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _UIGAGE_H_     // このマクロ定義がされてなかったら
#define _UIGAGE_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"
#include "garbage.h"

//===============================================
// 前方宣言
//===============================================
class CObject2D;

//===============================================
// マクロ定義
//===============================================
#define AMOUNT_OF_RISE_NORMAL	(3)				// ゲージ上昇量（通常）
#define AMOUNT_OF_RISE_BIG		(5)				// ゲージ上昇量（大きい）
#define AMOUNT_OF_RISE_SMALL	(1)				// ゲージ上昇量（小さい）
#define MAX_GARBAGE				(20)				// 最大ゴミ量

//===============================================
// ゴミゲージクラス
//===============================================
class CUiGage
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CUiGage();						// デフォルトコンストラクタ
	CUiGage(int nPriority = 5);		// オーバーロードされたコンストラクタ
	~CUiGage();						// デストラクタ

	enum TEX
	{
		TEX_OUTSIDE = 0,	// 外側
		TEX_INSIDE,			// 内側
		TEX_MAX
	};

	static CUiGage *Create(int nIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Vibration(float fMove, float fMaxMove);
	void Set(int fGarbage);
	int Get(void) { return m_nGarbage; }
	bool GetMax(CGarbage::MODELSIZE size = CGarbage::MODELSIZE_MAX);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CObject2D *m_apObject2D[TEX_MAX];		// オブジェクト2Dクラスのポインタ
	static const char *m_apTextureData[TEX_MAX];	// テクスチャファイル名
	static int m_aIdxTexture[TEX_MAX];				// 使用するテクスチャの番号

	float m_fTexU;									// テクスチャ座標の開始位置（U値）
	float m_fTexV;									// テクスチャ座標の開始位置（V値）
	int m_nGarbage;									// ゴミの量
	float m_fMoveShake;								// 震える動きの移動量
	bool m_bVMoveShake;								// 震える動きの向き
};

#endif