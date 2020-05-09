#ifndef _MAIL_HPP_
#define _MAIL_HPP_

#include <iostream>

namespace LogSupervisor{

//! Send an e-mail via the 'sendmail' program, that must be installed and configured on the system<br>
//! Doing so is quite basic, but it removes the hassle of configuring the smtp server a password etc.
class Mail{
public:
    //! Constructor. The arguments will be passed as-is to the 'sendmail' program
    Mail(const std::string& recipient, const std::string& subject, const std::string& content, const std::string& logPath);

    //! Send a mail, an return true on success
    bool send() const;

private:
    static constexpr const char* mail_binary = "/usr/sbin/sendmail";
    const std::string recipient;
    const std::string subject;
    const std::string content;
    const std::string logPath;

    std::string getUserName() const;
    std::string getHostname() const;
    std::string currentDateTime() const;
    int callSendmail() const;
};

} // namespace

#endif // _MAIL_HPP_