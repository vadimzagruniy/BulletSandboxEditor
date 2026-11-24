#pragma once

namespace editor
{

class CEditor;

} // namespace editor

namespace game
{

class CGame;

} // namespace game


#include <memory>

class CApplication
{
public: // Constructor/Destructor

  CApplication();

  ~CApplication();

public: // Interface

  void Init();

  void Update(float _DeltaTime);

  void Render();

  void Shutdown();

private: // Attributes

  std::shared_ptr<game::CGame>     m_Game;
  std::unique_ptr<editor::CEditor> m_Editor;
};



