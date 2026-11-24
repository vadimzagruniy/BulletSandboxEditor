#include "Application.h"

#include "editor/Editor.h"
#include "game/Game.h"
#include "imgui.h"

//
// Constructor/Destructor
//

CApplication::CApplication():
  m_Game(std::make_shared<game::CGame>()),
  m_Editor(std::make_unique<editor::CEditor>())
{
  // Empty
}

CApplication::~CApplication() = default;

//
// Interface
//

void CApplication::Init()
{
  m_Game->Init();
  m_Editor->Init();
}

void CApplication::Update(float _DeltaTime)
{
  m_Editor->Update(_DeltaTime); 
}

void CApplication::Render()
{
  m_Editor->Render();
}

void CApplication::Shutdown()
{
  m_Editor->Shutdown();
  m_Game->Shutdown();
}

