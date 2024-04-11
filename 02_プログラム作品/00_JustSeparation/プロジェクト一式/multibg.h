//=========================================================
//
// 多重の背景処理 [multibg.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MULTIBG_H_     // このマクロ定義がされてなかったら
#define _MULTIBG_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"

//===============================================
// 前方宣言
//===============================================
class CObject2D;

//===============================================
// マクロ定義
//===============================================
#define NUM_BG		(3)				// 背景の数

//===============================================
// 多重の背景クラス
//===============================================
class CMultiBg : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CMultiBg();		// デフォルトコンストラクタ
	~CMultiBg();	// デストラクタ

	static CMultiBg *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const float TexU, const float TexV);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_pos; }
	void SetMove(const D3DXVECTOR3 move);
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f); }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CObject2D *m_apObject2D[MAX_OBJECT];			// オブジェクト2Dクラスのポインタ
	static int m_aIdxTexture[NUM_BG];					// 使用するテクスチャの番号
	static const char *m_apTextureData[NUM_BG];			// テクスチャファイル名
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_rot;									// 向き
	float m_fTexU;										// テクスチャ座標の開始位置（U値）
	float m_fTexV;										// テクスチャ座標の開始位置（V値）
};

#endif