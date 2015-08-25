
cd ~/MT/code/EVA/server

shard stop

clean_log.sh

rm src/*/*.packed_sheets

make

shard batchstart
