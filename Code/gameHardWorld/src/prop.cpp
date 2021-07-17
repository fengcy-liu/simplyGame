#include "prop.h"

propList_t propAll[PROP_MAX_NUM_OF_ALLPROP] =
{
    {0, 0, HEALPROP},/* 草药 */
    {1, 1, HEALPROP},/* 药剂 */

    {2, 0, CUREPROP},/* 解毒剂 */

    {3, 0, MULITYFUNCPROP},/* 小圣水 */
};

cureProp_t curePropList[PROP_MAX_NUM_OF_CUREPROP] =
{
    {0, L"解毒剂", L"治疗中毒       ", CUREINTOXICATION},
};

healProp_t healPropList[PROP_MAX_NUM_OF_CUREPROP] =
{
    {0, L"草药", L"服用后可恢复100HP", 100, 0},
    {1, L"药剂", L"服用后可恢复200HP", 200, 0},
};

mulityFuncProp_t mfPropList[PROP_MAX_NUM_OF_CUREPROP] =
{
    {0, L"小圣水", L"恢复500HP，100MP，同时解除所有负面效果", 500, 100, CUREALL},
};
