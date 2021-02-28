```plantuml
[kliens szoftver] <<Qt desktop application>> as client
[szerver szoftver] <<Qt desktop application>> as server
[adatbázis] <<JSON file>> as database
client --0)- server : "  HTTP"
server -0)- database : "  JSON"
```