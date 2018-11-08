LIBRARY()



SRCS(
    mode_calc_helpers.cpp
    mode_fstr_helpers.cpp
    proceed_pool_in_blocks.cpp
)

PEERDIR(
    catboost/libs/algo
    catboost/libs/data
    catboost/libs/eval_result
    catboost/libs/fstr
    catboost/libs/logging
    catboost/libs/model
    catboost/libs/options
    library/threading/local_executor
)

GENERATE_ENUM_SERIALIZATION(implementation_type_enum.h)

END()
