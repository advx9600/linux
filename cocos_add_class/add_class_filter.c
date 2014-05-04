#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define ITEMGROUP "ItemGroup"
#define CPPPRO "ClCompile"
#define CINCPRO "ClInclude"
#define SETPRO "Include"

static void
set_element_names(xmlNode * a_node,const char* className)
{
    char cppName[100],incName[100];

   strcpy(cppName,"..\\Classes\\");
   strcat(cppName,className);
   strcpy(incName,cppName);

   strcat(cppName,".cpp");
   strcat(incName,".h");

    xmlNode *cur_node = a_node->children;
    for (cur_node; cur_node; cur_node = cur_node->next) {

	if (strcmp(ITEMGROUP,cur_node->name)) continue;
	if (cur_node->children==NULL) continue;

	xmlNode* child = cur_node->children;
	while(child=child->next){
		if (strcmp(child->name,CPPPRO) &&
			strcmp(child->name,CINCPRO))
			continue;

		xmlNode* textNode=xmlCopyNode(child->prev,0);
		if (!strcmp(child->name,CPPPRO)){
			xmlNode* cppNode=xmlNewNode(NULL,CPPPRO);
			xmlSetProp(cppNode,"Include",cppName);
			xmlNode* cppChildNode=xmlNewChild(cppNode,NULL,"Filter","Classes");
			xmlAddChild(child->parent,cppNode);
			xmlAddChild(child->parent,textNode);
			break;
		}

		if (!strcmp(child->name,CINCPRO)){
			xmlNode* incNode=xmlNewNode(NULL,CINCPRO);
			xmlSetProp(incNode,"Include",incName);
			xmlNode* incChildNode=xmlNewChild(incNode,NULL,"Filter","Classes");
			xmlAddChild(child->parent,incNode);
			xmlAddChild(child->parent,textNode);
			break;
		}

	}
    }
}

int main(int argc ,char* argv[])
{
	xmlDoc* doc=NULL;
	xmlNode *root_element = NULL;
	 
	if (argc != 3) {
		printf("openFileName className\n");
		return(1);
	}
	
	const char* openFile=argv[1];
	const char* className=argv[2];
	doc = xmlReadFile(openFile, NULL, 0);
	if (doc == NULL) {
		printf("error: could not parse file %s\n", openFile);
		return -1;
	}

	root_element = xmlDocGetRootElement(doc);
	set_element_names(root_element,className);
	xmlSaveFile(openFile,doc);
	xmlFreeDoc(doc);
	return 0;
}
