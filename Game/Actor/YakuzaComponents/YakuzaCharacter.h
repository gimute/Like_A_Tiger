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
	~YakuzaCharacter()
	{
		if (m_bodyCollision)
		{
			DeleteGO(m_bodyCollision);
			m_bodyCollision = nullptr;
		}
		
		DeleteAttackCollision();
	}

	//スタート関数
	virtual bool Start() override;
	//更新関数
	virtual void Update() override;
	//描画関数
	virtual void Render(RenderContext& rc) override;

	virtual void YakuzaCharacterDeadProcces() = 0;

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

		//m_attackCollision->SetIsEnable(true);

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

	inline bool IsAttackCollisionActive()
	{
		if (m_attackCollision)
		{
			return true;
		}
		return false;
	}

protected:
	//本体の当たり判定
	CollisionObject* m_bodyCollision = nullptr;
	//攻撃の当たり判定
	CollisionObject* m_attackCollision = nullptr;
	//攻撃のコリジョンネーム
	const char* m_attackCollisionName = "";
	//無敵時間
	float m_invincibleTimeLeft = 0.0f;
	//設定された時間
	float m_invincibleDuration = 0.0f;
	//無敵フラグ
	bool m_isInvincible = 0.0f;

	const char* m_modelFilePath = nullptr;

	int m_maxAnimationNum = 0;

	std::vector<Character::AnimationData> m_animationData;
	//このYakuzaのMAXHp
	float m_yakuzaMaxHp = 150.0f;
	//このYakuzaのHp
	float m_yakuzaCurrentHp = m_yakuzaMaxHp;
public:
	inline YakuzaStateMachine& GetYakuzaStateMachine() { return *m_yakuzaStateMachine; }

	inline void SetBodyCollision() { m_bodyCollision->SetIsEnable(false); }

	inline bool GetIsInvicible() { return m_isInvincible; }

	inline void SetHP(float max)
	{
		m_yakuzaMaxHp = max;
		m_yakuzaCurrentHp;
	}

	inline void TakeDamage(float amount)
	{
		m_yakuzaCurrentHp -= amount;
	}

	//HPを見てDeadかどうか確認
	inline bool IsCharacterHpDead()
	{
		if (m_yakuzaCurrentHp <= 0.0f)
		{
			return true;
		}

		return false;
	}

	inline float GetYakuzaCurrentHp() { return m_yakuzaCurrentHp; }

	inline float GetYakuzaMaxHp() { return m_yakuzaMaxHp; }

	inline void StartInvincible(float sec) 
	{
		m_invincibleTimeLeft = sec;
		m_isInvincible = true;
	}

	inline void UpdateInvincibleTime()
	{
		if (!m_isInvincible)
		{
			return;
		}

		m_invincibleTimeLeft -= g_gameTime->GetFrameDeltaTime();

		if (m_invincibleTimeLeft <= 0.0f)
		{
			m_isInvincible = false;
			m_invincibleTimeLeft = 0.0f;
		}
	}

	//モデル初期設定
	inline void InitYakuzaModel(const char* filePath, std::vector<Character::AnimationData>& ptr)
	{
		m_modelFilePath = filePath;

		m_maxAnimationNum = ptr.size();

		m_animationData = ptr;
	}

	inline void SetAttackCollisionIsEnable(bool setIs) 
	{
		if (!m_attackCollision)
		{
			return;
		}
		m_attackCollision->SetIsEnable(setIs); 
	}
};

