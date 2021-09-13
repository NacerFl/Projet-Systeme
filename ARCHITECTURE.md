# Architecture logicielle

Afin de faciliter notre projet, en plus des librairies standard du C, nous avons utilisé la librairie <sys/xattr.h>. Cette librairie permet de manipuler les attributs étendus sur un système de fichiers, c'est-à-dire, d'associer des métadonnées, autrement dit un certain nombre d'informations supplémentaires, à un i-noeud.

Notre projet compose 14 fonctions, le main inclut, dont 5 fonctions principales, markTag, removeTag, listTag, combinationList, makeHierarchy. Elles permettent respectivement de marquer un fichier avec un ou plusieur tag, effacer un ou plusieur tag d'un fichier, lister les tags d'un fichier, lister les fichiers possédant une combinaison de tags, spécifier un lien hiérarchique entre plusieurs tags.

**marquer un tag**

Le prototype de la fonction markTag est le suivant : void markTag(char *path, char *attribute)
Cette fonction prend en argument une chaîne de caractères représentant un chemin et une chaîne de caractères représentant un tag. Une vérification va d'abord s'effectuer pour savoir si le tag existe déjà ou non grâce à la valeur de retour de la fonction tagExist. S'il n'existe pas, le fichier/dossier va être marqué du tag mais aussi des tags qui spécifient un lien hiérarchique avec le tag en question. Pour marquer le tag, nous utilisons la fonction int setxattr (const char *chemin, const char *nom, const void *valeur, size_t taille, int options) de la librairie <syst/xattr.h> qui va écrire la valeur dans l'attribut étendu identifié par le nom et associé avec le chemin dans le système de fichiers.

**effacer un tag**

Le prototype de la fonction removeTag est le suivant : void removeTag(char *path, char *attribute)
Cette fonction prend en paramètre une chaîne de caractères représentant un chemin et une chaîne de caractères représentant un tag. Elle permet d'effacer le tag du fichier dont on a spécifié son chemin dans les paramètres de la fonction. La fonction s'occupe seulement de passer le chemin du fichier et l'attribut à la fonction int removexattr(const char *chemin, const char *nom) de la librairie <syst/xattr.h> qui se charge de supprimer le tag.

**appliquer une hiérarchie**

Le prototype de la fonction applyHierarchy est le suivant : void applyHierarchy(char *currentPATH, char *attribute)
Cette fonction prend en paramètres une chaîne de caractères représentant un chemin et une chaîne de caractères représentant un tag. Elle va parcourir à partir du chemin passé en paramètre tous les dossiers/fichiers enfants pour appliquer toutes les hiérarchies avec ce tag, c'est-à-dire, marquer les fichiers des tags qui spécifient un lien hiérarchique avec le tag passé en second paramètre de la fonction.

**créer une hiérarchie**

Le prototype de la fonction makeHierarchy est le suivant : void makeHierarchy(char *attributes[], int numberAttributes)
Cette fonction prend en paramètres un tableau d'une chaînes de caractères représentant des tags et la taille de ce tableau. Elle permet de spécifier un lien hiérarchique entre les tags. Ce lien sera ensuite écrit dans le fichier /usr/bin/hierarchy . Une fois la hiérarchie créée, elle va utiliser la fonction applyHierarchy qui va se charger de parcourir tous les dossiers à partir de la racine pour appliquer la hiérarchie.

**vérifier l'existence d'un tag**

Le prototype de la fonction tagExist est le suivant : int tagExist(char *path, char *attribute)
Cette fonction prend en paramètres une chaîne de caractères représentant et une chaîne de caractères représentant un tag. Elle permet de vérifier l'existence d'un tag sur un fichier. Cette fonction utilise la fonction ssize_t listxattr (const char *chemin, char *liste, size_t taille) de la librairie <syst/xattr.h> qui récupère la liste des attributs étendus associés avec le chemin indiqué du système de fichiers. La liste est l'ensemble des noms (terminés par NULL), l'un après l'autre. Les noms des attributs étendus auxquels le processus appelant n'a pas accès peuvent être omis dans la liste. La longueur de la liste de noms d'attributs est renvoyée. Une fois avoir récupéré la liste, la fonction parcourt cette dernière et vérifie que l'attribut passé en argument de la fonction n'est pas présent dedans.

**lister les tags d'un fichier**

Le prototype de la fonction listTag est le suivant : void listTag(char *path)
Cette fonction prend en paramètre une chaîne de caractères représentant un chemin. Elle utilise la fonction ssize_t listxattr (const char *chemin, char *liste, size_t taille) de la librairie <syst/xattr.h> qui récupère la liste des attributs étendus associés avec le chemin indiqué du système de fichiers. La liste est l'ensemble des noms (terminés par NULL), l'un après l'autre. Les noms des attributs étendus auxquels le processus appelant n'a pas accès peuvent être omis dans la liste. La longueur de la liste de noms d'attributs est renvoyée. Une fois avoir récupéré la liste, la fonction parcourt cette dernière et affiche les attributs présents dedans.

