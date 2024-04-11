//=========================================================
//
// ごみ処理 [garbage.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "garbage.h"
#include "manager.h"
#include "renderer.h"
#include "score.h"
#include "objectX.h"
#include "player.h"
#include "input.h"
#include "particle.h"
#include "debugproc.h"
#include "xfile.h"
#include "target.h"
#include "rockon.h"
#include "dumpster.h"
#include "game.h"
#include "effect.h"
#include "uigage.h"
#include "tutorial.h"
#include "sound.h"
#include "justdust.h"

//===============================================
// 静的メンバ変数
//===============================================
int CGarbage::m_aIdxXFile[TYPE_MAX] = {};			// 使用するXファイルの番号
int CGarbage::m_nNumAll = 0;						// 総数
int CGarbage::m_nRandCounter = 0;					// randリセット用

//===============================================
// マクロ定義
//===============================================
#define MAX_GABAGE		(1024)		// ゴミの最大数
#define MOVE_DIFF		(0.02f)		// 位置の補正係数（数が大きい程素早く変わる）
#define JUMP_HEIGHT		(16.85f)	// ジャンプ力
#define ADD_SCORE		(500)		// 正解時のスコア
#define ADD_SOCREJUST	(800)		// JUSTDUSTのスコア
#define SUB_SCORE		(-50)		// 不正解時のスコア

#define DISPOSE_LENTHZ			(150.0f)	// ゴミを回収できる範囲(Z)
#define DISPOSE_LENTHX			(300.0f)	// ゴミを回収できる範囲(X)
#define DISPOSE_DISTANCE		(250.0f)	// ゴミを回収できるプレイヤーからの前方距離
#define DIFF_DISPOSE			(10.0f)		// ゴミを回収できる範囲の誤差
#define TURN_ROT_DIFF			(0.001f)	// 曲がる角度の差分

#define DIFF_HEIGHT			(100.0f)	// ゴミ回収時の目的の高さ
#define GRAVITY_DISPOSE		(10.0f)		// 捨てる時の重力
#define WAIT_TIME			(60)		// 捨てたあとに静止している時間

// TUTORIAL
#define DELETE_POSITION			(650.0f)	// 床を破棄する位置

