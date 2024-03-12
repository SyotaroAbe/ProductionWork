//=========================================================
//
// アイテム処理 [item.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _ITEM_H_  // このマクロ定義がされてなかったら
#define _ITEM_H_  // ２重インクルード防止のマクロを定義する

#include "objectX.h"
#include "xfile.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_NAME	(256)	// 最大文字数

//===============================================
// アイテムクラス
//===============================================
class CItem : public CObjectX
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CItem();						// デフォルトコンストラクタ
	CItem(int nPriority = 3);	// オーバーロードされたコンストラクタ
	virtual ~CItem();			// デストラクタ

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawXFile(int nIdx, CXFile::COL col);
	static bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	static bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

	void SetMagnet(const bool bMagnet) { m_bMagnet = bMagnet; }

protected:	// 派生クラスからもアクセスできる

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_aIdxXFile[MODEL_MAX];			// 使用するXファイルの番号
	
	D3DXVECTOR3 m_vtxMax;						// モデルの最大値
	D3DXVECTOR3 m_vtxMin;						// モデルの最小値
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nIdxShadow;							// 使用する影の番号
	bool m_bMagnet;								// プレイヤーとくっついているか
};

#endif