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
#include "bullet.h"
#include "camera.h"
#include "object.h"
#include "particle.h"
#include "model.h"
#include "meshfield.h"
#include "motion.h"
#include "fileload.h"
#include "shadow.h"
#include "object3D.h"
#include "garbage.h"
#include "game.h"
#include "tutorial.h"
#include "time.h"
#include "target.h"
#include "sound.h"
#include "title.h"

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

// フィーバー
#define MOVE_FEVER			(0.6f)		// 移動速度（フィーバー）

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
	m_speed = 0.0f;
	m_bJump = true;
	m_fRotBullet = 0.0f;
	m_turnType = TURN_NONE;
	m_turnTypeOld = TURN_NONE;
	m_nTurnCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_nParticleCounter = 0;
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
	m_speed = 0.0f;
	m_bJump = true;
	m_fRotBullet = 0.0f;
	m_turnType = TURN_NONE;
	m_turnTypeOld = TURN_NONE;
	m_nTurnCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_nParticleCounter = 0;
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

	// プレイヤーの生成
	pPlayer = new CPlayer(nPriority);

	// 種類の設定
	pPlayer->SetType(CObject::TYPE_PLAYER);
	
	// 初期化処理
	pPlayer->Init(pos);

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
	m_nNumModel = CManager::GetLoad()->GetNumModel();

	// 位置の設定
	m_pos = pos;

	// 移動速度の初期化
	m_speed = MOVE_PLAYER;

	// パーティクル発生防止
	m_nParticleCounter = COUNT_PARTICLE;

	const char *apModelFile[MAX_MODEL];		// モデルファイル名

	// モデルの生成（全パーツ）
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		apModelFile[nCntModel] = CManager::GetLoad()->GetFileName(nCntModel);		// ファイル名取得
		pos = CManager::GetLoad()->GetPos(nCntModel);								// 位置の取得
		rot = CManager::GetLoad()->GetRot(nCntModel);								// 向きの取得

		m_apModel[nCntModel] = CModel::Create(apModelFile[nCntModel], pos, rot);	// 生成
	}
	//m_apModel[0] = CModel::Create("data\\MODEL\\GarbageCan_Wood.x", pos, D3DXVECTOR3(0.0f, 0.0f , 0.0f));	// 生成

	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// 親モデルの設定（全パーツ）
	m_apModel[0]->SetParent(NULL);
	for (int nCntModel = 1; nCntModel < m_nNumModel; nCntModel++)
	{
		int nParent = 0;

		nParent = CManager::GetLoad()->GetParent(nCntModel);	// 親を取得

		m_apModel[nCntModel]->SetParent(m_apModel[nParent]);
	}

	// モーション情報の設定
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{// モーション数分繰り返す
		m_pMotion->Set(nCntMotion);
		m_pMotion->SetInfo(CManager::GetLoad()->GetInfo(nCntMotion));
	}

	// 初期モーション設定
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

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
		if (m_apModel[nCntModel] != NULL)
		{// 使用されている
			// モデルの終了処理
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// モーションへのポインタの破棄
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	this->Release();
}

//===============================================
// 更新処理
//===============================================
void CPlayer::Update(void)
{
#if _DEBUG
	// ゴミを配置
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_0) == true)
	{// ENTERキーが押された
		CGarbage::Create(m_pos, 3);
		CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f,
			m_pos.y, m_pos.z + cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f), 3);
		CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f,
			m_pos.y, m_pos.z + cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f), 3);
	}

	// 移動速度変更
	if (CManager::GetKeyboardInput()->GetPress(DIK_O) == true || CManager::GetInputGamePad()->GetJoyStickRY(0) > 0)
	{// Oキーが押された
		m_speed += 0.01f;
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_L) == true || CManager::GetInputGamePad()->GetJoyStickRY(0) < 0)
	{// Lキーが押された
		m_speed -= 0.01f;
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_8) == true || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LS, 0))
	{// Lキーが押された
		m_speed = MOVE_PLAYER;
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_9) == true || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RS, 0))
	{// Lキーが押された
		m_speed = MOVE_FEVER;
	}
