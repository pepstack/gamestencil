// SceneMapConfig.h
//
#ifndef SCENE_MAP_CONFIG_H_
#define SCENE_MAP_CONFIG_H_

#include "SceneConfig.h"


class SceneMapConfig
{
typedef pair<string, SceneConfig> SceneMapPair;
typedef map<string, SceneConfig>::value_type SceneMapValue;
typedef map<string, SceneConfig>::iterator SceneMapIter;
typedef map<string, SceneConfig>::const_iterator SceneMapCIter;

public:

    SceneMapConfig() {
    }

    virtual ~SceneMapConfig() {
    }


    bool init(mxml_node_t *parent) {
        bool boolRes = false;

        // Resolutions
        mxml_node_t *node;
        for (node = mxmlFindElement(parent, parent, "cc:Scene", 0, 0, MXML_DESCEND);
            node != 0;
            node = mxmlFindElement(node, parent, "cc:Scene", 0, 0, MXML_DESCEND)) {

            SceneConfig sceneConfig;

            boolRes = sceneConfig.init(node);
            CHECK_BOOL_RESULT(boolRes);

            sceneMap.insert(SceneMapPair(sceneConfig.getId(), sceneConfig));
        }

        CHECK_BOOL_RESULT(sceneMap.size());
        return boolRes;
    }


    map<string, SceneConfig> & getSceneMapConfig() {
        return sceneMap;
    }


private:

    map<string, SceneConfig> sceneMap;
    
};

#endif // SCENE_MAP_CONFIG_H_
