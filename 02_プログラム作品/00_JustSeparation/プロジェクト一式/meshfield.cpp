//=========================================================
// 
// メッシュフィールド描画処理 [meshfield.cpp]
// Author = 阿部翔大郎
// 
//=========================================================
#include "main.h"
#include "meshfield.h"
#include "texture.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"
#include "garbage.h"
#include "game.h"
#include "tutorial.h"
#include "target.h"
#include "title.h"
#include "renderer.h"
#include "time.h"

//===============================================
// マクロ定義
//===============================================
#define FIELD_COUNTX			(6)			// 床の配置数（X）
#define FIELD_COUNTY			(6)			// 床の配置数（Y）
#define MIN_FIELDPOS			(9900.0f)	// 床最小位置初期値
#define MAX_FIELDPOS			(-9900.0f)	// 床最大位置初期値

#define TURN_ROT_DIFF			(0.001f)	// 曲がる角度の差分
#define TURN_LENTH				(0.25f)		// 曲がれる範囲
#define TURN_DIFF				(50.0f)		// 曲がれる差分

#define LENTH_FAR				(240.0f)	// 道の中心からの距離（遠い）
#define LENTH_GABAGE			(180.0f)	// 道の中心からの距離（中）
#define LENTH_NEAR				(150.0f)	// 道の中心からの距離（近い）
#define GABAGE_POSITION			(280.0f)	// ゴミの置かれる位置
#define DISTANCE_HEIGHT			(100.0f)	// ゴミ同士の縦間隔
#define DISTANCE_WIDTH			(220.0f)		// ゴミ同士の横間隔

// TUTORIAL
#define DELETE_POSITION			(650.0f)	// 床を破棄する位置

// FEVER
#define CREATEGABAGETIME_FEVER		(120)									// フィーバー状態時にゴミを生成する時間間隔
#define RAND_GABAGEPOSFEVER			(140)									// フィーバー状態時にゴミが生成される確率
#define ADD_RANDFEVER				(1)										// フィーバー状態のランダムの確率上昇数

// マップランダム自動生成
#define MAX_COUNTX				(FIELD_COUNTX -1)	// 床の最大配置数（X）
#define MAX_COUNTY				(FIELD_COUNTY -1)	// 床の最大配置数（Y）

#define FIRST_PATTERN			(0)			// １パターン目
#define SECOND_PATTERN			(1)			// ２パターン目
#define THIRD_PATTERN			(2)			// ３パターン目

#define RAND_UP_MOVELEFT						(3)			// 上の左進めるときのrand
#define RAND_UP_NOTMOVELEFT						(4)			// 上の左進めないときのrand
#define RAND_LEFTUP								(1)			// 左上のときのrand
#define RAND_LEFT_MOVEDOWN_MOVELEFT				(1)			// 左の下進める左進めるときのrand
#define RAND_LEFT_MOVEDOWN_NOTMOVELEFT			(1)			// 左の下進める左進めないときのrand
#define RAND_LEFT_NOTMOVEDOWN					(1)			// 左の下進めない左進めるときのrand
#define RAND_RIGHTDOWN							(3)			// 右下のときのrand
#define RAND_RIGHT_MOVEDOWN						(4)			// 右の下進めるときのrand
#define RAND_RIGHT_NOTMOVEDOWN					(2)			// 右の下進めないときのrand
#define RAND_DOWN_NOTMOVEDOWN_MOVELEFT			(1)			// 下の下進めないが左進めるときのrand
#define RAND_DOWN_NOTMOVEDOWN_NOTMOVELEFT		(1)			// 下の下進めないが左進めないときのrand
#define RAND_INSIDE_MOVEDOWN_MOVELEFT			(3)			// 内側の下進める左進めるときのrand
#define RAND_INSIDE_MOVEDOWN_NOTMOVELEFT		(3)			// 内側の下進める左進めないときのrand
#define RAND_INSIDE_NOTMOVEDOWN_MOVELEFT		(3)			// 内側の下進めない左進めるときのrand
#define RAND_INSIDE_NOTMOVEDOWN_NOTMOVELEFT		(3)			// 内側の下進めない左進めないときのrand

// 床繋がっているかチェック
#define HALF_POS				(23)					// 半分の位置
#define TWO_BACK				(-2)					// ２つ前
#define HALF_COUNT				(FIELD_COUNTX / 2)		// 床の半分配置数（X）

#define STRAIGHT_UPH_POS		(12)			// 上半分の縦方向で繋がっているか確認位置
#define STRAIGHT_DOWNH_POS		(30)			// 下半分の縦方向で繋がっているか確認位置
#define STRAIGHT_RIGHTW_POS		(2)			// 右半分の横方向で繋がっているか確認位置
#define STRAIGHT_LEFTW_POS		(5)			// 左半分の横方向で繋がっているか確認位置

// ゴミの配置
#define RANDPOSITION_MAX		(3)			// ゴミのランダム最大数

//===============================================
// 静的メンバ変数
//===============================================
char CMeshField::m_aFileName[TYPE_MAX][MAX_NAME] = {};		// テクスチャファイル名を保存
int CMeshField::m_nIdxTexture[TYPE_MAX] = {};				// 使用するテクスチャの番号
LPDIRECT3DTEXTURE9 CMeshField::m_apTexture[TYPE_MAX] = {};	// テクスチャへのポインタ
CMeshField::TYPE CMeshField::m_aType[MAX_FIELD] = {};		// 床の種類
int CMeshField::m_nNumAll = 0;								// 総数

//===============================================
// コンストラクタ
//===============================================
CMeshField::CMeshField() : CObjectMesh(3)
{
	//値をクリアする
	m_type = TYPE_STRAIGHT_H;
	m_destination = DESTINATION_NONE;
	m_nTimeFever = 0;
	m_nRandFever = 0;

	m_nNumAll++;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CMeshField::CMeshField(int nPriority) : CObjectMesh(nPriority)
{
	//値をクリアする
	m_type = TYPE_STRAIGHT_H;
	m_destination = DESTINATION_NONE;
	m_nTimeFever = 0;
	m_nRandFever = 0;

	m_nNumAll++;
}

//===============================================
// デストラクタ
//===============================================
CMeshField::~CMeshField()
{
	m_nNumAll--;
}

//===============================================
// 生成処理
//===============================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, TYPE nType, int nPriority)
{
	CMeshField *pObjectMesh;

	// メッシュフィールドの生成
	pObjectMesh = new CMeshField(nPriority);

	// 種類の設定
	pObjectMesh->SetType(TYPE_FIELD);

	// サイズの設定
	pObjectMesh->SetSize(fSizeX, fSizeZ);

	// 初期化処理
	pObjectMesh->Init(pos);

	// 設定処理
	pObjectMesh->Set(pos, rot, nType);

	// テクスチャの割り当て
	pObjectMesh->BindTexture(m_nIdxTexture[nType]);

	return pObjectMesh;
}

//===============================================
// 読み込み処理
//===============================================
void CMeshField::Load(HWND hWnd)
{
	FILE *pFile;

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
		MessageBox(hWnd, "ファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);	// 警告表示
	}

	//if (CManager::GetMode() == CScene::MODE_GAME || CManager::GetMode() == CScene::MODE_TITLE)
	//{// モードがゲーム
	//	// マップランダム自動生成
	//	RandArrange();
	//}
}

//===============================================
// テクスチャの破棄
//===============================================
void CMeshField::Unload(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//===============================================
// Script情報読み込み処理
//===============================================
void CMeshField::Script(FILE *pFile)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   //デバイスの取得

	char aStr[MAX_NAME] = {};
	int nCntTexture = 0;				// テクスチャ枚数をカウント

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TEXTURE_FILENAME") == 0)
		{// テクスチャ読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%s", &m_aFileName[nCntTexture][0]);

			// テクスチャの設定
			m_nIdxTexture[nCntTexture] = CManager::GetTexture()->Regist((const char*)&m_aFileName[nCntTexture][0]);

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				(const char*)&m_aFileName[nCntTexture][0],
				&m_apTexture[nCntTexture]);

			nCntTexture++;	// テクスチャ枚数をカウントアップ
		}
		else if (strcmp(&aStr[0], "FIELDSET") == 0)
		{// 床情報読み込み
			FieldSet(pFile);
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// 最後まで読み込んだ
			break;
		}
	}
}

//===============================================
// Field情報読み込み処理
//===============================================
void CMeshField::FieldSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};		// 文字列を取得
	TYPE type;						// 種類
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 size;				// サイズ

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TEXTYPE") == 0)
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
		else if (strcmp(&aStr[0], "SIZE") == 0)
		{// サイズ読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &size.x);
			fscanf(pFile, "%f", &size.z);
		}

		if (strcmp(&aStr[0], "END_FIELDSET") == 0)
		{//読み込んだモデル数に達した
			//CMeshField::Create(pos, rot, size.x, size.z, type, 3);
			break;
		}
	}
}

