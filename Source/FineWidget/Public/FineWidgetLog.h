// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FineBaseLoggerMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFineWidget, Log, All);

#define FW_FATAL(FORMAT, ...) FINEBASE_FATAL(LogFineWidget, FORMAT, ##__VA_ARGS__)
#define FW_ERROR(FORMAT, ...) FINEBASE_ERROR(LogFineWidget, FORMAT, ##__VA_ARGS__)
#define FW_WARNING(FORMAT, ...) FINEBASE_WARNING(LogFineWidget, FORMAT, ##__VA_ARGS__)
#define FW_DISPLAY(FORMAT, ...) FINEBASE_DISPLAY(LogFineWidget, FORMAT, ##__VA_ARGS__)
#define FW_LOG(VERBOSITY, FORMAT, ...) FINEBASE_LOG(LogFineWidget, VERBOSITY, FORMAT, ##__VA_ARGS__)
#define FW_VERBOSE(FORMAT, ...) FINEBASE_VERBOSE(LogFineWidget, FORMAT, ##__VA_ARGS__)
#define FW_VERYVERBOSE(FORMAT, ...) FINEBASE_VERYVERBOSE(LogFineWidget, FORMAT, ##__VA_ARGS__)
