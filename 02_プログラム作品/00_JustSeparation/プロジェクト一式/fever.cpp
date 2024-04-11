//=========================================================
//
// フィーバー描画処理 [fever.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "fever.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//===============================================
// マクロ定義
//===============================================
#define TEXTURE		"data\\TEXTURE\\fever000.png"	// UIのテクスチャ
#define SPEED_MOVE	(0.08f)							// 移動速度
#define SIZE_X		(287.0f)						// 横サイズ
#define SIZE_Y		(61.0f)							// 縦サイズ

//===============================================
// 静的メンバ変数
//===============================================
int CFever::m_IdxTexture = 0;				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CFever::CFever() : CObject2D(0)
{
	// 値のクリア
	m_bEndAnim = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CFever::CFever(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_bEndAnim = false;
}

//===============================================
// デストラクタ
//===============================================
CFever::~CFever()
{
	
}

//===============================================
// 生成処理
//===============================================
CFever *CFever::Create(int nPriority)
{
	CFever *pUi;

	// 背景の生成
	pUi = new CFever(nPriority);

	// 種類の設定
	pUi->SetType(TYPE_BG);

	// サイズの設定
	pUi->SetSize(SIZE_X, SIZE_Y);

	// 初期化処理
	pUi->Init(D3DXVECTOR3(-SIZE_X, 250.0f, 0.0f));

	// テクスチャの割り当て
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CFever::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_IdxTexture = CManager::GetTexture()->Regist(TEXTURE);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CFever::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CFever::Update(void)
{
	// 位置補正処理
	if (m_bEndAnim == false && CObject2D::RevisionPos(D3DXVECTOR3(620.0f, 250.0f, 0.0f), SPEED_MOVE, true) == true)
	{// 位置補正終了
		m_bEndAnim = true;		// アニメーション終了
	}
	else if (m_bEndAnim == true)
	{
		CObject2D::RevisionPos(D3DXVECTOR3(SCREEN_WIDTH + SIZE_X, 250.0f, 0.0f), SPEED_MOVE, true);
	}
}

//===============================================
// 描画処理
//===============================================
void CFever::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}