//=========================================================
//
// ゲーム画面処理 [game.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _GAME_H_     // このマクロ定義がされてなかったら
#define _GAME_H_     // 2重インクルード防止のマクロ定義する

#include "manager.h"
#include "dumpster.h"

//===============================================
// マクロ定義
//===============================================
#define TIME_STATEREADY		(120)	// 待機状態へ画面が切り替わるまでの余韻
#define TIME_STATEFEVER		(5400)	// フィーバー状態へ画面が切り替わるまでの時間

//===============================================
// 前方宣言
//===============================================
class CPlayer;
class CNumber;
class CTime;
class CScore;
class CObjectX;
class CGarbage;
class CMeshField;
class CObject3D;
class CTarget;
class CDumpster;
class CPause;
class CUi;
class CUiGage;
class CUiTarget;
class CJustDust;
class CFever;
class CHint;
class CGageFull;

//===============================================
// ゲーム画面クラス
//===============================================
class CGame : public CScene
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CGame();						// デフォルトコンストラクタ
	~CGame();						// デストラクタ

	// ゲームの状態
	enum STATE
	{
		STATE_NONE = 0,	// 何もしていない状態
		STATE_READY,	// 開始待機状態
		STATE_NORMAL,	// 通常状態
		STATE_FEVER,	// フィーバー状態
		STATE_END,		// 終了状態
		STAET_MAX
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *GetPlayer() { return m_pPlayer; }
	static CNumber *GetNumber() { return m_pNumber; }
	static CTime *GetTime() { return m_pTime; }
	static CScore *GetScore() { return m_pScore; }
	static CGarbage *GetGarbage() { return m_pGarbage; }
	static CMeshField *GetMeshField() { return m_pMeshField; }
	static CObject3D *GetObject3D() { return m_pObject3D; }
	static CTarget *GetTarget() { return m_pTarget; }
	static CDumpster *GetDumpster(int nIdx) { return m_pDumpster[nIdx]; }
	static CPause *GetPause() { return m_pPause; }
	static CUi *GetUi() { return m_pUi; }
	static CUiGage *GetUiGage() { return m_pUiGage; }
	static CUiTarget *GetUiTarget() { return m_pUiTarget; }
	static CJustDust *GetJustDust() { return m_pJustDust; }
	static CFever *GetFever() { return m_pFever; }
	static CHint *GetHint() { return m_pHint; }
	static CGageFull *GetGageFull() { return m_pGageFull; }

	static void SetEnablePause(const bool bPause);
	static bool GetPauseState(void) { return m_bPause; }
	static bool GetStateReady(void) { return m_bStateReady; }
	static bool GetPauseCamera(void) { return m_bPauseCamera; }

	STATE GetState(void) { return m_state; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CPlayer *m_pPlayer;						// プレイヤークラスのポインタ
	static CNumber *m_pNumber;						// ナンバークラスのポインタ
	static CTime *m_pTime;							// タイムクラスのポインタ
	static CScore *m_pScore;						// スコアクラスのポインタ
	static CGarbage *m_pGarbage;					// ゴミクラスのポインタ
	static CMeshField *m_pMeshField;				// メッシュフィールドクラスのポインタ
	static CObject3D *m_pObject3D;					// オブジェクト3Dクラスのポインタ
	static CTarget *m_pTarget;						// ターゲットクラスのポインタ
	static CDumpster *m_pDumpster[MAX_DUMPSTER];	// ゴミステーションクラスのポインタ
	static CPause *m_pPause;						// ポーズクラスのポインタ
	static CUi *m_pUi;								// UIクラスのポインタ
	static CUiGage *m_pUiGage;						// ゴミゲージクラスのポインタ
	static CUiTarget *m_pUiTarget;					// ターゲットUIクラスのポインタ
	static CJustDust *m_pJustDust;					// JustDust表示クラスのポインタ
	static CFever *m_pFever;						// Fever表示クラスのポインタ
	static CHint *m_pHint;							// ヒント表示クラスのポインタ
	static CGageFull *m_pGageFull;					// ゲージフル表示クラスのポインタ

	static bool m_bStateReady;		// GAMSESTATE_READYかどうか
	static bool m_bPause;			// 現在のポーズの状態
	static bool m_bPauseCamera;		// ポーズ時のカメラ操作可能か
	STATE m_state;					// ゲームの状態
	int m_nCounterState;			// 状態管理カウンター
	int m_nTimeFever;				// フィーバー状態のタイム
};

#endif