#endif

	// 前回の位置を保存
	m_posOld = m_pos;

	// 方向転換
	switch (m_turnType)
	{
	case TURN_NONE:		// 曲がらない
		m_move.x += sinf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rot.y = D3DX_PI + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		// 位置補正処理
		if (m_turnTypeOld != TURN_NONE)
		{
			if (CManager::GetMode() == CScene::MODE_TUTORIAL)
			{
				CTutorial::GetTarget()->RevisionPos();
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetTarget()->RevisionPos();
				m_nTurnCounter++;

				if (m_nTurnCounter >= 20)
				{
					m_turnTypeOld = TURN_NONE;
				}
			}
			else if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				//CTitle::GetTarget()->RevisionPos();
				m_nTurnCounter++;

				if (m_nTurnCounter >= 20)
				{
					m_turnTypeOld = TURN_NONE;
				}
			}
		}

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{// タイトルじゃない
			// エフェクトの生成（煙）
			CParticle::Create(1)->Set(m_pos, CParticle::TYPE_MOVE);
		}

		if (m_pMotion->IsFinish() == true || m_pMotion->GetType() == MOTIONTYPE_NEUTRAL)
		{
			// モーション設定
			m_pMotion->Set(MOTIONTYPE_MOVE);
		}
		break;

	case TURN_RIGHT:	// 右へ曲がる
		m_move.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rotDest.y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		m_nParticleCounter = 0;		// パーティクル発生させる
		m_turnTypeOld = m_turnType;	// 前回の情報を保存
		m_nTurnCounter = 0;			// カウンタをリセット
		m_turnType = TURN_NONE;		// 曲がらない設定にする

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{
			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_CURVE);
		}
		break;

	case TURN_LEFT:		// 左へ曲がる
		m_move.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rotDest.y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		m_nParticleCounter = 0;		// パーティクル発生させる
		m_turnTypeOld = m_turnType;	// 前回の情報を保存
		m_nTurnCounter = 0;			// カウンタをリセット
		m_turnType = TURN_NONE;		// 曲がらない設定にする

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{
			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_CURVE);
		}
		break;

	case TURN_BACK:		// 折り返し
		m_move.x += sinf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rotDest.y = D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		m_nParticleCounter = 0;		// パーティクル発生させる
		m_turnTypeOld = m_turnType;	// 前回の情報を保存
		m_nTurnCounter = 0;			// カウンタをリセット
		m_turnType = TURN_NONE;		// 曲がらない設定にする

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{
			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_CURVE);
		}
		break;
	}

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// ゲームモード
		if (CGame::GetTime()->Get() == TIME_FEVER)
		{// フィーバータイム
			m_speed = MOVE_FEVER;		// 速度を上げる
		}
	}

	if (m_speed == MOVE_FEVER)
	{// フィーバー
		m_nParticleCounter = 0;		// パーティクル発生させる
	}

	if (m_nParticleCounter < COUNT_PARTICLE && CManager::GetMode() != CScene::MODE_TITLE)
	{// パーティクル発生時間に達していない
		// パーティクルの生成
		CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x + cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z - sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
		CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x - cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z + sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
		m_nParticleCounter++;											// 発生時間をカウントアップ
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
	m_move.y -= MOVE_GRAVITY;

	// 位置を更新
	m_pos += m_move;

	// 3Dポリゴンとの当たり判定
	//m_pos.y = CManager::GetObject3D()->CollisionVec(m_pos);

	// 移動量を更新（減衰させる）
	m_move.x += (0.0f - m_move.x) * MOVE_MINUS;
	m_move.z += (0.0f - m_move.z) * MOVE_MINUS;

	if (m_pos.y < LAND_POS)
	{// 着地した
		m_pos.y = LAND_POS;
		m_move.y = 0.0f;
	}

	// モーションの更新処理
	m_pMotion->Update();

	// デバッグ表示
	CManager::GetDebugProc()->Print(" [Q E]：移動方向変更\n");
	CManager::GetDebugProc()->Print(" [SPACE]：捨てる\n");
	CManager::GetDebugProc()->Print(" [← ↑ →]：分別\n");
	CManager::GetDebugProc()->Print(" [A D]：ターゲット移動\n\n");

	CManager::GetDebugProc()->Print(" [ENTER]：ゴミの生成\n");
	CManager::GetDebugProc()->Print(" [O L]：移動速度変更\n\n");

	CManager::GetDebugProc()->Print(" プレイヤーの位置：（%f, %f, %f）\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetDebugProc()->Print(" プレイヤーの移動速度：%f\n", m_speed);
	CManager::GetDebugProc()->Print(" プレイヤーの向き：%f\n\n", m_rot.y);
	
	CManager::GetDebugProc()->Print(" カメラの向き：%f\n", CManager::GetCamera()->GetRot().y);
	CManager::GetDebugProc()->Print(" 視点：（%f, %f, %f）\n", CManager::GetCamera()->GetPosV().x, CManager::GetCamera()->GetPosV().y, CManager::GetCamera()->GetPosV().z);
	CManager::GetDebugProc()->Print(" 注視点：（%f, %f, %f）\n", CManager::GetCamera()->GetPosR().x, CManager::GetCamera()->GetPosR().y, CManager::GetCamera()->GetPosR().z);
	CManager::GetDebugProc()->Print(" ゴミの総数：%d\n", CManager::GetGarbage()->GetNumAll());
	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		CManager::GetDebugProc()->Print(" 床の総数：%d\n", CGame::GetMeshField()->GetNumAll());
	}
}

//===============================================
// 描画処理
//===============================================
void CPlayer::Draw(void)
{
	if (CManager::GetMode() != CScene::MODE_TITLE)
	{// タイトルじゃない
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
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
			m_apModel[nCntModel]->Draw();

			// モデルの影の描画処理
			m_apModel[nCntModel]->DrawShadowmtx();
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
// カーブの設定
//===============================================
void CPlayer::SetTurn(TURN turn)
{
	m_turnType = turn;
}

//===============================================
// モーションの設定
//===============================================
void CPlayer::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}