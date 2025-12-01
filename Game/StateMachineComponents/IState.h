#pragma once

class IStateBase 
{
public:
	//コンストラクタ
	IStateBase() = default;
	//デストラクタ
	~IStateBase() = default;

	//純粋仮想関数、ステートイン
	virtual void OnEnter() = 0;
	//純粋仮想関数、ステートアップデート
	virtual void OnUpdate() = 0;
	//純粋仮想関数、ステートアウト
	virtual void OnExit() = 0;

	
};

