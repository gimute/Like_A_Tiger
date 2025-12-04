#pragma once
#include "Actor\Character.h"
#include "Actor\YakuzaComponents\YakuzaStateMachine.h"

class YakuzaCharacter : public Character
{
private:
	std::unique_ptr <YakuzaStateMachine> m_yakuzaStateMachine;
public:
	//コンストラクタ
	YakuzaCharacter(YakuzaCharacter* hasCharacter) 
	{
		m_yakuzaStateMachine = std::make_unique<YakuzaStateMachine>(hasCharacter);
	}
	//デストラクタ
	~YakuzaCharacter() = default;

	//スタート関数
	virtual bool Start() override;
	//更新関数
	virtual void Update() override;
	//描画関数
	virtual void Render(RenderContext& rc) override;

	inline void InitBodyCollision(YakuzaCharacter* useCharacter,const char* name)
	{
		m_bodyCollision = NewGO<CollisionObject>(0, "collision");

		m_bodyCollision->CreateSphere(
			GetPosition(),
			GetRotation(),
			25.0f,
			useCharacter
		);

		m_bodyCollision->SetName(name);

		m_bodyCollision->SetIsEnableAutoDelete(false);
	}

	inline void SpwanAttackCollision(
		YakuzaCharacter* useCharacter,
		const Vector3& position,
		const Quaternion rotation,
		float size,
		const char* name)
	{
		m_attackCollision = NewGO<CollisionObject>(0, "collision");

		m_bodyCollision->CreateSphere(
			position,
			rotation,
			size,
			useCharacter
		);

		m_bodyCollision->SetName(name);

		m_bodyCollision->SetIsEnableAutoDelete(false);
	}

	inline void DeleteAttackCollision()
	{
		m_bodyCollision->Dead();
	}

	//本体の当たり判定
	CollisionObject* m_bodyCollision = nullptr;
	//攻撃の当たり判定
	CollisionObject* m_attackCollision = nullptr;
public:
	inline YakuzaStateMachine& GetYakuzaStateMachine() 
	{
		return *m_yakuzaStateMachine; 
	}
	inline void SetBodyCollision()
	{
		m_bodyCollision->SetIsEnable(false);
	}
};

