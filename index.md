# Présentation de Markdown / Gitlab / Mkdocs pour les projets étudiants

Ceci est un guide de démarrage minimaliste pour :

* créer un dépôt Gitlab
* y gérer les mises à jour régulières du projet
* créer la documentation associée, au format markdown
* configurer la génération automatique du site web via MkDocs (comme ce site)
Des références sont données à chaque partie pour approfondir.

## Git
Vous trouverez une introduction au système de sauvegarde / versionning Git sur le site suivant : https://git-scm.com/book/fr/v2/ 

Git est un système de versionning très utilisé en informatique. Il permet de garder une trace des évolutions du code et de faire des branches différentes notamment.
Pour cela il faut installer un serveur Git sur la machine de développement.

En quelques lignes, pour une machine linux (tout est expliqué également pour Windows et Mac sur le site git-scm cité ci-dessus) : 
Pour installer Git sur votre machine : 

* `sudo apt install git-all`

Pour configurer votre Git : 

* `git config --global user.name "Anatole Latuille"`
* `git config --global user.email anatolle.latuille@ens-paris-saclay.fr`
* `git config --global core.editor nano`
* `git config -list`

Une fois un dépôt créer, on crée un instantané du projet via la commande : 

* `git commit -a` 

## GitLab
Pour travailler en équipe ou pour partager son travail avec une communauté de développeur, il est possible d'utiliser un serveur de dépôt compatible Git accessible au public. GitHub (propriété de Microsoft) et GitLab.com sont les 2 serveurs de dépôt les plus utilisés. GitLab est un service opensource qui peut être installé sur un serveur privé. gitlab.crans.org en est un exemple.

On peut alors envoyer ses modifications du PC au serveur via la commande push et récupérer les modifications via la commande pull.

![commandes GitLab](https://gitlab.com/ajuton/utilisationgitlab/-/raw/master/Images/commandesgitlab2.png)

Créer un compte sur un serveur GitLab (gitlab.com ou gitlab.crans.org par exemple).
Créer un projet vide sur le serveur (Project > Create Blank Project)
Obtenir le lien http vers ce projet (Bouton bleu Clone) : 

![bouton clone](https://gitlab.com/ajuton/utilisationgitlab/-/raw/master/Images/clone.png)

Cloner le dépôt gitlab sur le PC de développement : 

* `git clone https://gitlab.com/alatuille/utilisationgitlab.git`

Pour ajouter un fichier au suivi ou pour indexer ce fichier après des modifications : 

* `git add Image/image.png`

pour indexer et valider toutes les modifications

* `git commit -a`

pour connaître les serveurs associés à ce dépôt : 

* `git remote -v`

pour envoyer vers le dépôt vers le serveur après avoir validé les changements (commit) : 

* `git push origin nom_branche`

pour récupérer les données distantes : 

* `git fetch origin` 


## Markdown
Le format retenu le plus souvent pour les fichiers texte d'explication des projets est Markdown.
Un fichier Markdown est un fichier texte, d’extension .md, avec des balises très simples. 
VSCode ou VSCodium permettent d’éditer des fichiers Markdown en visualisant la mise en forme en temps réel. Typora est un logiciel gratuit spécialisé pour l’édition de fichiers Markdown.

![copie d'écran de VSCode](https://gitlab.com/ajuton/utilisationgitlab/-/raw/master/Images/copieecranvscode.png)

Voici un lien vers les éléments de syntaxe Markdown : https://wprock.fr/blog/markdown-syntaxe/

Pour insérer des images, il faut déposer les images dans un dossier, habituellement un sous-dossier __images__ dans le dossier __docs__. Pour assurer la robustesse des liens, il est préférable d'ouvrir l'image et d'obtenir l'URL de l'image via *clic droit > Copy Image Location*.

Pour commencer, il est possible de copier ce fichier index.md du dépôt https://gitlab.com/ajuton/utilisationgitlab.

Les fichiers Markdown peuvent ensuite être mis en page sous forme d'un site web élégant via MkDocs, ce qui est expliqué un peu plus loin.

