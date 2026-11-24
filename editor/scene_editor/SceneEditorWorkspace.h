#pragma once

#include "editor/workspace/EditorWorkspace.h"
#include "editor/tools/NodeSelection.h"
#include "editor/scene_editor/tools/TransformGizmo.h"

#include <string>
#include "tools/NodeCopyBuffer.h"

namespace ui
{

class CSceneNodeTreeWindow;
class CSceneObjectsWindow;
class CScenePropertyWindow;

} // namespace ui

namespace core
{

class CSceneNode;

} // namespace core

namespace editor
{

class CSceneDocument;
class CSceneSimulator;

class CSceneEditorWorkspace:
  public std::enable_shared_from_this<CSceneEditorWorkspace>,
  public CEditorWorkspace
{
public: // Constructor

  CSceneEditorWorkspace(std::shared_ptr<CSceneDocument> _AssetDocument);

public: // CEditorWorkspace

  void Update(float _DeltaTime) override;

public: // CEditorWorkspace events

  void OnActivate(const std::weak_ptr<ui::IWindowHost> & _WindowHost) override;

  void OnDeactivate() override;

  void OnRenderContent(const core::CCamera & _Camera) override;

public: // Interface

  bool IsNodeSelected(const std::shared_ptr<core::CSceneNode> & _SceneNode) const;

  void SelectNode(const std::shared_ptr<core::CSceneNode> & _SceneNode);

  std::shared_ptr<core::CSceneNode> GetRootNode() const;

  std::shared_ptr<core::CSceneNode> GetSelectedNode() const;

  bool CreateNode(const std::string & _TypeID, const std::shared_ptr<core::CSceneNode> & _Parent);

  bool CreateNode(const std::string & _TypeID, const std::shared_ptr<core::CSceneNode> & _Parent, const ImVec2 & _WorldPos);

  void DeleteNode(const std::shared_ptr<core::CSceneNode> & _Node);

  void RenameNode(const std::string & _Name, const std::string & _NewName);

  void DeleteSelectedNode();

  void CopyNode(const std::shared_ptr<core::CSceneNode> & _Node);

  void CopySelectedNode();

  void PasteToNode(const std::shared_ptr<core::CSceneNode> & _Node);

  void PasteToSelectedNode();

  bool IsSimulating() const;

  bool CanPaste() const;

private: // CEditorWorkspace Events

  void OnRenderToolBar() override;

  void OnCanvasMouseDown(const ImVec2 & _ScreenPos) override;

  void OnCanvasMouseUp(const ImVec2 & _ScreenPos) override;

  void OnCanvasMouseDrag(const ImVec2 & _ScreenPos) override;

  std::shared_ptr<CAssetDocument> GetAssetDocument() override;

  bool TryBeginDragGizmo(const ImVec2 & _ScreenPos);

private: // Service

  void RenderRootBounds(const core::CCamera & _Camera) const;

  void RenderSimulationBounds(const core::CCamera & _Camera) const;

public: // Events

  void OnNodePropertyChanged();

private: // Attr

  std::shared_ptr<CSceneDocument>           m_SceneDocument;
  std::shared_ptr<CSceneSimulator>          m_SceneSimulator;
  std::shared_ptr<ui::CSceneNodeTreeWindow> m_NodeTreeWindow;
  std::shared_ptr<ui::CSceneObjectsWindow>  m_SceneObjectsWindow;
  std::shared_ptr<ui::CScenePropertyWindow> m_ScenePropertyWindow;
  CNodeSelection                            m_Selection;
  CTransformGizmo                           m_TransformGizmo;
  CNodeCopyBuffer                           m_NodeCopyBuffer;
};

} // namespace editor