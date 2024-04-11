//=========================================================
//
// プレイヤー処理 [player.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "object.h"
#include "particle.h"
#include "model.h"
#include "meshfield.h"
#include "motion.h"
#include "fileload.h"
#include "shadow.h"
#include "object3D.h"
#include "game.h"
#include "tutorial.h"
#include "sound.h"
#include "title.h"
#include "bullet.h"
#include "objectX.h"
#include "magnet.h"
#include "renderer.h"
#include "fade.h"
#include "time.h"
#include "item.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE_PLAYER			(0.45f)		// 動く速度
#define JUMP_PLAYER			(16.85f)	// ジャンプ力
#define MOVE_GRAVITY		(0.75f)		// 重力
#define LAND_POS			(0.0f)		// 地面

#define MOVE_MINUS			(0.07f)		// 移動量の減衰
#define TURN_TIME			(1)			// 曲がる時間
#define COUNT_PARTICLE		(25)		// パーティクル発生時間

//===============================================
// 静的メンバ変数
//===============================================
CPlayer* CPlayer::m_pPlayer = nullptr;				// プレイヤークラスのポインタ
int CPlayer::m_nPriority = 0;

//===============================================
// コンストラクタ
//===============================================
CPlayer::CPlayer() : CObject(4)
{
	// 値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_fLenthCamera = 0.0f;
	m_fRotBullet = 0.0f;
	m_nStateCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
	m_bInvincible = false;
	m_nInvincibleCounter = 0;
	m_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDisp = true;
	m_magnet = EMAGNET_NONE;
	m_magnetOld = EMAGNET_NONE;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_fLenthCamera = 0.0f;
	m_fRotBullet = 0.0f;
	m_nStateCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_pMotion = nullptr;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
	m_bInvincible = false;
	m_nInvincibleCounter = 0;
	m_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDisp = true;
	m_magnet = EMAGNET_NONE;
	m_magnetOld = EMAGNET_NONE;
}

//===============================================
// デストラクタ
//===============================================
CPlayer::~CPlayer()
{

}

