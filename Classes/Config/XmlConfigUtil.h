// XmlConfigUtil.h
//
#ifndef XML_CONFIG_UTIL_H_
#define XML_CONFIG_UTIL_H_

#include <assert.h>
#include <stdlib.h>

#include "../../thirdparty/mxml-2.9/mxml.h"


#include <map>
#include <vector>
#include <string>
#include <memory>
using namespace std;


#define CHECK_BOOL_RESULT(boolRes) if (!(boolRes)) return false


static bool MxmlNodeGetStringAttr(mxml_node_t *node, const char * nodeName, char * strValue, int sizeValue)
{
    if (!node) {
        *strValue = 0;
        return false;
    }

    const char * szAttrTemp = mxmlElementGetAttr(node, nodeName);
    if (!szAttrTemp) {
        *strValue = 0;
        return false;
    }

    strncpy(strValue, szAttrTemp, sizeValue);
    strValue[sizeValue - 1] = 0;
    return true;
}


static bool MxmlNodeGetIntegerAttr(mxml_node_t *node, const char * nodeName, int * intValue)
{
    if (!node) {
        *intValue = 0;
        return false;
    }

    const char * szAttrTemp = mxmlElementGetAttr(node, nodeName);
    if (!szAttrTemp || !strcmp(szAttrTemp, "") || !strcmp(szAttrTemp, "(null)")) {
        *intValue = 0;
        return false;
    }

    if (!strcmp(szAttrTemp, "true") || !strcmp(szAttrTemp, "yes") || !strcmp(szAttrTemp, "1")) {
        *intValue = 1;
    } else {
        *intValue = atoi(szAttrTemp);
    }

    return true;
}


#endif // XML_CONFIG_UTIL_H_