//===============================================
// コンストラクタ
//===============================================
CGarbage::CGarbage() : CObjectX(4)
{
	// 値のクリア
	particleType = CParticle::TYPE_BURN;
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nextposDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_BAG;
	m_size = MODELSIZE_NORMAL;
	m_state = STATE_NONE;
	m_separation = SEPARATION_NONE;
	m_rockonType = SEPARATION_NONE;
	m_bLand = false;
	m_bJustDust = false;
	m_fMoveDiff = 0.0f;
	m_nIdxDumpster = 0;

	m_nNumAll++;	// 総数をカウントアップ
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CGarbage::CGarbage(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	particleType = CParticle::TYPE_BURN;
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nextposDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_BAG;
	m_size = MODELSIZE_NORMAL;
	m_state = STATE_NONE;
	m_separation = SEPARATION_NONE;
	m_rockonType = SEPARATION_NONE;
	m_bLand = false;
	m_bJustDust = false;
	m_fMoveDiff = 0.0f;
	m_nIdxDumpster = 0;

	m_nNumAll++;		// 総数をカウントアップ
}

//===============================================
// デストラクタ
//===============================================
CGarbage::~CGarbage()
{
	m_nNumAll--;		// 総数をカウントダウン
}

//===============================================
// モデルの生成（読み込み）
//===============================================
HRESULT CGarbage::Load(void)
{
	// Ｘファイルに読み込み
	m_aIdxXFile[TYPE_BOOKOLD] = CManager::GetXFile()->Regist("data\\MODEL\\BookOld.x");
	m_aIdxXFile[TYPE_BOOT] = CManager::GetXFile()->Regist("data\\MODEL\\boot.x");
	m_aIdxXFile[TYPE_BAG] = CManager::GetXFile()->Regist("data\\MODEL\\bag001.x");
	m_aIdxXFile[TYPE_LIGHT] = CManager::GetXFile()->Regist("data\\MODEL\\Stage_Illumination.x");
	m_aIdxXFile[TYPE_CAMERA] = CManager::GetXFile()->Regist("data\\MODEL\\Camera001.x");
	m_aIdxXFile[TYPE_BUCKET] = CManager::GetXFile()->Regist("data\\MODEL\\bucket.x");
	m_aIdxXFile[TYPE_BIN] = CManager::GetXFile()->Regist("data\\MODEL\\bin.x");
	m_aIdxXFile[TYPE_CAN] = CManager::GetXFile()->Regist("data\\MODEL\\can.x");
	m_aIdxXFile[TYPE_DANBORU] = CManager::GetXFile()->Regist("data\\MODEL\\danboru.x");

	return S_OK;
}

//===============================================
// テクスチャの破棄
//===============================================
void CGarbage::Unload(void)
{
	
}

//===============================================
// 生成処理
//===============================================
CGarbage *CGarbage::Create(D3DXVECTOR3 pos, int nPriority)
{
	CGarbage *pGarbage = NULL;
	bool bMatchPos = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while (pObject != NULL)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得

			if (type == CObject::TYPE_GARBAGE)
			{// ゴミ
				if (pObject->GetPos().x <= pos.x + 3.0f && pObject->GetPos().x >= pos.x - 3.0f
					&& pObject->GetPos().y <= pos.y + 3.0f && pObject->GetPos().y >= pos.y - 3.0f
					&& pObject->GetPos().z <= pos.z + 3.0f && pObject->GetPos().z >= pos.z - 3.0f)
				{// 既に配置されたゴミと位置が一致
					bMatchPos = true;
				}
			}

			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	if (bMatchPos == false)
	{
		if (MAX_GABAGE > m_nNumAll)
		{// 総数に達していない
			// ゴミの生成
			pGarbage = new CGarbage(nPriority);

			// 種類の設定
			pGarbage->SetType(CObject::TYPE_GARBAGE);

			// 初期化処理
			pGarbage->Init(pos);

			// パーティクルの生成
			CParticle::Create(1)->Set(pos, CParticle::TYPE_ENEMY);		// パーティクルの生成
		}
		else
		{
			m_nNumAll = MAX_GABAGE;		// 総数を戻す
		}
	}

	return pGarbage;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CGarbage::Init(D3DXVECTOR3 pos)
{
	// オブジェクトXの初期化処理
	CObjectX::Init(pos);

	// 種類の設定
	srand(m_nRandCounter);							// randの値をリセット
	m_type = (TYPE)(rand() % TYPE_MAX);				// randの値を取得(種類)
	m_size = (MODELSIZE)(rand() % MODELSIZE_MAX);	// randの値を取得(サイズ)

	// 状態の設定
	m_state = STATE_NORMAL;

	// 分別の種類
	if (m_type == TYPE_BOOKOLD || m_type == TYPE_BOOT || m_type == TYPE_BAG)
	{// 古本・長靴・鞄
		m_separation = SEPARATION_BURN;				// 燃えるゴミ
	}
	else if (m_type == TYPE_LIGHT || m_type == TYPE_CAMERA || m_type == TYPE_BUCKET)
	{// ライト・カメラ・バケツ
		m_separation = SEPARATION_NONFLAMMABLE;		// 燃えないゴミ
	}
	else if (m_type == TYPE_BIN || m_type == TYPE_CAN || m_type == TYPE_DANBORU)
	{// 瓶・缶・段ボール
		m_separation = SEPARATION_RECYCLABLE;		// 資源ゴミ
	}

	// モデルサイズ設定
	if (m_type == TYPE_BOOKOLD || m_type == TYPE_LIGHT || m_type == TYPE_BIN)
	{// 古本・ライト・瓶
		m_size = MODELSIZE_NORMAL;					// 通常
	}
	else if (m_type == TYPE_BOOT || m_type == TYPE_CAMERA || m_type == TYPE_CAN)
	{// 長靴・カメラ・缶
		m_size = MODELSIZE_SMALL;					// 小さい
	}
	else if (m_type == TYPE_BAG || m_type == TYPE_BUCKET || m_type == TYPE_DANBORU)
	{// 鞄・バケツ・段ボール
		m_size = MODELSIZE_BIG;						// 大きい
	}

	m_nRandCounter++;	// rand数値変更

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGarbage::Uninit(void)
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CGarbage::Update(void)
{
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの位置を取得
	D3DXVECTOR3 PlayerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの向きを取得

	// 前回の位置を保存
	m_posOld = m_pos;

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		PlayerPos = CTutorial::GetPlayer()->GetPos();			// プレイヤーの位置を取得
		PlayerRot = CTutorial::GetPlayer()->GetRot();			// プレイヤーの向きを取得
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		PlayerPos = CGame::GetPlayer()->GetPos();			// プレイヤーの位置を取得
		PlayerRot = CGame::GetPlayer()->GetRot();			// プレイヤーの向きを取得
	}

	//重力処理
	m_move.y -= MOVE_GRAVITY;

	if (m_pos.y < LAND_POS)
	{// 着地した
		// 位置を戻す
		m_pos.y = LAND_POS;
		m_move.y = 0.0f;
	}

	// 状態別に処理する
	switch (m_state)
	{
	case STATE_NORMAL:			// 通常状態
		if (CManager::GetMode() == CScene::MODE_TUTORIAL &&
			(m_pos.x <= CTutorial::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX && m_pos.x >= CTutorial::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
			&& m_pos.z <= CTutorial::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY && m_pos.z >= CTutorial::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY)
			|| CManager::GetMode() == CScene::MODE_GAME &&
			(m_pos.x <= CGame::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX && m_pos.x >= CGame::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
				&& m_pos.z <= CGame::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY && m_pos.z >= CGame::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY))
		{// ターゲットの範囲内に入った
			if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax(m_size) == true)
				|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax(m_size) == true))
			{
				if (CManager::GetMode() == CScene::MODE_TUTORIAL)
				{// チュートリアル
					CTutorial::GetTarget()->SetMaxGage(true);
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{// ゲーム
					CGame::GetTarget()->SetMaxGage(true);
				}
			}
			else
			{
				if (CManager::GetMode() == CScene::MODE_TUTORIAL)
				{// チュートリアル
					CTutorial::GetTarget()->SetMaxGage(false);
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{// ゲーム
					CGame::GetTarget()->SetMaxGage(false);
				}
			}

			if (CManager::GetMode() == CScene::MODE_TUTORIAL &&(CTutorial::GetTarget()->GetState() != SEPARATION_NONE && CTutorial::GetTarget()->GetWithinRange() == true)
				|| CManager::GetMode() == CScene::MODE_GAME && (CGame::GetTarget()->GetState() != SEPARATION_NONE && CGame::GetTarget()->GetWithinRange() == true))
			{// 分別の設定がされている
				CRockon::Create(m_pos, CTarget::GetState(), 0, 5);		// その位置にターゲットを設定
				m_rockonType = CTarget::GetState();

				if (m_rockonType != SEPARATION_NONE)
				{
					m_state = STATE_SEPARATION;								// 分別状態へ
				}
			}
		}
		break;

	case STATE_ROCKON:			// ロックオン可能
		// ゴミ分別
		if (CManager::GetKeyboardInput()->GetPress(DIK_LEFT) == true
			|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_X, 0) == true)
		{// 左キーが押された
			// 燃えるゴミ
			CRockon::Create(m_pos, SEPARATION_BURN, 0, 5);			// その位置にターゲットを設定
			m_rockonType = SEPARATION_BURN;
		}
		else if (CManager::GetKeyboardInput()->GetPress(DIK_UP) == true
			|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_Y, 0) == true)
		{// 上キーが押された
			// 燃えないゴミ
			CRockon::Create(m_pos, SEPARATION_NONFLAMMABLE, 0, 5);	// その位置にターゲットを設定
			m_rockonType = SEPARATION_NONFLAMMABLE;
		}
		else if (CManager::GetKeyboardInput()->GetPress(DIK_RIGHT) == true
			|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_B, 0) == true)
		{// 右キーが押された
			// 資源ゴミ
			CRockon::Create(m_pos, SEPARATION_RECYCLABLE, 0, 5);		// その位置にターゲットを設定
			m_rockonType = SEPARATION_RECYCLABLE;
		}

		if (CManager::GetMode() == CScene::MODE_TUTORIAL &&
			(m_pos.x >= CTutorial::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX || m_pos.x <= CTutorial::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
			|| m_pos.z >= CTutorial::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY || m_pos.z <= CTutorial::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY)
			|| CManager::GetMode() == CScene::MODE_GAME &&
			(m_pos.x >= CGame::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX || m_pos.x <= CGame::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
			|| m_pos.z >= CGame::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY || m_pos.z <= CGame::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY))
		{// ターゲットの範囲外へ外れた
			m_state = STATE_NORMAL;		// 通常状態にする
		}
		break;

	case STATE_SEPARATION:		// 分別
		if ((m_rockonType == SEPARATION_BURN && (CManager::GetKeyboardInput()->GetRelease(DIK_RIGHT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_B, 0) == true))
			|| (m_rockonType == SEPARATION_NONFLAMMABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_LEFT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_X, 0) == true))
			|| (m_rockonType == SEPARATION_RECYCLABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_UP) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_Y, 0) == true)))
		{// キーを離した
			if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax(m_size) == false)
				|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax(m_size) == false))
			{// ゴミゲージが最大じゃない
				if ((PlayerRot.y <= D3DX_PI * ROT_UP + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_UP - TURN_ROT_DIFF && m_pos.z < PlayerPos.z) 
					|| (PlayerRot.y <= D3DX_PI * ROT_DOWN + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_DOWN - TURN_ROT_DIFF && m_pos.z > PlayerPos.z)
					|| (PlayerRot.y <= -D3DX_PI * ROT_DOWN + TURN_ROT_DIFF && PlayerRot.y >= -D3DX_PI * ROT_DOWN - TURN_ROT_DIFF && m_pos.z > PlayerPos.z)
					|| (PlayerRot.y <= D3DX_PI * ROT_RIGHT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_RIGHT - TURN_ROT_DIFF && m_pos.x < PlayerPos.x)
					|| (PlayerRot.y <= D3DX_PI * ROT_LEFT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_LEFT - TURN_ROT_DIFF && m_pos.x > PlayerPos.x))
				{// ロックオンしたゴミが画面内にある
					if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetTarget()->GetFlameJust() < FLAME_JUSTDUST && CTutorial::GetTarget()->GetFlameJust() >= 0)
						|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetTarget()->GetFlameJust() < FLAME_JUSTDUST && CGame::GetTarget()->GetFlameJust() >= 0))
					{// ジャストタイミング
						m_bJustDust = true;		// JustDustに設定
					}

					if (CManager::GetMode() == CScene::MODE_TUTORIAL)
					{// チュートリアル
						CTutorial::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);		// プレイヤーモーションを設定
					}
					else if (CManager::GetMode() == CScene::MODE_GAME)
					{// ゲーム
						CGame::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);			// プレイヤーモーションを設定
					}

					m_posDest.y = DIFF_HEIGHT;		// 目的の高さを設定
					m_state = STATE_DISPOSE;		// 捨てる
				}
				else
				{
					m_state = STATE_NORMAL;		// 通常状態へ戻す
				}
			}
			else
			{// ゲージが最大を超えてしまう
				m_state = STATE_NORMAL;		// 通常状態へ戻す

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_DISABLE);
			}
		}
		else if (m_rockonType == SEPARATION_NONE)
		{// 分別の設定がされていない
			m_state = STATE_NORMAL;		// 通常状態にする
		}
		//// プレイヤーの上で一定時間静止させる
		//m_pos = PlayerPos;
		//m_pos.y = DIFF_HEIGHT;

		//m_nWaitCounter--;	// 時間をデクリメント

		//if (m_nWaitCounter <= 0)
		//{// 設定した時間に達した
		//	m_state = STATE_COLLECT;		// 分別状態へ
		//}
		break;

	case STATE_DISPOSE:			// 捨てる
		m_fMoveDiff += MOVE_DIFF;	// 補正係数を加算する

		// 目的の位置を設定
		m_posDest.x = PlayerPos.x;
		m_posDest.z = PlayerPos.z;

		m_posDiff = m_posDest - m_pos;	// 目的の位置までの差分

		m_pos += m_posDiff * m_fMoveDiff;	// 位置の補正

		if (m_rockonType == m_separation)
		{// 分別成功
			if (m_bJustDust == false)
			{
				// エフェクトの生成
				CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);
			}
			else
			{
				// エフェクトの生成
				CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.9f, 0.5f, 0.2f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);
			}
		}
		else
		{// 分別失敗
			// エフェクトの生成
			CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);
		}

		if (m_pos.x <= m_posDest.x + DIFF_DISPOSE
			&& m_pos.x >= m_posDest.x - DIFF_DISPOSE
			&& m_pos.z <= m_posDest.z + DIFF_DISPOSE
			&& m_pos.z >= m_posDest.z - DIFF_DISPOSE)
		{// 目的の位置になった
			m_state = STATE_COLLECT;		// 回収
			m_nWaitCounter = WAIT_TIME;		// 静止している時間を設定
			m_fMoveDiff = 0.0f;				// 補正係数を初期化
		}
		break;

	case STATE_COLLECT:			// 回収
		m_pos.y -= GRAVITY_DISPOSE;		// 重力を付与

		if (m_pos.y <= LAND_POS + 50.0f)
		{// プレイヤーに貫通
			// パーティクルの生成
			CParticle::Create(3)->Set(D3DXVECTOR3(m_pos.x - sinf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f), m_pos.y, 
				m_pos.z - cosf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f)), particleType);

			// 設定処理
			if (CManager::GetMode() == CScene::MODE_TUTORIAL)
			{
				// モデルのサイズ別にゲージ上昇量を変更
				switch (m_size)
				{
				case MODELSIZE_NORMAL:		// 通常
					CTutorial::GetUiGage()->Set(AMOUNT_OF_RISE_NORMAL);		// ゲージ上昇
					break;

				case MODELSIZE_SMALL:		// 小さい
					CTutorial::GetUiGage()->Set(AMOUNT_OF_RISE_SMALL);		// ゲージ上昇
					break;

				case MODELSIZE_BIG:			// 大きい
					CTutorial::GetUiGage()->Set(AMOUNT_OF_RISE_BIG);		// ゲージ上昇
					break;
				}
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				// モデルのサイズ別にゲージ上昇量を変更
				switch (m_size)
				{
				case MODELSIZE_NORMAL:		// 通常
					CGame::GetUiGage()->Set(AMOUNT_OF_RISE_NORMAL);		// ゲージ上昇
					break;

				case MODELSIZE_SMALL:		// 小さい
					CGame::GetUiGage()->Set(AMOUNT_OF_RISE_SMALL);		// ゲージ上昇
					break;

				case MODELSIZE_BIG:			// 大きい
					CGame::GetUiGage()->Set(AMOUNT_OF_RISE_BIG);		// ゲージ上昇
					break;
				}
			}

			Separation(m_rockonType);		// 分別処理（スコアに加算される）

			m_state = STATE_INPLAYER;		// 描画しない状態にする
		}
		break;

	case STATE_INPLAYER:		// 描画しない
		if (CManager::GetMode() == CScene::MODE_TUTORIAL)
		{
			m_pos = CTutorial::GetPlayer()->GetPos();	// ゴミの位置をプレイヤーと同期させる
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			m_pos = CGame::GetPlayer()->GetPos();	// ゴミの位置をプレイヤーと同期させる
		}

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

			while (pObject != NULL)
			{// 使用されている
				CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
				CObject::TYPE type = pObject->GetType();		// 種類を取得

				if (type == CObject::TYPE_DUMPSTER)
				{// ゴミステーション
					if ((CManager::GetMode() == CScene::MODE_TUTORIAL
						&& pObject->GetPos().x <= CTutorial::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().x >= CTutorial::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
						&& pObject->GetPos().z <= CTutorial::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEY && pObject->GetPos().z >= CTutorial::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEY)
						|| (CManager::GetMode() == CScene::MODE_GAME
						&& pObject->GetPos().x <= CGame::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().x >= CGame::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
						&& pObject->GetPos().z <= CGame::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEY && pObject->GetPos().z >= CGame::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEY))
					{// ターゲットがゴミステーションを利用できる範囲内に入った
						if (CManager::GetKeyboardInput()->GetTrigger(DIK_SPACE) == true
							|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
						{// SPACEキーが押された
							if (CManager::GetMode() == CScene::MODE_TUTORIAL)
							{// チュートリアル
								CTutorial::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);		// プレイヤーモーションを設定
							}
							else if (CManager::GetMode() == CScene::MODE_GAME)
							{// ゲーム
								CGame::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);			// プレイヤーモーションを設定
							}

							m_posDest.y = DIFF_HEIGHT;			// 目的の高さを設定
							m_nextposDest = pObject->GetPos();	// 次の目的の位置を保存

							m_state = STATE_DUMPSTER;		// ゴミステーション
						}
					}
				}
				pObject = pObjectNext;		// 次のオブジェクトを代入
			}
		}
		break;

	case STATE_DUMPSTER:		// ゴミステーション
		m_fMoveDiff += MOVE_DIFF;	// 補正係数を加算する

		// 目的の位置を設定
		m_posDest.x = m_nextposDest.x;
		m_posDest.z = m_nextposDest.z;

		m_posDiff = m_posDest - m_pos;		// 目的の位置までの差分
		m_pos += m_posDiff * m_fMoveDiff;	// 位置の補正

		// エフェクトの生成
		CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.2f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);

		if (m_pos.x <= m_posDest.x + DIFF_DISPOSE
			&& m_pos.x >= m_posDest.x - DIFF_DISPOSE
			&& m_pos.z <= m_posDest.z + DIFF_DISPOSE
			&& m_pos.z >= m_posDest.z - DIFF_DISPOSE)
		{// 目的の位置になった
			m_state = STATE_DELETE;			// 破棄

			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_DUMPSTER);
		}
		break;

	case STATE_DELETE:			// 破棄
		m_pos.y -= GRAVITY_DISPOSE;		// 重力を付与

		if (m_pos.y <= LAND_POS + 50.0f)
		{// プレイヤーに貫通
			// パーティクルの生成
			CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x - sinf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f), m_pos.y,
				m_pos.z - cosf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f)), particleType);

			if (CManager::GetMode() == CScene::MODE_GAME)
			{// ゲームモード
				CGame::GetScore()->Add(ADD_SCORE);		// スコア加算

				// 設定処理
				CGame::GetUiGage()->Set(-MAX_GARBAGE);
			}
			else if (CManager::GetMode() == CScene::MODE_TUTORIAL)
			{// チュートリアル
				CTutorial::GetScore()->Add(ADD_SCORE);		// スコア加算

				// 設定処理
				CTutorial::GetUiGage()->Set(-MAX_GARBAGE);
			}

			 // 終了処理
			Uninit(); 
		}
		break;
	}

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (PlayerPos.z < m_pos.z - DELETE_POSITION)
		{// 床が画面外へいった
			// 終了処理
			Uninit();
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CGarbage::Draw(void)
{
	if (m_state != STATE_INPLAYER)
	{// 描画しない状態じゃない
		if (m_state == STATE_ROCKON)
		{// ロックオン可能状態
			// オブジェクトXの描画処理（赤）
			CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_RED);
		}
		else
		{// それ以外
			// 分別の種類別に色を変える
			switch (m_separation)
			{
			case SEPARATION_BURN:			// 燃えるゴミ
				// オブジェクトXの描画処理（赤）
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_RED);
				break;

			case SEPARATION_NONFLAMMABLE:	// 燃えないゴミ
				// オブジェクトXの描画処理（青）
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_BLUE);
				break;

			case SEPARATION_RECYCLABLE:		// 資源ゴミ
				// オブジェクトXの描画処理（黄色）
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_YELLOW);
				break;

			default:						// それ以外
				// オブジェクトXの描画処理
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_NORMAL);
				break;
			}
		}
	}
}

