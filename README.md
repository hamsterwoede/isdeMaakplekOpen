# isdeMaakplekOpen

                        
Je hebt een chinese kloon van een "wemos d1 mini".                        
Daar zit een esp8266 chip en een brein op.                        
Communicatie met je computer gaat via een CH340G chip.
Daar moet je even de driver voor installeren op je Mac.                        
Dan gebruik je (of installeer je) de Arduino IDE van Arduino.cc                        
Bij de settings van Arduino stel je "http://arduino.esp8266.com/stable/package_esp8266com_index.json" in als "Additional board managers URL"                        
Dan bij Tools -> Boards -> Board Manager                        
esp8266 installeren.                        
Let op: OOK op OSX eerst driver installeren en dan pas voor het eerst aansluiten.                        
Let op 2: de installatie van de driver vereist herstarten, ook bij OSX.

Dan code van https://github.com/HermanKopinga/isdeMaakplekOpen gebruiken
In Arduino: Tools -> Boards -> "WeMos D1 R2 & mini" kiezen
Dan 'de juiste' compoort selecteren bij Tools -> Port
