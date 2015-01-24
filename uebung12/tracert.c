//TI3 - Uebung 12
//Tutor: Thomas
//Bearbeiter: Jasmine Cavael und Maximilian Stendler

/*
Begriffserklaerungen:

Traceroute: Traceroute ist ein Programm, welches eine Route bis zu einem gewuenschten Host anzeigt.
            Dies wird dadurch realisisert, dass ein Internet-Paket mit einem TTL von 1 versendet wird, wodurch es
            vom ersten Router, der dieses Paket bekommt verworfen wird. Der Router schickt eine Fehlermeldung zurueck.
            Dies wird jeweils mit einem inkrementierten TTL wiederholt, bis der gewuenschte Host erreicht wird.
            Alle Router, die dabei einen Fehler gemeldet haben, bilden dann eine Route.

Time-To-Live(TTL): TTL in IP Paketen bezeichnet die uebrige Lebensdauer eines Paketes bis es verworfen wird.
                  Jeder Hop, der dieses Paket erhaelt dekrementiert den TTL um 1 und verwirft das Paket, wenn der TTL 0 erreicht.
                  Zusaetzlich wird ein Fehler dem urspruenglichen Sender des Pakets uebermittelt.
                  Diese Methode wird verwende, damit Pakete, die ihr Ziel nicht finden, nicht das Netzwerk fluten können.

Internet Control Message Protocol (ICMP): Das ICMP wird von Netzwerk Geraeten dazu verwendet, um Fehlerberichte zu uebermitteln.
            Sollte z.B. der TTL eines IP Pakets 0 erreichen, dann wird der Hop einen Fehler mittels ICMP versenden.
            In dem 8-Byte Header vom ICMP wird unter anderem der Typ des Fehlers angegeben.

Raw-Socket: Raw-Sockets sind Internet-Sockets, die nicht wie Standart-Sockets auf dem Transport Layer aufsetzen, sondern
            einen Layer tiefer Raw-Packets inklusive header senden und empfangen koennen.

*/
