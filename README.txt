README: TP Geometrie Algorithmique et Maillages

Etudiant: Peuriere Romain
N°: 11707721

TPs:
Tous les TPs et fonctions sont complets excepté le test et la priority queue sur le terrain 3D.

Interface:

Boutons:

Exit: 		Quitte le programme et ferme la fenetre
Tetraedra: 	Affiche le mesh tetraedre
Pyramid: 	Affiche le mesh pyramide
.off File: 	Charge et affiche le mesh queen.off

Box2D: 		Charger un mesh d'une box 2D

Full/Wire: 	Change le mode d'affichage entre triangle 'full' et 'wireframe'
Circulator: 	Teste le mesh actuel avec le code fourni utilisant les circulateurs.

/!\ Fonctions applicables que sur le mesh Box2D : (sinon bloqué et indiqué en console) /!\
    Chaque changement d'état On/Off est notifié en console.

On/Off Split:	Active ou Desactive (verification en console) le split d'une face au clic.
			Attention: bien cliquer sur une face en affichage 'full' pour ne pas casser le maillage (utilisation du zBuffer).
+ConvexHull: 	Rajoute des points et étend la convexhull (non delaunay)
Load .xy File: 	Charge et rajoute les points du fichier test.xy dans la box 2D (non delaunay)
			Je n'ai pas compris comment utiliser terrain.xyz en 2D, j'ai donc créé mon propre fichier.
Delauniser: 	Rend le maillage actuel delaunay avec l'algorithme de Lawson.
On/Off IncrLawson: Active l'algorithme incremental de Lawson.
			c.a.d. -Si le split est actif: le point sera rajouté avec cet algorithme et rendra le maillage delaunay
	       		       -Si on charge le fichier test.xy (Load .xy File), il sera chargé par insertion incrémentale avec l'algorithme de Lawson
			       (permet de vérifier que le résultat est le même que Lawson global sur les mêmes données)
			Rappel: Le maillage est censé être déjà delaunay avant application de cet algorithme.






	
