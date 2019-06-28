// ResolutionConfig.h
//
#ifndef RESOLUTION_CONFIG_H_
#define RESOLUTION_CONFIG_H_

#include "XmlConfigUtil.h"


class ResolutionConfig
{
public:

    ResolutionConfig() { 
        *id = 0;
        *descr = 0;
    }

    virtual ~ResolutionConfig() {
    }


    bool init(mxml_node_t *node) {
        bool boolRes;

        boolRes = MxmlNodeGetStringAttr(node, "Id", id, sizeof(id));
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetIntegerAttr(node, "Width", &width);
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetIntegerAttr(node, "Height", &height);
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetStringAttr(node, "Policy", descr, sizeof(descr));
        CHECK_BOOL_RESULT(boolRes);

        boolRes = setPolicy(descr);
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetStringAttr(node, "Descr", descr, sizeof(descr));
        CHECK_BOOL_RESULT(boolRes);

        mxml_node_t * pathsNode = mxmlFindElement(node, node, "cc:SearchPaths", 0, 0, MXML_DESCEND);
        CHECK_BOOL_RESULT(pathsNode);

        const char * paths = mxmlGetText(pathsNode, 0);
        CHECK_BOOL_RESULT(paths);

        do {
            const char * p = paths;
            const char * q = p;
            char path[260];
            int len = 0;

            for (;;) {
                if (*q == ',' || !(*q)) {
                    len = q - p;
                    if (len >= sizeof(path)) {
                        len = sizeof(path) - 1;
                    }

                    strncpy(path, p, len);
                    path[len] = 0;
                    searchPaths.push_back(path);

                    if (!(*q)) {
                        break;
                    }

                    p = ++q;
                }
                else {
                    q++;
                }
            }
        } while (0);
        CHECK_BOOL_RESULT(searchPaths.size());

        return boolRes;
    }


    vector<string> & getSearchPaths() {
        return searchPaths;
    }


    float getResolutionSize() const {
        return (width > height ? width : height);        
    }


    bool match(float screenWidth, float screenHeight) const {
        float resSize = getResolutionSize();

        return (resSize >= screenWidth && resSize >= screenHeight)? true : false;
    }


    int getPolicy() const {
        return policy;
    }

#if _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif

    bool setPolicy(const char *pol) {
        if (!stricmp(pol, "NoBorder") || !stricmp(pol, "Default")) {
            policy = (int) ResolutionPolicy::NO_BORDER;
        }
        else if (!stricmp(pol, "ExactFit")) {
            policy = (int) ResolutionPolicy::EXACT_FIT;
        }
        else if (!stricmp(pol, "ShowAll")) {
            policy = (int) ResolutionPolicy::SHOW_ALL;
        }
        else if (!stricmp(pol, "FixedHeight")) {
            policy = (int) ResolutionPolicy::FIXED_HEIGHT;
        }
        else if (!stricmp(pol, "FixedWidth")) {
            policy = (int) ResolutionPolicy::FIXED_WIDTH;
        }
        else if (!stricmp(pol, "") || !stricmp(pol, "Unknown")) {
            policy = (int) ResolutionPolicy::UNKNOWN;
        }
        else {
            policy = -1;
            return false;
        }

        return true;
    }

#if _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (pop)
#endif

private:

    int  policy;
    int  width;
    int  height;

    char id[30];
    char descr[60];

    vector<string> searchPaths;
    
};

#endif // RESOLUTION_CONFIG_H_
