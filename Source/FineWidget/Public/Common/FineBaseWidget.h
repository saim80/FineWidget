// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UObject/Object.h"
#include "FineBaseWidget.generated.h"

class UWidgetAnimation;
/**
 * Base class for all user widgets in FineWidget.
 *
 * This class provides some basic functionality for all user widgets in FineWidget.
 * It automatically binds widget animations to the widget's appearance and disappearance events.
 * Also, it registers viewport resize events and invokes update layout function which can be overriden by child classes.
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineBaseWidget : public UCommonUserWidget
{
	GENERATED_BODY()

	/**
	 * Lifecycle
	 */
public:
	/// Constructor.
	UFineBaseWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE bool CanRemoveFromParentOnHide() const { return bRemoveFromParentOnHide; }
	FORCEINLINE bool CanPlayShowAnimationOnConstruct() const { return bPlayShowAnimationOnConstruct; }
	FORCEINLINE UWidgetAnimation* GetHideAnimation() const { return HideAnimation; }
	FORCEINLINE UWidgetAnimation* GetShowAnimation() const { return ShowAnimation; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	/*
	 * Transition
	 */
private:
	/// The widget animation binding for widget's appearance event.
	UPROPERTY(BlueprintReadOnly, Category = "Transition", Transient,
		meta = (AllowPrivateAccess = "true", BindWidgetAnimOptional))
	UWidgetAnimation* ShowAnimation;

	/// The widget animation binding for widget's disappearance event.
	UPROPERTY(BlueprintReadOnly, Category = "Transition", Transient,
		meta = (AllowPrivateAccess = "true", BindWidgetAnimOptional))
	UWidgetAnimation* HideAnimation;

	/// When `UFineBaseWidget.Show()` is invoked, `ShowAnimation` is automatically played if this is true.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	bool bPlayShowAnimationOnConstruct;

	/// When `UFineBaseWidget.Hide()` is invoked, at the end of `HideAnimation`, the widget is removed from its parent if this is true.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	bool bRemoveFromParentOnHide;

	/// The visibility of the widget before `ShowAnimation` is played.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	ESlateVisibility PreShowAnimationVisibility;
	/// The visibility of the widget after `ShowAnimation` is played.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	ESlateVisibility PostShowAnimationVisibility;
	/// The visibility of the widget before `HideAnimation` is played.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	ESlateVisibility PreHideAnimationVisibility;
	/// The visibility of the widget after `HideAnimation` is played.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	ESlateVisibility PostHideAnimationVisibility;

public:
	/// Shows the widget. Plays `ShowAnimation` if it is valid and `bPlayShowAnimationOnConstruct` is true.
	UFUNCTION(BlueprintCallable, Category="Transition")
	void Show();

	/// Hides the widget. Plays `HideAnimation` if it is valid. At the end of `HideAnimation`, the widget is removed from its parent if `bRemoveFromParentOnHide` is true.
	UFUNCTION(BlueprintCallable, Category="Transition")
	void Hide();

	/*
	 * Layout
	 */
private:
	/// Determines if `UpdateLayout()` is invoked when the viewport is resized.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, BlueprintSetter=SetUpdateLayoutOnViewportResized, Category="Layout",
		meta=(AllowPrivateAccess="true"))
	bool bUpdateLayoutOnViewportResized;

	/// Handle to keep track of the registered viewport resize event callback.
	FDelegateHandle ViewportResizeHandle;

	/// Registers the callback to viewport resize events according to `bUpdateLayoutOnViewportResized`.
	void RegisterViewportResizeEvent();
	/// Unregisters the callback to viewport resize events according to `bUpdateLayoutOnViewportResized`.
	void UnregisterViewportResizeEvent();

protected:
	/// An event that is invoked when the viewport is resized.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Layout")
	void UpdateLayout();
	void UpdateLayout_Implementation();

public:
	/// Sets `bUpdateLayoutOnViewportResized`. This function registers/unregisters the callback to viewport resize events.
	UFUNCTION(BlueprintCallable, Category="Layout")
	void SetUpdateLayoutOnViewportResized(const bool& UpdateLayoutOnViewportResized);
};
