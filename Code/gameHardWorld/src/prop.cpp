#include "prop.h"

propList_t propAll[PROP_MAX_NUM_OF_ALLPROP] =
{
    {0, 0, HEALPROP},/* ��ҩ */
    {1, 1, HEALPROP},/* ҩ�� */

    {2, 0, CUREPROP},/* �ⶾ�� */

    {3, 0, MULITYFUNCPROP},/* Сʥˮ */
};

cureProp_t curePropList[PROP_MAX_NUM_OF_CUREPROP] =
{
    {0, L"�ⶾ��", L"�����ж�       ", CUREINTOXICATION},
};

healProp_t healPropList[PROP_MAX_NUM_OF_CUREPROP] =
{
    {0, L"��ҩ", L"���ú�ɻָ�100HP", 100, 0},
    {1, L"ҩ��", L"���ú�ɻָ�200HP", 200, 0},
};

mulityFuncProp_t mfPropList[PROP_MAX_NUM_OF_CUREPROP] =
{
    {0, L"Сʥˮ", L"�ָ�500HP��100MP��ͬʱ������и���Ч��", 500, 100, CUREALL},
};
