user=lqx
host=stttdev

rootdir=/home/lqx/code/pow/equihash-xenon/Linux/demo/

rsync --exclude ".git" --exclude ".idea" -aruv --verbose ./ $user@$host:"$rootdir"

