#pragma once

/*
* サウンド管理
* 必要なサウンドファイルの読み込み再生などを管理する
*/

//サウンド用ハンドル名
using SoundHandle = uint32_t;
//ハンドル無効値
static constexpr SoundHandle INVALID_SOUND_HANDLE = 0xffffffff;

/// <summary>
/// サウンド管理クラス
/// </summary>
class SoundManager
{
private:
	//BGM用のサウンドソースインスタンスを保持
	SoundSource* m_bgm = nullptr;
	//SE用サウンドソースインスタンスを保持
	std::map<SoundHandle, SoundSource*> m_seList;

	//マップで参照する用にハンドル数を保持
	//各サウンドソースを参照するための数値はユニークな数値になる
	SoundHandle m_soundHandleCount = 0;

private:
	SoundManager();
	~SoundManager();

public:
	//更新処理
	//不要になったインスタンスの削除など
	void Update();

public:
	//BGM再生
	void PlayBGM(const int kind);
	//BGM停止
	void StopBGM();

	void SetBGMVolume(float volume);

	//SE再生
	SoundHandle PlaySE(const int kind, const bool isLood = false, const bool is3D = false, float volume = 1.0f);
	//SE停止
	void StopSE(const SoundHandle handle);

	SoundSource* FindSE(const SoundHandle handle)
	{
		auto it = m_seList.find(handle);
		if (it != m_seList.end())
		{
			return it->second;
		}
		K2_ASSERT(false, "削除済みか追加されてないSEにアクセスしようとしています。\n");
		return nullptr;
	}


	//シングルトン用
private:
	//シングルトンインスタンス
	static SoundManager* m_instance;

public:
	//インスタンス生成
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new SoundManager();
		}
	}

	//インスタンスを取得
	static SoundManager& Get()
	{
		return *m_instance;
	}

	//インスタンスを破棄
	static void DestroyInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}
};