//===============================================
// ランダム配置処理
//===============================================
void CMeshField::RandArrange(void)
{
	bool bMoveOn = false;					// その先に進めるか
	int nCntPrevX = 1;						// Y方向

	// 床のランダム自動生成
	for (int nCntZ = 0; nCntZ < FIELD_COUNTY; nCntZ++)
	{
		for (int nCntX = 0; nCntX < FIELD_COUNTX; nCntX++)
		{
			int nCntField = nCntZ * FIELD_COUNTY + nCntX;				// 現在の床番号
			m_aType[nCntField] = TYPE_CROSSROADS;						// 床の種類
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 床の位置

			srand(timeGetTime());	// randの値を初期化
			int nTypeRand = 0;		// randの値を取得
			bool bNext = false;		// 隣接の床が正しく繋がっているか

			if (nCntZ == 0 && nCntX == 0)
			{// 右上角
				m_aType[nCntField] = TYPE_CURVE_UPRIGHT;	// 種類をカーブ（右上角）に設定
			}
			else
			{// それ以外
				// 隣接の床種類による床設定処理
				nCntPrevX = NextField(nCntField, nCntField - 1, (nCntZ - 1) * FIELD_COUNTY + nCntX, nCntPrevX);
			}
		}
	}

	for (int nCntZ = 0; nCntZ < FIELD_COUNTY; nCntZ++)
	{
		for (int nCntX = 0; nCntX < FIELD_COUNTX; nCntX++)
		{
			int nCntField = nCntZ * FIELD_COUNTY + nCntX;				// 現在の床番号

			// 床の生成
			CMeshField::Create(D3DXVECTOR3(-1000.0f * nCntX, 0.0f, -1000.0f * nCntZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, 500.0f, m_aType[nCntField], 3);
		}
	}
}

//===============================================
// 隣接の床種類による床設定処理
//===============================================
int CMeshField::NextField(int type, int prevTypeX, int prevTypeY, int nCntPrevX)
{
	int nCntPrev = nCntPrevX;	// 代入

	srand(timeGetTime());	// randの値を初期化
	int nTypeRand = 0;		// randの値

	if (prevTypeY < 0)
	{// 初期向きから見て奥の床が存在しない
		if (type != MAX_COUNTX)
		{// 左上角じゃない
			// 直前の床の種類別に処理を行う
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 直前の床が左進める上進めない
				srand(timeGetTime() + type);				// randの値を初期化
				nTypeRand = rand() % RAND_UP_MOVELEFT;		// randの値を取得

				// 右進める上進めない床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_W;	// 横直線
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;	// カーブ左上角
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_UP;	// T字路上
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;	// 行き止まり左
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 左進めない上進めない
				srand(timeGetTime() + type);					// randの値を初期化
				nTypeRand = rand() % RAND_UP_NOTMOVELEFT;		// randの値を取得

				// 右進めない上進めない床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_DEADEND_RIGHT;	// 行き止まり右
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_DEADEND_UP;	// 行き止まり上
					break;

				default:
					m_aType[type] = TYPE_CURVE_UPRIGHT;	// カーブ右上角
					break;
				}
			}
		}
		else
		{// 左上角
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_UP
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 直前の床が左進める上進めない
				srand(timeGetTime() + type);		// randの値を初期化
				nTypeRand = rand() % RAND_LEFTUP;	// randの値を取得

				// 左進めない右進める上進めない床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;	// カーブ左上角
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;	// 行き止まり左
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 直前の床が左進めない上進めない
				// 左進めない右進めない上進めない床を生成
				m_aType[type] = TYPE_DEADEND_UP;	// 行き止まり上
			}
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 2)
	{// 左下角１つ前
		// 直前の床の種類別に処理を行う
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が下に進める
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進める
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// randの値を取得

				if (m_aType[prevTypeY + 1] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN || m_aType[prevTypeY + 1] == TYPE_DEADEND_LEFT
					|| m_aType[prevTypeY + 1] == TYPE_STRAIGHT_H || m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
				{// この左上の床が左に進めない下に進めない、もしくは左進めない下進める右進めない
					// 下に進めない右に進める上に進める左に進める床を生成
					m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
				}
				else
				{
					// 下進めない右進める上進める床からランダムで生成
					switch (nTypeRand)
					{
					case FIRST_PATTERN:
						m_aType[type] = TYPE_CURVE_DOWNLEFT;	// カーブ左下角
						break;

					default:
						m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
						break;
					}
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進めない
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// randの値を取得

				if (m_aType[prevTypeY + 1] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN || m_aType[prevTypeY + 1] == TYPE_DEADEND_LEFT
					|| m_aType[prevTypeY + 1] == TYPE_STRAIGHT_H || m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
				{// この左上の床が左に進めない下に進めない、もしくは左進めない下進める右進めない
					// 下に進めない右に進めない上に進める左に進める床を生成
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
				}
				else
				{
					// 下進めない右進めない上進める床からランダムで生成
					switch (nTypeRand)
					{
					case FIRST_PATTERN:
						m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
						break;

					default:
						m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
						break;
					}
				}
			}			
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// 上の床が下に進めない
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進める
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_NOTMOVELEFT;	// randの値を取得

				if (m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
				{// 左上の床が行き止まり上
					m_aType[type] = TYPE_STRAIGHT_W;		// 直線横
				}
				else
				{
					// 下進めない右進める上進めない床からランダムで生成
					switch (nTypeRand)
					{
					case FIRST_PATTERN:
						m_aType[type] = TYPE_STRAIGHT_W;		// 直線横
						break;

					default:
						m_aType[type] = TYPE_DEADEND_LEFT;		// 行き止まり左
						break;
					}
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進めない
				// 下進めない右進めない上進めない床を生成
				m_aType[type] = TYPE_DEADEND_RIGHT;		// 行き止まり左
			}
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 1)
	{// 左下角
		// 直前の床の種類別に処理を行う
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が下に進める
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進める
				// 左進めない下進めない右進める上進める床を生成
				m_aType[type] = TYPE_CURVE_DOWNLEFT;	// カーブ左下角
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進めない
				// 左進めない下進めない右進めない上進める床を生成
				m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
			}			
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// 上の床が下に進めない
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進める
				// 左進めない下進めない右進める上進めない床を生成
				m_aType[type] = TYPE_DEADEND_LEFT;		// 行き止まり左
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 右の床が下に進めない左に進めない
				// 左進めない下進めない右進めない上進めない床を生成（そんなものはない）
				m_aType[type] = TYPE_CROSSROADS;		// 十字路
			}
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY)
	{// 外側下の位置
		// 直前の床の種類別に処理を行う
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が下に進める
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進める
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// randの値を取得

				// 下進めない右進める上進める床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNLEFT;	// カーブ左下角
					break;

				default:
					m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 右の床が下に進めない左に進めない
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// randの値を取得

				// 下進めない右進めない上進める床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
					break;

				default:
					m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
					break;
				}
			}			
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// 上の床が下に進めない
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が下に進めない左に進める
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_NOTMOVELEFT;	// randの値を取得

				// 下進めない右進める上進めない床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_W;		// 直線横
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// 行き止まり左
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 右の床が下に進めない左に進めない
				// 下進めない右進めない上進めない床を生成
				m_aType[type] = TYPE_DEADEND_RIGHT;		// 行き止まり右
			}
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX -1)
	{// 外側左の位置
		// 直前の床の種類別に処理を行う
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が下に進める
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が左に進める
				srand(timeGetTime() + type);						// randの値を初期化
				nTypeRand = rand() % RAND_LEFT_MOVEDOWN_MOVELEFT;	// randの値を取得

				// 左進めない右進める上進める床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNLEFT;	// カーブ左下角
					break;

				default:
					m_aType[type] = TYPE_TJUNCTION_LEFT;	// T字路左
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 右の床が左に進めない
				//srand(timeGetTime() + type);							// randの値を初期化
				//nTypeRand = rand() % RAND_LEFT_MOVEDOWN_NOTMOVELEFT;	// randの値を取得

				//// 左進めない右進めない上進める床からランダムで生成
				//switch (nTypeRand)
				//{
				//case FIRST_PATTERN:
				//	m_aType[type] = TYPE_STRAIGHT_H;		// 直線縦
				//	break;

				//default:
				//	m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
				//	break;
				//}

				m_aType[type] = TYPE_STRAIGHT_H;		// 直線縦
			}
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// 上の床が下に進めない
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が左に進める
				srand(timeGetTime() + type);				// randの値を初期化
				nTypeRand = rand() % RAND_LEFT_NOTMOVEDOWN;	// randの値を取得

				// 左進めない右進める上進めない床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;		// カーブ左上角
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// 行き止まり左
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 右の床が左に進めない
				// 左進めない右進めない上進めない床を生成
				m_aType[type] = TYPE_DEADEND_UP;		// 行き止まり上
			}
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
	{// 右下角
		// 直前の床の種類別に処理を行う
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が下に進める
			//srand(timeGetTime() + type);			// randの値を初期化
			//nTypeRand = rand() % RAND_RIGHTDOWN;	// randの値を取得

			//// 下進めない右進めない上進める床からランダムで生成
			//switch (nTypeRand)
			//{
			//case FIRST_PATTERN:
			//	m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
			//	break;

			//default:
			//	m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
			//	break;
			//}

			m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// 上の床が下に進めない
			// 下進めない右進めない上進めない床を生成
			m_aType[type] = TYPE_DEADEND_RIGHT;		// 行き止まり右
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX)
	{// 外側右の位置
		// 直前の床の種類別に処理を行う
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が下に進める
			srand(timeGetTime() + type);				// randの値を初期化
			nTypeRand = rand() % RAND_RIGHT_MOVEDOWN;	// randの値を取得

			// 右進めない上進める床からランダムで生成
			switch (nTypeRand)
			{
			case FIRST_PATTERN:
				m_aType[type] = TYPE_STRAIGHT_H;		// 直線縦
				break;

			case SECOND_PATTERN:
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
				break;

			case THIRD_PATTERN:
				m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
				break;

			default:
				m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T字路右
				break;
			}
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// 上の床が下に進めない
			srand(timeGetTime() + type);					// randの値を初期化
			nTypeRand = rand() % RAND_RIGHT_NOTMOVEDOWN;	// randの値を取得

			// 右進めない上進めない床からランダムで生成
			switch (nTypeRand)
			{
			case FIRST_PATTERN:
				m_aType[type] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
				break;

			case SECOND_PATTERN:
				m_aType[type] = TYPE_DEADEND_UP;		// 行き止まり上
				break;

			default:
				m_aType[type] = TYPE_DEADEND_RIGHT;		// 行き止まり右
				break;
			}
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);

		nCntPrev++;
	}
	else
	{// 内側の床
		// 直前の床の種類別に処理を行う
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が下に進める
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が左に進める
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_INSIDE_MOVEDOWN_MOVELEFT;		// randの値を取得

				// 右進める上進める床からランダムで生成	
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNLEFT;	// カーブ左下角
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_LEFT;	// T字路左
					break;

				default:
					m_aType[type] = TYPE_CROSSROADS;		// 十字路
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 右の床が左に進めない
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_INSIDE_MOVEDOWN_NOTMOVELEFT;	// randの値を取得

				// 右進めない上進める床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_H;		// 直線縦
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T字路右
					break;

				default:
					m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
					break;
				}
			}
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// 上の床が下に進めない
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT 
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// 右の床が左に進める
				srand(timeGetTime() + type);							// randの値を初期化
				nTypeRand = rand() % RAND_INSIDE_NOTMOVEDOWN_MOVELEFT;	// randの値を取得

				// 右進める上進めない床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_W;		// 直線横
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;		// カーブ左上角
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_UP;		// T字路上
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// 行き止まり左
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT 
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP
				|| m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// 右の床が左に進めない
				srand(timeGetTime() + type);								// randの値を初期化
				nTypeRand = rand() % RAND_INSIDE_NOTMOVEDOWN_NOTMOVELEFT;	// randの値を取得
				
				// 右進めない上進めない床からランダムで生成
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_DEADEND_RIGHT;		// 行き止まり右
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_DEADEND_UP;		// 行き止まり上
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// 行き止まり左
					break;
				}
			}
		}

		// 床が繋がっているかチェック
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	return nCntPrev;
}

