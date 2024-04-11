//=========================================================
//
// カメラ処理 [camera.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "object.h"
#include "game.h"
#include "debugproc.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE_CAMERA			(2.0f)		// 移動量
#define RAD_CAMERA			(60.0f)		// 基本軸からの回転角度
#define SLANTING			(0.25f)		// 斜めの角度
#define MOVE_X				(0.5f)		// X方向への移動
#define PI_SLASH			(100.0f)	// IPを割る数
#define LENGTH_CAMERA		(-300.0f)	// 視点の距離
#define HIGHT_CAMERA		(250.0f)	// カメラの高さ
#define MOVEV_MINUS			(0.3f)		// 視点の移動量の減衰
#define MOVER_MINUS			(0.2f)		// 注視点の移動量の減衰
#define POSR_DEST			(-300.0f)	// 注視点の少し前に設定
#define CAMERA_ROT_SPEED	(0.7f)		// カメラがプレイヤーの背後に回り込むまでのスピード
#define CAMERA_ROT_MOVE		(70)		// 向きが変わるタイミング
#define POS_DIFF			(0.15f)		// 位置の値の補正
#define MAPVIEW_TIME		(80)		// 上空カメラになっている時間

//===============================================
// コンストラクタ
//===============================================
CCamera::CCamera()
{
	// 値のクリア
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nViewMapCounter = 0;
	m_bMapCamera = false;
}

//===============================================
// デストラクタ
//===============================================
CCamera::~CCamera()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CCamera::Init(void)
{
	// 値を初期化
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		m_posV = D3DXVECTOR3(-2500.0f, 7400.0f, -1800.0f);
		m_posR = D3DXVECTOR3(-2500.0f, 0.0f, -2500.0f);
	}
	else if (CScene::GetMode() == CScene::MODE_TUTORIAL || CScene::GetMode() == CScene::MODE_TITLE)
	{
		m_posV = D3DXVECTOR3(0.0f, HIGHT_CAMERA, 300.0f);
		m_posR = D3DXVECTOR3(0.0f, 0.0f, -400.0f);
	}

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_nViewMapCounter = MAPVIEW_TIME;
	m_bMapCamera = true;

	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カメラの追従
	// 目的の視点・注視点を設定
	m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
	m_posRDest.y = playerPos.y;
	m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
	m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
	m_posVDest.y = playerPos.y;
	m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CCamera::Uninit(void)
{
	
}

