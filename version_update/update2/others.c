char* getOpenVerFileName(const char* verDir,const char* imgType,\
		const char* gitBranch,char* openVerName)
{

  sprintf(openVerName,"%s/%s_%s_version.xml",verDir,imgType,gitBranch);
  if (access(verDir,0777) == -1){
        mkdir(verDir,0777);
  }
  return openVerName;
}
static char* get_commit_id(const char* buf,char* val)
{
  int i;
  for (i=7;i<strlen(buf);i++)
  {
    if (buf[i]=='\n'){val[i-7]='\0';break;}
    val[i-7]=buf[i];
  }
  return val;
}
static char* get_commit_val(const char* buf,char* val)
{
  int i=0,j;
  while(buf[++i] == ' ') continue;

  for (j=0;i<strlen(buf);i++,j++)
  {
    if (buf[i] == '\n') {val[j]='\0';break;}
    val[j]=buf[i];
  }
  return val;
}
static char* get_lastest_commit_id(char* getBuf)
{
  char buf[300];
  int i;
  FILE* fd = popen("git log ","r");
  fgets(buf,sizeof(buf),fd);
  fclose(fd);

  return get_commit_id(buf,getBuf);
}
static char* get_file_element_value(const char* file,const char* seg,char* getVal)
{
  FILE *fp;
  mxml_node_t *tree;
  fp = fopen(file, "r");
  if (fp == NULL) return NULL;
  tree = mxmlLoadFile(NULL, fp,MXML_TEXT_CALLBACK);
  fclose(fp);

  mxml_node_t *node;
  node = mxmlFindElement(tree, tree, seg, NULL, NULL, MXML_DESCEND);
  
  int i=0;
  if (mxmlGetText(node,&i) == NULL) return NULL;
  strcpy(getVal, mxmlGetText(node,&i));
 
  mxmlDelete(tree);

  return getVal;
}
int is_need_update_ver(const char* fileName)
{
  char buf[1024];
  char buf2[1024];
  get_lastest_commit_id(buf); 
  if (get_file_element_value(fileName,SEG_LATEST_COMMIT_ID,buf2)){
     return strcmp(buf,buf2);
  }
  return 1;
}

static const char * whitespace_cb(mxml_node_t *node,int where)
{
  const char *name;
  name = mxmlGetElement(node);

  if (name[0]=='?' && where == MXML_WS_BEFORE_OPEN){
    return NULL;
  }

  if (where == MXML_WS_BEFORE_OPEN) {
    return "\n";
  }
  if (where == MXML_WS_BEFORE_CLOSE){
    if (!strcmp(name,SEG_DATA) || !strcmp(name,"item")) return "\n";
  } 

  return NULL;
}

int update_file_ver(const char* fileName,const char* board,\
	const char* gitBranch,const char* imgType,const char* startVer)
{
  FILE* fd = popen("git log","r");
  //FILE* fd = fopen("a.txt","r");
  FILE* fdSave= fopen(fileName,"wb");
  if (fd == NULL || fdSave == NULL){
        perror("fopen");
        return -1;
  }

  mxml_node_t *xml;
  mxml_node_t *data;
  mxml_node_t *node;

  mxml_node_t *lastCommitNode;
  char lastComId[500];
  char latestVer[50];

  xml = mxmlNewXML("1.0");
  data = mxmlNewElement(xml, SEG_DATA);

  lastCommitNode = mxmlNewElement(data, SEG_LATEST_COMMIT_ID);
  mxmlNewText(lastCommitNode, 0, get_lastest_commit_id(lastComId));
  node = mxmlNewElement(data, SEG_BOARD);
  mxmlNewText(node, 0, board);
  node = mxmlNewElement(data, SEG_GIT_BRANCH);
  mxmlNewText(node, 0, gitBranch);
  node = mxmlNewElement(data, SEG_IMG_TYPE);
  mxmlNewText(node, 0, imgType);
  

  int count=0;
  while( !feof(fd) )
  {
    int i;
    char buf[1024];
    const char* temp=fgets(buf,sizeof(buf),fd);
    if (temp == NULL) continue;
     /* 是否包含中文 */
    for (i=0;i<strlen(buf);i++){
	if ((buf[i]&0xff)>0x7F) {count++;break;}
     }
  }
  fclose(fd);

  node = mxmlNewElement(data, SEG_LATEST_VER);
  strcpy(latestVer,startVer);
  mxmlNewText(node, 0, ver_add(latestVer,count));

  //rewind(fd); this not work
  fd = popen("git log","r");
  while( !feof(fd) )
  {
    char buf[1024];
    const char* temp=fgets(buf,sizeof(buf),fd);
    if (temp == NULL) continue;

    if (!strncmp("commit",buf,6)){continue;}

    int i;
    int isCh=0; /* 是否包含中文 */
    for (i=0;i<strlen(buf);i++){
	if ((buf[i]&0xff)>0x7F) {isCh=1;break;}
     }

    if (!isCh) continue;
 
    char curVer[50];strcpy(curVer,startVer);
    ver_add(curVer,count--);
    char comVal[300];
    get_commit_val(buf,comVal);

    mxml_node_t * itemData=  mxmlNewElement(data, "item");
    mxml_node_t * nodeVer=  mxmlNewElement(itemData, "ver");
    mxml_node_t * nodeCom=  mxmlNewElement(itemData, "comment");

    mxmlNewText(nodeVer, 0, curVer);
    mxmlNewText(nodeCom, 0, comVal);

  }

  mxmlSaveFile(xml, fdSave,whitespace_cb);
  mxmlDelete(xml);

  fclose(fdSave);
  fclose(fd);
  return 0;
}