//===============================================
// 生成処理
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, int nPriority)
{
	CPlayer *pPlayer;
	m_nPriority = nPriority;

	// プレイヤーの生成
	pPlayer = new CPlayer(nPriority);

	if (pPlayer != nullptr)
	{
		// 種類の設定
		pPlayer->SetType(CObject::TYPE_PLAYER);

		// 初期化処理
		pPlayer->Init(pos);
	}

	return pPlayer;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// モーションの生成
	m_pMotion = new CMotion;

	// モーションの初期化
	m_pMotion->Init();

	// モデルの総数
	m_nNumModel = CManager::GetInstance()->GetLoad()->GetNumModel(CFileLoad::FILE_PLAYER);

	// 位置の設定
	m_pos = pos;
	m_posShadow = m_pos;

	// 向きの設定
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	// 移動速度の初期化
	m_fSpeed = MOVE_PLAYER;

	// パーティクル発生防止
	m_nParticleCounter = COUNT_PARTICLE;

	const char *apModelFile[MAX_MODEL];		// モデルファイル名

	// モデルの生成（全パーツ）
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		apModelFile[nCntModel] = CManager::GetInstance()->GetLoad()->GetFileName(CFileLoad::FILE_PLAYER, nCntModel);		// ファイル名取得
		pos = CManager::GetInstance()->GetLoad()->GetPos(CFileLoad::FILE_PLAYER, nCntModel);								// 位置の取得
		rot = CManager::GetInstance()->GetLoad()->GetRot(CFileLoad::FILE_PLAYER, nCntModel);								// 向きの取得

		m_apModel[nCntModel] = CModel::Create(apModelFile[nCntModel], pos, rot);	// 生成
	}
	//m_apModel[0] = CModel::Create("data\\MODEL\\GarbageCan_Wood.x", pos, D3DXVECTOR3(0.0f, 0.0f , 0.0f));	// 生成

	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// 親モデルの設定（全パーツ）
	m_apModel[0]->SetParent(nullptr);
	for (int nCntModel = 1; nCntModel < m_nNumModel; nCntModel++)
	{
		int nParent = 0;

		nParent = CManager::GetInstance()->GetLoad()->GetParent(CFileLoad::FILE_PLAYER, nCntModel);	// 親を取得

		m_apModel[nCntModel]->SetParent(m_apModel[nParent]);
	}

	// 値の初期化
	m_vtxMin = D3DXVECTOR3(900.0f, 900.0f, 900.0f);
	m_vtxMax = D3DXVECTOR3(-900.0f, -900.0f, -900.0f);

	// 全ての頂点を比較してモデルの最小値・最大値を抜き出す
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// 最小値の比較
		if (m_apModel[nCntModel]->GetVtxMin().x <= m_vtxMin.x)
		{
			m_vtxMin.x = m_apModel[nCntModel]->GetVtxMin().x;
		}

		if (m_apModel[nCntModel]->GetVtxMin().y <= m_vtxMin.y)
		{
			m_vtxMin.y = m_apModel[nCntModel]->GetVtxMin().y;
		}

		if (m_apModel[nCntModel]->GetVtxMin().z <= m_vtxMin.z)
		{
			m_vtxMin.z = m_apModel[nCntModel]->GetVtxMin().z;
		}

		// 最大値の比較
		if (m_apModel[nCntModel]->GetVtxMax().x >= m_vtxMax.x)
		{
			m_vtxMax.x = m_apModel[nCntModel]->GetVtxMax().x;
		}

		if (m_apModel[nCntModel]->GetVtxMax().y >= m_vtxMax.y)
		{
			m_vtxMax.y = m_apModel[nCntModel]->GetVtxMax().y;
		}

		if (m_apModel[nCntModel]->GetVtxMax().z >= m_vtxMax.z)
		{
			m_vtxMax.z = m_apModel[nCntModel]->GetVtxMax().z;
		}
	}

	// モーション情報の設定
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{// モーション数分繰り返す
		m_pMotion->Set(nCntMotion);
		m_pMotion->SetInfo(CManager::GetInstance()->GetLoad()->GetInfo(CFileLoad::FILE_PLAYER, nCntMotion));
	}

	// 初期モーション設定
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	// モーションの更新処理
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update();
	}

	// 初期状態設定
	m_state = STATE_NORMAL;
	m_fLenthCamera = LENTH_NORMAL;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPlayer::Uninit(void)
{
	// モデルの破棄
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != nullptr)
		{// 使用されている
			// モデルの終了処理
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = nullptr;
		}
	}

	// モーションへのポインタの破棄
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	this->Release();

	//if (m_pPlayer != nullptr)
	//{
	//	delete m_pPlayer;
	//	m_pPlayer = nullptr;
	//}
}

