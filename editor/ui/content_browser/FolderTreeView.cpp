#include "FolderTreeView.h"

#include "IContentBrowserController.h"
#include "imgui.h"

namespace ui
{

namespace // unnamed
{

bool HasSubfolders(const editor::TAssetNode & _AssetNode)
{
  return std::any_of(_AssetNode.Children.begin(), _AssetNode.Children.end(), [](const editor::TAssetNode & _Child)
  {
    return _Child.IsDirectory;
  });
}

} // namespace unnamed

//
// Constructor
//

CFolderTreeView::CFolderTreeView(std::shared_ptr<IContentBrowserController> _Controller):
  m_Controller(std::move(_Controller))
{
  // Empty
}

//
// Interface
//

void CFolderTreeView::Render()
{
  DrawFolder(m_Controller->GetRootNode());
}

//
// Service
//

void CFolderTreeView::DrawFolder(const editor::TAssetNode & _AssetNode)
{
  if (_AssetNode.Name.empty())
    return;

  const auto & AssetPath     = _AssetNode.Path;
  const auto & CurrentFolder = m_Controller->GetCurrentFolder();
  const bool   IsRoot        = AssetPath == m_Controller->GetRootNode().Path;

  ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

  if (IsRoot)
    Flags |= ImGuiTreeNodeFlags_DefaultOpen;

  if (AssetPath == CurrentFolder)
    Flags |= ImGuiTreeNodeFlags_Selected;

  if (!HasSubfolders(_AssetNode))
    Flags |= ImGuiTreeNodeFlags_Leaf;

  const bool IsOpen = ImGui::TreeNodeEx(_AssetNode.Name.c_str(), Flags);

  if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
    m_Controller->NavigateTo(AssetPath);

  if (!IsOpen)
    return;

  for (const auto & Child : _AssetNode.Children)
  {
    if (Child.IsDirectory)
      DrawFolder(Child);
  }

  ImGui::TreePop();
}

} // namespace ui