// Fill out your copyright notice in the Description page of Project Settings.

#include "MkTreeView.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Views/SExpanderArrow.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
//#include "EditorStyle.h"
//#include "TextureResource.h"
//#include "Styling/SlateTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"


#define LOCTEXT_NAMESPACE "PluginTreeView"

/////////////////////////////////////////////////////
// UListView
UMkTreeView::UMkTreeView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, DataSource(MakeShareable(new FMkTreeViewItemInternal()))
{
	bIsVariable = true;

	ItemHeight = 16.0f;
	SelectionMode = ESelectionMode::Single;

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
		Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
	}
}

void UMkTreeView::RebuildClassification() {
	DataSource->Children.Empty();

	AllItems.Empty();
	AllItems.Reserve(Items.Num() + 1);
	AllItems.Add(DataSource);
	for (int i = 0; i < Items.Num(); ++i) {
		TSharedRef<FMkTreeViewItemInternal> obj = MakeShareable(new FMkTreeViewItemInternal());
		obj->Name = Items[i].Name;
		obj->CustomData = Items[i].CustomData;
		obj->Icon = Items[i].Icon;
		AllItems.Add(obj);
	}

	for (int i = 0; i < Items.Num(); ++i) {
		AllItems[Items[i].Parent]->Children.Add(AllItems[i + 1]);
		AllItems[i + 1]->ParentName = AllItems[Items[i].Parent]->Name;
	}

}

TSharedRef<SWidget> UMkTreeView::RebuildWidget()
{
	RebuildClassification();

	MyTreeView = SNew(STreeView< TSharedRef<FMkTreeViewItemInternal> >)
		.SelectionMode(SelectionMode)
		.TreeItemsSource(&(DataSource->Children))
		.OnGenerateRow(BIND_UOBJECT_DELEGATE(STreeView< TSharedRef<FMkTreeViewItemInternal> >::FOnGenerateRow, OnGenerateItemRow))
		.OnGetChildren(BIND_UOBJECT_DELEGATE(STreeView< TSharedRef<FMkTreeViewItemInternal> >::FOnGetChildren, OnGetChildren))
		.OnSelectionChanged(BIND_UOBJECT_DELEGATE(STreeView< TSharedRef<FMkTreeViewItemInternal> >::FOnSelectionChanged, OnSelectionChanged))
		;
	return BuildDesignTimeWidget(MyTreeView.ToSharedRef());
}

void UMkTreeView::OnSelectionChanged(TSharedPtr<FMkTreeViewItemInternal> InItem, ESelectInfo::Type Type)
{
	if (InItem.IsValid())
		OnSelectChanged.ExecuteIfBound(InItem->Name,InItem->ParentName, InItem->CustomData);
};

void UMkTreeView::OnGetChildren(TSharedRef<FMkTreeViewItemInternal> InInfo, TArray< TSharedRef<FMkTreeViewItemInternal> >& OutChildren) {
	OutChildren = InInfo->Children;
}

TSharedRef<ITableRow> UMkTreeView::OnGenerateItemRow(TSharedRef<FMkTreeViewItemInternal> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	FSlateColor ColorAndOpacity;
	if (InItem->ParentName == "")
		ColorAndOpacity = Lv1ColorAndOpacity;
	else
		ColorAndOpacity = Lv2ColorAndOpacity;

	FText text = FText::FromString(InItem->Name);
	FSlateBrush* icon = &(InItem->Icon);
	return
		SNew(SMkTreeRow< TSharedRef<FString> >, OwnerTable, Expander, ExpanderHover, Collapsed, CollapsedHover)
		.ExpanderStyleSet(&FCoreStyle::Get())
		.Style(&RowStyle)
		[
			/*SNew(STextBlock)
			.Text(text)
			.Font(Font)
			.ColorAndOpacity(ColorAndOpacity)*/
			SNew(SHorizontalBox)
			.Visibility(EVisibility::Visible)

		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(4.0f, 0.0f, 0.0f, 0.0f)
		[SNew(SImage)
		.Image(icon)
		]

	+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(4.0f, 0.0f, 0.0f, 0.0f)
		[SNew(STextBlock)
		.Text(text)
		.Font(Font)
		.ColorAndOpacity(ColorAndOpacity)
		.Margin(Padding)
		]

		];
}

void UMkTreeView::PostLoad()
{
	Super::PostLoad();

	//if (GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS)
	//{
	//	if (Style_DEPRECATED != nullptr)
	//	{
	//		const FTableRowStyle* StylePtr = Style_DEPRECATED->GetStyle<FTableRowStyle>();
	//		if (StylePtr != nullptr)
	//		{
	//			WidgetStyle = *StylePtr;
	//		}

	//		Style_DEPRECATED = nullptr;
	//	}
	//}

	///** Brush used as a selector when a row is focused */
	//WidgetStyle.SetSelectorFocusedBrush(RowStyle.SelectorFocusedBrush);

	///** Brush used when a selected row is active and hovered */
	//WidgetStyle.SetActiveHoveredBrush(RowStyle.ActiveHoveredBrush);

	///** Brush used when a selected row is active */
	//WidgetStyle.SetActiveBrush(RowStyle.ActiveBrush);
	//
	///** Brush used when an selected row is inactive and hovered */
	//WidgetStyle.SetInactiveHoveredBrush(RowStyle.InactiveHoveredBrush);

	//WidgetStyle.SetInactiveBrush(RowStyle.InactiveBrush);

	//WidgetStyle.SetEvenRowBackgroundHoveredBrush(RowStyle.EvenRowBackgroundHoveredBrush);
	//WidgetStyle.SetOddRowBackgroundHoveredBrush(RowStyle.OddRowBackgroundHoveredBrush);

	//WidgetStyle.SetEvenRowBackgroundBrush(RowStyle.EvenRowBackgroundBrush);
	//WidgetStyle.SetOddRowBackgroundBrush(RowStyle.OddRowBackgroundBrush);
}

void UMkTreeView::SynchronizeProperties() {
	Super::SynchronizeProperties();
}

#if WITH_EDITOR

const FText UMkTreeView::GetPaletteCategory()
{
	return LOCTEXT("Misc", "Misc");
}

#endif

void UMkTreeView::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	//MyTreeView.Reset();
}

void UMkTreeView::Refresh() {
	RebuildClassification();
	if (MyTreeView.IsValid()) {
		MyTreeView->RequestTreeRefresh();
	}
}

void UMkTreeView::SetSelection(int index) {
	
	if (index <= 0 || index >= AllItems.Num())
		MyTreeView->ClearSelection();
	else
	{
		if (MyTreeView->GetSelectedItems().Find(AllItems[index]) == INDEX_NONE)
			MyTreeView->SetSelection(AllItems[index]);
	}
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE




