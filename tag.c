#include "tag.h"

int currentUserBelongsGroup(gid_t group){
  int nbrgroups;

  nbrgroups = getgroups(0, NULL);

  gid_t groups[nbrgroups];

  getgroups(nbrgroups, groups);

  for(int i = 0; i < nbrgroups; i++){
    if(groups[i] == group)
      return 1;
  }
  return -1;
}

void markTag(char *path, char *attribute){
  char *line, *tag, *firstAttribute;
  struct stat fileStatus;
  size_t len;
  ssize_t read;
  FILE *file;

  if(tagExist(path, attribute) != 1){
    tag = malloc(sizeof(char) * (strlen("user.") + strlen(attribute) + 1));

    if(tag == NULL){
      perror("error malloc");
      exit(EXIT_FAILURE);
    }

    sprintf(tag, "%s%s", "user.", attribute);

    if(setxattr((const char *)path, tag, "", sizeof(""), XATTR_CREATE) > -1){
      printf("%s %s\n", attribute, "added.");
      free(tag);
    }else{
      perror("error markTag");
      free(tag);
      exit(EXIT_FAILURE);
    }
  }

  len = 0;
  line = NULL;
  if((file = fopen("/usr/bin/hierarchy", "r")) != NULL){
    while((read = getline(&line, &len, file)) != -1){
      firstAttribute = getFirstAttribute(line);
      if(lineContainAttribute(line, attribute) == 1 && tagExist(path, firstAttribute) != 1){
        printf("%s\n", firstAttribute);
        markTag(path, firstAttribute);
      }
      free(firstAttribute);
    }
    if(line)
      free(line);
    fclose(file);
  }else{
    perror("error open file");
    exit(EXIT_FAILURE);
  }
}

void applyHierarchy(char *currentPATH, char *attribute){
  char *path;
  struct dirent *read;
  DIR *dir;
  FILE *file;

  if((dir = opendir(currentPATH)) != NULL){
    while((read = readdir(dir)) != NULL){

      path = malloc(sizeof(char) * (strlen(currentPATH) + 1 + strlen(read->d_name) + 1));

      if(path == NULL){
        perror("error malloc");
        exit(EXIT_FAILURE);
      }

      strcat(strcat(strcpy(path, currentPATH), "/"), read -> d_name);

      if(read->d_type == DT_DIR){
        if(strcmp(read -> d_name, ".") != 0 && strcmp(read -> d_name, "..") != 0){
          applyHierarchy(path, attribute);
        }
      }

      if(tagExist(path, attribute) == 1){
        markTag(path, attribute);
      }

      free(path);
    }
    closedir(dir);
  }
}

int tagExist(char *path, char *attribute){
  char *tag, *index, *string, *ptr, *tmp;
  int i;
  ssize_t len;

  len = listxattr(path, NULL, 0);

  if(len > 0){
    tag = malloc(sizeof(ssize_t) * len);

    if(tag == NULL){
      perror("error malloc");
      exit(EXIT_FAILURE);
    }

    len = listxattr(path, tag, len);
    index = tag;
    while(len > 0){
      ptr = strtok(index, ".");
      while(ptr != NULL){
        if(strcmp(ptr, attribute) == 0){
          free(tag);

          return 1;
        }
        ptr = strtok(NULL, ".");
      }

      len -= strlen(index) + 1;
      if(len > 0){
        index += strlen(index) + 1;
      }
    }

    free(tag);
  }

  return -1;
}

void removeTag(char *path, char *attribute){
  char *tag;

  tag = malloc(sizeof(char) * (strlen("user.") + strlen(attribute)));

  if(tag == NULL){
    perror("error malloc");
    exit(EXIT_FAILURE);
  }

  strcat(strcat(tag, "user."), attribute);

  if(removexattr(path, tag) > -1){
    printf("%s %s\n", attribute, "removed.");
    free(tag);
  }else{
    perror("error remove tag");
    free(tag);
    exit(EXIT_FAILURE);
  }
}

