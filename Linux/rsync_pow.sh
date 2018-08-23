user=lqx
host=stttpow

rootdir=/home/lqx/code/pow/Linux

rsync --exclude ".git" --exclude ".idea" -aruv --verbose ./ $user@$host:"$rootdir"

