#include "MyLocalization.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <Foundation/Foundation.h>
#endif


void MyLocal::initLocal()
{	
	ko();
	en();
	ja();
	
//
//	
//	CCDictionary* de = CCDictionary::create();
//	de->setObject(CCString::create("Die Verbindung zum Server ist nicht möglich."), kMyLocalKey_canNotConnectedServer);
//	de->setObject(CCString::create("Berühre den Bildschirm!"), kMyLocalKey_touchPlease);
//	de->setObject(CCString::create("Es ist ein Spiel, dass du\nmit der horizontalen und\nvertikalen Gestik mehrere\nBereiche erwerben sollst."), kMyLocalKey_tutorial1);
//	de->setObject(CCString::create("Die Kontrollfunktion läuft\nwie folgt ab."), kMyLocalKey_tutorial2);
//	de->setObject(CCString::create("Gestikuliere nach rechts,\num den Bereich zu erwerben!\nDie Gestik funktioniert überall\nauf dem Bildschirm."), kMyLocalKey_tutorial3);
//	de->setObject(CCString::create("Gestikuliere nach oben!"), kMyLocalKey_tutorial4);
//	de->setObject(CCString::create("Gestikuliere nach links!"), kMyLocalKey_tutorial5);
//	de->setObject(CCString::create("Gestikuliere nach unten!"), kMyLocalKey_tutorial6);
//	de->setObject(CCString::create("Erwerb des Bereiches abgeschlossen!\nGut gemacht! Berühre den Bildschrim,\num die Funktion abzubrechen\nwährend die Linie gezeichnet wird!"), kMyLocalKey_tutorial7);
//	de->setObject(CCString::create("Die Bewegung auf dem erhaltenen\nBereich funktioniert wie folgt."), kMyLocalKey_tutorial8);
//	de->setObject(CCString::create("Wenn du deine Finer auf die\nRichtung nach die Umrisslinie\ngestikulierst, bewegt sich die\nLinie entlang der Umrisslinie."), kMyLocalKey_tutorial9);
//	de->setObject(CCString::create("Gestikuliere nach links!"), kMyLocalKey_tutorial10);
//	de->setObject(CCString::create("Berühre einfach dem Bildschirm,\num zu stoppen während der Bewegung."), kMyLocalKey_tutorial11);
//	de->setObject(CCString::create("Berühre dem Bildschirm!"), kMyLocalKey_tutorial12);
//	de->setObject(CCString::create("Nun folgt die Erklärung über\ndie Interface-Funktion."), kMyLocalKey_tutorial14);
//	de->setObject(CCString::create("Es ist der Bildschrim, wo man\ndas Kapitel auswählen kann."), kMyLocalKey_tutorial15);
//	de->setObject(CCString::create("Der markierte Teil ist das Kapitel.\nEin Kapitel besteht aus 5 Stufen.\nWenn alle Stufen gecleart werden,\nkannst du auf das nächste\nKapitel gehen."), kMyLocalKey_tutorial16);
//	de->setObject(CCString::create("Unten kannst du im\nFacebook einloggen."), kMyLocalKey_tutorial17);
//	de->setObject(CCString::create("Beim Einloggen kannst du\ndie Note deiner Freunde sehen\nund Items austauschen."), kMyLocalKey_tutorial17-1);
//	de->setObject(CCString::create("Wenn du das Kapitel auswählst,\nerscheint derselbe Bildschrim wieder.\nHier kannst du Stufen, Merkmalen,\nItems auswählen."), kMyLocalKey_tutorial18);
//	de->setObject(CCString::create("Der markierte Teil ist die Stufe.\nDie geclearte Stufe kann\nnochmals gespielt werden,\nwenn du diese Stufe wieder auswählst."), kMyLocalKey_tutorial19);
//	de->setObject(CCString::create("Die Figuren und Merkmale der\nFeinden, die von dir derzeit\nausgewählt wurden, werden gezeigt."), kMyLocalKey_tutorial20);
//	de->setObject(CCString::create("Es ist das derzeitig\nausgewählte Merkmal."), kMyLocalKey_tutorial21);
//	de->setObject(CCString::create("Die Auswahl der Merkmale\nist möglich. Mit Gold kannst\ndu Merkmale kaufen oder\naktualisieren. Während des\nSpiels kannst du Gold erwerben."), kMyLocalKey_tutorial22);
//	de->setObject(CCString::create("Es ist das derzeitig\nausgewählte Item."), kMyLocalKey_tutorial23);
//	de->setObject(CCString::create("Die Auswahl der Items\nist möglich. Mit dem Gold\nkannst du Items kaufen."), kMyLocalKey_tutorial24);
//	de->setObject(CCString::create("Letztes geht es um die\nStarttaste. Der Pinsel ist\nbenötigt, um das Spiel zu beginnen."), kMyLocalKey_tutorial25);
//	de->setObject(CCString::create("Danke dir, dass du das\nTutorial erfolgreich nachvollgezogen hast.\nEnde das Tutorial und bewege\ndir auf die Funktion, wo man\ndas Kapitel einstellen kann."), kMyLocalKey_tutorial26);
//	
//	de->setObject(CCString::create("Ein spezielles Image tritt auf dem letzten Stufen der jeweiligen Kapitels auf."), kMyLocalKey_loading1);
//	de->setObject(CCString::create("Wenn du eine schmale Line zeichnen möchtest, gestikuliere in die entgegengesetzte Richtung während der Bewegung."), kMyLocalKey_loading2);
//	de->setObject(CCString::create("Wenn du ab dem Rand zeichnen möchtest, gestikuliere in dieselbe Richtug nochmals, wenn die Linie sich an den Rand nährt."), kMyLocalKey_loading3);
//	de->setObject(CCString::create("Beim Einloggen im Facebook kannst du die Note der Freunden sehen."), kMyLocalKey_loading4);
//	de->setObject(CCString::create("Ein Pinsel ist erhältich, wenn du deine Freunde einlädst oder mitspielst. Einladen oder Mitspielen ist für eine Person einmal pro Tag möglich."), kMyLocalKey_loading5);
//	de->setObject(CCString::create("Hier kannst du Pinsels, die deine Freunde gesendet haben, bestätigen."), kMyLocalKey_loading6);
//	de->setObject(CCString::create("Erleichterung der Angriffsbedingung: Mehrere Angriffe sind möglich beim Bereich-Erwerb."), kMyLocalKey_loading11);
//	de->setObject(CCString::create("Erweiterung des Startbereiches: Das Spiel auf dem größeren Bereich ist möglich."), kMyLocalKey_loading12);
//	de->setObject(CCString::create("Abwehr bei der unnormalen Bedingung : Abwehr gegen Boss / Abgewehrte Angriffe : Ohnmacht, Schlafen, Einfrieren, Chaos"), kMyLocalKey_loading13);
//	de->setObject(CCString::create("Erhöhung der Geschwindigkeit : Beim Itemerwerb verdoppelt sich die Bewegungsgeschwindigkeit für 5 Sekunden."), kMyLocalKey_loading14);
//	de->setObject(CCString::create("Schweigen : Beim Itemerwerb wird der Angriff vom Boss für 20 Sekunden verhindert."), kMyLocalKey_loading15);
//	de->setObject(CCString::create("Reduzierung der Geschwindigkeit : Beim Itemerwerb halbiert sich die Bewegungsgeschwindigkeit der Feinde."), kMyLocalKey_loading16);
//	de->setObject(CCString::create("Wenn du die Merkmalen auswählst, erscheinen Angriffskills entsprechend der ausgewählten Merkmale während des Spiels."), kMyLocalKey_loading17);
//	de->setObject(CCString::create("Merkmalen"), kMyLocalKey_loading19);
//	de->setObject(CCString::create("Wenn du dich jetzt bei Facebook einloggen, wirst du 5 zusätzliche BRUSH erhalten."), kMyLocalKey_loading20);
//	
//	de->setObject(CCString::create("Erleichterung der Angriffsbedingung"), kMyLocalKey_item1);
//	de->setObject(CCString::create("Erweiterung des Startbereiches"), kMyLocalKey_item2);
//	de->setObject(CCString::create("Abwehr bei der unnormalen Bedingung"), kMyLocalKey_item3);
//	de->setObject(CCString::create("Erhöhung der Geschwindigkeit"), kMyLocalKey_item4);
//	de->setObject(CCString::create("Schweigen"), kMyLocalKey_item5);
//	de->setObject(CCString::create("Reduzierung der Geschwindigkeit"), kMyLocalKey_item6);
//	
//	de->setObject(CCString::create("Es gibt keine ausgewählte Items."), kMyLocalKey_notSelectedItem);
//	de->setObject(CCString::create("Du hast schon die ausgewählte Items gekauft."), kMyLocalKey_boughtItem);
//	de->setObject(CCString::create("Die Arten der Items, die in einem Spiel verwendet werden können, sind höchstens 2."), kMyLocalKey_multiSelect);
//	de->setObject(CCString::create("Wegen der fehlenden Golds kannst du das Item nicht kaufen."), kMyLocalKey_notEnoughGold);
//	de->setObject(CCString::create("Möchtest du in den Einkaufladen gehen?"), kMyLocalKey_goShop);
//	de->setObject(CCString::create("Wegen der fehlenden Pinsels kannst du das Spiel nicht starten."), kMyLocalKey_notEnoughBrush);
//	de->setObject(CCString::create("Wegen der fehlenden Rubys kannst du das Item nicht kaufen."), kMyLocalKey_notEnoughRuby);
//	de->setObject(CCString::create("Möchtest du das Spiel aufhören?"), kMyLocalKey_exit);
//	de->setObject(CCString::create("Wenn du hier die Kundenrezension schreibst, wird der Pinsel aufgefüllt."), kMyLocalKey_review);
//	de->setObject(CCString::create("Du bist auf der Galerie registriert. Möchtest du in die Galerie gehen?"), kMyLocalKey_regiGallery);
//	de->setObject(CCString::create("Schwierige Kapitels wurden veröffentlicht."), kMyLocalKey_openHard);
//	de->setObject(CCString::create("Der Abwehrschild wird beseitigt, da die dämonische Atmoshähre herrscht."), kMyLocalKey_shieldCancel);
//	de->setObject(CCString::create("Nach der Ausführung des Spiels kannst du Belohnungspunkten bekommen."), kMyLocalKey_exeAppReward);
//	de->setObject(CCString::create("GIFTCODE kann innerhalb einer Stunde einmal verwendet werden."), kMyLocalKey_giftAHour);
//	de->setObject(CCString::create("GIFTCODE ist falsch."), kMyLocalKey_giftInvalid);
//	de->setObject(CCString::create("Dieser GIFTCODE ist schon verwendet."), kMyLocalKey_giftUsed);
//	de->setObject(CCString::create("GIFTCODE wird von LitQoo veröffentlicht."), kMyLocalKey_giftLitqoo);
//	de->setObject(CCString::create("Möchtest du das Spiel aufhören und auf die Hauptseite gehen?"), kMyLocalKey_goHome);
//	de->setObject(CCString::create("Wenn das Spiel mit 3 Sternen gecleart wird, wird das Bild in der Gallerie registriert."), kMyLocalKey_star3clear);
//	de->setObject(CCString::create("Item kann verwendet werden."), kMyLocalKey_itemEnable);
//	de->setObject(CCString::create("Das Spiel ist erst ermöglicht, wenn die Applikation aktuallisiert wird."), kMyLocalKey_needUpdate);
//	de->setObject(CCString::create("Die aktuelle Version wurde veröffentlicht. Möchtest du aktuallisieren?"), kMyLocalKey_newVersion);
//	de->setObject(CCString::create("Spiele jetzt das Game zusammen!"), kMyLocalKey_facebookInvite);
//	de->setObject(CCString::create("%s hat BRUSH gesendet."), kMyLocalKey_facebookWith);
//	
//	de->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	de->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	de->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	de->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	de->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	de->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	de->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
//	
//	de->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
//	de->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
//	de->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
//	de->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
//	de->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
//	setObject(de, "de");
//	
//	
//	CCDictionary* es = CCDictionary::create();
//	es->setObject(CCString::create("No se puede conectar al servidor."), kMyLocalKey_canNotConnectedServer);
//	es->setObject(CCString::create("Toca la pantalla."), kMyLocalKey_touchPlease);
//	es->setObject(CCString::create("En este juego debes ocupar\nla mayor zona posible\ndeslizándote a los 4 lados."), kMyLocalKey_tutorial1);
//	es->setObject(CCString::create("Te enseñaremos la forma de manejo."), kMyLocalKey_tutorial2);
//	es->setObject(CCString::create("Desliza hacia la derecha\npara obtener la zona. Puedes\nhacerlo desde todos los\npuntos de la pantalla"), kMyLocalKey_tutorial3);
//	es->setObject(CCString::create("Desliza hacia arriba."), kMyLocalKey_tutorial4);
//	es->setObject(CCString::create("Desliza hacia la izquierda."), kMyLocalKey_tutorial5);
//	es->setObject(CCString::create("Desliza hacia abajo"), kMyLocalKey_tutorial6);
//	es->setObject(CCString::create("Obtención de zona complete.\nMuy bien hecho. Para cancelar la\nlínea que estás dibujando, basta\ncon hacer un toque en la pantalla."), kMyLocalKey_tutorial7);
//	es->setObject(CCString::create("Ahora te enseñaremos a transladarte\nsobre la zona obtenida."), kMyLocalKey_tutorial8);
//	es->setObject(CCString::create("Al deslizarte hacia el contorno\nde la zona obtenida, podrás\nmoverte hacia esa dirección."), kMyLocalKey_tutorial9);
//	es->setObject(CCString::create("Deslízate hacia la izquierda."), kMyLocalKey_tutorial10);
//	es->setObject(CCString::create("Para detenerte haz un toque\nsuave en la pantalla."), kMyLocalKey_tutorial11);
//	es->setObject(CCString::create("Toca la pantalla."), kMyLocalKey_tutorial12);
//	es->setObject(CCString::create("Ahora explicaremos la interfaz."), kMyLocalKey_tutorial14);
//	es->setObject(CCString::create("Es la pantalla en donde\nse escoje el escenario."), kMyLocalKey_tutorial15);
//	es->setObject(CCString::create("La parte marcada es el escenario.\nEn cada capítulo hay 5 escenarios.\nUna vez que hayas despejado\ntodos los escenarios, podrás\npasar al próximo capítulo."), kMyLocalKey_tutorial16);
//	es->setObject(CCString::create("La siguiente es la pantalla\nque conecta al Facebook."), kMyLocalKey_tutorial17);
//	es->setObject(CCString::create("Al iniciar sesión puedes ver\nla puntuación de tus amigos\ne intercambiar items con ellos."), kMyLocalKey_tutorial17-1);
//	es->setObject(CCString::create("Al escoger un capítulo puedes\nver una pantalla como ésta.\nAquí puedes escoger el escenario,\nlos atributos y los items."), kMyLocalKey_tutorial18);
//	es->setObject(CCString::create("La parte marcada es el escenario.\nPuedes volver a jugar en uno\nque ya has despejado."), kMyLocalKey_tutorial19);
//	es->setObject(CCString::create("Muestra a los enemigos y sus\natributos del escenario escogido."), kMyLocalKey_tutorial20);
//	es->setObject(CCString::create("Es el atributo escogido actualmente."), kMyLocalKey_tutorial21);
//	es->setObject(CCString::create("Puedes escoger el atributo\no bien adquirirlo o mejorarlo\ncon Gold. Pueder obtener Gold\ndurante el juego."), kMyLocalKey_tutorial22);
//	es->setObject(CCString::create("Es el item escogido actualmente."), kMyLocalKey_tutorial23);
//	es->setObject(CCString::create("Puedes escoger el item y\nadquirirlo con Gold."), kMyLocalKey_tutorial24);
//	es->setObject(CCString::create("Por último, es el botón de\ninicio de juego. Para comenzar\na jugar necesitas de un pincel."), kMyLocalKey_tutorial25);
//	es->setObject(CCString::create("Gracias por realizar el\ntutorial. Finaliza el tutorial\ny pasa al ajuste de capítulos."), kMyLocalKey_tutorial26);
//	
//	es->setObject(CCString::create("En el escenario final de cada capítulo aparece una imagen especial."), kMyLocalKey_loading1);
//	es->setObject(CCString::create("Si deseas dibujar una zona angosta, deslízate hacia el lado opuesto durante el movimiento."), kMyLocalKey_loading2);
//	es->setObject(CCString::create("Para dibujar desde una esquina, deslízate una vez más cuando casi hayas llegado a ella."), kMyLocalKey_loading3);
//	es->setObject(CCString::create("Al iniciar sesión en Facebook puedes ver la puntuación de tus amigos."), kMyLocalKey_loading4);
//	es->setObject(CCString::create("Al enviar (invitar o juntos) a un amigo podrás obtener pinceles. Puedes enviar solo una vez por día a cada amigo."), kMyLocalKey_loading5);
//	es->setObject(CCString::create("Aquí puedes verificar los pinceles que te han enviado tus amigos."), kMyLocalKey_loading6);
//	es->setObject(CCString::create("Mitigación de ataque: realiza más ataques al obtener la zona."), kMyLocalKey_loading11);
//	es->setObject(CCString::create("Aumento de la zona de comienzo: Puedes comenzar el juego desde una zona más amplia."), kMyLocalKey_loading12);
//	es->setObject(CCString::create("Defensa de ataque: Te defiende el ataque del Boss. Ataque que puede defender: desmayo, sueño, congelado, confusión."), kMyLocalKey_loading13);
//	es->setObject(CCString::create("Aumento de velocidad: Al obtener el item, la velocidad de translado aumenta al doble durante 5 segundos."), kMyLocalKey_loading14);
//	es->setObject(CCString::create("Silencio: Al obtener el item, el Boss no podrá atacer durante 20 segundos."), kMyLocalKey_loading15);
//	es->setObject(CCString::create("Disminución de velocidad: Al obtener el item, disminuye la velocidad de los enemigos a la mitad durante 20 segundos."), kMyLocalKey_loading16);
//	es->setObject(CCString::create("Al escoger el atributo, aparecerá durante el juego la habilidad correspondiente."), kMyLocalKey_loading17);
//	es->setObject(CCString::create("atributos"), kMyLocalKey_loading19);
//	es->setObject(CCString::create("Si se conecta al Facebook ahora, le regalamos 5 BRUSH más."), kMyLocalKey_loading20);
//	
//	es->setObject(CCString::create("Mitigación de ataque"), kMyLocalKey_item1);
//	es->setObject(CCString::create("Aumento de la zona de comienzo"), kMyLocalKey_item2);
//	es->setObject(CCString::create("Defensa de ataque"), kMyLocalKey_item3);
//	es->setObject(CCString::create("Aumento de velocidad"), kMyLocalKey_item4);
//	es->setObject(CCString::create("Silencio"), kMyLocalKey_item5);
//	es->setObject(CCString::create("Disminución de velocidad"), kMyLocalKey_item6);
//	
//	es->setObject(CCString::create("No se ha escogido ningún item."), kMyLocalKey_notSelectedItem);
//	es->setObject(CCString::create("Ya has comprador el item escogido."), kMyLocalKey_boughtItem);
//	es->setObject(CCString::create("Se pueden utilizar un máximo de 2 items por juego."), kMyLocalKey_multiSelect);
//	es->setObject(CCString::create("No tienes Gold suficientes para la compra."), kMyLocalKey_notEnoughGold);
//	es->setObject(CCString::create("¿Deseas ir a la tienda?"), kMyLocalKey_goShop);
//	es->setObject(CCString::create("No tienes pinceles suficientes para jugar."), kMyLocalKey_notEnoughBrush);
//	es->setObject(CCString::create("No tienes Rubíes suficientes para la compra."), kMyLocalKey_notEnoughRuby);
//	es->setObject(CCString::create("¿Deseas finalizar el juego?"), kMyLocalKey_exit);
//	es->setObject(CCString::create("Si nos dejas una opinión te llenaremos los pinceles."), kMyLocalKey_review);
//	es->setObject(CCString::create("Registrado en la galería. ¿Deseas ir a la galería?"), kMyLocalKey_regiGallery);
//	es->setObject(CCString::create("Se han abierto los capítulos más difíciles."), kMyLocalKey_openHard);
//	es->setObject(CCString::create("Se deshace la capa de defensa por la energía maligna."), kMyLocalKey_shieldCancel);
//	es->setObject(CCString::create("Se puede obtener luego de ejecutar el juego."), kMyLocalKey_exeAppReward);
//	es->setObject(CCString::create("Se pude utilizar solo un GIFTCODE por hora."), kMyLocalKey_giftAHour);
//	es->setObject(CCString::create("GIFTCODE incorrecto."), kMyLocalKey_giftInvalid);
//	es->setObject(CCString::create("GIFTCODE ya utilizado."), kMyLocalKey_giftUsed);
//	es->setObject(CCString::create("El GIFTCODE se emite desde LitQoo."), kMyLocalKey_giftLitqoo);
//	es->setObject(CCString::create("¿Desea terminar el juego y trasladarse a la página principal?"), kMyLocalKey_goHome);
//	es->setObject(CCString::create("Al despejar con 3 estrellas, se registrará la imagen en la galleria."), kMyLocalKey_star3clear);
//	es->setObject(CCString::create("Puedes utilizar el item"), kMyLocalKey_itemEnable);
//	es->setObject(CCString::create("Se necesita actualizar el juego para ejecutarlo."), kMyLocalKey_needUpdate);
//	es->setObject(CCString::create("Existe una versión actualizada. ¿Desea actualizar?"), kMyLocalKey_newVersion);
//	es->setObject(CCString::create("¡Juguemos ya mismo!"), kMyLocalKey_facebookInvite);
//	es->setObject(CCString::create("%s le ha enviado BRUSH."), kMyLocalKey_facebookWith);
//	
//	es->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	es->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	es->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	es->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	es->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	es->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	es->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
//	
//	es->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
//	es->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
//	es->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
//	es->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
//	es->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
//	setObject(es, "es");
//	
//	
//	CCDictionary* fr = CCDictionary::create();
//	fr->setObject(CCString::create("La connexion au Serveur n’est pas possible."), kMyLocalKey_canNotConnectedServer);
//	fr->setObject(CCString::create("Touchez l’écran."), kMyLocalKey_touchPlease);
//	fr->setObject(CCString::create("Ce jeu consiste à obtenir\nbeaucoup de zones en faisant\nglisser le doigt dans tous les sens."), kMyLocalKey_tutorial1);
//	fr->setObject(CCString::create("Voilà, c’est l’information\nsur la méthode de contrôle."), kMyLocalKey_tutorial2);
//	fr->setObject(CCString::create("Faites glisser le doigt à\ndroite pour obtenir la zone.\nC’est possible à n’importe\nquel endroit de l’écran."), kMyLocalKey_tutorial3);
//	fr->setObject(CCString::create("Faites glisser le doigt vers le haut."), kMyLocalKey_tutorial4);
//	fr->setObject(CCString::create("Faites glisser le doigt à gauche."), kMyLocalKey_tutorial5);
//	fr->setObject(CCString::create("Faites glisser le doigt vers le bas."), kMyLocalKey_tutorial6);
//	fr->setObject(CCString::create("Fin d’obtention de la zone.\nBravo! Si vous voulez arrêter\nà dessiner la ligne, il suffit\nde toucher l’écran."), kMyLocalKey_tutorial7);
//	fr->setObject(CCString::create("Voilà, c’est l’information\nsur la méthode de déplacement\nsur la zone obtenue."), kMyLocalKey_tutorial8);
//	fr->setObject(CCString::create("Si vous faites glisser le\ndoigt vers le sens de la\nligne périphérique de la zone\nobtenue, elle se déplace dans\nla direction correspondante\nen suivant la ligne périphérique."), kMyLocalKey_tutorial9);
//	fr->setObject(CCString::create("Faites glisser le doigt à gauche."), kMyLocalKey_tutorial10);
//	fr->setObject(CCString::create("Si vous voulez vous arrêter\nen cours du mouvement, touchez\nlégèrement l’écran."), kMyLocalKey_tutorial11);
//	fr->setObject(CCString::create("Touchez l’écran."), kMyLocalKey_tutorial12);
//	fr->setObject(CCString::create("Voilà, c’est l’explication\nsur l’interface."), kMyLocalKey_tutorial14);
//	fr->setObject(CCString::create("C’est un écran à decider le chapître."), kMyLocalKey_tutorial15);
//	fr->setObject(CCString::create("Le point marqué , c’est le chapître.\nUn chapître comprend les 5 scènes.\nSi toutes les scènes sont terminées,\nil possible de passer pour\nle chapître suivant."), kMyLocalKey_tutorial16);
//	fr->setObject(CCString::create("En bas, il y a une partie\npour l’ouverture de\nsession sur Facebook."), kMyLocalKey_tutorial17);
//	fr->setObject(CCString::create("Suite à l’ouverture de\nsession, il est possible de\nregarder les notes des amis\net d’échanger les items avec eux."), kMyLocalKey_tutorial17-1);
//	fr->setObject(CCString::create("Si vous sélectionnez le\nchapître, il est possible de\nregarder le même écran comme\nmaintenant. Ici, il est possible\nde sélectionner la scène, les\ncaractéristiques et les items."), kMyLocalKey_tutorial18);
//	fr->setObject(CCString::create("Le point marquée, c’est la\nscène. Il est possible de\nsélectionner de nouveau et\nde jouer à la scène déja passée."), kMyLocalKey_tutorial19);
//	fr->setObject(CCString::create("Cela montre les images et\nles caractéristiques des\nennemis de la scène\nsélectionnée pour le moment."), kMyLocalKey_tutorial20);
//	fr->setObject(CCString::create("Ce sont les caractéristiques\nsélectionnées pour le moment."), kMyLocalKey_tutorial21);
//	fr->setObject(CCString::create("Il est possible de sélectionner\nles caractéristiques et de\nles acheter et de mettre à\nniveau avec l’or. Il est\npossible d’obtenir l’or\nen cours du jeu."), kMyLocalKey_tutorial22);
//	fr->setObject(CCString::create("Ce sont les items sélectionnés\npour le moment."), kMyLocalKey_tutorial23);
//	fr->setObject(CCString::create("Il est possible de sélectionner\nles items et de les acheter avec l’or."), kMyLocalKey_tutorial24);
//	fr->setObject(CCString::create("C’est le bouton pour démarrer\nle jeu pour la dernière fois.\nLe pinceau est nécessaire\nà commencer le jeu."), kMyLocalKey_tutorial25);
//	fr->setObject(CCString::create("Vous avez bien réalisé le\nTutoriel. Le Tutoriel est\nterminé et déplacez sur le lieu\nà sélectionner le chapître."), kMyLocalKey_tutorial26);
//	
//	fr->setObject(CCString::create("Un image spécial sera apparu sur la dernière scène de chaque chapître."), kMyLocalKey_loading1);
//	fr->setObject(CCString::create("Si vous voulez dessiner de manière plus fine, faites glisser le doigt vers le sens opposé."), kMyLocalKey_loading2);
//	fr->setObject(CCString::create("Pour dessiner à partir d’un angle, faites glisser encore le doigt vers le sens correspondant auprès de l’angle."), kMyLocalKey_loading3);
//	fr->setObject(CCString::create("Suite à l’ouverture de session sur Facebook, il est possible de regarder les notes des amis."), kMyLocalKey_loading4);
//	fr->setObject(CCString::create("Si vous envoyez le message (Invitation or Ensemble), vous pouvez obtenir les pinceaux. Il est possible d’envoyer le message à une personne une fois par jour."), kMyLocalKey_loading5);
//	fr->setObject(CCString::create("Il est possible de vérifier les pinceaux envoyés par les amis."), kMyLocalKey_loading6);
//	fr->setObject(CCString::create("Adoucissement des conditions d’attaque; Il est possible de faire plus beaucoup d’attaques lors de l’obtention de la zone."), kMyLocalKey_loading11);
//	fr->setObject(CCString::create("Elargissement de la zone de démarrage; Il est possible de démarrer le jeu dans la zone plus large."), kMyLocalKey_loading12);
//	fr->setObject(CCString::create("Défense de la situation anormale; cela permet de défendre contre l’attaque de boss. Attaques à défendre; évanouissement, sommeil, congélation, désordre."), kMyLocalKey_loading13);
//	fr->setObject(CCString::create("Accélération; La vitesse sera accélérée deux fois plus rapide pendant 5 secondes après l’obtention d’un item."), kMyLocalKey_loading14);
//	fr->setObject(CCString::create("Silence; Le Boss ne peut pas faire une attaque pendant 20 secondes après l’obtention d’un item."), kMyLocalKey_loading15);
//	fr->setObject(CCString::create("Décélération;  La vitesse de déplacement des ennemis sera réduite à moitié pendant 20 secondes après l’obtention d’un item."), kMyLocalKey_loading16);
//	fr->setObject(CCString::create("Si vous sélectionnez une caractéristique, une technique d’attaque correspondante sera apparue en cours du jeu."), kMyLocalKey_loading17);
//	fr->setObject(CCString::create("caractéristiques"), kMyLocalKey_loading19);
//	fr->setObject(CCString::create("Si vous ouvrez maintenant la session sur Facebook , vous aurez encore 5 BRUSH."), kMyLocalKey_loading20);
//	
//	fr->setObject(CCString::create("Adoucissement des conditions d’attaque"), kMyLocalKey_item1);
//	fr->setObject(CCString::create("Elargissement de la zone de démarrage"), kMyLocalKey_item2);
//	fr->setObject(CCString::create("Défense de la situation anormale"), kMyLocalKey_item3);
//	fr->setObject(CCString::create("Accélération"), kMyLocalKey_item4);
//	fr->setObject(CCString::create("Silence"), kMyLocalKey_item5);
//	fr->setObject(CCString::create("Décélération"), kMyLocalKey_item6);
//	
//	fr->setObject(CCString::create("Vous n’avez pas sélectionné les items."), kMyLocalKey_notSelectedItem);
//	fr->setObject(CCString::create("Vous avez déjà acheté les items sélectionnés."), kMyLocalKey_boughtItem);
//	fr->setObject(CCString::create("Il est possible d’utiliser deux types d’item au maximum pour un seul jeu."), kMyLocalKey_multiSelect);
//	fr->setObject(CCString::create("L’achat n’est pas possible d’un manque d’or"), kMyLocalKey_notEnoughGold);
//	fr->setObject(CCString::create("Voulez-vous entrer dans le magasin?"), kMyLocalKey_goShop);
//	fr->setObject(CCString::create("S’il n’y a pas assez de pinceau, il ne sera pas possible de démarrer le jeu."), kMyLocalKey_notEnoughBrush);
//	fr->setObject(CCString::create("L’achat n’est pas possible d’un manque de rubis"), kMyLocalKey_notEnoughRuby);
//	fr->setObject(CCString::create("Voulez-vous terminer le jeu?"), kMyLocalKey_exit);
//	fr->setObject(CCString::create("Si vous laissez quelques mots sur la revue, vous aurez plein de pinceaux."), kMyLocalKey_review);
//	fr->setObject(CCString::create("Vous êtes inscrit à la galerie. Voulez-vous visiter la galerie?"), kMyLocalKey_regiGallery);
//	fr->setObject(CCString::create("Les chapîtres difficiles sont sortis."), kMyLocalKey_openHard);
//	fr->setObject(CCString::create("La clôture de protection est dégagée par beaucoup de tendance malheureuse."), kMyLocalKey_shieldCancel);
//	fr->setObject(CCString::create("La récompense sera donnée après le démarrage du jeu."), kMyLocalKey_exeAppReward);
//	fr->setObject(CCString::create("Possible d’utiliser un seul GIFTCODE pendant une heure."), kMyLocalKey_giftAHour);
//	fr->setObject(CCString::create("Ce GIFTCODE n’est pas correct."), kMyLocalKey_giftInvalid);
//	fr->setObject(CCString::create("Ce GIFTCODE a été déjà utilisé."), kMyLocalKey_giftUsed);
//	fr->setObject(CCString::create("GIFTCODE est diffusé par LitQoo"), kMyLocalKey_giftLitqoo);
//	fr->setObject(CCString::create("Voulez- vous terminer le jeu et retourner à la page d’accueil?"), kMyLocalKey_goHome);
//	fr->setObject(CCString::create("Si il est possible de passer au niveau supérieur, la peinture sera inscrite dans la galerie."), kMyLocalKey_star3clear);
//	fr->setObject(CCString::create("Il est possible d’utiliser l’item"), kMyLocalKey_itemEnable);
//	fr->setObject(CCString::create("Possible de faire du jeu suite au téléchargement de la nouvelle version."), kMyLocalKey_needUpdate);
//	fr->setObject(CCString::create("Une nouvelle version est sortie. Voulez- vous la télécharger?"), kMyLocalKey_newVersion);
//	fr->setObject(CCString::create("Faisons- le ensemble!"), kMyLocalKey_facebookInvite);
//	fr->setObject(CCString::create("%s a envoyé BRUSH."), kMyLocalKey_facebookWith);
//	
//	fr->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	fr->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	fr->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	fr->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	fr->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	fr->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	fr->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
//	
//	fr->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
//	fr->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
//	fr->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
//	fr->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
//	fr->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
//	setObject(fr, "fr");
//	
//	
//	CCDictionary* zh = CCDictionary::create();
//	zh->setObject(CCString::create("无法连接服务器。"), kMyLocalKey_canNotConnectedServer);
//	zh->setObject(CCString::create("请触摸画面。"), kMyLocalKey_touchPlease);
//	zh->setObject(CCString::create("利用上下左右滑动要多获得领域的游戏。"), kMyLocalKey_tutorial1);
//	zh->setObject(CCString::create("对控制方法的说明。"), kMyLocalKey_tutorial2);
//	zh->setObject(CCString::create("为获得领域请向右滑动。\n在画面所有的角落都可以做到。"), kMyLocalKey_tutorial3);
//	zh->setObject(CCString::create("向上滑动。"), kMyLocalKey_tutorial4);
//	zh->setObject(CCString::create("向左滑动。"), kMyLocalKey_tutorial5);
//	zh->setObject(CCString::create("向下滑动。"), kMyLocalKey_tutorial6);
//	zh->setObject(CCString::create("领域获得结束。 做的很好。\n在画线的过程中想取消就请触摸画面。"), kMyLocalKey_tutorial7);
//	zh->setObject(CCString::create("这一次，说明对获得的领域向上移动的方法。"), kMyLocalKey_tutorial8);
//	zh->setObject(CCString::create("以获得领域的外轮廓方向滑动，\n在相应的方向会根据外轮廓移动。"), kMyLocalKey_tutorial9);
//	zh->setObject(CCString::create("请向左滑动。"), kMyLocalKey_tutorial10);
//	zh->setObject(CCString::create("移动中想停止就轻轻地触摸。"), kMyLocalKey_tutorial11);
//	zh->setObject(CCString::create("请触摸画面。"), kMyLocalKey_tutorial12);
//	zh->setObject(CCString::create("现在对接口说明一下。"), kMyLocalKey_tutorial14);
//	zh->setObject(CCString::create("决定篇章的画面。"), kMyLocalKey_tutorial15);
//	zh->setObject(CCString::create("表示的部分就是篇章。每个篇章里有5个阶段。\n阶段都过了关就会进入下个篇章。"), kMyLocalKey_tutorial16);
//	zh->setObject(CCString::create("下面是对脸谱注册的部分。"), kMyLocalKey_tutorial17);
//	zh->setObject(CCString::create("注册会看到朋友们的分数，\n可以和朋友传接商品。"), kMyLocalKey_tutorial17-1);
//	zh->setObject(CCString::create("选择篇章就会看到现在的画面。\n在这里可以选择阶段和属性，商品。"), kMyLocalKey_tutorial18);
//	zh->setObject(CCString::create("表示的部分就是阶段。\n已过的阶段也可以重选玩。"), kMyLocalKey_tutorial19);
//	zh->setObject(CCString::create("显现当前被选阶段的对抗者模样和属性。"), kMyLocalKey_tutorial20);
//	zh->setObject(CCString::create("当前被选的属性。"), kMyLocalKey_tutorial21);
//	zh->setObject(CCString::create("可以选择属性，以黄金购买及升级。\n黄金可在游戏中获得。"), kMyLocalKey_tutorial22);
//	zh->setObject(CCString::create("是当前被选的商品。"), kMyLocalKey_tutorial23);
//	zh->setObject(CCString::create("可以选择商品也可用黄金购买。"), kMyLocalKey_tutorial24);
//	zh->setObject(CCString::create("最后是游戏开始按钮。\n为开始游戏需要有画笔。"), kMyLocalKey_tutorial25);
//	zh->setObject(CCString::create("为进行教程辛苦啦。\n结束教程向设定篇章方向移动。"), kMyLocalKey_tutorial26);
//	
//	zh->setObject(CCString::create("个篇章的最后阶段出现特别的形象。"), kMyLocalKey_loading1);
//	zh->setObject(CCString::create("想画的薄一些就请在移动中相反方向滑动。"), kMyLocalKey_loading2);
//	zh->setObject(CCString::create("想从角落开始画起，那就要在靠近角落时在相应的方向再次滑动。"), kMyLocalKey_loading3);
//	zh->setObject(CCString::create("注册脸谱可看到朋友们的分数。"), kMyLocalKey_loading4);
//	zh->setObject(CCString::create("向朋友发送（邀请 or 一起）可获得画笔。可以向每个人一天发送一次。"), kMyLocalKey_loading5);
//	zh->setObject(CCString::create("朋友发送过来的画笔可以在这里确认。"), kMyLocalKey_loading6);
//	zh->setObject(CCString::create("攻击条件放宽：获得领域时攻击更多。"), kMyLocalKey_loading11);
//	zh->setObject(CCString::create("开始领域增加：在更宽的领域开始游戏。"), kMyLocalKey_loading12);
//	zh->setObject(CCString::create("状态异常防御：防御头领的攻击。防御起来的攻击：气绝，睡眠，冷冻，发昏"), kMyLocalKey_loading13);
//	zh->setObject(CCString::create("速度增加：获得商品时移动速度5秒钟增加两倍快速。"), kMyLocalKey_loading14);
//	zh->setObject(CCString::create("沉默：获得商品时20秒钟不让头领攻击。"), kMyLocalKey_loading15);
//	zh->setObject(CCString::create("减速：获得商品时20秒钟将对抗者的移动速度减半。"), kMyLocalKey_loading16);
//	zh->setObject(CCString::create("选择属性，游戏中就会显现符合该属性的攻击技术。"), kMyLocalKey_loading17);
//	zh->setObject(CCString::create("属性"), kMyLocalKey_loading19);
//	zh->setObject(CCString::create("现在登录脸谱网的话多送5个BRUSH"), kMyLocalKey_loading20);
//	
//	zh->setObject(CCString::create("攻击条件放宽"), kMyLocalKey_item1);
//	zh->setObject(CCString::create("开始领域增加"), kMyLocalKey_item2);
//	zh->setObject(CCString::create("状态异常防御"), kMyLocalKey_item3);
//	zh->setObject(CCString::create("速度增加"), kMyLocalKey_item4);
//	zh->setObject(CCString::create("沉默"), kMyLocalKey_item5);
//	zh->setObject(CCString::create("减速"), kMyLocalKey_item6);
//	
//	zh->setObject(CCString::create("没有被选商品。"), kMyLocalKey_notSelectedItem);
//	zh->setObject(CCString::create("购买了已被选择的商品。"), kMyLocalKey_boughtItem);
//	zh->setObject(CCString::create("一次游戏中可使用的商品种类为最多2种。"), kMyLocalKey_multiSelect);
//	zh->setObject(CCString::create("缺少黄金无法购买。"), kMyLocalKey_notEnoughGold);
//	zh->setObject(CCString::create("去商店吗？"), kMyLocalKey_goShop);
//	zh->setObject(CCString::create("缺少画笔无法开始游戏。"), kMyLocalKey_notEnoughBrush);
//	zh->setObject(CCString::create("缺少红宝石无法购买。"), kMyLocalKey_notEnoughRuby);
//	zh->setObject(CCString::create("结束游戏吗？"), kMyLocalKey_exit);
//	zh->setObject(CCString::create("留下评论会填满画笔。"), kMyLocalKey_review);
//	zh->setObject(CCString::create("登录画廊。去画廊吗？"), kMyLocalKey_regiGallery);
//	zh->setObject(CCString::create("公开了高难篇章。"), kMyLocalKey_openHard);
//	zh->setObject(CCString::create("充满了恶意防御盾解除。"), kMyLocalKey_shieldCancel);
//	zh->setObject(CCString::create("执行游戏后可得到赔偿金"), kMyLocalKey_exeAppReward);
//	zh->setObject(CCString::create("GIFTCODE 1小时只能使用一次"), kMyLocalKey_giftAHour);
//	zh->setObject(CCString::create("是不正确的 GIFTCODE"), kMyLocalKey_giftInvalid);
//	zh->setObject(CCString::create("是以使用过的 GIFTCODE"), kMyLocalKey_giftUsed);
//	zh->setObject(CCString::create("GIFTCODE发生在LitQoo。"), kMyLocalKey_giftLitqoo);
//	zh->setObject(CCString::create("结束游戏转移到首页吗？"), kMyLocalKey_goHome);
//	zh->setObject(CCString::create("用3个星清除的话画廊里会登录图片"), kMyLocalKey_star3clear);
//	zh->setObject(CCString::create("可以使用商品"), kMyLocalKey_itemEnable);
//	zh->setObject(CCString::create("升级到最新版本才可能玩"), kMyLocalKey_needUpdate);
//	zh->setObject(CCString::create("推出最新版本。升级吗？"), kMyLocalKey_newVersion);
//	zh->setObject(CCString::create("现在一起来吧！"), kMyLocalKey_facebookInvite);
//	zh->setObject(CCString::create("%s 送来 BRUSH"), kMyLocalKey_facebookWith);
//	
//	zh->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	zh->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	zh->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	zh->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	zh->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	zh->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	zh->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
//	
//	zh->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
//	zh->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
//	zh->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
//	zh->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
//	zh->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
//	setObject(zh, "zh");
//	
//	
//	CCDictionary* it = CCDictionary::create();
//	it->setObject(CCString::create("Impossibile connettersi al server."), kMyLocalKey_canNotConnectedServer);
//	it->setObject(CCString::create("Tocca il display."), kMyLocalKey_touchPlease);
//	it->setObject(CCString::create("E’ un gioco con cui dovrai\nconquistare più territori con\nmovimenti su e  giù/destra\ne sinistra."), kMyLocalKey_tutorial1);
//	it->setObject(CCString::create("Ora ti spieghiamo il controllo."), kMyLocalKey_tutorial2);
//	it->setObject(CCString::create("Per conquistare il territorio\nmuoviti verso destra. Puoi\nfarlo in tutte le parti del display."), kMyLocalKey_tutorial3);
//	it->setObject(CCString::create("Muoviti in su."), kMyLocalKey_tutorial4);
//	it->setObject(CCString::create("Muoviti verso sinistra."), kMyLocalKey_tutorial5);
//	it->setObject(CCString::create("Muoviti in giù."), kMyLocalKey_tutorial6);
//	it->setObject(CCString::create("Completatala conquista del\nterritorio. Hai fatto un buon\nlavoro. Per cancellare\nl’operazione mentre tracci la\nlinea, basta toccare il display."), kMyLocalKey_tutorial7);
//	it->setObject(CCString::create("Ora ti spieghiamo come spostarsi\nsopra il territorio conquistato."), kMyLocalKey_tutorial8);
//	it->setObject(CCString::create("Muovendoti verso il contorno\ndel territorio conquistato, puoi\nspostarti lungo il contorno\nin quella direzione."), kMyLocalKey_tutorial9);
//	it->setObject(CCString::create("Prova a muoverti verso sinistra."), kMyLocalKey_tutorial10);
//	it->setObject(CCString::create("Per fermarti durante lo\nspostamento, basta toccare\nleggermente il display."), kMyLocalKey_tutorial11);
//	it->setObject(CCString::create("Prova a toccare il display."), kMyLocalKey_tutorial12);
//	it->setObject(CCString::create("Ora ti spieghiamo l’interfaccia."), kMyLocalKey_tutorial14);
//	it->setObject(CCString::create("E’ il display che determina\nil capitolo."), kMyLocalKey_tutorial15);
//	it->setObject(CCString::create("La parte segnata è un capitolo.\nIn un capitolo ci sono 5 fasi.\nAzzerando tutte le fasi, puoi\npassare al prossimo capitolo."), kMyLocalKey_tutorial16);
//	it->setObject(CCString::create("Sotto trovi l’Area Accesso a Facebook."), kMyLocalKey_tutorial17);
//	it->setObject(CCString::create("Accedendo a Facebook, puoi\nvedere i punteggi degli amici\ne scambiare oggetti con loro."), kMyLocalKey_tutorial17-1);
//	it->setObject(CCString::create("Selezionando un capitolo,\npuoi vedere il display come\nquesto. Qui puoi selezionare\nla fase, la caratteristica\ne l’oggetto."), kMyLocalKey_tutorial18);
//	it->setObject(CCString::create("La parte segnata è la fase.\nAnche la fase azzerata può\nessere giocata di\nnuovo selezionandola."), kMyLocalKey_tutorial19);
//	it->setObject(CCString::create("Vengono visualizzati gli\naspetti e le caratteristiche\ndella fase attualmente selezionata."), kMyLocalKey_tutorial20);
//	it->setObject(CCString::create("Sono le caratteristiche selezionate."), kMyLocalKey_tutorial21);
//	it->setObject(CCString::create("Puoi selezionare, acquistare\nle caratteristiche con Oro e\nmigliorarle. Puoi ottenere\nOro durante il gioco."), kMyLocalKey_tutorial22);
//	it->setObject(CCString::create("E’ l’oggetto attualmente selezionato."), kMyLocalKey_tutorial23);
//	it->setObject(CCString::create("Puoi selezionare e\nacquistare l’oggetto con Oro."), kMyLocalKey_tutorial24);
//	it->setObject(CCString::create("E’ il tasto per iniziare\nil gioco per l’ultima volta.\nPer iniziarlo hai bisogno\ndi un pennello."), kMyLocalKey_tutorial25);
//	it->setObject(CCString::create("Grazie per aver appreso il\ntutorial. Ora terminiamo il\ntutorial e spostiamoci nell’Area\nImpostazione Capitolo."), kMyLocalKey_tutorial26);
//	
//	it->setObject(CCString::create("Nella fase finale di ogni capitolo si visualizza un’immagine speciale."), kMyLocalKey_loading1);
//	it->setObject(CCString::create("Se vuoi tracciare la linea sottile, muoviti nella direzione opposta durante lo spostamento."), kMyLocalKey_loading2);
//	it->setObject(CCString::create("Se vuoi tracciare a cominciare da unangolo, quando ti avvicini all’angolo muoviti un’altra volta in quella direzione."), kMyLocalKey_loading3);
//	it->setObject(CCString::create("Accedendo a Facebook, puoi vedere i punteggi degli amici."), kMyLocalKey_loading4);
//	it->setObject(CCString::create("Se inviti o condividi il gioco con amici puoi ricevere un pennello. Puoi inviare la richiesta a un amico una volta al giorno."), kMyLocalKey_loading5);
//	it->setObject(CCString::create("Puoi visualizzare qui i pennelli ricevuti dagli amici."), kMyLocalKey_loading6);
//	it->setObject(CCString::create("Facilitazione della condizione d’attacco: Puoi attaccare di più quando conquisti il territorio."), kMyLocalKey_loading11);
//	it->setObject(CCString::create("Aumento dell’Area Iniziale: Puoi iniziare il gioco in un’area più ampia."), kMyLocalKey_loading12);
//	it->setObject(CCString::create("Difesa allo stato anormale: Puoi difenderti dagli attacchi del capo. Attacchi da difendere: svenimento, sonno, congelamento, confusione"), kMyLocalKey_loading13);
//	it->setObject(CCString::create("Aumento della velocità: Quando ottieni l’oggetto, la velocità dello spostamento si raddoppia per 5 secondi."), kMyLocalKey_loading14);
//	it->setObject(CCString::create("Silenzio: Quando ottieni l’oggetto, il capo non ti può attaccare per 20 secondi."), kMyLocalKey_loading15);
//	it->setObject(CCString::create("Riduzione della velocità: Quando ottieni l’oggetto, la velocità dello spostamento dei nemici si dimezza per 20 secondi."), kMyLocalKey_loading16);
//	it->setObject(CCString::create("Quando selezioni la caratteristica, il trucco d’attacco si visualizza durante il gioco."), kMyLocalKey_loading17);
//	it->setObject(CCString::create("caratteristica"), kMyLocalKey_loading19);
//	it->setObject(CCString::create("Se accedi adesso a Facebook, ti regaliamo altri 5 BRUSH."), kMyLocalKey_loading20);
//	
//	
//	it->setObject(CCString::create("Facilitazione della condizione d’attacco"), kMyLocalKey_item1);
//	it->setObject(CCString::create("Aumento dell’Area Iniziale"), kMyLocalKey_item2);
//	it->setObject(CCString::create("Difesa allo stato anormale"), kMyLocalKey_item3);
//	it->setObject(CCString::create("Aumento della velocità"), kMyLocalKey_item4);
//	it->setObject(CCString::create("Silenzio"), kMyLocalKey_item5);
//	it->setObject(CCString::create("Riduzione della velocità"), kMyLocalKey_item6);
//	
//	it->setObject(CCString::create("Nessun oggetto selezionato."), kMyLocalKey_notSelectedItem);
//	it->setObject(CCString::create("Hai già acquistato l’oggetto selezionato."), kMyLocalKey_boughtItem);
//	it->setObject(CCString::create("Puoi usare max. 2 tipi di oggetti per un gioco."), kMyLocalKey_multiSelect);
//	it->setObject(CCString::create("Oro insufficiente per fare un acquisto."), kMyLocalKey_notEnoughGold);
//	it->setObject(CCString::create("Vuoiandare al negozio?"), kMyLocalKey_goShop);
//	it->setObject(CCString::create("Pennello insufficiente per iniziare il gioco."), kMyLocalKey_notEnoughBrush);
//	it->setObject(CCString::create("Rubino insufficiente per fare un acquisto."), kMyLocalKey_notEnoughRuby);
//	it->setObject(CCString::create("Vuoiterminareilgioco?"), kMyLocalKey_exit);
//	it->setObject(CCString::create("Se lasci i commenti, ti riempiamo di pennelli."), kMyLocalKey_review);
//	it->setObject(CCString::create("E’ stato registrato alla galleria. Vuoiandarealla galleria?"), kMyLocalKey_regiGallery);
//	it->setObject(CCString::create("Si sono rivelati i capitoli difficili."), kMyLocalKey_openHard);
//	it->setObject(CCString::create("La forza del male disattiva la protezione."), kMyLocalKey_shieldCancel);
//	it->setObject(CCString::create("Puoi prendere la compensazione dopo aver eseguito il gioco. "), kMyLocalKey_exeAppReward);
//	it->setObject(CCString::create("GIFTCODE può essere utilizzato entro un’ora."), kMyLocalKey_giftAHour);
//	it->setObject(CCString::create("GIFTCODE non corretto."), kMyLocalKey_giftInvalid);
//	it->setObject(CCString::create("GIFTCODE già utilizzato."), kMyLocalKey_giftUsed);
//	it->setObject(CCString::create("GIFTCODE viene rilasciato a LitQoo."), kMyLocalKey_giftLitqoo);
//	it->setObject(CCString::create("Vuoi terminare il gioco e spostarti sulla pagina principale?"), kMyLocalKey_goHome);
//	it->setObject(CCString::create("Azzerando con tre stelle, l’immagine verrà registrata nella galleria."), kMyLocalKey_star3clear);
//	it->setObject(CCString::create("Puoi utilizzare l’oggetto"), kMyLocalKey_itemEnable);
//	it->setObject(CCString::create("E’ possibile giocare solo con la versione aggiornata."), kMyLocalKey_needUpdate);
//	it->setObject(CCString::create("E’ uscita l’ultima versione. Vuoi aggiornarla?"), kMyLocalKey_newVersion);
//	it->setObject(CCString::create("Divertiamoci insieme adesso!"), kMyLocalKey_facebookInvite);
//	it->setObject(CCString::create("%s ha inviato BRUSH."), kMyLocalKey_facebookWith);
//	
//	it->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	it->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	it->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	it->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	it->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	it->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	it->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
//	
//	it->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
//	it->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
//	it->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
//	it->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
//	it->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
//	setObject(it, "it");
}

