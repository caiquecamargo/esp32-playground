IP=192.168.4.1
URI=test

curl "http://$IP/$URI" -i -X GET \
  -H 'Content-Type: application/json'