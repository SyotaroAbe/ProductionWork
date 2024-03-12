//=========================================================
//
// アイテム処理 [item.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "item.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "xfile.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "tutorial.h"
#include "manager.h"
#include "debugproc.h"

//===============================================
// 定数定義
//===============================================
namespace
{
	const float LENTH_PLAYER = 100.0f;		// アイテムが反応する距離
	const float MAGNET_PLAYER = 20.0f;		// アイテムがプレイヤーとくっつく距離
	const float ITEM_SPEED = 135.0f;			// アイテムが飛ぶときのスピード
}

//===============================================
// 静的メンバ変数
//===============================================
int CItem::m_aIdxXFile[MODEL_MAX] = {};	// 使用するXファイルの番号

//===============================================
// コンストラクタ
//===============================================
CItem::CItem() : CObjectX(3)
{
	// 値をクリアする
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
{
	// 値をクリアする
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// デストラクタ
//===============================================
CItem::~CItem()
{

}

//===============================================
// 生成処理
//===============================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority)
{
	CItem *pObjX;

	// オブジェクトの生成
	pObjX = new CItem(nPriority);

	// 種類の設定
	pObjX->SetType(TYPE_ITEM);

	// モデルの設定
	pObjX->SetModel(type);

	// 初期化処理
	pObjX->Init(pos);

	// 向き設定
	pObjX->SetRot(rot);

	return pObjX;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	//// 影を設定
	//m_nIdxShadow = SetShadow();

	// 位置を反映
	m_pos = pos;

	CObjectX::Init(m_pos);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CItem::Uninit(void)
{
	CObjectX::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CItem::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの位置
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤーの前回の位置
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの向き
	CObject *pObj = nullptr;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while (pObject != nullptr)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得

			if (type == CObject::TYPE_PLAYER)
			{// プレイヤー
				playerPos = pObject->GetPos();			// 位置を取得
				playerPosOld = pObject->GetPosOld();	// 前回の位置を取得
				playerRot = pObject->GetRot();			// 向きを取得
				pObj = pObject;
			}

			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetPlayer()->GetPos() == m_pos)
		{
			m_bMagnet = true;
		}
	}

	if (CollisionModel(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin) == true)
	{
		m_move.y = 0.0f;
	}

	CManager::GetInstance()->GetDebugProc()->Print(" アイテムの位置：（%f, %f, %f）\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print(" アイテムの前回の位置：（%f, %f, %f）\n", m_posOld.x, m_posOld.y, m_posOld.z);
	CManager::GetInstance()->GetDebugProc()->Print(" アイテムの移動量：（%f, %f, %f）\n", m_move.x, m_move.y, m_move.z);

	//CollisionModel(&playerPos, &playerPosOld, 0, pObj, true);

	// 当たり判定
	//CGame::GetPlayer()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	//CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);

	CObjectX::Update();
}

//===============================================
// 描画処理
//===============================================
void CItem::Draw(void)
{
	int nIdxXFile = CObjectX::GetXFile(CObjectX::MODEL_ITEM);

	CObjectX::DrawXFile(nIdxXFile, CXFile::COL_RED);
}

//===============================================
// 描画処理
//===============================================
void CItem::DrawXFile(int nIdx, CXFile::COL col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// Xファイルの描画
	CManager::GetInstance()->GetXFile()->Draw(nIdx, col);
}

//===============================================
// プレイヤーとの当たり判定
//===============================================
bool CItem::CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;
	float fLenth = 0.0f;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入
		
		while (pObject != nullptr)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得
			D3DXVECTOR3 pos = pObject->GetPos();			// 位置
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// 前回の位置
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// 最大サイズ
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// 最小サイズ

			if (type == CObject::TYPE_ITEM)
			{
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z - MAGNET_PLAYER <= pPos->z - vtxMin.z && pos.z + sizeMax.z + MAGNET_PLAYER >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y - MAGNET_PLAYER <= pPos->y + vtxMax.y && pos.y + sizeMax.y + MAGNET_PLAYER >= pPos->y + vtxMin.y)
				{// 範囲内にある
					if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N))
					{// 反発
						if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_A) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) < 0)
						{// 左キーが押された
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_DOWN) * ITEM_SPEED, cosf(D3DX_PI * ROT_DOWN) * ITEM_SPEED));
						}
						else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_D) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) > 0)
						{// 右キーが押された
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_UP) * ITEM_SPEED, cosf(D3DX_PI * ROT_UP) * ITEM_SPEED));
						}
						else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_W) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLY(0) < 0)
						{// 上キーが押された
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_RIGHT) * ITEM_SPEED, cosf(D3DX_PI * ROT_RIGHT) * ITEM_SPEED));
						}
						else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_S) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLY(0) > 0)
						{// 下キーが押された
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_LEFT) * ITEM_SPEED, cosf(D3DX_PI * ROT_LEFT) * ITEM_SPEED));
						}
						else
						{// プレイヤーの向いている方向
							if (CManager::GetMode() == CScene::MODE_GAME)
							{
								pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_UP + (1.0f * CGame::GetPlayer()->GetRot().y)) * ITEM_SPEED, cosf(D3DX_PI * ROT_UP) * ITEM_SPEED));
							}
						}
					}
					else if((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S))
					{// 引き寄せ
						pObject->SetPos(D3DXVECTOR3(pPos->x, pPos->y, pPos->z));
					}
				}
				else if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z - LENTH_PLAYER <= pPos->z - vtxMin.z && pos.z + sizeMax.z + LENTH_PLAYER >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y - LENTH_PLAYER <= pPos->y + vtxMax.y && pos.y + sizeMax.y + LENTH_PLAYER >= pPos->y + vtxMin.y)
				{// 範囲内にある
					CPlayer::EState state = CPlayer::STATE_NONE;

					if (CManager::GetMode() == CScene::MODE_GAME)
					{
						state = CGame::GetPlayer()->GetState();
					}
					else if (CManager::GetMode() == CScene::MODE_TUTORIAL)
					{
						state = CTutorial::GetPlayer()->GetState();
					}

					if (CManager::GetMode() == CScene::MODE_GAME)
					{
						// プレイヤーとブロックの距離や向きを計算し逆向きに飛ばす
						D3DXVECTOR3 vecDiff;
						vecDiff.z = pos.z - pPos->z;
						vecDiff.y = pos.y - pPos->y;

						D3DXVec3Normalize(&vecDiff, &vecDiff);
						//vecDiff *= 0.08f;

						// 向きを目標方向に補正
						float fAngleDist = atan2f(vecDiff.y, vecDiff.z);

						// プレイヤーまでの距離を計算
						D3DXVECTOR3 vecDiffDelete;
						vecDiffDelete.x = pos.x - pPos->x;
						vecDiffDelete.y = pos.y - pPos->y;
						vecDiffDelete.z = pos.z - pPos->z;
						fLenth = D3DXVec3Length(&vecDiffDelete);
						fLenth = 220.0f - fLenth;

						//CGame::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, posOld.y - vtxMin.y + sizeMax.y, pPos->z));
						if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N)
							|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N))
						{// 反発
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_UP + fAngleDist) * fLenth * 0.1f, cosf(D3DX_PI * ROT_UP + fAngleDist) * fLenth * 0.1f));
						}
						else if((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S)
							|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S))
						{// 引き寄せ
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f, cosf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f));
						}
					}
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	return bLand;
}

//===============================================
// 敵との当たり判定
//===============================================
bool CItem::CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bDeath = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject* pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while (pObject != nullptr)
		{// 使用されている
			CObject* pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得
			D3DXVECTOR3 pos = pObject->GetPos();			// 位置
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// 前回の位置
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// 最大サイズ
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// 最小サイズ

			if (type == CObject::TYPE_ITEM)
			{
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// 範囲内にある
					bDeath = true;
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	return bDeath;
}