CCString* MyLocal::getLocalCode()
{
	string tempCode;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
	NSString *currentLanguage = [languages objectAtIndex:0];
	
	// get the current language code.(such as English is "en", Chinese is "zh" and so on)
	NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
	NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
	
	tempCode = [languageCode cStringUsingEncoding:NSASCIIStringEncoding];
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	tempCode = getCurrentLanguageJNI();
#endif
	
	return CCString::create(tempCode);
}

const char* MyLocal::getSupportLocalCode()
{
	languageType = getLocalCode();
	CCArray* allkeys_ = this->allKeys();
	bool supportable = false;
	for(int i=0;i<allkeys_->count();i++)
	{
		CCString* key = (CCString*)allkeys_->objectAtIndex(i);
		if(languageType->isEqual(key))
		{
			supportable = true;
			break;
		}
	}
	if(!supportable)
	{
		languageType = CCString::create("en");
	}
	
	return languageType->getCString();
}

const char* MyLocal::getLocalForKey( MyLocalKey key )
{
	languageType = getLocalCode();
	CCArray* allkeys_ = this->allKeys();
	bool supportable = false;
	for(int i=0;i<allkeys_->count();i++)
	{
		CCString* key = (CCString*)allkeys_->objectAtIndex(i);
		if(languageType->isEqual(key))
		{
			supportable = true;
			break;
		}
	}
	if(!supportable)
	{
		languageType = CCString::create("en");
	}
	CCDictionary* localDic = (CCDictionary*)this->objectForKey(languageType->getCString());
	CCString* returnLocal = (CCString*)localDic->objectForKey(key);

	if(returnLocal == NULL)
		returnLocal = (CCString*)((CCDictionary*)objectForKey("en"))->objectForKey(key);
	if(returnLocal == NULL)
		returnLocal = CCString::create("");
	
	return returnLocal->getCString();
}

MyLocal* MyLocal::sharedInstance()
{
	static MyLocal* myLocal = NULL;
	if(myLocal == NULL)
	{
		myLocal = new MyLocal();
	}
	return myLocal;
}

