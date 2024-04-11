//=========================================================
//
// ヒント描画処理 [hint.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "hint.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//===============================================
// マクロ定義
//===============================================
#define TEXTURE			"data\\TEXTURE\\hint.png"	// ヒントのテクスチャ
#define SIZE_X			(285.0f)					// 横サイズ
#define SIZE_Y			(50.0f)						// 縦サイズ
#define ARRIVAL_TIME	(1200)						// 出現するまでの時間

//===============================================
// 静的メンバ変数
//===============================================
int CHint::m_IdxTexture = 0;				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CHint::CHint() : CObject2D(0)
{
	// 値のクリア
	m_nTime = 0;
	m_bDisp = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CHint::CHint(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nTime = 0;
	m_bDisp = false;
}

//===============================================
// デストラクタ
//===============================================
CHint::~CHint()
{
	
}

//===============================================
// 生成処理
//===============================================
CHint *CHint::Create(int nPriority)
{
	CHint *pHint;

	// ヒントの生成
	pHint = new CHint(nPriority);

	// 種類の設定
	pHint->SetType(TYPE_BG);

	// サイズの設定
	pHint->SetSize(SIZE_X, SIZE_Y);

	// 初期化処理
	pHint->Init(D3DXVECTOR3(620.0f, 635.0f, 0.0f));

	// テクスチャの割り当て
	pHint->BindTexture(m_IdxTexture);

	return pHint;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CHint::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_IdxTexture = CManager::GetTexture()->Regist(TEXTURE);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CHint::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CHint::Update(void)
{
	m_nTime++;		// 時間をカウント

	if (CManager::GetKeyboardInput()->GetPress(DIK_Q) == true || CManager::GetKeyboardInput()->GetPress(DIK_Z) == true 
		|| CManager::GetKeyboardInput()->GetPress(DIK_E) == true || CManager::GetKeyboardInput()->GetPress(DIK_C) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RB, 0))
	{// 曲がる入力
		m_nTime = 0;		// 時間をリセット
	}
}

//===============================================
// 描画処理
//===============================================
void CHint::Draw(void)
{
	if (m_nTime >= ARRIVAL_TIME)
	{
		// オブジェクト2Dの描画処理
		CObject2D::Draw();
	}
}