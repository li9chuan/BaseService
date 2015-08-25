#ifndef TOOLS_SERVICE_DAEMON_THREAD_H
#define TOOLS_SERVICE_DAEMON_THREAD_H

#include <nel/misc/thread.h>
#include <nel/misc/sstring.h>
#include <game_share/buf_fifo_ring.h>

const std::string  START_COUNTER_FILE  = ".start_count";
const std::string  STATE_FILE          = ".state";
const std::string  CTRL_FILE           = ".launch_ctrl";
const std::string  NEXT_CTRL_FILE      = ".deferred_launch_ctrl";

class DaemonThread : public NLMISC::IRunnable
{
public:
    volatile bool  _run;

    void init( std::string path, std::string alias, std::string exec );

    static std::string  ReadFile( std::string filename );
    static void WriteFile( std::string filename, std::string out_txt );

private:

    void run ();

    std::string  PATH;
    std::string  ALIAS;
    std::string  EXEC;
};

#endif