//===============================================
// 床が繋がっているかチェック
//===============================================
void CMeshField::ConnectCheck(int type, int prevTypeX, int prevTypeY, int nCntPrevX)
{
	if ((m_aType[type] == TYPE_CURVE_DOWNLEFT && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
		|| (m_aType[type] == TYPE_DEADEND_DOWN && m_aType[type - 1] == TYPE_DEADEND_DOWN && m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
		|| (m_aType[type] == TYPE_CURVE_DOWNLEFT && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY] == TYPE_DEADEND_UP && m_aType[prevTypeY - 1] == TYPE_DEADEND_UP)
		|| (m_aType[type] == TYPE_DEADEND_LEFT && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY] == TYPE_DEADEND_LEFT && m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
		|| (m_aType[type] == TYPE_CURVE_DOWNLEFT && m_aType[type - 1] == TYPE_DEADEND_RIGHT && m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - 1] == TYPE_DEADEND_RIGHT))
	{// 円状の形ができている
		if (prevTypeY <= MAX_COUNTX)
		{// 上の位置
			if (prevTypeY == MAX_COUNTX)
			{// 左上
				// 形別に床の種類を変える
				if (m_aType[type] == TYPE_DEADEND_DOWN)
				{// 逆U型
					m_aType[type - 1] = TYPE_STRAIGHT_H;		// 縦直線
					m_aType[type] = TYPE_STRAIGHT_H;			// 縦直線
				}
				else if (m_aType[type] == TYPE_DEADEND_LEFT)
				{// コ型
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
					m_aType[type] = TYPE_CURVE_UPLEFT;			// カーブ左上角
				}
				else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
				{// 逆コ型
					m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T字路左
				}
				else
				{// U型、ロ型
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T字路左
				}
			}
			else if (prevTypeY - 1 == 0)
			{// 右上
				// 形別に床の種類を変える
				if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
				{// U型
					m_aType[type - 1] = TYPE_STRAIGHT_H;		// 縦直線
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T字路右
				}
				else if (m_aType[type] == TYPE_DEADEND_LEFT)
				{// コ型
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
					m_aType[type] = TYPE_TJUNCTION_UP;			// T字路上
				}
				else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
				{// 逆コ型
					m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
					m_aType[type] = TYPE_CROSSROADS;			// 十字路
				}
				else
				{// 逆U型、口型
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
					m_aType[type] = TYPE_CROSSROADS;			// 十字路
				}
			}
			else
			{// 上内側
				// 形別に床の種類を変える
				if (m_aType[type] == TYPE_DEADEND_DOWN)
				{// 逆U型
					m_aType[type - 1] = TYPE_TJUNCTION_LEFT;	// T字路左
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T字路右
				}
				else if (m_aType[type] == TYPE_DEADEND_LEFT)
				{// コ型
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
					m_aType[type] = TYPE_TJUNCTION_UP;			// T字路上
				}
				else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
				{// 逆コ型
					m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T字路左
				}
				else
				{// U型、口型
					m_aType[type - 1] = TYPE_CROSSROADS;		// 十字路
					m_aType[type] = TYPE_CROSSROADS;			// 十字路
				}
			}
		}
		else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// 左下
			// 上の床（下通れない）の種類別にその上の床の種類を変える
			switch (m_aType[prevTypeY - FIELD_COUNTY - 1])
			{
			case TYPE_STRAIGHT_W:		// 横直線
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_TJUNCTION_UP;		// T字路上
				break;

			case TYPE_CURVE_DOWNRIGHT:	// カーブ右下角
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
				break;

			case TYPE_CURVE_DOWNLEFT:	// カーブ左下角
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_TJUNCTION_LEFT;	// T字路左
				break;

			case TYPE_TJUNCTION_DOWN:	// T字路下
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_CROSSROADS;		// 十字路
				break;

			case TYPE_DEADEND_RIGHT:	// 行き止まり右
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
				break;

			case TYPE_DEADEND_DOWN:		// 行き止まり下
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_STRAIGHT_H;		// 縦直線
				break;

			case TYPE_DEADEND_LEFT:		// 行き止まり左
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_CURVE_UPLEFT;		// カーブ左上角
				break;
			}

			// 形別に床の種類を変える
			if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// U型
				m_aType[prevTypeY - 1] = TYPE_STRAIGHT_H;		// 縦直線
			}
			else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
			{// 逆コ型
				m_aType[prevTypeY - 1] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
			}
			else
			{// 逆U型、コ型、口型
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
			}
		}
		else if (type - 1 == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
		{// 右下
			// 形別に床の種類を変える
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// 逆U型
				m_aType[prevTypeY] = TYPE_TJUNCTION_UP;			// T字路上
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;			// カーブ右下角
			}
			else if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// U型
				m_aType[prevTypeY] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
				m_aType[type] = TYPE_TJUNCTION_DOWN;			// T字路下
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// コ型
				m_aType[prevTypeY] = TYPE_STRAIGHT_W;			// 横直線
				m_aType[type] = TYPE_STRAIGHT_W;				// 横直線
			}
			else
			{// 逆コ型、口型
				m_aType[prevTypeY] = TYPE_TJUNCTION_UP;			// T字路上
				m_aType[type] = TYPE_TJUNCTION_DOWN;			// T字路下
			}

			// 左上の床の種類の変更
			if (m_aType[prevTypeY + 1] == TYPE_STRAIGHT_H)
			{// 縦直線
				m_aType[prevTypeY + 1] = TYPE_TJUNCTION_LEFT;		// T字路左
			}
			else if (m_aType[prevTypeY + 1] == TYPE_CURVE_UPRIGHT)
			{// カーブ右上角
				m_aType[prevTypeY + 1] = TYPE_TJUNCTION_UP;			// T字路上
			}
			else if (m_aType[prevTypeY + 1] == TYPE_CURVE_DOWNRIGHT)
			{// カーブ右下角
				m_aType[prevTypeY + 1] = TYPE_TJUNCTION_DOWN;		// T字路下
			}
			else if (m_aType[prevTypeY + 1] == TYPE_TJUNCTION_RIGHT)
			{// T字路右
				m_aType[prevTypeY + 1] = TYPE_CROSSROADS;			// 十字路
			}
			else if (m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
			{// 行き止まり上
				m_aType[prevTypeY + 1] = TYPE_CURVE_UPLEFT;			// カーブ左上角
			}
			else if (m_aType[prevTypeY + 1] == TYPE_DEADEND_RIGHT)
			{// 行き止まり右
				m_aType[prevTypeY + 1] = TYPE_STRAIGHT_W;			// 横直線
			}
			else if (m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN)
			{// 行き止まり下
				m_aType[prevTypeY + 1] = TYPE_CURVE_DOWNLEFT;		// カーブ左下角
			}
		}
		else if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY)
		{// 下の位置
			// 形別に床の種類を変える
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// 逆U型
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// コ型
				m_aType[type] = TYPE_STRAIGHT_W;		// 横直線
			}
			else
			{// U型、逆コ型、口型
				m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
			}
		}
		else if (type - 1 == (MAX_COUNTX * nCntPrevX) + nCntPrevX)
		{// 右の位置
			// 形別に床の種類を変える
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// 逆U型
				m_aType[type - 1] = TYPE_STRAIGHT_H;		// 縦直線
				m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T字路右
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// コ型
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
				m_aType[type] = TYPE_TJUNCTION_UP;			// T字路上
			}
			else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
			{// 逆コ型
				m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
				m_aType[type] = TYPE_CROSSROADS;			// 十字路
			}
			else
			{// U型、口型
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
				m_aType[type] = TYPE_CROSSROADS;			// 十字路
			}
		}
		else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1)
		{// 左の位置
			// 形別に床の種類を変える
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// 逆U型
				m_aType[type - 1] = TYPE_STRAIGHT_H;		// 縦直線
				m_aType[type] = TYPE_STRAIGHT_H;			// 縦直線
			}
			else if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// U型
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
				m_aType[type] = TYPE_TJUNCTION_LEFT;		// T字路左
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// コ型
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
				m_aType[type] = TYPE_CURVE_UPLEFT;			// カーブ左上角
			}
			else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
			{// 逆コ型
				m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
				m_aType[type] = TYPE_TJUNCTION_LEFT;		// T字路左
			}
			else
			{// 口型
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
				m_aType[type] = TYPE_TJUNCTION_LEFT;		// T字路左
			}
		}
	}

	bool bMoveOn = false;	// 列ごとの床の種類チェック

	// 横一列行き止まり
	if (type == FIELD_COUNTX * FIELD_COUNTY - 1)
	{// 一番下の列
		for (int nCntField = 0; nCntField < FIELD_COUNTX; nCntField++)
		{// 横一列で確認
			if (m_aType[type - nCntField] == TYPE_DEADEND_LEFT)
			{// 行き止まりの開始位置がある
				for (int nCntLine = nCntField; nCntLine < FIELD_COUNTX; nCntLine++)
				{					
					if (m_aType[type - nCntField] == TYPE_DEADEND_RIGHT)
					{// 行き止まり右
						if (bMoveOn == false)
						{// 上に行ける床が１つもない
							// 現在の上の床の種類から別の種類へ変更
							switch (m_aType[prevTypeY - nCntField])
							{
							case TYPE_STRAIGHT_W:			// 直線横
								m_aType[prevTypeY - nCntField] = TYPE_TJUNCTION_UP;		// T字路上
								break;

							case TYPE_CURVE_DOWNRIGHT:		// カーブ右下角
								m_aType[prevTypeY - nCntField] = TYPE_TJUNCTION_RIGHT;	// T字路右
								break;

							case TYPE_CURVE_DOWNLEFT:		// カーブ左下角
								m_aType[prevTypeY - nCntField] = TYPE_TJUNCTION_LEFT;	// T字路左
								break;

							case TYPE_TJUNCTION_DOWN:		// T字路下
								m_aType[prevTypeY - nCntField] = TYPE_CROSSROADS;		// 十字路
								break;
							
							case TYPE_DEADEND_RIGHT:		// 行き止まり右
								m_aType[prevTypeY - nCntField] = TYPE_CURVE_UPRIGHT;	// カーブ右上角
								break;
							
							case TYPE_DEADEND_DOWN:			// 行き止まり下
								m_aType[prevTypeY - nCntField] = TYPE_STRAIGHT_W;		// 直線縦
								break;
							
							case TYPE_DEADEND_LEFT:			// 行き止まり左
								m_aType[prevTypeY - nCntField] = TYPE_CURVE_DOWNLEFT;	// カーブ左下角
								break;

							default:
								m_aType[prevTypeY - nCntField] = TYPE_CROSSROADS;		// 十字路（ここ出たらバグ）
								break;
							}

							// 行き止まり左の床を上へ繋がるように変更
							m_aType[type - nCntField] = TYPE_CURVE_DOWNLEFT;		// カーブ左下角
						}
					}
					else if(m_aType[type - nCntField] != TYPE_STRAIGHT_W && m_aType[type - nCntField] != TYPE_DEADEND_LEFT)
					{// 行き止まり左、行き止まり右、横直線じゃない
						bMoveOn = true;	// フラグを無効化
					}
				}
				bMoveOn = false;	// フラグを初期化
			}
		}
	}

	for (int nCntLine = 0; nCntLine < FIELD_COUNTX; nCntLine++)
	{// 縦一列で確認
		if (type == FIELD_COUNTX * nCntLine - 1)
		{// 半分の位置の床
			for (int nCntField = 0; nCntField < FIELD_COUNTX; nCntField++)
			{// 横一列で確認
				if (m_aType[type - nCntField] != TYPE_STRAIGHT_W && m_aType[type - nCntField] != TYPE_CURVE_DOWNLEFT && m_aType[type - nCntField] != TYPE_CURVE_DOWNRIGHT
					&& m_aType[type - nCntField] != TYPE_TJUNCTION_DOWN && m_aType[type - nCntField] != TYPE_DEADEND_DOWN && m_aType[type - nCntField] != TYPE_DEADEND_RIGHT
					&& m_aType[type - nCntField] != TYPE_DEADEND_LEFT)
				{// その床の下に行ける
					bMoveOn = true;
				}
			}

			if (bMoveOn == false)
			{// 下に行ける床が１つもない
				// 現在の床の種類から別の種類へ変更
				switch (m_aType[type])
				{
				case TYPE_CURVE_DOWNLEFT:	// カーブ左下角
					m_aType[type] = TYPE_TJUNCTION_LEFT;	// T字路左
					break;

				case TYPE_DEADEND_DOWN:		// 行き止まり下
					m_aType[type] = TYPE_DEADEND_DOWN;		// 行き止まり下
					break;

				case TYPE_DEADEND_LEFT:		// 行き止まり左
					m_aType[type] = TYPE_CURVE_UPLEFT;		// カーブ左上角
					break;

				default:
					m_aType[type] = TYPE_CROSSROADS;		// 十字路
					break;
				}
			}
		}
		bMoveOn = false;	// フラグの初期化
	}

	// １／４範囲で確認
	for (int nCntLine = 0; nCntLine < MAX_COUNTX; nCntLine++)
	{
		if (type == STRAIGHT_UPH_POS * nCntLine || type == STRAIGHT_DOWNH_POS * nCntLine)
		{// 上半分の縦方向で繋がっているか確認位置 ／ 下半分の縦方向で繋がっているか確認位置
			for (int nCntField = 0; nCntField < HALF_COUNT; nCntField++)
			{
				if (m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_STRAIGHT_W || m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_CURVE_UPRIGHT
					|| m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_CURVE_DOWNRIGHT || m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_TJUNCTION_RIGHT
					|| m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_DEADEND_RIGHT)
				{// 左へいける
					bMoveOn = true;
				}
			}

			if (bMoveOn == false)
			{// 左へいける床がない
				// 左へいける床を元の種類を生成
				switch (m_aType[type])
				{
				case TYPE_STRAIGHT_H:		// 縦直線
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T字路右
					break;

				case TYPE_CURVE_UPLEFT:		// カーブ左上角
					m_aType[type] = TYPE_TJUNCTION_UP;			// T字路上
					break;

				case TYPE_CURVE_DOWNLEFT:	// カーブ左下角
					m_aType[type] = TYPE_TJUNCTION_DOWN;		// T字路下
					break;

				case TYPE_TJUNCTION_LEFT:	// T字路左
					m_aType[type] = TYPE_CROSSROADS;			// 十字路
					break;

				case TYPE_DEADEND_UP:		// 行き止まり上
					m_aType[type] = TYPE_CURVE_UPRIGHT;			// カーブ右上角
					break;

				case TYPE_DEADEND_LEFT:		// 行き止まり左
					m_aType[type] = TYPE_STRAIGHT_W;			// 横直線
					break;

				case TYPE_DEADEND_DOWN:		// 行き止まり下
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;		// カーブ右下角
					break;
				}
			}

			bMoveOn = false;	// フラグの初期化
		}
		if (type == STRAIGHT_RIGHTW_POS * nCntLine || type == STRAIGHT_LEFTW_POS * nCntLine)
		{// 右半分の横方向で繋がっているか確認位置 ／ 左半分の横方向で繋がっているか確認位置
			for (int nCntField = 0; nCntField < HALF_COUNT; nCntField++)
			{
				if (m_aType[type - nCntField] == TYPE_STRAIGHT_H || m_aType[type - nCntField] == TYPE_CURVE_UPLEFT || m_aType[type - nCntField] == TYPE_CURVE_UPRIGHT
					|| m_aType[type - nCntField] == TYPE_TJUNCTION_UP || m_aType[type - nCntField] == TYPE_TJUNCTION_RIGHT || m_aType[type - nCntField] == TYPE_TJUNCTION_LEFT
					|| m_aType[type - nCntField] == TYPE_CROSSROADS || m_aType[type - nCntField] == TYPE_DEADEND_UP)
				{// 下へいける
					bMoveOn = true;
				}
			}

			if (bMoveOn == false)
			{// 下へ行ける床がない
				// 下へいける床を元の種類を生成
				switch (m_aType[type])
				{
				case TYPE_STRAIGHT_W:		// 横直線
					m_aType[type] = TYPE_TJUNCTION_UP;			// T字路上
					break;

				case TYPE_CURVE_DOWNRIGHT:	// カーブ右下角
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T字路右
					break;

				case TYPE_CURVE_DOWNLEFT:	// カーブ左下角
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T字路左
					break;

				case TYPE_TJUNCTION_DOWN:	// T字路下
					m_aType[type] = TYPE_CROSSROADS;			// 十字路
					break;

				case TYPE_DEADEND_RIGHT:	// 行き止まり右
					m_aType[type] = TYPE_CURVE_UPRIGHT;			// カーブ右上角
					break;

				case TYPE_DEADEND_LEFT:		// 行き止まり左
					m_aType[type] = TYPE_CURVE_UPLEFT;			// カーブ左上角
					break;

				case TYPE_DEADEND_DOWN:		// 行き止まり下
					m_aType[type] = TYPE_STRAIGHT_H;			// 縦直線
					break;
				}
			}

			bMoveOn = false;	// フラグの初期化
		}
	}

	// 床の種類別に処理する
	switch (m_aType[type])
	{
	case TYPE_STRAIGHT_H:		// 縦直線
		if (type == (MAX_COUNTX * (MAX_COUNTX - 1)) + (MAX_COUNTX - 1) - 1)
		{// 左下角の上
			for (int nCntField = 1; nCntField < MAX_COUNTX; nCntField++)
			{
				if (m_aType[(MAX_COUNTX * nCntField) + nCntField - 1] != TYPE_STRAIGHT_H)
				{// 縦一列縦直線じゃない
					bMoveOn = true;
				}

				if (bMoveOn == false)
				{// 縦一列直線
					// 縦一列の真ん中位置の種類を変更
					m_aType[HALF_POS] = TYPE_TJUNCTION_LEFT;	// T字路左

					// 変更された床に合わせて、その右の床の種類も変更する
					switch (m_aType[HALF_POS - 1])
					{
					case TYPE_STRAIGHT_H:		// 縦直線
						m_aType[HALF_POS - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
						break;

					case TYPE_CURVE_UPLEFT:		// カーブ（左上角）
						m_aType[HALF_POS - 1] = TYPE_TJUNCTION_UP;		// T字路上
						break;

					case TYPE_TJUNCTION_LEFT:	// T字路左
						m_aType[HALF_POS - 1] = TYPE_CROSSROADS;		// 十字路
						break;

					case TYPE_DEADEND_UP:		// 行き止まり上
						m_aType[HALF_POS - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
						break;

					case TYPE_DEADEND_DOWN:		// 行き止まり下
						m_aType[HALF_POS - 1] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
						break;

					case TYPE_DEADEND_LEFT:		// 行き止まり左
						m_aType[HALF_POS - 1] = TYPE_STRAIGHT_W;		// 横直線
						break;
					}
				}
			}
		}
		break;

	case TYPE_STRAIGHT_W:		// 横直線
		if (type == MAX_COUNTX - 1)
		{// 左上１つ前
			for (int nCntField = 0; nCntField < MAX_COUNTX - 2; nCntField++)
			{
				if (m_aType[type - nCntField] != TYPE_STRAIGHT_W)
				{// 横一列が横直線じゃない
					bMoveOn = true;		// フラグを折る
				}
			}

			if (bMoveOn == false)
			{// 横一列内側が横直線のみ
				m_aType[type] = TYPE_STRAIGHT_W;	// 横直線
			}
		}
		break;

	case TYPE_CURVE_UPLEFT:		// カーブ（左上角）
		break;

	case TYPE_CURVE_UPRIGHT:	// カーブ（右上角）
		break;

	case TYPE_CURVE_DOWNRIGHT:	// カーブ（右下角）
		// 上と左上を見てカーブで一周している場合自身を変更。
		if (m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT)
		{// 上の床がカーブ右上角
			if (m_aType[prevTypeY + 1] == TYPE_CURVE_UPLEFT)
			{// 左上の床がカーブ左上角
				// 床の位置別に種類を変更する
				if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY || type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
				{// 外側下の位置、右下角
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
				}
				else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1)
				{// 外側左の位置
					m_aType[type] = TYPE_STRAIGHT_H;		// 直線縦
				}
				else
				{// それ以外
					m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T字路右
				}
			}
		}

		// 右側じゃなく上が行き止まり上の場合、T字路下へ変更。
		if (type != (MAX_COUNTX * nCntPrevX) + nCntPrevX)
		{// 右側じゃない
			if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// 行き止まり上
				m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
			}
			else if (m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT)
			{// T字路右
				m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
			}

			if (m_aType[prevTypeY] == TYPE_DEADEND_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT)
			{
				// 右の種類を元の種類別に変更
				switch (m_aType[prevTypeX])
				{
				case TYPE_STRAIGHT_H:			// 縦直線
					m_aType[prevTypeX] = TYPE_TJUNCTION_RIGHT;		// T字路右
					break;

				case TYPE_CURVE_UPLEFT:			// カーブ左上角
					m_aType[prevTypeX] = TYPE_TJUNCTION_UP;			// T字路上
					break;

				case TYPE_CURVE_DOWNLEFT:		// カーブ左下角
					m_aType[prevTypeX] = TYPE_TJUNCTION_DOWN;		// T字路下
					break;

				case TYPE_TJUNCTION_LEFT:		// T字路左
					m_aType[prevTypeX] = TYPE_CROSSROADS;			// 十字路
					break;

				case TYPE_DEADEND_UP:			// 行き止まり上
					m_aType[prevTypeX] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
					break;

				case TYPE_DEADEND_DOWN:			// 行き止まり下
					m_aType[prevTypeX] = TYPE_CURVE_DOWNRIGHT;		// カーブ右下角
					break;

				case TYPE_DEADEND_LEFT:			// 行き止まり左
					m_aType[prevTypeX] = TYPE_STRAIGHT_W;			// 横直線
					break;
				}
			}
		}
		break;

	case TYPE_CURVE_DOWNLEFT:	// カーブ（左下角）
		// 床の位置別に種類を変更する
		if (type == (FIELD_COUNTY * FIELD_COUNTX) - 3)
		{// 左下角２つ前
			if (m_aType[prevTypeY + 2] == TYPE_STRAIGHT_W || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeY + 2] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY + 2] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY + 2] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeY + 2] == TYPE_DEADEND_LEFT)
			{// 左下角の上が下に繋がっていない
				m_aType[type] = TYPE_TJUNCTION_DOWN;		// T字路下
			}
		}
		else if(type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && type != (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// 外側下 かつ 右隣がカーブ右下角 かつ 左下じゃない
			m_aType[type] = TYPE_TJUNCTION_DOWN;		// T字路下
		}

		if (m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && type != (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1 && type <= FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY)
		{// 上がカーブ左上角、外側左じゃない、外側下じゃない
			m_aType[type] = TYPE_CROSSROADS;		// 十字路
		}
		else if (m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - MAX_COUNTX] == TYPE_CURVE_DOWNLEFT)
		{// 上がカーブ左上角、さらに左上がカーブ左下角
			m_aType[type] = TYPE_TJUNCTION_DOWN;	// T字路下
		}

		break;

	case TYPE_TJUNCTION_UP:		// T字路（上）
		break;

	case TYPE_TJUNCTION_RIGHT:	// T字路（右）
		break;

	case TYPE_TJUNCTION_DOWN:	// T字路（下）
		if (m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY - 1] == TYPE_DEADEND_UP && prevTypeY - 1 >= MAX_COUNTX)
		{// 右がカーブ右下角、右上が行き止まり上、外側上じゃない
			m_aType[prevTypeY - 1] = TYPE_STRAIGHT_H;		// 縦直線

			switch (m_aType[prevTypeY - FIELD_COUNTX - 1])
			{
			case TYPE_STRAIGHT_W:			// 横直線
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_TJUNCTION_UP;		// T字路上
				break;

			case TYPE_CURVE_DOWNRIGHT:		// カーブ右下角
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_TJUNCTION_RIGHT;	// T字路右
				break;

			case TYPE_CURVE_DOWNLEFT:		// カーブ左下角
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_TJUNCTION_LEFT;	// T字路左
				break;

			case TYPE_TJUNCTION_DOWN:		// T字路下
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_CROSSROADS;		// 十字路
				break;

			case TYPE_DEADEND_RIGHT:		// 行き止まり右
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
				break;

			case TYPE_DEADEND_DOWN:			// 行き止まり下
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_STRAIGHT_H;		// 縦直線
				break;

			case TYPE_DEADEND_LEFT:			// 行き止まり左
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_CURVE_UPLEFT;		// カーブ左上角
				break;
			}
		}
		break;

	case TYPE_TJUNCTION_LEFT:	// T字路（左）
		break;

	case TYPE_CROSSROADS:		// 十字路
		break;

	case TYPE_DEADEND_UP:		// 行き止まり（上）
		if (type == MAX_COUNTX)
		{// 左上の位置
			// １つ前の床から種類を変更する
			switch (m_aType[type - 1])
			{
			case TYPE_CURVE_UPLEFT:		// カーブ（左上角）
				m_aType[type] = TYPE_CURVE_UPLEFT;		// カーブ（左上角）
				m_aType[type - 1] = TYPE_TJUNCTION_UP;	// T字路（上）
				break;
			
			case TYPE_DEADEND_UP:		// 行き止まり（上）
				m_aType[type] = TYPE_CURVE_UPLEFT;		// カーブ（左上角）
				m_aType[type - 1] = TYPE_CURVE_UPRIGHT;	// カーブ（右上角）
				break;

			case TYPE_DEADEND_LEFT:		// 行き止まり（左）
				m_aType[type] = TYPE_CURVE_UPLEFT;		// カーブ（左上角）
				m_aType[type - 1] = TYPE_STRAIGHT_W;	// 横直線
				break;
			}
		}
		else
		{
			if (m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
			{// 現在地の右上がカーブ（右上角）
				// 現在地と上の種類を変更し繋ぐ
				switch (m_aType[prevTypeY])
				{
				case TYPE_STRAIGHT_W:			// 横直線
					m_aType[prevTypeY] = TYPE_TJUNCTION_UP;			// T字路（上）
					break;

				case TYPE_CURVE_DOWNLEFT:		// カーブ（左下角）
					m_aType[prevTypeY] = TYPE_TJUNCTION_LEFT;		// T字路（左）
					break;

				case TYPE_TJUNCTION_DOWN:		// T字路（下）
					m_aType[prevTypeY] = TYPE_CROSSROADS;			// 十字路
					break;

				case TYPE_DEADEND_LEFT:			// 行き止まり（左）
					m_aType[prevTypeY] = TYPE_CURVE_UPLEFT;			// カーブ（左上角）
					break;
				}

				m_aType[type] = TYPE_STRAIGHT_H;	// 縦直線
			}

			if (m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN)
			{// 現在地の左上が行き止まり下
				if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
				{// 現在地の上が行き止まり上
					// 現在地と全て繋げる
					m_aType[prevTypeY] = TYPE_CURVE_UPRIGHT;		// 現在地の上をカーブ右上角へ変更
					m_aType[prevTypeY + 1] = TYPE_CURVE_DOWNLEFT;	// 現在地の左上をカーブ左下角へ変更
				}
			}
		}
		break;

	case TYPE_DEADEND_RIGHT:	// 行き止まり（右）
		if (type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
		{// 右下の位置
			// 床の種類の変更
			m_aType[type] = TYPE_CURVE_DOWNRIGHT;		// カーブ（右下角）

			// 上の床を、変更した床に繋がるようにする
			switch (m_aType[prevTypeY])
			{
			case TYPE_CURVE_DOWNRIGHT:	// カーブ右下角
				m_aType[prevTypeY] = TYPE_TJUNCTION_RIGHT;	// T字路右	
				break;

			case TYPE_DEADEND_RIGHT:	// 行き止まり右
				m_aType[prevTypeY] = TYPE_CURVE_UPRIGHT;	// カーブ右上角
				break;

			case TYPE_DEADEND_DOWN:		// 行き止まり下
				m_aType[prevTypeY] = TYPE_STRAIGHT_H;		// 縦直線
				break;
			}
		}
		else if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY && type != (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// 外側下の位置（左下ではない）
			// 床の種類の変更
			m_aType[type] = TYPE_STRAIGHT_W;		// 横直線

			// 右の床を、変更した床に繋がるようにする
			switch (m_aType[prevTypeX])
			{
			case TYPE_CURVE_DOWNLEFT:	// カーブ左下角
				m_aType[prevTypeX] = TYPE_TJUNCTION_DOWN;	// T字路下
				break;

			case TYPE_DEADEND_DOWN:		// 行き止まり下
				m_aType[prevTypeX] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
				break;

			case TYPE_DEADEND_LEFT:		// 行き止まり左
				m_aType[prevTypeX] = TYPE_STRAIGHT_W;		// 横直線
				break;
			}
		}

		if (m_aType[prevTypeY] == TYPE_DEADEND_RIGHT)
		{// 上が行き止まり右
			m_aType[prevTypeY] = TYPE_STRAIGHT_W;		// 横直線

			// 右上の位置別に種類を変更する
			switch (m_aType[prevTypeY - 1])
			{
			case TYPE_STRAIGHT_H:			// 縦直線
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_RIGHT;		// T字路右
				break;

			case TYPE_CURVE_UPLEFT:			// カーブ左上角
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_UP;			// T字路上
				break;

			case TYPE_CURVE_DOWNLEFT:		// カーブ左下角
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_DOWN;		// T字路下
				break;

			case TYPE_TJUNCTION_LEFT:		// T字路左
				m_aType[prevTypeY - 1] = TYPE_CROSSROADS;			// 十字路
				break;

			case TYPE_DEADEND_UP:			// 行き止まり上
				m_aType[prevTypeY - 1] = TYPE_CURVE_UPRIGHT;		// カーブ右上角
				break;

			case TYPE_DEADEND_DOWN:			// 行き止まり下
				m_aType[prevTypeY - 1] = TYPE_CURVE_DOWNRIGHT;		// カーブ右下角
				break;

			case TYPE_DEADEND_LEFT:			// 行き止まり左
				m_aType[prevTypeY - 1] = TYPE_STRAIGHT_W;			// 横直線
				break;
			}
		}
		break;

	case TYPE_DEADEND_DOWN:		// 行き止まり（下）
		if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// 上の床が行き止まり上
			// 床の位置別に種類を変更する
			if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY || type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
			{// 外側下の位置、右下角
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// カーブ右下角
			}
			else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1)
			{// 外側左の位置
				m_aType[type] = TYPE_STRAIGHT_H;		// 直線縦
			}
			else
			{// それ以外
				m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T字路右
			}
		}
		break;

	case TYPE_DEADEND_LEFT:		// 行き止まり（左）
		// 床の位置別に種類を変更する
		if (type == (FIELD_COUNTY * FIELD_COUNTX) - 3)
		{// 左下角２つ前
			if (m_aType[prevTypeY + 2] == TYPE_STRAIGHT_W || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeY + 2] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY + 2] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY + 2] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeY + 2] == TYPE_DEADEND_LEFT)
			{// 左下角の上が下に繋がっていない
				m_aType[type] = TYPE_STRAIGHT_W;		// 横直線
			}
		}
		else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// 左下角
			// 床の種類の変更
			m_aType[type] = TYPE_CURVE_DOWNLEFT;		// カーブ（左下角）

			// 上の床を、変更した床に繋がるようにする
			switch (m_aType[prevTypeY])
			{
			case TYPE_CURVE_DOWNLEFT:	// カーブ左下角
				m_aType[prevTypeY] = TYPE_TJUNCTION_LEFT;	// T字路左	
				break;

			case TYPE_DEADEND_DOWN:		// 行き止まり下
				m_aType[prevTypeY] = TYPE_STRAIGHT_H;		// 縦直線
				break;

			case TYPE_DEADEND_LEFT:		// 行き止まり左
				m_aType[prevTypeY] = TYPE_CURVE_DOWNLEFT;	// カーブ左下角
				break;
			}
		}
		break;
	}
}

