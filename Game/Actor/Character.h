#pragma once
#include "Actor.h"
#include "StateMachineComponents\IStateMachine.h"

class CharacterStatus;

//キャラクターとなる要素を持ったクラス
class Character : public Actor
{
private:

	//ステートマシンのポインタ格納変数
	std::unique_ptr<IStateMachine> m_stateMachine = nullptr;

	//アニメーションのリスト
	std::vector<AnimationClip*> m_animetionList;

protected:

	//アニメーション配列類
	struct AnimationData
	{
		//ファイルパス
		const char* fileName = nullptr;
		//ループするかしないか
		bool isLoop = false;
	};
	//キャラクターコントローラー
	CharacterController m_characterController;

public:
	//コンストラクタ
	Character() = default;
	//デストラクタ
	~Character() = default;

	//スタート関数
	virtual bool Start() override;
	//更新関数
	virtual void Update() override;
	//描画関数
	virtual void Render(RenderContext& rc) override;
	//指定クラスのステートマシンを作成し登録
	template<typename StateMachine, typename... Args>
	inline void MakeStateMachineUniquePtr(Args&&... args)
	{
		m_stateMachine = std::make_unique<StateMachine>(std::forward<Args>(args)...);
	}
	//アニメーションのリストを登録
	template<size_t N>
	inline void InitAnimationClipList(int animationMaxNum,const AnimationData (&animationData)[N])
	{
		uint8_t uint8AnimationMax = static_cast<uint8_t>(animationMaxNum);

		//アニメーションの最大数でリストをリサイズ
		m_animetionList.resize(uint8AnimationMax);

		for (int i = 0;i < uint8AnimationMax;i++)
		{
			auto* newAnimationClip = new AnimationClip();
			newAnimationClip->Load(animationData[i].fileName);
			newAnimationClip->SetLoopFlag(animationData[i].isLoop);
			m_animetionList[i] = newAnimationClip;
		}
	}

	inline void InitModelRender(const char* filePath)
	{
		m_modelRender.Init(filePath,*m_animetionList.data(),m_animetionList.size());
	}
public:

	IStateMachine* GetStateMachine()
	{
		return m_stateMachine.get();
	}

	//キャラクターのステータス取得関数
	CharacterStatus* GetStatus()
	{
		return dynamic_cast<CharacterStatus*>(m_actorStatus);
	}

	//当たり判定チェック用関数
	CharacterController* GetCharacterController()
	{
		return &m_characterController;
	}

	//アニメーション再生関数
	void PlayAnimetionClip(int animationNum,float interpolateTime)
	{
		m_modelRender.PlayAnimation(animationNum, interpolateTime);
	}
};

