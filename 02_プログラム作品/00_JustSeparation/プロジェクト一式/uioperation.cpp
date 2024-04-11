//=========================================================
//
// 操作説明表示処理 [uioperation.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "uioperation.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "tutorial.h"
#include "uigage.h"
#include "target.h"
#include "dumpster.h"
#include "player.h"

//===============================================
// マクロ定義
//===============================================
#define SIZE_X				(285.0f)		// 横サイズ
#define SIZE_Y				(90.0f)			// 縦サイズ
#define TIME_PROGRESS		(240)			// 進捗移行時間
#define TIME_PROGRESSTART	(1200)			// 進捗移行開始時間

//===============================================
// 静的メンバ変数
//===============================================
int CUiOperation::m_aIdxTexture[TEX_MAX] = {};				// 使用するテクスチャの番号
int CUiOperation::m_nProgress = -1;							// 進捗
int CUiOperation::m_nTime = 0;								// 経過時間

//===============================================
// テクスチャファイル名
//===============================================
const char *CUiOperation::m_apTextureData[TEX_MAX] = 
{
	{ "data\\TEXTURE\\tutorial008.png" },	// 移動操作できない
	{ "data\\TEXTURE\\tutorial009.png" },	// ターゲットを重ねる
	{ "data\\TEXTURE\\tutorial001.png" },	// ターゲット移動
	{ "data\\TEXTURE\\tutorial010.png" },	// 色を揃えよう
	{ "data\\TEXTURE\\tutorial002.png" },	// 燃えるゴミ
	{ "data\\TEXTURE\\tutorial003.png" },	// 燃えないゴミ
	{ "data\\TEXTURE\\tutorial004.png" },	// 資源ゴミ
	{ "data\\TEXTURE\\tutorial005.png" },	// ゲージ
	{ "data\\TEXTURE\\tutorial006.png" },	// ゴミステーション
	{ "data\\TEXTURE\\tutorial007.png" }	// 終了確認
};

//===============================================
// コンストラクタ
//===============================================
CUiOperation::CUiOperation() : CObject2D(5)
{
	// 値のクリア
	m_nTypeTex = 0;
	m_nGarbage = 0;
	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		m_bPressInput[nCntButton] = false;
	}
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CUiOperation::CUiOperation(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nTypeTex = 0;
	m_nGarbage = 0;
	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		m_bPressInput[nCntButton] = false;
	}
}

//===============================================
// デストラクタ
//===============================================
CUiOperation::~CUiOperation()
{
	
}

