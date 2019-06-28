// GLViewConfig.h
//
#ifndef GLVIEW_CONFIG_H_
#define GLVIEW_CONFIG_H_

#include "ResolutionConfig.h"


class GLViewConfig
{
public:

    GLViewConfig() :
            frameWidth(1),
            frameHeight(1),
            FPS(60),
            displayStats(false) {
        *viewName = 0;
    }


    virtual ~GLViewConfig() {
        for (int i = 0; i < (int) resolutionConfigs.size(); ++i) {
            delete ((ResolutionConfig*) resolutionConfigs.at(i));
        }
        resolutionConfigs.clear();
    }


    bool init(mxml_node_t *node) {
        bool boolRes;

        boolRes = MxmlNodeGetStringAttr(node, "Name", viewName, sizeof(viewName));
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetIntegerAttr(node, "FrameWidth", &frameWidth);
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetIntegerAttr(node, "FrameHeight", &frameHeight);
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetIntegerAttr(node, "FPS", &FPS);
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetIntegerAttr(node, "FPS", &FPS);
        CHECK_BOOL_RESULT(boolRes);

        boolRes = MxmlNodeGetIntegerAttr(node, "DisplayStats", &displayStats);
        CHECK_BOOL_RESULT(boolRes);

        // Resolutions
        mxml_node_t *resNode;
        for (resNode = mxmlFindElement(node, node, "cc:DesignResolution", 0, 0, MXML_DESCEND);
            resNode != 0;
            resNode = mxmlFindElement(resNode, node, "cc:DesignResolution", 0, 0, MXML_DESCEND)) {

            ResolutionConfig * pResConfig = new ResolutionConfig;
            CHECK_BOOL_RESULT(pResConfig);

            boolRes = pResConfig->init(resNode);
            CHECK_BOOL_RESULT(boolRes);

            resolutionConfigs.push_back(pResConfig);
        }
        CHECK_BOOL_RESULT(resolutionConfigs.size());

        return boolRes;
    }


    const char * getViewName() const {
        return viewName;
    }


    bool frameSizeValid() const {
        return (frameWidth > 0 && frameHeight > 0);
    }


    float getFrameWidth() const {
        return (float) frameWidth;
    }


    float getFrameHeight() const {
        return (float) frameHeight;
    }


    float getFPS() const {
        return (float) FPS;
    }


    bool isDisplayStatsEnabled() const {
        return displayStats? true : false;
    }


private:
    char viewName[30];
    int frameWidth;
    int frameHeight;
    int FPS;
    int displayStats;

    vector<ResolutionConfig*> resolutionConfigs;
};

#endif // GLVIEW_CONFIG_H_
