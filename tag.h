#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <grp.h>

#ifndef __TAG__H__
#define __TAG__H__

/*----------------PROTOTYPES----------------*/

void applyHierarchy(char *currentPATH, char *attribute);
int tagExist(char *path, char *attribute);
void removeTag(char *path, char *attribute);
void listTag(char *path);
void makeHierarchy(char *attributes[], int numberAttributes);
int complementaryAttribute(char *string);
char * extractAttributeFromComplementary(char *complementary);
int getNumberPoint(char *hierarchy);
char * getFirstAttribute(char *hierarchy);
int lineContainAttribute(char *hierarchy, char *attribute);
void markTag(char *path, char *attribute);
int getTheFilesThatMatchCombination(char *currentPATH, char *attributes[], int numberAttributes, char *match[], int lengthMatch);
void combinationList(char *attributes[], int numberAttributes);

#endif