//===============================================
// 生成処理
//===============================================
CUiOperation *CUiOperation::Create(int nTex, int nPriority)
{
	CUiOperation *pUi;

	// 背景の生成
	pUi = new CUiOperation(nPriority);

	// 種類の設定
	pUi->SetType(TYPE_BULLET);

	// サイズの設定
	pUi->SetSize(SIZE_X, SIZE_Y);

	// 初期化処理
	pUi->Init(D3DXVECTOR3(650.0f, 600.0f, 0.0f), nTex, nPriority);

	// 設定処理
	pUi->Set(nTex);

	return pUi;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CUiOperation::Init(D3DXVECTOR3 pos, int nTex, int nPriority)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CUiOperation::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CUiOperation::Update(void)
{
	if (m_nProgress != TEX_GAGE)
	{// ゲージ説明じゃないとき
		m_nGarbage = CTutorial::GetUiGage()->Get();		// ゴミの量を保存
	}

	//if (m_nProgress >= TEX_MATCHCOL)
	//{
	//	// 設定処理
	//	CTutorial::GetUiGage()->Set(-MAX_GARBAGE);
	//}

	if (m_nTypeTex == m_nProgress)
	{// 進捗とテクスチャの種類が同じ
		switch (m_nProgress)
		{
		case TEX_NOT_OPERATION:	// 移動操作できない
			m_nTime++;		// 時間をカウントアップ

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// カウントをリセット
				m_nProgress++;		// 進捗を進める

				// 終了処理
				Uninit();
			}
			break;

		case TEX_COLLECT:		// ターゲットを重ねる
			m_nTime++;		// 時間をカウントアップ

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// カウントをリセット
				m_nProgress++;		// 進捗を進める

				// 終了処理
				Uninit();
			}
			break;

		case TEX_TARGET:		// ターゲット
			if (CManager::GetKeyboardInput()->GetPress(DIK_A) == true || CManager::GetInputGamePad()->GetJoyStickLX(0) < 0)
			{
				m_bPressInput[BUTTON_A] = true;
			}
			else if (CManager::GetKeyboardInput()->GetPress(DIK_W) == true || CManager::GetInputGamePad()->GetJoyStickLY(0) > 0)
			{
				m_bPressInput[BUTTON_W] = true;
			}
			else if (CManager::GetKeyboardInput()->GetPress(DIK_S) == true || CManager::GetInputGamePad()->GetJoyStickLY(0) < 0)
			{
				m_bPressInput[BUTTON_S] = true;
			}
			else if (CManager::GetKeyboardInput()->GetPress(DIK_D) == true || CManager::GetInputGamePad()->GetJoyStickLX(0) > 0)
			{
				m_bPressInput[BUTTON_D] = true;
			}

			if(m_bPressInput[BUTTON_A] == true && m_bPressInput[BUTTON_W] == true && m_bPressInput[BUTTON_S] == true && m_bPressInput[BUTTON_D] == true)
			{// 
				m_nProgress++;		// 進捗を進める

				// 終了処理
				Uninit();
			}
			break;

		case TEX_MATCHCOL:		// 色を揃えよう
			m_nTime++;		// 時間をカウントアップ

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// カウントをリセット
				m_nProgress++;		// 進捗を進める

				// 終了処理
				Uninit();
			}
			break;

		case TEX_BURN:			// 燃えるゴミ
			if ((CManager::GetKeyboardInput()->GetPress(DIK_RIGHT) == true
				|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_B, 0) == true)
				&& CTutorial::GetTarget()->GetWithinRange() == true)
			{// 
				m_nProgress++;		// 進捗を進める

				// 終了処理
				Uninit();
			}
			break;

		case TEX_NONFLAMMABLE:	// 燃えないゴミ
			if ((CManager::GetKeyboardInput()->GetPress(DIK_UP) == true
				|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_Y, 0) == true)
				&& CTutorial::GetTarget()->GetWithinRange() == true)
			{// 
				m_nProgress++;		// 進捗を進める

				// 終了処理
				Uninit();
			}
			break;

		case TEX_RECYCLABLE:	// 資源ゴミ
			if ((CManager::GetKeyboardInput()->GetPress(DIK_LEFT) == true
				|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_X, 0) == true)
				&& CTutorial::GetTarget()->GetWithinRange() == true)
			{// 
				m_nProgress++;		// 進捗を進める

				// 終了処理
				Uninit();
			}
			break;

		case TEX_GAGE:			// ゲージ
			m_nTime++;		// 時間をカウントアップ

			//// ゴミゲージの設定処理
			//CTutorial::GetUiGage()->Set(0.01f);

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// カウントをリセット
				m_nProgress++;		// 進捗を進める

				//// ゴミの量を元に戻す
				//CTutorial::GetUiGage()->Set(-1.0f);
				//CTutorial::GetUiGage()->Set(m_nGarbage);

				// 終了処理
				Uninit();
			}
			break;

		case TEX_DUMPSTER:		// ゴミステーション
			for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
			{
				CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

				while (pObject != NULL)
				{// 使用されている
					CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
					CObject::TYPE type = pObject->GetType();		// 種類を取得

					if (type == CObject::TYPE_DUMPSTER)
					{// ゴミステーション
						if ((CManager::GetKeyboardInput()->GetTrigger(DIK_SPACE) == true
							|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
							&& pObject->GetPos().x <= CTutorial::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().x >= CTutorial::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
							&& pObject->GetPos().z <= CTutorial::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().z >= CTutorial::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEX)
						{// ゴミステーションを使った
							m_nProgress++;		// 進捗を進める

							// 終了処理
							Uninit();
						}
					}

					pObject = pObjectNext;		// 次のオブジェクトを代入
				}
			}
			break;

		case TEX_END:			// 終了確認
			break;
		}
	}
	else if (m_nProgress == -1)
	{
		m_nTime++;		// 時間をカウントアップ

		if (m_nTime > TIME_PROGRESSTART)
		{// 一定時間経過
			m_nTime = 0;		// カウントをリセット
			m_nProgress++;		// 進捗を進める
		}
	}

	if (CManager::GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true
		|| CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{// 画面遷移した
		m_nTime = 0;			// カウントをリセット
		m_nProgress = -1;		// 進捗をリセット
	}

	// オブジェクト2Dの更新処理
	CObject2D::Update();
}

//===============================================
// 描画処理
//===============================================
void CUiOperation::Draw(void)
{
	if (m_nTypeTex == m_nProgress)
	{// 進捗とテクスチャの種類が同じ
		// オブジェクト2Dの描画処理
		CObject2D::Draw();
	}
}

//===============================================
// 設定処理
//===============================================
void CUiOperation::Set(const int nType)
{
	m_nTypeTex = nType;

	// テクスチャの設定
	m_aIdxTexture[m_nTypeTex] = CManager::GetTexture()->Regist(m_apTextureData[m_nTypeTex]);

	// テクスチャの割り当て
	CObject2D::BindTexture(m_aIdxTexture[m_nTypeTex]);
}