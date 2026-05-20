git fetch --all
git checkout -b %1

git add .
git commit -m "%2"
git push --set-upstream origin %1
