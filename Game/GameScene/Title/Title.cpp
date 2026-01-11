#include "stdafx.h"
#include "Title.h"

Title::Title(){
	m_canvas = std::make_shared<UICanvas>();

	m_titleUI = m_canvas->CreateUI<UIImage>();
}

Title::~Title() {

}

bool Title::Start() {
	return true;
}

void Title::Update() {
	m_canvas->Update();
}

void Title::Render(RenderContext& rc) {
	m_canvas->Render(rc);
}

void Title::Init(const TitleInitData* initData) {
	if (initData == nullptr){
		//Ý’è‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î‰¼‰æ‘œ‚Å‰Šú‰»
		Init(&defaultTitle);
		return;
	}

	m_titleUI->Init(initData->titleFilePath, initData->frameSize_x, initData->frameSiza_y);
	m_titleUI->SetPivot(0.5f, 0.5f);

	m_canvas->m_transform.m_localPosition = Vector3(0.0f, 0.0f, 0.0f);
}

