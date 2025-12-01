#pragma once

//UIのベース、インターフェース
class UIBase : public Noncopyable
{
public:
	Transform m_transform;

protected:
	bool m_isStart = false;
	bool m_isUpdate = true;
	bool m_isDraw = false;

public:
	UIBase() {}
	virtual ~UIBase() {}

	virtual bool Start() = 0;
	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;


	/// <summary>
	/// 描画の制御
	/// </summary>
	/// <param name="flag"></param>
	void SetDrawFlag(bool flag)
	{
		m_isDraw = flag;
	}

	/// <summary>
	/// 更新処理の制御
	/// </summary>
	/// <param name="flag"></param>
	void SetUpdateFlag(bool flag)
	{
		m_isUpdate = flag;
	}
};

/// <summary>
/// UIの生成、管理、親子付けをするためのクラス
/// </summary>
class UICanvas
{
	using RefUIBasePtr = std::shared_ptr<UIBase>;

public:
	Transform m_transform;

private:
	std::vector<RefUIBasePtr> m_uiList;
	bool m_isVisible = true;	//UIの表示、非表示
	bool m_isUpdate = true;		//UIの更新処理の有効無効

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

	/// <summary>
	/// この関数で生成したUIはデストラクタでdeleteするのでほっといても大丈夫なはず
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template <typename T>
	std::shared_ptr<T> CreateUI()
	{
		//UIBaseを継承しているか確認
		if constexpr (!(std::is_base_of_v<UIBase, T>))
		{
			return nullptr;
		}

		//UI生成
		auto ui = std::make_shared<T>();

		////Canvasのtransformと親子付け
		ui->m_transform.SetParent(&m_transform);

		m_uiList.push_back(ui);

		ui->Start();

		return ui;
	}

	/// <summary>
	/// UIの表示、非表示
	/// </summary>
	void SetVisible(bool visible);

	void SetUpdateEnabled(bool enable);
};



/// <summary>
/// 画像を使うUI
/// </summary>
class UIImage : public UIBase
{

protected:
	SpriteRender m_spriteRender;	//表示する画像
	Vector4 m_mulColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);	//乗算カラー
	

public:
	UIImage();
	~UIImage();

	virtual void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
	virtual void SetPivot(float x, float y);
	virtual void SetPivot(const Vector2& pivot);
	/// <summary>
	/// 乗算カラーを設定
	/// </summary>
	/// <param name="mulColor"></param>
	virtual void SetColor(Vector4 mulColor);
	virtual void SetColor(float r, float g, float b, float a);
};


/// <summary>
/// 伸び縮みするUI(例：HPバー)
/// 何らかの値と連動させて使う、
/// 最大値と現在値をもとに割合を計算して画像を伸び縮みさせる
/// </summary>
class UIGauge : public UIImage
{

private:
	float m_maxValue = 1.0f;
	float m_Value = 1.0f;
	

public:
	UIGauge();
	~UIGauge();

	void Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void SetPivot(float x, float y) override;
	void SetPivot(const Vector2& pivot) override;

	const float GetMaxValue() const;
	const float GetValue() const;

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

protected:
	FontRender m_fontRender;	//表示する文字

	Vector4 m_color;	//文字の色

public:
	UIText() {};
	~UIText() {};

	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
	virtual void SetText(const wchar_t* text);
	virtual void SetColor(const Vector4& color);
	virtual void SetColor(float r, float g, float b, float a);
	virtual void SetPivot(float x, float y);
	virtual void SetPivot(const Vector2& pivot);
};

