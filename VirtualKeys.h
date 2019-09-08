
#pragma once
#include "framework.h"

// constant data, should not ever change
struct VKeyStruct
{
	CString Name;
    UINT VKeyID;
    CString Description;
};

struct VKeys
{
    static VKeyStruct VKeyList[254];
    VKeyStruct get_vkeystruct_from_vkeyid(int VKeyID);
    VKeyStruct get_vkeystruct(int index);
};
