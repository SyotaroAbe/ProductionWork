//=========================================================
//
// ブロック処理 [block.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "explosion.h"
#include "texture.h"

//===============================================
// 静的メンバ変数
//===============================================
int CBlock::m_nIdxTexture = 0;						// 使用するテクスチャの番号
int CBlock::m_nNumAll = 0;							// 総数

//===============================================
// コンストラクタ
//===============================================
CBlock::CBlock() : CObject2D(3)
{
	m_nNumAll++;	// 総数をインクリメント
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	m_nNumAll++;	// 総数をインクリメント
}

//===============================================
// デストラクタ
//===============================================
CBlock::~CBlock()
{
	m_nNumAll--;	// 総数をデクリメント
}

//===============================================
// 生成処理
//===============================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, int nPriority)
{
	CBlock *pBlock;

	// ブロックの生成
	pBlock = new CBlock(3);

	// 種類の設定
	pBlock->SetType(TYPE_BLOCK);

	// サイズの設定
	pBlock->SetSize(BLOCK_SIZEX, BLOCK_SIZEY);

	// 初期化処理
	pBlock->Init(pos);

	// テクスチャの割り当て
	pBlock->BindTexture(m_nIdxTexture);

	return pBlock;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CBlock::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\block000.png");

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CBlock::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CBlock::Update(void)
{
	// 前回の情報の保存
	m_posOld = m_pos;

	// 位置の更新
	UpdatePos(m_pos, BLOCK_SIZEX, BLOCK_SIZEY);
}

//===============================================
// 描画処理
//===============================================
void CBlock::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}

//===============================================
// 設定処理
//===============================================
void CBlock::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;
}

//===============================================
// プレイヤーとの当たり判定
//===============================================
bool CBlock::CollisionPlayer(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pBlock = CObject::GetTop(nCntPriority);	// 先頭のオブジェクトへのポインタを取得

		while (pBlock != NULL)
		{// 使用されている
			CObject *pBlockNext = pBlock->GetNext();	// 次のオブジェクトへのポインタを取得
			TYPE typeBlock = pBlock->GetType();			// 種類を取得

			if (typeBlock == TYPE_BLOCK)
			{// 種類がブロックの場合
				for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
				{
					CObject *pObject = CObject::GetTop(nCntPriority);	// 先頭のオブジェクトへのポインタを取得

					while (pObject != NULL)
					{// 使用されている
						CObject *pObjectNext = pObject->GetNext();	// 次のオブジェクトへのポインタを取得
						TYPE type = pObject->GetType();				// 種類を取得

						if (type == TYPE_PLAYER)
						{// 種類がプレイヤーの場合
							D3DXVECTOR3 BlockPos = pBlock->GetPos();			// 位置を取得
							D3DXVECTOR3 BlockPosOld = pBlock->GetPosOld();		// 前回の位置を取得

							if (pos->x >= BlockPos.x - fSezeX
								&& pos->x <= BlockPos.x + BLOCK_SIZEX + fSezeX
								&& pos->y <= BlockPos.y + BLOCK_SIZEY + fSezeY
								&& pos->y >= BlockPos.y)
							{//	現在の位置がブロックの範囲内

								if (posOld->y <= BlockPosOld.y && pos->y >= BlockPos.y)
								{// ブロックの上からめり込んだ
									bLand = true;
									pObject->SetPos(D3DXVECTOR3(pos->x, BlockPos.y, 0.0f));		// ブロックの上にのせる
									pObject->SetMove(D3DXVECTOR3(move->x, 0.0f, 0.0f));			// 移動量を初期化する
								}
								else if (posOld->y - fSezeY >= BlockPosOld.y + BLOCK_SIZEY
									&& pos->y - fSezeY <= BlockPos.y + BLOCK_SIZEY)
								{//ブロックの下からめり込んだ
									pObject->SetPos(D3DXVECTOR3(pos->x, BlockPos.y + BLOCK_SIZEY + fSezeY, 0.0f));	// ブロックの下に移動させる
									pObject->SetMove(D3DXVECTOR3(move->x, 0.0f, 0.0f));								// 移動量を初期化する
								}
								else if (posOld->y >= BlockPosOld.y
									|| posOld->y - fSezeY <= BlockPosOld.y + BLOCK_SIZEY)
								{// ブロックの横からめり込んだ

									if (posOld->x - fSezeX * 0.85f >= BlockPosOld.x + BLOCK_SIZEX * 0.85f
										&& pos->x - fSezeX * 0.85f <= BlockPos.x + BLOCK_SIZEX)
									{// 右からめり込んだ
										pObject->SetMove(D3DXVECTOR3(0.0f, move->y, 0.0f));
										pObject->SetPos(D3DXVECTOR3(BlockPos.x + BLOCK_SIZEX + fSezeX * 0.85f, pos->y, 0.0f));	// ブロックの右に移動させる
									}
									else if (posOld->x + fSezeX * 0.85f <= BlockPosOld.x
										&& pos->x + fSezeX * 0.85f >= BlockPos.x)
									{// 左からめり込んだ
										pObject->SetMove(D3DXVECTOR3(0.0f, move->y, 0.0f));
										pObject->SetPos(D3DXVECTOR3(BlockPos.x - fSezeX * 0.85f, pos->y, 0.0f));	// ブロックの左に移動させる
									}
								}
							}
						}

						pObject = pObjectNext;		// 次のオブジェクトを代入
					}
				}
			}

			pBlock = pBlockNext;		// 次のオブジェクトを代入
		}
	}

	return bLand;
}

//===============================================
// ブロックとの当たり判定
//===============================================
bool CBlock::Collision(const TYPE Objtype, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pBlock = CObject::GetTop(nCntPriority);	// 先頭のオブジェクトへのポインタを取得

		while (pBlock != NULL)
		{// 使用されている
			CObject *pBlockNext = pBlock->GetNext();	// 次のオブジェクトへのポインタを取得
			TYPE typeBlock = pBlock->GetType();			// 種類を取得

			if (typeBlock == TYPE_BLOCK)
			{// ブロックの場合
				D3DXVECTOR3 BlockPos = pBlock->GetPos();			// 位置を取得
				D3DXVECTOR3 BlockPosOld = pBlock->GetPosOld();		// 前回の位置を取得

				if (pos->x >= BlockPos.x - fSezeX
					&& pos->x <= BlockPos.x + BLOCK_SIZEX + fSezeX
					&& pos->y <= BlockPos.y + BLOCK_SIZEY + fSezeY
					&& pos->y >= BlockPos.y - fSezeY)
				{//現在の位置がブロックの範囲内
					bLand = true;
				}
			}

			pBlock = pBlockNext;		// 次のオブジェクトを代入
		}
	}

	return bLand;
}