//=========================================================
//
// ランキング処理 [ranking.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "input.h"
#include "texture.h"
#include "rankIn.h"
#include "result.h"
#include "corecrt_wstdio.h"
#include <stdio.h>

//===============================================
// 静的メンバ変数
//===============================================
CNumber *CRanking::m_apNumber[MAX_RANK][MAX_NUMBER] = {};		// ナンバークラスのポインタ
int CRanking::m_nIdxTexture = 0;								// 使用するテクスチャの番号

//===============================================
// マクロ定義
//===============================================
#define NUM_PLACE			(8)				// スコアの桁数
#define SCORE_SIZEX			(27)			// スコアの幅（半分）
#define SCORE_SIZEY			(60)			// スコアの高さ（半分）
#define DISTANCE_SCOREX		(50.0f)			// スコア間の幅
#define DISTANCE_SCOREY		(1.65f)			// スコア間の高さの倍率

#define MAX_SCORE		(99999999)		// 最大スコア

//===============================================
// コンストラクタ
//===============================================
CRanking::CRanking()
{
	// 値のクリア
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_aScore[nCntRank] = 0;
	}
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_nYourRank = 0;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CRanking::CRanking(int nPriority)
{
	// 値のクリア
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_aScore[nCntRank] = 0;
	}
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_nYourRank = 0;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// デストラクタ
//===============================================
CRanking::~CRanking()
{

}

//===============================================
// 生成処理
//===============================================
CRanking *CRanking::Create(int nPriority)
{
	CRanking *pScore;

	// スコアの生成
	pScore = new CRanking(nPriority);

	// 初期化処理
	pScore->Init(D3DXVECTOR3(350.0f, 140.0f, 0.0f), SCORE_SIZEX, SCORE_SIZEY, nPriority);

	return pScore;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] == NULL)
			{// 使用されていない
				// 生成
				m_apNumber[nCntRank][nCntObj] = new CNumber(nPriority);

				// 種類の設定
				m_apNumber[nCntRank][nCntObj]->SetType(CNumber::TYPE_SCORE);

				// 初期化
				m_apNumber[nCntRank][nCntObj]->Init(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2 + nCntRank * DISTANCE_SCOREX, pos.y + nCntRank * fSizeY * DISTANCE_SCOREY, pos.z), fSizeX, fSizeY, nPriority);
				m_apNumber[nCntRank][nCntObj]->SetPos(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2 + nCntRank * DISTANCE_SCOREX, pos.y + nCntRank * fSizeY * DISTANCE_SCOREY, pos.z), fSizeX, fSizeY);

				// テクスチャの設定
				m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\score000.png");

				// テクスチャの割り当て
				m_apNumber[nCntRank][nCntObj]->BindTexture(m_nIdxTexture);
			}
		}
	}

	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data\\TXT\\ranking.txt", "r");

	if (pFile != NULL)
	{
		for (int nCntRank = 0; nCntRank < YOUR_SCORE; nCntRank++)
		{
			// ファイルから文字列を読み込む
			fscanf(pFile, "%d", &m_aScore[nCntRank]);
		}
		// ファイルを閉じる
		fclose(pFile);
	}

	// サイズを設定
	m_fSizeX = SCORE_SIZEX;
	m_fSizeY = SCORE_SIZEY;

	// 値を初期化
	m_nYourRank = - 1;

	// スコア初期設定
	Set();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRanking::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] != NULL)
			{// 使用されている
				// ナンバーの終了処理
				m_apNumber[nCntRank][nCntObj]->Uninit();
				delete m_apNumber[nCntRank][nCntObj];
				m_apNumber[nCntRank][nCntObj] = NULL;
			}
		}
	}
}

//===============================================
// 更新処理
//===============================================
void CRanking::Update(void)
{
	if (m_fBrightness <= MIN_COLA)
	{// ある程度暗くなった
		m_bFlash = false;		// 明るくする
	}
	else if (m_fBrightness >= MAX_COLA)
	{// ある程度明るくなった
		m_bFlash = true;		// 暗くする
	}

	if (m_bFlash == true)
	{// 暗くする
		m_fBrightness += COL_DARK;
	}
	else if (m_bFlash == false)
	{// 明るくする
		m_fBrightness += COL_LIGHT;
	}

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] != NULL)
			{// 使用されている
				// ナンバーの更新処理
				m_apNumber[nCntRank][nCntObj]->Update();

				if (nCntRank == YOUR_SCORE || nCntRank == m_nYourRank)
				{// 今回のスコア
					// 色の設定
					m_apNumber[nCntRank][nCntObj]->SetCol(D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, 1.0f));
				}

			}
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CRanking::Draw(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] != NULL)
			{// 使用されている
				// ナンバーの描画処理
				m_apNumber[nCntRank][nCntObj]->Draw();
			}
		}
	}
}

