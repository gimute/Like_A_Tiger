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


	//いくつUIが登録されるごとに改行するか、0なら改行しない
	int m_newLineNum = 0;
	//改行でずらす量ベクトル
	Vector3 m_newLineOffsetPos;

public:
	//座標の更新
	void Update();

	//UI追加
	void AddUI(UICanvas::RefUIBasePtr ui)
	{
		m_uiList.push_back(ui);
	}

	//UIを取り除く
	void RemoveUI(UICanvas::RefUIBasePtr ui)
	{
		auto newEnd = std::remove(m_uiList.begin(), m_uiList.end(), ui);
		m_uiList.erase(newEnd, m_uiList.end());
	}

	//ずらすベクトルを設定
	void SetOffset(Vector3 offset)
	{
		m_offsetPosition = offset;
	}

	//改行する際にずらすベクトル
	void SetNewLineOffset(Vector3 offset)
	{
		m_newLineOffsetPos = offset;
	}

	//UIいくつごとに改行するか設定、0に設定すると改行しない
	void SetNewLineNum(int newLineNum)
	{
		m_newLineNum = newLineNum;
	}
};

