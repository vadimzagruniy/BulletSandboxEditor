#include "ContentItemsView.h"

#include "IContentBrowserController.h"
#include <imgui.h>

namespace ui
{

namespace // unnamed
{

//
// Constants
//

constexpr float ICON_SIZE_SCALE  = 4.0f;
constexpr float TILE_WIDTH_SCALE = 1.5f;
constexpr float TILE_PADDING     = 6.0f;

const ImU32 FOLDER_ICON_COLOR = IM_COL32(214, 172, 84, 255);
const ImU32 FILE_ICON_COLOR   = IM_COL32(104, 136, 178, 255);

//
// Utils
//

std::string FitTextToWidth(const std::string & _Text, float _Width)
{
  if (ImGui::CalcTextSize(_Text.c_str()).x <= _Width)
    return _Text;

  const char * Ellipsis      = "...";
  const float  EllipsisWidth = ImGui::CalcTextSize(Ellipsis).x;

  std::string Fitted = _Text;

  while (!Fitted.empty() && ImGui::CalcTextSize(Fitted.c_str()).x + EllipsisWidth > _Width)
    Fitted.pop_back();

  return Fitted + Ellipsis;
}

} // namespace uunnamed

CContentItemsView::CContentItemsView(std::shared_ptr<IContentBrowserController> _Controller):
  m_Controller(std::move(_Controller))
{
  // Empty
}

//
// Interface
//

void CContentItemsView::Render()
{
  if (ImGui::BeginChild("##ContentItems"))
  {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    RenderItems(m_Controller->GetContentItems());

    ImGui::PopStyleVar();

    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
      m_Controller->ClearSelection();
  }

  ImGui::EndChild();
}

//
// Service
//

void CContentItemsView::RenderItems(const std::vector<TContentItem> & _ContentItems)
{
  const float  AvailableWidth = ImGui::GetContentRegionAvail().x;
  const float  IconSize       = ImGui::GetFrameHeight() * ICON_SIZE_SCALE;
  const ImVec2 TileSize       = ImVec2(IconSize * TILE_WIDTH_SCALE, IconSize + ImGui::GetFrameHeight() + TILE_PADDING * 3.0f);
  const float  LabelWidth     = TileSize.x - TILE_PADDING * 2.0f;
  const int    ColumnCount    = std::max(1, static_cast<int>(AvailableWidth/TileSize.x));
  const int    ItemCount      = static_cast<int>(_ContentItems.size());
  const int    RowCount       = (ItemCount + ColumnCount - 1) / ColumnCount;

  ImGuiListClipper Clipper;
  Clipper.Begin(RowCount, TileSize.y);

  while (Clipper.Step())
  {
    for (int RowIndex = Clipper.DisplayStart; RowIndex < Clipper.DisplayEnd; ++RowIndex)
    {
      const ImVec2 RowPosition = ImGui::GetCursorScreenPos();

      for (int ColumnIndex = 0; ColumnIndex < ColumnCount; ++ColumnIndex)
      {
        const int ItemIndex = RowIndex * ColumnCount + ColumnIndex;
        if (ItemIndex >= ItemCount)
          break;

        const auto & Item         = _ContentItems[static_cast<std::size_t>(ItemIndex)];
        const ImVec2 TilePosition(RowPosition.x + TileSize.x * static_cast<float>(ColumnIndex), RowPosition.y);
        const ImVec2 IconPosition(TilePosition.x + (TileSize.x - IconSize) * 0.5f, TilePosition.y + TILE_PADDING);
        const ImVec2 LabelPosition(TilePosition.x + TILE_PADDING, IconPosition.y + IconSize + TILE_PADDING);

        ImGui::PushID(ItemIndex);

        RenderItemButton(Item, TilePosition, TileSize);
        RenderItemIcon(Item, IconPosition, IconSize);
        RenderItemLabel(Item, LabelPosition, LabelWidth, true);
        
        ImGui::PopID();
      }

      ImGui::SetCursorScreenPos(RowPosition);
      ImGui::Dummy(ImVec2(AvailableWidth, TileSize.y));

    }
  }
}

void CContentItemsView::RenderItemButton(const TContentItem & _Item, const ImVec2 & _Position, const ImVec2 & _Size)
{
  ImGui::SetCursorScreenPos(_Position);
  ImGui::InvisibleButton("##Item", _Size);

  if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
  {
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
      m_Controller->Open(_Item.Path);
    /*else
      m_Presenter.Select(_Item.RelativePath, GetSelectionMode());*/
  }

  const bool IsSelected = m_Controller->IsSelected(_Item.Path);
  const bool IsHovered  = ImGui::IsItemHovered();

  if (IsSelected || IsHovered)
  {
    const ImGuiCol ColorIndex = IsSelected ? ImGuiCol_Header : ImGuiCol_HeaderHovered;
    const ImVec2   Max(_Position.x + _Size.x, _Position.y + _Size.y);

    ImGui::GetWindowDrawList()->AddRectFilled(_Position, Max, ImGui::GetColorU32(ColorIndex), ImGui::GetStyle().FrameRounding);
  }
}

void CContentItemsView::RenderItemIcon(const TContentItem & _Item, const ImVec2 & _Position, float _Size) const
{
  auto &       DrawList = *ImGui::GetWindowDrawList();
  const float  Rounding = _Size * 0.08f;
  const ImVec2 Max(_Position.x + _Size, _Position.y + _Size);

  if (!_Item.IsDirectory)
  {
    DrawList.AddRectFilled(ImVec2(_Position.x + _Size * 0.12f, _Position.y), ImVec2(Max.x - _Size * 0.12f, Max.y), FILE_ICON_COLOR, Rounding);
    return;
  }

  const float TabHeight = _Size * 0.15f;

  DrawList.AddRectFilled(_Position, ImVec2(_Position.x + _Size * 0.45f, _Position.y + TabHeight * 2.0f), FOLDER_ICON_COLOR, Rounding);
  DrawList.AddRectFilled(ImVec2(_Position.x, _Position.y + TabHeight), Max, FOLDER_ICON_COLOR, Rounding);
}

void CContentItemsView::RenderItemLabel(const TContentItem & _Item, const ImVec2 & _Position, float _Width, bool _IsCentered)
{
  const std::string Label      = FitTextToWidth(_Item.Name, _Width);
  const float       LabelWidth = ImGui::CalcTextSize(Label.c_str()).x;
  const float       OffsetX    = _IsCentered ? (_Width - LabelWidth) * 0.5f : 0.0f;

  ImGui::GetWindowDrawList()->AddText(ImVec2(_Position.x + OffsetX, _Position.y), ImGui::GetColorU32(ImGuiCol_Text), Label.c_str());

  ImGui::SetItemTooltip("%s", _Item.Name.c_str());
}

} // namespace ui