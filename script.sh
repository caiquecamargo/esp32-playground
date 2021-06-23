IP=192.168.4.1
URI=user
DATA="name=cabriola&id=1111"

curl \
  "http://$IP/$URI" -i -X PUT \
  -H 'Content-Type: text/plain' \
  -d $DATA