
## Vorbemerkung

### Kompilierung

Es gibt Änderungen zur Kompilierung. Ich habe eine weitere Kompilierungsart (über cmake) hinzugefügt. Der Grund dafür ist, dass aktuell der gesamte Code immer wieder neu kompiliert werden muss, inklusive der umfangreichen ImGui Bibliotheken, was die Kompilierung sehr zeitaufwendig macht.

Damit beide Kompilierungsarten parallel funktionieren, gibt es ein Makro, das man ein- bzw. auskommentieren muss. Direkt in der main.cpp, gleich die erste Zeile beinhaltet ein "#define INCLUDE_CPP".

* Kompilieren über g++: #define INCLUDE_CPP einkommentieren
* Kompilieren über cmake: #define INCLUDE_CPP auskommentieren

### Spielstart Änderung
Beim Starten wird ein schwarzer Bildschirm angezeigt. Beim Druck auf "M" wird ein Spiel gestartet. Du kannst ebenfalls ein Spiel starten lassen (z.B. eine neue GUI), wenn du ein das Event "StartTestGameEvent" aufrufst. Dieses Event wird von der App-Klasse abgefangen und ruft daraufhin eine Methode auf, die alles Weitere in die Wege leitet.


## ECS System

### Allgemein
Alle spielrelevanten Objekte (Map, Spieler, Einheiten, Gebäude, Wände, Gegener-Spawner, Projektile, ...) werden als Entitäten realisiert. Entitäten werden von einem EntityManager verwaltet und können alle mit einer eigenen ID (einer hochzählenden Ganzzahl) identifiziert werden. Je nach Rolle der Entität wird ihm verschiedene Komponenten zugewiesen. Beispielsweise braucht eine bewegliche Einheit mindestens die Komponenten Position und Movement (Position, damit die Position getrackt wird und Movement, damit die Spiellogik weiß, dass und wie es zu bewegen ist). Während der Laufzeit können Komponenten hinzugefügt und entfernt werden. Jede Entität hat zudem noch eine AssetID, im Grunde ein String, an dem ablesen kann, von welchem Asset sie entstanden ist.

Man kann die Entitäten je nachdem, welche Komponenten sie haben, in verschiedene Archetypen einteilen. Hierzu eine Liste unten.

Alle Entitäten werden über den EntityManager in eine Hierarchie geordnet. D.h. (fast) jede Entität hat einen Parent und kann Children haben. Beispielsweise besteht jedes Spiel aus einer World (= Entität mit einer World-Komponente), das eine oder mehrere Maps (= Entitäten mit einer Map-Komponente) unter sich haben kann. Jede Map wiederum hat verschiedenste Einheiten und Objekte als Kinder. Jede Entität kann durch gewissermaßen als Container fungieren. Dies ist aktuell besonders für zwei Dinge nützlich: Einerseits damit das Spiel weiß, welche Objekte auf welcher Map gehören und andererseits können Objekte sogenannte Techs (= Entitäten mit einer Tech-Komponente) beinhalten (siehe unten).

Wenn eine Entität stirbt, werden rekursiv alle Children ebenfalls gelöscht.

### Assets
Assets ist eine Vorlage für eine Entität. Technisch ist ein Asset ebenfalls eine Entität. Sie wird allerdings von einem AssetManager verwaltet und ihre Identifizierung läuft über eine AssetID (ein String). Die Spiellogik beinhaltet Funktionalitäten, um auf Basis eines Assets eine Entität zu erzeugen: Dabei wird das Asset kopiert und dem EntityManager übergeben. Assets sind damit an sich nicht Teil der eigentlichen Spiellogik - es sind nur Vorlagen. Ein Vorteil dadurch ist, dass man dem Spiel sagen kann "erstelle eine neue Entität mit Vorlage XYZ", anstatt dass man mühsam sämtliche Komponenten und Attribute manuell eintragen muss.

Es gibt spezielle Asset-Komponenten. Das sind Komponenten, die für Assets relevant sind, aber für Entitäten an sich nicht mehr. Sie werden beim kopieren also ausgelassen (d.h. eine Entität enthält dadurch nie eine Asset-Komponente). Beispiel hierfür wäre die Textkomponente, die Name und Beschreibung (potentiell in mehreren Sprachen) beinhalten kann. Es wäre Verschwendung, wenn diese Komponente immer mitkopiert werden würde. Stattdessen würde man in dem Fall das Asset der Entität bestimmen und über das Asset dann die Texte auslesen.