//===============================================
// 更新処理
//===============================================
void CPlayer::Update(void)
{
	float fGravity = MOVE_GRAVITY;

	// カウンタを更新
	m_nStateCounter--;
	m_nInvincibleCounter--;

	// 前回の位置を保存
	m_posOld = m_pos;
	m_magnetOld = m_magnet;

	if (m_state != STATE_CLEAR)
	{
		if (m_nParticleCounter < COUNT_PARTICLE && CManager::GetMode() != CScene::MODE_TITLE)
		{// パーティクル発生時間に達していない
			// パーティクルの生成
			CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x + cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z - sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
			CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x - cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z + sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
			m_nParticleCounter++;											// 発生時間をカウントアップ
		}

		if (m_state == STATE_NORMAL)
		{
			if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_A) == true
				|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) < 0)
			{//左キーが押された
				m_move.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_rotDest.y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
				if (m_bJump == false)
				{
					m_pMotion->Set(MOTIONTYPE_MOVE);				// 移動モーション設定
				}
			}
			else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_D) == true
				|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) > 0)
			{//右キーが押された
				m_move.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_rotDest.y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
				if (m_bJump == false)
				{
					m_pMotion->Set(MOTIONTYPE_MOVE);				// 移動モーション設定
				}
			}
			else if (m_bJump == false)
			{
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);				// 待機モーション設定
			}

			if ((CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_SPACE) == true 
				|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0)) && m_bJump == false)
			{// SPACEキーが押された
				m_pMotion->Set(MOTIONTYPE_JUMP);				// ジャンプモーション

				// サウンドの再生
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);

				m_move.y = JUMP_PLAYER;
				m_bJump = true;
			}
		}
	}

	if (m_state == STATE_NORMAL || m_state == STATE_DAMAGE)
	{
		// カメラ距離を一定に設定
		m_fLenthCamera = LENTH_NORMAL;
		CManager::GetInstance()->GetCamera()->ScalingLenth(m_fLenthCamera, 0.15f);
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// 通常
		m_fSpeed = MOVE_PLAYER;
		break;

	case STATE_DAMAGE:		// ダメージ
		if (m_nStateCounter < 0)
		{
			m_state = STATE_NORMAL;
			m_bInvincible = true;
			m_nInvincibleCounter = 120;				// 状態カウンターを設定
		}
		break;

	case STATE_INVINCIBLE:	// 無敵
		break;

	case STATE_DEATH:		// 死亡
		break;
	}

	if (m_bInvincible == true)
	{
		if ((m_nInvincibleCounter % 10) == 0)
		{
			m_bDisp = m_bDisp ? false : true;
		}

		if (m_nInvincibleCounter < 0)
		{
			m_bInvincible = false;
			m_bDisp = true;
		}
	}

	m_fRotDiff = m_rotDest.y - m_rot.y;	// 目的の向きまでの差分

	// 角度の値の補正
	if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * ROT_DIFF_PI;
	}

	m_rot.y += m_fRotDiff * ROT_CAMERA_DIFF;	// 角度の補正

	// 角度の値の補正
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * ROT_DIFF_PI;
	}

	// 重力処理
	m_move.y -= fGravity;

	// 位置を更新
	m_pos += m_move;

	// 3Dポリゴンとの当たり判定
	//m_pos.y = CManager::GetObject3D()->CollisionVec(m_pos);

	// 移動量を更新（減衰させる）
	m_move.x += (0.0f - m_move.x) * MOVE_MINUS;
	m_move.z += (0.0f - m_move.z) * MOVE_MINUS;

	//if (m_pos.y < LAND_POS && CManager::GetMode() == CScene::MODE_TUTORIAL)
	//{// 着地した
	//	m_pos.y = LAND_POS;
	//	m_move.y = 0.0f;
	//	m_bJump = false;
	//	m_bAirJump = false;
	//}

	// チュートリアル画面の画面範囲設定
	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		if (m_pos.z < -650.0f)
		{
			m_pos.z = -650.0f;
		}
		if (m_pos.z > 900.0f)
		{
			m_pos.z = 900.0f;
		}
	}
	else if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (m_pos.z < -500.0f)
		{
			m_pos.z = -500.0f;
		}
		if (m_pos.z > 600.0f)
		{
			m_pos.z = 600.0f;
		}
	}

	// モーションの更新処理
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update();
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_LB, 0)
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_RB, 0))
	{
		m_magnet++;

		if (m_magnet >= EMAGNET_MAX)
		{
			m_magnet = EMAGNET_NONE;
		}
	}

	// 地形との当たり判定
	CItem::CollisionItem(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin);
	if (CObjectX::CollisionModel(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin) == true)
	{// 着地している
		SetState(STATE_NORMAL);
		SetJump(false);			// ジャンプフラグをリセット
	}
	else
	{
		m_bJump = true;
	}
	CMagnet::CollisionModel(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin);

	// 影の位置の設定
	SetPosShadow();

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(" 移動          ：A D\n");
	CManager::GetInstance()->GetDebugProc()->Print(" ジャンプ      ：SPACE\n");
	CManager::GetInstance()->GetDebugProc()->Print(" 磁力切り替え  ：ENTER\n");
	CManager::GetInstance()->GetDebugProc()->Print(" リザルトへ    ：BACKSPACE\n\n");

	CManager::GetInstance()->GetDebugProc()->Print(" プレイヤーの位置：（%f, %f, %f）\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print(" プレイヤーの移動量：（%f, %f, %f）\n", m_move.x, m_move.y, m_move.z);
	CManager::GetInstance()->GetDebugProc()->Print(" プレイヤーの移動速度：%f\n", m_fSpeed);
	CManager::GetInstance()->GetDebugProc()->Print(" プレイヤーの向き：%f\n\n", m_rot.y);
	
	CManager::GetInstance()->GetDebugProc()->Print(" カメラの向き：%f\n", CManager::GetInstance()->GetCamera()->GetRot().y);
	CManager::GetInstance()->GetDebugProc()->Print(" 視点：（%f, %f, %f）\n", CManager::GetInstance()->GetCamera()->GetPosV().x, CManager::GetInstance()->GetCamera()->GetPosV().y, CManager::GetInstance()->GetCamera()->GetPosV().z);
	CManager::GetInstance()->GetDebugProc()->Print(" 注視点：（%f, %f, %f）\n\n", CManager::GetInstance()->GetCamera()->GetPosR().x, CManager::GetInstance()->GetCamera()->GetPosR().y, CManager::GetInstance()->GetCamera()->GetPosR().z);
}

