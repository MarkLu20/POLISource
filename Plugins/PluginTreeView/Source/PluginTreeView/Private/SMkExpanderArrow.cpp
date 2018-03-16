// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "SMkExpanderArrow.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/STableRow.h"
#include "Brushes/SlateImageBrush.h"
#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

void SMkExpanderArrow::Construct( const FArguments& InArgs, const TSharedPtr<class ITableRow>& TableRow, FSlateBrush& expander, FSlateBrush& expanderHover, FSlateBrush& collapsed, FSlateBrush& collapsedHover)
{
	OwnerRowPtr = TableRow;
	StyleSet = InArgs._StyleSet;
	IndentAmount = InArgs._IndentAmount;
	BaseIndentLevel = InArgs._BaseIndentLevel;

	this->ChildSlot
	.Padding( TAttribute<FMargin>( this, &SMkExpanderArrow::GetExpanderPadding ) )
	[
		SAssignNew(ExpanderArrow, SButton)
		.ButtonStyle( FCoreStyle::Get(), "NoBorder" )
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Visibility( this, &SMkExpanderArrow::GetExpanderVisibility )
		.ClickMethod( EButtonClickMethod::MouseDown )
		.OnClicked( this, &SMkExpanderArrow::OnArrowClicked )
		.ContentPadding(0.f)
		.ForegroundColor( FSlateColor::UseForeground() )
		.IsFocusable( false )
		[
			SNew(SImage)
			.Image( this, &SMkExpanderArrow::GetExpanderImage )
			.ColorAndOpacity( FSlateColor::UseForeground() )
		]
	];

	Expander = &expander;
	ExpanderHover = &expanderHover;
	Collapsed = &collapsed;
	CollapsedHover = &collapsedHover;
}

/** Invoked when the expanded button is clicked (toggle item expansion) */
FReply SMkExpanderArrow::OnArrowClicked()
{
	// Recurse the expansion if "shift" is being pressed
	const FModifierKeysState ModKeyState = FSlateApplication::Get().GetModifierKeys();
	if(ModKeyState.IsShiftDown())
	{
		OwnerRowPtr.Pin()->Private_OnExpanderArrowShiftClicked();
	}
	else
	{
		OwnerRowPtr.Pin()->ToggleExpansion();
	}

	return FReply::Handled();
}

/** @return Visible when has children; invisible otherwise */
EVisibility SMkExpanderArrow::GetExpanderVisibility() const
{
	return OwnerRowPtr.Pin()->DoesItemHaveChildren() ? EVisibility::Visible : EVisibility::Hidden;
}

/** @return the margin corresponding to how far this item is indented */
FMargin SMkExpanderArrow::GetExpanderPadding() const
{
	const int32 NestingDepth = FMath::Max(0, OwnerRowPtr.Pin()->GetIndentLevel() - BaseIndentLevel.Get());
	const float Indent = IndentAmount.Get(10.f);
	return FMargin( NestingDepth * Indent, 0,0,0 );
}

/** @return the name of an image that should be shown as the expander arrow */
const FSlateBrush* SMkExpanderArrow::GetExpanderImage() const
{
	const bool bIsItemExpanded = OwnerRowPtr.Pin()->IsItemExpanded();

	FName ResourceName;
	if (bIsItemExpanded)
	{
		if ( ExpanderArrow->IsHovered() )
		{
			return ExpanderHover;
			static FName ExpandedHoveredName = "TreeArrow_Expanded_Hovered";
			ResourceName = ExpandedHoveredName;
		}
		else
		{
			return Expander;
			static FName ExpandedName = "TreeArrow_Expanded";
			ResourceName = ExpandedName;
		}
	}
	else
	{
		if ( ExpanderArrow->IsHovered() )
		{
			return CollapsedHover;
			static FName CollapsedHoveredName = "TreeArrow_Collapsed_Hovered";
			ResourceName = CollapsedHoveredName;
		}
		else
		{
			return Collapsed;
			static FName CollapsedName = "TreeArrow_Collapsed";
			ResourceName = CollapsedName;
		}
	}
//	const FVector2D Icon24x24(24.0f, 24.0f);
//	FString RootToContentDir(const ANSICHAR* RelativePath, const TCHAR* Extension);
//	FSlateImageBrush brush = new FSlateImageBrush(FPaths::EngineContentDir() / TEXT("Slate/Checkerboard.png"), FVector2D(16, 16));// , FLinearColor::White, ESlateBrushTileType::Both);
//	FSlateBrush b = new IMAGE_BRUSH("Common/TreeArrow_Expanded_Hovered", Icon10x10, DefaultForeground);
		//new FSlateImageBrush((RootToContentDir(TEXT("ExportAnimSequence_24x.png")), Icon24x24));
		// new IMAGE_BRUSH("Common/TreeArrow_Expanded_Hovered", Icon10x10, DefaultForeground);
	return  StyleSet->GetBrush(ResourceName);
}

