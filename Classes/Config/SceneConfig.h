// SceneConfig.h
//
#ifndef SCENE_CONFIG_H_
#define SCENE_CONFIG_H_

#include "XmlConfigUtil.h"


class SceneConfig
{
public:

    SceneConfig() {
        *id = 0;
        *descr = 0;
        *config = 0;
    }


    SceneConfig(const SceneConfig &copy) {
        memcpy(id, copy.getId(), sizeof(id));
        memcpy(config, copy.getConfig(), sizeof(config));
        memcpy(descr, copy.getDescr(), sizeof(descr));
    }


    virtual ~SceneConfig() {

    }


    bool init(mxml_node_t *node) {
        bool boolRes = false;

        boolRes = MxmlNodeGetStringAttr(node, "Id", id, sizeof(id));
        CHECK_BOOL_RESULT(boolRes);

        MxmlNodeGetStringAttr(node, "Descr", descr, sizeof(descr));

        boolRes = MxmlNodeGetStringAttr(node, "Config", config, sizeof(config));
        CHECK_BOOL_RESULT(boolRes);

        boolRes = loadConfig();

        return boolRes;
    }


    bool loadConfig() {
        // TODO: Config
        return true;
    }


    const char * getId() const {
        return id;
    }


    const char * getDescr() const {
        return descr;
    }


    const char * getConfig() const {
        return config;
    }

private:

    char id[30];
    char descr[60];
    char config[200];
};

#endif // SCENE_CONFIG_H_
