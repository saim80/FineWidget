// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineStackTitleBar.generated.h"

class USizeBox;
class UWidget;
class UTextBlock;
class UOverlay;
class UFineContent;
/**
 * This widget is a title bar for stack navigation widget. The container for left and right widgets are provided.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineStackTitleBar : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineStackTitleBar(const FObjectInitializer& ObjectInitializer);

	/// Returns left widget.
	FORCEINLINE UWidget* GetLeftWidget() const { return LeftWidget; }
	/// Returns right widget.
	FORCEINLINE UWidget* GetRightWidget() const { return RightWidget; }
	/// Returns title widget.
	FORCEINLINE UWidget* GetTitleWidget() const { return TitleWidget; }

	/// Updates the internal widgets using the given content widget as data source.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	void UpdateInternal(UFineContent* ContentWidget);
	virtual void UpdateInternal_Implementation(UFineContent* ContentWidget);
	
private:
	/// Left widget given by the corresponding stack content widget.
	UPROPERTY(BlueprintReadWrite, Category="StackNavigation", meta = (AllowPrivateAccess = true))
	UWidget* LeftWidget;
	/// Right widget given by the corresponding stack content widget.
	UPROPERTY(BlueprintReadWrite, Category="StackNavigation", meta = (AllowPrivateAccess = true))
	UWidget* RightWidget;
	/// Title widget given by the corresponding stack content widget.
	UPROPERTY(BlueprintReadWrite, Category="StackNavigation", meta = (AllowPrivateAccess = true))
	UWidget* TitleWidget;

	/// The default title widget to show title text when no title widget is given by the content widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* TitleTextBlock;
	/// The containing widget for left widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* LeftOverlay;
	/// The containing widget for right widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* RightOverlay;
	/// The containing widget for title widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* TitleOverlay;

	/// The size box for the title bar to override height of the title bar.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	USizeBox* BarSizeBox;
};
