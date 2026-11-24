
#include "SceneObjectDropHandler.h"
#include "editor/scene_editor/SceneEditorWorkspace.h"

namespace editor
{

//
// Constructor
//

CSceneObjectDropHandler::CSceneObjectDropHandler(CSceneEditorWorkspace & _SceneWorkspace):
  m_SceneWorkspace(_SceneWorkspace)
{
  // Empty
}

//
// IWorkspaceDropHandler
//

bool CSceneObjectDropHandler::CanHandle(const TWorkspaceDropContext & _Context)
{
  return _Context.PayloadType == SCENE_OBJECT_PAYLOAD;
}

std::string CSceneObjectDropHandler::GetToolTip(const TWorkspaceDropContext & _Context)
{
  return "Create: " + _Context.Value;
}

void CSceneObjectDropHandler::Drop(const TWorkspaceDropContext & _Context)
{
  m_SceneWorkspace.CreateNode(_Context.Value, m_SceneWorkspace.GetRootNode(), _Context.WorldPos);
}

} // namespace editor