void MyLocal::setLocal()
{
	languageType = getLocalCode();
}
void MyLocal::ko()
{
	
	CCDictionary* ko = CCDictionary::create();
	ko->setObject(CCString::create("게임을 시작 중 입니다..."), kMyLocalKey_connectingServer);
	ko->setObject(CCString::create("닉네임 입력"), kMyLocalKey_inputNick);
	ko->setObject(CCString::create(" 닉네임을 입력해주세요."), kMyLocalKey_inputPlease);
	ko->setObject(CCString::create("확인"), kMyLocalKey_ok);
	ko->setObject(CCString::create("이미지 정보를 받아옵니다."), kMyLocalKey_downImgInfo);
	ko->setObject(CCString::create("정보 불러오기 실패"), kMyLocalKey_failLoadInfo);
	ko->setObject(CCString::create("재시도"), kMyLocalKey_replay);
	ko->setObject(CCString::create("다운로드중 %.0f%% : %d/%d"), kMyLocalKey_downloadingProgress);
	ko->setObject(CCString::create("이미지 받아오기 실패"), kMyLocalKey_downImgFail);
	ko->setObject(CCString::create("로그인 성공"), kMyLocalKey_successLogin);
	ko->setObject(CCString::create("사용할 수 없는 닉네임 입니다."), kMyLocalKey_sameNick);
	ko->setObject(CCString::create("사용할 수 없는 닉네임 입니다."), kMyLocalKey_invalidNick);
	ko->setObject(CCString::create("닉네임이 너무 깁니다."), kMyLocalKey_longNick);
	ko->setObject(CCString::create("닉네임이 너무 짧습니다."), kMyLocalKey_shortNick);
	ko->setObject(CCString::create("특수문자는 사용 할 수 없습니다."), kMyLocalKey_specialCharacterError);
	ko->setObject(CCString::create("지금 열기"), kMyLocalKey_openRightNow);
	ko->setObject(CCString::create("RUBY %d 로 오픈"), kMyLocalKey_openRubyValue);
	ko->setObject(CCString::create("GOLD %d 로 오픈"), kMyLocalKey_openGoldValue);
	ko->setObject(CCString::create("CGP 일반 보상"), kMyLocalKey_cgpNormalTitle);
	ko->setObject(CCString::create("보상받기를 눌러주세요."), kMyLocalKey_cgpNormalContent);
	ko->setObject(CCString::create("보상받기"), kMyLocalKey_getReward);
	ko->setObject(CCString::create("받기"), kMyLocalKey_take);
	ko->setObject(CCString::create("CGP 전체 팝업 보상"), kMyLocalKey_cgpAllPopupTitle);
	ko->setObject(CCString::create("상점"), kMyLocalKey_shop);
	ko->setObject(CCString::create("랭킹"), kMyLocalKey_ranking);
	ko->setObject(CCString::create("내카드"), kMyLocalKey_mycard);
	ko->setObject(CCString::create("오늘의 미션"), kMyLocalKey_todaymission);
	ko->setObject(CCString::create("이벤트"), kMyLocalKey_event);
	ko->setObject(CCString::create("스페셜 스테이지"), kMyLocalKey_openStage);
	ko->setObject(CCString::create("스페셜 스테이지를 오픈하시겠습니까?"), kMyLocalKey_openStageContent);
	ko->setObject(CCString::create("오픈하기"), kMyLocalKey_doOpen);
	ko->setObject(CCString::create("스테이지 오픈"), kMyLocalKey_stageOpenTitle);
	ko->setObject(CCString::create("새로운 스테이지가\n오픈 되었습니다."), kMyLocalKey_stageOpenContent);
	ko->setObject(CCString::create("퍼즐 오픈"), kMyLocalKey_puzzleOpenTitle);
	ko->setObject(CCString::create("새로운 퍼즐이\n오픈 되었습니다."), kMyLocalKey_puzzleOpenContent);
	ko->setObject(CCString::create("보기"), kMyLocalKey_view);
	ko->setObject(CCString::create("영역 85%"), kMyLocalKey_condition1);
	ko->setObject(CCString::create("영역 85% + 체인지"), kMyLocalKey_condition2);
	ko->setObject(CCString::create("영역 100%"), kMyLocalKey_condition3);
	ko->setObject(CCString::create("영역 100% + 체인지"), kMyLocalKey_condition4);
	ko->setObject(CCString::create("85%"), kMyLocalKey_conditionTwoLine1);
	ko->setObject(CCString::create("85%\n+ 체인지"), kMyLocalKey_conditionTwoLine2);
	ko->setObject(CCString::create("100%"), kMyLocalKey_conditionTwoLine3);
	ko->setObject(CCString::create("100%\n+ 체인지"), kMyLocalKey_conditionTwoLine4);
	ko->setObject(CCString::create("%d 스테이지"), kMyLocalKey_stageValue);
	ko->setObject(CCString::create("나의 위치 %d"), kMyLocalKey_myrankValue);
	ko->setObject(CCString::create("랭킹 정보 확인 실패"), kMyLocalKey_failCheckRanking);
	ko->setObject(CCString::create("스테이지"), kMyLocalKey_stage);
	ko->setObject(CCString::create("게임에 사용할 아이템을 선택하세요."), kMyLocalKey_selectUseItem);
	ko->setObject(CCString::create("레벨 %d"), kMyLocalKey_levelValue);
	ko->setObject(CCString::create("파워 %s"), kMyLocalKey_powerValue);
	ko->setObject(CCString::create("레벨 %d 업그레이드"), kMyLocalKey_upgradeLevelValue);
	ko->setObject(CCString::create("아이템은 최대 3개까지\n선택이 가능합니다."), kMyLocalKey_totalItemSelectCount3);
	ko->setObject(CCString::create("골드가 부족합니다."), kMyLocalKey_goldNotEnought);
	ko->setObject(CCString::create("잼이 부족합니다."), kMyLocalKey_rubyNotEnought);
	ko->setObject(CCString::create("하트가 부족합니다."), kMyLocalKey_heartNotEnought);
	ko->setObject(CCString::create("구매하러 가시겠습니까?"), kMyLocalKey_goToShop);
	ko->setObject(CCString::create("기본속도향상"), kMyLocalKey_baseSpeedUpTitle);
	ko->setObject(CCString::create("유저 속도가 빠른상태로 시작합니다."), kMyLocalKey_baseSpeedUpContent);
	ko->setObject(CCString::create("더블아이템"), kMyLocalKey_doubleItemTitle);
	ko->setObject(CCString::create("아이템을 빠르게 생성합니다."), kMyLocalKey_doubleItemContent);
	ko->setObject(CCString::create("시간추가"), kMyLocalKey_longTimeTitle);
	ko->setObject(CCString::create("게임시간을 증가시킵니다."), kMyLocalKey_longTimeContent);
	ko->setObject(CCString::create("자석"), kMyLocalKey_magnetTitle);
	ko->setObject(CCString::create("골드와 아이템을 빨아들입니다."), kMyLocalKey_magnetContent);
	ko->setObject(CCString::create("아이템 뽑기"), kMyLocalKey_itemGachaTitle);
	ko->setObject(CCString::create("어떤 아이템이 나올까요?"), kMyLocalKey_itemGachaDefaultContent1);
	ko->setObject(CCString::create("뽑기에만 출몰하는 다양한 아이템!!"), kMyLocalKey_itemGachaDefaultContent2);
	ko->setObject(CCString::create("부활,무적아이템에 도전하세요!"), kMyLocalKey_itemGachaContent);
	ko->setObject(CCString::create("잼상점"), kMyLocalKey_rubyShop);
	ko->setObject(CCString::create("골드상점"), kMyLocalKey_goldShop);
	ko->setObject(CCString::create("하트상점"), kMyLocalKey_heartShop);
	ko->setObject(CCString::create("구매확인"), kMyLocalKey_checkBuy);
	ko->setObject(CCString::create("구매하시겠습니까?"), kMyLocalKey_realBuy);
	ko->setObject(CCString::create("설정"), kMyLocalKey_option);
	ko->setObject(CCString::create("쿠폰등록"), kMyLocalKey_regCoupon);
	ko->setObject(CCString::create("커뮤니티"), kMyLocalKey_community);
	ko->setObject(CCString::create("게임tip"), kMyLocalKey_gametip);
	ko->setObject(CCString::create("튜토리얼"), kMyLocalKey_tutorial);
	ko->setObject(CCString::create("시스템"), kMyLocalKey_system);
	ko->setObject(CCString::create("배경음"), kMyLocalKey_bgm);
	ko->setObject(CCString::create("효과음"), kMyLocalKey_effect);
	ko->setObject(CCString::create("조이스틱\n위치"), kMyLocalKey_joystickPosition);
	ko->setObject(CCString::create("조이스틱\n조작"), kMyLocalKey_joystickControl);
	ko->setObject(CCString::create("안전모드"), kMyLocalKey_safetyMode);
	ko->setObject(CCString::create("알림"), kMyLocalKey_noti);
	ko->setObject(CCString::create("푸쉬알림"), kMyLocalKey_pushNoti);
	ko->setObject(CCString::create("메세지\n알림"), kMyLocalKey_messageNoti);
	ko->setObject(CCString::create("선물을 받았습니다."), kMyLocalKey_getgift);
	ko->setObject(CCString::create("받은 메세지가 없습니다."), kMyLocalKey_nogift);
	ko->setObject(CCString::create("*30일 이후에 삭제됩니다."), kMyLocalKey_giftboxAlert);
	ko->setObject(CCString::create("재접속"), kMyLocalKey_reConnect);
	ko->setObject(CCString::create("다른 기기로 연결되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert1);
	ko->setObject(CCString::create("서버와의 접속에 오류가 발생하였습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert2);
	ko->setObject(CCString::create("세션이 종료되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert3);
	ko->setObject(CCString::create("서버연결에 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_reConnectAlert4);
	ko->setObject(CCString::create("BLOCKED"), kMyLocalKey_blocked);
	ko->setObject(CCString::create("계정이 아래의 사유로 정지 되었습니다."), kMyLocalKey_blockedMsg);
	ko->setObject(CCString::create("회원ID : "), kMyLocalKey_memberID);
	ko->setObject(CCString::create("게임버젼 : "), kMyLocalKey_gameversion);
	ko->setObject(CCString::create("게임탈퇴"), kMyLocalKey_withdraw);
	ko->setObject(CCString::create("고객센터"), kMyLocalKey_callCenter);
	ko->setObject(CCString::create("로그아웃 하시겠습니까?"), kMyLocalKey_checkLogout);
	ko->setObject(CCString::create("취소"), kMyLocalKey_cancel);
	ko->setObject(CCString::create("게임 탈퇴"), kMyLocalKey_withdrawTitle);
	ko->setObject(CCString::create("탈퇴시 게임을 이용하실 수 없으며"), kMyLocalKey_withdrawContent1);
	ko->setObject(CCString::create("모든 데이터가 영구적으로 삭제됩니다."), kMyLocalKey_withdrawContent2);
	ko->setObject(CCString::create("정말 탈퇴하시겠습니까?"), kMyLocalKey_withdrawContent3);
	ko->setObject(CCString::create("탈퇴하기"), kMyLocalKey_doWithdraw);
	ko->setObject(CCString::create("꺼짐"), kMyLocalKey_lightOff);
	ko->setObject(CCString::create("켜짐"), kMyLocalKey_lightOn);
	ko->setObject(CCString::create("오른쪽"), kMyLocalKey_right);
	ko->setObject(CCString::create("왼쪽"), kMyLocalKey_left);
	ko->setObject(CCString::create("고정"), kMyLocalKey_fix);
	ko->setObject(CCString::create("이동"), kMyLocalKey_move);
	ko->setObject(CCString::create("주간누적랭킹"), kMyLocalKey_weeklyranking);
	ko->setObject(CCString::create("%d일후 리셋"), kMyLocalKey_dayAfterReset);
	ko->setObject(CCString::create("%d시간후 리셋"), kMyLocalKey_hourAfterReset);
	ko->setObject(CCString::create("%d분후 리셋"), kMyLocalKey_secondAfterReset);
	ko->setObject(CCString::create("선물함"), kMyLocalKey_giftbox);
	ko->setObject(CCString::create("모두수락"), kMyLocalKey_allAccept);
	ko->setObject(CCString::create("티켓함"), kMyLocalKey_ticketBox);
	ko->setObject(CCString::create("도움함"), kMyLocalKey_helpBox);
	ko->setObject(CCString::create("도전함"), kMyLocalKey_challengeBox);
	ko->setObject(CCString::create("하트함"), kMyLocalKey_heartBox);
	ko->setObject(CCString::create("전체보기"), kMyLocalKey_allView);
	ko->setObject(CCString::create("하트가 도착했어요."), kMyLocalKey_arriveHeart);
	ko->setObject(CCString::create("도전장이 도착했어요."), kMyLocalKey_arriveChallenge);
	ko->setObject(CCString::create("도전을 받아들이시겠습니까?"), kMyLocalKey_checkAcceptChallenge);
	ko->setObject(CCString::create("도망가기"), kMyLocalKey_ttt);
	ko->setObject(CCString::create("도전에 임하는 자세를 가집시다!!\n도망으로 간주, 패하셨습니다."), kMyLocalKey_tttContent);
	ko->setObject(CCString::create("도전결과!!"), kMyLocalKey_resultChallenge);
	ko->setObject(CCString::create("졌습니다. 브레이크 포인트 필요함. 위에 contentObj"), kMyLocalKey_loseContent);
	ko->setObject(CCString::create("도움요청이 도착했어요."), kMyLocalKey_arriveHelp);
	ko->setObject(CCString::create("상대방을 도와줍니다."), kMyLocalKey_acceptHelp);
	ko->setObject(CCString::create("거절"), kMyLocalKey_deny);
	ko->setObject(CCString::create("수락"), kMyLocalKey_accept);
	ko->setObject(CCString::create("상대방의 도움이 왔어요!"), kMyLocalKey_arriveHelped);
	ko->setObject(CCString::create("카드 정보 로딩"), kMyLocalKey_cardInfoLoading);
	ko->setObject(CCString::create("도움을 받았습니다!!"), kMyLocalKey_iHelped);
	ko->setObject(CCString::create("티켓요청이 도착했어요."), kMyLocalKey_arriveNeedTicket);
	ko->setObject(CCString::create("티켓요청이 도착. 티켓은 퍼즐을 열 때 필요합니다. 친구를 도와주세요!!"), kMyLocalKey_arriveNeedTicketContent);
	ko->setObject(CCString::create("보내기"), kMyLocalKey_send);
	ko->setObject(CCString::create("티켓이 왔네요 어서 받으세요."), kMyLocalKey_arriveTicket);
	ko->setObject(CCString::create("티켓이 도착했습니다. 티켓을 받으세요."), kMyLocalKey_arriveTicketContent);
	ko->setObject(CCString::create("티켓받기"), kMyLocalKey_takeTicket);
	ko->setObject(CCString::create("친구추가 요청이 왔습니다."), kMyLocalKey_arriveAddFriend);
	ko->setObject(CCString::create("오류가 발생했습니다."), kMyLocalKey_whatError);
	ko->setObject(CCString::create("알 수 없는 요청이 도착했어요."), kMyLocalKey_arriveWhatError);
	ko->setObject(CCString::create("기본정렬"), kMyLocalKey_defaultSort);
	ko->setObject(CCString::create("획득순"), kMyLocalKey_takeOrder);
	ko->setObject(CCString::create("등급순"), kMyLocalKey_gradeOrder);
	ko->setObject(CCString::create("회전"), kMyLocalKey_rotation);
	ko->setObject(CCString::create("레벨을 올리면 보스를 좀 더 쉽게 물리칠 수 있어요!"), kMyLocalKey_upgradeSubMent);
	ko->setObject(CCString::create("버리고 다시 뽑기"), kMyLocalKey_itemRegacha);
	ko->setObject(CCString::create("%.0f%% 할인"), kMyLocalKey_itemRegachaDiscountValue);
	ko->setObject(CCString::create("이 아이템 사용"), kMyLocalKey_thisItemUse);
	ko->setObject(CCString::create("타임"), kMyLocalKey_time);
	ko->setObject(CCString::create("골드"), kMyLocalKey_gold);
	ko->setObject(CCString::create("점수"), kMyLocalKey_score);
	ko->setObject(CCString::create("총점"), kMyLocalKey_totalScore);
	ko->setObject(CCString::create("다음스테이지"), kMyLocalKey_nextStage);
	ko->setObject(CCString::create("다시하기"), kMyLocalKey_regame);
	ko->setObject(CCString::create("메인으로"), kMyLocalKey_toMain);
	ko->setObject(CCString::create("이전 스테이지를\n클리어 해주세요!"), kMyLocalKey_beforeNotClearPuzzle);
	ko->setObject(CCString::create("이전 액자를\n모두 클리어하면\n다음액자가 나타납니다."), kMyLocalKey_waitForUpdate);
	ko->setObject(CCString::create("기다릴게요!\n제 사진\n모아주세요!"), kMyLocalKey_diaryNoImg);
	ko->setObject(CCString::create("보상"), kMyLocalKey_reward);
	ko->setObject(CCString::create("스피드"), kMyLocalKey_speed);
	ko->setObject(CCString::create("준비하기"), kMyLocalKey_ready);
	ko->setObject(CCString::create("상대찾기"), kMyLocalKey_endlessReady);
	ko->setObject(CCString::create("지존"), kMyLocalKey_rankA);
	ko->setObject(CCString::create("영웅"), kMyLocalKey_rankB);
	ko->setObject(CCString::create("평민"), kMyLocalKey_rankC);
	ko->setObject(CCString::create("하수"), kMyLocalKey_rankD);
	ko->setObject(CCString::create("구입"), kMyLocalKey_buy);
	ko->setObject(CCString::create("꾸욱"), kMyLocalKey_controlClick);
	ko->setObject(CCString::create("잠김"), kMyLocalKey_locked);
	ko->setObject(CCString::create("상자를 선택해주세요."), kMyLocalKey_selectClearBox);
	
	ko->setObject(CCString::create(""), kMyLocalKey_titleLoadingBegin);
	ko->setObject(CCString::create("각 스테이지 별로 각기 다른 4장의 이미지를 획득해 보세요."), kMyLocalKey_titleLoading1);
	ko->setObject(CCString::create("공공장소나 대중교통 이용시에는 '대중교통 모드'로 Play 하세요. 획득한 이미지는 내 컬렉션에서 확인 할 수 있습니다."), kMyLocalKey_titleLoading2);
	ko->setObject(CCString::create("파란색 실루엣 영역을 획득해야 % 가 증가해요."), kMyLocalKey_titleLoading3);
	ko->setObject(CCString::create("'영역 다시 뽑기' 를 해서 넓은 영역으로 play 를 시작하면 좀 더 쉽게 높은 등급의 카드를 획득하기가 쉬워져요"), kMyLocalKey_titleLoading4);
	ko->setObject(CCString::create("'대중교통 모드' 는 play 중에 일시정지 버튼을 탭해서 설정할 수 있습니다."), kMyLocalKey_titleLoading5);
	ko->setObject(CCString::create("Clear 등급에 따라 획득되는 별의 개수가 다릅니다. 별이 모이면 다음 액자를 열 수 있어요."), kMyLocalKey_titleLoading7);
	ko->setObject(CCString::create("'업적' 창에서 달성한 업적을 보상받으세요. 보석을 드립니다."), kMyLocalKey_titleLoading8);
	ko->setObject(CCString::create("일일미션에 도전해 보세요. 일일 미션을 달성하시면 다양한 선물을 드립니다."), kMyLocalKey_titleLoading9);
	ko->setObject(CCString::create("주간랭킹에 도전해 보세요. 매주 새로운 선물을 푸짐하게 드립니다."), kMyLocalKey_titleLoading10);
	ko->setObject(CCString::create("change 글자를 모아서 특별한 이미지를 확인하세요."), kMyLocalKey_titleLoading11);
	ko->setObject(CCString::create("미사일을 업그레이드 하면 강력한 공격력으로 높은 등급 카드 획득이 가능합니다."), kMyLocalKey_titleLoading12);
	ko->setObject(CCString::create("아이템뽑기에서는 '부활'/'무적' 아이템을 뽑을 수 있어요."), kMyLocalKey_titleLoading13);
	ko->setObject(CCString::create("스테이지마다 showtime 에 도전해 보세요."), kMyLocalKey_titleLoading14);
	ko->setObject(CCString::create("매주 새로운 이벤트가 열립니다. 공식 카페를 확인해 주세요."), kMyLocalKey_titleLoading15);
	ko->setObject(CCString::create("게임 내에서는 이름 대신 닉네임을 사용합니다."), kMyLocalKey_titleLoading16);
	ko->setObject(CCString::create("99% 라 너무 아쉽다면, 보너스 뽑기에 도전 해 보세요."), kMyLocalKey_titleLoading17);
	ko->setObject(CCString::create("이벤트 스테이지에서는 특별한 이미지와 선물이 기다립니다."), kMyLocalKey_titleLoading18);
	ko->setObject(CCString::create("스테이지별 미션 clear 가 어렵다면 아이템 뽑기를 통해 미션 전용 아이템을 사용해 보세요."), kMyLocalKey_titleLoading19);
	ko->setObject(CCString::create("한 번에 많은 영역을 획득할 수록 여러 개의 미사일이 발사되며, 공격을 성공한 만큼 콤보가 적용됩니다."), kMyLocalKey_titleLoading6);
	ko->setObject(CCString::create("이어하기는 두 번 까지만 사용할 수 있습니다."), kMyLocalKey_titleLoading20);
	ko->setObject(CCString::create("내카드 목록에서 카드 이미지를 터치하면 카드 상세 내용을 보실 수 있습니다."), kMyLocalKey_titleLoading21);
	ko->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading22);
	ko->setObject(CCString::create("85% 로 클리어 하면 별 1개를 획득합니다."), kMyLocalKey_titleLoading23);
	ko->setObject(CCString::create("change를 모두 모은 후 클리어하면 별 2개를 획득합니다."), kMyLocalKey_titleLoading24);
	ko->setObject(CCString::create("100%로 클리어 하면 별 3개를 획득합니다."), kMyLocalKey_titleLoading25);
	ko->setObject(CCString::create("change를 모두 모은 후 100%로 클리어하면 별 4개를 모을 수 있습니다."), kMyLocalKey_titleLoading26);
	ko->setObject(CCString::create("무적 아이템을 먹으면 몇 초간 무적상태가 되어 보스의 공격을 받지 않습니다."), kMyLocalKey_titleLoading27);
	ko->setObject(CCString::create("플레이 중 획득한 영역에 따라 골드를 드립니다."), kMyLocalKey_titleLoading28);
	ko->setObject(CCString::create("30%나 할인되는 종합 아이템 구매 찬스를 놓치지 마세요!"), kMyLocalKey_titleLoading29);
	ko->setObject(CCString::create("미션스테이지에서는 미션을 먼저 완료한 후 클리어가 가능합니다."), kMyLocalKey_titleLoading30);
	ko->setObject(CCString::create("미사일 공격 시 그리는 선에 미사일이 닿으면 유도 공격을 당해요."), kMyLocalKey_titleLoading31);
	ko->setObject(CCString::create("게임 중 타임 보너스 아이템을 먹으면 남은 시간에서 10초가 추가됩니다."), kMyLocalKey_titleLoading32);
	ko->setObject(CCString::create("선을 그리다가 꾸욱 버튼에서 손을 떼면 캐릭터가 안전 영역으로 되돌아 갑니다."), kMyLocalKey_titleLoading33);
	ko->setObject(CCString::create("보스에게 연속 공격을 성공시키면 콤보가 적용되며, 게임 결과에서 콤보 보너스를 받을 수 있습니다."), kMyLocalKey_titleLoading34);
	ko->setObject(CCString::create("내가 모은 별의 개수는 메인화면의 왼쪽 상단에 있는 별 아이콘을 통해 확인하실 수 있습니다."), kMyLocalKey_titleLoading35);
	ko->setObject(CCString::create("운석이 떨어지면 캐릭터에 맞지 않도록 주의하세요."), kMyLocalKey_titleLoading36);
	ko->setObject(CCString::create("조이스틱과 꾸욱 버튼의 위치를 바꾸고 싶다면 설정메뉴에서 조이스틱위치를 반대로 변경해 보세요."), kMyLocalKey_titleLoading37);
	ko->setObject(CCString::create("조이스틱의 위치를 고정하고 싶다면 설정메뉴에서 조이스틱조작을 고정으로 변경해 보세요"), kMyLocalKey_titleLoading38);
	ko->setObject(CCString::create("꾸욱 버튼을 누른채 한쪽 방향으로 그린 다음 되돌아 오면 한줄 그리기가 가능합니다."), kMyLocalKey_titleLoading39);
	ko->setObject(CCString::create("그리는 도중 줄이 꼬여도 당황하지 마세요. 줄이 꼬인 상태에서도 영역을 획득할 수 있습니다."), kMyLocalKey_titleLoading40);
	ko->setObject(CCString::create("작은 몬스터에는 HP가 존재하며, 공격이 성공할 때마다 HP가 줄어듭니다."), kMyLocalKey_titleLoading41);
	ko->setObject(CCString::create("액자 하나를 완성할 때마다 골드선물을 드립니다."), kMyLocalKey_titleLoading42);
	ko->setObject(CCString::create("클리어 점수는 영역획득점수와 콤보점수, 남은 타임 보너스가 합산된 점수입니다."), kMyLocalKey_titleLoading43);
	ko->setObject(CCString::create("고득점을 노린다면 연속 콤보를 성공시켜 보세요."), kMyLocalKey_titleLoading44);
	ko->setObject(CCString::create(""), kMyLocalKey_titleLoadingEnd);
	
	ko->setObject(CCString::create("게임시작"), kMyLocalKey_gamestart);
	ko->setObject(CCString::create("대전 시작"), kMyLocalKey_battleStart);
	ko->setObject(CCString::create("내 미사일"), kMyLocalKey_mymissile);
	ko->setObject(CCString::create("파워"), kMyLocalKey_power);
	ko->setObject(CCString::create("뽑기"), kMyLocalKey_gacha);
	ko->setObject(CCString::create("일일미션을 완료해보세요!\n선물함으로 선물을 드려요! ^^"), kMyLocalKey_stageListDown);
	ko->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_frameOpenConditionTitle);
	ko->setObject(CCString::create("이전 액자 전체 클리어\n+ 별 %d개 이상"), kMyLocalKey_frameOpenConditionContent);
	ko->setObject(CCString::create("잼로 오픈"), kMyLocalKey_frameOpenConditionContentRuby);
	ko->setObject(CCString::create("%s\n%d:00-%d:00"), kMyLocalKey_frameOpenConditionContentTimeWeek);
	ko->setObject(CCString::create("%s월 %s일 %s:%s 오픈"), kMyLocalKey_frameOpenConditionContentTimeDate);
	ko->setObject(CCString::create("자세히 보기"), kMyLocalKey_detailView);
	ko->setObject(CCString::create("별 획득 방법"), kMyLocalKey_detailConditionPopupTitle);
	ko->setObject(CCString::create("<font newline=14 strokecolor=000>스테이지를 클리어 할 때마다<font newline=30>클리어 단계에 따라 별을 획득합니다.<font color=961>한 스테이지<font newline=14>에서 획득할 수 있는<font color=961>별 갯수는 총 10개<font newline=14>입니다."), kMyLocalKey_detailConditionPopupContent);
//	ko->setObject(CCString::create("<font color=999 size=12>스테이지에서 <font color=990 size=12>4가지 등급<font color=999 size=12 newline=13>을 모두 Clear하면<font color=990 size=12>별 10개<font color=999 size=12>를 모을 수 있습니다."), kMyLocalKey_detailConditionPopupContent2);
//	ko->setObject(CCString::create("에서 획득할 수 있는"), kMyLocalKey_detailConditionPopupContent3);
//	ko->setObject(CCString::create("별 갯수는 총 10개"), kMyLocalKey_detailConditionPopupContent4);
//	ko->setObject(CCString::create("입니다."), kMyLocalKey_detailConditionPopupContent5);
	ko->setObject(CCString::create("결제 실패"), kMyLocalKey_failPurchase);
	ko->setObject(CCString::create("지금 상점으로 이동하시겠습니까?"), kMyLocalKey_enoughtGoodsContent);
	ko->setObject(CCString::create("포기하기"), kMyLocalKey_giveup);
	ko->setObject(CCString::create("상점 바로가기"), kMyLocalKey_goShopButton);
	
	ko->setObject(CCString::create("액자를 플레이할 수 있는 시간이 지났습니다.\n홈으로 돌아갑니다."), kMyLocalKey_timeOutFrame);
	ko->setObject(CCString::create("바로입장"), kMyLocalKey_directEnter);
	ko->setObject(CCString::create("카드 승급 기회!!"), kMyLocalKey_rankUpTitle);
	ko->setObject(CCString::create("승급 성공시 한 단계 높은 카드를 획득할 수 있어요!"), kMyLocalKey_rankUpSubTitle);
	ko->setObject(CCString::create("현재 획득카드"), kMyLocalKey_recentTakeCard);
	ko->setObject(CCString::create("승급확률"), kMyLocalKey_rankUpRate);
	ko->setObject(CCString::create("승급하기"), kMyLocalKey_rankup);
	
	ko->setObject(CCString::create("구매하기"), kMyLocalKey_purchase);
	ko->setObject(CCString::create("첫구매시 할인"), kMyLocalKey_firstPurchaseSale);
	ko->setObject(CCString::create("%s할인은 첫 구매 회원님께만"), kMyLocalKey_firstPurchaseMent1);
	ko->setObject(CCString::create(" 적용됩니다."), kMyLocalKey_firstPurchaseMent2);
	ko->setObject(CCString::create("이 창을 닫으면 %s 할인 기회는 사라집니다."), kMyLocalKey_firstPurchaseMent3);
	ko->setObject(CCString::create("지금 묶음 아이템을 구매하시면"), kMyLocalKey_emptyItemSaleMent1);
	ko->setObject(CCString::create("%s 할인해 드립니다."), kMyLocalKey_emptyItemSaleMent2);
	ko->setObject(CCString::create("플레이를 도와주는 기능성 아이템을"), kMyLocalKey_stupidNpuHelpMent1);
	ko->setObject(CCString::create("%s할인된 가격으로 사용해보세요."), kMyLocalKey_stupidNpuHelpMent2);
	ko->setObject(CCString::create("묶음아이템 할인"), kMyLocalKey_packageItemSale);
	ko->setObject(CCString::create("5번째 구매시"), kMyLocalKey_mileageMent1);
	ko->setObject(CCString::create("아주 특별한 할인혜택의 기회가 있으니"), kMyLocalKey_mileageMent2);
	ko->setObject(CCString::create("혜택을 꼭! 확인해주세요."), kMyLocalKey_mileageMent3);
	ko->setObject(CCString::create("단 한번의 구매 찬스"), kMyLocalKey_eventShopTitle);
	ko->setObject(CCString::create("당신만을 위한 특별한 기회! %s 보너스 혜택을 드려요."), kMyLocalKey_eventShopMent1);
	ko->setObject(CCString::create("※ 이 창을 닫으면 %s 보너스 기회는 사라집니다."), kMyLocalKey_eventShopMent2);
	ko->setObject(CCString::create("공격레벨을 올리면"), kMyLocalKey_levelupGuideMent1);
	ko->setObject(CCString::create("보스를 좀 더 쉽게 물리칠 수 있습니다."), kMyLocalKey_levelupGuideMent2);
	ko->setObject(CCString::create("업그레이드 버튼을 눌러"), kMyLocalKey_levelupGuideMent3);
	ko->setObject(CCString::create("공격레벨을 올려보세요!"), kMyLocalKey_levelupGuideMent4);
	ko->setObject(CCString::create("다음에"), kMyLocalKey_next);
	ko->setObject(CCString::create("업그레이드"), kMyLocalKey_levelupGo);
	ko->setObject(CCString::create("업적"), kMyLocalKey_achievement);
	ko->setObject(CCString::create("모든보상 받기"), kMyLocalKey_allRewardGet);
	ko->setObject(CCString::create("종합 아이템 구매 찬스!"), kMyLocalKey_itemPackageChance);
	ko->setObject(CCString::create("아이템이 하나도 없네요."), kMyLocalKey_emptyItemSaleTitle);
	ko->setObject(CCString::create("첫구매 대폭할인!!"), kMyLocalKey_firstPurchaseSaleTitle);
	ko->setObject(CCString::create("사랑합니다. 고객님!"), kMyLocalKey_mileageTitle);
	ko->setObject(CCString::create("미사일 업그레이드"), kMyLocalKey_missileUpgrade);
	ko->setObject(CCString::create("<font>누적영역</font><font color=961>%d%%</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalPercent1);
	ko->setObject(CCString::create("%s%%"), kMyLocalKey_todaymissionTotalPercent2);
	ko->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalPercent3);
	ko->setObject(CCString::create("현재 획득영역 "), kMyLocalKey_todaymissionTotalPercent4);
	ko->setObject(CCString::create("<font>누적점수</font><font color=961>%s점</font><font>을 획득하라</font>"), kMyLocalKey_todaymissionTotalScore1);
	ko->setObject(CCString::create("%s점"), kMyLocalKey_todaymissionTotalScore2);
	ko->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalScore3);
	ko->setObject(CCString::create("현재 획득점수 "), kMyLocalKey_todaymissionTotalScore4);
	ko->setObject(CCString::create("<font color=961>%s골드</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalTakeGold1);
	ko->setObject(CCString::create("%s골드"), kMyLocalKey_todaymissionTotalTakeGold2);
	ko->setObject(CCString::create("현재 획득골드 "), kMyLocalKey_todaymissionTotalTakeGold3);
	ko->setObject(CCString::create("<font>부하몹</font><font color=961>%d마리</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalCatch1);
	ko->setObject(CCString::create("%s마리"), kMyLocalKey_todaymissionTotalCatch2);
	ko->setObject(CCString::create(" 사냥하라!!"), kMyLocalKey_todaymissionTotalCatch3);
	ko->setObject(CCString::create("현재 잡은 부하몹 "), kMyLocalKey_todaymissionTotalCatch4);
	ko->setObject(CCString::create("오늘의 미션을 완료하였습니다."), kMyLocalKey_todaymissionSuccess);
	ko->setObject(CCString::create("완료"), kMyLocalKey_complete);
	ko->setObject(CCString::create("업적달성"), kMyLocalKey_achieveSuccess);
	ko->setObject(CCString::create("업적미완성"), kMyLocalKey_achieveNotSuccess);
	ko->setObject(CCString::create("업적보상"), kMyLocalKey_achieveReward);
	ko->setObject(CCString::create("달성한 업적이 없습니다."), kMyLocalKey_nothingSuccessAchieve);
	ko->setObject(CCString::create("보상받을 업적이 없습니다."), kMyLocalKey_nothingRewardAchieve);
	
	
	ko->setObject(CCString::create("영역획득"), kMyLocalKey_missionTitle0);
	ko->setObject(CCString::create("용감한 기사"), kMyLocalKey_missionTitle1);
	ko->setObject(CCString::create("부하몹 사냥"), kMyLocalKey_missionTitle2);
	ko->setObject(CCString::create("욕심쟁이"), kMyLocalKey_missionTitle3);
	ko->setObject(CCString::create("수집가"), kMyLocalKey_missionTitle4);
	ko->setObject(CCString::create("완벽주의자"), kMyLocalKey_missionTitle5);
	ko->setObject(CCString::create("결벽주의자"), kMyLocalKey_missionTitle6);
	ko->setObject(CCString::create("비지니스맨"), kMyLocalKey_missionTitle7);
	
	ko->setObject(CCString::create("<font color=961 size=13>%.0f%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionDiscription0);
	ko->setObject(CCString::create("<font color=999 size=13>보스의 에너지를 모두 소진시켜라!"), kMyLocalKey_missionDiscription1);
	ko->setObject(CCString::create("<font color=999 size=13>부하 몬스터를 가두어 잡으세요!"), kMyLocalKey_missionDiscription2);
	ko->setObject(CCString::create("<font color=999 size=13>정해진 횟수만큼 한번에 많이 먹으세요!"), kMyLocalKey_missionDiscription3);
	ko->setObject(CCString::create("<font color=999 size=13>정해진 숫자만큼 아이템을 모으세요!"), kMyLocalKey_missionDiscription4);
	ko->setObject(CCString::create("<font color=999 size=13>정해진 목표로 정확하게 영역을 획득하세요!"), kMyLocalKey_missionDiscription5);
	ko->setObject(CCString::create("<font color=999 size=13>CHANGE를 순서대로 획득하세요!"), kMyLocalKey_missionDiscription6);
	ko->setObject(CCString::create("<font color=999 size=13>목표시간 내에 클리어하세요!"), kMyLocalKey_missionDiscription7);
	
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition0);
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition1);
	ko->setObject(CCString::create("<font>목표 <font color=961 size=18>%d마리"), kMyLocalKey_missionCondition2);
	ko->setObject(CCString::create("<font color=961 size=18>%d%%이상 %d번"), kMyLocalKey_missionCondition3);
	ko->setObject(CCString::create("<font>목표 <font color=961 size=18>%d개"), kMyLocalKey_missionCondition4);
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition5);
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition6);
	ko->setObject(CCString::create("<font>목표시간 <font color=961 size=18>%d초"), kMyLocalKey_missionCondition7);
	
	
	ko->setObject(CCString::create("대쉬"), kMyLocalKey_item4title);
	ko->setObject(CCString::create("잠깐동안 빠른속도로 이동 가능합니다."), kMyLocalKey_item4ment);
	ko->setObject(CCString::create("부하몹제거"), kMyLocalKey_item5title);
	ko->setObject(CCString::create("부하 몬스터 한마리를 제거합니다."), kMyLocalKey_item5ment);
	ko->setObject(CCString::create("침묵"), kMyLocalKey_item7title);
	ko->setObject(CCString::create("잠깐동안 몬스터가 공격을 못하도록 합니다."), kMyLocalKey_item7ment);
	ko->setObject(CCString::create("부활"), kMyLocalKey_item10title);
	ko->setObject(CCString::create("마지막 순간에 부활합니다."), kMyLocalKey_item10ment);
	ko->setObject(CCString::create("시간추가"), kMyLocalKey_item8title);
	ko->setObject(CCString::create("제한시간을 늘려줍니다."), kMyLocalKey_item8ment);
	ko->setObject(CCString::create("일시정지"), kMyLocalKey_pause);
	ko->setObject(CCString::create("계속하기"), kMyLocalKey_continue);
	ko->setObject(CCString::create("나가기"), kMyLocalKey_ingameOut);
	ko->setObject(CCString::create("재시작"), kMyLocalKey_ingameReplay);
	ko->setObject(CCString::create("대중교통\n모드"), kMyLocalKey_publicPlaceMode);
	ko->setObject(CCString::create("이어하기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass1);
	ko->setObject(CCString::create("맵다시뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass2);
	ko->setObject(CCString::create("업그레이드 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass3);
	ko->setObject(CCString::create("아이템뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass4);
	ko->setObject(CCString::create("1프로뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass5);
	ko->setObject(CCString::create("<font newline=16>오늘의 미션을 완료하였습니다.<font color=961 newline=16>보상은 선물함으로 지급됩니다."), kMyLocalKey_todaymissionSuccessCommon);
	ko->setObject(CCString::create("무료"), kMyLocalKey_free);
	ko->setObject(CCString::create("영역 다시뽑기"), kMyLocalKey_areaReGacha);
	ko->setObject(CCString::create("버튼을 눌러주세요."), kMyLocalKey_pleaseClickButton);
	ko->setObject(CCString::create("멈춰!"), kMyLocalKey_stop);
	ko->setObject(CCString::create("100% 도전하기"), kMyLocalKey_go100percent);
	ko->setObject(CCString::create("계정연결"), kMyLocalKey_accountLink);
	ko->setObject(CCString::create("최고 레벨 %d"), kMyLocalKey_maxLevel);
	ko->setObject(CCString::create("업그레이드 완료"), kMyLocalKey_endUpgrade);
	ko->setObject(CCString::create("%d성 카드 획득방법"), kMyLocalKey_cardLockedTitle);
	ko->setObject(CCString::create("<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent1);
	ko->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent2);
	ko->setObject(CCString::create("<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent3);
	ko->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent4);
	ko->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTime);
	ko->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTimeMinute);
	ko->setObject(CCString::create("<font color=961 size=15>%dHour"), kMyLocalKey_todaymissionRemainTime2);
	ko->setObject(CCString::create("<font color=961 size=15>%dMinute"), kMyLocalKey_todaymissionRemainTimeMinute2);
	ko->setObject(CCString::create("<font color=961 size=15>모든 피스 획득!"), kMyLocalKey_puzzleSuccessTitle);
	ko->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 하트를 드릴께요!"), kMyLocalKey_puzzleSuccessMent);
	ko->setObject(CCString::create("<font color=961 size=15>놀라워요! <font color=961 size=15>퍼펙트 클리어!!"), kMyLocalKey_puzzlePerfectTitle);
	ko->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 잼를 드릴께요!"), kMyLocalKey_puzzlePerfectMent);
	ko->setObject(CCString::create("이어하시겠습니까?"), kMyLocalKey_doYouWantToContinue);
	ko->setObject(CCString::create("이어하기"), kMyLocalKey_onContinue);
	ko->setObject(CCString::create("그만하기"), kMyLocalKey_offContinue);
	ko->setObject(CCString::create("이번 스테이지는 아이템을 사용할 수 없습니다."), kMyLocalKey_notUseItem);
	ko->setObject(CCString::create("새로운 아이템 등장!"), kMyLocalKey_newItemTitle);
	ko->setObject(CCString::create("새로운 아이템을 선물로 드릴께요!"), kMyLocalKey_newItemMent);
	ko->setObject(CCString::create("보스가 쏜 미사일이\n선에 닿으면 불씨가 생겨 닿으면 죽어요."), kMyLocalKey_dieTutorial1);
	ko->setObject(CCString::create("선을 그을때 몬스터가\n선에 닿으면 파동이 생겨 죽어요!"), kMyLocalKey_dieTutorial2);
	ko->setObject(CCString::create(""), kMyLocalKey_dieTutorial3);
	ko->setObject(CCString::create("남은시간 "), kMyLocalKey_restTime);
	ko->setObject(CCString::create("%dDay"), kMyLocalKey_restTimeDay);
	ko->setObject(CCString::create("%dHour"), kMyLocalKey_restTimeHour);
	ko->setObject(CCString::create("%dMinute"), kMyLocalKey_restTimeMinute);
	ko->setObject(CCString::create("%dSecond"), kMyLocalKey_restTimeSecond);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
	
	
	ko->setObject(CCString::create("게임을 종료하시겠습니까?"), kMyLocalKey_exit);
	
	ko->setObject(CCString::create("카운트 폭탄"), kMyLocalKey_warning9);
	ko->setObject(CCString::create("주황 톱니바퀴"), kMyLocalKey_warning105);
	ko->setObject(CCString::create("파란 톱니바퀴"), kMyLocalKey_warning106);
	ko->setObject(CCString::create("파괴 레이저"), kMyLocalKey_warning107);
	ko->setObject(CCString::create("미사일"), kMyLocalKey_warning108);
	ko->setObject(CCString::create("돌진"), kMyLocalKey_warning109);
	ko->setObject(CCString::create("불꽃놀이"), kMyLocalKey_warning110);
	ko->setObject(CCString::create("해바라기"), kMyLocalKey_warning111);
	ko->setObject(CCString::create("얼음폭탄"), kMyLocalKey_warning112);
	ko->setObject(CCString::create("유도폭탄"), kMyLocalKey_warning113);
	ko->setObject(CCString::create("토네이도"), kMyLocalKey_warning1001);
	ko->setObject(CCString::create("태양권"), kMyLocalKey_warning1002);
	ko->setObject(CCString::create("슬로우존"), kMyLocalKey_warning1003);
	ko->setObject(CCString::create("가시감옥"), kMyLocalKey_warning1004);
	ko->setObject(CCString::create("빙결"), kMyLocalKey_warning1005);
	ko->setObject(CCString::create("혼란"), kMyLocalKey_warning1006);
	ko->setObject(CCString::create("순간이동"), kMyLocalKey_warning1007);
	ko->setObject(CCString::create("투명망토"), kMyLocalKey_warning1008);
	ko->setObject(CCString::create("화염 방사"), kMyLocalKey_warning1009);
	ko->setObject(CCString::create("벼락"), kMyLocalKey_warning1010);
	ko->setObject(CCString::create("스피드레이저"), kMyLocalKey_warning1011);
	ko->setObject(CCString::create("방사능"), kMyLocalKey_warning1012);
	ko->setObject(CCString::create("메테오"), kMyLocalKey_warning1013);
	ko->setObject(CCString::create("운석"), kMyLocalKey_warning1014);
	ko->setObject(CCString::create("뾰족풍차"), kMyLocalKey_warning1015);
	ko->setObject(CCString::create("다이너마이트 지뢰"), kMyLocalKey_warning1016);
	ko->setObject(CCString::create("불꽃스토커"), kMyLocalKey_warning1017);
	ko->setObject(CCString::create("문득구름"), kMyLocalKey_warning1018);
	ko->setObject(CCString::create("보스공격저지"), kMyLocalKey_warningBossSuccess);
	ko->setObject(CCString::create("마지막 목숨"), kMyLocalKey_warningLastLife);
	
	ko->setObject(CCString::create("새로운 보스 공격!"), kMyLocalKey_newBossPattern);
	
	ko->setObject(CCString::create("맵에 부딪힐때마다 숫자가 줄고, 0이 되면 폭발해요."), kMyLocalKey_patternContent9);
	ko->setObject(CCString::create("하나의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent105);
	ko->setObject(CCString::create("여러개의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent106);
	ko->setObject(CCString::create("레이저가 획득영역을 부숴요."), kMyLocalKey_patternContent107);
	ko->setObject(CCString::create("그리는도중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent108);
	ko->setObject(CCString::create("보스가 획득영역을 파괴하며 이동해요."), kMyLocalKey_patternContent109);
	ko->setObject(CCString::create("획득영역에서 폭발후 미사일을 퍼트려요."), kMyLocalKey_patternContent110);
	ko->setObject(CCString::create("사방으로 미사일을 발사해요."), kMyLocalKey_patternContent111);
	ko->setObject(CCString::create("연속으로 발사되어 획득영역을 부숴요."), kMyLocalKey_patternContent112);
	ko->setObject(CCString::create("따라와서 폭발해요."), kMyLocalKey_patternContent113);
	ko->setObject(CCString::create("맵의 일부를 일정시간 가립니다."), kMyLocalKey_patternContent1001);
	ko->setObject(CCString::create("일정시간 화면 전체를 뒤덮어 시야를 가려요."), kMyLocalKey_patternContent1002);
	ko->setObject(CCString::create("영역안에서는 느려져요."), kMyLocalKey_patternContent1003);
	ko->setObject(CCString::create("붉은 원 밖으로 나갈 수 없어요."), kMyLocalKey_patternContent1004);
	ko->setObject(CCString::create("일정시간 움직이지 못해요."), kMyLocalKey_patternContent1005);
	ko->setObject(CCString::create("조작 방향의 반대로 움직여요."), kMyLocalKey_patternContent1006);
	ko->setObject(CCString::create("보스가 다른위치로 순간이동해요."), kMyLocalKey_patternContent1007);
	ko->setObject(CCString::create("일정시간 보스가 보이지 않아요."), kMyLocalKey_patternContent1008);
	ko->setObject(CCString::create("획득영역에 불을 피워요."), kMyLocalKey_patternContent1009);
	ko->setObject(CCString::create("캐릭터를 따라오며 맵을 부숴요."), kMyLocalKey_patternContent1010);
	ko->setObject(CCString::create("그리는중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent1011);
	ko->setObject(CCString::create("방사능이 퍼진곳을 조심하세요."), kMyLocalKey_patternContent1012);
	ko->setObject(CCString::create("대각선으로 떨어져 맵을 부숴요."), kMyLocalKey_patternContent1013);
	ko->setObject(CCString::create("위에서 아래로 운석이 떨어져요."), kMyLocalKey_patternContent1014);
	ko->setObject(CCString::create("닿지않도록 주의하세요."), kMyLocalKey_patternContent1015);
	ko->setObject(CCString::create("카운트가 끝나면 폭발해요."), kMyLocalKey_patternContent1016);
	ko->setObject(CCString::create("일정시간 맵을 따라 움직여요."), kMyLocalKey_patternContent1017);
	ko->setObject(CCString::create("언제 폭발할지 몰라요."), kMyLocalKey_patternContent1018);
	
//	ko->setObject(CCString::create("서버에 연결할 수 없습니다."), kMyLocalKey_canNotConnectedServer);
//	ko->setObject(CCString::create("화면을 터치 해주세요."), kMyLocalKey_touchPlease);
	
	ko->setObject(CCString::create("조작방법에 대한 튜토리얼을 시작하겠습니다.\n가운데 빨간 동그라미가 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다."), kMyLocalKey_tutorial1);
	ko->setObject(CCString::create("먼저 영역 위를 이동하는 방법에 대해 소개해드릴게요.\n오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있어요.\n조이스틱으로 캐릭터를 위로 이동시켜보세요."), kMyLocalKey_tutorial2);
	ko->setObject(CCString::create("캐릭터를 위로 이동시키기"), kMyLocalKey_tutorial3);
	ko->setObject(CCString::create("다음에는 영역을 획득하는 방법을 알아보도록 해요.\n왼쪽 아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있답니다.\n보이는 것처럼 영역을 획득해보세요."), kMyLocalKey_tutorial4);
	ko->setObject(CCString::create("영역획득하기"), kMyLocalKey_tutorial5);
	ko->setObject(CCString::create("파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다."), kMyLocalKey_tutorial6);
	ko->setObject(CCString::create("<font>제한시간 내에 달성도 <font color=961 newline=13>85%를 넘기면 클리어!!"), kMyLocalKey_tutorial7);
	ko->setObject(CCString::create("기본 튜토리얼을 모두 진행하셨습니다.\n보상으로 5000골드를 드립니다.\n본 게임으로 들아갑니다."), kMyLocalKey_tutorial8);
	
	ko->setObject(CCString::create("point1. 겹친선도 획득!"), kMyLocalKey_controlTip_lineTangle_title);
	ko->setObject(CCString::create("꼬인선! 걱정마세요! 그대로 획득할 수 있어요!"), kMyLocalKey_controlTip_lineTangle_content);
	ko->setObject(CCString::create("point2. 한줄 먹기!"), kMyLocalKey_controlTip_slimLine_title);
	ko->setObject(CCString::create("캐릭터를 왼쪽으로 이동 후 오른쪽으로 조작하면 OK!"), kMyLocalKey_controlTip_slimLine_content);
	
	ko->setObject(CCString::create("미션"), kMyLocalKey_mission);
	ko->setObject(CCString::create("PVP"), kMyLocalKey_endlessMode);
	ko->setObject(CCString::create("주간연승랭킹"), kMyLocalKey_endlessWeeklyranking);
	ko->setObject(CCString::create("정보"), kMyLocalKey_endlessMyInfo);
	ko->setObject(CCString::create("전적"), kMyLocalKey_endlessInfoScore);
	ko->setObject(CCString::create("<font color=999 strokecolor=000 strokesize=1>%d승 %d패 </font><font color=950 strokecolor=000 strokesize=1>(승률%d%%)</font>"), kMyLocalKey_endlessInfoScoreValue2);
	ko->setObject(CCString::create("%d승 %d패 승률%d%%"), kMyLocalKey_endlessInfoScoreValue);
	ko->setObject(CCString::create("최고점수"), kMyLocalKey_endlessHighScore);
	ko->setObject(CCString::create("최고연승"), kMyLocalKey_endlessHighStraight);
	ko->setObject(CCString::create("<font size=15 color=#FFAA14>%d</font><font size=10 color=#FFAA14>연승</font>"), kMyLocalKey_endlessHighStraightValue1);
	ko->setObject(CCString::create("%d연승"), kMyLocalKey_endlessHighStraightValue2);
	ko->setObject(CCString::create("뭔가 문제가 발생하였습니다.\n다시 시도해주세요."), kMyLocalKey_endlessServerError);
	ko->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_endlessDownloadFail);
	ko->setObject(CCString::create("포기하시면 1패가 추가됩니다.\n계속 하시겠습니까?"), kMyLocalKey_endlessCheckGiveup);
	
	ko->setObject(CCString::create("스페셜데이"), kMyLocalKey_attendanceSpecialTitle);
	ko->setObject(CCString::create("매일 매일 연속 출석!!"), kMyLocalKey_attendanceSeqTitle);
	ko->setObject(CCString::create("연속출석 일수에 따라 더욱 푸짐한 선물을 드립니다."), kMyLocalKey_attendanceSpecialContent);
	
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>골드"), kMyLocalKey_attendanceSpecialGoodsTypeGold);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>잼"), kMyLocalKey_attendanceSpecialGoodsTypeRuby);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem9);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem6);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem11);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass1);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass2);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass3);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass4);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass5);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass6);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>종류"), kMyLocalKey_attendanceSpecialGoodsTypeMany);
	
	ko->setObject(CCString::create("<font size=12>%d<font size=8>골드"), kMyLocalKey_attendanceGoodsTypeGold);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>잼"), kMyLocalKey_attendanceGoodsTypeRuby);
	ko->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem9);
	ko->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem6);
	ko->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem11);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass1);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass2);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass3);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass4);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass5);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass6);
	ko->setObject(CCString::create("선물상자"), kMyLocalKey_attendanceGoodsTypeMany);
	
	ko->setObject(CCString::create("출석체크"), kMyLocalKey_attendanceTitle);
	ko->setObject(CCString::create("지급된 보상은 수신함에서 확인 가능합니다."), kMyLocalKey_attendanceContent);
	
	ko->setObject(CCString::create("일차"), kMyLocalKey_attendanceDay);
	
	ko->setObject(CCString::create("생명의 돌"), kMyLocalKey_buyMorphingTitle);
	ko->setObject(CCString::create("<font color=961 newline=18>생명의 돌을 사용하면 카드가 살아납니다.<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_buyMorphingContent);
	
	ko->setObject(CCString::create("보유갯수 :"), kMyLocalKey_buyMorphingTake);
	ko->setObject(CCString::create("개"), kMyLocalKey_buyMorphingTakeValue);
	
	ko->setObject(CCString::create("사용하기"), kMyLocalKey_buyMorphingBuy);
	
	ko->setObject(CCString::create("생명의 돌이 부족합니다."), kMyLocalKey_notEnoughtMorphingTitle);
	ko->setObject(CCString::create("<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_notEnoughtMorphingContent);
	
	ko->setObject(CCString::create("쿠폰등록"), kMyLocalKey_couponTitle);
	ko->setObject(CCString::create("입력"), kMyLocalKey_couponContent);
	ko->setObject(CCString::create("쿠폰등록성공"), kMyLocalKey_couponSuccess);
	ko->setObject(CCString::create("선물함을 확인하세요."), kMyLocalKey_couponGiftbox);
	ko->setObject(CCString::create("쿠폰등록실패"), kMyLocalKey_couponFail);
	ko->setObject(CCString::create("이미 사용한 쿠폰입니다."), kMyLocalKey_couponAlready);
	ko->setObject(CCString::create("유효기간이 만료 쿠폰입니다."), kMyLocalKey_couponExpire);
	ko->setObject(CCString::create("잘못된 사용입니다."), kMyLocalKey_couponOserror);
	ko->setObject(CCString::create("쿠폰을 찾을 수 없습니다."), kMyLocalKey_couponDontfind);
	ko->setObject(CCString::create("통신이 원활하지 않습니다. 잠시후 다시 시도해주세요."), kMyLocalKey_couponOtherError);
	
	ko->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle);
	ko->setObject(CCString::create("<font newline=16>별이 부족해요.<font newline=16>더 높은 등급으로 클리어해서<font color=961 newline=16>별 %d개 모아주세요."), kMyLocalKey_notOpenPuzzleNotEnoughtStarContent);
	
	ko->setObject(CCString::create("대전 준비"), kMyLocalKey_endlessStartSettingTitle);
	
	ko->setObject(CCString::create("영역점수"), kMyLocalKey_endlessCalcTitleAreaScore);
	ko->setObject(CCString::create("공격점수"), kMyLocalKey_endlessCalcTitleDamageScore);
	ko->setObject(CCString::create("콤보점수"), kMyLocalKey_endlessCalcTitleComboScore);
	ko->setObject(CCString::create("생명점수"), kMyLocalKey_endlessCalcTitleLifeScore);
	ko->setObject(CCString::create("시간점수"), kMyLocalKey_endlessCalcTitleTimeScore);
	ko->setObject(CCString::create("등급점수"), kMyLocalKey_endlessCalcTitleGradeScore);
	ko->setObject(CCString::create("획득골드"), kMyLocalKey_endlessCalcTitleTakeGold);
	ko->setObject(CCString::create("획득영역"), kMyLocalKey_endlessCalcTitleTakeArea);
	ko->setObject(CCString::create("피격점수"), kMyLocalKey_endlessCalcTitleDamagedScore);
	
	ko->setObject(CCString::create("승"), kMyLocalKey_win);
	ko->setObject(CCString::create("게임결과"), kMyLocalKey_gameResult);
	ko->setObject(CCString::create("실패"), kMyLocalKey_fail);
	ko->setObject(CCString::create("달성!!"), kMyLocalKey_achieve);
	ko->setObject(CCString::create("닫기"), kMyLocalKey_close);
	
	ko->setObject(CCString::create("%d연승중"), kMyLocalKey_endlessIngWin);
	
	ko->setObject(CCString::create("PVP 오픈조건"), kMyLocalKey_endlessOpenConditionTitle);
	ko->setObject(CCString::create("<font color=961 newline=16>PVP는 %d스테이지 플레이 후<font newline=16>자동으로 오픈됩니다."), kMyLocalKey_endlessOpenConditionContent);
	
	ko->setObject(CCString::create("대전배패"), kMyLocalKey_endlessKeepWin);
	
	ko->setObject(CCString::create("연승저장"), kMyLocalKey_endlessKeepWinTitle);
	ko->setObject(CCString::create("연승 정보는 유지됩니다."), kMyLocalKey_endlessKeepWinContent);
	 
	ko->setObject(CCString::create("%d연승 도전"), kMyLocalKey_endlessReadyIngWin);
	ko->setObject(CCString::create("닉네임 오류"), kMyLocalKey_nicknameError);
	ko->setObject(CCString::create("<font color=#FFFFFF>남은 판수 </font>"
																								"<font color=#FFAA14 size=14>%d회 / </font>"
																								"<font color=#FFFFFF>성공시 </font>"
																								"<font color=#FFAA14>%.1f%% 획득</font>"), kMyLocalKey_onePercentDesc2);
	
	ko->setObject(CCString::create("대전상대를 찾는 중입니다."), kMyLocalKey_rivalSearch);
	
	ko->setObject(CCString::create("%d레벨"), kMyLocalKey_rivalLevel);
	ko->setObject(CCString::create("%d 연승 중"), kMyLocalKey_rivalIngWin);
	ko->setObject(CCString::create("대전상대를 선택해주세요."), kMyLocalKey_rivalSelect);
	
	ko->setObject(CCString::create("업적완료"), kMyLocalKey_achieveSuccess2);
	
	ko->setObject(CCString::create("소녀들의 위치를 파악하고 있습니다."), kMyLocalKey_titleTempScript1);
	ko->setObject(CCString::create("소녀들의 의상을 점검하고 있습니다."), kMyLocalKey_titleTempScript2);
	ko->setObject(CCString::create("소녀들을 불러모으고 있습니다."), kMyLocalKey_titleTempScript3);
	ko->setObject(CCString::create("하트 공짜"), kMyLocalKey_heartFree);
	
	ko->setObject(CCString::create("지난주 랭킹 결과"), kMyLocalKey_rankRewardTitle);
	ko->setObject(CCString::create("보상선물들은 선물함에서 확인하세요."), kMyLocalKey_rankRewardContent);
	ko->setObject(CCString::create("주간 누적 랭킹"), kMyLocalKey_rankRewardStageTitle);
	ko->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardStageScore);
	ko->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardStageRank);
	ko->setObject(CCString::create("지난주 플레이횟수"), kMyLocalKey_rankRewardStagePlayCount);
	ko->setObject(CCString::create("PVP 랭킹"), kMyLocalKey_rankRewardEndlessTitle);
	ko->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardEndlessScore);
	ko->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardEndlessRank);
	ko->setObject(CCString::create("지난주 연승기록"), kMyLocalKey_rankRewardEndlessVictory);
	ko->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardStageReward);
	ko->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardEndlessReward);
	
	ko->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardStageRankValue);
	ko->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardEndlessRankValue);
	
	ko->setObject(CCString::create("%s연승"), kMyLocalKey_rankRewardEndlessVictoryScript);
	ko->setObject(CCString::create("%d위"), kMyLocalKey_rankRewardRankValue);
	
	ko->setObject(CCString::create("%s골드"), kMyLocalKey_rankRewardGoodsTypeGold);
	ko->setObject(CCString::create("%s잼"), kMyLocalKey_rankRewardGoodsTypeRuby);
	ko->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem9);
	ko->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem6);
	ko->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem11);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass1);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass2);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass3);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass4);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass5);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass6);
	ko->setObject(CCString::create("선물상자"), kMyLocalKey_rankRewardGoodsTypeMany);
	
	ko->setObject(CCString::create("카드 승급 이벤트!"), kMyLocalKey_rankUpEventTitle);
	ko->setObject(CCString::create("승급 비용 공짜! 승급 확률 100%!!"), kMyLocalKey_rankUpEventContent);
	
	ko->setObject(CCString::create("아니오"), kMyLocalKey_no);
	ko->setObject(CCString::create("네"), kMyLocalKey_yes);
	ko->setObject(CCString::create("<font>기본 클리어조건 <font color=961 newline=18>%.0f%%를<font color=961>%.0f%%로 하향<font newline=18> 시켜드립니다.<font newline=18>적용하시겠습니까?"), kMyLocalKey_clearTimeEventMent);
	ko->setObject(CCString::create("이벤트"), kMyLocalKey_clearTimeEventTitle);
	
	ko->setObject(CCString::create("숨겨진 업적"), kMyLocalKey_achieveHiddenTitle);
	ko->setObject(CCString::create("저를 찾아주세요! 곳곳에 숨겨져 있어요!"), kMyLocalKey_achieveHiddenContent);
	
	ko->setObject(CCString::create("<font newline=20>액자를 선택하면<font color=961 newline=20>퍼즐화면으로 이동합니다."), kMyLocalKey_mainflowDimmed1);
	ko->setObject(CCString::create("<font newline=20>각 스테이지에서 획득한<font color=961>별의 총 갯수<font newline=20>입니다."), kMyLocalKey_mainflowDimmed2);
	ko->setObject(CCString::create("<font newline=20>선물함"), kMyLocalKey_mainflowDimmed3);
	ko->setObject(CCString::create("<font newline=20>업적"), kMyLocalKey_mainflowDimmed4);
	ko->setObject(CCString::create("<font newline=20>설정"), kMyLocalKey_mainflowDimmed5);
	ko->setObject(CCString::create("<font newline=20>이벤트"), kMyLocalKey_mainflowDimmed6);
	ko->setObject(CCString::create("<font newline=20>랭킹"), kMyLocalKey_mainflowDimmed7);
	ko->setObject(CCString::create("<font newline=20>상점"), kMyLocalKey_mainflowDimmed8);
	ko->setObject(CCString::create("<font newline=20>카드"), kMyLocalKey_mainflowDimmed9);
	ko->setObject(CCString::create("<font size=13 newline=16>오늘의<font size=13 newline=16>미션"), kMyLocalKey_mainflowDimmed10);
	
	ko->setObject(CCString::create("모든 피스 획득!"), kMyLocalKey_puzzleClearCardTitle);
	ko->setObject(CCString::create("<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzleClearCardContent);
	ko->setObject(CCString::create("모든 카드 획득!"), kMyLocalKey_puzzlePerfectCardTitle);
	ko->setObject(CCString::create("<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzlePerfectCardContent);
	
	ko->setObject(CCString::create("%d연승 돌파!"), kMyLocalKey_endlessSeqWinRewardTitle);
	ko->setObject(CCString::create("%d연승을 축하드립니다!"), kMyLocalKey_endlessSeqWinRewardContent);
	ko->setObject(CCString::create("<font newline=18>%d골드를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeGold);
	ko->setObject(CCString::create("<font newline=18>%d잼을 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby);
	ko->setObject(CCString::create("<font newline=18>기본속도업아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9);
	ko->setObject(CCString::create("<font newline=18>더블아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6);
	ko->setObject(CCString::create("<font newline=18>자석아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11);
	ko->setObject(CCString::create("<font newline=18>이어하기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass1);
	ko->setObject(CCString::create("<font newline=18>맵가챠 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass2);
	ko->setObject(CCString::create("<font newline=18>업그레이드 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass3);
	ko->setObject(CCString::create("<font newline=18>아이템뽑기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass4);
	ko->setObject(CCString::create("<font newline=18>1프로찬스 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass5);
	ko->setObject(CCString::create("<font newline=18>생명의 돌을 %d개를 드립니다.<font newline=18>생명의 돌은 내카드 다이어리에서 사용하세요."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass6);
	ko->setObject(CCString::create("<font newline=18>선물상자를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeMany);
	
	ko->setObject(CCString::create("퍼즐에서 획득한 카드입니다."), kMyLocalKey_cardSettingMyCardContent);
	ko->setObject(CCString::create("스페셜 카드 보기"), kMyLocalKey_cardSettingMyCardToSpecial);
	
	ko->setObject(CCString::create("모든 피스\n획득시!"), kMyLocalKey_cardSettingClearCardMent);
	ko->setObject(CCString::create("퍼펙트\n클리어시!"), kMyLocalKey_cardSettingPerfectCardMent);
	
	ko->setObject(CCString::create("스페셜 카드"), kMyLocalKey_cardSettingSpecialCardTitle);
	ko->setObject(CCString::create("이벤트를 통해 얻을 수 있는 카드입니다."), kMyLocalKey_cardSettingSpecialCardContent);
	ko->setObject(CCString::create("내 카드 보기"), kMyLocalKey_cardSettingSpecialCardToMyCard);
	
	ko->setObject(CCString::create("<font newline=16>PVP모드가<font newline=16>오픈되었어요!"), kMyLocalKey_endlessTutorialMent1);
	ko->setObject(CCString::create("<font newline=16>내 정보 입니다."), kMyLocalKey_endlessTutorialMent2);
	ko->setObject(CCString::create("<font>목록에서 <font color=961>다른 친구를 선택<font newline=16>해보세요.<font newline=16>다른 친구의 정보를 볼 수 있어요."), kMyLocalKey_endlessTutorialMent3);
	ko->setObject(CCString::create("<font color=961>대전준비를 눌러보세요."), kMyLocalKey_endlessTutorialMent4);
	ko->setObject(CCString::create("<font color=961>대전시작을 눌러보세요."), kMyLocalKey_endlessTutorialMent5);
	ko->setObject(CCString::create("<font color=961>내 미니맵"), kMyLocalKey_endlessTutorialMent6);
	ko->setObject(CCString::create("<font color=961>내 점수"), kMyLocalKey_endlessTutorialMent7);
	ko->setObject(CCString::create("<font color=961>내 장착아이템"), kMyLocalKey_endlessTutorialMent8);
	ko->setObject(CCString::create("<font color=961>상대 정보"), kMyLocalKey_endlessTutorialMent9);
	ko->setObject(CCString::create("<font color=961 newline=16>5콤보 달성시 마다<font color=961 newline=16>미사일을 발사하여<font color=961 newline=16>상대의 점수를 깍습니다."), kMyLocalKey_endlessTutorialMent10);
	ko->setObject(CCString::create("화면을 터치하면 게임 진행됩니다."), kMyLocalKey_endlessTutorialMent11);
	ko->setObject(CCString::create("<font color=961>나의 결과"), kMyLocalKey_endlessTutorialMent12);
	ko->setObject(CCString::create("<font color=961>상대방 결과"), kMyLocalKey_endlessTutorialMent13);
	ko->setObject(CCString::create("<font>스크롤하면 점수 내역을 볼 수 있어요."), kMyLocalKey_endlessTutorialMent14);
	ko->setObject(CCString::create("<font color=961>2연승에 도전할 수 있어요!"), kMyLocalKey_endlessTutorialMent15);
	ko->setObject(CCString::create("잠깐!"), kMyLocalKey_endlessTutorialMent16);
	ko->setObject(CCString::create("<font newline=16>연승에 도전하시면<font newline=20>생명의 돌을 받을 수 있어요.<font newline=16>튜토리얼을 마치겠습니다.(--)(__)"), kMyLocalKey_endlessTutorialMent17);
	ko->setObject(CCString::create("<font>대전 상대를 선택해주세요."), kMyLocalKey_endlessTutorialMent18);
	
	ko->setObject(CCString::create("%d 퍼즐"), kMyLocalKey_puzzleValue);
	
	ko->setObject(CCString::create("보너스 영역 뽑기"), kMyLocalKey_onePercentTutorial1);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아슬아슬하게 영역을 100% 획득을 못했군요!</font>"
								   "<font color=#FFAA14 size=12 newline=30>보너스 영역 뽑기에 도전해보세요!</font>"
								   "<font color=#FFAA14 size=14 newline=18>100% 영역 획득 성공시</font>"
								   "<font color=#FFAA14 size=14>두 단계 위 등급 카드를 획득</font>"
								   "<font color=#FFFFFF size=14 newline=18>할 수 있어요!</font>"), kMyLocalKey_onePercentTutorial2);
	ko->setObject(CCString::create("다  음"), kMyLocalKey_onePercentTutorial3);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아래 게이지바에서 분홍색 영역을 맞히면</font>"
								   "<font color=#FFAA14 size=12 newline=16>0.3%를 추가로 획득할 수 있습니다.</font>"
								   "<font color=#FFFFFF size=12 newline=30>기회는 총 3번 주어집니다.</font>"
								   "<font color=#FFAA14 size=14 newline=15>자 그럼 100% 에 도전해 볼까요?</font>"), kMyLocalKey_onePercentTutorial4);
	
	ko->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>뽑기 기회는 단 3번!!</font><font color=#FFFFFF size=12>부족한 영역을 채워 100%에 도전하세요.</font>"), kMyLocalKey_onePercentGame1);
	ko->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>100%에 성공시</font><font color=#FFAA14 size=12 tag=1>한 단계 높은 카드</font><font color=#FFFFFF size=12>를 획득할 수 있어요.</font>"), kMyLocalKey_onePercentGame2);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12>현재 획득 영역       </font><font color=#FFFF00 size=30 strokecolor=000 strokesize=2 tag=1>99.9%</font>"), kMyLocalKey_onePercentGame3);
	ko->setObject(CCString::create("<font size=12 color=#FFFFFF newline=16>분홍색 영역에 맞게</font>"
								   "<font size=12 color=#FFAA14>멈춰 버튼 터치</font><font size=12 color=#FFFFFF>하세요!</font>"), kMyLocalKey_onePercentGame4);
	ko->setObject(CCString::create("멈춰!"), kMyLocalKey_onePercentGame5);
	ko->setObject(CCString::create("100% 실패"), kMyLocalKey_onePercentGame6);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12>최종 획득 영역     </font>"
								   "<font color=#FFAA14 size=30>%.1f%%</font>"), kMyLocalKey_onePercentGame7);
	ko->setObject(CCString::create("닫기"), kMyLocalKey_onePercentGame8);
	ko->setObject(CCString::create("100% 달성!!"), kMyLocalKey_onePercentGame9);
	ko->setObject(CCString::create("카드 받기"), kMyLocalKey_onePercentGame10);
	
	ko->setObject(CCString::create("보상내용"), kMyLocalKey_endlessRewardInfo);
	
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>가위 바위 보 중</font>"
								   "<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>"), kMyLocalKey_gababoContent1);
	ko->setObject(CCString::create("<font size=12 newline=13>연</font>"
								   "<font size=12 newline=13>승</font>"
								   "<font size=12 newline=13>보</font>"
								   "<font size=12 newline=13>상</font>"), kMyLocalKey_gababoContent2);
	ko->setObject(CCString::create("보상"), kMyLocalKey_gababoContent3);
	ko->setObject(CCString::create("1승"), kMyLocalKey_gababoContent4);
	ko->setObject(CCString::create("2승"), kMyLocalKey_gababoContent5);
	ko->setObject(CCString::create("3승"), kMyLocalKey_gababoContent6);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent7);
	ko->setObject(CCString::create("확 인"), kMyLocalKey_gababoContent8);
	ko->setObject(CCString::create("나"), kMyLocalKey_gababoContent9);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런, 무승부네요!</font>"
								   "<font color=#FFAA14 size=12>한번 더!</font>"), kMyLocalKey_gababoContent10);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent11);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>세상에, 4판모두 이긴 사람은 당신이 처음이에요.</font>"
								   "<font color=#FFAA14 size=12 newline=14>최고의 보상을 드릴게요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>보너스 게임은 퍼펙트 클리어시마다 할 수 있어요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>그럼 다음에 또 만나요!</font>"), kMyLocalKey_gababoContent12);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런이런... 지고 말았네요. 너무 상심마세요!</font>"
								   "<font color=#FFAA14 size=12>보너스게임은 퍼펙트 클리어시마다 할 수 있어요!</font>"), kMyLocalKey_gababoContent13);
	ko->setObject(CCString::create("루우비~!"), kMyLocalKey_gababoContent14);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>보너스 게임에 오신 것을 환영합니다!</font>"
								   "<font color=#FFFFFF size=12 newline=14>벌써 퍼즐을 퍼펙트 클리어 하시다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12>게임 설명은 최초 1회만 해드리니 잘 봐주세요!</font>"), kMyLocalKey_gababoContent15);
	ko->setObject(CCString::create("다 음"), kMyLocalKey_gababoContent16);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>게임은 가위바위보 입니다!</font>"
								   "<font color=#FFAA14 size=12 newline=14>가위바위보 중 하나를 선택하여 저를 이기시면 됩니다.</font>"
								   "<font color=#FFAA14 size=12 newline=14>연승 횟수에 따라 더 큰 보상이 있어요!</font>"
								   "<font color=#FFFFFF size=12 newline=14>그럼 행운을 빌어요!</font>"), kMyLocalKey_gababoContent17);
	
	ko->setObject(CCString::create("x%d"), kMyLocalKey_gababoReward);
	
	ko->setObject(CCString::create("PVP 튜토리얼 중에는 나가실 수 없습니다."), kMyLocalKey_notGohomeEndlessTutorialMent);
	ko->setObject(CCString::create("PVP 중에는 재시도할 수 없습니다."), kMyLocalKey_notReplayEndlessMent);
	
	ko->setObject(CCString::create("대중교통 모드"), kMyLocalKey_optionSecretTitle);
	ko->setObject(CCString::create("<font newline=16>대중교통모드가 적용중입니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 해제하실 수 있습니다."), kMyLocalKey_optionSecretOn);
	ko->setObject(CCString::create("<font newline=16>대중교통모드가 풀렸습니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 적용하실 수 있습니다."), kMyLocalKey_optionSecretOff);
	
	ko->setObject(CCString::create("카드 선물하기"), kMyLocalKey_cardGiftTitle);
	ko->setObject(CCString::create("<font color=999>내 아이디는 <font color=961>%s<font color=999> 입니다."), kMyLocalKey_cardGiftMyID);
	ko->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_cardGiftInputText);
	ko->setObject(CCString::create("검색"), kMyLocalKey_cardGiftSearch);
	ko->setObject(CCString::create("선물하기"), kMyLocalKey_cardGiftSend);
	ko->setObject(CCString::create("검색결과가 없습니다."), kMyLocalKey_cardGiftNotFound);
	
	ko->setObject(CCString::create("<font color=999>실패시 다음번 승급 성공확률이 <font color=961>%.0f%%<font color=999> 올라갑니다."), kMyLocalKey_rankUpFailRateUp);
	
	ko->setObject(CCString::create("타임 오버"), kMyLocalKey_failTitleTimeover);
	ko->setObject(CCString::create("미션 실패"), kMyLocalKey_failTitleMissionfail);
	ko->setObject(CCString::create("게임 오버"), kMyLocalKey_failTitleGameover);
	ko->setObject(CCString::create("스테이지 클리어"), kMyLocalKey_clearTitle);
	
	ko->setObject(CCString::create("모든 퍼즐 클리어"), kMyLocalKey_allPuzzleClearTitle);
	ko->setObject(CCString::create("<font newline=15>마지막 퍼즐을 클리어하셨습니다.<font newline=15>다음 퍼즐은 곧 업데이트 됩니다.<font newline=15>아직 획득하지 못한 카드들을<font>모두 모아보세요."), kMyLocalKey_allPuzzleClearMent);
	
	ko->setObject(CCString::create("주의!"), kMyLocalKey_warningDie);
	
//	ko->setObject(CCString::create("이번엔 획득한 영역 위를 이동하는\n방법을 알려드리겠습니다."), kMyLocalKey_tutorial8);
//	ko->setObject(CCString::create("획득한 영역의 외곽선 방향으로\n제스쳐를 하면 해당 방향으로\n외곽선을 따라 이동하게 됩니다."), kMyLocalKey_tutorial9);
//	ko->setObject(CCString::create("왼쪽으로 제스쳐를 해보세요."), kMyLocalKey_tutorial10);
//	ko->setObject(CCString::create("이동중에 멈추려면 화면을\n살짝 터치하시면 됩니다."), kMyLocalKey_tutorial11);
//	ko->setObject(CCString::create("화면을 터치 해보세요."), kMyLocalKey_tutorial12);
//	ko->setObject(CCString::create("이제 인터페이스에 대한\n설명을 하겠습니다."), kMyLocalKey_tutorial14);
//	ko->setObject(CCString::create("챕터를 결정하는 화면입니다."), kMyLocalKey_tutorial15);
//	ko->setObject(CCString::create("표시된 부분이 챕터 입니다.\n하나의 챕터에는 5개의 스테이지가 있습니다.\n스테이지를 모두 클리어 하면\n다음 챕터로 넘어갈 수 있습니다."), kMyLocalKey_tutorial16);
//	ko->setObject(CCString::create("아래는 페이스북에\n로그인 하는 부분입니다."), kMyLocalKey_tutorial17);
//	ko->setObject(CCString::create("로그인 하시면 친구들의\n점수를 볼 수 있고, 친구와\n아이템을 주고 받을 수 있습니다."), kMyLocalKey_tutorial17_1);
//	ko->setObject(CCString::create("챕터를 선택하시면\n지금과 같은 화면을 볼수 있습니다.\n여기서는 스테이지와 속성, 아이템을\n선택할 수 있습니다."), kMyLocalKey_tutorial18);
//	ko->setObject(CCString::create("표시된 부분이 스테이지 입니다.\n클리어한 스테이지도 다시 선택해서\n플레이할 수 있습니다."), kMyLocalKey_tutorial19);
//	ko->setObject(CCString::create("현재 선택된 스테이지의\n적들 모습과 속성을 보여줍니다."), kMyLocalKey_tutorial20);
//	ko->setObject(CCString::create("현재 선택된 속성입니다."), kMyLocalKey_tutorial21);
//	ko->setObject(CCString::create("속성을 선택할 수 있으며 골드로\n구매 및 업그레이드할 수 있습니다.\n골드는 게임중에 얻을 수 있습니다."), kMyLocalKey_tutorial22);
//	ko->setObject(CCString::create("현재 선택된 아이템 입니다."), kMyLocalKey_tutorial23);
//	ko->setObject(CCString::create("아이템을 선택할 수 있으며\n골드로 구매할 수 있습니다."), kMyLocalKey_tutorial24);
//	ko->setObject(CCString::create("마지막으로 게임을 시작하는\n버튼 입니다. 게임을 하기 위해서는\n붓이 필요합니다."), kMyLocalKey_tutorial25);
//	ko->setObject(CCString::create("튜토리얼을 진행하느라 수고하셨습니다.\n튜토리얼을 종료하고\n챕터를 설정하는 곳으로 갑니다."), kMyLocalKey_tutorial26);
//	
//	ko->setObject(CCString::create("각 챕터의 마지막 스테이지엔 특별한 이미지가 나옵니다."), kMyLocalKey_loading1);
//	ko->setObject(CCString::create("얇게 그리고 싶다면 이동중에 반대방향으로 제스쳐 해보세요."), kMyLocalKey_loading2);
//	ko->setObject(CCString::create("모서리부터 그리려면 모서리에 가까워졌을때 해당 방향으로 한번 더 제스쳐 하세요."), kMyLocalKey_loading3);
//	ko->setObject(CCString::create("페이스북 로그인을 하시면 친구들의 점수를 볼수 있습니다."), kMyLocalKey_loading4);
//	ko->setObject(CCString::create("친구에게 (초대or함께)하기 를 보내면 붓을 얻을 수 있습니다. 한 사람에게 하루에 한번씩 보낼 수 있습니다."), kMyLocalKey_loading5);
//	ko->setObject(CCString::create("친구들이 내게 보낸 붓들은 여기서 확인할 수 있습니다."), kMyLocalKey_loading6);
//	ko->setObject(CCString::create("공격조건완화 : 영역획득시 더 많은 공격을 합니다."), kMyLocalKey_loading11);
//	ko->setObject(CCString::create("시작영역증가 : 더 넓은 영역으로 게임을 시작할 수 있습니다."), kMyLocalKey_loading12);
//	ko->setObject(CCString::create("상태이상방어 : 보스의 공격을 방어합니다. 방어되는 공격 : 기절, 수면, 빙결, 혼란"), kMyLocalKey_loading13);
//	ko->setObject(CCString::create("속도증가 : 아이템을 획득시 이동속도가 5초 동안 두배로 빨라집니다."), kMyLocalKey_loading14);
//	ko->setObject(CCString::create("침묵 : 아이템을 획득시 20초 동안 보스가 공격을 못하도록 합니다."), kMyLocalKey_loading15);
//	ko->setObject(CCString::create("속도감소 : 아이템을 획득시 20초 동안 적들의 이동속도를 절반으로 낮춥니다."), kMyLocalKey_loading16);
//	ko->setObject(CCString::create("속성을 선택하면 해당 속성에 맞는 공격 스킬이 게임중에 나옵니다."), kMyLocalKey_loading17);
//	ko->setObject(CCString::create("속성상관관계"), kMyLocalKey_loading19);
//	ko->setObject(CCString::create("지금 페이스북에 로그인하면 BRUSH 5개를 더 드립니다."), kMyLocalKey_loading20);
//	
//	ko->setObject(CCString::create("공격조건완화"), kMyLocalKey_item1);
//	ko->setObject(CCString::create("시작영역증가"), kMyLocalKey_item2);
//	ko->setObject(CCString::create("상태이상방어"), kMyLocalKey_item3);
//	ko->setObject(CCString::create("속도증가"), kMyLocalKey_item4);
//	ko->setObject(CCString::create("침묵"), kMyLocalKey_item5);
//	ko->setObject(CCString::create("속도감소"), kMyLocalKey_item6);
//	
//	ko->setObject(CCString::create("선택된 아이템이 없습니다."), kMyLocalKey_notSelectedItem);
//	ko->setObject(CCString::create("이미 선택된 아이템을 구매하셨습니다."), kMyLocalKey_boughtItem);
//	ko->setObject(CCString::create("한번의 게임에 사용할 수 있는 아이템 종류는 최대 2가지 입니다."), kMyLocalKey_multiSelect);
//	ko->setObject(CCString::create("골드가 부족하여 구매할 수 없습니다."), kMyLocalKey_notEnoughGold);
//	ko->setObject(CCString::create("상점으로 가시겠습니까?"), kMyLocalKey_goShop);
//	ko->setObject(CCString::create("붓이 부족하여 게임을 시작할 수 없습니다."), kMyLocalKey_notEnoughBrush);
//	ko->setObject(CCString::create("잼이 부족하여 구매할 수 없습니다."), kMyLocalKey_notEnoughRuby);
//	
//	ko->setObject(CCString::create("리뷰를 남겨주시면 붓을 가득 채워드립니다."), kMyLocalKey_review);
//	ko->setObject(CCString::create("갤러리에 등록 되었습니다. 갤러리로 가시겠습니까?"), kMyLocalKey_regiGallery);
//	ko->setObject(CCString::create("어려운 챕터들이 공개되었습니다."), kMyLocalKey_openHard);
//	ko->setObject(CCString::create("악의 기운이 가득하여 방어막이 해제됩니다."), kMyLocalKey_shieldCancel);
//	ko->setObject(CCString::create("게임 실행 후 보상을 받을 수 있습니다."), kMyLocalKey_exeAppReward);
//	ko->setObject(CCString::create("GIFTCODE 는 한시간에 하나만 사용할 수 있습니다."), kMyLocalKey_giftAHour);
//	ko->setObject(CCString::create("올바르지 않은 GIFTCODE 입니다."), kMyLocalKey_giftInvalid);
//	ko->setObject(CCString::create("이미 사용한 GIFTCODE 입니다."), kMyLocalKey_giftUsed);
//	ko->setObject(CCString::create("GIFTCODE는 LitQoo에서 발행합니다."), kMyLocalKey_giftLitqoo);
//	ko->setObject(CCString::create("게임을 종료하고 홈으로 이동하시겠습니까?"), kMyLocalKey_goHome);
//	ko->setObject(CCString::create("별3개로 클리어 하면 갤러리에 그림이 등록됩니다."), kMyLocalKey_star3clear);
//	ko->setObject(CCString::create("아이템을 사용할 수 있습니다."), kMyLocalKey_itemEnable);
//	ko->setObject(CCString::create("최신버전으로 업데이트를 해야 플레이가 가능합니다."), kMyLocalKey_needUpdate);
//	ko->setObject(CCString::create("최신버전이 출시되었습니다. 업데이트 하시겠습니까?"), kMyLocalKey_newVersion);
//	ko->setObject(CCString::create("지금 같이 해요!"), kMyLocalKey_facebookInvite);
//	ko->setObject(CCString::create("%s가 BRUSH를 보냈습니다."), kMyLocalKey_facebookWith);
//	
//	ko->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	ko->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	ko->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	ko->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	ko->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	ko->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	ko->setObject(CCString::create("몬스터를 획득하셨습니다."), kMyLocalKey_getMonster);
//	
//	ko->setObject(CCString::create("수집한 이미지, 펫, 몬스터의 정보는 컬렉션에서 확인 할 수 있습니다."), kMyLocalKey_loading21);
//	ko->setObject(CCString::create("수집한 이미지, 펫, 몬스터의 정보는 컬렉션에서 확인 할 수 있습니다."), kMyLocalKey_loading22);
//	ko->setObject(CCString::create("컨트롤 방식은 게임 좌측 화면의 옵션을 통하여 변경 할 수 있습니다."), kMyLocalKey_loading23);
//	ko->setObject(CCString::create("펫은 펫샵에서 구매 할 수 있습니다."), kMyLocalKey_loading24);
//	ko->setObject(CCString::create("펫의 종류에 따라 (게임 중) 여러 가지 버프 효과를 받을 수 있습니다."), kMyLocalKey_loading25);
	setObject(ko, "ko");
}
void MyLocal::en()
{
	
	CCDictionary* en = CCDictionary::create();
	
	en->setObject(CCString::create("Server Online"), kMyLocalKey_connectingServer);
	en->setObject(CCString::create("Nickname enter"), kMyLocalKey_inputNick);
	en->setObject(CCString::create("Please enter."), kMyLocalKey_inputPlease);
	en->setObject(CCString::create("OK"), kMyLocalKey_ok);
	en->setObject(CCString::create("Loading images."), kMyLocalKey_downImgInfo);
	en->setObject(CCString::create("Information Retrieval Failure"), kMyLocalKey_failLoadInfo);
	en->setObject(CCString::create("RE-TRY"), kMyLocalKey_replay);
	en->setObject(CCString::create("Downloading %.0f%% : %d/%d"), kMyLocalKey_downloadingProgress);
	en->setObject(CCString::create("Receive images fail"), kMyLocalKey_downImgFail);
	en->setObject(CCString::create("Login successful"), kMyLocalKey_successLogin);
	en->setObject(CCString::create("This nickname is already in use."), kMyLocalKey_sameNick);
	en->setObject(CCString::create("Nickname error"), kMyLocalKey_invalidNick);
	en->setObject(CCString::create("Nickname is too long."), kMyLocalKey_longNick);
	en->setObject(CCString::create("Nickname is too short."), kMyLocalKey_shortNick);
	en->setObject(CCString::create("특수문자는 사용 할 수 없습니다."), kMyLocalKey_specialCharacterError);
	en->setObject(CCString::create("Now Open"), kMyLocalKey_openRightNow);
	en->setObject(CCString::create("RUBY %d open"), kMyLocalKey_openRubyValue);
	en->setObject(CCString::create("GOLD %d open"), kMyLocalKey_openGoldValue);
	en->setObject(CCString::create("CGP 일반 보상"), kMyLocalKey_cgpNormalTitle);
	en->setObject(CCString::create("Press to receive reward."), kMyLocalKey_cgpNormalContent);
	en->setObject(CCString::create("Receive reward"), kMyLocalKey_getReward);
	en->setObject(CCString::create("take"), kMyLocalKey_take);
	en->setObject(CCString::create("CGP 전체 팝업 보상"), kMyLocalKey_cgpAllPopupTitle);
	en->setObject(CCString::create("Shop"), kMyLocalKey_shop);
	en->setObject(CCString::create("Ranking"), kMyLocalKey_ranking);
	en->setObject(CCString::create("My card"), kMyLocalKey_mycard);
	en->setObject(CCString::create("Today mission"), kMyLocalKey_todaymission);
	en->setObject(CCString::create("Event"), kMyLocalKey_event);
	en->setObject(CCString::create("스페셜 스테이지"), kMyLocalKey_openStage);
	en->setObject(CCString::create("스페셜 스테이지를 오픈하시겠습니까?"), kMyLocalKey_openStageContent);
	en->setObject(CCString::create("오픈하기"), kMyLocalKey_doOpen);
	en->setObject(CCString::create("Open Stage"), kMyLocalKey_stageOpenTitle);
	en->setObject(CCString::create("A new stage\nhas been unlocked"), kMyLocalKey_stageOpenContent);
	en->setObject(CCString::create("Puzzle open"), kMyLocalKey_puzzleOpenTitle);
	en->setObject(CCString::create("A new puzzle\nhas been unlocked."), kMyLocalKey_puzzleOpenContent);
	en->setObject(CCString::create("View"), kMyLocalKey_view);
	en->setObject(CCString::create("Area 85%"), kMyLocalKey_condition1);
	en->setObject(CCString::create("Area 85% + Change"), kMyLocalKey_condition2);
	en->setObject(CCString::create("Area 100%"), kMyLocalKey_condition3);
	en->setObject(CCString::create("Area 100% + Change"), kMyLocalKey_condition4);
	en->setObject(CCString::create("85%"), kMyLocalKey_conditionTwoLine1);
	en->setObject(CCString::create("85%\n+ Change"), kMyLocalKey_conditionTwoLine2);
	en->setObject(CCString::create("100%"), kMyLocalKey_conditionTwoLine3);
	en->setObject(CCString::create("100%\n+ Change"), kMyLocalKey_conditionTwoLine4);
	en->setObject(CCString::create("%d Stage"), kMyLocalKey_stageValue);
	en->setObject(CCString::create("My location %d"), kMyLocalKey_myrankValue);
	en->setObject(CCString::create("Failure Ranking Information"), kMyLocalKey_failCheckRanking);
	en->setObject(CCString::create("Stage"), kMyLocalKey_stage);
	en->setObject(CCString::create("Choose the items that you will use during the game."), kMyLocalKey_selectUseItem);
	en->setObject(CCString::create("Level %d"), kMyLocalKey_levelValue);
	en->setObject(CCString::create("Power %s"), kMyLocalKey_powerValue);
	en->setObject(CCString::create("Level %d Upgrade"), kMyLocalKey_upgradeLevelValue);
	en->setObject(CCString::create("Up to three items\ncan be selected."), kMyLocalKey_totalItemSelectCount3);
	en->setObject(CCString::create("You don’t have enough gold"), kMyLocalKey_goldNotEnought);
	en->setObject(CCString::create("You don’t have enough ruby."), kMyLocalKey_rubyNotEnought);
	en->setObject(CCString::create("You don’t have enough heart."), kMyLocalKey_heartNotEnought);
	en->setObject(CCString::create("Would you like to go buy?"), kMyLocalKey_goToShop);
	en->setObject(CCString::create("The default speed"), kMyLocalKey_baseSpeedUpTitle);
	en->setObject(CCString::create("The user starts with a fast state."), kMyLocalKey_baseSpeedUpContent);
	en->setObject(CCString::create("Double items"), kMyLocalKey_doubleItemTitle);
	en->setObject(CCString::create("Quickly create items."), kMyLocalKey_doubleItemContent);
	en->setObject(CCString::create("자석"), kMyLocalKey_magnetTitle);
	en->setObject(CCString::create("골드와 아이템을 빨아들입니다."), kMyLocalKey_magnetContent);
	en->setObject(CCString::create("Additional time"), kMyLocalKey_longTimeTitle);
	en->setObject(CCString::create("Game time increases."), kMyLocalKey_longTimeContent);
	en->setObject(CCString::create("Items Draw"), kMyLocalKey_itemGachaTitle);
	en->setObject(CCString::create("어떤 아이템이 나올까요?"), kMyLocalKey_itemGachaDefaultContent1);
	en->setObject(CCString::create("뽑기에만 출몰하는 다양한 아이템!!"), kMyLocalKey_itemGachaDefaultContent2);
	en->setObject(CCString::create("Resurrection, the invincible challenge your items!"), kMyLocalKey_itemGachaContent);
	en->setObject(CCString::create("RUBY SHOP"), kMyLocalKey_rubyShop);
	en->setObject(CCString::create("GOLD SHOP"), kMyLocalKey_goldShop);
	en->setObject(CCString::create("HEART SHOP"), kMyLocalKey_heartShop);
	en->setObject(CCString::create("구매확인"), kMyLocalKey_checkBuy);
	en->setObject(CCString::create("Do you want to buy?"), kMyLocalKey_realBuy);
	en->setObject(CCString::create("Option"), kMyLocalKey_option);
	en->setObject(CCString::create("Coupon"), kMyLocalKey_regCoupon);
	en->setObject(CCString::create("Community"), kMyLocalKey_community);
	en->setObject(CCString::create("Gametip"), kMyLocalKey_gametip);
	en->setObject(CCString::create("Tutorial"), kMyLocalKey_tutorial);
	en->setObject(CCString::create("System"), kMyLocalKey_system);
	en->setObject(CCString::create("Background sound"), kMyLocalKey_bgm);
	en->setObject(CCString::create("Sound Effects"), kMyLocalKey_effect);
	en->setObject(CCString::create("Joystick\nposition"), kMyLocalKey_joystickPosition);
	en->setObject(CCString::create("Operation\nJoystick"), kMyLocalKey_joystickControl);
	en->setObject(CCString::create("Safe Mode"), kMyLocalKey_safetyMode);
	en->setObject(CCString::create("Notify"), kMyLocalKey_noti);
	en->setObject(CCString::create("Push"), kMyLocalKey_pushNoti);
	en->setObject(CCString::create("Message\nAlarm"), kMyLocalKey_messageNoti);
	en->setObject(CCString::create("선물을 받았습니다."), kMyLocalKey_getgift);
	en->setObject(CCString::create("받은 메세지가 없습니다."), kMyLocalKey_nogift);
	en->setObject(CCString::create("*30일 이후에 삭제됩니다."), kMyLocalKey_giftboxAlert);
	en->setObject(CCString::create("재접속"), kMyLocalKey_reConnect);
	en->setObject(CCString::create("다른 기기로 연결되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert1);
	en->setObject(CCString::create("서버와의 접속에 오류가 발생하였습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert2);
	en->setObject(CCString::create("세션이 종료되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert3);
	en->setObject(CCString::create("서버연결에 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_reConnectAlert4);
	en->setObject(CCString::create("BLOCKED"), kMyLocalKey_blocked);
	en->setObject(CCString::create("계정이 아래의 사유로 정지 되었습니다."), kMyLocalKey_blockedMsg);
	en->setObject(CCString::create("Member ID : "), kMyLocalKey_memberID);
	en->setObject(CCString::create("Version : "), kMyLocalKey_gameversion);
	en->setObject(CCString::create("Secession"), kMyLocalKey_withdraw);
	en->setObject(CCString::create("Customer Service"), kMyLocalKey_callCenter);
	en->setObject(CCString::create("Are you sure you want to logout?"), kMyLocalKey_checkLogout);
	en->setObject(CCString::create("Cancel"), kMyLocalKey_cancel);
	en->setObject(CCString::create("게임 탈퇴"), kMyLocalKey_withdrawTitle);
	en->setObject(CCString::create("Can not use the room when you leave the game,"), kMyLocalKey_withdrawContent1);
	en->setObject(CCString::create("all data will be deleted permanently."), kMyLocalKey_withdrawContent2);
	en->setObject(CCString::create("Are you sure you want to really leave?"), kMyLocalKey_withdrawContent3);
	en->setObject(CCString::create("탈퇴하기"), kMyLocalKey_doWithdraw);
	en->setObject(CCString::create("Off"), kMyLocalKey_lightOff);
	en->setObject(CCString::create("On"), kMyLocalKey_lightOn);
	en->setObject(CCString::create("Right"), kMyLocalKey_right);
	en->setObject(CCString::create("Left"), kMyLocalKey_left);
	en->setObject(CCString::create("Fixing"), kMyLocalKey_fix);
	en->setObject(CCString::create("Movement"), kMyLocalKey_move);
	en->setObject(CCString::create("Weekly cumulative ranking"), kMyLocalKey_weeklyranking);
	en->setObject(CCString::create("%ddays after the reset"), kMyLocalKey_dayAfterReset);
	en->setObject(CCString::create("%dtimes after the reset"), kMyLocalKey_hourAfterReset);
	en->setObject(CCString::create("%dMinutes after the reset"), kMyLocalKey_secondAfterReset);
	en->setObject(CCString::create("Gift Box"), kMyLocalKey_giftbox);
	en->setObject(CCString::create("Accept all"), kMyLocalKey_allAccept);
	en->setObject(CCString::create("티켓함"), kMyLocalKey_ticketBox);
	en->setObject(CCString::create("도움함"), kMyLocalKey_helpBox);
	en->setObject(CCString::create("도전함"), kMyLocalKey_challengeBox);
	en->setObject(CCString::create("하트함"), kMyLocalKey_heartBox);
	en->setObject(CCString::create("전체보기"), kMyLocalKey_allView);
	en->setObject(CCString::create("하트가 도착했어요."), kMyLocalKey_arriveHeart);
	en->setObject(CCString::create("도전장이 도착했어요."), kMyLocalKey_arriveChallenge);
	en->setObject(CCString::create("도전을 받아들이시겠습니까?"), kMyLocalKey_checkAcceptChallenge);
	en->setObject(CCString::create("도망가기"), kMyLocalKey_ttt);
	en->setObject(CCString::create("도전에 임하는 자세를 가집시다!!\n도망으로 간주, 패하셨습니다."), kMyLocalKey_tttContent);
	en->setObject(CCString::create("도전결과!!"), kMyLocalKey_resultChallenge);
	en->setObject(CCString::create("졌습니다. 브레이크 포인트 필요함. 위에 contentObj"), kMyLocalKey_loseContent);
	en->setObject(CCString::create("도움요청이 도착했어요."), kMyLocalKey_arriveHelp);
	en->setObject(CCString::create("상대방을 도와줍니다."), kMyLocalKey_acceptHelp);
	en->setObject(CCString::create("거절"), kMyLocalKey_deny);
	en->setObject(CCString::create("수락"), kMyLocalKey_accept);
	en->setObject(CCString::create("상대방의 도움이 왔어요!"), kMyLocalKey_arriveHelped);
	en->setObject(CCString::create("카드 정보 로딩"), kMyLocalKey_cardInfoLoading);
	en->setObject(CCString::create("도움을 받았습니다!!"), kMyLocalKey_iHelped);
	en->setObject(CCString::create("티켓요청이 도착했어요."), kMyLocalKey_arriveNeedTicket);
	en->setObject(CCString::create("티켓요청이 도착. 티켓은 퍼즐을 열 때 필요합니다. 친구를 도와주세요!!"), kMyLocalKey_arriveNeedTicketContent);
	en->setObject(CCString::create("보내기"), kMyLocalKey_send);
	en->setObject(CCString::create("티켓이 왔네요 어서 받으세요."), kMyLocalKey_arriveTicket);
	en->setObject(CCString::create("티켓이 도착했습니다. 티켓을 받으세요."), kMyLocalKey_arriveTicketContent);
	en->setObject(CCString::create("티켓받기"), kMyLocalKey_takeTicket);
	en->setObject(CCString::create("친구추가 요청이 왔습니다."), kMyLocalKey_arriveAddFriend);
	en->setObject(CCString::create("An error has occurred."), kMyLocalKey_whatError);
	en->setObject(CCString::create("I can not find the request arrived."), kMyLocalKey_arriveWhatError);
	en->setObject(CCString::create("Default"), kMyLocalKey_defaultSort);
	en->setObject(CCString::create("Take"), kMyLocalKey_takeOrder);
	en->setObject(CCString::create("Rank"), kMyLocalKey_gradeOrder);
	en->setObject(CCString::create("Rotation"), kMyLocalKey_rotation);
	en->setObject(CCString::create("Raising levels to defeat the boss can do it more easily!"), kMyLocalKey_upgradeSubMent);
	en->setObject(CCString::create("Draw again"), kMyLocalKey_itemRegacha);
	en->setObject(CCString::create("%.0f%% Discount"), kMyLocalKey_itemRegachaDiscountValue);
	en->setObject(CCString::create("Use of this item"), kMyLocalKey_thisItemUse);
	en->setObject(CCString::create("Time"), kMyLocalKey_time);
	en->setObject(CCString::create("Gold"), kMyLocalKey_gold);
	en->setObject(CCString::create("Score"), kMyLocalKey_score);
	en->setObject(CCString::create("총점"), kMyLocalKey_totalScore);
	en->setObject(CCString::create("The next stage"), kMyLocalKey_nextStage);
	en->setObject(CCString::create("AGAIN TO"), kMyLocalKey_regame);
	en->setObject(CCString::create("MAIN"), kMyLocalKey_toMain);
	en->setObject(CCString::create("Please clear\nthe previous stage."), kMyLocalKey_beforeNotClearPuzzle);
	en->setObject(CCString::create("이전 액자를\n모두 클리어하면\n다음액자가 나타납니다."), kMyLocalKey_waitForUpdate);
	en->setObject(CCString::create("I'll wait!\n My collection\nof pictures of it!"), kMyLocalKey_diaryNoImg);
	en->setObject(CCString::create("Reward"), kMyLocalKey_reward);
	en->setObject(CCString::create("Speed"), kMyLocalKey_speed);
	en->setObject(CCString::create("READY"), kMyLocalKey_ready);
	en->setObject(CCString::create("상대찾기"), kMyLocalKey_endlessReady);
	en->setObject(CCString::create("God"), kMyLocalKey_rankA);
	en->setObject(CCString::create("Hero"), kMyLocalKey_rankB);
	en->setObject(CCString::create("Normal"), kMyLocalKey_rankC);
	en->setObject(CCString::create("Monkey"), kMyLocalKey_rankD);
	en->setObject(CCString::create("Buy"), kMyLocalKey_buy);
	en->setObject(CCString::create("Push"), kMyLocalKey_controlClick);
	en->setObject(CCString::create("Locked"), kMyLocalKey_locked);
	en->setObject(CCString::create("Please select the box."), kMyLocalKey_selectClearBox);
	
	en->setObject(CCString::create(""), kMyLocalKey_titleLoadingBegin);
	en->setObject(CCString::create("각 스테이지 별로 각기 다른 4장의 이미지를 획득해 보세요."), kMyLocalKey_titleLoading1);
	en->setObject(CCString::create("공공장소나 대중교통 이용시에는 '대중교통 모드'로 Play 하세요. 획득한 이미지는 내 컬렉션에서 확인 할 수 있습니다."), kMyLocalKey_titleLoading2);
	en->setObject(CCString::create("파란색 실루엣 영역을 획득해야 % 가 증가해요."), kMyLocalKey_titleLoading3);
	en->setObject(CCString::create("'영역 다시 뽑기' 를 해서 넓은 영역으로 play 를 시작하면 좀 더 쉽게 높은 등급의 카드를 획득하기가 쉬워져요"), kMyLocalKey_titleLoading4);
	en->setObject(CCString::create("'대중교통 모드' 는 play 중에 일시정지 버튼을 탭해서 설정할 수 있습니다."), kMyLocalKey_titleLoading5);
	en->setObject(CCString::create("Clear 등급에 따라 획득되는 별의 개수가 다릅니다. 별이 모이면 다음 액자를 열 수 있어요."), kMyLocalKey_titleLoading7);
	en->setObject(CCString::create("'업적' 창에서 달성한 업적을 보상받으세요. 보석을 드립니다."), kMyLocalKey_titleLoading8);
	en->setObject(CCString::create("일일미션에 도전해 보세요. 일일 미션을 달성하시면 다양한 선물을 드립니다."), kMyLocalKey_titleLoading9);
	en->setObject(CCString::create("주간랭킹에 도전해 보세요. 매주 새로운 선물을 푸짐하게 드립니다."), kMyLocalKey_titleLoading10);
	en->setObject(CCString::create("change 글자를 모아서 특별한 이미지를 확인하세요."), kMyLocalKey_titleLoading11);
	en->setObject(CCString::create("미사일을 업그레이드 하면 강력한 공격력으로 높은 등급 카드 획득이 가능합니다."), kMyLocalKey_titleLoading12);
	en->setObject(CCString::create("아이템뽑기에서는 '부활'/'무적' 아이템을 뽑을 수 있어요."), kMyLocalKey_titleLoading13);
	en->setObject(CCString::create("스테이지마다 showtime 에 도전해 보세요."), kMyLocalKey_titleLoading14);
	en->setObject(CCString::create("매주 새로운 이벤트가 열립니다. 공식 카페를 확인해 주세요."), kMyLocalKey_titleLoading15);
	en->setObject(CCString::create("게임 내에서는 이름 대신 닉네임을 사용합니다."), kMyLocalKey_titleLoading16);
	en->setObject(CCString::create("99% 라 너무 아쉽다면, 보너스 뽑기에 도전 해 보세요."), kMyLocalKey_titleLoading17);
	en->setObject(CCString::create("이벤트 스테이지에서는 특별한 이미지와 선물이 기다립니다."), kMyLocalKey_titleLoading18);
	en->setObject(CCString::create("스테이지별 미션 clear 가 어렵다면 아이템 뽑기를 통해 미션 전용 아이템을 사용해 보세요."), kMyLocalKey_titleLoading19);
	en->setObject(CCString::create("한 번에 많은 영역을 획득할 수록 여러 개의 미사일이 발사되며, 공격을 성공한 만큼 콤보가 적용됩니다."), kMyLocalKey_titleLoading6);
	en->setObject(CCString::create("이어하기는 두 번 까지만 사용할 수 있습니다."), kMyLocalKey_titleLoading20);
	en->setObject(CCString::create("내카드 목록에서 카드 이미지를 터치하면 카드 상세 내용을 보실 수 있습니다."), kMyLocalKey_titleLoading21);
	en->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading22);
	en->setObject(CCString::create("85% 로 클리어 하면 별 1개를 획득합니다."), kMyLocalKey_titleLoading23);
	en->setObject(CCString::create("change를 모두 모은 후 클리어하면 별 2개를 획득합니다."), kMyLocalKey_titleLoading24);
	en->setObject(CCString::create("100%로 클리어 하면 별 3개를 획득합니다."), kMyLocalKey_titleLoading25);
	en->setObject(CCString::create("change를 모두 모은 후 100%로 클리어하면 별 4개를 모을 수 있습니다."), kMyLocalKey_titleLoading26);
	en->setObject(CCString::create("무적 아이템을 먹으면 몇 초간 무적상태가 되어 보스의 공격을 받지 않습니다."), kMyLocalKey_titleLoading27);
	en->setObject(CCString::create("플레이 중 획득한 영역에 따라 골드를 드립니다."), kMyLocalKey_titleLoading28);
	en->setObject(CCString::create("30%나 할인되는 종합 아이템 구매 찬스를 놓치지 마세요!"), kMyLocalKey_titleLoading29);
	en->setObject(CCString::create("미션스테이지에서는 미션을 먼저 완료한 후 클리어가 가능합니다."), kMyLocalKey_titleLoading30);
	en->setObject(CCString::create("미사일 공격 시 그리는 선에 미사일이 닿으면 유도 공격을 당해요."), kMyLocalKey_titleLoading31);
	en->setObject(CCString::create("게임 중 타임 보너스 아이템을 먹으면 남은 시간에서 10초가 추가됩니다."), kMyLocalKey_titleLoading32);
	en->setObject(CCString::create("선을 그리다가 꾸욱 버튼에서 손을 떼면 캐릭터가 안전 영역으로 되돌아 갑니다."), kMyLocalKey_titleLoading33);
	en->setObject(CCString::create("보스에게 연속 공격을 성공시키면 콤보가 적용되며, 게임 결과에서 콤보 보너스를 받을 수 있습니다."), kMyLocalKey_titleLoading34);
	en->setObject(CCString::create("내가 모은 별의 개수는 메인화면의 왼쪽 상단에 있는 별 아이콘을 통해 확인하실 수 있습니다."), kMyLocalKey_titleLoading35);
	en->setObject(CCString::create("운석이 떨어지면 캐릭터에 맞지 않도록 주의하세요."), kMyLocalKey_titleLoading36);
	en->setObject(CCString::create("조이스틱과 꾸욱 버튼의 위치를 바꾸고 싶다면 설정메뉴에서 조이스틱위치를 반대로 변경해 보세요."), kMyLocalKey_titleLoading37);
	en->setObject(CCString::create("조이스틱의 위치를 고정하고 싶다면 설정메뉴에서 조이스틱조작을 고정으로 변경해 보세요"), kMyLocalKey_titleLoading38);
	en->setObject(CCString::create("꾸욱 버튼을 누른채 한쪽 방향으로 그린 다음 되돌아 오면 한줄 그리기가 가능합니다."), kMyLocalKey_titleLoading39);
	en->setObject(CCString::create("그리는 도중 줄이 꼬여도 당황하지 마세요. 줄이 꼬인 상태에서도 영역을 획득할 수 있습니다."), kMyLocalKey_titleLoading40);
	en->setObject(CCString::create("작은 몬스터에는 HP가 존재하며, 공격이 성공할 때마다 HP가 줄어듭니다."), kMyLocalKey_titleLoading41);
	en->setObject(CCString::create("액자 하나를 완성할 때마다 골드선물을 드립니다."), kMyLocalKey_titleLoading42);
	en->setObject(CCString::create("클리어 점수는 영역획득점수와 콤보점수, 남은 타임 보너스가 합산된 점수입니다."), kMyLocalKey_titleLoading43);
	en->setObject(CCString::create("고득점을 노린다면 연속 콤보를 성공시켜 보세요."), kMyLocalKey_titleLoading44);
	en->setObject(CCString::create(""), kMyLocalKey_titleLoadingEnd);
				
	en->setObject(CCString::create("START"), kMyLocalKey_gamestart);
	en->setObject(CCString::create("대전 시작"), kMyLocalKey_battleStart);
	en->setObject(CCString::create("Missile"), kMyLocalKey_mymissile);
	en->setObject(CCString::create("Damage"), kMyLocalKey_power);
	en->setObject(CCString::create("Gacha"), kMyLocalKey_gacha);
	en->setObject(CCString::create("Try one day to complete the mission! Gifts for you!"), kMyLocalKey_stageListDown);
	en->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_frameOpenConditionTitle);
	en->setObject(CCString::create("이전 액자 전체 클리어\n+ 별 %d개 이상"), kMyLocalKey_frameOpenConditionContent);
	en->setObject(CCString::create("잼로 오픈"), kMyLocalKey_frameOpenConditionContentRuby);
	en->setObject(CCString::create("%s\n%d:00-%d:00"), kMyLocalKey_frameOpenConditionContentTimeWeek);
	en->setObject(CCString::create("%s월 %s일 %s:%s 오픈"), kMyLocalKey_frameOpenConditionContentTimeDate);
	en->setObject(CCString::create("자세히 보기"), kMyLocalKey_detailView);
	en->setObject(CCString::create("별 획득 방법"), kMyLocalKey_detailConditionPopupTitle);
	en->setObject(CCString::create("<font newline=14>스테이지를 클리어 할 때마다<font newline=30>클리어 단계에 따라 별을 획득합니다.<font color=961>한 스테이지<font newline=14>에서 획득할 수 있는<font color=961>별 갯수는 총 10개<font newline=14>입니다."), kMyLocalKey_detailConditionPopupContent);
//	en->setObject(CCString::create("<font color=999 size=12>스테이지에서 <font color=990 size=12>4가지 등급<font color=999 size=12 newline=12>을 모두 Clear하면<font color=990 size=12>별 10개<font color=999 size=12>를 모을 수 있습니다."), kMyLocalKey_detailConditionPopupContent2);
//	en->setObject(CCString::create("에서 획득할 수 있는"), kMyLocalKey_detailConditionPopupContent3);
//	en->setObject(CCString::create("별 갯수는 총 10개"), kMyLocalKey_detailConditionPopupContent4);
//	en->setObject(CCString::create("입니다."), kMyLocalKey_detailConditionPopupContent5);
	en->setObject(CCString::create("결제 실패"), kMyLocalKey_failPurchase);
	en->setObject(CCString::create("지금 상점으로 이동하시겠습니까?"), kMyLocalKey_enoughtGoodsContent);
	en->setObject(CCString::create("포기하기"), kMyLocalKey_giveup);
	en->setObject(CCString::create("상점 바로가기"), kMyLocalKey_goShopButton);
	
	en->setObject(CCString::create("액자를 플레이할 수 있는 시간이 지났습니다.\n홈으로 돌아갑니다."), kMyLocalKey_timeOutFrame);
	en->setObject(CCString::create("바로입장"), kMyLocalKey_directEnter);
	en->setObject(CCString::create("카드 승급 기회!!"), kMyLocalKey_rankUpTitle);
	en->setObject(CCString::create("승급 성공시 한 단계 높은 카드를 획득할 수 있어요!"), kMyLocalKey_rankUpSubTitle);
	en->setObject(CCString::create("현재 획득카드"), kMyLocalKey_recentTakeCard);
	en->setObject(CCString::create("승급확률"), kMyLocalKey_rankUpRate);
	en->setObject(CCString::create("승급하기"), kMyLocalKey_rankup);
	
	en->setObject(CCString::create("구매하기"), kMyLocalKey_purchase);
	en->setObject(CCString::create("첫구매시 할인"), kMyLocalKey_firstPurchaseSale);
	en->setObject(CCString::create("%s할인은 첫 구매 회원님께만"), kMyLocalKey_firstPurchaseMent1);
	en->setObject(CCString::create(" 적용됩니다."), kMyLocalKey_firstPurchaseMent2);
	en->setObject(CCString::create("이 창을 닫으면 %s 할인 기회는 사라집니다."), kMyLocalKey_firstPurchaseMent3);
	en->setObject(CCString::create("지금 묶음 아이템을 구매하시면"), kMyLocalKey_emptyItemSaleMent1);
	en->setObject(CCString::create("%s 할인해 드립니다."), kMyLocalKey_emptyItemSaleMent2);
	en->setObject(CCString::create("플레이를 도와주는 기능성 아이템을"), kMyLocalKey_stupidNpuHelpMent1);
	en->setObject(CCString::create("%s할인된 가격으로 사용해보세요."), kMyLocalKey_stupidNpuHelpMent2);
	en->setObject(CCString::create("묶음아이템 할인"), kMyLocalKey_packageItemSale);
	en->setObject(CCString::create("5번째 구매시"), kMyLocalKey_mileageMent1);
	en->setObject(CCString::create("아주 특별한 할인혜택의 기회가 있으니"), kMyLocalKey_mileageMent2);
	en->setObject(CCString::create("혜택을 꼭! 확인해주세요."), kMyLocalKey_mileageMent3);
	en->setObject(CCString::create("단 한번의 구매 찬스"), kMyLocalKey_eventShopTitle);
	en->setObject(CCString::create("당신만을 위한 특별한 기회! %s 보너스 혜택을 드려요."), kMyLocalKey_eventShopMent1);
	en->setObject(CCString::create("※ 이 창을 닫으면 %s 보너스 기회는 사라집니다."), kMyLocalKey_eventShopMent2);
	en->setObject(CCString::create("공격레벨을 올리면"), kMyLocalKey_levelupGuideMent1);
	en->setObject(CCString::create("보스를 좀 더 쉽게 물리칠 수 있습니다."), kMyLocalKey_levelupGuideMent2);
	en->setObject(CCString::create("업그레이드 버튼을 눌러"), kMyLocalKey_levelupGuideMent3);
	en->setObject(CCString::create("공격레벨을 올려보세요!"), kMyLocalKey_levelupGuideMent4);
	en->setObject(CCString::create("다음에"), kMyLocalKey_next);
	en->setObject(CCString::create("업그레이드"), kMyLocalKey_levelupGo);
	en->setObject(CCString::create("업적"), kMyLocalKey_achievement);
	en->setObject(CCString::create("모든보상 받기"), kMyLocalKey_allRewardGet);
	en->setObject(CCString::create("종합 아이템 구매 찬스!"), kMyLocalKey_itemPackageChance);
	en->setObject(CCString::create("아이템이 하나도 없네요."), kMyLocalKey_emptyItemSaleTitle);
	en->setObject(CCString::create("첫구매 대폭할인!!"), kMyLocalKey_firstPurchaseSaleTitle);
	en->setObject(CCString::create("사랑합니다. 고객님!"), kMyLocalKey_mileageTitle);
	en->setObject(CCString::create("미사일 업그레이드"), kMyLocalKey_missileUpgrade);
	en->setObject(CCString::create("<font>누적영역</font><font color=961> %d%%</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalPercent1);
	en->setObject(CCString::create("%s%%"), kMyLocalKey_todaymissionTotalPercent2);
	en->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalPercent3);
	en->setObject(CCString::create("현재 획득영역"), kMyLocalKey_todaymissionTotalPercent4);
	en->setObject(CCString::create("<font>누적점수</font><font color=961> %s점</font><font>을 획득하라</font>"), kMyLocalKey_todaymissionTotalScore1);
	en->setObject(CCString::create("%s점"), kMyLocalKey_todaymissionTotalScore2);
	en->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalScore3);
	en->setObject(CCString::create("현재 획득점수"), kMyLocalKey_todaymissionTotalScore4);
	en->setObject(CCString::create("<font color=961>%s골드</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalTakeGold1);
	en->setObject(CCString::create("%s골드"), kMyLocalKey_todaymissionTotalTakeGold2);
	en->setObject(CCString::create("현재 획득골드"), kMyLocalKey_todaymissionTotalTakeGold3);
	en->setObject(CCString::create("<font>부하몹</font><font color=961> %d마리</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalCatch1);
	en->setObject(CCString::create("%s마리"), kMyLocalKey_todaymissionTotalCatch2);
	en->setObject(CCString::create(" 사냥하라!!"), kMyLocalKey_todaymissionTotalCatch3);
	en->setObject(CCString::create("현재 잡은 부하몹"), kMyLocalKey_todaymissionTotalCatch4);
	en->setObject(CCString::create("오늘의 미션을 완료하였습니다."), kMyLocalKey_todaymissionSuccess);
	en->setObject(CCString::create("완료"), kMyLocalKey_complete);
	en->setObject(CCString::create("업적달성"), kMyLocalKey_achieveSuccess);
	en->setObject(CCString::create("업적미완성"), kMyLocalKey_achieveNotSuccess);
	en->setObject(CCString::create("업적보상"), kMyLocalKey_achieveReward);
	en->setObject(CCString::create("달성한 업적이 없습니다."), kMyLocalKey_nothingSuccessAchieve);
	en->setObject(CCString::create("보상받을 업적이 없습니다."), kMyLocalKey_nothingRewardAchieve);
	
	en->setObject(CCString::create("영역획득"), kMyLocalKey_missionTitle0);
	en->setObject(CCString::create("용감한 기사"), kMyLocalKey_missionTitle1);
	en->setObject(CCString::create("부하몹 사냥"), kMyLocalKey_missionTitle2);
	en->setObject(CCString::create("욕심쟁이"), kMyLocalKey_missionTitle3);
	en->setObject(CCString::create("수집가"), kMyLocalKey_missionTitle4);
	en->setObject(CCString::create("완벽주의자"), kMyLocalKey_missionTitle5);
	en->setObject(CCString::create("결벽주의자"), kMyLocalKey_missionTitle6);
	en->setObject(CCString::create("비지니스맨"), kMyLocalKey_missionTitle7);
	
	
	en->setObject(CCString::create("<font color=961 size=13>%.0f%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionDiscription0);
	en->setObject(CCString::create("<font color=999 size=13>보스의 에너지를 모두 소진시켜라!"), kMyLocalKey_missionDiscription1);
	en->setObject(CCString::create("<font color=999 size=13>부하 몬스터를 가두어 잡으세요!"), kMyLocalKey_missionDiscription2);
	en->setObject(CCString::create("<font color=999 size=13>정해진 횟수만큼 한번에 많이 먹으세요!"), kMyLocalKey_missionDiscription3);
	en->setObject(CCString::create("<font color=999 size=13>정해진 숫자만큼 아이템을 모으세요!"), kMyLocalKey_missionDiscription4);
	en->setObject(CCString::create("<font color=999 size=13>정해진 목표로 정확하게 영역을 획득하세요!"), kMyLocalKey_missionDiscription5);
	en->setObject(CCString::create("<font color=999 size=13>CHANGE를 순서대로 획득하세요!"), kMyLocalKey_missionDiscription6);
	en->setObject(CCString::create("<font color=999 size=13>목표시간 내에 클리어하세요!"), kMyLocalKey_missionDiscription7);
	
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition0);
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition1);
	en->setObject(CCString::create("<font>목표 <font color=961 size=18>%d마리"), kMyLocalKey_missionCondition2);
	en->setObject(CCString::create("<font color=961 size=18>%d%%이상 %d번"), kMyLocalKey_missionCondition3);
	en->setObject(CCString::create("<font>목표 <font color=961 size=18>%d개"), kMyLocalKey_missionCondition4);
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition5);
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition6);
	en->setObject(CCString::create("<font>목표시간 <font color=961 size=18>%d초"), kMyLocalKey_missionCondition7);
	
	en->setObject(CCString::create("대쉬"), kMyLocalKey_item4title);
	en->setObject(CCString::create("잠깐동안 빠른속도로 이동 가능합니다."), kMyLocalKey_item4ment);
	en->setObject(CCString::create("부하몹제거"), kMyLocalKey_item5title);
	en->setObject(CCString::create("부하 몬스터 한마리를 제거합니다."), kMyLocalKey_item5ment);
	en->setObject(CCString::create("침묵"), kMyLocalKey_item7title);
	en->setObject(CCString::create("잠깐동안 몬스터가 공격을 못하도록 합니다."), kMyLocalKey_item7ment);
	en->setObject(CCString::create("부활"), kMyLocalKey_item10title);
	en->setObject(CCString::create("마지막 순간에 부활합니다."), kMyLocalKey_item10ment);
	en->setObject(CCString::create("시간추가"), kMyLocalKey_item8title);
	en->setObject(CCString::create("제한시간을 늘려줍니다."), kMyLocalKey_item8ment);
	en->setObject(CCString::create("일시정지"), kMyLocalKey_pause);
	en->setObject(CCString::create("계속하기"), kMyLocalKey_continue);
	en->setObject(CCString::create("나가기"), kMyLocalKey_ingameOut);
	en->setObject(CCString::create("재시작"), kMyLocalKey_ingameReplay);
	en->setObject(CCString::create("대중교통\n모드"), kMyLocalKey_publicPlaceMode);
	en->setObject(CCString::create("이어하기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass1);
	en->setObject(CCString::create("맵다시뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass2);
	en->setObject(CCString::create("업그레이드 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass3);
	en->setObject(CCString::create("아이템뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass4);
	en->setObject(CCString::create("1프로뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass5);
	en->setObject(CCString::create("<font newline=16>오늘의 미션을 완료하였습니다.<font color=961 newline=16>보상은 선물함으로 지급됩니다."), kMyLocalKey_todaymissionSuccessCommon);
	en->setObject(CCString::create("무료"), kMyLocalKey_free);
	en->setObject(CCString::create("영역 다시뽑기"), kMyLocalKey_areaReGacha);
	en->setObject(CCString::create("버튼을 눌러주세요."), kMyLocalKey_pleaseClickButton);
	en->setObject(CCString::create("STOP"), kMyLocalKey_stop);
	en->setObject(CCString::create("100% 도전하기"), kMyLocalKey_go100percent);
	en->setObject(CCString::create("계정연결"), kMyLocalKey_accountLink);
	en->setObject(CCString::create("최고 레벨 %d"), kMyLocalKey_maxLevel);
	en->setObject(CCString::create("COMPLETE"), kMyLocalKey_endUpgrade);
	en->setObject(CCString::create("%d성 카드 획득방법"), kMyLocalKey_cardLockedTitle);
	en->setObject(CCString::create("<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent1);
	en->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent2);
	en->setObject(CCString::create("<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent3);
	en->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent4);
	en->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTime);
	en->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTimeMinute);
	en->setObject(CCString::create("<font color=961 size=15>%dHour"), kMyLocalKey_todaymissionRemainTime2);
	en->setObject(CCString::create("<font color=961 size=15>%dMinute"), kMyLocalKey_todaymissionRemainTimeMinute2);
	en->setObject(CCString::create("<font color=961 size=15>모든 피스 획득!"), kMyLocalKey_puzzleSuccessTitle);
	en->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 하트를 드릴께요!"), kMyLocalKey_puzzleSuccessMent);
	en->setObject(CCString::create("<font color=961 size=15>놀라워요! <font color=961 size=15>퍼펙트 클리어!!"), kMyLocalKey_puzzlePerfectTitle);
	en->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 잼를 드릴께요!"), kMyLocalKey_puzzlePerfectMent);
	en->setObject(CCString::create("이어하시겠습니까?"), kMyLocalKey_doYouWantToContinue);
	en->setObject(CCString::create("이어하기"), kMyLocalKey_onContinue);
	en->setObject(CCString::create("그만하기"), kMyLocalKey_offContinue);
	en->setObject(CCString::create("이번 스테이지는 아이템을 사용할 수 없습니다."), kMyLocalKey_notUseItem);
	en->setObject(CCString::create("새로운 아이템 등장!"), kMyLocalKey_newItemTitle);
	en->setObject(CCString::create("새로운 아이템을 선물로 드릴께요!"), kMyLocalKey_newItemMent);
	en->setObject(CCString::create("보스가 쏜 미사일이\n선에 닿으면 불씨가 생겨 닿으면 죽어요."), kMyLocalKey_dieTutorial1);
	en->setObject(CCString::create("선을 그을때 몬스터가\n선에 닿으면 파동이 생겨 죽어요!"), kMyLocalKey_dieTutorial2);
	en->setObject(CCString::create(""), kMyLocalKey_dieTutorial3);
	en->setObject(CCString::create("남은시간 "), kMyLocalKey_restTime);
	en->setObject(CCString::create("%dDay"), kMyLocalKey_restTimeDay);
	en->setObject(CCString::create("%dHour"), kMyLocalKey_restTimeHour);
	en->setObject(CCString::create("%dMinute"), kMyLocalKey_restTimeMinute);
	en->setObject(CCString::create("%dSecond"), kMyLocalKey_restTimeSecond);
	
	en->setObject(CCString::create("Do you want to exit the game?"), kMyLocalKey_exit);
	
	en->setObject(CCString::create("Count Bomb"), kMyLocalKey_warning9);
	en->setObject(CCString::create("Orange Saw"), kMyLocalKey_warning105);
	en->setObject(CCString::create("Blue Saw"), kMyLocalKey_warning106);
	en->setObject(CCString::create("Crash Laser"), kMyLocalKey_warning107);
	en->setObject(CCString::create("Missile"), kMyLocalKey_warning108);
	en->setObject(CCString::create("Rush"), kMyLocalKey_warning109);
	en->setObject(CCString::create("Fire Work"), kMyLocalKey_warning110);
	en->setObject(CCString::create("Sunflower"), kMyLocalKey_warning111);
	en->setObject(CCString::create("Ice Bomb"), kMyLocalKey_warning112);
	en->setObject(CCString::create("Guided Bomb"), kMyLocalKey_warning113);
	en->setObject(CCString::create("Tornado"), kMyLocalKey_warning1001);
	en->setObject(CCString::create("Flashing"), kMyLocalKey_warning1002);
	en->setObject(CCString::create("Slow Zone"), kMyLocalKey_warning1003);
	en->setObject(CCString::create("Thorn Prison"), kMyLocalKey_warning1004);
	en->setObject(CCString::create("Freezing"), kMyLocalKey_warning1005);
	en->setObject(CCString::create("Chaos"), kMyLocalKey_warning1006);
	en->setObject(CCString::create("Teleport"), kMyLocalKey_warning1007);
	en->setObject(CCString::create("Invisibility"), kMyLocalKey_warning1008);
	en->setObject(CCString::create("Flame Spraying"), kMyLocalKey_warning1009);
	en->setObject(CCString::create("ThunderBolt"), kMyLocalKey_warning1010);
	en->setObject(CCString::create("Speed Laser"), kMyLocalKey_warning1011);
	en->setObject(CCString::create("Radioactivity"), kMyLocalKey_warning1012);
	en->setObject(CCString::create("Meteor"), kMyLocalKey_warning1013);
	en->setObject(CCString::create("Falling Stone"), kMyLocalKey_warning1014);
	en->setObject(CCString::create("Do not come"), kMyLocalKey_warning1015);
	en->setObject(CCString::create("Dynamite Mine"), kMyLocalKey_warning1016);
	en->setObject(CCString::create("Flame Stoker"), kMyLocalKey_warning1017);
	en->setObject(CCString::create("Sudden Cloude"), kMyLocalKey_warning1018);
	en->setObject(CCString::create("Boss Attacks Blocked"), kMyLocalKey_warningBossSuccess);
	en->setObject(CCString::create("Last Life"), kMyLocalKey_warningLastLife);
	
	en->setObject(CCString::create("새로운 보스 공격!"), kMyLocalKey_newBossPattern);
	
	en->setObject(CCString::create("맵에 부딪힐때마다 숫자가 줄고, 0이 되면 폭발해요."), kMyLocalKey_patternContent9);
	en->setObject(CCString::create("하나의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent105);
	en->setObject(CCString::create("여러개의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent106);
	en->setObject(CCString::create("레이저가 획득영역을 부숴요."), kMyLocalKey_patternContent107);
	en->setObject(CCString::create("그리는도중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent108);
	en->setObject(CCString::create("보스가 획득영역을 파괴하며 이동해요."), kMyLocalKey_patternContent109);
	en->setObject(CCString::create("획득영역에서 폭발후 미사일을 퍼트려요."), kMyLocalKey_patternContent110);
	en->setObject(CCString::create("사방으로 미사일을 발사해요."), kMyLocalKey_patternContent111);
	en->setObject(CCString::create("연속으로 발사되어 획득영역을 부숴요."), kMyLocalKey_patternContent112);
	en->setObject(CCString::create("따라와서 폭발해요."), kMyLocalKey_patternContent113);
	en->setObject(CCString::create("맵의 일부를 일정시간 가립니다."), kMyLocalKey_patternContent1001);
	en->setObject(CCString::create("일정시간 화면 전체를 뒤덮어 시야를 가려요."), kMyLocalKey_patternContent1002);
	en->setObject(CCString::create("영역안에서는 느려져요."), kMyLocalKey_patternContent1003);
	en->setObject(CCString::create("붉은 원 밖으로 나갈 수 없어요."), kMyLocalKey_patternContent1004);
	en->setObject(CCString::create("일정시간 움직이지 못해요."), kMyLocalKey_patternContent1005);
	en->setObject(CCString::create("조작 방향의 반대로 움직여요."), kMyLocalKey_patternContent1006);
	en->setObject(CCString::create("보스가 다른위치로 순간이동해요."), kMyLocalKey_patternContent1007);
	en->setObject(CCString::create("일정시간 보스가 보이지 않아요."), kMyLocalKey_patternContent1008);
	en->setObject(CCString::create("획득영역에 불을 피워요."), kMyLocalKey_patternContent1009);
	en->setObject(CCString::create("캐릭터를 따라오며 맵을 부숴요."), kMyLocalKey_patternContent1010);
	en->setObject(CCString::create("그리는중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent1011);
	en->setObject(CCString::create("방사능이 퍼진곳을 조심하세요."), kMyLocalKey_patternContent1012);
	en->setObject(CCString::create("대각선으로 떨어져 맵을 부숴요."), kMyLocalKey_patternContent1013);
	en->setObject(CCString::create("위에서 아래로 운석이 떨어져요."), kMyLocalKey_patternContent1014);
	en->setObject(CCString::create("닿지않도록 주의하세요."), kMyLocalKey_patternContent1015);
	en->setObject(CCString::create("카운트가 끝나면 폭발해요."), kMyLocalKey_patternContent1016);
	en->setObject(CCString::create("일정시간 맵을 따라 움직여요."), kMyLocalKey_patternContent1017);
	en->setObject(CCString::create("언제 폭발할지 몰라요."), kMyLocalKey_patternContent1018);
	
	en->setObject(CCString::create("조작방법에 대한 튜토리얼을 시작하겠습니다.\n가운데 빨간 동그라미가 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다."), kMyLocalKey_tutorial1);
	en->setObject(CCString::create("먼저 영역 위를 이동하는 방법에 대해 소개해드릴게요.\n오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있어요.\n조이스틱으로 캐릭터를 위로 이동시켜보세요."), kMyLocalKey_tutorial2);
	en->setObject(CCString::create("캐릭터를 위로 이동시키기"), kMyLocalKey_tutorial3);
	en->setObject(CCString::create("다음에는 영역을 획득하는 방법을 알아보도록 해요.\n왼쪽 아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있답니다.\n보이는 것처럼 영역을 획득해보세요."), kMyLocalKey_tutorial4);
	en->setObject(CCString::create("영역획득하기"), kMyLocalKey_tutorial5);
	en->setObject(CCString::create("파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다."), kMyLocalKey_tutorial6);
	en->setObject(CCString::create("제한시간 내에 달성도 85%를 넘기면 클리어!!"), kMyLocalKey_tutorial7);
	en->setObject(CCString::create("기본 튜토리얼을 모두 진행하셨습니다.\n보상으로 5000골드를 드립니다.\n본 게임으로 들아갑니다."), kMyLocalKey_tutorial8);
	
	en->setObject(CCString::create("point1. 겹친선도 획득!"), kMyLocalKey_controlTip_lineTangle_title);
	en->setObject(CCString::create("꼬인선! 걱정마세요! 그대로 획득할 수 있어요!"), kMyLocalKey_controlTip_lineTangle_content);
	en->setObject(CCString::create("point2. 한줄 먹기!"), kMyLocalKey_controlTip_slimLine_title);
	en->setObject(CCString::create("캐릭터를 왼쪽으로 이동 후 오른쪽으로 조작하면 OK!"), kMyLocalKey_controlTip_slimLine_content);
	
	en->setObject(CCString::create("미션"), kMyLocalKey_mission);
	en->setObject(CCString::create("PVP"), kMyLocalKey_endlessMode);
	en->setObject(CCString::create("주간연승랭킹"), kMyLocalKey_endlessWeeklyranking);
	en->setObject(CCString::create("내 정보"), kMyLocalKey_endlessMyInfo);
	en->setObject(CCString::create("전적"), kMyLocalKey_endlessInfoScore);
	en->setObject(CCString::create("<font color=999 strokecolor=000 strokesize=1>%d승 %d패 </font><font color=950 strokecolor=000 strokesize=1>(승률%d%%)</font>"), kMyLocalKey_endlessInfoScoreValue2);
	en->setObject(CCString::create("%d승 %d패 승률%d%%"), kMyLocalKey_endlessInfoScoreValue);
//	en->setObject(CCString::create("<font color=999 strokecolor=0 strokesize=1>%d승 %d패 </font><font color=950 strokecolor=0 strokesize=1>승률%d%%</font>"), kMyLocalKey_endlessInfoScoreValue);
	en->setObject(CCString::create("최고점수"), kMyLocalKey_endlessHighScore);
	en->setObject(CCString::create("최고연승"), kMyLocalKey_endlessHighStraight);
	en->setObject(CCString::create("<font size=15 color=#FFAA14>%d</font><font size=10 color=#FFAA14>연승</font>"), kMyLocalKey_endlessHighStraightValue1);
	en->setObject(CCString::create("%d연승"), kMyLocalKey_endlessHighStraightValue2);
	en->setObject(CCString::create("뭔가 문제가 발생하였습니다.\n다시 시도해주세요."), kMyLocalKey_endlessServerError);
	en->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_endlessDownloadFail);
	en->setObject(CCString::create("포기하시면 1패가 추가됩니다.\n계속 하시겠습니까?"), kMyLocalKey_endlessCheckGiveup);
	
	en->setObject(CCString::create("스페셜데이"), kMyLocalKey_attendanceSpecialTitle);
	en->setObject(CCString::create("매일 매일 연속 출석!!"), kMyLocalKey_attendanceSeqTitle);
	en->setObject(CCString::create("연속출석 일수에 따라 더욱 푸짐한 선물을 드립니다."), kMyLocalKey_attendanceSpecialContent);
	
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>골드"), kMyLocalKey_attendanceSpecialGoodsTypeGold);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>잼"), kMyLocalKey_attendanceSpecialGoodsTypeRuby);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem9);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem6);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem11);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass1);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass2);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass3);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass4);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass5);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass6);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>종류"), kMyLocalKey_attendanceSpecialGoodsTypeMany);
	
	en->setObject(CCString::create("<font size=12>%d<font size=8>골드"), kMyLocalKey_attendanceGoodsTypeGold);
	en->setObject(CCString::create("<font size=12>%d<font size=8>잼"), kMyLocalKey_attendanceGoodsTypeRuby);
	en->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem9);
	en->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem6);
	en->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem11);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass1);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass2);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass3);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass4);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass5);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass6);
	en->setObject(CCString::create("선물상자"), kMyLocalKey_attendanceGoodsTypeMany);
	
	en->setObject(CCString::create("출석체크"), kMyLocalKey_attendanceTitle);
	en->setObject(CCString::create("지급된 보상은 수신함에서 확인 가능합니다."), kMyLocalKey_attendanceContent);
	en->setObject(CCString::create("DAY"), kMyLocalKey_attendanceDay);
	
	en->setObject(CCString::create("생명의 돌"), kMyLocalKey_buyMorphingTitle);
	en->setObject(CCString::create("<font color=961 newline=18>생명의 돌을 사용하면 카드가 살아납니다.<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_buyMorphingContent);
	
	en->setObject(CCString::create("보유갯수 :"), kMyLocalKey_buyMorphingTake);
	en->setObject(CCString::create("개"), kMyLocalKey_buyMorphingTakeValue);
	
	en->setObject(CCString::create("카드 업그레이드"), kMyLocalKey_buyMorphingBuy);
	
	en->setObject(CCString::create("생명의 돌이 부족합니다."), kMyLocalKey_notEnoughtMorphingTitle);
	en->setObject(CCString::create("<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_notEnoughtMorphingContent);
	
	en->setObject(CCString::create("쿠폰등록"), kMyLocalKey_couponTitle);
	en->setObject(CCString::create("입력"), kMyLocalKey_couponContent);
	en->setObject(CCString::create("쿠폰등록성공"), kMyLocalKey_couponSuccess);
	en->setObject(CCString::create("선물함을 확인하세요."), kMyLocalKey_couponGiftbox);
	en->setObject(CCString::create("쿠폰등록실패"), kMyLocalKey_couponFail);
	en->setObject(CCString::create("이미 사용한 쿠폰입니다."), kMyLocalKey_couponAlready);
	en->setObject(CCString::create("유효기간이 만료 쿠폰입니다."), kMyLocalKey_couponExpire);
	en->setObject(CCString::create("잘못된 사용입니다."), kMyLocalKey_couponOserror);
	en->setObject(CCString::create("쿠폰을 찾을 수 없습니다."), kMyLocalKey_couponDontfind);
	en->setObject(CCString::create("통신이 원활하지 않습니다. 잠시후 다시 시도해주세요."), kMyLocalKey_couponOtherError);
	
	en->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle);
	en->setObject(CCString::create("<font newline=16>별이 부족해요.<font newline=16>더 높은 등급으로 클리어해서<font color=961 newline=16>별 %d개 모아주세요."), kMyLocalKey_notOpenPuzzleNotEnoughtStarContent);
	
	en->setObject(CCString::create("대전 준비"), kMyLocalKey_endlessStartSettingTitle);
	
	en->setObject(CCString::create("영역점수"), kMyLocalKey_endlessCalcTitleAreaScore);
	en->setObject(CCString::create("공격점수"), kMyLocalKey_endlessCalcTitleDamageScore);
	en->setObject(CCString::create("콤보점수"), kMyLocalKey_endlessCalcTitleComboScore);
	en->setObject(CCString::create("생명점수"), kMyLocalKey_endlessCalcTitleLifeScore);
	en->setObject(CCString::create("시간점수"), kMyLocalKey_endlessCalcTitleTimeScore);
	en->setObject(CCString::create("등급점수"), kMyLocalKey_endlessCalcTitleGradeScore);
	en->setObject(CCString::create("획득골드"), kMyLocalKey_endlessCalcTitleTakeGold);
	en->setObject(CCString::create("획득영역"), kMyLocalKey_endlessCalcTitleTakeArea);
	en->setObject(CCString::create("피격점수"), kMyLocalKey_endlessCalcTitleDamagedScore);
	
	en->setObject(CCString::create("승"), kMyLocalKey_win);
	en->setObject(CCString::create("게임결과"), kMyLocalKey_gameResult);
	en->setObject(CCString::create("실패"), kMyLocalKey_fail);
	en->setObject(CCString::create("달성!!"), kMyLocalKey_achieve);
	en->setObject(CCString::create("닫기"), kMyLocalKey_close);
	en->setObject(CCString::create("%d연승중"), kMyLocalKey_endlessIngWin);
	
	en->setObject(CCString::create("PVP 오픈조건"), kMyLocalKey_endlessOpenConditionTitle);
	en->setObject(CCString::create("<font color=961 newline=16>PVP는 %d스테이지 플레이 후<font newline=16>자동으로 오픈됩니다."), kMyLocalKey_endlessOpenConditionContent);
	
	en->setObject(CCString::create("대전배패"), kMyLocalKey_endlessKeepWin);
	
	en->setObject(CCString::create("연승저장"), kMyLocalKey_endlessKeepWinTitle);
	en->setObject(CCString::create("연승 정보는 유지됩니다."), kMyLocalKey_endlessKeepWinContent);
	
	en->setObject(CCString::create("%d연승 도전"), kMyLocalKey_endlessReadyIngWin);
	
	en->setObject(CCString::create("닉네임 오류"), kMyLocalKey_nicknameError);
	en->setObject(CCString::create("<font color=#FFFFFF>남은 판수 </font>"
																								"<font color=#FFAA14 size=14>%d회 / </font>"
																								"<font color=#FFFFFF>성공시 </font>"
																								"<font color=#FFAA14>%.1f%% 획득</font>"), kMyLocalKey_onePercentDesc2);
	
	en->setObject(CCString::create("대전상대를 찾는 중입니다."), kMyLocalKey_rivalSearch);
	
	en->setObject(CCString::create("%d레벨"), kMyLocalKey_rivalLevel);
	en->setObject(CCString::create("%d 연승 중"), kMyLocalKey_rivalIngWin);
	en->setObject(CCString::create("대전상대를 선택해주세요."), kMyLocalKey_rivalSelect);
	
	en->setObject(CCString::create("업적완료"), kMyLocalKey_achieveSuccess2);
	
	en->setObject(CCString::create("소녀들의 위치를 파악하고 있습니다."), kMyLocalKey_titleTempScript1);
	en->setObject(CCString::create("소녀들의 의상을 점검하고 있습니다."), kMyLocalKey_titleTempScript2);
	en->setObject(CCString::create("소녀들을 불러모으고 있습니다."), kMyLocalKey_titleTempScript3);
	

	en->setObject(CCString::create("하트 공짜"), kMyLocalKey_heartFree);

	en->setObject(CCString::create("지난주 랭킹 결과"), kMyLocalKey_rankRewardTitle);
	en->setObject(CCString::create("보상선물들은 선물함에서 확인하세요."), kMyLocalKey_rankRewardContent);
	en->setObject(CCString::create("주간 누적 랭킹"), kMyLocalKey_rankRewardStageTitle);
	en->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardStageScore);
	en->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardStageRank);
	en->setObject(CCString::create("지난주 플레이횟수"), kMyLocalKey_rankRewardStagePlayCount);
	en->setObject(CCString::create("PVP 랭킹"), kMyLocalKey_rankRewardEndlessTitle);
	en->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardEndlessScore);
	en->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardEndlessRank);
	en->setObject(CCString::create("지난주 연승기록"), kMyLocalKey_rankRewardEndlessVictory);
	en->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardStageReward);
	en->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardEndlessReward);
	
	en->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardStageRankValue);
	en->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardEndlessRankValue);
	
	en->setObject(CCString::create("%s연승"), kMyLocalKey_rankRewardEndlessVictoryScript);
	
	en->setObject(CCString::create("%d위"), kMyLocalKey_rankRewardRankValue);
	
	en->setObject(CCString::create("%s골드"), kMyLocalKey_rankRewardGoodsTypeGold);
	en->setObject(CCString::create("%s잼"), kMyLocalKey_rankRewardGoodsTypeRuby);
	en->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem9);
	en->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem6);
	en->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem11);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass1);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass2);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass3);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass4);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass5);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass6);
	en->setObject(CCString::create("선물상자"), kMyLocalKey_rankRewardGoodsTypeMany);
	
	en->setObject(CCString::create("카드 승급 이벤트!"), kMyLocalKey_rankUpEventTitle);
	en->setObject(CCString::create("승급 비용 공짜! 승급 확률 100%!!"), kMyLocalKey_rankUpEventContent);
	
	en->setObject(CCString::create("아니오"), kMyLocalKey_no);
	en->setObject(CCString::create("네"), kMyLocalKey_yes);
	en->setObject(CCString::create("<font>기본 클리어조건 <font color=961 newline=18>%.0f%%를<font color=961>%.0f%%로 하향<font newline=18> 시켜드립니다.<font newline=18>적용하시겠습니까?"), kMyLocalKey_clearTimeEventMent);
	en->setObject(CCString::create("이벤트"), kMyLocalKey_clearTimeEventTitle);
	
	en->setObject(CCString::create("숨겨진 업적"), kMyLocalKey_achieveHiddenTitle);
	en->setObject(CCString::create("저를 찾아주세요! 곳곳에 숨겨져 있어요!"), kMyLocalKey_achieveHiddenContent);
	
	en->setObject(CCString::create("<font newline=20>액자를 선택하면<font color=961 newline=20>퍼즐화면으로 이동합니다."), kMyLocalKey_mainflowDimmed1);
	en->setObject(CCString::create("<font newline=20>각 스테이지에서 획득한<font color=961>별의 총 갯수<font newline=20>입니다."), kMyLocalKey_mainflowDimmed2);
	en->setObject(CCString::create("<font newline=20>선물함"), kMyLocalKey_mainflowDimmed3);
	en->setObject(CCString::create("<font newline=20>업적"), kMyLocalKey_mainflowDimmed4);
	en->setObject(CCString::create("<font newline=20>설정"), kMyLocalKey_mainflowDimmed5);
	en->setObject(CCString::create("<font newline=20>이벤트"), kMyLocalKey_mainflowDimmed6);
	en->setObject(CCString::create("<font newline=20>랭킹"), kMyLocalKey_mainflowDimmed7);
	en->setObject(CCString::create("<font newline=20>상점"), kMyLocalKey_mainflowDimmed8);
	en->setObject(CCString::create("<font newline=20>카드"), kMyLocalKey_mainflowDimmed9);
	en->setObject(CCString::create("<font size=13 newline=16>오늘의<font size=13 newline=16>미션"), kMyLocalKey_mainflowDimmed10);
	
	en->setObject(CCString::create("모든 피스 획득!"), kMyLocalKey_puzzleClearCardTitle);
	en->setObject(CCString::create("<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzleClearCardContent);
	en->setObject(CCString::create("모든 카드 획득!"), kMyLocalKey_puzzlePerfectCardTitle);
	en->setObject(CCString::create("<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzlePerfectCardContent);
	
	en->setObject(CCString::create("%d연승 돌파!"), kMyLocalKey_endlessSeqWinRewardTitle);
	en->setObject(CCString::create("%d연승을 축하드립니다!"), kMyLocalKey_endlessSeqWinRewardContent);
	en->setObject(CCString::create("<font newline=18>%d골드를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeGold);
	en->setObject(CCString::create("<font newline=18>%d잼을 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby);
	en->setObject(CCString::create("<font newline=18>기본속도업아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9);
	en->setObject(CCString::create("<font newline=18>더블아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6);
	en->setObject(CCString::create("<font newline=18>자석아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11);
	en->setObject(CCString::create("<font newline=18>이어하기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass1);
	en->setObject(CCString::create("<font newline=18>맵가챠 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass2);
	en->setObject(CCString::create("<font newline=18>업그레이드 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass3);
	en->setObject(CCString::create("<font newline=18>아이템뽑기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass4);
	en->setObject(CCString::create("<font newline=18>1프로찬스 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass5);
	en->setObject(CCString::create("<font newline=18>생명의 돌을 %d개를 드립니다.<font newline=18>생명의 돌은 내카드 다이어리에서 사용하세요."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass6);
	en->setObject(CCString::create("<font newline=18>선물상자를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeMany);
	
	en->setObject(CCString::create("퍼즐에서 획득한 카드입니다."), kMyLocalKey_cardSettingMyCardContent);
	en->setObject(CCString::create("스페셜 카드 보기"), kMyLocalKey_cardSettingMyCardToSpecial);

	en->setObject(CCString::create("모든 피스\n획득시!"), kMyLocalKey_cardSettingClearCardMent);
	en->setObject(CCString::create("퍼펙트\n클리어시!"), kMyLocalKey_cardSettingPerfectCardMent);
	
	en->setObject(CCString::create("스페셜 카드"), kMyLocalKey_cardSettingSpecialCardTitle);
	en->setObject(CCString::create("이벤트를 통해 얻을 수 있는 카드입니다."), kMyLocalKey_cardSettingSpecialCardContent);
	en->setObject(CCString::create("내 카드 보기"), kMyLocalKey_cardSettingSpecialCardToMyCard);
	
	en->setObject(CCString::create("<font newline=16>PVP모드가<font newline=16>오픈되었어요!"), kMyLocalKey_endlessTutorialMent1);
	en->setObject(CCString::create("<font newline=16>내 정보 입니다."), kMyLocalKey_endlessTutorialMent2);
	en->setObject(CCString::create("<font>목록에서 <font color=961>다른 친구를 선택<font newline=16>해보세요.<font newline=16>다른 친구의 정보를 볼 수 있어요."), kMyLocalKey_endlessTutorialMent3);
	en->setObject(CCString::create("<font color=961>대전준비를 눌러보세요."), kMyLocalKey_endlessTutorialMent4);
	en->setObject(CCString::create("<font color=961>대전시작을 눌러보세요."), kMyLocalKey_endlessTutorialMent5);
	en->setObject(CCString::create("<font color=961>내 미니맵"), kMyLocalKey_endlessTutorialMent6);
	en->setObject(CCString::create("<font color=961>내 점수"), kMyLocalKey_endlessTutorialMent7);
	en->setObject(CCString::create("<font color=961>내 장착아이템"), kMyLocalKey_endlessTutorialMent8);
	en->setObject(CCString::create("<font color=961>상대 정보"), kMyLocalKey_endlessTutorialMent9);
	en->setObject(CCString::create("<font color=961 newline=16>5콤보 달성시 마다<font color=961 newline=16>미사일을 발사하여<font color=961 newline=16>상대의 점수를 깍습니다."), kMyLocalKey_endlessTutorialMent10);
	en->setObject(CCString::create("화면을 터치하면 게임 진행됩니다."), kMyLocalKey_endlessTutorialMent11);
	en->setObject(CCString::create("<font color=961>나의 결과"), kMyLocalKey_endlessTutorialMent12);
	en->setObject(CCString::create("<font color=961>상대방 결과"), kMyLocalKey_endlessTutorialMent13);
	en->setObject(CCString::create("<font>스크롤하면 점수 내역을 볼 수 있어요."), kMyLocalKey_endlessTutorialMent14);
	en->setObject(CCString::create("<font color=961>2연승에 도전할 수 있어요!"), kMyLocalKey_endlessTutorialMent15);
	en->setObject(CCString::create("잠깐!"), kMyLocalKey_endlessTutorialMent16);
	en->setObject(CCString::create("<font newline=16>연승에 도전하시면<font newline=20>생명의 돌을 받을 수 있어요.<font newline=16>튜토리얼을 마치겠습니다.(--)(__)"), kMyLocalKey_endlessTutorialMent17);
	en->setObject(CCString::create("<font>대전 상대를 선택해주세요."), kMyLocalKey_endlessTutorialMent18);
	
	en->setObject(CCString::create("%d 퍼즐"), kMyLocalKey_puzzleValue);
	
	en->setObject(CCString::create("보너스 영역 뽑기"), kMyLocalKey_onePercentTutorial1);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아슬아슬하게 영역을 100% 획득을 못했군요!</font>"
								   "<font color=#FFAA14 size=12 newline=30>보너스 영역 뽑기에 도전해보세요!</font>"
								   "<font color=#FFAA14 size=14 newline=18>100% 영역 획득 성공시</font>"
								   "<font color=#FFAA14 size=14>두 단계 위 등급 카드를 획득</font>"
								   "<font color=#FFFFFF size=14 newline=18>할 수 있어요!</font>"), kMyLocalKey_onePercentTutorial2);
	en->setObject(CCString::create("다  음"), kMyLocalKey_onePercentTutorial3);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아래 게이지바에서 분홍색 영역을 맞히면</font>"
								   "<font color=#FFAA14 size=12 newline=16>0.3%를 추가로 획득할 수 있습니다.</font>"
								   "<font color=#FFFFFF size=12 newline=30>기회는 총 3번 주어집니다.</font>"
								   "<font color=#FFAA14 size=14 newline=15>자 그럼 100% 에 도전해 볼까요?</font>"), kMyLocalKey_onePercentTutorial4);
	
	en->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>뽑기 기회는 단 3번!!</font><font color=#FFFFFF size=12>부족한 영역을 채워 100%에 도전하세요.</font>"), kMyLocalKey_onePercentGame1);
	en->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>100%에 성공시</font><font color=#FFAA14 size=12 tag=1>한 단계 높은 카드</font><font color=#FFFFFF size=12>를 획득할 수 있어요.</font>"), kMyLocalKey_onePercentGame2);
	en->setObject(CCString::create("<font color=#FFFFFF size=12>현재 획득 영역       </font><font color=#FFFF00 size=30 strokecolor=000 strokesize=2 tag=1>99.9%</font>"), kMyLocalKey_onePercentGame3);
	en->setObject(CCString::create("<font size=12 color=#FFFFFF newline=16>분홍색 영역에 맞게</font>"
								   "<font size=12 color=#FFAA14>멈춰 버튼 터치</font><font size=12 color=#FFFFFF>하세요!</font>"), kMyLocalKey_onePercentGame4);
	en->setObject(CCString::create("멈춰!"), kMyLocalKey_onePercentGame5);
	en->setObject(CCString::create("100% 실패"), kMyLocalKey_onePercentGame6);
	en->setObject(CCString::create("<font color=#FFFFFF size=12>최종 획득 영역     </font>"
								   "<font color=#FFAA14 size=30>%.1f%%</font>"), kMyLocalKey_onePercentGame7);
	en->setObject(CCString::create("닫기"), kMyLocalKey_onePercentGame8);
	en->setObject(CCString::create("100% 달성!!"), kMyLocalKey_onePercentGame9);
	en->setObject(CCString::create("카드 받기"), kMyLocalKey_onePercentGame10);
	
	en->setObject(CCString::create("보상내용"), kMyLocalKey_endlessRewardInfo);
	
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>가위 바위 보 중</font>"
								   "<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>"), kMyLocalKey_gababoContent1);
	en->setObject(CCString::create("<font size=12 newline=13>연</font>"
								   "<font size=12 newline=13>승</font>"
								   "<font size=12 newline=13>보</font>"
								   "<font size=12 newline=13>상</font>"), kMyLocalKey_gababoContent2);
	en->setObject(CCString::create("보상"), kMyLocalKey_gababoContent3);
	en->setObject(CCString::create("1승"), kMyLocalKey_gababoContent4);
	en->setObject(CCString::create("2승"), kMyLocalKey_gababoContent5);
	en->setObject(CCString::create("3승"), kMyLocalKey_gababoContent6);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent7);
	en->setObject(CCString::create("확 인"), kMyLocalKey_gababoContent8);
	en->setObject(CCString::create("나"), kMyLocalKey_gababoContent9);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런, 무승부네요!</font>"
								   "<font color=#FFAA14 size=12>한번 더!</font>"), kMyLocalKey_gababoContent10);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent11);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>세상에, 4판모두 이긴 사람은 당신이 처음이에요.</font>"
								   "<font color=#FFAA14 size=12 newline=14>최고의 보상을 드릴게요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>보너스 게임은 퍼펙트 클리어시마다 할 수 있어요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>그럼 다음에 또 만나요!</font>"), kMyLocalKey_gababoContent12);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런이런... 지고 말았네요. 너무 상심마세요!</font>"
								   "<font color=#FFAA14 size=12>보너스게임은 퍼펙트 클리어시마다 할 수 있어요!</font>"), kMyLocalKey_gababoContent13);
	en->setObject(CCString::create("루우비~!"), kMyLocalKey_gababoContent14);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>보너스 게임에 오신 것을 환영합니다!</font>"
								   "<font color=#FFFFFF size=12 newline=14>벌써 퍼즐을 퍼펙트 클리어 하시다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12>게임 설명은 최초 1회만 해드리니 잘 봐주세요!</font>"), kMyLocalKey_gababoContent15);
	en->setObject(CCString::create("다 음"), kMyLocalKey_gababoContent16);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>게임은 가위바위보 입니다!</font>"
								   "<font color=#FFAA14 size=12 newline=14>가위바위보 중 하나를 선택하여 저를 이기시면 됩니다.</font>"
								   "<font color=#FFAA14 size=12 newline=14>연승 횟수에 따라 더 큰 보상이 있어요!</font>"
								   "<font color=#FFFFFF size=12 newline=14>그럼 행운을 빌어요!</font>"), kMyLocalKey_gababoContent17);
	
	en->setObject(CCString::create("x%d"), kMyLocalKey_gababoReward);
	
	en->setObject(CCString::create("PVP 튜토리얼 중에는 나가실 수 없습니다."), kMyLocalKey_notGohomeEndlessTutorialMent);
	en->setObject(CCString::create("PVP 중에는 재시도할 수 없습니다."), kMyLocalKey_notReplayEndlessMent);
	
	en->setObject(CCString::create("대중교통 모드"), kMyLocalKey_optionSecretTitle);
	en->setObject(CCString::create("<font newline=16>대중교통모드가 적용중입니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 해제하실 수 있습니다."), kMyLocalKey_optionSecretOn);
	en->setObject(CCString::create("<font newline=16>대중교통모드가 풀렸습니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 적용하실 수 있습니다."), kMyLocalKey_optionSecretOff);
	
	en->setObject(CCString::create("카드 선물하기"), kMyLocalKey_cardGiftTitle);
	en->setObject(CCString::create("<font color=999>내 아이디는 <font color=961>%s<font color=999> 입니다."), kMyLocalKey_cardGiftMyID);
	en->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_cardGiftInputText);
	en->setObject(CCString::create("검색"), kMyLocalKey_cardGiftSearch);
	en->setObject(CCString::create("선물하기"), kMyLocalKey_cardGiftSend);
	en->setObject(CCString::create("검색결과가 없습니다."), kMyLocalKey_cardGiftNotFound);
	
	en->setObject(CCString::create("<font color=999>실패시 다음번 승급 성공확률이 <font color=961>%.0f%%<font color=999> 올라갑니다."), kMyLocalKey_rankUpFailRateUp);
	
	en->setObject(CCString::create("TIME OVER"), kMyLocalKey_failTitleTimeover);
	en->setObject(CCString::create("MISSION FAIL"), kMyLocalKey_failTitleMissionfail);
	en->setObject(CCString::create("GAME OVER"), kMyLocalKey_failTitleGameover);
	
	en->setObject(CCString::create("STAGE CLEAR"), kMyLocalKey_clearTitle);
	en->setObject(CCString::create("Warning!"), kMyLocalKey_warningDie);
	
	//	en->setObject(CCString::create("Cannot connect to the server."), kMyLocalKey_canNotConnectedServer);
//	en->setObject(CCString::create("Touch the screen."), kMyLocalKey_touchPlease);
//	en->setObject(CCString::create("This is a game in which you\nhave to gain as much area as\npossible using up, down, left,\nand right gestures."), kMyLocalKey_tutorial1);
//	en->setObject(CCString::create("I will tell you\nabout the controls."), kMyLocalKey_tutorial2);
//	en->setObject(CCString::create("Gesture to the right to\nacquire the area.\nYou can do it anywhere on the screen."), kMyLocalKey_tutorial3);
//	en->setObject(CCString::create("Gesture to the top."), kMyLocalKey_tutorial4);
//	en->setObject(CCString::create("Gesture to the left."), kMyLocalKey_tutorial5);
//	en->setObject(CCString::create("Gesture to the bottom."), kMyLocalKey_tutorial6);
//	en->setObject(CCString::create("You’ve finished acquiring\nthe area. Good job. If you\nwant to cancel drawing aline,\nyou can just touch the screen."), kMyLocalKey_tutorial7);
//	en->setObject(CCString::create("Now I will tell you how to\nmove over the area you have\nalready acquired."), kMyLocalKey_tutorial8);
//	en->setObject(CCString::create("If you gesture along the\nedges of the acquired area,\nyou’ll move in that direction\nalong the edges."), kMyLocalKey_tutorial9);
//	en->setObject(CCString::create("Gesture to the left."), kMyLocalKey_tutorial10);
//	en->setObject(CCString::create("If you want to stop while\nmoving, just touch the screen\nlightly."), kMyLocalKey_tutorial11);
//	en->setObject(CCString::create("Touch the screen."), kMyLocalKey_tutorial12);
//	en->setObject(CCString::create("Now I will explain the interface."), kMyLocalKey_tutorial14);
//	en->setObject(CCString::create("This is the screen that\ndetermines the chapter."), kMyLocalKey_tutorial15);
//	en->setObject(CCString::create("The marked part is the chapter.\nEach chapter has 5 stages.\nOnce you clear all stages,\nyou can move to the next chapter."), kMyLocalKey_tutorial16);
//	en->setObject(CCString::create("This is to log in to Facebook."), kMyLocalKey_tutorial17);
//	en->setObject(CCString::create("If you log in, you can see\nyour friends’ scores, and\nyou can give and receive\nitems with friends."), kMyLocalKey_tutorial17-1);
//	en->setObject(CCString::create("If you select a chapter,\nyou willsee the followingscreen.\nYou can choose the stage,\nproperties, and items."), kMyLocalKey_tutorial18);
//	en->setObject(CCString::create("The marked part is the\nstage. You can play stages\nyou have cleared again\nby selecting it."), kMyLocalKey_tutorial19);
//	en->setObject(CCString::create("This shows you the selected\nstage’s enemies and properties."), kMyLocalKey_tutorial20);
//	en->setObject(CCString::create("This is the selected property."), kMyLocalKey_tutorial21);
//	en->setObject(CCString::create("You can select the property,\nand you can buy and upgrade\nwith gold. You can earn gold\nduring the game."), kMyLocalKey_tutorial22);
//	en->setObject(CCString::create("This is the item you have\ncurrently selected."), kMyLocalKey_tutorial23);
//	en->setObject(CCString::create("You can choose the item,\nand buy things with gold."), kMyLocalKey_tutorial24);
//	en->setObject(CCString::create("Finally, this is the game\nstart button. To start the game,\nyou will need a brush."), kMyLocalKey_tutorial25);
//	en->setObject(CCString::create("Congratulations on finishing\nthe tutorial. We will now end\nthe tutorial and move on to\nselecting the chapter."), kMyLocalKey_tutorial26);
//
//	en->setObject(CCString::create("A special image appears in every chapter."), kMyLocalKey_loading1);
//	en->setObject(CCString::create("If you want to draw a thin line, gesture inthe opposite direction as you’re moving."), kMyLocalKey_loading2);
//	en->setObject(CCString::create("If you want to draw from the corner, gesture again inthe relevant direction onceyou are close to the corner."), kMyLocalKey_loading3);
//	en->setObject(CCString::create("If you log in to Facebook, you can see your friends’ scores."), kMyLocalKey_loading4);
//	en->setObject(CCString::create("If you invite your friends, you can earn brushes. You can send one invite per person per day."), kMyLocalKey_loading5);
//	en->setObject(CCString::create("You can check the brushes your friends sent you here."), kMyLocalKey_loading6);
//	en->setObject(CCString::create("Easy Attack Property: When you acquire an area, you attack more."), kMyLocalKey_loading11);
//	en->setObject(CCString::create("Starting Area Increase: You start the game from a wider area."), kMyLocalKey_loading12);
//	en->setObject(CCString::create("Block Status Defects: You block attacksby the boss. Attacks that can be blocked: fainting, sleeping, freezing, confusion"), kMyLocalKey_loading13);
//	en->setObject(CCString::create("Increase speed: When you get this item, your speed doubles for 5 seconds."), kMyLocalKey_loading14);
//	en->setObject(CCString::create("Silence: When you get this item, the boss cannot attack for 20 seconds."), kMyLocalKey_loading15);
//	en->setObject(CCString::create("Decreasing speed: When you get this item, the enemies’ movement speed is cut in halffor 20 seconds."), kMyLocalKey_loading16);
//	en->setObject(CCString::create("If you select a property, an attack skill that matches the relevant property will come out during the game."), kMyLocalKey_loading17);
//	en->setObject(CCString::create("property"), kMyLocalKey_loading19);
//	en->setObject(CCString::create("If you log into your Facebook account now, you will receive 5 more BRUSH."), kMyLocalKey_loading20);
//
//	en->setObject(CCString::create("Easy Attack Property"), kMyLocalKey_item1);
//	en->setObject(CCString::create("Starting Area Increase"), kMyLocalKey_item2);
//	en->setObject(CCString::create("Block Status Defects"), kMyLocalKey_item3);
//	en->setObject(CCString::create("Increase speed"), kMyLocalKey_item4);
//	en->setObject(CCString::create("Silence"), kMyLocalKey_item5);
//	en->setObject(CCString::create("Decreasing speed"), kMyLocalKey_item6);
//
//	en->setObject(CCString::create("No item has been selected."), kMyLocalKey_notSelectedItem);
//	en->setObject(CCString::create("You have already purchased this item."), kMyLocalKey_boughtItem);
//	en->setObject(CCString::create("You can select a maximum of 2 items per game."), kMyLocalKey_multiSelect);
//	en->setObject(CCString::create("You do not have enough gold for this purchase."), kMyLocalKey_notEnoughGold);
//	en->setObject(CCString::create("Do you want to go to the store?"), kMyLocalKey_goShop);
//	en->setObject(CCString::create("You cannot start the game because you do nothave any brushes."), kMyLocalKey_notEnoughBrush);
//	en->setObject(CCString::create("You do not have enough rubies for this purchase."), kMyLocalKey_notEnoughRuby);
//	
//	en->setObject(CCString::create("If you leave a review, your brushes will be refilled."), kMyLocalKey_review);
//	en->setObject(CCString::create("Recorded to the gallery. Do you want to go to the gallery?"), kMyLocalKey_regiGallery);
//	en->setObject(CCString::create("The hard chapters have been revealed."), kMyLocalKey_openHard);
//	en->setObject(CCString::create("The shield will disappear because the force of evil is full."), kMyLocalKey_shieldCancel);
//	en->setObject(CCString::create("You can receive awards after running the game."), kMyLocalKey_exeAppReward);
//	en->setObject(CCString::create("You can only use one GIFTCODE per hour."), kMyLocalKey_giftAHour);
//	en->setObject(CCString::create("This is an incorrect GIFTCODE."), kMyLocalKey_giftInvalid);
//	en->setObject(CCString::create("This GIFTCODE has already been used."), kMyLocalKey_giftUsed);
//	en->setObject(CCString::create("LitQoo issues the GIFTCODE"), kMyLocalKey_giftLitqoo);
//	en->setObject(CCString::create("Do you want to go to the home screen and exit the game?"), kMyLocalKey_goHome);
//	en->setObject(CCString::create("If you clear with 3 stars, the photo will be registered in the gallery."), kMyLocalKey_star3clear);
//	en->setObject(CCString::create("You can use the "), kMyLocalKey_itemEnable);
//	en->setObject(CCString::create("You can only play after updating to the newest version."), kMyLocalKey_needUpdate);
//	en->setObject(CCString::create("A new version has been released. Do you want to update?"), kMyLocalKey_newVersion);
//	en->setObject(CCString::create("Play now!"), kMyLocalKey_facebookInvite);
//	en->setObject(CCString::create("%s has sent you a BRUSH."), kMyLocalKey_facebookWith);
//	
//	en->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	en->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	en->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	en->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	en->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	en->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	en->setObject(CCString::create("take the monster."), kMyLocalKey_getMonster);
//	
//	en->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading21);
//	en->setObject(CCString::create("In the collection you can confirm the images, pets and monsters you collected."), kMyLocalKey_loading22);
//	en->setObject(CCString::create("You can change the control-mode by using the option in the left-side."), kMyLocalKey_loading23);
//	en->setObject(CCString::create("You can buy the pet in pet-shop."), kMyLocalKey_loading24);
//	en->setObject(CCString::create("You can accept the different blessing-effect as the kind of pets."), kMyLocalKey_loading25);
	setObject(en, "en");
}
void MyLocal::ja()
{
	
	CCDictionary* ja = CCDictionary::create();
	
	ja->setObject(CCString::create("Server Online"), kMyLocalKey_connectingServer);
	ja->setObject(CCString::create("Nickname enter"), kMyLocalKey_inputNick);
	ja->setObject(CCString::create("Please enter."), kMyLocalKey_inputPlease);
	ja->setObject(CCString::create("OK"), kMyLocalKey_ok);
	ja->setObject(CCString::create("Loading images."), kMyLocalKey_downImgInfo);
	ja->setObject(CCString::create("Information Retrieval Failure"), kMyLocalKey_failLoadInfo);
	ja->setObject(CCString::create("Retry"), kMyLocalKey_replay);
	ja->setObject(CCString::create("Downloading %.0f%% : %d/%d"), kMyLocalKey_downloadingProgress);
	ja->setObject(CCString::create("Receive images fail"), kMyLocalKey_downImgFail);
	ja->setObject(CCString::create("Login successful"), kMyLocalKey_successLogin);
	ja->setObject(CCString::create("This nickname is already in use."), kMyLocalKey_sameNick);
	ja->setObject(CCString::create("Nickname error"), kMyLocalKey_invalidNick);
	ja->setObject(CCString::create("Nickname is too long."), kMyLocalKey_longNick);
	ja->setObject(CCString::create("Nickname is too short."), kMyLocalKey_shortNick);
	ja->setObject(CCString::create("특수문자는 사용 할 수 없습니다."), kMyLocalKey_specialCharacterError);
	ja->setObject(CCString::create("Now Open"), kMyLocalKey_openRightNow);
	ja->setObject(CCString::create("RUBY %d open"), kMyLocalKey_openRubyValue);
	ja->setObject(CCString::create("GOLD %d open"), kMyLocalKey_openGoldValue);
	ja->setObject(CCString::create("CGP 일반 보상"), kMyLocalKey_cgpNormalTitle);
	ja->setObject(CCString::create("Press to receive reward."), kMyLocalKey_cgpNormalContent);
	ja->setObject(CCString::create("Receive reward"), kMyLocalKey_getReward);
	ja->setObject(CCString::create("take"), kMyLocalKey_take);
	ja->setObject(CCString::create("CGP 전체 팝업 보상"), kMyLocalKey_cgpAllPopupTitle);
	ja->setObject(CCString::create("Shop"), kMyLocalKey_shop);
	ja->setObject(CCString::create("Ranking"), kMyLocalKey_ranking);
	ja->setObject(CCString::create("My card"), kMyLocalKey_mycard);
	ja->setObject(CCString::create("Today mission"), kMyLocalKey_todaymission);
	ja->setObject(CCString::create("Event"), kMyLocalKey_event);
	ja->setObject(CCString::create("스페셜 스테이지"), kMyLocalKey_openStage);
	ja->setObject(CCString::create("스페셜 스테이지를 오픈하시겠습니까?"), kMyLocalKey_openStageContent);
	ja->setObject(CCString::create("오픈하기"), kMyLocalKey_doOpen);
	ja->setObject(CCString::create("Open Stage"), kMyLocalKey_stageOpenTitle);
	ja->setObject(CCString::create("A new stage\nhas been unlocked"), kMyLocalKey_stageOpenContent);
	ja->setObject(CCString::create("Puzzle open"), kMyLocalKey_puzzleOpenTitle);
	ja->setObject(CCString::create("A new puzzle\nhas been unlocked."), kMyLocalKey_puzzleOpenContent);
	ja->setObject(CCString::create("View"), kMyLocalKey_view);
	ja->setObject(CCString::create("Area 85%"), kMyLocalKey_condition1);
	ja->setObject(CCString::create("Area 85% + Change"), kMyLocalKey_condition2);
	ja->setObject(CCString::create("Area 100%"), kMyLocalKey_condition3);
	ja->setObject(CCString::create("Area 100% + Change"), kMyLocalKey_condition4);
	ja->setObject(CCString::create("85%"), kMyLocalKey_conditionTwoLine1);
	ja->setObject(CCString::create("85%\n+ Change"), kMyLocalKey_conditionTwoLine2);
	ja->setObject(CCString::create("100%"), kMyLocalKey_conditionTwoLine3);
	ja->setObject(CCString::create("100%\n+ Change"), kMyLocalKey_conditionTwoLine4);
	ja->setObject(CCString::create("%d Stage"), kMyLocalKey_stageValue);
	ja->setObject(CCString::create("My location %d"), kMyLocalKey_myrankValue);
	ja->setObject(CCString::create("Failure Ranking Information"), kMyLocalKey_failCheckRanking);
	ja->setObject(CCString::create("Stage"), kMyLocalKey_stage);
	ja->setObject(CCString::create("Choose the items that you will use during the game."), kMyLocalKey_selectUseItem);
	ja->setObject(CCString::create("Level %d"), kMyLocalKey_levelValue);
	ja->setObject(CCString::create("Power %s"), kMyLocalKey_powerValue);
	ja->setObject(CCString::create("Level %d Upgrade"), kMyLocalKey_upgradeLevelValue);
	ja->setObject(CCString::create("Up to three items\ncan be selected."), kMyLocalKey_totalItemSelectCount3);
	ja->setObject(CCString::create("You don’t have enough gold"), kMyLocalKey_goldNotEnought);
	ja->setObject(CCString::create("You don’t have enough ruby."), kMyLocalKey_rubyNotEnought);
	ja->setObject(CCString::create("You don’t have enough heart."), kMyLocalKey_heartNotEnought);
	ja->setObject(CCString::create("Would you like to go buy?"), kMyLocalKey_goToShop);
	ja->setObject(CCString::create("The default speed"), kMyLocalKey_baseSpeedUpTitle);
	ja->setObject(CCString::create("The user starts with a fast state."), kMyLocalKey_baseSpeedUpContent);
	ja->setObject(CCString::create("Double items"), kMyLocalKey_doubleItemTitle);
	ja->setObject(CCString::create("Quickly create items."), kMyLocalKey_doubleItemContent);
	ja->setObject(CCString::create("자석"), kMyLocalKey_magnetTitle);
	ja->setObject(CCString::create("골드와 아이템을 빨아들입니다."), kMyLocalKey_magnetContent);
	ja->setObject(CCString::create("Additional time"), kMyLocalKey_longTimeTitle);
	ja->setObject(CCString::create("Game time increases."), kMyLocalKey_longTimeContent);
	ja->setObject(CCString::create("Items Draw"), kMyLocalKey_itemGachaTitle);
	ja->setObject(CCString::create("어떤 아이템이 나올까요?"), kMyLocalKey_itemGachaDefaultContent1);
	ja->setObject(CCString::create("뽑기에만 출몰하는 다양한 아이템!!"), kMyLocalKey_itemGachaDefaultContent2);
	ja->setObject(CCString::create("Resurrection, the invincible challenge your items!"), kMyLocalKey_itemGachaContent);
	ja->setObject(CCString::create("RUBY SHOP"), kMyLocalKey_rubyShop);
	ja->setObject(CCString::create("GOLD SHOP"), kMyLocalKey_goldShop);
	ja->setObject(CCString::create("HEART SHOP"), kMyLocalKey_heartShop);
	ja->setObject(CCString::create("구매확인"), kMyLocalKey_checkBuy);
	ja->setObject(CCString::create("Do you want to buy?"), kMyLocalKey_realBuy);
	ja->setObject(CCString::create("Option"), kMyLocalKey_option);
	ja->setObject(CCString::create("Coupon"), kMyLocalKey_regCoupon);
	ja->setObject(CCString::create("Community"), kMyLocalKey_community);
	ja->setObject(CCString::create("Gametip"), kMyLocalKey_gametip);
	ja->setObject(CCString::create("Tutorial"), kMyLocalKey_tutorial);
	ja->setObject(CCString::create("System"), kMyLocalKey_system);
	ja->setObject(CCString::create("Background sound"), kMyLocalKey_bgm);
	ja->setObject(CCString::create("Sound Effects"), kMyLocalKey_effect);
	ja->setObject(CCString::create("Joystick\nposition"), kMyLocalKey_joystickPosition);
	ja->setObject(CCString::create("Operation\nJoystick"), kMyLocalKey_joystickControl);
	ja->setObject(CCString::create("Safe Mode"), kMyLocalKey_safetyMode);
	ja->setObject(CCString::create("Notify"), kMyLocalKey_noti);
	ja->setObject(CCString::create("Push"), kMyLocalKey_pushNoti);
	ja->setObject(CCString::create("Message\nAlarm"), kMyLocalKey_messageNoti);
	ja->setObject(CCString::create("선물을 받았습니다."), kMyLocalKey_getgift);
	ja->setObject(CCString::create("받은 메세지가 없습니다."), kMyLocalKey_nogift);
	ja->setObject(CCString::create("*30일 이후에 삭제됩니다."), kMyLocalKey_giftboxAlert);
	ja->setObject(CCString::create("재접속"), kMyLocalKey_reConnect);
	ja->setObject(CCString::create("다른 기기로 연결되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert1);
	ja->setObject(CCString::create("서버와의 접속에 오류가 발생하였습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert2);
	ja->setObject(CCString::create("세션이 종료되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert3);
	ja->setObject(CCString::create("서버연결에 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_reConnectAlert4);
	ja->setObject(CCString::create("BLOCKED"), kMyLocalKey_blocked);
	ja->setObject(CCString::create("계정이 아래의 사유로 정지 되었습니다."), kMyLocalKey_blockedMsg);
	ja->setObject(CCString::create("Member ID : "), kMyLocalKey_memberID);
	ja->setObject(CCString::create("Version : "), kMyLocalKey_gameversion);
	ja->setObject(CCString::create("Secession"), kMyLocalKey_withdraw);
	ja->setObject(CCString::create("Customer Service"), kMyLocalKey_callCenter);
	ja->setObject(CCString::create("Are you sure you want to logout?"), kMyLocalKey_checkLogout);
	ja->setObject(CCString::create("Cancel"), kMyLocalKey_cancel);
	ja->setObject(CCString::create("게임 탈퇴"), kMyLocalKey_withdrawTitle);
	ja->setObject(CCString::create("Can not use the room when you leave the game,"), kMyLocalKey_withdrawContent1);
	ja->setObject(CCString::create("all data will be deleted permanently."), kMyLocalKey_withdrawContent2);
	ja->setObject(CCString::create("Are you sure you want to really leave?"), kMyLocalKey_withdrawContent3);
	ja->setObject(CCString::create("탈퇴하기"), kMyLocalKey_doWithdraw);
	ja->setObject(CCString::create("Off"), kMyLocalKey_lightOff);
	ja->setObject(CCString::create("On"), kMyLocalKey_lightOn);
	ja->setObject(CCString::create("Right"), kMyLocalKey_right);
	ja->setObject(CCString::create("Left"), kMyLocalKey_left);
	ja->setObject(CCString::create("Fixing"), kMyLocalKey_fix);
	ja->setObject(CCString::create("Movement"), kMyLocalKey_move);
	ja->setObject(CCString::create("Weekly cumulative ranking"), kMyLocalKey_weeklyranking);
	ja->setObject(CCString::create("%ddays after the reset"), kMyLocalKey_dayAfterReset);
	ja->setObject(CCString::create("%dtimes after the reset"), kMyLocalKey_hourAfterReset);
	ja->setObject(CCString::create("%dMinutes after the reset"), kMyLocalKey_secondAfterReset);
	ja->setObject(CCString::create("Gift Box"), kMyLocalKey_giftbox);
	ja->setObject(CCString::create("Accept all"), kMyLocalKey_allAccept);
	ja->setObject(CCString::create("티켓함"), kMyLocalKey_ticketBox);
	ja->setObject(CCString::create("도움함"), kMyLocalKey_helpBox);
	ja->setObject(CCString::create("도전함"), kMyLocalKey_challengeBox);
	ja->setObject(CCString::create("하트함"), kMyLocalKey_heartBox);
	ja->setObject(CCString::create("전체보기"), kMyLocalKey_allView);
	ja->setObject(CCString::create("하트가 도착했어요."), kMyLocalKey_arriveHeart);
	ja->setObject(CCString::create("도전장이 도착했어요."), kMyLocalKey_arriveChallenge);
	ja->setObject(CCString::create("도전을 받아들이시겠습니까?"), kMyLocalKey_checkAcceptChallenge);
	ja->setObject(CCString::create("도망가기"), kMyLocalKey_ttt);
	ja->setObject(CCString::create("도전에 임하는 자세를 가집시다!!\n도망으로 간주, 패하셨습니다."), kMyLocalKey_tttContent);
	ja->setObject(CCString::create("도전결과!!"), kMyLocalKey_resultChallenge);
	ja->setObject(CCString::create("졌습니다. 브레이크 포인트 필요함. 위에 contentObj"), kMyLocalKey_loseContent);
	ja->setObject(CCString::create("도움요청이 도착했어요."), kMyLocalKey_arriveHelp);
	ja->setObject(CCString::create("상대방을 도와줍니다."), kMyLocalKey_acceptHelp);
	ja->setObject(CCString::create("거절"), kMyLocalKey_deny);
	ja->setObject(CCString::create("수락"), kMyLocalKey_accept);
	ja->setObject(CCString::create("상대방의 도움이 왔어요!"), kMyLocalKey_arriveHelped);
	ja->setObject(CCString::create("카드 정보 로딩"), kMyLocalKey_cardInfoLoading);
	ja->setObject(CCString::create("도움을 받았습니다!!"), kMyLocalKey_iHelped);
	ja->setObject(CCString::create("티켓요청이 도착했어요."), kMyLocalKey_arriveNeedTicket);
	ja->setObject(CCString::create("티켓요청이 도착. 티켓은 퍼즐을 열 때 필요합니다. 친구를 도와주세요!!"), kMyLocalKey_arriveNeedTicketContent);
	ja->setObject(CCString::create("보내기"), kMyLocalKey_send);
	ja->setObject(CCString::create("티켓이 왔네요 어서 받으세요."), kMyLocalKey_arriveTicket);
	ja->setObject(CCString::create("티켓이 도착했습니다. 티켓을 받으세요."), kMyLocalKey_arriveTicketContent);
	ja->setObject(CCString::create("티켓받기"), kMyLocalKey_takeTicket);
	ja->setObject(CCString::create("친구추가 요청이 왔습니다."), kMyLocalKey_arriveAddFriend);
	ja->setObject(CCString::create("An error has occurred."), kMyLocalKey_whatError);
	ja->setObject(CCString::create("I can not find the request arrived."), kMyLocalKey_arriveWhatError);
	ja->setObject(CCString::create("Default"), kMyLocalKey_defaultSort);
	ja->setObject(CCString::create("Take"), kMyLocalKey_takeOrder);
	ja->setObject(CCString::create("Rank"), kMyLocalKey_gradeOrder);
	ja->setObject(CCString::create("Rotation"), kMyLocalKey_rotation);
	ja->setObject(CCString::create("Raising levels to defeat the boss can do it more easily!"), kMyLocalKey_upgradeSubMent);
	ja->setObject(CCString::create("Draw again"), kMyLocalKey_itemRegacha);
	ja->setObject(CCString::create("%.0f%% Discount"), kMyLocalKey_itemRegachaDiscountValue);
	ja->setObject(CCString::create("Use of this item"), kMyLocalKey_thisItemUse);
	ja->setObject(CCString::create("Time"), kMyLocalKey_time);
	ja->setObject(CCString::create("Gold"), kMyLocalKey_gold);
	ja->setObject(CCString::create("Score"), kMyLocalKey_score);
	ja->setObject(CCString::create("총점"), kMyLocalKey_totalScore);
	ja->setObject(CCString::create("The next stage"), kMyLocalKey_nextStage);
	ja->setObject(CCString::create("again to"), kMyLocalKey_regame);
	ja->setObject(CCString::create("MAIN"), kMyLocalKey_toMain);
	ja->setObject(CCString::create("Please clear\nthe previous stage."), kMyLocalKey_beforeNotClearPuzzle);
	ja->setObject(CCString::create("이전 액자를\n모두 클리어하면\n다음액자가 나타납니다."), kMyLocalKey_waitForUpdate);
	ja->setObject(CCString::create("I'll wait!\n My collection\nof pictures of it!"), kMyLocalKey_diaryNoImg);
	ja->setObject(CCString::create("Reward"), kMyLocalKey_reward);
	ja->setObject(CCString::create("Speed"), kMyLocalKey_speed);
	ja->setObject(CCString::create("Ready"), kMyLocalKey_ready);
	ja->setObject(CCString::create("상대찾기"), kMyLocalKey_endlessReady);
	ja->setObject(CCString::create("God"), kMyLocalKey_rankA);
	ja->setObject(CCString::create("Hero"), kMyLocalKey_rankB);
	ja->setObject(CCString::create("Normal"), kMyLocalKey_rankC);
	ja->setObject(CCString::create("Monkey"), kMyLocalKey_rankD);
	ja->setObject(CCString::create("Buy"), kMyLocalKey_buy);
	ja->setObject(CCString::create("Push"), kMyLocalKey_controlClick);
	ja->setObject(CCString::create("Locked"), kMyLocalKey_locked);
	ja->setObject(CCString::create("Please select the box."), kMyLocalKey_selectClearBox);
	
	ja->setObject(CCString::create(""), kMyLocalKey_titleLoadingBegin);
	ja->setObject(CCString::create("각 스테이지 별로 각기 다른 4장의 이미지를 획득해 보세요."), kMyLocalKey_titleLoading1);
	ja->setObject(CCString::create("공공장소나 대중교통 이용시에는 '대중교통 모드'로 Play 하세요. 획득한 이미지는 내 컬렉션에서 확인 할 수 있습니다."), kMyLocalKey_titleLoading2);
	ja->setObject(CCString::create("파란색 실루엣 영역을 획득해야 % 가 증가해요."), kMyLocalKey_titleLoading3);
	ja->setObject(CCString::create("'영역 다시 뽑기' 를 해서 넓은 영역으로 play 를 시작하면 좀 더 쉽게 높은 등급의 카드를 획득하기가 쉬워져요"), kMyLocalKey_titleLoading4);
	ja->setObject(CCString::create("'대중교통 모드' 는 play 중에 일시정지 버튼을 탭해서 설정할 수 있습니다."), kMyLocalKey_titleLoading5);
	ja->setObject(CCString::create("Clear 등급에 따라 획득되는 별의 개수가 다릅니다. 별이 모이면 다음 액자를 열 수 있어요."), kMyLocalKey_titleLoading7);
	ja->setObject(CCString::create("'업적' 창에서 달성한 업적을 보상받으세요. 보석을 드립니다."), kMyLocalKey_titleLoading8);
	ja->setObject(CCString::create("일일미션에 도전해 보세요. 일일 미션을 달성하시면 다양한 선물을 드립니다."), kMyLocalKey_titleLoading9);
	ja->setObject(CCString::create("주간랭킹에 도전해 보세요. 매주 새로운 선물을 푸짐하게 드립니다."), kMyLocalKey_titleLoading10);
	ja->setObject(CCString::create("change 글자를 모아서 특별한 이미지를 확인하세요."), kMyLocalKey_titleLoading11);
	ja->setObject(CCString::create("미사일을 업그레이드 하면 강력한 공격력으로 높은 등급 카드 획득이 가능합니다."), kMyLocalKey_titleLoading12);
	ja->setObject(CCString::create("아이템뽑기에서는 '부활'/'무적' 아이템을 뽑을 수 있어요."), kMyLocalKey_titleLoading13);
	ja->setObject(CCString::create("스테이지마다 showtime 에 도전해 보세요."), kMyLocalKey_titleLoading14);
	ja->setObject(CCString::create("매주 새로운 이벤트가 열립니다. 공식 카페를 확인해 주세요."), kMyLocalKey_titleLoading15);
	ja->setObject(CCString::create("게임 내에서는 이름 대신 닉네임을 사용합니다."), kMyLocalKey_titleLoading16);
	ja->setObject(CCString::create("99% 라 너무 아쉽다면, 보너스 뽑기에 도전 해 보세요."), kMyLocalKey_titleLoading17);
	ja->setObject(CCString::create("이벤트 스테이지에서는 특별한 이미지와 선물이 기다립니다."), kMyLocalKey_titleLoading18);
	ja->setObject(CCString::create("스테이지별 미션 clear 가 어렵다면 아이템 뽑기를 통해 미션 전용 아이템을 사용해 보세요."), kMyLocalKey_titleLoading19);
	ja->setObject(CCString::create("한 번에 많은 영역을 획득할 수록 여러 개의 미사일이 발사되며, 공격을 성공한 만큼 콤보가 적용됩니다."), kMyLocalKey_titleLoading6);
	ja->setObject(CCString::create("이어하기는 두 번 까지만 사용할 수 있습니다."), kMyLocalKey_titleLoading20);
	ja->setObject(CCString::create("내카드 목록에서 카드 이미지를 터치하면 카드 상세 내용을 보실 수 있습니다."), kMyLocalKey_titleLoading21);
	ja->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading22);
	ja->setObject(CCString::create("85% 로 클리어 하면 별 1개를 획득합니다."), kMyLocalKey_titleLoading23);
	ja->setObject(CCString::create("change를 모두 모은 후 클리어하면 별 2개를 획득합니다."), kMyLocalKey_titleLoading24);
	ja->setObject(CCString::create("100%로 클리어 하면 별 3개를 획득합니다."), kMyLocalKey_titleLoading25);
	ja->setObject(CCString::create("change를 모두 모은 후 100%로 클리어하면 별 4개를 모을 수 있습니다."), kMyLocalKey_titleLoading26);
	ja->setObject(CCString::create("무적 아이템을 먹으면 몇 초간 무적상태가 되어 보스의 공격을 받지 않습니다."), kMyLocalKey_titleLoading27);
	ja->setObject(CCString::create("플레이 중 획득한 영역에 따라 골드를 드립니다."), kMyLocalKey_titleLoading28);
	ja->setObject(CCString::create("30%나 할인되는 종합 아이템 구매 찬스를 놓치지 마세요!"), kMyLocalKey_titleLoading29);
	ja->setObject(CCString::create("미션스테이지에서는 미션을 먼저 완료한 후 클리어가 가능합니다."), kMyLocalKey_titleLoading30);
	ja->setObject(CCString::create("미사일 공격 시 그리는 선에 미사일이 닿으면 유도 공격을 당해요."), kMyLocalKey_titleLoading31);
	ja->setObject(CCString::create("게임 중 타임 보너스 아이템을 먹으면 남은 시간에서 10초가 추가됩니다."), kMyLocalKey_titleLoading32);
	ja->setObject(CCString::create("선을 그리다가 꾸욱 버튼에서 손을 떼면 캐릭터가 안전 영역으로 되돌아 갑니다."), kMyLocalKey_titleLoading33);
	ja->setObject(CCString::create("보스에게 연속 공격을 성공시키면 콤보가 적용되며, 게임 결과에서 콤보 보너스를 받을 수 있습니다."), kMyLocalKey_titleLoading34);
	ja->setObject(CCString::create("내가 모은 별의 개수는 메인화면의 왼쪽 상단에 있는 별 아이콘을 통해 확인하실 수 있습니다."), kMyLocalKey_titleLoading35);
	ja->setObject(CCString::create("운석이 떨어지면 캐릭터에 맞지 않도록 주의하세요."), kMyLocalKey_titleLoading36);
	ja->setObject(CCString::create("조이스틱과 꾸욱 버튼의 위치를 바꾸고 싶다면 설정메뉴에서 조이스틱위치를 반대로 변경해 보세요."), kMyLocalKey_titleLoading37);
	ja->setObject(CCString::create("조이스틱의 위치를 고정하고 싶다면 설정메뉴에서 조이스틱조작을 고정으로 변경해 보세요"), kMyLocalKey_titleLoading38);
	ja->setObject(CCString::create("꾸욱 버튼을 누른채 한쪽 방향으로 그린 다음 되돌아 오면 한줄 그리기가 가능합니다."), kMyLocalKey_titleLoading39);
	ja->setObject(CCString::create("그리는 도중 줄이 꼬여도 당황하지 마세요. 줄이 꼬인 상태에서도 영역을 획득할 수 있습니다."), kMyLocalKey_titleLoading40);
	ja->setObject(CCString::create("작은 몬스터에는 HP가 존재하며, 공격이 성공할 때마다 HP가 줄어듭니다."), kMyLocalKey_titleLoading41);
	ja->setObject(CCString::create("액자 하나를 완성할 때마다 골드선물을 드립니다."), kMyLocalKey_titleLoading42);
	ja->setObject(CCString::create("클리어 점수는 영역획득점수와 콤보점수, 남은 타임 보너스가 합산된 점수입니다."), kMyLocalKey_titleLoading43);
	ja->setObject(CCString::create("고득점을 노린다면 연속 콤보를 성공시켜 보세요."), kMyLocalKey_titleLoading44);
	ja->setObject(CCString::create(""), kMyLocalKey_titleLoadingEnd);
	
	ja->setObject(CCString::create("START"), kMyLocalKey_gamestart);
	ja->setObject(CCString::create("대전 시작"), kMyLocalKey_battleStart);
	ja->setObject(CCString::create("Missile"), kMyLocalKey_mymissile);
	ja->setObject(CCString::create("Damage"), kMyLocalKey_power);
	ja->setObject(CCString::create("Gacha"), kMyLocalKey_gacha);
	ja->setObject(CCString::create("Try one day to complete the mission! Gifts for you!"), kMyLocalKey_stageListDown);
	ja->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_frameOpenConditionTitle);
	ja->setObject(CCString::create("이전 액자 전체 클리어\n+ 별 %d개 이상"), kMyLocalKey_frameOpenConditionContent);
	ja->setObject(CCString::create("잼로 오픈"), kMyLocalKey_frameOpenConditionContentRuby);
	ja->setObject(CCString::create("%s\n%d:00-%d:00"), kMyLocalKey_frameOpenConditionContentTimeWeek);
	ja->setObject(CCString::create("%s월 %s일 %s:%s 오픈"), kMyLocalKey_frameOpenConditionContentTimeDate);
	ja->setObject(CCString::create("자세히 보기"), kMyLocalKey_detailView);
	ja->setObject(CCString::create("별 획득 방법"), kMyLocalKey_detailConditionPopupTitle);
	ja->setObject(CCString::create("<font newline=14>스테이지를 클리어 할 때마다<font newline=30>클리어 단계에 따라 별을 획득합니다.<font color=961>한 스테이지<font newline=14>에서 획득할 수 있는<font color=961>별 갯수는 총 10개<font newline=14>입니다."), kMyLocalKey_detailConditionPopupContent);
//	ja->setObject(CCString::create("<font color=999 size=12>스테이지에서 <font color=990 size=12>4가지 등급<font color=999 size=12 newline=13>을 모두 Clear하면<font color=990 size=12>별 10개<font color=999 size=12>를 모을 수 있습니다."), kMyLocalKey_detailConditionPopupContent2);
//	ja->setObject(CCString::create("에서 획득할 수 있는"), kMyLocalKey_detailConditionPopupContent3);
//	ja->setObject(CCString::create("별 갯수는 총 10개"), kMyLocalKey_detailConditionPopupContent4);
//	ja->setObject(CCString::create("입니다."), kMyLocalKey_detailConditionPopupContent5);
	ja->setObject(CCString::create("결제 실패"), kMyLocalKey_failPurchase);
	ja->setObject(CCString::create("지금 상점으로 이동하시겠습니까?"), kMyLocalKey_enoughtGoodsContent);
	ja->setObject(CCString::create("포기하기"), kMyLocalKey_giveup);
	ja->setObject(CCString::create("상점 바로가기"), kMyLocalKey_goShopButton);
	
	ja->setObject(CCString::create("액자를 플레이할 수 있는 시간이 지났습니다.\n홈으로 돌아갑니다."), kMyLocalKey_timeOutFrame);
	ja->setObject(CCString::create("바로입장"), kMyLocalKey_directEnter);
	ja->setObject(CCString::create("카드 승급 기회!!"), kMyLocalKey_rankUpTitle);
	ja->setObject(CCString::create("승급 성공시 한 단계 높은 카드를 획득할 수 있어요!"), kMyLocalKey_rankUpSubTitle);
	ja->setObject(CCString::create("현재 획득카드"), kMyLocalKey_recentTakeCard);
	ja->setObject(CCString::create("승급확률"), kMyLocalKey_rankUpRate);
	ja->setObject(CCString::create("승급하기"), kMyLocalKey_rankup);
	
	ja->setObject(CCString::create("구매하기"), kMyLocalKey_purchase);
	ja->setObject(CCString::create("첫구매시 할인"), kMyLocalKey_firstPurchaseSale);
	ja->setObject(CCString::create("%s할인은 첫 구매 회원님께만"), kMyLocalKey_firstPurchaseMent1);
	ja->setObject(CCString::create(" 적용됩니다."), kMyLocalKey_firstPurchaseMent2);
	ja->setObject(CCString::create("이 창을 닫으면 %s 할인 기회는 사라집니다."), kMyLocalKey_firstPurchaseMent3);
	ja->setObject(CCString::create("지금 묶음 아이템을 구매하시면"), kMyLocalKey_emptyItemSaleMent1);
	ja->setObject(CCString::create("%s 할인해 드립니다."), kMyLocalKey_emptyItemSaleMent2);
	ja->setObject(CCString::create("플레이를 도와주는 기능성 아이템을"), kMyLocalKey_stupidNpuHelpMent1);
	ja->setObject(CCString::create("%s할인된 가격으로 사용해보세요."), kMyLocalKey_stupidNpuHelpMent2);
	ja->setObject(CCString::create("묶음아이템 할인"), kMyLocalKey_packageItemSale);
	ja->setObject(CCString::create("5번째 구매시"), kMyLocalKey_mileageMent1);
	ja->setObject(CCString::create("아주 특별한 할인혜택의 기회가 있으니"), kMyLocalKey_mileageMent2);
	ja->setObject(CCString::create("혜택을 꼭! 확인해주세요."), kMyLocalKey_mileageMent3);
	ja->setObject(CCString::create("단 한번의 구매 찬스"), kMyLocalKey_eventShopTitle);
	ja->setObject(CCString::create("당신만을 위한 특별한 기회! %s 보너스 혜택을 드려요."), kMyLocalKey_eventShopMent1);
	ja->setObject(CCString::create("※ 이 창을 닫으면 %s 보너스 기회는 사라집니다."), kMyLocalKey_eventShopMent2);
	ja->setObject(CCString::create("공격레벨을 올리면"), kMyLocalKey_levelupGuideMent1);
	ja->setObject(CCString::create("보스를 좀 더 쉽게 물리칠 수 있습니다."), kMyLocalKey_levelupGuideMent2);
	ja->setObject(CCString::create("업그레이드 버튼을 눌러"), kMyLocalKey_levelupGuideMent3);
	ja->setObject(CCString::create("공격레벨을 올려보세요!"), kMyLocalKey_levelupGuideMent4);
	ja->setObject(CCString::create("다음에"), kMyLocalKey_next);
	ja->setObject(CCString::create("업그레이드"), kMyLocalKey_levelupGo);
	ja->setObject(CCString::create("업적"), kMyLocalKey_achievement);
	ja->setObject(CCString::create("모든보상 받기"), kMyLocalKey_allRewardGet);
	ja->setObject(CCString::create("종합 아이템 구매 찬스!"), kMyLocalKey_itemPackageChance);
	ja->setObject(CCString::create("아이템이 하나도 없네요."), kMyLocalKey_emptyItemSaleTitle);
	ja->setObject(CCString::create("첫구매 대폭할인!!"), kMyLocalKey_firstPurchaseSaleTitle);
	ja->setObject(CCString::create("사랑합니다. 고객님!"), kMyLocalKey_mileageTitle);
	ja->setObject(CCString::create("미사일 업그레이드"), kMyLocalKey_missileUpgrade);
	ja->setObject(CCString::create("<font>누적영역</font><font color=961>%d%%</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalPercent1);
	ja->setObject(CCString::create("%s%%"), kMyLocalKey_todaymissionTotalPercent2);
	ja->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalPercent3);
	ja->setObject(CCString::create("현재 획득영역"), kMyLocalKey_todaymissionTotalPercent4);
	ja->setObject(CCString::create("<font>누적점수</font><font color=961>%s점</font><font>을 획득하라</font>"), kMyLocalKey_todaymissionTotalScore1);
	ja->setObject(CCString::create("%s점"), kMyLocalKey_todaymissionTotalScore2);
	ja->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalScore3);
	ja->setObject(CCString::create("현재 획득점수"), kMyLocalKey_todaymissionTotalScore4);
	ja->setObject(CCString::create("<font color=961>%s골드</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalTakeGold1);
	ja->setObject(CCString::create("%s골드"), kMyLocalKey_todaymissionTotalTakeGold2);
	ja->setObject(CCString::create("현재 획득골드"), kMyLocalKey_todaymissionTotalTakeGold3);
	ja->setObject(CCString::create("<font>부하몹</font><font color=961>%d마리</font><font>를 사냥하라</font>"), kMyLocalKey_todaymissionTotalCatch1);
	ja->setObject(CCString::create("%s마리"), kMyLocalKey_todaymissionTotalCatch2);
	ja->setObject(CCString::create(" 사냥하라!!"), kMyLocalKey_todaymissionTotalCatch3);
	ja->setObject(CCString::create("현재 잡은 부하몹"), kMyLocalKey_todaymissionTotalCatch4);
	ja->setObject(CCString::create("오늘의 미션을 완료하였습니다."), kMyLocalKey_todaymissionSuccess);
	ja->setObject(CCString::create("완료"), kMyLocalKey_complete);
	ja->setObject(CCString::create("업적달성"), kMyLocalKey_achieveSuccess);
	ja->setObject(CCString::create("업적미완성"), kMyLocalKey_achieveNotSuccess);
	ja->setObject(CCString::create("업적보상"), kMyLocalKey_achieveReward);
	ja->setObject(CCString::create("달성한 업적이 없습니다."), kMyLocalKey_nothingSuccessAchieve);
	ja->setObject(CCString::create("보상받을 업적이 없습니다."), kMyLocalKey_nothingRewardAchieve);
	
	
	ja->setObject(CCString::create("영역획득"), kMyLocalKey_missionTitle0);
	ja->setObject(CCString::create("용감한 기사"), kMyLocalKey_missionTitle1);
	ja->setObject(CCString::create("부하몹 사냥"), kMyLocalKey_missionTitle2);
	ja->setObject(CCString::create("욕심쟁이"), kMyLocalKey_missionTitle3);
	ja->setObject(CCString::create("수집가"), kMyLocalKey_missionTitle4);
	ja->setObject(CCString::create("완벽주의자"), kMyLocalKey_missionTitle5);
	ja->setObject(CCString::create("결벽주의자"), kMyLocalKey_missionTitle6);
	ja->setObject(CCString::create("비지니스맨"), kMyLocalKey_missionTitle7);
	
	
	
	ja->setObject(CCString::create("<font color=961 size=13>%.0f%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionDiscription0);
	ja->setObject(CCString::create("<font color=999 size=13>보스의 에너지를 모두 소진시켜라!"), kMyLocalKey_missionDiscription1);
	ja->setObject(CCString::create("<font color=999 size=13>부하 몬스터를 가두어 잡으세요!"), kMyLocalKey_missionDiscription2);
	ja->setObject(CCString::create("<font color=999 size=13>정해진 횟수만큼 한번에 많이 먹으세요!"), kMyLocalKey_missionDiscription3);
	ja->setObject(CCString::create("<font color=999 size=13>정해진 숫자만큼 아이템을 모으세요!"), kMyLocalKey_missionDiscription4);
	ja->setObject(CCString::create("<font color=999 size=13>정해진 목표로 정확하게 영역을 획득하세요!"), kMyLocalKey_missionDiscription5);
	ja->setObject(CCString::create("<font color=999 size=13>CHANGE를 순서대로 획득하세요!"), kMyLocalKey_missionDiscription6);
	ja->setObject(CCString::create("<font color=999 size=13>목표시간 내에 클리어하세요!"), kMyLocalKey_missionDiscription7);
	
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition0);
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition1);
	ja->setObject(CCString::create("<font>목표 <font color=961 size=18>%d마리"), kMyLocalKey_missionCondition2);
	ja->setObject(CCString::create("<font color=961 size=18>%d%%이상 %d번"), kMyLocalKey_missionCondition3);
	ja->setObject(CCString::create("<font>목표 <font color=961 size=18>%d개"), kMyLocalKey_missionCondition4);
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition5);
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition6);
	ja->setObject(CCString::create("<font>목표시간 <font color=961 size=18>%d초"), kMyLocalKey_missionCondition7);
	
	
	ja->setObject(CCString::create("대쉬"), kMyLocalKey_item4title);
	ja->setObject(CCString::create("잠깐동안 빠른속도로 이동 가능합니다."), kMyLocalKey_item4ment);
	ja->setObject(CCString::create("부하몹제거"), kMyLocalKey_item5title);
	ja->setObject(CCString::create("부하 몬스터 한마리를 제거합니다."), kMyLocalKey_item5ment);
	ja->setObject(CCString::create("침묵"), kMyLocalKey_item7title);
	ja->setObject(CCString::create("잠깐동안 몬스터가 공격을 못하도록 합니다."), kMyLocalKey_item7ment);
	ja->setObject(CCString::create("부활"), kMyLocalKey_item10title);
	ja->setObject(CCString::create("마지막 순간에 부활합니다."), kMyLocalKey_item10ment);
	ja->setObject(CCString::create("시간추가"), kMyLocalKey_item8title);
	ja->setObject(CCString::create("제한시간을 늘려줍니다."), kMyLocalKey_item8ment);
	ja->setObject(CCString::create("일시정지"), kMyLocalKey_pause);
	ja->setObject(CCString::create("계속하기"), kMyLocalKey_continue);
	ja->setObject(CCString::create("나가기"), kMyLocalKey_ingameOut);
	ja->setObject(CCString::create("재시작"), kMyLocalKey_ingameReplay);
	ja->setObject(CCString::create("대중교통\n모드"), kMyLocalKey_publicPlaceMode);
	ja->setObject(CCString::create("이어하기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass1);
	ja->setObject(CCString::create("맵다시뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass2);
	ja->setObject(CCString::create("업그레이드 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass3);
	ja->setObject(CCString::create("아이템뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass4);
	ja->setObject(CCString::create("1프로뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass5);
	ja->setObject(CCString::create("<font newline=16>오늘의 미션을 완료하였습니다.<font color=961 newline=16>보상은 선물함으로 지급됩니다."), kMyLocalKey_todaymissionSuccessCommon);
	ja->setObject(CCString::create("무료"), kMyLocalKey_free);
	ja->setObject(CCString::create("영역 다시뽑기"), kMyLocalKey_areaReGacha);
	ja->setObject(CCString::create("버튼을 눌러주세요."), kMyLocalKey_pleaseClickButton);
	ja->setObject(CCString::create("STOP"), kMyLocalKey_stop);
	ja->setObject(CCString::create("100% 도전하기"), kMyLocalKey_go100percent);
	ja->setObject(CCString::create("계정연결"), kMyLocalKey_accountLink);
	ja->setObject(CCString::create("최고 레벨 %d"), kMyLocalKey_maxLevel);
	ja->setObject(CCString::create("업그레이드 완료"), kMyLocalKey_endUpgrade);
	ja->setObject(CCString::create("%d성 카드 획득방법"), kMyLocalKey_cardLockedTitle);
	ja->setObject(CCString::create("<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent1);
	ja->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent2);
	ja->setObject(CCString::create("<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent3);
	ja->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent4);
	ja->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTime);
	ja->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTimeMinute);
	ja->setObject(CCString::create("<font color=961 size=15>%dHour"), kMyLocalKey_todaymissionRemainTime2);
	ja->setObject(CCString::create("<font color=961 size=15>%dMinute"), kMyLocalKey_todaymissionRemainTimeMinute2);
	ja->setObject(CCString::create("<font color=961 size=15>모든 피스 획득!"), kMyLocalKey_puzzleSuccessTitle);
	ja->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 하트를 드릴께요!"), kMyLocalKey_puzzleSuccessMent);
	ja->setObject(CCString::create("<font color=961 size=15>놀라워요! <font color=961 size=15>퍼펙트 클리어!!"), kMyLocalKey_puzzlePerfectTitle);
	ja->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 잼를 드릴께요!"), kMyLocalKey_puzzlePerfectMent);
	ja->setObject(CCString::create("이어하시겠습니까?"), kMyLocalKey_doYouWantToContinue);
	ja->setObject(CCString::create("이어하기"), kMyLocalKey_onContinue);
	ja->setObject(CCString::create("그만하기"), kMyLocalKey_offContinue);
	ja->setObject(CCString::create("이번 스테이지는 아이템을 사용할 수 없습니다."), kMyLocalKey_notUseItem);
	ja->setObject(CCString::create("새로운 아이템 등장!"), kMyLocalKey_newItemTitle);
	ja->setObject(CCString::create("새로운 아이템을 선물로 드릴께요!"), kMyLocalKey_newItemMent);
	ja->setObject(CCString::create("보스가 쏜 미사일이\n선에 닿으면 불씨가 생겨 닿으면 죽어요."), kMyLocalKey_dieTutorial1);
	ja->setObject(CCString::create("선을 그을때 몬스터가\n선에 닿으면 파동이 생겨 죽어요!"), kMyLocalKey_dieTutorial2);
	ja->setObject(CCString::create(""), kMyLocalKey_dieTutorial3);
	ja->setObject(CCString::create("남은시간 "), kMyLocalKey_restTime);
	ja->setObject(CCString::create("%dDay"), kMyLocalKey_restTimeDay);
	ja->setObject(CCString::create("%dHour"), kMyLocalKey_restTimeHour);
	ja->setObject(CCString::create("%dMinute"), kMyLocalKey_restTimeMinute);
	ja->setObject(CCString::create("%dSecond"), kMyLocalKey_restTimeSecond);
	
		
	ja->setObject(CCString::create("操作方法のチュートリアルを始めます。\n真ん中の赤い丸がキャラクターです。\nキャラクターを動かせて領域の外側に移動させることも\n領域を獲得することもできます。"), kMyLocalKey_tutorial1);
	ja->setObject(CCString::create("まずは領域の線の上を移動してみましょう。\n右下にジョイスティックでキャラクターの方向を変えることができます。\nキャラクターを上に移動させてみましょう。"), kMyLocalKey_tutorial2);
	ja->setObject(CCString::create("キャラクターを上に移動"), kMyLocalKey_tutorial3);
	ja->setObject(CCString::create("次は領域を獲得する方法を調べてみましょう。\n左下のボタンを押したら\n領域の外に動けますよ。\n領域を獲得してみてください。"), kMyLocalKey_tutorial4);
	ja->setObject(CCString::create("青のシルエット領域を獲得することでゲームをクリアすることができます。"), kMyLocalKey_tutorial5);
	ja->setObject(CCString::create("時間制限内に領域を85％以上獲得したらクリア！"), kMyLocalKey_tutorial6);
	ja->setObject(CCString::create("基本チュートリアルが終わりました。\n5000ゴールドをプレゼントします。\n本ゲームに戻ります。"), kMyLocalKey_tutorial7);
	
	ja->setObject(CCString::create("ゲームを終了しますか?"), kMyLocalKey_exit);
	
	
	ja->setObject(CCString::create("Count Bomb"), kMyLocalKey_warning9);
	ja->setObject(CCString::create("Orange Saw"), kMyLocalKey_warning105);
	ja->setObject(CCString::create("Blue Saw"), kMyLocalKey_warning106);
	ja->setObject(CCString::create("Crash Laser"), kMyLocalKey_warning107);
	ja->setObject(CCString::create("Missile"), kMyLocalKey_warning108);
	ja->setObject(CCString::create("Rush"), kMyLocalKey_warning109);
	ja->setObject(CCString::create("Fire Work"), kMyLocalKey_warning110);
	ja->setObject(CCString::create("Sunflower"), kMyLocalKey_warning111);
	ja->setObject(CCString::create("Ice Bomb"), kMyLocalKey_warning112);
	ja->setObject(CCString::create("Guided Bomb"), kMyLocalKey_warning113);
	ja->setObject(CCString::create("Tornado"), kMyLocalKey_warning1001);
	ja->setObject(CCString::create("Flashing"), kMyLocalKey_warning1002);
	ja->setObject(CCString::create("Slow Zone"), kMyLocalKey_warning1003);
	ja->setObject(CCString::create("Thorn Prison"), kMyLocalKey_warning1004);
	ja->setObject(CCString::create("Freezing"), kMyLocalKey_warning1005);
	ja->setObject(CCString::create("Chaos"), kMyLocalKey_warning1006);
	ja->setObject(CCString::create("Teleport"), kMyLocalKey_warning1007);
  ja->setObject(CCString::create("Invisibility"), kMyLocalKey_warning1008);
	ja->setObject(CCString::create("Flame Spraying"), kMyLocalKey_warning1009);
	ja->setObject(CCString::create("ThunderBolt"), kMyLocalKey_warning1010);
	ja->setObject(CCString::create("Speed Laser"), kMyLocalKey_warning1011);
	ja->setObject(CCString::create("Radioactivity"), kMyLocalKey_warning1012);
	ja->setObject(CCString::create("Meteor"), kMyLocalKey_warning1013);
	ja->setObject(CCString::create("Falling Stone"), kMyLocalKey_warning1014);
	ja->setObject(CCString::create("Do not come"), kMyLocalKey_warning1015);
	ja->setObject(CCString::create("Dynamite Mine"), kMyLocalKey_warning1016);
	ja->setObject(CCString::create("Flame Stoker"), kMyLocalKey_warning1017);
	ja->setObject(CCString::create("Sudden Cloude"), kMyLocalKey_warning1018);
	ja->setObject(CCString::create("Boss Attacks Blocked"), kMyLocalKey_warningBossSuccess);
	
	ja->setObject(CCString::create("새로운 보스 공격!"), kMyLocalKey_newBossPattern);
	
	ja->setObject(CCString::create("맵에 부딪힐때마다 숫자가 줄고, 0이 되면 폭발해요."), kMyLocalKey_patternContent9);
	ja->setObject(CCString::create("하나의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent105);
	ja->setObject(CCString::create("여러개의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent106);
	ja->setObject(CCString::create("레이저가 획득영역을 부숴요."), kMyLocalKey_patternContent107);
	ja->setObject(CCString::create("그리는도중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent108);
	ja->setObject(CCString::create("보스가 획득영역을 파괴하며 이동해요."), kMyLocalKey_patternContent109);
	ja->setObject(CCString::create("획득영역에서 폭발후 미사일을 퍼트려요."), kMyLocalKey_patternContent110);
	ja->setObject(CCString::create("사방으로 미사일을 발사해요."), kMyLocalKey_patternContent111);
	ja->setObject(CCString::create("연속으로 발사되어 획득영역을 부숴요."), kMyLocalKey_patternContent112);
	ja->setObject(CCString::create("따라와서 폭발해요."), kMyLocalKey_patternContent113);
	ja->setObject(CCString::create("맵의 일부를 일정시간 가립니다."), kMyLocalKey_patternContent1001);
	ja->setObject(CCString::create("일정시간 화면 전체를 뒤덮어 시야를 가려요."), kMyLocalKey_patternContent1002);
	ja->setObject(CCString::create("영역안에서는 느려져요."), kMyLocalKey_patternContent1003);
	ja->setObject(CCString::create("붉은 원 밖으로 나갈 수 없어요."), kMyLocalKey_patternContent1004);
	ja->setObject(CCString::create("일정시간 움직이지 못해요."), kMyLocalKey_patternContent1005);
	ja->setObject(CCString::create("조작 방향의 반대로 움직여요."), kMyLocalKey_patternContent1006);
	ja->setObject(CCString::create("보스가 다른위치로 순간이동해요."), kMyLocalKey_patternContent1007);
	ja->setObject(CCString::create("일정시간 보스가 보이지 않아요."), kMyLocalKey_patternContent1008);
	ja->setObject(CCString::create("획득영역에 불을 피워요."), kMyLocalKey_patternContent1009);
	ja->setObject(CCString::create("캐릭터를 따라오며 맵을 부숴요."), kMyLocalKey_patternContent1010);
	ja->setObject(CCString::create("그리는중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent1011);
	ja->setObject(CCString::create("방사능이 퍼진곳을 조심하세요."), kMyLocalKey_patternContent1012);
	ja->setObject(CCString::create("대각선으로 떨어져 맵을 부숴요."), kMyLocalKey_patternContent1013);
	ja->setObject(CCString::create("위에서 아래로 운석이 떨어져요."), kMyLocalKey_patternContent1014);
	ja->setObject(CCString::create("닿지않도록 주의하세요."), kMyLocalKey_patternContent1015);
	ja->setObject(CCString::create("카운트가 끝나면 폭발해요."), kMyLocalKey_patternContent1016);
	ja->setObject(CCString::create("일정시간 맵을 따라 움직여요."), kMyLocalKey_patternContent1017);
	ja->setObject(CCString::create("언제 폭발할지 몰라요."), kMyLocalKey_patternContent1018);
	

	
	ja->setObject(CCString::create("조작방법에 대한 튜토리얼을 시작하겠습니다.\n가운데 빨간 동그라미가 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다."), kMyLocalKey_tutorial1);
	ja->setObject(CCString::create("먼저 영역 위를 이동하는 방법에 대해 소개해드릴게요.\n오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있어요.\n조이스틱으로 캐릭터를 위로 이동시켜보세요."), kMyLocalKey_tutorial2);
	ja->setObject(CCString::create("캐릭터를 위로 이동시키기"), kMyLocalKey_tutorial3);
	ja->setObject(CCString::create("다음에는 영역을 획득하는 방법을 알아보도록 해요.\n왼쪽 아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있답니다.\n보이는 것처럼 영역을 획득해보세요."), kMyLocalKey_tutorial4);
	ja->setObject(CCString::create("영역획득하기"), kMyLocalKey_tutorial5);
	ja->setObject(CCString::create("파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다."), kMyLocalKey_tutorial6);
	ja->setObject(CCString::create("제한시간 내에 달성도 85%를 넘기면 클리어!!"), kMyLocalKey_tutorial7);
	ja->setObject(CCString::create("기본 튜토리얼을 모두 진행하셨습니다.\n보상으로 5000골드를 드립니다.\n본 게임으로 들아갑니다."), kMyLocalKey_tutorial8);
	
	ja->setObject(CCString::create("point1. 겹친선도 획득!"), kMyLocalKey_controlTip_lineTangle_title);
	ja->setObject(CCString::create("꼬인선! 걱정마세요! 그대로 획득할 수 있어요!"), kMyLocalKey_controlTip_lineTangle_content);
	ja->setObject(CCString::create("point2. 한줄 먹기!"), kMyLocalKey_controlTip_slimLine_title);
	ja->setObject(CCString::create("캐릭터를 왼쪽으로 이동 후 오른쪽으로 조작하면 OK!"), kMyLocalKey_controlTip_slimLine_content);
	
	ja->setObject(CCString::create("미션"), kMyLocalKey_mission);
	ja->setObject(CCString::create("PVP"), kMyLocalKey_endlessMode);
	ja->setObject(CCString::create("주간연승랭킹"), kMyLocalKey_endlessWeeklyranking);
	ja->setObject(CCString::create("내 정보"), kMyLocalKey_endlessMyInfo);
	ja->setObject(CCString::create("전적"), kMyLocalKey_endlessInfoScore);
	ja->setObject(CCString::create("<font color=999 strokecolor=000 strokesize=1>%d승 %d패 </font><font color=950 strokecolor=000 strokesize=1>(승률%d%%)</font>"), kMyLocalKey_endlessInfoScoreValue2);
	ja->setObject(CCString::create("%d승 %d패 승률%d%%"), kMyLocalKey_endlessInfoScoreValue);
	ja->setObject(CCString::create("최고점수"), kMyLocalKey_endlessHighScore);
	ja->setObject(CCString::create("최고연승"), kMyLocalKey_endlessHighStraight);
	ja->setObject(CCString::create("<font size=15 color=#FFAA14>%d</font><font size=10 color=#FFAA14>연승</font>"), kMyLocalKey_endlessHighStraightValue1);
	ja->setObject(CCString::create("%d연승"), kMyLocalKey_endlessHighStraightValue2);
	ja->setObject(CCString::create("뭔가 문제가 발생하였습니다.\n다시 시도해주세요."), kMyLocalKey_endlessServerError);
	ja->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_endlessDownloadFail);
	ja->setObject(CCString::create("포기하시면 1패가 추가됩니다.\n계속 하시겠습니까?"), kMyLocalKey_endlessCheckGiveup);
	
	ja->setObject(CCString::create("스페셜데이"), kMyLocalKey_attendanceSpecialTitle);
	ja->setObject(CCString::create("매일 매일 연속 출석!!"), kMyLocalKey_attendanceSeqTitle);
	ja->setObject(CCString::create("연속출석 일수에 따라 더욱 푸짐한 선물을 드립니다."), kMyLocalKey_attendanceSpecialContent);
	
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>골드"), kMyLocalKey_attendanceSpecialGoodsTypeGold);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>잼"), kMyLocalKey_attendanceSpecialGoodsTypeRuby);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem9);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem6);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem11);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass1);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass2);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass3);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass4);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass5);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass6);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>종류"), kMyLocalKey_attendanceSpecialGoodsTypeMany);
	
	ja->setObject(CCString::create("<font size=12>%d<font size=8>골드"), kMyLocalKey_attendanceGoodsTypeGold);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>잼"), kMyLocalKey_attendanceGoodsTypeRuby);
	ja->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem9);
	ja->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem6);
	ja->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem11);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass1);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass2);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass3);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass4);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass5);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass6);
	ja->setObject(CCString::create("선물상자"), kMyLocalKey_attendanceGoodsTypeMany);
	
	ja->setObject(CCString::create("출석체크"), kMyLocalKey_attendanceTitle);
	ja->setObject(CCString::create("지급된 보상은 수신함에서 확인 가능합니다."), kMyLocalKey_attendanceContent);
	ja->setObject(CCString::create("DAY"), kMyLocalKey_attendanceDay);
	
	ja->setObject(CCString::create("생명의 돌"), kMyLocalKey_buyMorphingTitle);
	ja->setObject(CCString::create("<font color=961 newline=18>생명의 돌을 사용하면 카드가 살아납니다.<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_buyMorphingContent);
	
	ja->setObject(CCString::create("보유갯수 :"), kMyLocalKey_buyMorphingTake);
	ja->setObject(CCString::create("개"), kMyLocalKey_buyMorphingTakeValue);
	
	ja->setObject(CCString::create("카드 업그레이드"), kMyLocalKey_buyMorphingBuy);
	
	ja->setObject(CCString::create("생명의 돌이 부족합니다."), kMyLocalKey_notEnoughtMorphingTitle);
	ja->setObject(CCString::create("<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_notEnoughtMorphingContent);
	
	ja->setObject(CCString::create("쿠폰등록"), kMyLocalKey_couponTitle);
	ja->setObject(CCString::create("입력"), kMyLocalKey_couponContent);
	ja->setObject(CCString::create("쿠폰등록성공"), kMyLocalKey_couponSuccess);
	ja->setObject(CCString::create("선물함을 확인하세요."), kMyLocalKey_couponGiftbox);
	ja->setObject(CCString::create("쿠폰등록실패"), kMyLocalKey_couponFail);
	ja->setObject(CCString::create("이미 사용한 쿠폰입니다."), kMyLocalKey_couponAlready);
	ja->setObject(CCString::create("유효기간이 만료 쿠폰입니다."), kMyLocalKey_couponExpire);
	ja->setObject(CCString::create("잘못된 사용입니다."), kMyLocalKey_couponOserror);
	ja->setObject(CCString::create("쿠폰을 찾을 수 없습니다."), kMyLocalKey_couponDontfind);
	ja->setObject(CCString::create("통신이 원활하지 않습니다. 잠시후 다시 시도해주세요."), kMyLocalKey_couponOtherError);
	
	ja->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle);
	ja->setObject(CCString::create("<font newline=16>별이 부족해요.<font newline=16>더 높은 등급으로 클리어해서<font color=961 newline=16>별 %d개 모아주세요."), kMyLocalKey_notOpenPuzzleNotEnoughtStarContent);
	
	ja->setObject(CCString::create("대전 준비"), kMyLocalKey_endlessStartSettingTitle);
	
	ja->setObject(CCString::create("영역점수"), kMyLocalKey_endlessCalcTitleAreaScore);
	ja->setObject(CCString::create("공격점수"), kMyLocalKey_endlessCalcTitleDamageScore);
	ja->setObject(CCString::create("콤보점수"), kMyLocalKey_endlessCalcTitleComboScore);
	ja->setObject(CCString::create("생명점수"), kMyLocalKey_endlessCalcTitleLifeScore);
	ja->setObject(CCString::create("시간점수"), kMyLocalKey_endlessCalcTitleTimeScore);
	ja->setObject(CCString::create("등급점수"), kMyLocalKey_endlessCalcTitleGradeScore);
	ja->setObject(CCString::create("획득골드"), kMyLocalKey_endlessCalcTitleTakeGold);
	ja->setObject(CCString::create("획득영역"), kMyLocalKey_endlessCalcTitleTakeArea);
	ja->setObject(CCString::create("피격점수"), kMyLocalKey_endlessCalcTitleDamagedScore);
	
	ja->setObject(CCString::create("승"), kMyLocalKey_win);
	ja->setObject(CCString::create("게임결과"), kMyLocalKey_gameResult);
	ja->setObject(CCString::create("실패"), kMyLocalKey_fail);
	ja->setObject(CCString::create("달성!!"), kMyLocalKey_achieve);
	ja->setObject(CCString::create("닫기"), kMyLocalKey_close);
	ja->setObject(CCString::create("%d연승중"), kMyLocalKey_endlessIngWin);
	
	ja->setObject(CCString::create("PVP 오픈조건"), kMyLocalKey_endlessOpenConditionTitle);
	ja->setObject(CCString::create("<font color=961 newline=16>PVP는 %d스테이지 플레이 후<font newline=16>자동으로 오픈됩니다."), kMyLocalKey_endlessOpenConditionContent);
	
	ja->setObject(CCString::create("대전배패"), kMyLocalKey_endlessKeepWin);
	
	ja->setObject(CCString::create("연승저장"), kMyLocalKey_endlessKeepWinTitle);
	ja->setObject(CCString::create("연승 정보는 유지됩니다."), kMyLocalKey_endlessKeepWinContent);
	
	ja->setObject(CCString::create("%d연승 도전"), kMyLocalKey_endlessReadyIngWin);
	
	ja->setObject(CCString::create("닉네임 오류"), kMyLocalKey_nicknameError);
	ja->setObject(CCString::create("<font color=#FFFFFF>남은 판수 </font>"
																								"<font color=#FFAA14 size=14>%d회 / </font>"
																								"<font color=#FFFFFF>성공시 </font>"
																								"<font color=#FFAA14>%.1f%% 획득</font>"), kMyLocalKey_onePercentDesc2);
	
	ja->setObject(CCString::create("대전상대를 찾는 중입니다."), kMyLocalKey_rivalSearch);
	
	ja->setObject(CCString::create("%d레벨"), kMyLocalKey_rivalLevel);
	ja->setObject(CCString::create("%d 연승 중"), kMyLocalKey_rivalIngWin);
	ja->setObject(CCString::create("대전상대를 선택해주세요."), kMyLocalKey_rivalSelect);
	
	ja->setObject(CCString::create("업적완료"), kMyLocalKey_achieveSuccess2);
	
	ja->setObject(CCString::create("소녀들의 위치를 파악하고 있습니다."), kMyLocalKey_titleTempScript1);
	ja->setObject(CCString::create("소녀들의 의상을 점검하고 있습니다."), kMyLocalKey_titleTempScript2);
	ja->setObject(CCString::create("소녀들을 불러모으고 있습니다."), kMyLocalKey_titleTempScript3);
	

	ja->setObject(CCString::create("하트 공짜"), kMyLocalKey_heartFree);

	ja->setObject(CCString::create("지난주 랭킹 결과"), kMyLocalKey_rankRewardTitle);
	ja->setObject(CCString::create("보상선물들은 선물함에서 확인하세요."), kMyLocalKey_rankRewardContent);
	ja->setObject(CCString::create("주간 누적 랭킹"), kMyLocalKey_rankRewardStageTitle);
	ja->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardStageScore);
	ja->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardStageRank);
	ja->setObject(CCString::create("지난주 플레이횟수"), kMyLocalKey_rankRewardStagePlayCount);
	ja->setObject(CCString::create("PVP 랭킹"), kMyLocalKey_rankRewardEndlessTitle);
	ja->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardEndlessScore);
	ja->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardEndlessRank);
	ja->setObject(CCString::create("지난주 연승기록"), kMyLocalKey_rankRewardEndlessVictory);
	ja->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardStageReward);
	ja->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardEndlessReward);
	
	ja->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardStageRankValue);
	ja->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardEndlessRankValue);
	
	ja->setObject(CCString::create("%s연승"), kMyLocalKey_rankRewardEndlessVictoryScript);
	
	ja->setObject(CCString::create("%d위"), kMyLocalKey_rankRewardRankValue);
	
	ja->setObject(CCString::create("%s골드"), kMyLocalKey_rankRewardGoodsTypeGold);
	ja->setObject(CCString::create("%s잼"), kMyLocalKey_rankRewardGoodsTypeRuby);
	ja->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem9);
	ja->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem6);
	ja->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem11);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass1);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass2);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass3);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass4);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass5);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass6);
	ja->setObject(CCString::create("선물상자"), kMyLocalKey_rankRewardGoodsTypeMany);
	
	ja->setObject(CCString::create("카드 승급 이벤트!"), kMyLocalKey_rankUpEventTitle);
	ja->setObject(CCString::create("승급 비용 공짜! 승급 확률 100%!!"), kMyLocalKey_rankUpEventContent);
	
	ja->setObject(CCString::create("아니오"), kMyLocalKey_no);
	ja->setObject(CCString::create("네"), kMyLocalKey_yes);
	ja->setObject(CCString::create("<font>기본 클리어조건 <font color=961 newline=18>%.0f%%를<font color=961>%.0f%%로 하향<font newline=18> 시켜드립니다.<font newline=18>적용하시겠습니까?"), kMyLocalKey_clearTimeEventMent);
	ja->setObject(CCString::create("이벤트"), kMyLocalKey_clearTimeEventTitle);
	
	ja->setObject(CCString::create("숨겨진 업적"), kMyLocalKey_achieveHiddenTitle);
	ja->setObject(CCString::create("저를 찾아주세요! 곳곳에 숨겨져 있어요!"), kMyLocalKey_achieveHiddenContent);
	
	ja->setObject(CCString::create("<font newline=20>액자를 선택하면<font color=961 newline=20>퍼즐화면으로 이동합니다."), kMyLocalKey_mainflowDimmed1);
	ja->setObject(CCString::create("<font newline=20>각 스테이지에서 획득한<font color=961>별의 총 갯수<font newline=20>입니다."), kMyLocalKey_mainflowDimmed2);
	ja->setObject(CCString::create("<font newline=20>선물함"), kMyLocalKey_mainflowDimmed3);
	ja->setObject(CCString::create("<font newline=20>업적"), kMyLocalKey_mainflowDimmed4);
	ja->setObject(CCString::create("<font newline=20>설정"), kMyLocalKey_mainflowDimmed5);
	ja->setObject(CCString::create("<font newline=20>이벤트"), kMyLocalKey_mainflowDimmed6);
	ja->setObject(CCString::create("<font newline=20>랭킹"), kMyLocalKey_mainflowDimmed7);
	ja->setObject(CCString::create("<font newline=20>상점"), kMyLocalKey_mainflowDimmed8);
	ja->setObject(CCString::create("<font newline=20>카드"), kMyLocalKey_mainflowDimmed9);
	ja->setObject(CCString::create("<font size=13 newline=16>오늘의<font size=13 newline=16>미션"), kMyLocalKey_mainflowDimmed10);
	
	ja->setObject(CCString::create("모든 피스 획득!"), kMyLocalKey_puzzleClearCardTitle);
	ja->setObject(CCString::create("<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzleClearCardContent);
	ja->setObject(CCString::create("모든 카드 획득!"), kMyLocalKey_puzzlePerfectCardTitle);
	ja->setObject(CCString::create("<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzlePerfectCardContent);
	
	ja->setObject(CCString::create("%d연승 돌파!"), kMyLocalKey_endlessSeqWinRewardTitle);
	ja->setObject(CCString::create("%d연승을 축하드립니다!"), kMyLocalKey_endlessSeqWinRewardContent);
	ja->setObject(CCString::create("<font newline=18>%d골드를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeGold);
	ja->setObject(CCString::create("<font newline=18>%d잼을 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby);
	ja->setObject(CCString::create("<font newline=18>기본속도업아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9);
	ja->setObject(CCString::create("<font newline=18>더블아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6);
	ja->setObject(CCString::create("<font newline=18>자석아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11);
	ja->setObject(CCString::create("<font newline=18>이어하기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass1);
	ja->setObject(CCString::create("<font newline=18>맵가챠 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass2);
	ja->setObject(CCString::create("<font newline=18>업그레이드 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass3);
	ja->setObject(CCString::create("<font newline=18>아이템뽑기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass4);
	ja->setObject(CCString::create("<font newline=18>1프로찬스 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass5);
	ja->setObject(CCString::create("<font newline=18>생명의 돌을 %d개를 드립니다.<font newline=18>생명의 돌은 내카드 다이어리에서 사용하세요."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass6);
	ja->setObject(CCString::create("<font newline=18>선물상자를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeMany);
	
	ja->setObject(CCString::create("퍼즐에서 획득한 카드입니다."), kMyLocalKey_cardSettingMyCardContent);
	ja->setObject(CCString::create("스페셜 카드 보기"), kMyLocalKey_cardSettingMyCardToSpecial);
	
	ja->setObject(CCString::create("모든 피스\n획득시!"), kMyLocalKey_cardSettingClearCardMent);
	ja->setObject(CCString::create("퍼펙트\n클리어시!"), kMyLocalKey_cardSettingPerfectCardMent);
	
	ja->setObject(CCString::create("스페셜 카드"), kMyLocalKey_cardSettingSpecialCardTitle);
	ja->setObject(CCString::create("이벤트를 통해 얻을 수 있는 카드입니다."), kMyLocalKey_cardSettingSpecialCardContent);
	ja->setObject(CCString::create("내 카드 보기"), kMyLocalKey_cardSettingSpecialCardToMyCard);
	
	ja->setObject(CCString::create("<font newline=16>PVP모드가<font newline=16>오픈되었어요!"), kMyLocalKey_endlessTutorialMent1);
	ja->setObject(CCString::create("<font newline=16>내 정보 입니다."), kMyLocalKey_endlessTutorialMent2);
	ja->setObject(CCString::create("<font>목록에서 <font color=961>다른 친구를 선택<font newline=16>해보세요.<font newline=16>다른 친구의 정보를 볼 수 있어요."), kMyLocalKey_endlessTutorialMent3);
	ja->setObject(CCString::create("<font color=961>대전준비를 눌러보세요."), kMyLocalKey_endlessTutorialMent4);
	ja->setObject(CCString::create("<font color=961>대전시작을 눌러보세요."), kMyLocalKey_endlessTutorialMent5);
	ja->setObject(CCString::create("<font color=961>내 미니맵"), kMyLocalKey_endlessTutorialMent6);
	ja->setObject(CCString::create("<font color=961>내 점수"), kMyLocalKey_endlessTutorialMent7);
	ja->setObject(CCString::create("<font color=961>내 장착아이템"), kMyLocalKey_endlessTutorialMent8);
	ja->setObject(CCString::create("<font color=961>상대 정보"), kMyLocalKey_endlessTutorialMent9);
	ja->setObject(CCString::create("<font color=961 newline=16>5콤보 달성시 마다<font color=961 newline=16>미사일을 발사하여<font color=961 newline=16>상대의 점수를 깍습니다."), kMyLocalKey_endlessTutorialMent10);
	ja->setObject(CCString::create("화면을 터치하면 게임 진행됩니다."), kMyLocalKey_endlessTutorialMent11);
	ja->setObject(CCString::create("<font color=961>나의 결과"), kMyLocalKey_endlessTutorialMent12);
	ja->setObject(CCString::create("<font color=961>상대방 결과"), kMyLocalKey_endlessTutorialMent13);
	ja->setObject(CCString::create("<font>스크롤하면 점수 내역을 볼 수 있어요."), kMyLocalKey_endlessTutorialMent14);
	ja->setObject(CCString::create("<font color=961>2연승에 도전할 수 있어요!"), kMyLocalKey_endlessTutorialMent15);
	ja->setObject(CCString::create("잠깐!"), kMyLocalKey_endlessTutorialMent16);
	ja->setObject(CCString::create("<font newline=16>연승에 도전하시면<font newline=20>생명의 돌을 받을 수 있어요.<font newline=16>튜토리얼을 마치겠습니다.(--)(__)"), kMyLocalKey_endlessTutorialMent17);
	ja->setObject(CCString::create("<font>대전 상대를 선택해주세요."), kMyLocalKey_endlessTutorialMent18);
	
	ja->setObject(CCString::create("%d 퍼즐"), kMyLocalKey_puzzleValue);
	
	ja->setObject(CCString::create("보너스 영역 뽑기"), kMyLocalKey_onePercentTutorial1);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아슬아슬하게 영역을 100% 획득을 못했군요!</font>"
								   "<font color=#FFAA14 size=12 newline=30>보너스 영역 뽑기에 도전해보세요!</font>"
								   "<font color=#FFAA14 size=14 newline=18>100% 영역 획득 성공시</font>"
								   "<font color=#FFAA14 size=14>두 단계 위 등급 카드를 획득</font>"
								   "<font color=#FFFFFF size=14 newline=18>할 수 있어요!</font>"), kMyLocalKey_onePercentTutorial2);
	ja->setObject(CCString::create("다  음"), kMyLocalKey_onePercentTutorial3);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아래 게이지바에서 분홍색 영역을 맞히면</font>"
								   "<font color=#FFAA14 size=12 newline=16>0.3%를 추가로 획득할 수 있습니다.</font>"
								   "<font color=#FFFFFF size=12 newline=30>기회는 총 3번 주어집니다.</font>"
								   "<font color=#FFAA14 size=14 newline=15>자 그럼 100% 에 도전해 볼까요?</font>"), kMyLocalKey_onePercentTutorial4);
	
	ja->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>뽑기 기회는 단 3번!!</font><font color=#FFFFFF size=12>부족한 영역을 채워 100%에 도전하세요.</font>"), kMyLocalKey_onePercentGame1);
	ja->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>100%에 성공시</font><font color=#FFAA14 size=12 tag=1>한 단계 높은 카드</font><font color=#FFFFFF size=12>를 획득할 수 있어요.</font>"), kMyLocalKey_onePercentGame2);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12>현재 획득 영역       </font><font color=#FFFF00 size=30 strokecolor=000 strokesize=2 tag=1>99.9%</font>"), kMyLocalKey_onePercentGame3);
	ja->setObject(CCString::create("<font size=12 color=#FFFFFF newline=16>분홍색 영역에 맞게</font>"
								   "<font size=12 color=#FFAA14>멈춰 버튼 터치</font><font size=12 color=#FFFFFF>하세요!</font>"), kMyLocalKey_onePercentGame4);
	ja->setObject(CCString::create("멈춰!"), kMyLocalKey_onePercentGame5);
	ja->setObject(CCString::create("100% 실패"), kMyLocalKey_onePercentGame6);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12>최종 획득 영역     </font>"
								   "<font color=#FFAA14 size=30>%.1f%%</font>"), kMyLocalKey_onePercentGame7);
	ja->setObject(CCString::create("닫기"), kMyLocalKey_onePercentGame8);
	ja->setObject(CCString::create("100% 달성!!"), kMyLocalKey_onePercentGame9);
	ja->setObject(CCString::create("카드 받기"), kMyLocalKey_onePercentGame10);
	
	ja->setObject(CCString::create("보상내용"), kMyLocalKey_endlessRewardInfo);
	
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>가위 바위 보 중</font>"
								   "<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>"), kMyLocalKey_gababoContent1);
	ja->setObject(CCString::create("<font size=12 newline=13>연</font>"
								   "<font size=12 newline=13>승</font>"
								   "<font size=12 newline=13>보</font>"
								   "<font size=12 newline=13>상</font>"), kMyLocalKey_gababoContent2);
	ja->setObject(CCString::create("보상"), kMyLocalKey_gababoContent3);
	ja->setObject(CCString::create("1승"), kMyLocalKey_gababoContent4);
	ja->setObject(CCString::create("2승"), kMyLocalKey_gababoContent5);
	ja->setObject(CCString::create("3승"), kMyLocalKey_gababoContent6);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent7);
	ja->setObject(CCString::create("확 인"), kMyLocalKey_gababoContent8);
	ja->setObject(CCString::create("나"), kMyLocalKey_gababoContent9);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런, 무승부네요!</font>"
								   "<font color=#FFAA14 size=12>한번 더!</font>"), kMyLocalKey_gababoContent10);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent11);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>세상에, 4판모두 이긴 사람은 당신이 처음이에요.</font>"
								   "<font color=#FFAA14 size=12 newline=14>최고의 보상을 드릴게요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>보너스 게임은 퍼펙트 클리어시마다 할 수 있어요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>그럼 다음에 또 만나요!</font>"), kMyLocalKey_gababoContent12);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런이런... 지고 말았네요. 너무 상심마세요!</font>"
								   "<font color=#FFAA14 size=12>보너스게임은 퍼펙트 클리어시마다 할 수 있어요!</font>"), kMyLocalKey_gababoContent13);
	ja->setObject(CCString::create("루우비~!"), kMyLocalKey_gababoContent14);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>보너스 게임에 오신 것을 환영합니다!</font>"
								   "<font color=#FFFFFF size=12 newline=14>벌써 퍼즐을 퍼펙트 클리어 하시다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12>게임 설명은 최초 1회만 해드리니 잘 봐주세요!</font>"), kMyLocalKey_gababoContent15);
	ja->setObject(CCString::create("다 음"), kMyLocalKey_gababoContent16);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>게임은 가위바위보 입니다!</font>"
								   "<font color=#FFAA14 size=12 newline=14>가위바위보 중 하나를 선택하여 저를 이기시면 됩니다.</font>"
								   "<font color=#FFAA14 size=12 newline=14>연승 횟수에 따라 더 큰 보상이 있어요!</font>"
								   "<font color=#FFFFFF size=12 newline=14>그럼 행운을 빌어요!</font>"), kMyLocalKey_gababoContent17);
	
	ja->setObject(CCString::create("x%d"), kMyLocalKey_gababoReward);
	
	ja->setObject(CCString::create("PVP 튜토리얼 중에는 나가실 수 없습니다."), kMyLocalKey_notGohomeEndlessTutorialMent);
	ja->setObject(CCString::create("PVP 중에는 재시도할 수 없습니다."), kMyLocalKey_notReplayEndlessMent);
	
	ja->setObject(CCString::create("대중교통 모드"), kMyLocalKey_optionSecretTitle);
	ja->setObject(CCString::create("<font newline=16>대중교통모드가 적용중입니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 해제하실 수 있습니다."), kMyLocalKey_optionSecretOn);
	ja->setObject(CCString::create("<font newline=16>대중교통모드가 풀렸습니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 적용하실 수 있습니다."), kMyLocalKey_optionSecretOff);
	
	ja->setObject(CCString::create("카드 선물하기"), kMyLocalKey_cardGiftTitle);
	ja->setObject(CCString::create("<font color=999>내 아이디는 <font color=961>%s<font color=999> 입니다."), kMyLocalKey_cardGiftMyID);
	ja->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_cardGiftInputText);
	ja->setObject(CCString::create("검색"), kMyLocalKey_cardGiftSearch);
	ja->setObject(CCString::create("선물하기"), kMyLocalKey_cardGiftSend);
	ja->setObject(CCString::create("검색결과가 없습니다."), kMyLocalKey_cardGiftNotFound);
	
	ja->setObject(CCString::create("<font color=999>실패시 다음번 승급 성공확률이 <font color=961>%.0f%%<font color=999> 올라갑니다."), kMyLocalKey_rankUpFailRateUp);
	
	ja->setObject(CCString::create("TIME OVER"), kMyLocalKey_failTitleTimeover);
	ja->setObject(CCString::create("MISSION FAIL"), kMyLocalKey_failTitleMissionfail);
	ja->setObject(CCString::create("GAME OVER"), kMyLocalKey_failTitleGameover);
	
	ja->setObject(CCString::create("STAGE CLEAR"), kMyLocalKey_clearTitle);
	ja->setObject(CCString::create("Warning!"), kMyLocalKey_warningDie);

//	ja->setObject(CCString::create("サーバーに連結できません。"), kMyLocalKey_canNotConnectedServer);
//	ja->setObject(CCString::create("画面をタッチしてください。"), kMyLocalKey_touchPlease);
//	ja->setObject(CCString::create("上下左右にジェスチャーを利用し、\n多くのエリアを獲得しなければなら\nないゲームです。"), kMyLocalKey_tutorial1);
//	ja->setObject(CCString::create("コントロール方法についてお知らせします。"), kMyLocalKey_tutorial2);
//	ja->setObject(CCString::create("エリアを獲得するために右にジェスチャーして\nください。 画面のどこでもできます。"), kMyLocalKey_tutorial3);
//	ja->setObject(CCString::create("上にジェスチャーしてください。"), kMyLocalKey_tutorial4);
//	ja->setObject(CCString::create("左にジェスチャーしてください。"), kMyLocalKey_tutorial5);
//	ja->setObject(CCString::create("下にジェスチャーしてください。"), kMyLocalKey_tutorial6);
//	ja->setObject(CCString::create("エリア獲得完了。 よくできました。\n線を描く途中で取消ししたい時は画面を\nタッチしてください。"), kMyLocalKey_tutorial7);
//	ja->setObject(CCString::create("今度は獲得したエリアの上を移動する\n方法をお知らせします。"), kMyLocalKey_tutorial8);
//	ja->setObject(CCString::create("獲得したエリアの外郭線方向にジェスチ\nャーすると、該当方向に外郭線に従って\n移動します。"), kMyLocalKey_tutorial9);
//	ja->setObject(CCString::create("左にジェスチャーしてください。"), kMyLocalKey_tutorial10);
//	ja->setObject(CCString::create("移動中に止めたい時は画面に軽く\nタッチしてください。"), kMyLocalKey_tutorial11);
//	ja->setObject(CCString::create("画面をタッチしてください。"), kMyLocalKey_tutorial12);
//	ja->setObject(CCString::create("ではインターペースについての説明をします。"), kMyLocalKey_tutorial14);
//	ja->setObject(CCString::create("チャプターを決定する画面です。"), kMyLocalKey_tutorial15);
//	ja->setObject(CCString::create("表示された部分がチャプターです。\n一つのチャプターには5個のステージがあります。\nステージを全てクリアすると次のチャプターに移ります。"), kMyLocalKey_tutorial16);
//	ja->setObject(CCString::create("下の説明はフェースブックにログ\nインする部分です。"), kMyLocalKey_tutorial17);
//	ja->setObject(CCString::create("ログインすると仲間の点数を見ることができ、\n仲間とアイテムをあげたり貰ったりできます。"), kMyLocalKey_tutorial17-1);
//	ja->setObject(CCString::create("チャプターを選択すると今と同じ画面が見れます。\nここではステージと属性、アイテムが選択できます。"), kMyLocalKey_tutorial18);
//	ja->setObject(CCString::create("表示された部分がステージ です。クリ\nアしたステージも再び選択してプレーできます。"), kMyLocalKey_tutorial19);
//	ja->setObject(CCString::create("現在選択されたステージの敵の姿と\n属性が見えます。"), kMyLocalKey_tutorial20);
//	ja->setObject(CCString::create("現在選択された属性です。"), kMyLocalKey_tutorial21);
//	ja->setObject(CCString::create("属性を選択することができ、またゴール\nドで購買およびアップグレードできます。\nゴールドはゲーム中に得られます。"), kMyLocalKey_tutorial22);
//	ja->setObject(CCString::create("現在選択されたアイテムです。"), kMyLocalKey_tutorial23);
//	ja->setObject(CCString::create("アイテムを選択することができ、\nまたゴールドで購買できます。"), kMyLocalKey_tutorial24);
//	ja->setObject(CCString::create("最後にゲームを始めるボタンです。\nゲームを始めるためには筆が必要です。"), kMyLocalKey_tutorial25);
//	ja->setObject(CCString::create("チュトリアルを進めるのは大変だったで\nしょう。 チュトリアルを終了し、\nチャプターを設定する場所に移動します。"), kMyLocalKey_tutorial26);
//	
//	ja->setObject(CCString::create("各チャプターの最後のステージには特別なイメージが出ます。"), kMyLocalKey_loading1);
//	ja->setObject(CCString::create("薄く描きたいなら移動中に反対方向にジェスチャーしてください。"), kMyLocalKey_loading2);
//	ja->setObject(CCString::create("角から描きたいなら角に近くなった時に、該当方向にもう一度ジェスチャーしてください。"), kMyLocalKey_loading3);
//	ja->setObject(CCString::create("フェースブックにログインすると仲間達の点数を見ることができます。"), kMyLocalKey_loading4);
//	ja->setObject(CCString::create("“仲間に(招待or共に)する”を送ると筆を得られます。一人の人に一日に一度だけ送れます。"), kMyLocalKey_loading5);
//	ja->setObject(CCString::create("仲間が私に送った筆はここで確認できます。"), kMyLocalKey_loading6);
//	ja->setObject(CCString::create("攻撃条件緩和: エリアを獲得するともっとたくさん攻撃します。"), kMyLocalKey_loading11);
//	ja->setObject(CCString::create("始まりエリア増加: もっと広いエリアでゲームを始めることができます。"), kMyLocalKey_loading12);
//	ja->setObject(CCString::create("状態異常防御: ボスの攻撃を防御します。防御になる攻撃: 気絶、睡眠、氷結、混乱"), kMyLocalKey_loading13);
//	ja->setObject(CCString::create("速度増加: アイテムを獲得すると、移動速度が5秒間二倍に速くなります。"), kMyLocalKey_loading14);
//	ja->setObject(CCString::create("沈黙: アイテムを獲得すると20秒間ボスが攻撃できなくなります。"), kMyLocalKey_loading15);
//	ja->setObject(CCString::create("速度減少: アイテムを獲得すると20秒間敵の移動速度が半分に下がります。"), kMyLocalKey_loading16);
//	ja->setObject(CCString::create("属性を選択すると該当属性に合う攻撃スキルがゲーム中に出てきます。"), kMyLocalKey_loading17);
//	ja->setObject(CCString::create("属性"), kMyLocalKey_loading19);
//	ja->setObject(CCString::create("今すぐフェースブックにログインされますと BRUSH 5 個を差し上げます。"), kMyLocalKey_loading20);
//	
//	ja->setObject(CCString::create("攻撃条件緩和"), kMyLocalKey_item1);
//	ja->setObject(CCString::create("始まりエリア増加"), kMyLocalKey_item2);
//	ja->setObject(CCString::create("状態異常防御"), kMyLocalKey_item3);
//	ja->setObject(CCString::create("速度増加"), kMyLocalKey_item4);
//	ja->setObject(CCString::create("沈黙"), kMyLocalKey_item5);
//	ja->setObject(CCString::create("速度減少"), kMyLocalKey_item6);
//	
//	ja->setObject(CCString::create("選択されたアイテムがありません。"), kMyLocalKey_notSelectedItem);
//	ja->setObject(CCString::create("既に選択されたアイテムを購買しました。"), kMyLocalKey_boughtItem);
//	ja->setObject(CCString::create("一度のゲームに使用できるアイテムの種類は最大2種類です。"), kMyLocalKey_multiSelect);
//	ja->setObject(CCString::create("ゴールドが足りないので購買できません。"), kMyLocalKey_notEnoughGold);
//	ja->setObject(CCString::create("商店に行きますか?"), kMyLocalKey_goShop);
//	ja->setObject(CCString::create("筆が足りないのでゲームを始めることができません。"), kMyLocalKey_notEnoughBrush);
//	ja->setObject(CCString::create("ルビーが足りないので購買できません。"), kMyLocalKey_notEnoughRuby);

//	ja->setObject(CCString::create("レビューを残してくだされば筆を一杯にしてあげます。"), kMyLocalKey_review);
//	ja->setObject(CCString::create("ギャラリーに登録されました。ギャラリーに行きますか?"), kMyLocalKey_regiGallery);
//	ja->setObject(CCString::create("難しいチャプターが公開されました。"), kMyLocalKey_openHard);
//	ja->setObject(CCString::create("悪い気運が一杯で防御幕が解除されます。"), kMyLocalKey_shieldCancel);
//	ja->setObject(CCString::create("ゲーム実行後、保証を貰えます。"), kMyLocalKey_exeAppReward);
//	ja->setObject(CCString::create("GIFTCODE は同時に一つだけ使えます。"), kMyLocalKey_giftAHour);
//	ja->setObject(CCString::create("正しくない GIFTCODE です。"), kMyLocalKey_giftInvalid);
//	ja->setObject(CCString::create("既に使っている GIFTCODE です。"), kMyLocalKey_giftUsed);
//	ja->setObject(CCString::create("GIFTCODEは LitQooで発行します。"), kMyLocalKey_giftLitqoo);
//	ja->setObject(CCString::create("ゲームを終了しホームに移動しますか?"), kMyLocalKey_goHome);
//	ja->setObject(CCString::create("星3個でクリアするとギャラリーに絵が登録されます。"), kMyLocalKey_star3clear);
//	ja->setObject(CCString::create("アイテムが使えます。"), kMyLocalKey_itemEnable);
//	ja->setObject(CCString::create("最新バージョンにアップデートするとプレーできます。"), kMyLocalKey_needUpdate);
//	ja->setObject(CCString::create("最新バージョンがリリースされました。アップデートしますか?"), kMyLocalKey_newVersion);
//	ja->setObject(CCString::create("今すぐ一緒にしましょう！"), kMyLocalKey_facebookInvite);
//	ja->setObject(CCString::create("%sがBRUSHを送りました。"), kMyLocalKey_facebookWith);
//	
//	ja->setObject(CCString::create("Empty"), kMyLocalKey_petEmpty);
//	ja->setObject(CCString::create("Attack"), kMyLocalKey_petAttack);
//	ja->setObject(CCString::create("Gold"), kMyLocalKey_petGold);
//	ja->setObject(CCString::create("Fast"), kMyLocalKey_petFast);
//	ja->setObject(CCString::create("Silence"), kMyLocalKey_petSilence);
//	ja->setObject(CCString::create("Slow"), kMyLocalKey_petSlow);
//	ja->setObject(CCString::create("モンスターを得ました。"), kMyLocalKey_getMonster);
//	
//	ja->setObject(CCString::create("集まったimageとpetとmonsterの情報はcollectionで確認することができます。"), kMyLocalKey_loading21);
//	ja->setObject(CCString::create("集まったimageとpetとmonsterの情報はcollectionで確認することができます。"), kMyLocalKey_loading22);
//	ja->setObject(CCString::create("コントロールの方法はゲームの左にあるオプションを利用して変更することができます。"), kMyLocalKey_loading23);
//	ja->setObject(CCString::create("ペットはペットショップで買えます。"), kMyLocalKey_loading24);
//	ja->setObject(CCString::create("ペットの種類に従って色んなblessingの効果を受けます。"), kMyLocalKey_loading25);
	setObject(ja, "ja");
}

void MyLocal::printList(){
	CCDictElement* element = NULL;
	CCDictionary* loDic = (CCDictionary*)(myLoc->objectForKey("ko"));
	printf("################## Localpack ####################\n");
	CCDICT_FOREACH(loDic, element) {
		CCString *objectString = (CCString *)element -> getObject();
		printf("%d\t\"%s\"\n",element -> getIntKey(),objectString->getCString());
	}
	printf("################## Localpack ####################\n");
}
