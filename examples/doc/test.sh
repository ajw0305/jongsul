# /bin/bash
declare -a array
array=("wait" "date" "gate" "status")
declare -i num=0;
temp1="wait"

while true; do

while read LINE; do
array[num]=$LINE
(( num++))
done <  id.txt

num=0

while [ "${array[0]}" == "${temp1}" ]; do


while read LINE; do
array[num]=$LINE
(( num++))
done <  id.txt

num=0

echo ${array[0]}
echo ${array[1]}
echo ${array[2]}
echo ${array[3]}

echo "done" > id.txt
done
done