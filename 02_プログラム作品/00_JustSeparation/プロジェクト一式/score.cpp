//=========================================================
//
// スコア処理 [score.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "input.h"
#include "texture.h"

//===============================================
// 静的メンバ変数
//===============================================
CNumber *CScore::m_apNumber[MAX_NUMBER] = {};		// ナンバークラスのポインタ
int CScore::m_nIdxTexture = 0;						// 使用するテクスチャの番号
int CScore::m_nScore = 0;							// スコアの値

//===============================================
// マクロ定義
//===============================================
#define NUM_PLACE		(8)				// スコアの桁数
#define SCORE_SIZEX		(18)			// スコアの幅（半分）
#define SCORE_SIZEY		(40)			// スコアの高さ（半分）

#define MAX_SCORE		(99999999)		// 最大スコア

//===============================================
// コンストラクタ
//===============================================
CScore::CScore()
{

}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CScore::CScore(int nPriority)
{
	
}

//===============================================
// デストラクタ
//===============================================
CScore::~CScore()
{

}

//===============================================
// 生成処理
//===============================================
CScore *CScore::Create(int nPriority)
{
	CScore *pScore;

	// スコアの生成
	pScore = new CScore(nPriority);

	// 初期化処理
	pScore->Init(D3DXVECTOR3(900.0f, 50.0f, 0.0f), SCORE_SIZEX, SCORE_SIZEY, nPriority);

	return pScore;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority)
{
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] == NULL)
		{// 使用されていない
			// 生成
			m_apNumber[nCntObj] = new CNumber(nPriority);

			// 種類の設定
			m_apNumber[nCntObj]->SetType(CNumber::TYPE_SCORE);

			// 初期化
			m_apNumber[nCntObj]->Init(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2, pos.y, pos.z), fSizeX, fSizeY, nPriority);
			m_apNumber[nCntObj]->SetPos(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2, pos.y, pos.z), fSizeX, fSizeY);

			// テクスチャの設定
			m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\score000.png");

			// テクスチャの割り当て
			m_apNumber[nCntObj]->BindTexture(m_nIdxTexture);
		}
	}

	// サイズを設定
	m_fSizeX = SCORE_SIZEX;
	m_fSizeY = SCORE_SIZEY;

	// スコア初期設定
	Set(0);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CScore::Uninit(void)
{
	// ナンバーの終了処理
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{// 使用されている
			m_apNumber[nCntObj]->Uninit();
			delete m_apNumber[nCntObj];
			m_apNumber[nCntObj] = NULL;
		}
	}
}

//===============================================
// 更新処理
//===============================================
void CScore::Update(void)
{
	// ナンバーの更新処理
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{// 使用されている
			m_apNumber[nCntObj]->Update();
		}
	}

#ifdef _DEBUG
	if (CManager::GetKeyboardInput()->GetPress(DIK_U) == true)
	{// Uキー
		Add(100000);
	}
#endif
}

//===============================================
// 描画処理
//===============================================
void CScore::Draw(void)
{
	// ナンバーの描画処理
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{// 使用されている
			m_apNumber[nCntObj]->Draw();
		}
	}
}

//===============================================
// 設定処理
//===============================================
void CScore::Set(const int nScore)
{
	int aTexU[NUM_PLACE];  // 各行の数字を格納

	m_nScore = nScore;

	aTexU[0] = m_nScore % 100000000 / 10000000;
	aTexU[1] = m_nScore % 10000000 / 1000000;
	aTexU[2] = m_nScore % 1000000 / 100000;
	aTexU[3] = m_nScore % 100000 / 10000;
	aTexU[4] = m_nScore % 10000 / 1000;
	aTexU[5] = m_nScore % 1000 / 100;
	aTexU[6] = m_nScore % 100 / 10;
	aTexU[7] = m_nScore % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// 使用されている
			CNumber::TYPE type = m_apNumber[nCntScore]->GetType();

			if (type == CNumber::TYPE_SCORE)
			{// スコアの場合
				// ナンバー設定
				m_apNumber[nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}

//===============================================
// 加算処理
//===============================================
void CScore::Add(const int nScore)
{
	int aTexU[NUM_PLACE];  // 各行の数字を格納

	m_nScore += nScore;

	if (m_nScore > MAX_SCORE)
	{// 最大スコアを超えた
		m_nScore = MAX_SCORE;
	}
	else if (m_nScore < 0)
	{// スコアが０を下回った
		m_nScore = 0;
	}

	aTexU[0] = m_nScore % 100000000 / 10000000;
	aTexU[1] = m_nScore % 10000000 / 1000000;
	aTexU[2] = m_nScore % 1000000 / 100000;
	aTexU[3] = m_nScore % 100000 / 10000;
	aTexU[4] = m_nScore % 10000 / 1000;
	aTexU[5] = m_nScore % 1000 / 100;
	aTexU[6] = m_nScore % 100 / 10;
	aTexU[7] = m_nScore % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// 使用されている
			CNumber::TYPE type = m_apNumber[nCntScore]->GetType();

			if (type == CNumber::TYPE_SCORE)
			{// スコアの場合
				// ナンバー設定
				m_apNumber[nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}