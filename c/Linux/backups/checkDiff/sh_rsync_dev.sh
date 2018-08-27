user=lqx
host=stttdev

rootdir=/home/lqx/code/pow/equihash-xenon/checkDiff/

rsync --exclude ".git" --exclude ".idea" -aruv --verbose ./ $user@$host:"$rootdir"

