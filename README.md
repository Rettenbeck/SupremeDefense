Es gibt Änderungen zur Kompilierung. Ich habe eine weitere Kompilierungsart (über cmake) hinzugefügt. Der Grund dafür ist, dass aktuell der gesamte immer wieder neu kompiliert werden muss, inklusive der umfangreichen ImGui Bibliotheken, was die Kompilierung sehr zeitaufwendig macht.

Damit beide Kompilierungsarten parallel funktionieren, gibt es ein Makro, das man ein- bzw. auskommentieren muss. Direkt in der main.cpp, gleich die erste Zeile beinhaltet ein "#define INCLUDE_CPP".

-> Kompilieren über g++: #define INCLUDE_CPP einkommentieren

-> Kompilieren über cmake: #define INCLUDE_CPP auskommentieren

Weitere Änderung: Beim Starten wird ein schwarzer Bildschirm angezeigt. Beim Druck auf "M" wird ein Spiel gestartet. Du kannst ebenfalls ein Spiel starten, wenn du ein bestimmtes Event aufrufst (dieses wird in der App Klasse abgefangen und führt zum Spielstart).
