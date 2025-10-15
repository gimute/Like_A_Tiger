#pragma once

template<typename TStatus>
static TStatus* CreateStatus()
{
	auto* status = new TStatus();
	status->Setup();
	return status;
}

//アクターのステータス用基底クラス
class ActorStatus
{
public:
	//コンストラクタ
	ActorStatus() = default;
	//デストラクタ
	~ActorStatus() = default;

	//ステータス用のセットアップ関数※必ず作成する
	virtual void Setup() = 0;
};

//キャラクターのステータス
class CharacterStatus : public ActorStatus
{
protected:

public:
	//コンストラクタ
	CharacterStatus() = default;
	//デストラクタ
	~CharacterStatus() = default;

	//セットアップ関数
	virtual void Setup() override {}
public:
};

