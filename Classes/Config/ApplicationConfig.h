// ApplicationConfig.h
//
//
//
// Init Created: 2016-06-23
// Last Updated: 2016-06-23
//
#ifndef APP_LAYOUT_CONFIG_H
#define APP_LAYOUT_CONFIG_H

#include "GLViewConfig.h"
#include "SceneMapConfig.h"


class ApplicationConfig
{
public:
    ApplicationConfig() {
        *version = 0;
    }

    virtual ~ApplicationConfig() {
    }


    bool init(const char * appLayoutXml) {
        FILE *fp;
        mxml_node_t *tree;
        mxml_node_t *root;
        mxml_node_t *node;
        bool ret = false;

        fp = fopen(appLayoutXml, "r");
        if (!fp) {
            return false;
        }

        tree = mxmlLoadFile(0, fp, MXML_TEXT_CALLBACK);
        if (!tree) {
            fclose(fp);
            return false;
        }

        root = mxmlFindElement(tree, tree, "cc:Application", 0, 0, MXML_DESCEND);
        if (!root) {
            goto SAFE_RETUTN;
        }

        ret = MxmlNodeGetStringAttr(root, "Version", version, sizeof(version));
        if (!ret) {
            goto SAFE_RETUTN;
        }

        node = mxmlFindElement(root, root, "cc:GLView", 0, 0, MXML_DESCEND);
        if (!node || !glViewConfig.init(node)) {
            goto SAFE_RETUTN;
        }

        node = mxmlFindElement(root, root, "cc:SceneMap", 0, 0, MXML_DESCEND);
        if (!node || !sceneMapConfig.init(node)) {
            goto SAFE_RETUTN;
        }

        ret = true;

    SAFE_RETUTN:
        mxmlDelete(tree);
        fclose(fp);
        return ret;
    }


    const char * getVersion() {
        return version;
    }


    GLViewConfig & getGLViewConfig() {
        return glViewConfig;
    }


    SceneMapConfig & getSceneMapConfig() {
        return sceneMapConfig;
    }


private:
    char version[10];

    GLViewConfig  glViewConfig;

    SceneMapConfig sceneMapConfig;
};

#endif // APP_LAYOUT_CONFIG_H
