/* Do not remove the headers from this file! see /USAGE for more info. */

#define MAX_IDLE_TIME	600     // This is really x+60 seconds.  LWI.
#define NEEDS_ARG()     if(!arg){ info->cmdPipe->send("500 command not understood.\n"); return; }
#define FTPLOG(x)	LOG_D->log(LOG_FTP, x)
#define FTP_WELCOME      "/data/config/FTPWELCOME"

#ifdef ALLOW_ANON_FTP
#define ANON_PREFIX	"/ftp/pub"
#define ANON_USER()	(member_array(info->user, anon_logins) != -1)
#define ANON_CHECK(x)    if(ANON_USER() && x[0..(strlen(ANON_PREFIX)-1)] != ANON_PREFIX) { info->cmdPipe->send("550 Pemission denied.\n"); if(info->dataPipe) destruct(info->dataPipe); return; }

#else
#define ANON_CHECK(x)
#endif

#define FTP_BLOCK_SIZE  1024

class ftp_session {
   int		connected;
   string	user;
   mixed	priv;
   string	pwd;
   object	dataPipe;
   object	cmdPipe;
   int		idleTime;
   int		binary;
   string	targetFile;
   string       command;
   int          filepos;
   string       renamefrom;
}

private void FTP_CMD_user(class ftp_session, string);
private void FTP_CMD_pass(class ftp_session, string);
private void FTP_CMD_retr(class ftp_session, string);
private void FTP_CMD_stor(class ftp_session, string);
private void FTP_CMD_nlst(class ftp_session, string);
private void FTP_CMD_list(class ftp_session, string);
private void FTP_CMD_pwd(class ftp_session, string);
private void FTP_CMD_cdup(class ftp_session, string);
private void FTP_CMD_cwd(class ftp_session, string);
private void FTP_CMD_quit(class ftp_session, string);
private void FTP_CMD_type(class ftp_session, string);
private void FTP_CMD_mkd(class ftp_session, string);
private void FTP_CMD_port(class ftp_session, string);
private void FTP_CMD_noop(class ftp_session, string);
private void FTP_CMD_dele(class ftp_session, string);
private void FTP_CMD_syst(class ftp_session, string);
string FTP_CMD_retr_callback(object);