void listTag(char *path){
  char *tag, *index;
  int i;
  ssize_t len;

  len = listxattr(path, NULL, 0);

  if(len > 0){
    tag = malloc(sizeof(ssize_t) * len);

    if(tag == NULL){
      perror("error malloc");
      exit(EXIT_FAILURE);
    }

    len = listxattr(path, tag, len);

    index = tag;
    while(len > 0){
      for(i = strlen("user."); i < strlen(index); i++){
        printf("%c", *(index + i));
      }
      putchar('\n');
      len -= strlen(index) + 1;
      if(len > 0){
        index += strlen(index) + 1;
      }
    }

    free(tag);
  }else if(len == 0){
    printf("%s\n", "No tag to list.");
    exit(EXIT_SUCCESS);
  }else{
    perror("error list tag.");
    exit(EXIT_FAILURE);
  }
}

void makeHierarchy(char *attributes[], int numberAttributes){
  char *tag, *line;
  size_t len;
  int boolean, i;
  ssize_t read;
  FILE *file;

  len = 0;
  for(i = 2 ; i < numberAttributes; i++){
      len = len + strlen(attributes[i]);
  }

  tag = malloc(sizeof(char) * (len + (numberAttributes - 2) + 1));

  if(tag == NULL){
    perror("error malloc\n");
    exit(EXIT_FAILURE);
  }

  for(i = 2; i < numberAttributes; i++){
    strcat(strcat(tag, attributes[i]), ".");
  }
  strcat(tag, "\n");

  if((file = fopen("/usr/bin/hierarchy", "r")) != NULL){
    boolean = -1;
    len = 0;
    line = NULL;
    while((read = getline(&line, &len, file)) != -1){
      if(strcmp(line, tag) == 0){
        boolean = 1;
      }
    }
    if(line){
      free(line);
    }
    fclose(file);

    if(boolean == -1){
      if((file = fopen("/usr/bin/hierarchy", "a")) != NULL){
        fputs(tag, file);
        fclose(file);
        applyHierarchy("/", attributes[numberAttributes - 1]);
      }else{
        free(tag);
        perror("error open file");
        exit(EXIT_FAILURE);
      }
    }

    free(tag);
  }else{
    free(tag);
    perror("error open file.");
    exit(EXIT_FAILURE);
  }
}

int complementaryAttribute(char *string){
  int i;

  if(strlen(string) >= 5){
    if(*string != 'p' || *(string + 1) != 'a' || *(string + 2) != 's' || *(string + 3) != '('){
      return -1;
    }
    if(*(string + strlen(string) -1) == '\n'){
      if(*(string + strlen(string) - 2) != ')'){
        return -1;
      }
    }else{
      if(*(string + strlen(string) -1) != ')'){
        return -1;
      }
    }
  }

  return 1;
}

char * extractAttributeFromComplementary(char *complementary){
  char *tag;
  int i;

  tag = malloc(sizeof(char) * ((strlen(complementary) - 5) + 1));
  if(tag == NULL){
    perror("error malloc");
    exit(EXIT_FAILURE);
  }

  for(i = 4; i < strlen(complementary) - 1; i++){
    *(tag + i - 4) = *(complementary + i);
  }
  *(tag + i - 4) = '\0';

  return tag;
}

int getNumberPoint(char *hierarchy){
  int i, count;

  count = 0;
  for(i = 0; i < strlen(hierarchy); i++){
    if(*(hierarchy + i) == '.'){
      count += 1;
    }
  }

  return count;
}

char * getFirstAttribute(char *hierarchy){
  char *tag;
  int i, len;

  len = 0;
  for(i = 0; i < strlen(hierarchy); i++){
    if(*(hierarchy + i) != '.')
      len += 1;
    else
      break;
  }

  tag = malloc(sizeof(char) * (len + 1));

  if(tag == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < len; i++){
    *(tag + i) = *(hierarchy + i);
  }
  *(tag + i) = '\0';

  return tag;
}

int lineContainAttribute(char *hierarchy, char *attribute){
  if(strstr(hierarchy, attribute) != NULL){
    return 1;
  }

  return -1;
}

