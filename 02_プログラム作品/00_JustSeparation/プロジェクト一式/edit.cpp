//=========================================================
//
// エディット処理 [editor.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "xfile.h"
#include "player.h"
#include "debugproc.h"
#include "garbage.h"
#include "game.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE_MODEL			(1.0f)					// モデルの移動量
#define ROT_LEFTUP			(-0.25f)				// 左上の角度
#define ROT_LEFT			(-0.5f)					// 左の角度
#define ROT_LEFTDOWN		(-0.75f)				// 左下の角度
#define ROT_RIGHTUP			(0.25f)					// 右上の角度
#define ROT_RIGHT			(0.5f)					// 右の角度
#define ROT_RIGHTDOWN		(0.75f)					// 右下の角度
#define ROT_UP				(0.0f)					// 上の角度
#define ROT_DOWN			(1.0f)					// 下の角度

//===============================================
// 静的メンバ変数
//===============================================
CObjectX *CEdit::m_apObjectX[MAX_MODELEDIT];			// オブジェクトXクラスのポインタ

//===============================================
// コンストラクタ
//===============================================
CEdit::CEdit()
{
	// 値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nType = CGarbage::TYPE_BAG;
	m_nID = 0;
	m_bEdit = false;
}

//===============================================
// デストラクタ
//===============================================
CEdit::~CEdit()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CEdit::Init(void)
{
	for (int nCntObj = 0; nCntObj < 1; nCntObj++)
	{
		if (m_apObjectX[nCntObj] == NULL)
		{// 使用されていない
			// 生成
			m_apObjectX[nCntObj] = new CObjectX(3);

			// 種類の設定
			m_apObjectX[nCntObj]->SetType(CObject::TYPE_XFAIL);

			// 初期化
			m_apObjectX[nCntObj]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CEdit::Uninit(void)
{
	// オブジェクトXの終了処理
	if (m_apObjectX[0] != NULL)
	{// 使用されている
		m_apObjectX[0]->Uninit();
	}
}

//===============================================
// 更新処理
//===============================================
void CEdit::Update(void)
{
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_A) == true)
	{// 左キーが押された
		if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true)
		{// 左上移動
			m_pos.x += sinf(D3DX_PI * ROT_LEFTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_LEFTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true)
		{// 左下移動
			m_pos.x += sinf(D3DX_PI * ROT_LEFTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_LEFTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else
		{// 左移動
			m_pos.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_D) == true)
	{// 右キーが押された
		if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true)
		{// 右上移動
			m_pos.x += sinf(D3DX_PI * ROT_RIGHTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_RIGHTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true)
		{// 右下移動
			m_pos.x += sinf(D3DX_PI * ROT_RIGHTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_RIGHTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else
		{// 右移動
			m_pos.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true)
	{// 上キーが押された
		m_pos.x += sinf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		m_pos.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true)
	{// 下キーが押された
		m_pos.x += sinf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		m_pos.z += cosf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
	}

	// モデルの種類の変更
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_LSHIFT) == true)
	{// 左SHIFTキー
		m_nType++;

		if (m_nType >= CGarbage::TYPE_MAX)
		{// 最大種類を超えた
			// 種類を範囲内に設定する
			m_nType = CGarbage::TYPE_BAG;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_LCONTROL) == true)
	{// 左CTRLキー
		m_nType--;

		if (m_nType < CGarbage::TYPE_BAG)
		{// 最小種類を超えた
			// 種類を範囲内に設定する
			m_nType = CGarbage::TYPE_MAX -1;
		}
	}

	// 位置の設定
	m_apObjectX[m_nID]->SetPos(m_pos);

	CManager::GetDebugProc()->Print(" モデルタイプ：%d\n", m_nType);
	CManager::GetDebugProc()->Print(" モデルposX：%f\n", m_pos.x);
	CManager::GetDebugProc()->Print(" モデルposZ：%f\n", m_pos.z);
}

//===============================================
// 描画処理
//===============================================
void CEdit::Draw(void)
{
	// Xファイルの透明描画処理
	m_apObjectX[m_nID]->Draw();
	CManager::GetXFile()->CleannessDraw(m_nType);
}

//===============================================
// エディット切り替え処理
//===============================================
void CEdit::Set(bool bEdit)
{
	m_bEdit = bEdit;
	m_pos = CGame::GetPlayer()->GetPos();
}