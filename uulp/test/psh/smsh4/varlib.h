/*************************************************************
 Date       : 2020年 05月 31日 星期日 18:24:34 CST
 Author     : rogerg6
 File       : varlib.h
 Description:
*************************************************************/
int VLstore(char *name, char *val);
char *VLlookup(char *name);
int VLexport(char *name);
void VLlist();
int VLenviron2table(char *env[]);
char **VLtable2environ();