**vérifier la complémentarité d'un tag**

Le prototype de la fonction complementaryAttribute est le suivant : int complementaryAttribute(char *string)
Cette fonction prend en paramètre une chaîne de caractères représentant un attribut et vérifie s'il est complémentaire. Par exemple renvoie 1 pour "pas(bleu)" et -1 pour "bleu".

**extraction de l'attribut**

Le prototype de la fonction extractAttributeFromComplementary est le suivant : char * extractAttributeFromComplementary(char *complementary)
Cette fonction prend en paramètre une chaîne de caractères représentant un attribut complémentaire. Elle extrait l'attribut et le retourne. Par exemple pour "pas(bleu)", elle renvoie "bleu".

**récupérer le nombre de points d'une hierarchie**

Le prototype de la fonction getNumberPoint est le suivant : int getNumberPoint(char *hierarchy)
Cette fonction prend en paramètre une chaîne de caractères représentant une hiérarchie. Elle renvoie le nombre de caractère point qui apparaît dedans. Par exemple pour couleur.bleu.bleu-marin, elle renvoie 2.

**récupérer le premier attribut d'une hierarchie**

Le prototype de la fonction getFirstAttribute est le suivant : char * getFirstAttribute(char *hierarchy)
Cette fonction prend en paramètre une chaîne de caractères représentant une hiérarchie. Elle renvoie le premier attribut de cette chaîne. Par exemple pour "couleur.bleu.bleu-marin", elle renvoie "couleur".

**vérifier l'existance d'un attribut dans une hierarchie**

Le prototype de la fonction lineContainAttribute est le suivant : int lineContainAttribute(char *hierarchy, char *attribute)
Cette fonction prend en paramètres une chaîne de caractères représentant une hiérarchie et une chaîne de caractères représentant un tag. Elle renvoie 1 si la hierarchie contient le tag, -1 sinon. Par exemple pour la hiérarchie couleur.bleu.bleu-marin et le tag bleu, elle renvoie 1.

**récupérer les fichiers respectant une certaine combinaison de tag**

Le prototype de la fonction getTheFilesThatMatchCombination est le suivant : int getTheFilesThatMatchCombination(char *currentPATH, char *attributes[], int numberAttributes, char *match[], int lengthMatch)
Cette fonction prend en paramètres une chaîne de caractères représentant un chemin, un tableau de chaînes de caractères représentant une combinaison, la taille de ce tableau, un tableau de chaînes de caractères dans lequel on va mettre au fur et à mesure les fichiers qui répondent correctement à la combinaison, et la longueur de ce tableau. Elle parcourt depuis la racine tous les dossiers accessibles et vérifie pour chaque fichier rencontré si elle respecte la combinaison. Si c'est le cas, elle insère le chemin du fichier dans le tableau match et trie le tableau en supprimant les doublons, c'est-à-dire en enlevant les fichiers qui ont le même i-noeud.

**afficher les fichiers respectant une certaine combinaison de tag**

Le prototype de la fonction combinationList est le suivant : void combinationList(char *attributes[], int numberAttributes)
Cette fonction prend en paramètre un tableau de chaînes de caractères représentant une combinaison et un entier représentant la taille de ce tableau. Elle appelle la fonction getTheFilesThatMatchCombination pour récupérer un tableau qui contient les chemins des fichiers qui respectent la combinaison et les affiche.

# Structures

La librairie <sys/stat.h> nous a servi pour trier les fichiers possédant un même i-noeud lors de l'affichage des fichiers répondant à une combinaison de tags.
De plus, la librairie <dirent.h> nous a également servi pour obtenir des informations sur les dossiers/fichiers enfants à partir d'un chemin donné.

# Stockage et mis à jour des tags

La fonction setxattr de la librairie <syst/xattr.h> stock, le tag et chemin du fichier donné en paramètre, dans l'i-noeud du fichier.
La fonction removexattr de la librairie <syst/xattr.h> supprime, le tag et chemin du fichier donné en paramètre, de l'i-noeud du fichier.

# Hiérarchie et programme grobal

Pour créer une hiérarchie, il faut utiliser la commande tag à l'aide de l'option h et spécifier la hiérarchie, voici un exemple : tag -h couleur bleu
Cette hiérarchie sera ensuite stocké dans le fichier hierarchy, qui est créée et deplacé dans le dossier /usr/bin ,  de la forme : couleur.bleu
Pour rendre le programme exécutable de n'importe quel endroit du système, nous déplaçons, une fois avoir créé l'exécutable tag, dans le dossier /usr/bin .