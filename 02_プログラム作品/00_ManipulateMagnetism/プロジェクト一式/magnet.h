//=========================================================
//
// 磁石ブロック処理 [magnet.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MAGNET_H_  // このマクロ定義がされてなかったら
#define _MAGNET_H_  // ２重インクルード防止のマクロを定義する

#include "objectX.h"
#include "xfile.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_NAME	(256)	// 最大文字数

//===============================================
// 磁石ブロッククラス
//===============================================
class CMagnet : public CObjectX
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CMagnet();						// デフォルトコンストラクタ
	CMagnet(int nPriority = 3);	// オーバーロードされたコンストラクタ
	virtual ~CMagnet();			// デストラクタ

	enum STATE
	{
		STATE_NONE = 0,	// 無し
		STATE_N,
		STATE_S,
		STATE_MAX
	};

	static CMagnet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawXFile(int nIdx, CXFile::COL col);
	static bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	static bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

protected:	// 派生クラスからもアクセスできる

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_aIdxXFile[MODEL_MAX];			// 使用するXファイルの番号
	
	STATE m_state;								// 磁石の極状態
	D3DXVECTOR3 m_vtxMax;						// モデルの最大値
	D3DXVECTOR3 m_vtxMin;						// モデルの最小値
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nIdxShadow;							// 使用する影の番号
};

#endif