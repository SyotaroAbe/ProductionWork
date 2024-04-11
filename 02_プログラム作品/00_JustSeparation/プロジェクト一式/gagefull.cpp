//=========================================================
//
// ゲージフル描画処理 [gagefull.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "gagefull.h"
#include "manager.h"
#include "texture.h"
#include "tutorial.h"
#include "game.h"
#include "target.h"
#include "uigage.h"

//===============================================
// マクロ定義
//===============================================
#define TEXTURE			"data\\TEXTURE\\gagefull.png"	// ヒントのテクスチャ
#define SIZE_X			(40.0f)							// 横サイズ
#define SIZE_Y			(40.0f)							// 縦サイズ
#define SCALING_SIZE	(30.0f)							// 縮小後のサイズ
#define ARRIVAL_TIME	(1200)							// 出現するまでの時間

//===============================================
// 静的メンバ変数
//===============================================
int CGageFull::m_nIdxTexture = 0;											// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CGageFull::CGageFull() : CObjectBillboard(3)
{
	// 値のクリア
	m_bMaxScaling = false;
	m_bMaxGage = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CGageFull::CGageFull(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	m_bMaxScaling = false;
	m_bMaxGage = false;
}

//===============================================
// デストラクタ
//===============================================
CGageFull::~CGageFull()
{
	
}

//===============================================
// 生成処理
//===============================================
CGageFull *CGageFull::Create(int nPriority)
{
	CGageFull *pGageFull;

	// オブジェクトの生成
	pGageFull = new CGageFull(nPriority);

	//// 種類の設定
	//pGageFull->SetType(TYPE_TARGET);

	// サイズの設定
	pGageFull->SetSize(SIZE_X, SIZE_Y);

	// 初期化処理
	pGageFull->Init();

	// テクスチャの割り当て
	pGageFull->BindTexture(m_nIdxTexture);

	return pGageFull;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CGageFull::Init()
{
	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		m_rot.y = CTutorial::GetTarget()->GetRot().y;	// ターゲットの向きを取得
		m_pos = CTutorial::GetTarget()->GetPos();		// ターゲットの位置
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		m_rot.y = CGame::GetTarget()->GetRot().y;	// ターゲットの向きを取得
		m_pos = CGame::GetTarget()->GetPos();		// ターゲットの位置
	}

	// ビルボードの初期化処理
	CObjectBillboard::Init(m_pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetTexture()->Regist(TEXTURE);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGageFull::Uninit(void)
{
	// ビルボードの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CGageFull::Update(void)
{
	float fScalingSize = 0.0f;

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		m_pos = CTutorial::GetTarget()->GetPos();
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		m_pos = CGame::GetTarget()->GetPos();
	}

	if (m_bMaxScaling == true)
	{
		fScalingSize = SCALING_SIZE;
	}
	else
	{
		fScalingSize = SIZE_X;
	}

	//// 拡大縮小処理
	//if (CObjectBillboard::ScalingSize(fScalingSize, 0.2f) == true)
	//{
	//	m_bMaxScaling = m_bMaxScaling ? false : true;		// サイズ切り替え
	//}

	//// 点滅処理
	//CObjectBillboard::Brightness(0.02f);
}

//===============================================
// 描画処理
//===============================================
void CGageFull::Draw(void)
{
	if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax() == true)
		|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax() == true)
		|| m_bMaxGage == true)
	{
		// ビルボードの描画処理
		CObjectBillboard::Draw();
	}
}

//===============================================
// 設定処理
//===============================================
void CGageFull::Set(const bool bMax)
{
	m_bMaxGage = bMax;
}