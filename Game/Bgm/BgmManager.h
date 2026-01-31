#pragma once

#include "Battle\BattleManager.h"

enum BgmFadeState
{
	en_fadein,
	en_fadeout,
	en_arrayInitNum,
	en_ready
};

struct BgmFadeFormat
{
	//BGM切り替え時にフェードアウトするかのフラグ
	bool m_bgmFadeOutFlag = false;
	//BGM切り替え時にフェードインするかのフラグ
	bool m_bgmFadeInFlag = false;

	float m_bgmFadeOutSpeed = 1.0f;

	float m_bgmFadeInSpeed = 1.0f;

	int m_preNextBgmNo = -1;

	BgmFadeFormat() = default;

	BgmFadeFormat(
		const bool bgmFadeOutFlag,
		const bool bgmFadeInFlag,
		const float bgmFadeOutSpeed,
		const float bgmFadeInSpeed
	)
		: m_bgmFadeOutFlag(bgmFadeOutFlag)
		, m_bgmFadeInFlag(bgmFadeInFlag)
		, m_bgmFadeOutSpeed(bgmFadeOutSpeed)
		, m_bgmFadeInSpeed(bgmFadeInSpeed)
	{
	}
};

class BgmManager
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static BgmManager* m_instance;
	//コンストラクタ
	BgmManager()
	{
		BattleManager::GetInstance()->RegisterBattleStartCallBack(
			[&](const BattleStartEventInfo& eventInfo)
			{

			}
		);
	}
	//コピー禁止
	BgmManager(const BgmManager&) = delete;
	//代入禁止にする
	BgmManager& operator=(const BgmManager) = delete;
public:
	//デストラクタ
	~BgmManager() = default;
	//インスタンス取得
	static BgmManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new BgmManager();
		}
		return m_instance;
	}
	//BGM再生依頼
	void RequestPlayBgm(int kindBgmNo);
	//BGM停止依頼
	void RequestStopBgm();
	//BGM切り替え依頼
	void RequestChangeBgm(BgmFadeFormat format);
	//BGM更新
	void Update();
private:

	void BgmFadeOutUpdate();

	//BGMフェードフラグ
	bool m_bgmFadeFlag[en_arrayInitNum] = {false,false};
	//BGMフェードイン、アウトスピード
	float m_bgmFadeSpeed[en_arrayInitNum] = { 1.0f,1.0f };
	//BGMフェードイン、アウト線形補間用割合変数
	float m_bgmFadeLerpRate = 0.0f;
	//BGM切り替えステート
	BgmFadeState m_bgmChangeState = en_ready;
	//次に再生するBGM番号
	int m_nextBgmNo = -1;

	//線形補間関数
	const float Leap(const float a, const float b, const float t)
	{
		return (1.0f - t) * a + t * b;
	}
};