//===============================================
// 分別処理
//===============================================
void CGarbage::Separation(SEPARATION separation)
{
	m_rockonType = separation;	// ロックオン時の分別の種類

	if (m_separation == separation)
	{// 設定された分別の種類と入力した分別の種類が一致
		CManager::GetDebugProc()->Print(" 正解");
		if (CManager::GetMode() == CScene::MODE_TUTORIAL)
		{
			if (m_bJustDust == true)
			{// JustDustだった場合
				CTutorial::GetScore()->Add(ADD_SOCREJUST);	// スコア加算
				CTutorial::GetJustDust()->Set(true);		// JustDust設定

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_JUSTDUST);
			}
			else
			{
				CTutorial::GetScore()->Add(ADD_SCORE);		// スコア加算

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_SUCCESS);
			}
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			if (m_bJustDust == true)
			{// JustDustだった場合
				CGame::GetScore()->Add(ADD_SOCREJUST);	// スコア加算
				CGame::GetJustDust()->Set(true);		// JustDust設定

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_JUSTDUST);
			}
			else
			{
				CGame::GetScore()->Add(ADD_SCORE);		// スコア加算

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_SUCCESS);
			}
		}
	}
	else
	{// 不一致
		CManager::GetDebugProc()->Print(" 不正解");
		if (CManager::GetMode() == CScene::MODE_TUTORIAL)
		{
			CTutorial::GetScore()->Add(SUB_SCORE);		// スコア減算
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetScore()->Add(SUB_SCORE);		// スコア減算
		}

		// サウンドの再生
		CManager::GetSound()->Play(CSound::LABEL_SE_FAILURE);
	}
}