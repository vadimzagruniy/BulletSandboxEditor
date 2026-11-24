#pragma once

#include <memory>

namespace game
{

class CGame
{
public: // Constructor/Destructor

  CGame();

  ~CGame();

public: // Interface

  bool Init();

  void Shutdown();

private: // Service

  void RegisterGameObjects();

private: // Attr

};

} // namespace game
