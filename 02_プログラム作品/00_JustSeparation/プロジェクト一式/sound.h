//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 阿部翔大郎
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=========================================================
// サウンドクラス
//=========================================================
class CSound
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CSound();		// デフォルトコンストラクタ
	~CSound();		// デストラクタ

	// サウンド一覧
	typedef enum
	{
		LABEL_BGM_TITLE = 0,	// BGM（タイトル）
		LABEL_BGM_TUTORIAL,		// BGM（チュートリアル）
		LABEL_BGM_GAME,			// BGM（ゲーム）
		LABEL_BGM_FEVER,		// BGM（フィーバー）
		LABEL_BGM_SCOREATTACK,	// BGM（スコアアタック）
		LABEL_BGM_RANKING,		// BGM（ランキング）
		LABEL_SE_CURVE,			// カーブ
		LABEL_SE_SEPARATION,	// 分別
		LABEL_SE_JUSTDUST,		// JUSTDUST
		LABEL_SE_SUCCESS,		// 成功
		LABEL_SE_FAILURE,		// 失敗
		LABEL_SE_ROCKON,		// ロックオン
		LABEL_SE_ABLE,			// ロックオン可
		LABEL_SE_DISABLE,		// ロックオン不可
		LABEL_SE_DUMPSTER,		// ゴミステーション
		LABEL_SE_FEVER,			// フィーバー
		LABEL_SE_PAUSE,			// ポーズ起動
		LABEL_SE_PAUSE_CANCEL,	// ポーズキャンセル
		LABEL_SE_PAUSE_CURSOR,	// ポーズのカーソル移動
		LABEL_SE_PAUSE_ENTER,	// ポーズ決定
		LABEL_SE_TITLE_ENTER,	// タイトル決定
		LABEL_MAX,
	} LABEL;

	// サウンド情報の構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} INFO;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);
	bool GetPlay(LABEL label);

private:	// 自分のみアクセス可能 [アクセス指定子]
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;		// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX] = {};						// オーディオデータサイズ
	static const INFO m_aInfo[LABEL_MAX];					// サウンドの情報
};

#endif