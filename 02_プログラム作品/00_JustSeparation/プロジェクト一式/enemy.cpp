//=========================================================
//
// プレイヤー処理 [enemy.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "bullet.h"
#include "texture.h"

//===============================================
// 静的メンバ変数
//===============================================
int CEnemy::m_nIdxTexture = 0;						// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CEnemy::CEnemy() : CObject2D(3)
{

}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CEnemy::CEnemy(int nPriority) : CObject2D(nPriority)
{
	
}

//===============================================
// デストラクタ
//===============================================
CEnemy::~CEnemy()
{

}

//===============================================
// 生成処理
//===============================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int nPriority)
{
	CEnemy *pEnemy;

	// 敵の生成
	pEnemy = new CEnemy(nPriority);

	// 種類の設定
	pEnemy->SetType(TYPE_ENEMY);

	// サイズの設定
	pEnemy->SetSize(ENEMY_SIZEX, ENEMY_SIZEY);

	// 初期化処理
	pEnemy->Init(pos);

	// テクスチャの割り当て
	pEnemy->BindTexture(m_nIdxTexture);

	return pEnemy;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\enemy000.png");

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CEnemy::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CEnemy::Update(void)
{
	
}

//===============================================
// 描画処理
//===============================================
void CEnemy::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}