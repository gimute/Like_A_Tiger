#pragma once

class Player;

class MiniMap : public IGameObject
{
	//戦闘エリアの座標とマップ上に表示するUIのセット
	struct BattlePointUIData
	{
		std::shared_ptr<UIImage> battlePointIcon;
		Vector3 battleAreaPos;
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

	//戦闘ポイント追加
	//とりあえず座標だけ受け取る
	void AddBattlePoint(Vector3 pos);

private:
	std::shared_ptr<UICanvas> m_canvas;
	std::shared_ptr<UIImage> m_mapImage;

	std::shared_ptr<UIImage> m_playerIcon;
	Player* m_player = nullptr;

	std::vector<BattlePointUIData> m_battlePointUIDataList;


private:
	void CalcBattlePointUIPos();

};

