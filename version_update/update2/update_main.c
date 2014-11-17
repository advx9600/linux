#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mxml.h>

#define SEG_DATA "data"
#define SEG_LATEST_COMMIT_ID "lastestCommitId"
#define SEG_BOARD "board"
#define SEG_GIT_BRANCH "gitBranch"
#define SEG_IMG_TYPE "imgType"
#define SEG_LAST_CHINESE_COMMIT "lastChineseCommit"
#define SEG_LATEST_VER "latestVer"

#include "c_lib/my_c_lib.c"
#include "others.c"

#define MY_DIR "my_dir"
#define MY_BIN "my_bin"


int main(int argc,char* argv[])
{
  if (argc != 6){
        printf("argc err\n");
        printf("board(smdkv210,...) startVer(1.0.00)  imgType(uboot,kernel.system) imgVerInputFile(version.c) imgVerOutputFile\n");
        return -1;
  }

  const char* board = argv[1];
  const char* startVer=argv[2];
  const char* imgType = argv[3];
  const char* verIn = argv[4];
  const char* verOut = argv[5];
  char gitBranch[20];
  getCurGitBranch(gitBranch);

  char fileName[50];
  getOpenVerFileName(MY_DIR,imgType,gitBranch,fileName);

  if (!is_need_update_ver(fileName)){
    printf("don't need update\n");
    return 0;
  }
 
  update_file_ver(fileName,board,gitBranch,imgType,startVer);

  char buf[500];
  sprintf(buf,"java -jar %s/updateImg.jar %s %s %s",MY_BIN,fileName,verIn,verOut);
  system(buf);

  return 0;
}
