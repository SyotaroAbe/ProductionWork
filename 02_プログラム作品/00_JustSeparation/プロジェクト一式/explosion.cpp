//=========================================================
//
// 爆発処理 [explosion.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"

//===============================================
// 静的メンバ変数
//===============================================
int CExplosion::m_nIdxTexture = 0;						// 使用するテクスチャの番号

//===============================================
// マクロ定義
//===============================================
#define EXPLOSION_SIZEX		(25)								// 幅（半分）
#define EXPLOSION_SIZEY		(25)								// 高さ（半分）

#define PATTERNANIM_X		(8)									// アニメ横パターン数
#define PATTERNANIM_Y		(1)									// アニメ縦パターン数
#define PATTERNANIM_ALL		(PATTERNANIM_X * PATTERNANIM_Y)		// アニメ縦パターン数
#define SPEED_ANIM			(10)								// アニメーションスピード

//===============================================
// コンストラクタ
//===============================================
CExplosion::CExplosion() : CObject2D(3)
{
	// 値をクリアする
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CExplosion::CExplosion(int nPriority) : CObject2D(nPriority)
{
	// 値をクリアする
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// デストラクタ
//===============================================
CExplosion::~CExplosion()
{

}

//===============================================
// 生成処理
//===============================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, int nPriority)
{
	CExplosion *pExplosion;

	// 爆発の生成
	pExplosion = new CExplosion(nPriority);

	// 種類の設定
	pExplosion->SetType(TYPE_EXPLOSION);

	// 設定のサイズ
	pExplosion->SetSize(EXPLOSION_SIZEX, EXPLOSION_SIZEY);

	// 初期化処理
	pExplosion->Init(pos);

	// テクスチャの割り当て
	pExplosion->BindTexture(m_nIdxTexture);

	return pExplosion;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\enemy000.png");

	// アニメーションカウンターの設定
	m_nCounterAnim = SPEED_ANIM;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CExplosion::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CExplosion::Update(void)
{
	if (m_nPatternAnim <= PATTERNANIM_ALL)
	{// 総パターン数を超えてない
		if ((m_nCounterAnim % SPEED_ANIM) == 0)
		{// カウンターが設定した速さの数値に達した
			m_nCounterAnim = 0;		// カウンターを初期値に戻す

			VERTEX_2D *pVtx = 0;	// 頂点情報へのポインタ

			// 頂点バッファをロックし頂点情報へのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * m_nPatternAnim, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * m_nPatternAnim, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * m_nPatternAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * m_nPatternAnim, 1.0f);

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();

			m_nPatternAnim++;     // パターンNo.を更新する
		}

		m_nCounterAnim++;   // カウンターを加算
	}
	else
	{// 総パターン数を超えた
		Uninit();
	}
}

//===============================================
// 描画処理
//===============================================
void CExplosion::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}