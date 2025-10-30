#pragma once

class Player;

class PlayerController : public IGameObject
{
public:

	PlayerController() {}

	~PlayerController() {}

	//スタート関数
	bool Start() override;
	//アップデート関数
	void Update() override;

	//ターゲット変数
	Player* m_controllTarget = nullptr;
	//ターゲット決定関数
	void TargetSet(Player* target) { m_controllTarget = target; }
private:
	//左スティックの入力量を取得
	Vector3 GetStickL() const;
	//右スティックの入力量を取得
	Vector3 GetStickR() const;

};

