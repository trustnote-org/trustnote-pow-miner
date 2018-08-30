user=lqx
host=stttpow

rootdir=/home/lqx/code/trustnote-pow-miner

rsync --exclude ".git" --exclude ".idea" -aruv --verbose ./ $user@$host:"$rootdir"

