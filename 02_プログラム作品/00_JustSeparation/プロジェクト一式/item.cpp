//=========================================================
//
// アイテム処理 [item.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "explosion.h"
#include "player.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "score.h"
#include "texture.h"
#include "game.h"

//===============================================
// 静的メンバ変数
//===============================================
int CItem::m_aIdxTexture[CItem::TYPE_MAX] = {};						// 使用するテクスチャの番号
const char *CItem::m_apTextureData[CItem::TYPE_MAX] = {};			// テクスチャファイル名
int CItem::m_nNumAll = 0;											// 総数

//===============================================
// マクロ定義
//===============================================
#define ITEM_SIZEX		(35)				// 幅（半分）
#define ITEM_SIZEY		(35)				// 高さ（半分）
#define TREASURE_SCORE	(1000)				// 宝箱のスコア
#define COIN_SCORE		(500)				// コインのスコア

//===============================================
// コンストラクタ
//===============================================
CItem::CItem() : CObject2D(4)
{
	// 値をクリアする
	m_type = TYPE_TREASURE;

	m_nNumAll++;		// 総数をインクリメント
}

//===============================================
// デストラクタ
//===============================================
CItem::~CItem()
{
	m_nNumAll--;		// 総数をデクリメント
}

//===============================================
// 生成処理
//===============================================
CItem *CItem::Create(D3DXVECTOR3 pos, int nPriority)
{
	CItem *pItem;

	// アイテムの生成
	pItem = new CItem;

	// 種類の設定
	pItem->SetType(TYPE_ITEM);

	// サイズの設定
	pItem->SetSize(ITEM_SIZEX, ITEM_SIZEY);

	// 初期化処理
	pItem->Init(pos);

	return pItem;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャファイル名
	const char *m_apTextureData[TYPE_MAX] =
	{
		{ "data\\TEXTURE\\item000.png" },	// 宝箱
		{ "data\\TEXTURE\\item001.png" }	// 王冠
	};

	// テクスチャの設定
	for (int nCntItem = 0; nCntItem < TYPE_MAX; nCntItem++)
	{
		m_aIdxTexture[TYPE_TREASURE] = CManager::GetTexture()->Regist(m_apTextureData[nCntItem]);
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CItem::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CItem::Update(void)
{
	// プレイヤーとの当たり判定
	if (CollisionEnemy(m_pos) == false)
	{// アイテムが当たってない
		if (m_pos.x < 0.0f || m_pos.x > SCREEN_WIDTH || m_pos.y < 0.0f || m_pos.y > SCREEN_HEIGHT)
		{// アイテムの位置が画面外に出た場合
			CExplosion::Create(m_pos, 3)->SetPos(m_pos);	// 爆発の生成
			Uninit();				// 自分の終了処理
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CItem::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}

//===============================================
// 設定処理
//===============================================
void CItem::Set(const TYPE type, const D3DXVECTOR3 pos)
{
	// 設定された値を代入
	m_type = type;
	m_pos = pos;

	// テクスチャの割り当て
	CObject2D::BindTexture(m_aIdxTexture[m_type]);

	// 位置の更新処理
	CObject2D::UpdatePos(m_pos, ITEM_SIZEX, ITEM_SIZEY);
}

//===============================================
// プレイヤーとの当たり判定
//===============================================
bool CItem::CollisionEnemy(D3DXVECTOR3 pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObj = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while(pObj != NULL)
		{// 使用されている			// 種類を取得

			CObject::TYPE type = pObj->GetType();	// 種類を取得

			if (type == TYPE_PLAYER)
			{// プレイヤー
				D3DXVECTOR3 PlayerPos = pObj->GetPos();	// 位置を取得

				if (pos.x >= PlayerPos.x - PLAYER_SIZEX - ITEM_SIZEX
					&& pos.x <= PlayerPos.x + PLAYER_SIZEX + ITEM_SIZEX
					&& pos.y <= PlayerPos.y + PLAYER_SIZEX + ITEM_SIZEX
					&& pos.y >= PlayerPos.y - PLAYER_SIZEX - ITEM_SIZEX)
				{// プレイヤーとアイテムが当たった
					//CParticle::Create(1)->Set(m_pos, CParticle::TYPE_ITEM);		// パーティクルの生成

					// スコア加算
					switch (m_type)
					{
					case TYPE_TREASURE:		// 宝箱
						CGame::GetScore()->Add(TREASURE_SCORE);
						break;

					case TYPE_COIN:			// コイン
						CGame::GetScore()->Add(COIN_SCORE);
						break;
					}

					this->Uninit();			// 自分の終了処理

					//// サウンドの再生
					//CManager::GetSound()->Play(CSound::LABEL_SE_ITEM);
					return true;
				}
			}
		}
	}
	return false;
}