Projet Architecture logicielle
# INSTALL
> **Executer la commande make pour pouvoir installer le projet.**
# MODE D'EMPLOI
| Commande | Résultat |
| ------ | ------ |
| tag -m file tag1 tag2 tagN | l'option -m permet de marquer un fichier avec un ou plusieurs tag |
| tag -r file tag1 tag2 tagN | l'option -r permet d'effacer un ou plusieurs tag d'un fichier |
| tag -l file | l'option -l permet de lister les tag d'un fichier |
| tag -cl tag1 et pas\(tag2\) et ... | l'option -cl permet de lister les fichiers possédant une combinaison de tags, un fichier possédant plusieurs liens physiques apparaît dans cette liste qu'une fois. Cette commande prend du temps car elle parcourt tous les dossiers accessibles depuis la racine |
| tag -h tag1 ta2 tagN | l'option -h permet de spécifier un lien hiérarchique entre plusieurs tags, cette commande prend du temps car elle parcourt tous les dossiers accessibles depuis la racine |
| rm tagged_file | la commande rm supprime le fichier ainsi que ses tags |
| mv tagged_file target | la commande mv déplace le fichier en préservant ses tags  |
| cp --preserve=xattr tagged_file target | la commande cp copie le fichier et ses tags si l'option --preserve=xattr est utilisé |
| ln tagged_file target | la commande ln créer un lien physique qui va hériter automatiquement des tags, si une modification des tags a lieu sur un des lien cela affectera tous les liens |
