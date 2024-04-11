//=========================================================
//
// ターゲットUI処理 [uitarget.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "uitarget.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectBillboard.h"
#include "game.h"
#include "input.h"
#include "target.h"
#include "player.h"
#include "tutorial.h"

//===============================================
// マクロ定義
//===============================================
#define SIZE_X			(38.0f)						// 横サイズ
#define SIZE_Y			(38.0f)						// 縦サイズ
#define DISTANCE_CENTER	(80.0f)						// 中心からの距離
#define MOVE_SPEED		(0.4f)						// 移動スピード

//===============================================
// 静的メンバ変数
//===============================================
CObjectBillboard *CUiTarget::m_apObjectBillboard[TYPE_MAX];	// オブジェクトビルボードクラスのポインタ
const char *CUiTarget::m_apTextureData[TYPE_MAX] = {};		// テクスチャファイル名
int CUiTarget::m_aIdxTexture[TYPE_MAX] = {};				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CUiTarget::CUiTarget()
{
	// 値のクリア
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CUiTarget::CUiTarget(int nPriority)
{
	// 値のクリア
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// デストラクタ
//===============================================
CUiTarget::~CUiTarget()
{
	
}

//===============================================
// 生成処理
//===============================================
CUiTarget *CUiTarget::Create(int nIdx, int nPriority)
{
	CUiTarget *pUi;

	// 背景の生成
	pUi = new CUiTarget(nPriority);

	// 初期化処理
	pUi->Init(D3DXVECTOR3(600.0f, 300.0f, 0.0f), nPriority);

	return pUi;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CUiTarget::Init(D3DXVECTOR3 pos, int nPriority)
{
	// テクスチャファイル名
	const char *m_apTextureData[TYPE_MAX] =
	{
		{ "data\\TEXTURE\\ui005.png" },	// 燃えるゴミ
		{ "data\\TEXTURE\\ui006.png" },	// 燃えないゴミ
		{ "data\\TEXTURE\\ui007.png" }	// 資源ゴミ
	};

	D3DXVECTOR3 TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ターゲットの位置を取得
	float fPlayerRot = 0.0f;								// プレイヤーの向きを取得

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		TargetPos = CTutorial::GetTarget()->GetPos();
		fPlayerRot = CTutorial::GetPlayer()->GetRot().y;
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		TargetPos = CGame::GetTarget()->GetPos();
		fPlayerRot = CGame::GetPlayer()->GetRot().y;
	}

	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_apObjectBillboard[nCntObj] = new CObjectBillboard(nPriority);

		// サイズ設定処理
		m_apObjectBillboard[nCntObj]->SetSize(SIZE_X, SIZE_Y);

		// オブジェクトビルボードの初期化処理
		m_apObjectBillboard[nCntObj]->Init(TargetPos);

		// テクスチャの設定
		m_aIdxTexture[nCntObj] = CManager::GetTexture()->Regist(m_apTextureData[nCntObj]);

		// テクスチャの割り当て
		m_apObjectBillboard[nCntObj]->BindTexture(m_aIdxTexture[nCntObj]);
	}

	// ローカル位置を設定
	m_aPos[TYPE_BURN] = D3DXVECTOR3(TargetPos.x + cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z - sinf(fPlayerRot) * DISTANCE_CENTER);
	m_aPos[TYPE_NONFLAMMABLE] = D3DXVECTOR3(TargetPos.x, TargetPos.y + DISTANCE_CENTER, TargetPos.z);
	m_aPos[TYPE_RECYCLABLE] = D3DXVECTOR3(TargetPos.x - cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z + sinf(fPlayerRot) * DISTANCE_CENTER);

	// オブジェクトビルボードの初期化処理
	m_apObjectBillboard[TYPE_BURN]->SetPos(m_aPos[TYPE_BURN]);
	m_apObjectBillboard[TYPE_NONFLAMMABLE]->SetPos(m_aPos[TYPE_NONFLAMMABLE]);
	m_apObjectBillboard[TYPE_RECYCLABLE]->SetPos(m_aPos[TYPE_RECYCLABLE]);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CUiTarget::Uninit(void)
{
	
}

//===============================================
// 更新処理
//===============================================
void CUiTarget::Update(void)
{
	bool bKeyBurn = false;			// 燃えるゴミのキー入力されたか
	bool bKeyNonflammable = false;	// 燃えないゴミのキー入力されたか
	bool bKeyPlastic = false;		// 資源ゴミのキー入力されたか

	D3DXVECTOR3 TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ターゲットの位置を取得
	float fPlayerRot = 0.0f;								// プレイヤーの向きを取得

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		TargetPos = CTutorial::GetTarget()->GetPos();		// ターゲットの位置を取得
		fPlayerRot = CTutorial::GetPlayer()->GetRot().y;	// プレイヤーの向きを取得
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		TargetPos = CGame::GetTarget()->GetPos();		// ターゲットの位置を取得
		fPlayerRot = CGame::GetPlayer()->GetRot().y;	// プレイヤーの向きを取得
	}

	// 位置をターゲットの周りに固定
	m_apObjectBillboard[TYPE_BURN]->SetPos(m_aPos[TYPE_BURN]);
	m_apObjectBillboard[TYPE_NONFLAMMABLE]->SetPos(m_aPos[TYPE_NONFLAMMABLE]);
	m_apObjectBillboard[TYPE_RECYCLABLE]->SetPos(m_aPos[TYPE_RECYCLABLE]);
	
	if (CManager::GetKeyboardInput()->GetPress(DIK_LEFT) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_X, 0) == true)
	{// 左キーが押された
		// 燃えるゴミ
		// 目的の位置までの差分を計算
		D3DXVECTOR3 posDest = TargetPos - m_aPos[TYPE_BURN];

		// 位置の補正
		m_aPos[TYPE_BURN].x += posDest.x * MOVE_SPEED;
		m_aPos[TYPE_BURN].y += posDest.y * MOVE_SPEED;
		m_aPos[TYPE_BURN].z += posDest.z * MOVE_SPEED;

		bKeyBurn = true;	// 燃えるゴミのキー入力された
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_UP) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_Y, 0) == true)
	{// 上キーが押された
		// 燃えないゴミ
		// 目的の位置までの差分を計算
		D3DXVECTOR3 posDest = TargetPos - m_aPos[TYPE_NONFLAMMABLE];

		// 位置の補正
		m_aPos[TYPE_NONFLAMMABLE].x += posDest.x * MOVE_SPEED;
		m_aPos[TYPE_NONFLAMMABLE].z += posDest.z * MOVE_SPEED;
		m_aPos[TYPE_NONFLAMMABLE].y += posDest.y * MOVE_SPEED;

		bKeyNonflammable = true;	// 燃えないゴミのキー入力された
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_RIGHT) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_B, 0) == true)
	{// 右キーが押された
		// 資源ゴミ
		// 目的の位置までの差分を計算
		D3DXVECTOR3 posDest = TargetPos - m_aPos[TYPE_RECYCLABLE];

		// 位置の補正
		m_aPos[TYPE_RECYCLABLE].x += posDest.x * MOVE_SPEED;
		m_aPos[TYPE_RECYCLABLE].y += posDest.y * MOVE_SPEED;
		m_aPos[TYPE_RECYCLABLE].z += posDest.z * MOVE_SPEED;

		bKeyPlastic = true;		// 資源ゴミのキー入力された
	}

	if (bKeyBurn == false)
	{// ターゲットが燃えるゴミの状態じゃない
		// 位置を元に戻す
		m_aPos[TYPE_BURN] = D3DXVECTOR3(TargetPos.x + cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z - sinf(fPlayerRot) * DISTANCE_CENTER);
	}
	if (bKeyNonflammable == false
	|| (bKeyNonflammable == true && bKeyBurn == true))
	{// ターゲットが燃えないゴミの状態じゃない
		// 位置を元に戻す
		m_aPos[TYPE_NONFLAMMABLE] = D3DXVECTOR3(TargetPos.x, TargetPos.y + DISTANCE_CENTER, TargetPos.z);
	}
	if (bKeyPlastic == false
		|| (bKeyPlastic == true && (bKeyBurn == true || bKeyNonflammable == true)))
	{// ターゲットが資源ゴミの状態じゃない
		// 位置を元に戻す
		m_aPos[TYPE_RECYCLABLE] = D3DXVECTOR3(TargetPos.x - cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z + sinf(fPlayerRot) * DISTANCE_CENTER);
	}
}

//===============================================
// 描画処理
//===============================================
void CUiTarget::Draw(void)
{
	
}