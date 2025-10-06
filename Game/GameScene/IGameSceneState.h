#pragma once
#include "CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class IGameSceneState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	IGameSceneState() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IGameSceneState() = default;
	/// <summary>
	/// 純粋仮想関数、ステートに入るときに呼ばれる
	/// </summary>
	virtual void EnterScene() = 0;
	/// <summary>
	/// 純粋仮想関数、ステートから出るときに呼ばれる
	/// </summary>
	virtual void UpdateScene() = 0;
	/// <summary>
	/// 純粋仮想関数、ステートから出るときに呼ばれる
	/// </summary>
	virtual void ExitScene() = 0;
	/// <summary>
	/// 純粋仮想関数、ステート変更要求
	/// </summary>
	/// <param name="nextState"></param>
	/// <returns></returns>
	virtual bool ReqestSceneState(uint32_t& nextState) = 0;
};

