//=========================================================
//
// エディット処理 [editor.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _EDITOR_H_  // このマクロ定義がされてなかったら
#define _EDITOR_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"
#include "objectX.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_MODELEDIT		(256)		// 最大設置モデル

//===============================================
// エディットクラス
//===============================================
class CEdit
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CEdit();						// デフォルトコンストラクタ
	~CEdit();						// デストラクタ;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Get(void) { return m_bEdit; }
	void Set(bool bEdit);

private:	// 自分のみアクセス可能 [アクセス指定子]
	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_rot;										// 向き
	int m_nType;											// ゴミの種類
	int m_nID;												// 現在のオブジェクト
	bool m_bEdit;											// エディット切り替え
	static CObjectX *m_apObjectX[MAX_MODELEDIT];			// オブジェクトXクラスのポインタ
};

#endif