### Techs
Tech ist die Abkürzung für Technologie und soll eine Art von Objekt beschreiben, dass Einfluss nehmen kann auf das Objekt. Jede Tech ist eine Entität mit einer TechComponent (und potentiell noch weitere tech-relevante Komponenten). Techs lösen vielfältige Aufgaben:
* Damit können Buffs/Debuffs realisiert werden (z.B. Einheit wird langsamer oder ist mit einem DoT belegt).
* Einheiten können Aktionen zugewiesen werden (z.B. Einheit bewegen, oder über das Hauptgebäude einen Tower bauen).
* Es kann als Merker dienen, dass eine bestimmte Technologie erforscht ist.

Jede Tech hat einen Besitzer (das wäre einfach der Parent) und potentiell mehrere Assignees (also zugewiesene Entitäten). Der Besitzer ist nicht unbedingt derjenige, der von der Tech betroffen ist - der Besitzer ist nur insofern für die Tech zuständig, dass beim Löschen des Besitzers die Tech auch gelöscht wird (beispielsweise könnte eine Commander-Einheit alle umliegenden Einheiten buffen, wäre aber nicht selbst betroffen. Wenn der Commander stirbt, geht der Buff auch verloren).
Die Tech-Komponente hat diverse Optionen, um festzustellen, wer von der Tech betroffen sein soll (z.B. alle Entitäten, alle Entitäten in einem Radius, alle Entitäten mit gewissen Komponenten, alle Entitäten eines Spielers, etc.).

### Commands
Commands sind ebenfalls Techs, die allerdings noch eine Command-Komponente haben. Command ist die Möglichkeit eines Spielers, eine Aktion auszuführen (z.B. Bewegen, bauen). Die Möglichkeit, eine Aktion auszulösen, ist gewissermaßen auch eine Art Buff :)
TODO: genauer

## Komponenten-Liste

### Reguläre Komponenten

#### PositionComponent
Dient dazu, die Position eines Objektes (x und y Attribut) aufzunehmen. Hat zusätzlich noch ein xRel und yRel, sowie ein followParent Attribut. Falls letzteres auf true gesetzt ist, wird die Position über die Spiellogik immer an den jeweiligen Parent gesetzt, mit xRel und yRel als Offset.

#### MovementComponent
Dient dazu, die Bewegung eines Objektes aufzunehmen. Kann auch Ziele beinhalten, was komplexere Wegfindung ermöglicht. Jedes Objekt, das sich bewegen können soll, benötigt diese Komponente.
TODO: Mehr erklären

#### CollisionComponent
Dient dazu, kollisionsrelevante Daten aufzunehmen.

Sie beinhaltet eine Liste an "Shapes" (jedes Shape ist ein Objekt; aktuell gibt es zwei Shapes: Kreis und Rechteck). Jedes Shape hat einen X- und einen Y-Offset. Dadurch kann sich die Form eines Objektes aus verschiedenen Formen zusammensetzen und ermöglicht komplexere Hitboxes. So könnte beispielsweise ein Gebäude L-förmig sein, oder eine Flamme aus verschiedenen Kreisen (wie es bei vielen alten Games war, z.B. bei Contra).
Weiterhin gibt es einen "DummyRadius". Sollte diese Komponente zu einer Entität keine Shape haben, wird der DummyRadius ersatzweise für Kollisionsberechnungen herangezogen. Bei manchen simpleren Berechnungen wird ebenfalls der DummyRadius verwendet, anstatt der Shapes (z.B. für die Wegfindung von beweglichen Objekten).
Außerdem ist noch das Attribut BoundingBox interessant. Die BoundingBox beschreibt im Endeffekt ein minimal-großes Rechteck, das die komplette Hitbox miteinschließt. Dies ist nützlich für Vorberechnungen von Kollisionen, und es ermöglicht dem Programm, einen sinnvollen Mittelpunkt eines Objektes zu finden.

**WICHTIG**: Macht nur im Zusammenspiel einer PositionComponent Sinn!


## Archetypen
...

