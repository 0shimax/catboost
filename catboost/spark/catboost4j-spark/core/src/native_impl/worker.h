#pragma once

#include <catboost/libs/data/data_provider.h>
#include <catboost/libs/data/quantized_features_info.h>

#include <util/generic/fwd.h>
#include <util/generic/yexception.h>
#include <util/system/types.h>


i64 GetPartitionTotalObjectCount(const TVector<NCB::TDataProviderPtr>& trainDataProviders) throw (yexception);

void CreateTrainingDataForWorker(
    i32 hostId,
    i32 numThreads,
    const TString& plainJsonParamsAsString,
    const TVector<NCB::TDataProviderPtr>& trainDataProviders,
    NCB::TQuantizedFeaturesInfoPtr quantizedFeaturesInfo,
    const TVector<NCB::TDataProviderPtr>& trainEstimatedDataProviders, // can be empty
    const TString& precomputedOnlineCtrMetaDataAsJsonString
) throw (yexception);

// needed for forwarding exceptions from C++ to JVM
void RunWorkerWrapper(i32 numThreads, i32 nodePort) throw (yexception);
