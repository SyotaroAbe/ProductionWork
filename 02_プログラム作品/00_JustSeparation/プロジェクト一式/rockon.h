//=========================================================
//
// ロックオン処理 [rockon.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _ROCKON_H_     // このマクロ定義がされてなかったら
#define _ROCKON_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"
#include "objectBillboard.h"
#include "garbage.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_ROCKON		(256)		// ターゲットの最大数

//===============================================
// ロックオンクラス
//===============================================
class CRockon : public CObjectBillboard
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CRockon();						// デフォルトコンストラクタ
	CRockon(int nPriority = 2);		// オーバーロードされたコンストラクタ
	~CRockon();						// デストラクタ

	static CRockon *Create(D3DXVECTOR3 pos, CGarbage::SEPARATION separation, int nID, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	void SetSeparation(CGarbage::SEPARATION separation);
	void SetIDObj(int nID);
	static int GetNumAll(void) { return m_nNumAll; }
	static void SetObjRockon(int nCntRockon, int nCntObj);
	static int GetObjRockon(int nCntRockon) { return m_aObjRockon[nCntRockon]; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;				// 使用するテクスチャの番号
	static int m_nNumAll;					// 使用しているターゲット総数
	static int m_aObjRockon[MAX_ROCKON];	// ロックオンしているオブジェクトの番号保存

	D3DXVECTOR3 m_localPos;							// 初期位置から見ての距離
	CGarbage::SEPARATION m_separation;				// ターゲットの状態
	int m_nIDGarbage;								// ゴミの番号
};

#endif