```plantuml

:Felhasználó: as User
(Betöltés) as Load
(Mentés) as Save
(Indítás) as Start
(Sebesség állítása) as SetSpeed
(Szerkesztő megnyitása) as OpenEditor
(Robotok elhelyezése) as NewRobot
(Podok elhelyezése) as NewPod
(Célállomások elhelyezése) as NewDestination
(Töltőállomások elhelyezése) as NewCharger
(Termékek elhelyezése) as NewProduct
(Visszavonás) as Undo
(Újracsinálás) as Redo

(Kilépés) <- User
User --> OpenEditor

OpenEditor ..> NewRobot : <<include>>
OpenEditor ..> NewPod : <<include>>
OpenEditor ..> NewDestination : <<include>>
OpenEditor ..> NewCharger : <<include>>
OpenEditor ..> NewProduct : <<include>>
OpenEditor ..> Save : <<precedes>>
NewRobot ..> Undo : <<precedes>>
NewPod ..> Undo : <<precedes>>
NewDestination ..> Undo : <<precedes>>
NewCharger ..> Undo : <<precedes>>
NewProduct ..> Undo : <<precedes>>
Undo ..> Redo : <<precedes>>
Load ..> Save : <<precedes>>
OpenEditor .> Start : <<precedes>>
Start <. Load : <<precedes>>
Start .> SetSpeed : <<precedes>>

```