int getTheFilesThatMatchCombination(char *currentPATH, char *attributes[], int numberAttributes, char *match[], int lengthMatch){
	struct stat currentMathchedFileStatus, alreadyMathchedFileStatus;
  int i, sauteDeDeux, boolean, numberMatch;
  char *path, *tmp;
  struct dirent *read;
  DIR *dir;
  FILE *file;

  numberMatch = 0;
  if((dir = opendir(currentPATH)) != NULL){
    while((read = readdir(dir)) != NULL){

      path = malloc(sizeof(char) * (strlen(currentPATH) + 1 + strlen(read->d_name) + 1));

      if(path == NULL){
        perror("error malloc");
        exit(EXIT_FAILURE);
      }

      strcat(strcat(strcpy(path, currentPATH), "/"), read -> d_name);

      if(read->d_type == DT_DIR){
        if(strcmp(read -> d_name, ".") != 0 && strcmp(read -> d_name, "..") != 0){
          numberMatch += getTheFilesThatMatchCombination(path, attributes, numberAttributes, match, lengthMatch);
        }
      }
      
      boolean = 1;
      if((numberAttributes - 2) % 2 == 1){
        sauteDeDeux = 2;
        for (i = 0; i < ((numberAttributes - 2) / 2) + 1; i++) {
          if(complementaryAttribute(attributes[sauteDeDeux]) == 1){
            tmp = extractAttributeFromComplementary(attributes[sauteDeDeux]);
            if(tagExist(path, tmp) == 1){
              boolean = -1;
            }
            free(tmp);
          }else{
            if(tagExist(path, attributes[sauteDeDeux]) == -1){
              boolean = -1;
            }
          }
          sauteDeDeux += 2;
        }
        if(boolean == 1){
          numberMatch += 1;
          for(i = 0; i < lengthMatch; i++){
            if(match[i] == NULL){        
              match[i] = malloc(sizeof(char) * (strlen(path) + 1));
              sprintf(match[i], "%s", path);
              break;
            }

            if(lstat(path, &currentMathchedFileStatus) == 0 && lstat(match[i], &alreadyMathchedFileStatus) == 0){
              if(currentMathchedFileStatus.st_ino == alreadyMathchedFileStatus.st_ino){
                break;
              }
            }
          }
        }
      }else{
        free(path);
        printf("%s\n", "incorrect format combination list");
        exit(EXIT_FAILURE);
      }

      free(path);
    }
    closedir(dir);
  }

  return numberMatch;
}

void combinationList(char *attributes[], int numberAttributes){
  int i, numberMatch;

  numberMatch = getTheFilesThatMatchCombination("/", attributes, numberAttributes, NULL, 0);

  char *match[numberMatch];

  for(int i =0; i < numberMatch; i++){
    match[i] = NULL;
  }

  getTheFilesThatMatchCombination("/", attributes, numberAttributes, match, numberMatch);
  
  for(i = 0 ; i < numberMatch; i++){
    if(match[i] != NULL)
      printf("%s\n", match[i]);
  }

  for(i = 0; i < numberMatch; i++){
    free(match[i]);
  }
}

int main(int argc, char *argv[]){
  int i;
  struct stat test1, test2, test3;

  if(argc >= 3){
    // add tag
    if(strcmp(argv[1],"-m") == 0){

      for(i = 3; i < argc; i++){
        markTag(argv[2], argv[i]);
      }

    // remove tag
    }else if(strcmp(argv[1],"-r") == 0){

      for(i = 3; i < argc; i++){
        removeTag(argv[2], argv[i]);
      }

    // list tag
    }else if(strcmp(argv[1],"-l") == 0){

      listTag(argv[2]);

    // combination list tag
    }else if(strcmp(argv[1],"-cl") == 0){

      combinationList(argv, argc);

    // hierarchy tag
    }else if(strcmp(argv[1],"-h") == 0){

      makeHierarchy(argv, argc);

    // wrong flag
    }else{
      printf("incorrect flag\n");
    }
  // wrong number arguments
  }else{
    printf("%s\n", "incorrect number of arguments");
  }

  return 0;
}