IP=192.168.4.1
URI=user
DATA="{\"key1\":\"value1\", \"key2\":\"value2\"}"

curl \
  "http://$IP/$URI" -i -X POST \
  -H 'Content-Type: text/plain' \
  -d "test"