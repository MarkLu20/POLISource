// Fill out your copyright notice in the Description page of Project Settings.

#include "ExhTreeView.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Input/SComboBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"

#define LOCTEXT_NAMESPACE "PluginTreeView"

/////////////////////////////////////////////////////
// UExhTreeView
UExhTreeView::UExhTreeView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

TSharedRef<SWidget> UExhTreeView::RebuildWidget()
{

	
	MyTreeView = SNew(STreeView< TSharedRef<FTreeViewItem> >)
		.SelectionMode(SelectionMode)
		.TreeItemsSource(&SItems)
		.OnGetChildren(BIND_UOBJECT_DELEGATE(STreeView< TSharedRef<FTreeViewItem> >::FOnGetChildren, OnGetChildren))
		.OnGenerateRow(BIND_UOBJECT_DELEGATE(STreeView< TSharedRef<FTreeViewItem> >::FOnGenerateRow, OnGenerateItemRow))
		.OnSelectionChanged(BIND_UOBJECT_DELEGATE(STreeView< TSharedRef<FTreeViewItem> >::FOnSelectionChanged, OnSelectionChanged))
		;
	return BuildDesignTimeWidget(MyTreeView.ToSharedRef());
}

void UExhTreeView::OnSelectionChanged(TSharedPtr<FTreeViewItem> InItem, ESelectInfo::Type Type)
{
	if (InItem.IsValid())
	{
		
	}
}

void UExhTreeView::OnGetChildren(TSharedRef<FTreeViewItem> InInfo, TArray< TSharedRef<FTreeViewItem> >& OutChildren) {
	for (int i = 0; i < Items.Num(); ++i) {
		if (InInfo->Index == Items[i].Parent) {
		//if (InInfo->Content == Items[i].Parent) {
			OutChildren.Add(MakeShareable(new FTreeViewItem(Items[i])));
		}
	}
}

TSharedRef<ITableRow> UExhTreeView::OnGenerateItemRow(TSharedRef<FTreeViewItem> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(SComboRow< TSharedRef<FTreeViewItem> >, OwnerTable)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.FillWidth(1000)
		.Padding(2.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(InItem->Content))
		.Font(Font)
		]
		];

}

#if WITH_EDITOR

const FText UExhTreeView::GetPaletteCategory()
{
	return LOCTEXT("Misc", "Misc");
}

#endif

void UExhTreeView::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyTreeView.Reset();
}


void UExhTreeView::Refresh()
{
	SItems.Empty();
	for (int i = 0; i < Items.Num(); ++i) {
		Items[i].Index = i + 1;
		if (Items[i].Parent == 0)
			SItems.Add(MakeShareable(new FTreeViewItem(Items[i])));
	}
	if(MyTreeView.IsValid())
		MyTreeView->RequestTreeRefresh();
}
/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE


