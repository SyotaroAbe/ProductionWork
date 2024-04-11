//=========================================================
//
// ロックオン処理 [rockon.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "rockon.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "objectBillboard.h"
#include "texture.h"
#include "camera.h"
#include "player.h"
#include "debugproc.h"
#include "sound.h"

//===============================================
// マクロ定義
//===============================================
#define ROCKON_SIZEX		(300.0f)		// 幅（半分）
#define ROCKON_SIZEY		(300.0f)		// 高さ（半分）
#define DIFF_SIZE			(37.0f)			// 目的のサイズ

//===============================================
// 静的メンバ変数
//===============================================
int CRockon::m_nIdxTexture = 0;					// 使用するテクスチャの番号
int CRockon::m_nNumAll = 0;						// 使用しているターゲット総数
int CRockon::m_aObjRockon[MAX_ROCKON] = {};		// ターゲットしているオブジェクトの番号保存

//===============================================
// コンストラクタ
//===============================================
CRockon::CRockon() : CObjectBillboard(3)
{
	// 値のクリア
	m_localPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_separation = CGarbage::SEPARATION_NONE;
	m_nIDGarbage = 0;

	m_nNumAll++;	// 総数をカウントアップ
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CRockon::CRockon(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	m_localPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_separation = CGarbage::SEPARATION_NONE;
	m_nIDGarbage = 0;

	m_nNumAll++;	// 総数をカウントアップ
}

//===============================================
// デストラクタ
//===============================================
CRockon::~CRockon()
{
	m_nNumAll--;	// 総数をカウントダウン
}

//===============================================
// 生成処理
//===============================================
CRockon *CRockon::Create(D3DXVECTOR3 pos, CGarbage::SEPARATION separation, int nID, int nPriority)
{
	CRockon *pRockon = NULL;

	if (MAX_ROCKON > m_nNumAll)
	{// ロックオンの総数に達していない
		// ロックオンの生成
		pRockon = new CRockon(nPriority);

		// 種類の設定
		pRockon->SetType(TYPE_ROCKON);

		// サイズの設定
		pRockon->SetSize(ROCKON_SIZEX, ROCKON_SIZEY);

		// 初期化処理
		pRockon->Init(pos);

		// 状態の設定
		pRockon->SetSeparation(separation);

		// テクスチャの割り当て
		pRockon->BindTexture(m_nIdxTexture);
	}

	return pRockon;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CRockon::Init(D3DXVECTOR3 pos)
{
	// ビルボードの初期化処理
	CObjectBillboard::Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\target000.png");

	// サウンドの再生
	CManager::GetSound()->Play(CSound::LABEL_SE_ROCKON);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRockon::Uninit(void)
{
	// ビルボードの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CRockon::Update(void)
{
	// 縮小処理
	CObjectBillboard::ScalingSize(DIFF_SIZE);

	// 捨てたときにロックオンを外す
	if ((m_separation == CGarbage::SEPARATION_BURN && (CManager::GetKeyboardInput()->GetRelease(DIK_RIGHT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_B, 0) == true))
		|| (m_separation == CGarbage::SEPARATION_NONFLAMMABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_LEFT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_X, 0) == true))
		|| (m_separation == CGarbage::SEPARATION_RECYCLABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_UP) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_Y, 0) == true)))
	{// SPACEキーが押された
		// 終了処理
		Uninit();
	}
}

//===============================================
// 描画処理
//===============================================
void CRockon::Draw(void)
{
	// ビルボードの描画処理
	CObjectBillboard::Draw();
}

//===============================================
// 設定処理
//===============================================
void CRockon::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;
}

//===============================================
// 状態の設定処理
//===============================================
void CRockon::SetSeparation(CGarbage::SEPARATION separation)
{
	m_separation = separation;

	// 分別ごとに色を設定する
	switch (m_separation)
	{
	case CGarbage::SEPARATION_BURN:			// 燃えるゴミ
		// オブジェクトビルボードの色設定
		CObjectBillboard::SetCol(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));	// 赤
		break;

	case CGarbage::SEPARATION_NONFLAMMABLE:	// 燃えないゴミ
		// オブジェクトビルボードの色設定
		CObjectBillboard::SetCol(D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f));	// 青
		break;

	case CGarbage::SEPARATION_RECYCLABLE:		// 資源ゴミ
		// オブジェクトビルボードの色設定
		CObjectBillboard::SetCol(D3DXCOLOR(0.9f, 0.75f, 0.2f, 1.0f));	// 黄色
		break;
	}
}

//===============================================
// オブジェクトの番号設定処理
//===============================================
void CRockon::SetIDObj(int nID)
{
	m_nIDGarbage = nID;
}

//===============================================
// ロックオンしたオブジェクトの設定処理
//===============================================
void CRockon::SetObjRockon(int nCntRockon, int nCntObj)
{
	m_aObjRockon[nCntRockon] = nCntObj;
}