user=lqx
host=stttdev

rootdir=/home/lqx/code/pow/Linux

rsync --exclude ".git" --exclude ".idea" --exclude "Makefile" -aruv --verbose ./ $user@$host:"$rootdir"

