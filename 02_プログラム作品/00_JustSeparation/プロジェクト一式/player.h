//=========================================================
//
// プレイヤー処理 [player.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _PLAYER_H_     // このマクロ定義がされてなかったら
#define _PLAYER_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"
#include "fileload.h"

//===============================================
// 前方宣言
//===============================================
class CModel;
class CMotion;

//===============================================
// マクロ定義
//===============================================
#define PLAYER_SIZEX		(30)		// 幅（半分）
#define PLAYER_SIZEY		(105)		// 高さ（半分）
#define MOVE_GRAVITY		(0.75f)		// 重力
#define LAND_POS			(0.0f)		// 地面
#define ROT_LEFTUP			(-0.25f)	// 左上の角度
#define ROT_LEFT			(-0.5f)		// 左の角度
#define ROT_LEFTDOWN		(-0.75f)	// 左下の角度
#define ROT_RIGHTUP			(0.25f)		// 右上の角度
#define ROT_RIGHT			(0.5f)		// 右の角度
#define ROT_RIGHTDOWN		(0.75f)		// 右下の角度
#define ROT_UP				(0.0f)		// 上の角度
#define ROT_DOWN			(1.0f)		// 下の角度
#define TIME_FEVER			(30)		// フィーバーになる時間

//===============================================
// プレイヤークラス
//===============================================
class CPlayer : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CPlayer();						// デフォルトコンストラクタ
	CPlayer(int nPriority = 3);		// オーバーロードされたコンストラクタ
	~CPlayer();						// デストラクタ

	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	// 待機
		MOTIONTYPE_MOVE,		// 移動
		MOTIONTYPE_ACTION,		// アクション
		MOTIONTYPE_JUMP,		// ジャンプ
		MOTIONTYPE_LANDING,		// 着地
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	// 方向変更
	typedef enum
	{
		TURN_NONE = 0,		// なし
		TURN_RIGHT,			// 右へ曲がる
		TURN_LEFT,			// 左へ曲がる
		TURN_BACK,			// 折り返し
		TURN_MAX
	}TURN;

	static CPlayer *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetJump(const bool bJump);
	D3DXVECTOR3 GetSize(void) { return m_vtxMax; }
	void SetTurn(TURN turn);
	TURN GetTurn(void){return m_turnType;}
	TURN GetTurnOld(void) { return m_turnTypeOld; }
	void SetMotion(MOTIONTYPE type);

private:	// 自分のみアクセス可能 [アクセス指定子]
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_posOld;					// 前回の位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_rotDest;					// 目的の向き
	D3DXVECTOR3 m_move;						// 移動量
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_vtxMax;					// モデルの最大値
	D3DXVECTOR3 m_vtxMin;					// モデルの最小値

	float m_speed;							// 移動速度変更用
	bool m_bJump;							// ジャンプしたかどうか
	float m_fRotBullet;						// 照準の向き
	float m_fRotDiff;						// 目的の向きまでの差分
	TURN m_turnType;						// 曲がる向き
	TURN m_turnTypeOld;						// 前回の曲がる向き
	int m_nTurnCounter;						// 曲がっている時間
	int m_nParticleCounter;					// パーティクル発生時間

	CModel *m_apModel[MAX_MODEL];			// モデル（パーツ）へのポインタ
	int m_nNumModel;						// モデル（パーツ）の総数
	CMotion *m_pMotion;						// モーションのへのポインタ
};

#endif