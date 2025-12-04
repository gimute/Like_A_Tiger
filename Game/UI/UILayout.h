#pragma once

/// <summary>
/// UIを一定の間隔で並べるクラス
/// </summary>
class UILayout
{
private:
	//並べるUI
	std::vector<UICanvas::RefUIBasePtr> m_uiList;

	//ずらす量ベクトル
	Vector3 m_offsetPosition;

	//最初のUIの座標
	Vector3 m_position;

public:
	//座標の更新
	void Update();

	//UI追加
	void AddUI(UICanvas::RefUIBasePtr ui)
	{
		m_uiList.push_back(ui);
	}

	//ずらすベクトルを設定
	void SetOffset(Vector3 offset)
	{
		m_offsetPosition = offset;
	}

};

