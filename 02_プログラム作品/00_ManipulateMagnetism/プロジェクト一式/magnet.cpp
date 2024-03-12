//=========================================================
//
// 磁石ブロック処理 [magnet.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "magnet.h"
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
#include "particle.h"

//===============================================
// 定数定義
//===============================================
namespace
{
	const float LENTH_MAGNET = 80.0f;		// 磁石が反応する距離
}

//===============================================
// 静的メンバ変数
//===============================================
int CMagnet::m_aIdxXFile[MODEL_MAX] = {};	// 使用するXファイルの番号

//===============================================
// コンストラクタ
//===============================================
CMagnet::CMagnet() : CObjectX(3)
{
	// 値をクリアする
	m_state = STATE_NONE;
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
CMagnet::CMagnet(int nPriority) : CObjectX(nPriority)
{
	// 値をクリアする
	m_state = STATE_NONE;
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
CMagnet::~CMagnet()
{

}

//===============================================
// 生成処理
//===============================================
CMagnet *CMagnet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority)
{
	CMagnet *pObjX;

	// オブジェクトの生成
	pObjX = new CMagnet(nPriority);

	// 種類の設定
	if (type == MODEL_DAMAGE)
	{
		pObjX->SetType(TYPE_BOXDAMAGE);
	}
	else
	{
		pObjX->SetType(TYPE_BOXNORMAL);
	}

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
HRESULT CMagnet::Init(D3DXVECTOR3 pos)
{
	//// 影を設定
	//m_nIdxShadow = SetShadow();

	// 位置を反映
	m_pos = pos;

	m_state = STATE_N;

	CObjectX::Init(m_pos);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMagnet::Uninit(void)
{
	CObjectX::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CMagnet::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの位置
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤーの前回の位置
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの向き
	CObject *pObj = nullptr;

	m_posOld = m_pos;

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

	//CollisionModel(&playerPos, &playerPosOld, 0, pObj, true);

	// 当たり判定
	//pPlayer->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	//CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
}

//===============================================
// 描画処理
//===============================================
void CMagnet::Draw(void)
{
	CObjectX::Draw();
}

//===============================================
// 描画処理
//===============================================
void CMagnet::DrawXFile(int nIdx, CXFile::COL col)
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
bool CMagnet::CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;
	float fLenth = 0.0f;
	float fAngleDist = 0.0f;
	D3DXVECTOR3 vecDiff = {};

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

			if (type == CObject::TYPE_BOXDAMAGE)
			{// プレイヤー
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z - LENTH_MAGNET <= pPos->z - vtxMin.z && pos.z + sizeMax.z + LENTH_MAGNET >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y - LENTH_MAGNET <= pPos->y + vtxMax.y && pos.y + sizeMax.y + LENTH_MAGNET >= pPos->y + vtxMin.y)
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

					// プレイヤーとブロックの距離や向きを計算し逆向きに飛ばす
					vecDiff.z = pPos->z - pos.z;
					vecDiff.y = pPos->y - pos.y;

					D3DXVec3Normalize(&vecDiff, &vecDiff);
					//vecDiff *= 0.08f;

					// 向きを目標方向に補正
					fAngleDist = atan2f(vecDiff.y, vecDiff.z);

					// プレイヤーまでの距離を計算
					vecDiff.x = pPos->x - pos.x;
					vecDiff.y = pPos->y - pos.y;
					vecDiff.z = pPos->z - pos.z;
					fLenth = D3DXVec3Length(&vecDiff);
					//fLenth = 200.0f - fLenth;

					if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N))
					{// 反発
						if (pos.z + sizeMin.z <= pPos->z
							&& pos.z + sizeMax.z >= pPos->z)
						{// 上下から
							pMove->y += vecDiff.y * 0.02f;
						}
						else if (pos.z + sizeMax.z >= pPos->z || pos.z + sizeMin.z <= pPos->z)
						{// 右からもしくは左から
							pMove->y += vecDiff.y * 0.04f;
							pMove->z += vecDiff.z * 0.04f;
						}
						CParticle::Create()->Set({ pPos->x, pPos->y, pPos->z }, CParticle::TYPE_ENEMY);
					}
					else if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S))
					{// 引き寄せ
						if ((pos.z + sizeMin.z <= pPos->z && pos.z + sizeMax.z >= pPos->z)
							|| pos.z + sizeMax.z >= pPos->z || pos.z + sizeMin.z <= pPos->z)
						{// 上下からまたは右からもしくは左から
							pMove->y = sinf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f;
							pMove->z = cosf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f;
						}
					}

					// 移動量の上限設定
					if (pMove->y >= 25.0f)
					{
						pMove->y = 25.0f;
					}
					if (pMove->z >= 25.0f)
					{
						pMove->z = 25.0f;
					}
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}
	CManager::GetInstance()->GetDebugProc()->Print(" プレイヤーの移動距離：%f\n", fLenth);

	return bLand;
}

//===============================================
// 敵との当たり判定
//===============================================
bool CMagnet::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;

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

			if (type == CObject::TYPE_BOXNORMAL || type == CObject::TYPE_BOXDAMAGE)
			{// プレイヤー
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// 範囲内にある
					if (posOld.y + sizeMax.y <= pPosOld->y + vtxMin.y
						&& pos.y + sizeMax.y >= pPos->y + vtxMin.y)
					{// 上からめり込んだ
						// 上にのせる
						pPos->y = posOld.y - vtxMin.y + sizeMax.y;
						pMove->y = 0.0f;

						bLand = true;
					}
					else if (posOld.y + sizeMin.y >= pPosOld->y + vtxMax.y
						&& pos.y + sizeMin.y <= pPos->y + vtxMax.y)
					{// 下からめり込んだ
						// 下に戻す
						pPos->y = posOld.y - vtxMax.y + sizeMin.y;
						//pPlayer->SetMove(D3DXVECTOR3(pPlayer->GetMove().x, 0.0f, pPlayer->GetMove().z));
					}
					else if (posOld.z + sizeMin.z >= pPosOld->z - vtxMin.z
						&& pos.z + sizeMin.z <= pPos->z - vtxMin.z)
					{// 左から右にめり込んだ
						// 位置を戻す
						pPos->z = posOld.z + vtxMin.z + sizeMin.z;
						pRot->y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
					else if (posOld.z + sizeMax.z <= pPosOld->z + vtxMin.z
						&& pos.z + sizeMax.z >= pPos->z + vtxMin.z)
					{// 右から左にめり込んだ
						// 位置を戻す
						pPos->z = posOld.z - vtxMin.z + sizeMax.z;
						pRot->y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	return bLand;
}