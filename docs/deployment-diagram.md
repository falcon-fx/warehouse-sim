```plantuml
node "személyi számítógép" <<device>> {
[kliens szoftver]
[Qt] as QtClient
}
node szerver <<device>> {
[szimulációs szoftver]
[Qt] as QtServer
artifact JSON
}
[kliens szoftver] .> QtClient
[kliens szoftver] --> [szimulációs szoftver]
[szimulációs szoftver] .> QtServer
[szimulációs szoftver] -- JSON
```