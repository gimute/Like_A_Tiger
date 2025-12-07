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

	inline CollisionObject& GetBodyCollision()
	{
		return *m_bodyCollision;
	}

	inline void SetAttackCollisionName(const char* name)
	{
		m_attackCollisionName = name;
	}

	inline void SpwanAttackCollision(
		YakuzaCharacter* useCharacter,
		float size
	)
	{
		if (m_attackCollision)
		{
			return;
		}

		m_attackCollision = NewGO<CollisionObject>(0, "collision");

		Vector3 iPos = GetPosition();
		Vector3 collisionPos = iPos + GetForward() * 20.0f;
		collisionPos.y = 60.0f;

		m_attackCollision->CreateSphere(
			collisionPos,
			GetRotation(),
			size,
			useCharacter
		);

		m_attackCollision->SetName(m_attackCollisionName);

		m_attackCollision->SetIsEnableAutoDelete(false);
	}

	inline void DeleteAttackCollision()
	{
		if (!m_attackCollision)
		{
			return;
		}
		DeleteGO(m_attackCollision);
		m_attackCollision = nullptr;
	}

	//本体の当たり判定
	CollisionObject* m_bodyCollision = nullptr;
	//攻撃の当たり判定
	CollisionObject* m_attackCollision = nullptr;
	//攻撃のコリジョンネーム
	const char* m_attackCollisionName = "";
	//攻撃が当たったかどうか
	bool m_isAttackCollisionHit = false;
protected:
	const char* m_modelFilePath = nullptr;

	int m_maxAnimationNum = 0;

	std::vector<Character::AnimationData> m_animationData;
public:
	inline YakuzaStateMachine& GetYakuzaStateMachine() { return *m_yakuzaStateMachine; }

	inline void SetBodyCollision() { m_bodyCollision->SetIsEnable(false); }

	inline void SetIsAttackCollisionHit(bool isHit) { m_isAttackCollisionHit = isHit; }

	inline bool GetIsAttackCollisionHit() { return m_isAttackCollisionHit; }

	//モデル初期設定
	inline void InitYakuzaModel(const char* filePath, std::vector<Character::AnimationData>& ptr)
	{
		m_modelFilePath = filePath;

		m_maxAnimationNum = ptr.size();

		m_animationData = ptr;
	}
};

