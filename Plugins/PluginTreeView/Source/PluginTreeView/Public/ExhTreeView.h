// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TableViewBase.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STreeView.h"
#include "ExhTreeView.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTreeViewItem {
	GENERATED_BODY();

	int Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Content;
};


//struct FTreeViewItemInternel

UCLASS()
class PLUGINTREEVIEW_API UExhTreeView : public UTableViewBase
{
	GENERATED_UCLASS_BODY()

public:

	/** The height of each widget */
	UPROPERTY(EditAnywhere, Category = Content)
		float ItemHeight;

	/** The list of items to generate widgets for */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Content)
		TArray<FTreeViewItem> Items;

	/** The selection method for the list */
	UPROPERTY(EditAnywhere, Category = Content)
		TEnumAsByte<ESelectionMode::Type> SelectionMode;

	/** The font to render the text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Content)
		FSlateFontInfo Font;
	/** Called when a widget needs to be generated */
	//	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
	//		FOnGenerateRowFString OnGenerateRowEvent;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:

	TSharedPtr< STreeView< TSharedRef<FTreeViewItem> > > MyTreeView;

	/** Delegate to generate a row in the workspace list */
	TSharedRef<class ITableRow> OnGenerateItemRow(TSharedRef<FTreeViewItem> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	void OnGetChildren(TSharedRef<FTreeViewItem> InInfo, TArray< TSharedRef<FTreeViewItem> >& OutChildren);

	//DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSelectionChanged, FString, Name);

	void OnSelectionChanged(TSharedPtr<FTreeViewItem> InItem, ESelectInfo::Type Type);

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	TArray<TSharedRef<FTreeViewItem>> SItems;

public:
	UFUNCTION(BlueprintCallable, Category = "TreeView")
		void Refresh();
};

