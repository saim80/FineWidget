// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineStackNavigationWidget.generated.h"

class UFineStackTitleBarWidget;
class UFineStackContentWidget;
class UOverlay;
class UWidgetAnimation;
/**
 * This class provides a stack navigation functionality. Only widget bindings are provided. Actually, presentation
 * logic should be implemented in child classes as widget blueprint. See `W_StackNavigation` blueprint for example.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineStackNavigationWidget : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineStackNavigationWidget(const FObjectInitializer& ObjectInitializer);

	/// Pushes the given content widget to the stack. If no content widget is pushed, the root content widget is shown.
	UFUNCTION(BlueprintCallable, Category = "StackNavigation")
	void Push(UFineStackContentWidget* ContentWidget);
	/// Pops the top content widget from the stack. If no content widget is popped, the root content widget is shown.
	UFUNCTION(BlueprintCallable, Category = "StackNavigation")
	void Pop();
	/// Pops all content widgets from the stack until the root content is shown.
	UFUNCTION(BlueprintCallable, Category = "StackNavigation")
	void PopToRoot();

protected:
	virtual void OnAnimationStarted_Implementation(const UWidgetAnimation* Animation) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	/// The content widgets are managed by this widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (AllowPrivateAccess = true))
	TArray<UFineStackContentWidget*> ContentWidgets;

	/// The front content widget is the top of the stack.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* FrontOverlay;
	/// The back content widget is the bottom of the stack.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* BackOverlay;
	/// The title content widget is used to show the title of the front content widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* TitleOverlay;

	/// The title bar widget is used to show the title of the front content widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", Transient,
		meta = (BindWidgetAnim, AllowPrivateAccess = true))
	UWidgetAnimation* PushAnimation;
	/// The title bar widget is used to show the title of the front content widget.
	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", Transient,
		meta = (BindWidgetAnim, AllowPrivateAccess = true))
	UWidgetAnimation* PopAnimation;

	/// The content widgets scheduled for pushing. The array becomes empty after the push animation is finished.
	UPROPERTY()
	TArray<UFineStackContentWidget*> PushQueue;
	/// The content widgets scheduled for popping. The array becomes empty after the pop animation is finished.
	UPROPERTY()
	TArray<UFineStackContentWidget*> PopQueue;

	/// Update the title bar for a given content widget
	UFUNCTION()
	void UpdateTitleBar(UFineStackContentWidget* ContentWidget);

	/// The unused title bar widgets are managed by this widget.
	UPROPERTY()
	TArray<UFineStackTitleBarWidget*> FreeTitleBarWidgets;

	/// Title bar widget class
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="StackNavigation", meta = (AllowPrivateAccess = true))
	TSubclassOf<UFineStackTitleBarWidget> TitleBarClass;
};