//===============================================
// 描画処理
//===============================================
void CPlayer::Draw(void)
{
	if (m_bDisp == true)
	{// タイトルじゃない
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

		// モデルの描画（全パーツ）
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			// モデルの描画処理
			switch (m_magnet)
			{
			case EMAGNET_NONE:
				m_apModel[nCntModel]->Draw();
				break;

			case EMAGNET_N:
				m_apModel[nCntModel]->SetCol(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
				break;

			case EMAGNET_S:
				m_apModel[nCntModel]->SetCol(D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f));
				break;
			}

			// モデルの影の描画処理
			m_apModel[nCntModel]->DrawShadowmtx(m_posShadow.y);
		}
	}
}

//===============================================
// 死亡直後の処理
//===============================================
void CPlayer::Death(void)
{
	SetState(CPlayer::STATE_DEATH);
	CManager::GetInstance()->AddCountDeath(CManager::GetMode());			// 死亡回数をカウント
	CRenderer::GetFade()->Set(CScene::MODE_GAME);							// リザルト画面へ移動
	//Reset();	// リセット
	m_bDisp = false;

	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		CParticle::Create()->Set(m_pos, CParticle::TYPE_BULLET);
		int nTime = CGame::GetTime()->Get();
		CGame::SetTime(nTime);		// 時間の設定
	}
}

//===============================================
// オブジェクトXとの当たり判定
//===============================================
void CPlayer::CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPos->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPos->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPos->z + vtxMin.z <= m_pos.z - m_vtxMin.z && pPos->z + vtxMax.z >= m_pos.z + m_vtxMin.z
		&& pPos->y + vtxMin.y <= m_pos.y + m_vtxMax.y && pPos->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
	{// 範囲内にある
		if (pPosOld->y + vtxMax.y <= m_posOld.y + m_vtxMin.y
			&& pPos->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
		{// 上からめり込んだ
			// 上にのせる
			m_pos.y = pPosOld->y - m_vtxMin.y + vtxMax.y;

			if (m_state != STATE_DAMAGE)
			{// ヒップドロップ中
				SetState(STATE_NORMAL);
			}
		}
		else if (pPosOld->y + vtxMin.y >= m_posOld.y + m_vtxMax.y
			&& pPos->y + vtxMin.y <= m_pos.y + m_vtxMax.y)
		{// 下からめり込んだ
			// 下に戻す
			m_pos.y = pPosOld->y - m_vtxMax.y + vtxMin.y;
			m_move.y = 0.0f;
		}
		else if (pPosOld->z + vtxMin.z >= m_posOld.z - m_vtxMin.z
			&& pPos->z + vtxMin.z <= m_pos.z - m_vtxMin.z)
		{// 左から右にめり込んだ
			// 位置を戻す
			m_pos.z = pPosOld->z + m_vtxMin.z + vtxMin.z;
		}
		else if (pPosOld->z + vtxMax.z <= m_posOld.z + m_vtxMin.z
			&& pPos->z + vtxMax.z >= m_pos.z + m_vtxMin.z)
		{// 右から左にめり込んだ
			// 位置を戻す
			m_pos.z = pPosOld->z - m_vtxMin.z + vtxMax.z;
		}
	}
}

