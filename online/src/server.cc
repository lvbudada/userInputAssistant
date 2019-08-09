#include "SpellcorretServer.h"
#include "Configuration.h"

int main()
{   
    wd::SpellcorretServer server("../conf/my.conf");

    server.start();
    return 0;
}

