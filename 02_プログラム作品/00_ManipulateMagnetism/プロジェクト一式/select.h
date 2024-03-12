//=========================================================
//
// セレクト画面処理 [select.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _SELECT_H_     // このマクロ定義がされてなかったら
#define _SELECT_H_     // 2重インクルード防止のマクロ定義する

#include "manager.h"

//===============================================
// 前方宣言
//===============================================
class CBg;
class CObject2D;

//===============================================
// 前方宣言
//===============================================

//===============================================
// セレクト画面クラス
//===============================================
class CSelect : public CScene
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CSelect();						// デフォルトコンストラクタ
	~CSelect();						// デストラクタ

	// ステージ
	enum STAGE
	{
		STAGE_TUTORIAL = 0,		// チュートリアル
		STAGE_1,				// ステージ１
		STAGE_2,				// ステージ２
		STAGE_3,				// ステージ３
		STAGE_MAX
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg *GetBg() { return m_pBg; }
	static CObject2D* GetObject2D() { return m_pObject2D; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CBg *m_pBg;						// 背景クラスのポインタ
	static CObject2D* m_pObject2D;			// オブジェクト2Dクラスのポインタ

	CObject2D* m_pTutorial;		// チュートリアル
	CObject2D* m_pFirstStage;	// ステージ１
	CObject2D* m_pSecondStage;	// ステージ２
	CObject2D* m_pThirdStage;	// ステージ３
	int m_nTime;				// 経過時間をカウント
	int m_nSelectStage;			// 現在選択中のステージ
	bool m_bFade;				// フェードに入ったか
};

#endif