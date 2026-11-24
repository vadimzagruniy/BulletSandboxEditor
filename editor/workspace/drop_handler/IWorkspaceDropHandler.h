#pragma once

#include "WorkspaceDropContext.h"

#include <memory>

namespace editor
{

class IWorkspaceDropHandler
{
public: // Destructor

  virtual ~IWorkspaceDropHandler() = default;

public: // Interface

  virtual bool CanHandle(const TWorkspaceDropContext & _Context) = 0;

  virtual std::string GetToolTip(const TWorkspaceDropContext & _Context) = 0;

  virtual void Drop(const TWorkspaceDropContext & _Context) = 0;
};

} // namespace editor
