// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TableViewBase.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Views/SExpanderArrow.h"
#include "SMkExpanderArrow.h"
//#include "TextWidgetTypes.h"
#include "MkTreeView.generated.h"

class USlateWidgetStyleAsset;

USTRUCT(BlueprintType)
struct FMkTreeViewItem {
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UObject* CustomData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSlateBrush Icon;
};

struct FMkTreeViewItemInternal {
	FString Name;
	FString ParentName;
	UObject* CustomData;
	FSlateBrush Icon;
	TArray<TSharedRef<FMkTreeViewItemInternal>> Children;
};
/**
* SMkTreeRow
*/
template<typename ItemType>
class SMkTreeRow :public STableRow<ItemType>
{
public:
	SMkTreeRow()
		:STableRow<ItemType>()
	{}
	typedef typename STableRow<ItemType>::FArguments FTableRowArgs;

	void Construct(const typename STableRow<ItemType>::FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, FSlateBrush& expander, FSlateBrush& expanderHover, FSlateBrush& collapsed, FSlateBrush& collapsedHover)
	{
		/** Note: Please initialize any state in ConstructInternal, not here. This is because STableRow derivatives call ConstructInternal directly to avoid constructing children. **/

		ConstructInternal(InArgs, InOwnerTableView);

		ConstructChildren(
			InOwnerTableView->TableViewMode,
			InArgs._Padding,
			InArgs._Content.Widget,
			expander,
			expanderHover,
			collapsed,
			collapsedHover
		);
	}

	void ConstructChildren(ETableViewMode::Type InOwnerTableMode, const TAttribute<FMargin>& InPadding, const TSharedRef<SWidget>& InContent, FSlateBrush& expander, FSlateBrush& expanderHover, FSlateBrush& collapsed, FSlateBrush& collapsedHover)
	{
		this->Content = InContent;
		InnerContentSlot = nullptr;

		if (InOwnerTableMode == ETableViewMode::List || InOwnerTableMode == ETableViewMode::Tile)
		{
			// -- Row is in a ListView or the user --
			FSimpleSlot* InnerContentSlotNativePtr = nullptr;

			// We just need to hold on to this row's content.
			this->ChildSlot
				.Expose(InnerContentSlotNativePtr)
				.Padding(InPadding)
				[
					InContent
				];

			InnerContentSlot = InnerContentSlotNativePtr;
		}
		else
		{
			// -- Row is for TreeView --
			SHorizontalBox::FSlot* InnerContentSlotNativePtr = nullptr;

			// Rows in a TreeView need an expander button and some indentation
			this->ChildSlot
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Fill)
				[
					SNew(SMkExpanderArrow, SharedThis(this), expander, expanderHover, collapsed, collapsedHover)					
				//	.StyleSet(ExpanderStyleSet)
				]

			+ SHorizontalBox::Slot()
				.FillWidth(1)
				.Expose(InnerContentSlotNativePtr)
				.Padding(InPadding)
				[
					InContent
				]
				];

			InnerContentSlot = InnerContentSlotNativePtr;
		}
	}

};


/**
 * 
 */
UCLASS()
class PLUGINTREEVIEW_API UMkTreeView : public UTableViewBase
{
	GENERATED_UCLASS_BODY()

public:
	/** Delegate for constructing a UWidget based on a UObject */
	DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnSelectChanged, FString, Name, FString, ParentName, UObject*, CustomData);

	/** The height of each widget */
	UPROPERTY(EditAnywhere, Category = Content)
		float ItemHeight;

	UPROPERTY(EditAnywhere, Category = Content)
		FMargin Padding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Content)
		TArray<FMkTreeViewItem> Items;

	/** The selection method for the list */
	UPROPERTY(EditAnywhere, Category = Content)
		TEnumAsByte<ESelectionMode::Type> SelectionMode;

	/** The font to render the text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Font)
		FSlateFontInfo Font;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Font)
		FSlateColor Lv1ColorAndOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Font)
		FSlateColor Lv2ColorAndOpacity;

	/** Called when a widget needs to be generated */
	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
		FOnSelectChanged OnSelectChanged;

	//FTableRowStyle WidgetStyle;
	/** The style */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "TableRowStyle"))
		FTableRowStyle RowStyle;

	/** ExpandableStyle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpandableStyle")
		FSlateBrush Expander;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpandableStyle")
		FSlateBrush ExpanderHover;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpandableStyle")
		FSlateBrush Collapsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpandableStyle")
		FSlateBrush CollapsedHover;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "SelectedBrush"))
//		FSlateBrush SelectedBrush;
	 
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style", meta = (DisplayName = "NormalBrush"))
//		FSlateBrush BackgroundBrush;

	/** Style used for the tablerow */
	//UPROPERTY()
	//	USlateWidgetStyleAsset* Style_DEPRECATED;

public:
	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void Refresh();

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void SetSelection(int index);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	virtual void SynchronizeProperties() override;

protected:

	TSharedPtr< STreeView< TSharedRef<FMkTreeViewItemInternal> > > MyTreeView;

	/** Delegate to generate a row in the workspace list */
	TSharedRef<class ITableRow> OnGenerateItemRow(TSharedRef<FMkTreeViewItemInternal> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	void OnGetChildren(TSharedRef<FMkTreeViewItemInternal> InInfo, TArray< TSharedRef<FMkTreeViewItemInternal> >& OutChildren);
	void OnSelectionChanged(TSharedPtr<FMkTreeViewItemInternal> InTreeItem, ESelectInfo::Type Type);

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	void RebuildClassification();
	// End of UWidget interface

	//~ Begin UObject Interface
	virtual void PostLoad() override;

	TSharedRef<FMkTreeViewItemInternal> DataSource;
	TArray<TSharedRef<FMkTreeViewItemInternal>> AllItems;
};