//===============================================
// 初期化処理
//===============================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	// オブジェクトメッシュの初期化処理
	CObjectMesh::Init(pos);

	m_PosMin = D3DXVECTOR3(MIN_FIELDPOS, MIN_FIELDPOS, MIN_FIELDPOS);
	m_PosMax = D3DXVECTOR3(MAX_FIELDPOS, MAX_FIELDPOS, MAX_FIELDPOS);
	
	// 床の位置の最小値を比較
	if (m_pos.x <= m_PosMin.x)
	{// 最小値を更新
		m_PosMin.x = m_pos.x;
	}

	if (m_pos.z <= m_PosMin.z)
	{// 最小値を更新
		m_PosMin.z = m_pos.z;
	}

	// 床の位置の最大値を比較
	if (m_pos.x >= m_PosMax.x)
	{// 最大値を更新
		m_PosMax.x = m_pos.x;
	}

	if (m_pos.z >= m_PosMax.z)
	{// 最大値を更新
		m_PosMax.z = m_pos.z;
	}

	// フィーバー時のrandの値の初期化
	m_nRandFever = RAND_GABAGEPOSFEVER;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMeshField::Uninit(void)
{
	// オブジェクトメッシュの終了処理
	CObjectMesh::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CMeshField::Update(void)
{
	m_nTimeFever++;			// 時間をカウント

	// オブジェクトメッシュの更新処理
	CObjectMesh::Update();

	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 PlayerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CManager::GetMode() == CScene::MODE_TITLE)
	{// チュートリアル
		// プレイヤーの情報取得
		PlayerPos = CTitle::GetPlayer()->GetPos();	// 位置
		PlayerRot = CTitle::GetPlayer()->GetRot();	// 向き
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{// ゲームモード
		// プレイヤーの情報取得
		PlayerPos = CGame::GetPlayer()->GetPos();	// 位置
		PlayerRot = CGame::GetPlayer()->GetRot();	// 向き

		if (CGame::GetTime()->Get() <= TIME_FEVER)
		{// FEVER TIME
			//if ((m_nTimeFever % CREATEGABAGETIME_FEVER) == 0)
			//{// 一定時間ごと
				srand(timeGetTime());					// randの値を初期化
				int nRand = rand() % m_nRandFever;		// randの値を取得

				if (nRand == 0)
				{
					// 設定処理
					Set(m_pos, m_rot, m_type);
				}
			//}
		}
	}

	if (PlayerPos.x <= m_pos.x + m_fSizeX && PlayerPos.x >= m_pos.x - m_fSizeX
		&& PlayerPos.z <= m_pos.z + m_fSizeZ && PlayerPos.z >= m_pos.z - m_fSizeZ)
	{// プレイヤーが床の範囲内に入った
		CManager::GetDebugProc()->Print(" 床の範囲内ー\n");		// デバッグ表示

		if (PlayerRot.y <= TURN_ROT_DIFF && PlayerRot.y >= -TURN_ROT_DIFF
			&& PlayerPos.z >= m_pos.z + (m_fSizeZ * TURN_LENTH) - TURN_DIFF
			&& PlayerPos.z <= m_pos.z + (m_fSizeZ * TURN_LENTH) + TURN_DIFF)
		{// -のZ方向の角
			// 種類別判定処理
			TurnInfo(D3DX_PI * ROT_UP);

			CManager::GetDebugProc()->Print(" 上向きー\n");		// デバッグ表示
		}
		else if ((PlayerRot.y <= D3DX_PI + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI - TURN_ROT_DIFF
			&& PlayerPos.z <= m_pos.z - (m_fSizeZ * TURN_LENTH) + TURN_DIFF
			&& PlayerPos.z >= m_pos.z - (m_fSizeZ * TURN_LENTH) - TURN_DIFF)
				|| (PlayerRot.y <= -D3DX_PI + TURN_ROT_DIFF && PlayerRot.y >= -D3DX_PI - TURN_ROT_DIFF
			&& PlayerPos.z <= m_pos.z - (m_fSizeZ * TURN_LENTH) + TURN_DIFF
			&& PlayerPos.z >= m_pos.z - (m_fSizeZ * TURN_LENTH) - TURN_DIFF))
		{// +のZ方向の角
			// 種類別判定処理
			TurnInfo(D3DX_PI * ROT_DOWN);

			CManager::GetDebugProc()->Print(" 下向きー\n");		// デバッグ表示
		}
		else if (PlayerRot.y <= D3DX_PI * ROT_RIGHT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_RIGHT - TURN_ROT_DIFF
			&& PlayerPos.x >= m_pos.x + (m_fSizeX * TURN_LENTH) - TURN_DIFF
			&& PlayerPos.x <= m_pos.x + (m_fSizeX * TURN_LENTH) + TURN_DIFF)
		{// -のX方向の角
			// 種類別判定処理
			TurnInfo(D3DX_PI * ROT_RIGHT);

			CManager::GetDebugProc()->Print(" 右向きー\n");		// デバッグ表示
		}
		else if (PlayerRot.y <= D3DX_PI * ROT_LEFT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_LEFT - TURN_ROT_DIFF
			&& PlayerPos.x <= m_pos.x - (m_fSizeX * TURN_LENTH) + TURN_DIFF
			&& PlayerPos.x >= m_pos.x - (m_fSizeX * TURN_LENTH) - TURN_DIFF)
		{// +のX方向の角
			// 種類別判定処理
			TurnInfo(D3DX_PI * ROT_LEFT);
		
			CManager::GetDebugProc()->Print(" 左向きー\n");		// デバッグ表示
		}

#if _DEBUG
		// 床の色変更
		//CObjectMesh::SetCol(CObjectMesh::COL_RED);
#endif
	}

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (PlayerPos.z < m_pos.z - DELETE_POSITION)
		{// 床が画面外へいった
			// 終了処理
			Uninit();
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CMeshField::Draw(void)
{
	// オブジェクトメッシュの描画処理
	CObjectMesh::Draw();
}

//===============================================
// 設定処理
//===============================================
void CMeshField::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	m_pos = pos;
	m_rot = rot;
	m_type = type;

	srand(timeGetTime());										// randの値を初期化
	int nPosRand = rand() % 5 + GABAGEPOS_CENTER;	// randの値を取得（位置）
	int nNumXRand = 1;											// randの値を取得（数X）
	int nNumYRand = rand() % 2 + 1;								// randの値を取得（数Y）

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// ゲームモード
		if (/*type == TYPE_CURVE_UPRIGHT || type == TYPE_CURVE_DOWNLEFT ||*/ nPosRand > GABAGEPOS_EDGE)
		{
			nNumXRand = RANDPOSITION_MAX;				// randの値を取得（数X）
			nNumYRand = RANDPOSITION_MAX;				// randの値を取得（数Y）
		}
	}

	// 配置設定
	switch (nPosRand)
	{
	case GABAGEPOS_CENTER:		// 中心
		nNumYRand = 1;
		nNumXRand = 1;
		break;

	case GABAGEPOS_EDGE:		// 端
		nNumXRand = 1;
		break;

	default:					// 横並び
		if (m_type == TYPE_STRAIGHT_H || m_type == TYPE_DEADEND_DOWN || m_type == TYPE_DEADEND_UP)
		{
			nNumYRand = 1;
		}
		else if (m_type == TYPE_STRAIGHT_W || m_type == TYPE_DEADEND_RIGHT || m_type == TYPE_DEADEND_LEFT)
		{
			nNumXRand = 1;
		}
		else
		{
			nNumYRand = 1;
			nNumXRand = 1;
		}
		break;
	}

	// ゴミのランダム配置
	for (int nCntX = 0; nCntX < nNumXRand; nCntX++)
	{
		for (int nCntY = 0; nCntY < nNumYRand; nCntY++)
		{
			// 種類別に処理を行う
			switch (m_type)
			{
			case TYPE_STRAIGHT_H:		// 縦直線
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
					break;
				}
				break;

			case TYPE_STRAIGHT_W:		// 横直線
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH,
						m_pos.y, m_pos.z + cosf(D3DX_PI * ROT_RIGHT) * nCntY * DISTANCE_WIDTH + cosf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH), 2);
					break;
				}
				break;

			case TYPE_CURVE_UPLEFT:		// カーブ（左上角）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
					break;
				}
				break;

			case TYPE_CURVE_UPRIGHT:	// カーブ（右上角）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_CURVE_DOWNRIGHT:	// カーブ（右下角）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_CURVE_DOWNLEFT:	// カーブ（左下角）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_UP:		// T字路（上）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_RIGHT:	// T字路（右）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_DOWN:	// T字路（下）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_LEFT:	// T字路（左）
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_CROSSROADS:		// 十字路
				// ゴミの位置
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// 中心
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// 端
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// 横並び
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_DEADEND_UP:		// 行き止まり（上）
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
				break;

			case TYPE_DEADEND_RIGHT:	// 行き止まり（右）
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH), 2);
				break;

			case TYPE_DEADEND_DOWN:		// 行き止まり（下）
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
				break;

			case TYPE_DEADEND_LEFT:		// 行き止まり（左）
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH), 2);
				break;
			}
		}
	}
}

//===============================================
// 種類別判定処理
//===============================================
void CMeshField::TurnInfo(float fRot)
{
	// 種類別に処理を行う
	switch (m_type)
	{
	case TYPE_STRAIGHT_H:		// 縦直線
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// まがらない
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// まがらない
		}
		break;

	case TYPE_STRAIGHT_W:		// 横直線
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// まがらない
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// まがらない
		}
		break;

	case TYPE_CURVE_UPLEFT:		// カーブ（左上角）
		if (fRot == D3DX_PI * ROT_DOWN)
		{// 初期位置から見て上へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// 初期位置から見て左へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
		}
		break;

	case TYPE_CURVE_UPRIGHT:	// カーブ（右上角）
		if (fRot == D3DX_PI * ROT_DOWN)
		{// 初期位置から見て上へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
		}
		else if (fRot == D3DX_PI * ROT_LEFT)
		{// 初期位置から見て右へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
		}
		break;

	case TYPE_CURVE_DOWNRIGHT:	// カーブ（右下角）
		if (fRot == D3DX_PI * ROT_UP)
		{// 初期位置から見て上へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
		}
		else if (fRot == D3DX_PI * ROT_LEFT)
		{// 初期位置から見て右へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
		}
		break;

	case TYPE_CURVE_DOWNLEFT:	// カーブ（左下角）
		if (fRot == D3DX_PI * ROT_UP)
		{// 初期位置から見て下へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
			}
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// 初期位置から見て左へ侵入
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
		}
		break;

	case TYPE_TJUNCTION_UP:		// T字路（上）
		if (fRot == D3DX_PI * ROT_LEFT)
		{// 初期位置から見て右へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// 真っすぐ行けて右のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// 初期位置から見て左へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// 真っすぐ行けて左のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_DOWN)
		{// 初期位置から見て上へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_CURVE);			// 真っすぐいけないが左右曲がることはできる
		}
		break;

	case TYPE_TJUNCTION_RIGHT:	// T字路（右）
		if (fRot == D3DX_PI * ROT_DOWN)
		{// 初期位置から見て上へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// 真っすぐ行けて左のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_UP)
		{// 初期位置から見て下へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// 真っすぐ行けて右のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_LEFT)
		{// 初期位置から見て右へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_CURVE);			// 真っすぐいけないが左右曲がることはできる
		}
		break;

	case TYPE_TJUNCTION_DOWN:	// T字路（下）
		if (fRot == D3DX_PI * ROT_LEFT)
		{// 初期位置から見て右へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// 真っすぐ行けて左のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// 初期位置から見て左へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// 真っすぐ行けて右のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_UP)
		{// 初期位置から見て下へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_CURVE);			// 真っすぐいけないが左右曲がることはできる
		}
		break;

	case TYPE_TJUNCTION_LEFT:	// T字路（左）
		if (fRot == D3DX_PI * ROT_DOWN)
		{// 初期位置から見て上へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// 真っすぐ行けて右のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_UP)
		{// 初期位置から見て下へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// 真っすぐ行けて左のみ曲がることができる
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// 初期位置から見て左へ侵入
			// 入力方向変更処理
			TurnInput(DESTINATION_CURVE);			// 真っすぐいけないが左右曲がることはできる
		}
		break;

	case TYPE_CROSSROADS:		// 十字路
		// 入力方向変更処理
		TurnInput(DESTINATION_STRAIGHT_CURVE);		// 真っすぐいけて左右曲がることもできる
		break;

	case TYPE_DEADEND_UP:		// 行き止まり（上）
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		break;

	case TYPE_DEADEND_RIGHT:	// 行き止まり（右）
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		break;

	case TYPE_DEADEND_DOWN:		// 行き止まり（下）
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		break;

	case TYPE_DEADEND_LEFT:		// 行き止まり（左）
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// 折り返し
		}
		break;
	}
}

//===============================================
// 入力方向変更処理
//===============================================
void CMeshField::TurnInput(DESTINATION destination)
{
	if (CManager::GetKeyboardInput()->GetPress(DIK_Q) == true || CManager::GetKeyboardInput()->GetPress(DIK_Z) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LEFT, 0))
	{// Aキー入力
		if (destination == DESTINATION_STRAIGHT_CURVE || destination == DESTINATION_STRAIGHT_LEFT || destination == DESTINATION_CURVE || destination == DESTINATION_LEFT)
		{// 左に曲がれる
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// 左へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// 左へ曲がる
			}
		}
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_E) == true || CManager::GetKeyboardInput()->GetPress(DIK_C) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RIGHT, 0))
	{// Dキー入力
		if (destination == DESTINATION_STRAIGHT_CURVE || destination == DESTINATION_STRAIGHT_RIGHT || destination == DESTINATION_CURVE || destination == DESTINATION_RIGHT)
		{// 右に曲がれる
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
			}
		}
	}
	else
	{// 入力なし
		if (destination == DESTINATION_STRAIGHT_CURVE || destination == DESTINATION_STRAIGHT
			|| destination == DESTINATION_STRAIGHT_RIGHT || destination == DESTINATION_STRAIGHT_LEFT)
		{// 真っすぐ進める
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// まがらない
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// まがらない
			}
		}
		else
		{// 進めない
			switch (destination)
			{
			case DESTINATION_LEFT:		// 真っすぐいけないが左には曲がれる
				if (CManager::GetMode() == CScene::MODE_TITLE)
				{
					CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// 左へ曲がる
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{
					CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// 左へ曲がる
				}
				break;

			case DESTINATION_RIGHT:		// 真っすぐいけないが右には曲がれる
				if (CManager::GetMode() == CScene::MODE_TITLE)
				{
					CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{
					CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
				}
				break;

			case DESTINATION_CURVE:		// 真っすぐいけないが左右曲がれる
				// ランダムで左右どちらに曲がるか判別
				srand(timeGetTime());											// randの値を初期化
				int nRand = rand() % CPlayer::TURN_LEFT + CPlayer::TURN_RIGHT;	// randの値を取得

				switch (nRand)
				{
				case CPlayer::TURN_RIGHT:		// 右へ曲がる
					if (CManager::GetMode() == CScene::MODE_TITLE)
					{
						CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
					}
					else if (CManager::GetMode() == CScene::MODE_GAME)
					{
						CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// 右へ曲がる
					}
					break;

				case CPlayer::TURN_LEFT:		// 左へ曲がる
					if (CManager::GetMode() == CScene::MODE_TITLE)
					{
						CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
					}
					else if (CManager::GetMode() == CScene::MODE_GAME)
					{
						CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// 左へ曲がる
					}
					break;
				}
				break;
			}
		}
	}
}