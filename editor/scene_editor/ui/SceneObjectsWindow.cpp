#include "SceneObjectsWindow.h"

#include "core/scene/ObjectFactory.h"
#include "editor/workspace/drop_handler/WorkspaceDropContext.h"
#include "editor/ui/WindowIDs.h"
#include "imgui.h"

namespace ui
{

//
// Constructor/Destructor
//

CSceneObjectsWindow::CSceneObjectsWindow():
  CDockedWindow(SCENE_OBJECTS, true)
{
  // Empty
}

//
// CBaseWindow events
//

void CSceneObjectsWindow::OnRenderGui()
{
  const auto TypeNames = core::GetObjectFactory().GetTypeNames();
  for (const auto & TypeName : TypeNames)
  {
    ImGui::Selectable(TypeName.c_str());

    if (ImGui::BeginDragDropSource())
    {
      ImGui::SetDragDropPayload(editor::SCENE_OBJECT_PAYLOAD, TypeName.c_str(), TypeName.size() + 1);
      ImGui::TextUnformatted(TypeName.c_str());
      ImGui::EndDragDropSource();
    }
  }
}

} // namespace ui