//===============================================
// 敵との当たり判定
//===============================================
void CPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPosOld->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPosOld->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPosOld->z + vtxMin.z - m_vtxMax.z <= m_pos.z && pPosOld->z + vtxMax.z - m_vtxMin.z >= m_pos.z
		&& pPosOld->y + vtxMin.y - m_vtxMax.y <= m_pos.y && pPosOld->y + vtxMax.y - m_vtxMin.y >= m_pos.y)
	{// 範囲内にある
		if (m_state != STATE_DAMAGE && m_bInvincible == false)
		{// 敵に当たった
			Death();
		}
	}
}

//===============================================
// 位置の設定
//===============================================
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// 移動量の設定
//===============================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// ジャンプしたかの設定
//===============================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//===============================================
// サイズの設定
//===============================================
void CPlayer::SetSize(D3DXVECTOR3 size)
{
	m_vtxMax = size;
}

//===============================================
// サイズの設定
//===============================================
void CPlayer::SetSizeMin(D3DXVECTOR3 size)
{
	m_vtxMin = size;
}

//===============================================
// 状態の設定
//===============================================
void CPlayer::SetState(EState state)
{
	if (state != STATE_DAMAGE && m_state == STATE_NORMAL)
	{// ヒップドロップ中以外で地形へ着地
		m_move.y = 0.0f;
		SetJump(false);
		m_state = state;
	}

	if (m_state != STATE_DAMAGE && state == STATE_DAMAGE && m_bInvincible == false)
	{// 敵に当たった
		m_pMotion->Set(MOTIONTYPE_DAMAGE);
		m_nStateCounter = 30;
		m_move.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * m_rot.y)) * 10.0f;
		m_state = state;
	}

	if (state != STATE_DAMAGE)
	{
		m_state = state;
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// 通常
		break;

	case STATE_DAMAGE:		// ダメージ
		break;

	case STATE_INVINCIBLE:	// 無敵
		break;

	case STATE_DEATH:		// 死亡
		break;
	}
}

//===============================================
// モーションの設定
//===============================================
void CPlayer::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}

//===============================================
// 影の位置の設定
//===============================================
void CPlayer::SetPosShadow(void)
{
	float fPos = -500.0f;
	m_posShadow.y = fPos;

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
			{// 地形
				if (pos.x + sizeMin.x - m_vtxMax.x <= m_pos.x && pos.x + sizeMax.x - m_vtxMin.x >= m_pos.x
					&& pos.z + sizeMin.z <= m_pos.z - m_vtxMin.z && pos.z + sizeMax.z >= m_pos.z + m_vtxMin.z)
				{// 範囲内にある
					if (m_pos.y > pos.y)
					{// プレイヤーより下の位置
						if (fPos <= pos.y)
						{
							fPos = pos.y;
							m_posShadow.y = pos.y + sizeMax.y + 10.0f;
						}
					}
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}
}

//===============================================
// シングルトン
//===============================================
CPlayer* CPlayer::GetInstance(void)
{
	if (m_pPlayer == nullptr)
	{
		return m_pPlayer = new CPlayer(m_nPriority);
	}
	else
	{
		return m_pPlayer;
	}
}