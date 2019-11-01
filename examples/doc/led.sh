gpio -g mode 23 output
​
gpio -g write 23 1
sleep 10

gpio -g write 23 0

echo "led"