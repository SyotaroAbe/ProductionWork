//=========================================================
//
// リザルト画面処理 [result.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "result.h"
#include "bg.h"
#include "input.h"
#include "renderer.h"
#include "fade.h"
#include "manager.h"
#include "ranking.h"
#include "rankIn.h"

//===============================================
// マクロ定義
//===============================================
#define TIME_FADE		(600)			// 自動フェード時間

//===============================================
// 静的メンバ変数
//===============================================
CBg *CResult::m_pBg = nullptr;						// 背景クラスのポインタ
CRanking *CResult::m_pRanking = nullptr;			// ランキングクラスのポインタ
CRankIn *CResult::m_pRankIn = nullptr;				// ランクインクラスのポインタ

//===============================================
// コンストラクタ
//===============================================
CResult::CResult() : CScene()
{
	// 値のクリア
	m_nTimeFade = 0;
	m_bFade = false;
}

//===============================================
// デストラクタ
//===============================================
CResult::~CResult()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CResult::Init(HWND hWnd)
{
	// 背景の生成
	m_pBg = CBg::Create(CBg::TYPE_RESULT);

	// ランキングの生成
	m_pRanking = CRanking::Create(6);

	// ランクインの生成
	m_pRankIn = CRankIn::Create();
	
	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CResult::Uninit(void)
{
	if (m_pRanking != nullptr)
	{
		// ランキングの終了処理
		m_pRanking->Uninit();
		delete m_pRanking;
		m_pRanking = nullptr;
	}

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//===============================================
// 更新処理
//===============================================
void CResult::Update(void)
{
	m_nTimeFade++;		// 時間をカウント

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true
		|| m_nTimeFade > TIME_FADE)
	{// ENTER入力か一定時間経過
		if (m_bFade == false)
		{// フェードバグ防止
			CRenderer::GetFade()->Set(CScene::MODE_TITLE);	// ゲームタイトル画面へ移行

			m_bFade = true;
		}
	}

	if (m_pRanking != nullptr)
	{
		// ランキングの更新処理
		m_pRanking->Update();
	}
}

//===============================================
// 描画処理
//===============================================
void CResult::Draw(void)
{
	if (m_pRanking != nullptr)
	{
		// ランキングの描画処理
		m_pRanking->Draw();
	}
}