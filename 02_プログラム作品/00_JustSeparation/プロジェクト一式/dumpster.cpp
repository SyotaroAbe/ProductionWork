//=========================================================
//
// ゴミステーション処理 [dumpster.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "dumpster.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "xfile.h"
#include "game.h"
#include "target.h"
#include "tutorial.h"
#include "player.h"

//===============================================
// マクロ定義
//===============================================
#define DELETE_POSITION			(550.0f)	// 床を破棄する位置

//===============================================
// 静的メンバ変数
//===============================================
int CDumpster::m_nIdxXFile = 0;			// 使用するXファイルの番号

//===============================================
// コンストラクタ
//===============================================
CDumpster::CDumpster() : CObjectX(4)
{
	// 値のクリア
	m_bRockon = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CDumpster::CDumpster(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_bRockon = false;
}

//===============================================
// デストラクタ
//===============================================
CDumpster::~CDumpster()
{
	
}

//===============================================
// モデルの生成（読み込み）
//===============================================
void CDumpster::Load(HWND hWnd)
{
	FILE *pFile;
	
	// Ｘファイルに読み込み
	m_nIdxXFile = CManager::GetXFile()->Regist("data\\MODEL\\Dumpster001.x");

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// ゲームモード
		// ファイルを開く
		pFile = fopen("data\\TXT\\model.txt", "r");

		if (pFile != NULL)
		{// 読み込み成功
			char aStr[MAX_NAME] = {};

			while (1)
			{
				int nResult = fscanf(pFile, "%s", &aStr[0]);

				if (strcmp(&aStr[0], "SCRIPT") == 0)
				{// SCRIPT情報読み込み
					Script(pFile);
					break;
				}
				else if (nResult == EOF)
				{// 最後まで読み込んだ
					break;
				}
			}

			// ファイル閉じる
			fclose(pFile);
		}
		else
		{
			MessageBox(hWnd, "ファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
		}
	}
}

//===============================================
// Script情報読み込み処理
//===============================================
void CDumpster::Script(FILE *pFile)
{
	char aStr[MAX_NAME] = {};

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "DUMPSTERSET") == 0)
		{// モデル情報読み込み
			ModelSet(pFile);
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// 最後まで読み込んだ
			break;
		}
	}
}

//===============================================
// Model情報読み込み処理
//===============================================
void CDumpster::ModelSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};
	MODEL type;					// 種類
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 size;			// サイズ

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TYPE") == 0)
		{// 種類読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &type);
		}
		else if (strcmp(&aStr[0], "POS") == 0)
		{// 位置読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// 向き読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);
		}

		if (strcmp(&aStr[0], "END_DUMPSTERSET") == 0)
		{// 最後まで読み込んだ
			Create(pos, rot, 3);
			break;
		}
	}
}

//===============================================
// 生成処理
//===============================================
CDumpster *CDumpster::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CDumpster *pGarbage = NULL;

	// ゴミの生成
	pGarbage = new CDumpster(nPriority);

	// 種類の設定
	pGarbage->SetType(CObject::TYPE_DUMPSTER);

	// 初期化処理
	pGarbage->Init(pos);

	// 向きの設定
	pGarbage->SetRot(rot);

	return pGarbage;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CDumpster::Init(D3DXVECTOR3 pos)
{
	// オブジェクトXの初期化処理
	CObjectX::Init(pos);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CDumpster::Uninit(void)
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CDumpster::Update(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;

	if (CManager::GetMode() == CScene::MODE_TUTORIAL 
		&& (m_pos.x <= CTutorial::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.x >= CTutorial::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
		&& m_pos.z <= CTutorial::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.z >= CTutorial::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEX)
		|| CManager::GetMode() == CScene::MODE_GAME 
		&& (m_pos.x <= CGame::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.x >= CGame::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
		&& m_pos.z <= CGame::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.z >= CGame::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEX))
	{// ターゲットがゴミステーションを利用できる範囲内に入った
		m_bRockon = true;	// ターゲットの範囲内へ切り替える
	}
	else
	{
		m_bRockon = false;	// ターゲットの範囲外へ切り替える
	}

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (CTutorial::GetPlayer()->GetPos().z < m_pos.z - DELETE_POSITION)
		{// 画面外へいった
			// 終了処理
			Uninit();
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CDumpster::Draw(void)
{
	if (m_bRockon == true)
	{// ターゲットの範囲内
		// オブジェクトXの描画処理（緑色）
		CObjectX::DrawXFile(m_nIdxXFile, CXFile::COL_GREEN);
	}
	else
	{// ターゲットの範囲外
		// オブジェクトXの描画処理（通常色）
		CObjectX::DrawXFile(m_nIdxXFile, CXFile::COL_NORMAL);
	}
}