//===============================================
// 設定処理
//===============================================
void CRanking::Set(void)
{
	FILE *pFile;
	int nTemp;							// 置き換え用の変数
	int aTexU[NUM_PLACE];				// 各行の数字を格納
	int nScore = m_aScore[YOUR_SCORE];	// 今回のスコア

	// 処理を繰り返す
	for (int nCntRank = 0; nCntRank < YOUR_SCORE; nCntRank++)
	{
		// 数値を比較する
		if (nScore >= m_aScore[nCntRank])
		{// トップ5にランクインした
			nTemp = m_aScore[nCntRank];
			m_aScore[nCntRank] = nScore;

			if (m_nYourRank == - 1)
			{// ランクインの設定をしていない
				CResult::GetRankIn()->Set(nCntRank);	// 更新ランクNo.を更新
				m_nYourRank = nCntRank;
			}

			// 処理を繰り返す
			for (int nCount = nCntRank + 1; nCount < YOUR_SCORE; nCount++)
			{
				nScore = nTemp;
				nTemp = m_aScore[nCount];
				m_aScore[nCount] = nScore;
			}
		}
	}

	// ファイルを開く
	pFile = fopen("data\\TXT\\ranking.txt", "w");

	if (pFile != NULL)
	{
		for (int nCntRank = 0; nCntRank < YOUR_SCORE; nCntRank++)
		{
			// ファイルから文字列を読み込む
			fprintf(pFile, "%d\n", m_aScore[nCntRank]);
		}

		// ファイルを閉じる
		fclose(pFile);
	}

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		aTexU[0] = m_aScore[nCntRank] % 100000000 / 10000000;
		aTexU[1] = m_aScore[nCntRank] % 10000000 / 1000000;
		aTexU[2] = m_aScore[nCntRank] % 1000000 / 100000;
		aTexU[3] = m_aScore[nCntRank] % 100000 / 10000;
		aTexU[4] = m_aScore[nCntRank] % 10000 / 1000;
		aTexU[5] = m_aScore[nCntRank] % 1000 / 100;
		aTexU[6] = m_aScore[nCntRank] % 100 / 10;
		aTexU[7] = m_aScore[nCntRank] % 10;

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			if (m_apNumber[nCntScore] != NULL)
			{// 使用されている
				// ナンバー設定
				m_apNumber[nCntRank][nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}

//===============================================
// 加算処理
//===============================================
void CRanking::Add(const int nScore)
{
	int aTexU[NUM_PLACE];  // 各行の数字を格納

	m_aScore[YOUR_SCORE] = nScore;

	if (m_aScore[YOUR_SCORE] > MAX_SCORE)
	{// 最大スコアを超えた
		m_aScore[YOUR_SCORE] = MAX_SCORE;
	}
	else if (m_aScore[YOUR_SCORE] < 0)
	{// スコアが０を下回った
		m_aScore[YOUR_SCORE] = 0;
	}

	aTexU[0] = m_aScore[YOUR_SCORE] % 100000000 / 10000000;
	aTexU[1] = m_aScore[YOUR_SCORE] % 10000000 / 1000000;
	aTexU[2] = m_aScore[YOUR_SCORE] % 1000000 / 100000;
	aTexU[3] = m_aScore[YOUR_SCORE] % 100000 / 10000;
	aTexU[4] = m_aScore[YOUR_SCORE] % 10000 / 1000;
	aTexU[5] = m_aScore[YOUR_SCORE] % 1000 / 100;
	aTexU[6] = m_aScore[YOUR_SCORE] % 100 / 10;
	aTexU[7] = m_aScore[YOUR_SCORE] % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[YOUR_SCORE][nCntScore] != NULL)
		{// 使用されている
			// ナンバー設定
			m_apNumber[YOUR_SCORE][nCntScore]->Set(aTexU[nCntScore]);
		}
	}

	// 設定処理
	Set();
}