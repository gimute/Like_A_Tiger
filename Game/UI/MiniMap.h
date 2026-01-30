#pragma once

class Player;

class MiniMap : public IGameObject
{
	//戦闘エリアの座標とマップ上に表示するUIのセット
	struct BattleAreaUIData
	{
		std::shared_ptr<UIImage> battleAreaIcon;
		Vector3 battleAreaPos;
	};

	//エネミーアイコン生成用のデータ構造体
	struct EnemyIconData
	{
		std::string imageFilePath;
		float iconSize;
	};

public:
	MiniMap();
	~MiniMap();

	//更新処理
	void Update();

	//描画処理
	void Render(RenderContext& rc);

	//UIの位置設定
	void SetPosition(Vector3 pos)
	{
		m_canvas->m_transform.m_localPosition = pos;
	}

	//UIのスケール設定
	void SetScale(Vector3 scale)
	{
		m_canvas->m_transform.m_localScale = scale;
	}

	//プレイヤー登録
	void SetPlayer(Player* player);

	//登録されているバトルポイントの数を取得
	int GetBattleAreaNum()
	{
		return m_battleAreaUIDataList.size();
	}

	//バトルエリアの座標や数の更新
	void ButtleAreaDataUpdate();

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_mapImage;

	std::shared_ptr<UIImage> m_playerIcon;
	Player* m_player = nullptr;

	std::vector<BattleAreaUIData> m_battleAreaUIDataList;

	EnemyIconData m_enemyIconData;

private:
	void CalcBattleAreaUIPos();

};

