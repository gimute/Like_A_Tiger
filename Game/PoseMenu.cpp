#include "stdafx.h"
#include "PoseMenu.h"

PoseMenu::PoseMenu()
{
}


PoseMenu::~PoseMenu()
{
}


bool PoseMenu::Start() {
	return true;
}


void PoseMenu::Update() {
	m_canvas.Update();
}


void PoseMenu::Render(RenderContext& rc) {
	m_canvas.Render(rc);
}

void PoseMenu::Init(const PoseMenuInitData* initData)
{
	if (initData == nullptr) {
		//İ’è‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î‰¼‰æ‘œ‚Å‰Šú‰»
		Init(&defaultPoseMenu);
		return;
	}
}
