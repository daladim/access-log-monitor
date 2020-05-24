#include "Mail.hpp"
using namespace std;

#include <sstream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include "Pipeline.h"
#include "Timestamp.hpp"
#include "constants.hpp"

namespace LogSupervisor{

Mail::Mail(const string& recipient, const string& subject, const string& content, const string& logPath) :
    recipient(recipient), subject(subject), content(content), logPath(logPath)
{}

bool Mail::send() const{
    ostringstream msg;

    // Add the mail headers, as read by 'sendmail'
    msg << "To: " << recipient << "\n";
    msg << "Subject: " << subject << "\n";
    msg << "Content-type: text/html\n\n";

    // Add the human-readable header
    msg << "<div><b>Parsed " << logPath << "</b><br>\n";
    msg << "on host " << getHostname() << "<br>\n";
    msg << "by user " << getUserName() << " on " << Timestamp::NOW().to_string() << "<br>\n";
    msg << "supervisor version: " << GIT_VERSION_STRING << "</div>\n";

    // Add the content itself
    msg << content;

    // Call a binary to send this message
    const char* const args[] = {mail_binary, "-t", NULL};
    const char* const env[] = {"", NULL};
    string copy = msg.str();
    return execWithStdin(mail_binary, args, env, copy.c_str());
}

string Mail::getUserName() const{
    struct passwd *pw;
    uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);
    if (pw){
        return pw->pw_name;
    }
    return "<unknown>";
}

string Mail::getHostname() const{
#define BUF_SIZE 1024
    char hostname[BUF_SIZE];
    if(gethostname(hostname, BUF_SIZE) == 0){
        return hostname;
    }else{
        return "<unknown>";
    }
}

} // namespace