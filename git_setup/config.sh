#! bin/bash

#Personal information
echo "\033[36mWrite your name\033[0m"
read NAME
echo "\033[36mWrite your email\033[0m"
read EMAIL

git config user.name "$NAME"
git config user.email "$EMAIL"

echo "\033[36mUser infomation setup finished\033[0m"

#Commit template

git config commit.template git_setup/.gitmessage
git config alias.history "log --oneline --graph --decorate"
echo "\033[36mGit configuration finished\033[0m"
