// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Scene/FineScene.h"
#include "UObject/Object.h"
#include "FineLoadingScene.generated.h"

class UFineWidgetScreen;
/**
 * 
 */
UCLASS()
class FINEWIDGET_API AFineLoadingScene : public AFineScene
{
	GENERATED_BODY()
public:
	AFineLoadingScene();

protected:
	virtual void OnLoadingStarted_Implementation() override;
	virtual void OnLoadingFinished_Implementation() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Scene")
	UFineWidgetScreen *LoadingScreen;
};
