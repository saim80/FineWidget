// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FineAttributeType.generated.h"

UENUM()
enum class EFineAttributeType: uint8
{
	Health,
	Mana,
	Stamina,
};