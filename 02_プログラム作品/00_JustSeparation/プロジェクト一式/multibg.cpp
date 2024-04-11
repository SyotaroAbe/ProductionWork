//=========================================================
//
// 多重の背景処理 [multibg.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "multibg.h"
#include "manager.h"
#include "renderer.h"
#include "bg.h"
#include "texture.h"

//===============================================
// 静的メンバ変数
//===============================================
CObject2D *CMultiBg::m_apObject2D[MAX_OBJECT] = {};		// オブジェクト2Dクラスのポインタ
const char *CMultiBg::m_apTextureData[NUM_BG] = {};		// テクスチャファイル名
int CMultiBg::m_aIdxTexture[NUM_BG] = {};				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CMultiBg::CMultiBg() : CObject(0)
{
	// 値をクリア
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fTexU = 0.0f;
	m_fTexV = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CMultiBg::~CMultiBg()
{

}

//===============================================
// 生成処理
//===============================================
CMultiBg *CMultiBg::Create(int nPriority)
{
	CMultiBg *pMultiBg;

	// 背景の生成
	pMultiBg = new CMultiBg;

	// 種類の設定
	pMultiBg->SetType(TYPE_BG);

	// 初期化処理
	pMultiBg->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	return pMultiBg;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CMultiBg::Init(D3DXVECTOR3 pos)
{
	int nCntBg = 0;

	// テクスチャファイル名
	const char *m_apTextureData[NUM_BG] =
	{
		{ "data\\TEXTURE\\BgMulti000.png" },	// 背景0
		{ "data\\TEXTURE\\BgMulti001.png" },	// 背景1
		{ "data\\TEXTURE\\BgMulti002.png" }		// 背景2
	};

	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (m_apObject2D[nCntObj] == NULL)
		{
			// 背景の生成
			m_apObject2D[nCntObj] = CBg::Create(0);
			m_apObject2D[nCntObj]->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

			// テクスチャの割り当て
			m_aIdxTexture[nCntObj] = CManager::GetTexture()->Regist(m_apTextureData[nCntObj]);
			nCntBg++;

			if (nCntBg == NUM_BG)
			{// 背景の枚数が最大数になった
				break;
			}
		}
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMultiBg::Uninit(void)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != NULL)
		{// 使用されている
			m_apObject2D[nCntObj] = NULL;
		}
	}

	this->Release();
}

//===============================================
// 更新処理
//===============================================
void CMultiBg::Update(void)
{
	for (int nCntBg = 0; nCntBg < MAX_OBJECT; nCntBg++)
	{
		if (m_apObject2D[nCntBg] != NULL)
		{
			TYPE type;
			type = m_apObject2D[nCntBg]->GetType();

			if (type == TYPE_BG)
			{// 背景
				// オブジェクト2Dの更新処理
				m_apObject2D[nCntBg]->Update();
			}
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CMultiBg::Draw(void)
{
	for (int nCntBg = 0; nCntBg < MAX_OBJECT; nCntBg++)
	{
		if (m_apObject2D[nCntBg] != NULL)
		{
			TYPE type;
			type = m_apObject2D[nCntBg]->GetType();

			if (type == TYPE_BG)
			{// 背景
				// オブジェクト2Dの描画処理
				m_apObject2D[nCntBg]->Draw();
			}
		}
	}
}

//===============================================
// 設定処理
//===============================================
void CMultiBg::Set(const float TexU, const float TexV)
{
	m_fTexU = TexU;
	m_fTexV = TexV;
}

//===============================================
// 位置設定
//===============================================
void CMultiBg::SetPos(const D3DXVECTOR3 pos)
{
	
}

//===============================================
// 移動量設定
//===============================================
void CMultiBg::SetMove(const D3DXVECTOR3 move)
{

}

//===============================================
// 向き設定
//===============================================
void CMultiBg::SetRot(const D3DXVECTOR3 rot)
{
	
}