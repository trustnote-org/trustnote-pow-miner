user=lqx
host=stttdev

rootdir=/home/lqx/code/trustnote-pow-miner/

rsync --exclude ".git" --exclude ".idea" --exclude "node_modules" -aruv --verbose ./ $user@$host:"$rootdir"
