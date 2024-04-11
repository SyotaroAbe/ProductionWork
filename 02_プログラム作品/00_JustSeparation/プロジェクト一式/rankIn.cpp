//=========================================================
//
// ランクイン表示 [rankIn.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "rankIn.h"
#include "manager.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define RANKIN_SIZEX	(90)		// ランクインの幅（半分）
#define RANKIN_SIZEY	(30)		// ランクインの高さ（半分）
#define RANKIN_DISTANCE	(50.0f)		// ランク別にずらす位置

//===============================================
// 静的メンバ変数
//===============================================
int CRankIn::m_pIdxTexture = 0;				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CRankIn::CRankIn() : CObject2D(0)
{
	// 値のクリア
	m_fBrightness = 0.0f;
	m_bFlash = false;
	m_nRank = 0;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CRankIn::CRankIn(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_fBrightness = 0.0f;
	m_bFlash = false;
	m_nRank = 0;
}

//===============================================
// デストラクタ
//===============================================
CRankIn::~CRankIn()
{

}

//===============================================
// 生成処理
//===============================================
CRankIn *CRankIn::Create(int nPriority)
{
	CRankIn *pRankIn;

	// ランクインの生成
	pRankIn = new CRankIn(nPriority);

	// 種類の設定
	pRankIn->SetType(TYPE_RANKIN);

	// サイズの設定
	pRankIn->SetSize(RANKIN_SIZEX, RANKIN_SIZEY);

	// 初期化処理
	pRankIn->Init(D3DXVECTOR3(120.0f, 130.0f, 0.0f));

	// 色の設定
	pRankIn->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// テクスチャの割り当て
	pRankIn->BindTexture(m_pIdxTexture);

	return pRankIn;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CRankIn::Init(D3DXVECTOR3 pos)
{
	// 値の初期化
	m_nRank = -1;

	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_pIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\rankIn.png");

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRankIn::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CRankIn::Update(void)
{
	if (m_nRank > - 1)
	{// ランクインした
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
	}

	// オブジェクト2Dの更新処理
	CObject2D::Update();

	// 色の設定
	CObject2D::SetCol(D3DXCOLOR(m_fBrightness, m_fBrightness, 0.3f, 1.0f));
}

//===============================================
// 描画処理
//===============================================
void CRankIn::Draw(void)
{
	if (m_nRank > - 1)
	{// ランクインした
		// オブジェクト2Dの描画処理
		CObject2D::Draw();
	}
}

//===============================================
// 設定処理
//===============================================
void CRankIn::Set(const int nRank)
{
	m_nRank = nRank;	// ランクから位置を設定

	// 位置の設定処理
	CObject2D::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + RANKIN_DISTANCE * (m_nRank * 2), 0.0f));
}