#pragma once

//UIのベース、インターフェース
class UIBase : public Noncopyable
{
public:
	Transform m_transform;

protected:
	bool isStart = false;
	bool isUpdate = true;
	bool isDraw = false;

public:
	UIBase() {}
	virtual ~UIBase() {}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;
};

/// <summary>
/// UIの生成、管理、親子付けをするためのクラス
/// </summary>
class UICanvas
{
public:
	Transform m_transform;

private:

	std::vector<UIBase*> m_uiList;

public:
	UICanvas();
	~UICanvas();


	bool Start();
	/// <summary>
	/// 登録されているUIの更新処理を呼び出す
	/// </summary>
	void Update();
	/// <summary>
	/// 登録されているUIの描画処理を呼び出す
	/// </summary>
	/// <param name="rc"></param>
	void Render(RenderContext& rc);


public:
	/// <summary>
	/// この関数で生成したUIはデストラクタでdeleteするのでほっといても大丈夫なはず
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template <typename T>
	T* CreateUI()
	{
		//UIBaseを継承しているか確認
		if constexpr (!(std::is_base_of_v<UIBase, T>))
		{
			return nullptr;
		}

		//UI生成
		T* ui = new T();
		//Canvasのtransformと親子付け
		ui->m_transform.SetParent(&m_transform);
		m_uiList.push_back(ui);

		ui->Start();

		return ui;
	}
};



/// <summary>
/// 画像を使うUI
/// </summary>
class UIImage : public UIBase
{
	template<typename T>
	friend T* UICanvas::CreateUI();

protected:
	SpriteRender m_spriteRender;

protected:
	UIImage();
	~UIImage();

public:

	virtual void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
};


/// <summary>
/// 伸び縮みするUI(例：HPバー)
/// 何らかの値と連動させて使う、
/// 最大値と現在値をもとに割合を計算して画像を伸び縮みさせる
/// </summary>
class UIGauge : public UIImage
{
	template<typename T>
	friend T* UICanvas::CreateUI();

private:
	float m_maxValue = 1.0f;
	float m_Value = 0.0f;

private:
	UIGauge();
	~UIGauge();

public:
	virtual void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

	void SetMaxValue(float max)
	{
		m_maxValue = max;
	}

	void SetValue(float nowValue)
	{
		m_Value = nowValue;
		if (m_Value < 0.0f)
		{
			m_Value = 0.0f;
		}
	}
};


/// <summary>
/// テキストUI
/// Transformのスケールはxの値を参照する
/// </summary>
class UIText : public UIBase
{
	template<typename T>
	friend T* UICanvas::CreateUI();

protected:
	FontRender m_fontRender;


private:
	UIText() {};
	~UIText() {};


public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
	virtual void SetText(const wchar_t* text);
	virtual void SetColor(float r, float g, float b, float a);
	virtual void SetColor(const Vector4& color);
	virtual void SetPivot(float x, float y);
	virtual void SetPivot(const Vector2& pivot);
};

