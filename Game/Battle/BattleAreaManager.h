#pragma once
class EnemyIntroduction;

//戦闘エリア構造体
struct BattleArea
{
	//識別ID
	int m_id = -1;
	//中心地点
	Vector3 m_areaCenter = Vector3::Zero;
	//戦闘エリア半径
	float m_areaRadius = 0.0f;

	//この戦闘エリアに入っているかどうか
	bool IsBattleAreaInside(const Vector3& pos)
	{
		//座標へ伸びるベクトル
		Vector3 toPosVec = pos - m_areaCenter;
		//座標へ延びるベクトルの2乗を計算
		float toPosSq = toPosVec.LengthSq();
		//戦闘エリア半径の2乗を計算
		float areaSq = m_areaRadius * m_areaRadius;

		if (toPosSq <= areaSq)
		{
			return true;
		}

		return false;
	}

	BattleArea(
		int id,
		Vector3 position,
		float radius
	)
		: m_id(id)
		, m_areaCenter(position)
		, m_areaRadius(radius)
	{ }

	BattleArea() = default;
};

class BattleAreaManager
{
private:
	EnemyIntroduction* m_enemyIntroduction = nullptr;

	//シングルトーンパターンを採用するため、コンストラクタをprivateにする
	//インスタンス
	static BattleAreaManager* m_instance;
	//コンストラクタ
	BattleAreaManager() {};
	//コピー禁止
	BattleAreaManager(const BattleAreaManager&) = delete;
	//代入禁止にする
	BattleAreaManager& operator=(const BattleAreaManager) = delete;
public:
	//デストラクタ
	~BattleAreaManager() = default;
	//インスタンス取得
	static BattleAreaManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new BattleAreaManager();
		}

		return m_instance;
	}

	//エリア生成
	int CreateArea(const Vector3& center, float radius);
	//エリア削除
	void RemoveArea(int id);
	//エリアマネージャーリセット
	void ResetAreaManager();

	//更新
	void Update(const Vector3& playerPos);

	//エリア侵入時リスナー登録
	inline void RegisterOnEnterListener(
		std::function<void(const BattleArea&)> listener
	) 
	{
		m_onEnterListeners.push_back(listener);
	}

	//バトルエリアの座標リストの取得
	std::vector<Vector3> GetBattleAreaPositions();

private:
	//エリア状態構造体
	struct AreaState
	{
		//エリア構造体
		BattleArea m_battleArea;
		//プレイヤーが侵入しているかどうか
		bool m_isPlayerInside = false;

		AreaState(
			BattleArea battleArea
		) 
			: m_battleArea(battleArea)
		{}

		AreaState() = default;
	};

	//戦闘エリア侵入をバトルマネージャーに通知
	void NotifyOnBattleManagerEnter(AreaState areaState);

	//エリアの識別子カウント
	int m_areaCountNum = 0;
	//エリアのリスト
	std::vector<AreaState> m_areas;
	//リスナーのリスト
	std::vector<std::function<void(const BattleArea&)>> m_onEnterListeners;
};

