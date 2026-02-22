#pragma once

class EnemyInfoGroupe;
class Enemy;

enum LockOnInputDir
{
	enLockOnIn,
	enLockOnRight,
	enLockOnLeft,
	enInputNone
};

class PlayerCameraLockOn
{
private:
	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static PlayerCameraLockOn* m_instance;
	//コンストラクタ
	PlayerCameraLockOn() = default;
	//コピー禁止
	PlayerCameraLockOn(const PlayerCameraLockOn&) = delete;
	//代入禁止にする
	PlayerCameraLockOn& operator=(const PlayerCameraLockOn) = delete;
public:
	//デストラクタ
	~PlayerCameraLockOn() = default;
	//インスタンス取得
	static PlayerCameraLockOn* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new PlayerCameraLockOn();
		}

		return m_instance;
	}

	//ロックオン初期化
	void InitPlayerLockOn();

	//ロックオン対象敵グループ登録
	inline void RegisterLockOnEnemyGroupe(EnemyInfoGroupe* registerGroupe)
	{
		m_inBattleEnemys = registerGroupe;
	}

	//ロックオン開始
	inline void StartLockOn()
	{
		m_LockOnFlag = true;

		m_lockOnInputState = LockOnInputDir::enLockOnIn;
	}

	inline void SwitchLockOn(bool inputRight)
	{
		if (inputRight)
		{
			m_lockOnInputState = LockOnInputDir::enLockOnRight;
		}
		else
		{
			m_lockOnInputState = LockOnInputDir::enLockOnLeft;
		}
	}

	//ロックオン終了
	inline void EndLockOn()
	{
		m_LockOnFlag = false;

		m_lockOnInputState = LockOnInputDir::enInputNone;

		m_lockOnCurrent = nullptr;
	}

	//現在ロックオン中かどうかを取得
	inline bool IsLockOn()
	{
		return m_LockOnFlag;
	}

	//ロックオン時カメラ向き計算
	bool CalcCameraLockOn(
		Vector3& outValue,
		const Vector3& playerPos,
		LockOnInputDir inputDir
	);

private:
	//ロックオン開始時のロックオン敵を取得
	Enemy* LockOnStart(
		const Vector3& camPos,
		const Vector3& camForward
	);
	//ロックオン中対象切り替えの敵を取得
	Enemy* LockOnSwitch(
		const Vector3& camPos,
		const Vector3& camRight,
		Enemy* currentEnemy,
		bool inputRight
	);
	//ロックオン対象捜索関数
	Enemy* SearchLockOnEnemy(std::function<float(Enemy*)> scoringFunc);
	//ロックオンフラグ
	bool m_LockOnFlag = false;
	//現在戦闘中の敵集団情報
	EnemyInfoGroupe* m_inBattleEnemys = nullptr;
	//現在ロックオンしている敵
	Enemy* m_lockOnCurrent = nullptr;
	//現在の入力状態
	LockOnInputDir m_lockOnInputState = LockOnInputDir::enInputNone;
};