//===============================================
// 更新処理
//===============================================
void CCamera::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの位置
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤーの前回の位置
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの向き

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while(pObject != NULL)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得

			if (type == CObject::TYPE_PLAYER)
			{// プレイヤー
				playerPos = pObject->GetPos();			// 位置を取得
				playerPosOld = pObject->GetPosOld();	// 前回の位置を取得
				playerRot = pObject->GetRot();			// 向きを取得
			}

			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetStateReady() == true && CGame::GetPauseState() == false)
		{//	待機状態
			if (m_nViewMapCounter < 0)
			{// カウンタが残っている
				D3DXVECTOR3 posV = D3DXVECTOR3(0.0f, HIGHT_CAMERA, 300.0f);
				D3DXVECTOR3 posR = D3DXVECTOR3(0.0f, 0.0f, -400.0f);

				// 目的の位置までの差分を計算
				D3DXVECTOR3 fPosVDest = posV - m_posV;
				D3DXVECTOR3 fPosRDest = posR - m_posR;

				// 位置の補正
				m_posV += fPosVDest* POS_DIFF;
				m_posR += fPosRDest* POS_DIFF;
			}
			else
			{
				m_nViewMapCounter--;	// カウンタを減らす
			}
		}
		else
		{//	待機状態じゃない
			if (m_bMapCamera == true)
			{// マップカメラ
				D3DXVECTOR3 posV = D3DXVECTOR3(0.0f, HIGHT_CAMERA, 300.0f);
				D3DXVECTOR3 posR = D3DXVECTOR3(0.0f, 0.0f, -400.0f);
				m_bMapCamera = false;
			}

			if (CGame::GetPauseState() == true && CGame::GetPauseCamera() == true)
			{// ポーズ状態
				// 視点操作
				if (CManager::GetKeyboardInput()->GetPress(DIK_Y) == true)
				{
					// 上移動
					m_posV.y += MOVE_CAMERA;
				}
				else if (CManager::GetKeyboardInput()->GetPress(DIK_N) == true)
				{
					// 下移動
					m_posV.y -= MOVE_CAMERA;
				}

				// 注視点操作
				if (CManager::GetKeyboardInput()->GetPress(DIK_T) == true)
				{
					// 上移動
					m_posR.y += MOVE_CAMERA;
				}
				else if (CManager::GetKeyboardInput()->GetPress(DIK_B) == true)
				{
					// 下移動
					m_posR.y -= MOVE_CAMERA;
				}

				// カメラ位置リセット
				if (CManager::GetKeyboardInput()->GetTrigger(DIK_F2) == true)
				{
					m_posV.y = HIGHT_CAMERA;
					m_posR.y = 0.0f;
				}
			}

			if (CGame::GetPauseState() == false)
			{
				// カメラの追従
				// 目的の視点・注視点を設定
				m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
				m_posRDest.y = playerPos.y;
				m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
				m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
				m_posVDest.y = playerPos.y;
				m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

				// 移動量を更新（減衰させる）
				m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
				m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
				m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
				m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;

				m_rotDest.y = D3DX_PI + (ROT_CAMERA * playerRot.y);

				m_rotDest.y -= m_rot.y;		// 目的の向きまでの差分

				// 角度の値の補正
				if (m_rotDest.y > D3DX_PI)
				{
					m_rotDest.y += -D3DX_PI * ROT_DIFF_PI;
				}
				else if (m_rotDest.y < -D3DX_PI)
				{
					m_rotDest.y += D3DX_PI * ROT_DIFF_PI;
				}

				m_rot.y += m_rotDest.y * CAMERA_ROT_SPEED;	// 角度の補正

				// 角度の値の補正
				if (m_rot.y > D3DX_PI)
				{
					m_rot.y += -D3DX_PI * ROT_DIFF_PI;
				}
				else if (m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI * ROT_DIFF_PI;
				}
			}
		}
	}
	else if (CScene::GetMode() == CScene::MODE_TUTORIAL)
	{
		// カメラの追従
		// 目的の視点・注視点を設定
		m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
		m_posRDest.y = playerPos.y;
		m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
		m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
		m_posVDest.y = playerPos.y;
		m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

		// 移動量を更新（減衰させる）
		m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
		m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
		m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
		m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;

		m_rotDest.y = D3DX_PI + (ROT_CAMERA * playerRot.y);

		m_rotDest.y -= m_rot.y;		// 目的の向きまでの差分

		// 角度の値の補正
		if (m_rotDest.y > D3DX_PI)
		{
			m_rotDest.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rotDest.y < -D3DX_PI)
		{
			m_rotDest.y += D3DX_PI * ROT_DIFF_PI;
		}

		m_rot.y += m_rotDest.y * CAMERA_ROT_SPEED;	// 角度の補正

		// 角度の値の補正
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * ROT_DIFF_PI;
		}
	}
	if (CScene::GetMode() == CScene::MODE_TITLE)
	{
		// カメラの追従
		// 目的の視点・注視点を設定
		m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
		m_posRDest.y = playerPos.y;
		m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
		m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
		m_posVDest.y = playerPos.y;
		m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

		// 移動量を更新（減衰させる）
		m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
		m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
		m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
		m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;

		m_rotDest.y = D3DX_PI + (ROT_CAMERA * playerRot.y);

		m_rotDest.y -= m_rot.y;		// 目的の向きまでの差分

									// 角度の値の補正
		if (m_rotDest.y > D3DX_PI)
		{
			m_rotDest.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rotDest.y < -D3DX_PI)
		{
			m_rotDest.y += D3DX_PI * ROT_DIFF_PI;
		}

		m_rot.y += m_rotDest.y * CAMERA_ROT_SPEED;	// 角度の補正

													// 角度の値の補正
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * ROT_DIFF_PI;
		}
	}
}

//===============================================
// 設定処理
//===============================================
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		8000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
						&m_posV,
						&m_posR,
						&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}