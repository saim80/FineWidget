// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Components/ActorComponent.h"
#include "FineWidgetScreen.generated.h"


class UUserWidget;

/**
 * This class adds or remove widgets from the viewport. Activating this component will add the target widget to
 * the viewport. Deactivating this component will remove the target widget from the viewport.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINEWIDGET_API UFineWidgetScreen : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFineWidgetScreen();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UUserWidget *GetUserWidget() const { return UserWidget; }

	FORCEINLINE void SetWidgetClass(const TSoftClassPtr<UUserWidget>& InWidgetClass) { WidgetClass = InWidgetClass; }
	FORCEINLINE void SetLayerPriority(const int32& InLayerPriority) { LayerPriority = InLayerPriority; }
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	/// Animation binding to clean internal state after hiding the widget.
	UFUNCTION()
	void OnHideAnimationStarted();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "FineWidget")
	void ToggleWidget();
	
	virtual void AddWidget();
	virtual void RemoveWidget();
private:
	/// The widget class to add to the viewport.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FineWidget", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	TSoftClassPtr<UUserWidget> WidgetClass;
	
	/// Layer priority of the widget to add to the viewport.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FineWidget", meta = (AllowPrivateAccess = "true"))
	int32 LayerPriority;

	/// Player controller index. In case of split screen, this is the index of the player controller to add the widget to.
	int32 PlayerIndex;

	/// If true, the widget is added to the player screen. If false, the widget is added to the viewport.
	bool bAddToPlayerScreen;

	/// The widget instance added to the viewport. This is null if the widget is not added to the viewport.
	UPROPERTY(BlueprintReadOnly, Category = "FineWidget", Transient, meta = (AllowPrivateAccess = "true"))
	UUserWidget *UserWidget;

	/// Widget animation started event binding
	FWidgetAnimationDynamicEvent HideAnimationStartedEvent;
};
