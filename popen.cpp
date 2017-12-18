#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <string>

class CProcess
{
public:
	CProcess(const char* _cmd)
	{
		if(NULL == _cmd)
		{
			printf("T --- NULL == cmd\n");
			exit(-1);
		}
		
		cmd = _cmd;
		fp  = NULL;
	}

	int start_process()
	{
		fp = popen(cmd.c_str(), "r");
		if(!fp) {
			printf("Runing --- [%s]\n",cmd.c_str());
			return -1;
		} else {
			printf("process run success...");
			return 0;
		}
		pclose(fp);
	}

	int stop_process()
	{
		int rtn = false;
		char pid_buf[100];
		FILE* pid_fp = NULL;

		char *cmd_pid = (char *)calloc(200,1);		
		sprintf(cmd_pid, "ps aux | grep %s | grep -v \"sh -c\" | grep -v \"grep\" | awk '{print $2}'",cmd.c_str());
		pid_fp = popen(cmd_pid, "r");
		if(NULL == pid_fp){
			rtn = false;
			printf("Create get child process id task failed!");
			goto err;
		}
			
		if(fgets(pid_buf,100,pid_fp) == NULL) {
			rtn = false;
			printf("Get Child Process id failed!");
			goto err;
		} else {
			printf("T --- pid_buf[%s]\n", pid_buf);
			kill(atoi(pid_buf), SIGINT);  
			rtn = true;
		}
		err:
		pclose(pid_fp);
		pid_fp = NULL;
		printf("Process exited success!!!!");
		return rtn;
	}

	~CProcess()
	{

	}
		
private:	
	std::string cmd;
	FILE *fp;
};
  
int main(void)  
{  
	CProcess cp("./echo.sh");
	cp.start_process();
	sleep(10);
	cp.stop_process();
	
	return 0;
} 

  

