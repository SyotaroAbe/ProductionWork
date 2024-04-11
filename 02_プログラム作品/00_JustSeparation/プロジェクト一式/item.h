//=========================================================
//
// アイテム処理 [item.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _ITEM_H_     // このマクロ定義がされてなかったら
#define _ITEM_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// アイテムクラス
//===============================================
class CItem : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CItem();		// デフォルトコンストラクタ
	~CItem();		// デストラクタ

	static CItem *Create(D3DXVECTOR3 pos, int nPriority = 3);

	// アイテムの種類
	typedef enum
	{
		TYPE_TREASURE = 0,	// 宝箱
		TYPE_COIN,			// コイン
		TYPE_MAX
	}TYPE;

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const TYPE type, const D3DXVECTOR3 pos);

	bool CollisionEnemy(D3DXVECTOR3 pos);

	static int GetNumAll(void) { return m_nNumAll; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_aIdxTexture[TYPE_MAX];						// 使用するテクスチャの番号
	static const char *m_apTextureData[TYPE_MAX];			// テクスチャファイル名
	static int m_nNumAll;									// アイテムの総数
	TYPE m_type;											// 種類
};

#endif