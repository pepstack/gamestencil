/**
* GameStencil.cpp
*   The GameStencil class is the central point for creating
*     and managing your game state.
*
* Refer:
*   http://www.richardlord.net/blog/what-is-an-entity-framework
*   http://blog.csdn.net/i_dovelemon/article/details/30250049
*   http://blog.csdn.net/zhao_92221/article/details/46629553
*   http://blog.csdn.net/ubuntu64fan/article/details/8839778
*
* Init Created: 2016-06-20
* Last Updated: 2016-06-20
*/
#include "GameStencil.h"

#include <time.h>

using namespace ecs;


/**
 * mxml
 */
#include "../../thirdparty/mxml-2.9/config.h"
#include "../../thirdparty/mxml-2.9/mxml.h"

#ifndef WIN32
#  include <unistd.h>
#endif /* !WIN32 */
#include <fcntl.h>
#ifndef O_BINARY
#  define O_BINARY 0
#endif /* !O_BINARY */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif


static const char * current_timestr(char buf[])
{
    time_t tt = time(0);
    tm* t = localtime(&tt);

    sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec);

    return buf;
}


lresult_t GameStencil::saveStencilXML(const char * stencilXmlFile)
{
    mxml_node_t *xml;
    mxml_node_t *root;

    char buffer[200];

    xml = mxmlNewXML("1.0");

    root = mxmlNewElement(xml, ecsGameStencilNodeName);
   
    mxmlElementSetAttr(root, "copyright", "pepstack.com");
    mxmlElementSetAttr(root, "version", "0.0.1");
    mxmlElementSetAttr(root, "credate", current_timestr(buffer));

    // systems
    ((SystemMgmt*) m_spSystemMgmt.get())->updateStencilXML(root);

    // entities
    ((EntityMgmt*)m_spEntityMgmt.get())->updateStencilXML(root);

    FILE *fd;
    fd = fopen(stencilXmlFile, "w");

    if (fd == NULL) {
        mxmlDelete(root);
        mxmlDelete(xml);

        fclose(fd);

        return lres_e_writefile;
    }
    else {
        mxmlSaveFile(xml, fd, MXML_NO_CALLBACK);

        mxmlDelete(root);
        mxmlDelete(xml);

        fclose(fd);
    }

    return lres_success;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif