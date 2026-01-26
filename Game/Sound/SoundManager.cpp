#include "stdafx.h"
#include "SoundManager.h"


SoundManager* SoundManager::m_instance = nullptr; //初期化

SoundManager::SoundManager()
{
	m_seList.clear();
}

SoundManager::~SoundManager()
{

}

void SoundManager::Update()
{
	//SEリストから再生していないものがあれば削除する
	std::vector<SoundHandle> eraseList;
	for (auto& it : m_seList)
	{
		const auto key = it.first;
		auto* se = it.second;

		//再生が終わっているなら削除
		if (!se->IsPlaying())
		{
			eraseList.push_back(key);
		}
	}

	for (const auto& key : eraseList)
	{
		m_seList.erase(key);
	}
}

void SoundManager::PlayBGM(const int kind)
{
	//BGMが生成されていない
	if (m_bgm == nullptr)
	{
		//生成
		m_bgm = NewGO<SoundSource>(0, "bgm");
	}
	else
	{
		//すでに生成されているなら一度BGMを停止する
		m_bgm->Stop();
	}

	//初期化
	m_bgm->Init(kind);
	m_bgm->Play(true);	//BGMなのでループ再生する
}

void SoundManager::StopBGM()
{
	if (m_bgm == nullptr)
	{
		return;
	}
	m_bgm->Stop();
}

void SoundManager::SetBGMVolume(float volume)
{
	if (m_bgm != nullptr)
	{
		m_bgm->SetVolume(volume);
	}
}

SoundHandle SoundManager::PlaySE(const int kind, const bool isLoop, const bool is3D, float volume)
{
	// ハンドルが最大数になったら使えない
	if (m_soundHandleCount == INVALID_SOUND_HANDLE)
	{
		K2_ASSERT(false, "サウンドの再生が多いです。\n");
		return INVALID_SOUND_HANDLE;
	}
	auto* se = NewGO<SoundSource>(0, "se");
	se->Init(kind, is3D);
	se->SetVolume(volume);
	se->Play(isLoop);

	m_seList.emplace(m_soundHandleCount, se);

	return m_soundHandleCount++;
}

void SoundManager::StopSE(const SoundHandle handle)
{
	auto* se = FindSE(handle);
	if (se == nullptr)
	{
		return;
	}
	se->Stop();
}